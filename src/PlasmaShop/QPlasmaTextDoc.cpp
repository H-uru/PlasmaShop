#include "QPlasmaTextDoc.h"
#include <QInputDialog>
#include <QSettings>
#include <QMessageBox>
#include <QGridLayout>
#include <Stream/plEncryptedStream.h>

static QPlasmaTextDoc::EncodingMode DetectEncoding(hsStream* S)
{
    unsigned char markerbuf[4];
    QPlasmaTextDoc::EncodingMode mode = QPlasmaTextDoc::kTypeAnsi;
    if (S->size() >= 2) {
        S->read(2, markerbuf);
        if (markerbuf[0] == 0xFF && markerbuf[1] == 0xFE) {
            if (S->size() >= 4) {
                S->read(2, markerbuf + 2);
                if (markerbuf[2] == 0 && markerbuf[3] == 0) {
                    // UTF32
                    mode = QPlasmaTextDoc::kTypeUTF32;
                } else {
                    // some other UTF16 char, go back to it
                    S->seek(2);
                    mode = QPlasmaTextDoc::kTypeUTF16;
                }
            } else {
                // Not big enough for UTF32, must be UTF16
                mode = QPlasmaTextDoc::kTypeUTF16;
            }
        } else if (markerbuf[0] == 0xEF && markerbuf[1] == 0xBB) {
            if (S->size() >= 3) {
                S->read(1, markerbuf + 1);
                if (markerbuf[2] == 0xBF) {
                    // UTF8
                    mode = QPlasmaTextDoc::kTypeUTF8;
                } else {
                    // Random ANSI junk
                    S->seek(0);
                }
            } else {
                // Random ANSI junk
                S->seek(0);
            }
        } else {
            // Normal ANSI file
            S->seek(0);
        }
    }
    return mode;
}

static QString LoadData(hsStream* S, QPlasmaTextDoc::EncodingMode mode)
{
    size_t dataSize = S->size() - S->pos();
    unsigned char* buf = new unsigned char[dataSize];
    S->read(dataSize, buf);

    QString data;
    switch (mode) {
    case QPlasmaTextDoc::kTypeAnsi:
        data = QString::fromAscii((const char*)buf, dataSize);
        break;
    case QPlasmaTextDoc::kTypeUTF8:
        data = QString::fromUtf8((const char*)buf, dataSize);
        break;
    case QPlasmaTextDoc::kTypeUTF16:
        data = QString::fromUtf16((const ushort*)buf, dataSize / sizeof(ushort));
        break;
    case QPlasmaTextDoc::kTypeUTF32:
        data = QString::fromUcs4((const uint*)buf, dataSize / sizeof(uint));
        break;
    }

    delete[] buf;
    return data;
}

QPlasmaTextDoc::QPlasmaTextDoc(QWidget* parent)
              : QPlasmaDocument(kDocText, parent)
{
    QPalette pal;
    fEditor = new QsciScintilla(this);
    fEditor->setEolMode(QsciScintilla::EolWindows); // Because it's what Plasma uses
    fEditor->setMarginLineNumbers(1, true);
    fEditor->setFolding(QsciScintilla::BoxedTreeFoldStyle, 2);
    fEditor->setUtf8(true);
    fEditor->setTabWidth(4);

    fEditor->setFont(QFont(PLAT_FONT, PLAT_FONTSIZE));
    fEditor->setMarginsFont(QFont(PLAT_FONT, PLAT_FONTSIZE));
    fEditor->setMarginsBackgroundColor(pal.color(QPalette::Active, QPalette::Window));
    fEditor->setMarginsForegroundColor(pal.color(QPalette::Active, QPalette::WindowText));

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(4, 4, 4, 4);
    layout->addWidget(fEditor, 0, 0);
    setLayout(layout);

    fLexerFNI = new QsciLexerFni(fEditor);
    fLexerFX = new QsciLexerFX(fEditor);
    fLexerHEX = new QsciLexerHexIsle(fEditor);
    fLexerINI = new QsciLexerProperties(fEditor);
    fLexerPY = new QsciLexerPython(fEditor);
    fLexerSDL = new QsciLexerSDL(fEditor);
    fLexerXML = new QsciLexerXML(fEditor);

    fLexerFNI->setDefaultFont(QFont(PLAT_FONT, PLAT_FONTSIZE));
    fLexerFX->setDefaultFont(QFont(PLAT_FONT, PLAT_FONTSIZE));
    fLexerHEX->setDefaultFont(QFont(PLAT_FONT, PLAT_FONTSIZE));
    fLexerINI->setDefaultFont(QFont(PLAT_FONT, PLAT_FONTSIZE));
    fLexerPY->setDefaultFont(QFont(PLAT_FONT, PLAT_FONTSIZE));
    fLexerSDL->setDefaultFont(QFont(PLAT_FONT, PLAT_FONTSIZE));
    fLexerXML->setDefaultFont(QFont(PLAT_FONT, PLAT_FONTSIZE));

    connect(fEditor, SIGNAL(linesChanged()), this, SLOT(adjustLineNumbers()));
    connect(fEditor, SIGNAL(SCN_SAVEPOINTLEFT()), this, SIGNAL(becameDirty()));
    connect(fEditor, SIGNAL(SCN_SAVEPOINTREACHED()), this, SIGNAL(becameClean()));
}

