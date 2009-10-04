#include "QPlasmaTextDoc.h"
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

static void WriteEncoding(hsStream* S, QPlasmaTextDoc::EncodingMode type)
{
    unsigned char markerbuf[4];

    switch (type) {
    case QPlasmaTextDoc::kTypeAnsi:
        // No BOM
        break;
    case QPlasmaTextDoc::kTypeUTF8:
        markerbuf[0] = 0xEF;
        markerbuf[1] = 0xBB;
        markerbuf[2] = 0xBF;
        S->write(3, markerbuf);
        break;
    case QPlasmaTextDoc::kTypeUTF16:
        markerbuf[0] = 0xFF;
        markerbuf[1] = 0xFE;
        S->write(2, markerbuf);
        break;
    case QPlasmaTextDoc::kTypeUTF32:
        markerbuf[0] = 0xFF;
        markerbuf[1] = 0xFE;
        markerbuf[2] = 0;
        markerbuf[3] = 0;
        S->write(4, markerbuf);
        break;
    }
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

static void SaveData(hsStream* S, QPlasmaTextDoc::EncodingMode mode,
                     const QString& data)
{
    switch (mode) {
    case QPlasmaTextDoc::kTypeAnsi:
        {
            QByteArray buf = data.toAscii();
            S->write(buf.size(), buf.data());
        }
        break;
    case QPlasmaTextDoc::kTypeUTF8:
        {
            QByteArray buf = data.toUtf8();
            S->write(buf.size(), buf.data());
        }
        break;
    case QPlasmaTextDoc::kTypeUTF16:
        S->write(data.size() * sizeof(ushort), data.utf16());
        break;
    case QPlasmaTextDoc::kTypeUTF32:
        {
            QVector<uint> buf = data.toUcs4();
            S->write(buf.size() * sizeof(uint), buf.data());
        }
        break;
    }
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
    connect(fEditor, SIGNAL(SCN_SAVEPOINTLEFT()), this, SLOT(makeDirty()));
    connect(fEditor, SIGNAL(SCN_SAVEPOINTREACHED()), this, SLOT(makeClean()));
    connect(fEditor, SIGNAL(selectionChanged()), this, SIGNAL(statusChanged()));
    connect(fEditor, SIGNAL(textChanged()), this, SIGNAL(statusChanged()));
}

bool QPlasmaTextDoc::canCut() const
{ return fEditor->hasSelectedText(); }

bool QPlasmaTextDoc::canCopy() const
{ return fEditor->hasSelectedText(); }

bool QPlasmaTextDoc::canPaste() const
{ return fEditor->isPasteAvailable(); }

bool QPlasmaTextDoc::canDelete() const
{ return fEditor->hasSelectedText(); }

bool QPlasmaTextDoc::canSelectAll() const
{ return true; }

bool QPlasmaTextDoc::canUndo() const
{ return fEditor->isUndoAvailable(); }

bool QPlasmaTextDoc::canRedo() const
{ return fEditor->isRedoAvailable(); }

void QPlasmaTextDoc::performCut()
{ fEditor->cut(); }

void QPlasmaTextDoc::performCopy()
{ fEditor->copy(); }

void QPlasmaTextDoc::performPaste()
{ fEditor->paste(); }

void QPlasmaTextDoc::performDelete()
{ fEditor->removeSelectedText(); }

void QPlasmaTextDoc::performSelectAll()
{ fEditor->selectAll(true); }

void QPlasmaTextDoc::performUndo()
{ fEditor->undo(); }

void QPlasmaTextDoc::performRedo()
{ fEditor->redo(); }

bool QPlasmaTextDoc::loadFile(QString filename)
{
    if (plEncryptedStream::IsFileEncrypted(filename.toUtf8().data())) {
        plEncryptedStream S(pvUnknown);
        S.open(filename.toUtf8().data(), fmRead, plEncryptedStream::kEncAuto);
        if (S.getEncType() == plEncryptedStream::kEncDroid) {
            unsigned int key[4];
            if (!GetEncryptionKeyFromUser(this, key))
                return false;
            S.setKey(key);
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

    fEditor->SendScintilla(QsciScintillaBase::SCI_SETSAVEPOINT);
    return QPlasmaDocument::loadFile(filename);
}

bool QPlasmaTextDoc::saveTo(QString filename)
{
    if (fEncryption == kEncNone) {
        hsFileStream S(pvUnknown);
        S.open(filename.toUtf8().data(), fmCreate);
        WriteEncoding(&S, fEncoding);
        SaveData(&S, fEncoding, fEditor->text());
    } else {
        plEncryptedStream S(pvUnknown);
        plEncryptedStream::EncryptionType type = plEncryptedStream::kEncNone;
        if (fEncryption == kEncDroid) {
            unsigned int key[4];
            if (!GetEncryptionKeyFromUser(this, key))
                return false;
            S.setKey(key);
            type = plEncryptedStream::kEncDroid;
        } else if (fEncryption == kEncAes) {
            type = plEncryptedStream::kEncAES;
        } else if (fEncryption == kEncXtea) {
            type = plEncryptedStream::kEncXtea;
        }
        S.open(filename.toUtf8().data(), fmCreate, type);
        WriteEncoding(&S, fEncoding);
        SaveData(&S, fEncoding, fEditor->text());
    }

    fEditor->SendScintilla(QsciScintillaBase::SCI_SETSAVEPOINT);
    return QPlasmaDocument::saveTo(filename);
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