void QPlasmaTextDoc::loadFile(QString filename)
{
    if (plEncryptedStream::IsFileEncrypted(filename.toUtf8().data())) {
        plEncryptedStream S(pvUnknown);
        S.open(filename.toUtf8().data(), fmRead, plEncryptedStream::kEncAuto);
        if (S.getEncType() == plEncryptedStream::kEncDroid) {
            // Acquire the NTD key from the user
            QSettings settings("PlasmaShop", "PlasmaShop");
            QString keyStr = settings.value("NtdKey", "00000000000000000000000000000000").toString();
            bool dlgAccept, validKey = false;
            while (!validKey) {
                keyStr = QInputDialog::getText(this, tr("Enter NTD Key"),
                                               tr("Enter your MOUL/MQO Encryption key"),
                                               QLineEdit::Normal, keyStr, &dlgAccept);
                if (!dlgAccept) {
                    fFilename = "<>";
                    return;
                }
                if (keyStr.length() == 32) {
                    validKey = true;
                    for (int i=0; i<keyStr.length() && validKey; ++i) {
                        QChar ch = keyStr.at(i);
                        if (ch.isDigit())
                            continue;
                        if (ch >= 'A' && ch <= 'F')
                            continue;
                        if (ch >= 'a' && ch <= 'f')
                            continue;
                        validKey = false;
                    }
                }
                if (!validKey) {
                    QMessageBox::warning(this, tr("Invalid Key"),
                                         tr("You have entered an invalid encryption key.\n"
                                            "Encryption keys should be 32 hex digits, with no\n"
                                            "spaces or dashes"),
                                         QMessageBox::Ok);
                } else {
                    unsigned int key[4];
                    key[0] = keyStr.mid(0, 8).toUInt(0, 16);
                    key[1] = keyStr.mid(8, 8).toUInt(0, 16);
                    key[2] = keyStr.mid(16, 8).toUInt(0, 16);
                    key[3] = keyStr.mid(24, 8).toUInt(0, 16);
                    S.setKey(key);
                    settings.setValue("NtdKey", keyStr);
                }
            }
            fEncryption = kEncDroid;
        } else if (S.getEncType() == plEncryptedStream::kEncXtea) {
            fEncryption = kEncXtea;
        } else if (S.getEncType() == plEncryptedStream::kEncAES) {
            fEncryption = kEncAes;
        }
        fEncoding = DetectEncoding(&S);
        fEditor->setText(LoadData(&S, fEncoding));
    } else {
        hsFileStream S(pvUnknown);
        S.open(filename.toUtf8().data(), fmRead);
        fEncryption = kEncNone;
        fEncoding = DetectEncoding(&S);
        fEditor->setText(LoadData(&S, fEncoding));
    }
    QPlasmaDocument::loadFile(filename);
    fEditor->SendScintilla(QsciScintillaBase::SCI_SETSAVEPOINT);
}

void QPlasmaTextDoc::saveTo(QString filename)
{
    QPlasmaDocument::saveTo(filename);
    fEditor->SendScintilla(QsciScintillaBase::SCI_SETSAVEPOINT);
}

void QPlasmaTextDoc::setSyntax(SyntaxMode syn)
{
    fSyntax = syn;
    switch (fSyntax) {
    case kStxConsole:
        fEditor->setLexer(fLexerFNI);
        break;
    case kStxFX:
        fEditor->setLexer(fLexerFX);
        break;
    case kStxHex:
        fEditor->setLexer(fLexerHEX);
        break;
    case kStxIni:
        fEditor->setLexer(fLexerINI);
        break;
    case kStxPython:
        fEditor->setLexer(fLexerPY);
        break;
    case kStxSDL:
        fEditor->setLexer(fLexerSDL);
        break;
    case kStxXML:
        fEditor->setLexer(fLexerXML);
        break;
    default:
        fEditor->setLexer(0);
    }
}

void QPlasmaTextDoc::setEncryption(EncryptionMode enc)
{
    fEncryption = enc;
}

void QPlasmaTextDoc::setEncoding(EncodingMode type)
{
    fEncoding = type;
}

QPlasmaTextDoc::SyntaxMode QPlasmaTextDoc::syntax() const
{ return fSyntax; }

QPlasmaTextDoc::EncryptionMode QPlasmaTextDoc::encryption() const
{ return fEncryption; }

QPlasmaTextDoc::EncodingMode QPlasmaTextDoc::encoding() const
{ return fEncoding; }

void QPlasmaTextDoc::adjustLineNumbers()
{
    fEditor->setMarginWidth(1, QString(" %1").arg(fEditor->lines()));
}
