/* This file is part of PlasmaShop.
 *
 * PlasmaShop is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PlasmaShop is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PlasmaShop.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "QPlasmaTextDoc.h"
#include <QGridLayout>
#include <QSettings>
#include <QMessageBox>
#include <qticonloader.h>
#include <Stream/plEncryptedStream.h>
#include <Stream/hsElfStream.h>
#include "../QPlasma.h"

#define MARGIN_LINES 0
#define MARGIN_FOLDERS 1

// To preserve settings within a session
static struct {
    QString text, newText;
    bool regex, cs, wo, reverse;
    class TextFindDialog* current;
} s_findSettings = { QString(), QString(), false, false, false, false, NULL };

/* Find/Replace dialog */
TextFindDialog::TextFindDialog(QPlasmaTextDoc* parent, bool replace)
              : QDialog(parent)
{
    s_findSettings.current = this;
    fDocument = parent;
    setAttribute(Qt::WA_DeleteOnClose);
    if (replace) {
        setWindowTitle(tr("Replace..."));
        setWindowIcon(qStdIcon("edit-find-replace"));
    } else {
        setWindowTitle(tr("Find..."));
        setWindowIcon(qStdIcon("edit-find"));
    }

    fFindText = new QLineEdit(s_findSettings.text, this);
    if (replace)
        fNewText = new QLineEdit(s_findSettings.newText, this);
    else
        fNewText = NULL;
    fCaseSensitive = new QCheckBox(tr("&Case sensitive"), this);
    fRegEx = new QCheckBox(tr("&Regular expression search"), this);
    fWholeWord = new QCheckBox(tr("Match &whole word"), this);
    fReverse = new QCheckBox(tr("Search &up"), this);
    fFindText->selectAll();
    fCaseSensitive->setChecked(s_findSettings.cs);
    fRegEx->setChecked(s_findSettings.regex);
    fWholeWord->setChecked(s_findSettings.wo);
    fReverse->setChecked(s_findSettings.reverse);

    QWidget* buttonPanel = new QWidget(this);
    QPushButton* btnFind = new QPushButton(replace ? tr("&Replace") : tr("&Find"), buttonPanel);
    QPushButton* btnReplaceAll = NULL;
    if (replace) {
        btnReplaceAll = new QPushButton(tr("Replace &All"), buttonPanel);
        fBtnSkip = new QPushButton(tr("&Skip"), buttonPanel);
        fBtnSkip->setEnabled(false);
    }
    QPushButton* btnCancel = new QPushButton(tr("&Cancel"), buttonPanel);

    QGridLayout* buttonLayout = new QGridLayout(buttonPanel);
    buttonLayout->setContentsMargins(0, 0, 0, 0);
    buttonLayout->setVerticalSpacing(4);
    int idx = 0;
    buttonLayout->addWidget(btnFind, idx++, 0);
    if (replace) {
        buttonLayout->addWidget(btnReplaceAll, idx++, 0);
        buttonLayout->addWidget(fBtnSkip, idx++, 0);
    }
    buttonLayout->addWidget(btnCancel, idx++, 0);
    buttonLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding), idx, 0);

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->setVerticalSpacing(4);
    layout->setHorizontalSpacing(8);
    idx = 0;
    layout->addWidget(new QLabel(tr("Search string:"), this), idx, 0);
    layout->addWidget(fFindText, idx++, 1);
    if (replace) {
        layout->addWidget(new QLabel(tr("Replace with:"), this), idx, 0);
        layout->addWidget(fNewText, idx++, 1);
    }
    layout->addItem(new QSpacerItem(0, 16, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum), idx++, 0, 1, 2);
    layout->addWidget(fCaseSensitive, idx++, 1);
    layout->addWidget(fRegEx, idx++, 1);
    layout->addWidget(fWholeWord, idx++, 1);
    layout->addWidget(fReverse, idx++, 1);
    layout->addWidget(buttonPanel, 0, 2, idx, 1);

    connect(btnFind, SIGNAL(clicked()), this, SLOT(handleFind()));
    connect(btnCancel, SIGNAL(clicked()), this, SLOT(reject()));
    if (replace) {
        connect(btnReplaceAll, SIGNAL(clicked()), this, SLOT(handleReplaceAll()));
        connect(fBtnSkip, SIGNAL(clicked()), this, SLOT(handleSkip()));
    }

    resize(sizeHint().width() * 1.5, sizeHint().height());
}

TextFindDialog::~TextFindDialog()
{
    s_findSettings.text = fFindText->text();
    if (fNewText != NULL)
        s_findSettings.newText = fNewText->text();
    s_findSettings.regex = fRegEx->isChecked();
    s_findSettings.cs = fCaseSensitive->isChecked();
    s_findSettings.wo = fWholeWord->isChecked();
    s_findSettings.reverse = fReverse->isChecked();
    s_findSettings.current = NULL;
}

void TextFindDialog::handleFind()
{
    if (fNewText != NULL) {
        // Replace
        if (!fBtnSkip->isEnabled()) {
            if (!fDocument->onFind(fFindText->text(), fRegEx->isChecked(),
                                   fCaseSensitive->isChecked(), fWholeWord->isChecked(),
                                   fReverse->isChecked())) {
                QMessageBox::critical(this, tr("Not found"),
                                      tr("The pattern '%1' was not found in the current document")
                                      .arg(fFindText->text()), QMessageBox::Ok);
                accept();
            }
            fBtnSkip->setEnabled(true);
        } else {
            fDocument->onReplace(fNewText->text());
            fDocument->textFindNext();
        }
    } else {
        // Find (and close)
        if (!fDocument->onFind(fFindText->text(), fRegEx->isChecked(),
                               fCaseSensitive->isChecked(), fWholeWord->isChecked(),
                               fReverse->isChecked())) {
            QMessageBox::critical(this, tr("Not found"),
                                  tr("The pattern '%1' was not found in the current document")
                                  .arg(fFindText->text()), QMessageBox::Ok);
        }
        accept();
    }
}

void TextFindDialog::handleSkip()
{
    fDocument->textFindNext();
}

void TextFindDialog::handleReplaceAll()
{
    if (!fDocument->onReplaceAll(fFindText->text(), fRegEx->isChecked(),
                                 fCaseSensitive->isChecked(), fWholeWord->isChecked(),
                                 fNewText->text())) {
        QMessageBox::critical(this, tr("Not found"),
                              tr("The pattern '%1' was not found in the current document")
                              .arg(fFindText->text()), QMessageBox::Ok);
    }
    accept();
}


/* QPlasmaTextDoc */
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
                S->read(1, markerbuf + 2);
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
              : QPlasmaDocument(kDocText, parent),
                fSyntax(kStxNone), fEncoding(kTypeAnsi),
                fLexersInited(false)
{
    memset(fDroidKey, 0, sizeof(fDroidKey));

    fEditor = new QsciScintilla(this);
    fEditor->setEolMode(QsciScintilla::EolWindows); // Because it's what Plasma uses
    fEditor->setUtf8(true);
    fEditor->SendScintilla(QsciScintillaBase::SCI_SETENDATLASTLINE, 0);
    fEditor->SendScintilla(QsciScintillaBase::SCI_SETSCROLLWIDTHTRACKING, 1);
    fEditor->SendScintilla(QsciScintillaBase::SCI_SETSCROLLWIDTH, 1000);

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(4, 4, 4, 4);
    layout->addWidget(fEditor, 0, 0);
    setLayout(layout);

    // Initialize the editor settings, fonts, etc
    updateSettings();

    connect(fEditor, SIGNAL(linesChanged()), this, SLOT(adjustLineNumbers()));
    connect(fEditor, SIGNAL(SCN_SAVEPOINTLEFT()), this, SLOT(makeDirty()));
    connect(fEditor, SIGNAL(SCN_SAVEPOINTREACHED()), this, SLOT(maybeClean()));
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

void QPlasmaTextDoc::updateSettings()
{
    QSettings settings("PlasmaShop", "PlasmaShop");
    QFont textFont(settings.value("SciFont", PLAT_FONT).toString(),
                   settings.value("SciFontSize", 10).toInt(),
                   settings.value("SciFontWeight", QFont::Normal).toInt(),
                   settings.value("SciFontItalic", false).toBool());

    fEditor->setLexer(NULL);
    if (fLexersInited) {
        delete fLexerFNI;
        delete fLexerFX;
        delete fLexerHEX;
        delete fLexerINI;
        delete fLexerPY;
        delete fLexerSDL;
        delete fLexerXML;
    }
    fLexerFNI = new QsciLexerFni(fEditor);
    fLexerFX = new QsciLexerFX(fEditor);
    fLexerHEX = new QsciLexerHexIsle(fEditor);
    fLexerINI = new QsciLexerProperties(fEditor);
    fLexerPY = new QsciLexerPython(fEditor);
    fLexerSDL = new QsciLexerSDL(fEditor);
    fLexerXML = new QsciLexerXML(fEditor);
    fLexersInited = true;

    fLexerFNI->setDefaultFont(textFont);
    fLexerFX->setDefaultFont(textFont);
    fLexerHEX->setDefaultFont(textFont);
    fLexerINI->setDefaultFont(textFont);
    fLexerPY->setDefaultFont(textFont);
    fLexerSDL->setDefaultFont(textFont);
    fLexerXML->setDefaultFont(textFont);

    QFont braceFont = textFont;
    braceFont.setBold(true);
    fEditor->setMatchedBraceFont(braceFont);
    fEditor->setUnmatchedBraceFont(braceFont);

    QPalette pal;
    fEditor->setMarginsBackgroundColor(pal.color(QPalette::Active, QPalette::Window));
    fEditor->setMarginsForegroundColor(pal.color(QPalette::Active, QPalette::WindowText));
    fEditor->setMatchedBraceForegroundColor(QColor(0x00, 0x00, 0xff));
    fEditor->setUnmatchedBraceForegroundColor(QColor(0xff, 0x00, 0x00));
    fEditor->setBraceMatching(QsciScintilla::SloppyBraceMatch);

    fEditor->setTabWidth(settings.value("SciTabWidth", 4).toInt());
    fEditor->setIndentationsUseTabs(!settings.value("SciUseSpaces", true).toBool());
    fEditor->setAutoIndent(settings.value("SciAutoIndent", true).toBool());
    fEditor->setIndentationGuides(settings.value("SciIndentGuides", false).toBool());
    fEditor->setWhitespaceVisibility(settings.value("SciShowWhitespace", false).toBool()
                                     ? QsciScintilla::WsVisible : QsciScintilla::WsInvisible);
    fEditor->setEdgeColor(QColor(0xE0, 0xE0, 0xE0));
    fEditor->setEdgeMode(settings.value("SciLongLineMark", false).toBool()
                         ? QsciScintilla::EdgeLine : QsciScintilla::EdgeNone);
    fEditor->setEdgeColumn(settings.value("SciLongLineSize", 80).toInt());

    // Margin Magic (tm)
    fEditor->setMarginWidth(MARGIN_LINES, 0);
    fEditor->setMarginWidth(MARGIN_FOLDERS, 0);
    if (settings.value("SciMargin", true).toBool()) {
        fDoLineNumbers = settings.value("SciLineNumberMargin", true).toBool();
        if (settings.value("SciFoldMargin", true).toBool())
            fEditor->setFolding(QsciScintilla::BoxedTreeFoldStyle, MARGIN_FOLDERS);
        fEditor->setMarginLineNumbers(MARGIN_LINES, fDoLineNumbers);
        adjustLineNumbers();
        if (!fDoLineNumbers)
            fEditor->setMarginWidth(MARGIN_LINES, 16);
    } else {
        fDoLineNumbers = false;
        fEditor->setMarginLineNumbers(MARGIN_LINES, false);
    }

    setSyntax(fSyntax);
    fEditor->setMarginsFont(textFont);
    adjustLineNumbers();
}

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

void QPlasmaTextDoc::expandAll()
{ fEditor->setFoldAll(false); }

void QPlasmaTextDoc::collapseAll()
{ fEditor->setFoldAll(true); }

void QPlasmaTextDoc::textFind()
{
    if (s_findSettings.current != NULL) {
        s_findSettings.current->raise();
        return;
    }

    TextFindDialog* dlg = new TextFindDialog(this, false);
    dlg->show();
    dlg->raise();
    dlg->activateWindow();
}

void QPlasmaTextDoc::textFindNext()
{
    fEditor->findNext();
}

void QPlasmaTextDoc::textReplace()
{
    if (s_findSettings.current != NULL) {
        s_findSettings.current->raise();
        return;
    }

    TextFindDialog* dlg = new TextFindDialog(this, true);
    dlg->show();
    dlg->raise();
    dlg->activateWindow();
}

bool QPlasmaTextDoc::onFind(QString text, bool regex, bool cs, bool wo, bool reverse)
{
    if (fEditor->length() == 0)
        return false;
    return fEditor->findFirst(text, regex, cs, wo, true, !reverse);
}

void QPlasmaTextDoc::onReplace(QString newText)
{
    fEditor->replace(newText);
}

bool QPlasmaTextDoc::onReplaceAll(QString text, bool regex, bool cs, bool wo, QString newText)
{
    if (fEditor->length() == 0)
        return false;

    int ln, idx;
    fEditor->getCursorPosition(&ln, &idx);
    if (!fEditor->findFirst(text, regex, cs, wo, false, true, 0, 0))
        return false;
    do {
        fEditor->replace(newText);
    } while (fEditor->findNext());
    fEditor->setCursorPosition(ln, idx);
    return true;
}

bool QPlasmaTextDoc::loadFile(QString filename)
{
    if (filename.right(4).toLower() == ".elf") {
        // Encrypted Log File...  We have to handle it specially
        hsElfStream S;
        S.open(filename.toUtf8().data(), fmRead);
        fEditor->clear();
        while (!S.eof())
            fEditor->append(~S.readLine() + "\n");
        fEditor->setReadOnly(true);
    } else if (plEncryptedStream::IsFileEncrypted(filename.toUtf8().data())) {
        plEncryptedStream S(PlasmaVer::pvUnknown);
        S.open(filename.toUtf8().data(), fmRead, plEncryptedStream::kEncAuto);
        if (S.getEncType() == plEncryptedStream::kEncDroid) {
            if (!GetEncryptionKeyFromUser(this, fDroidKey))
                return false;
            S.setKey(fDroidKey);
            fEncryption = kEncDroid;
        } else if (S.getEncType() == plEncryptedStream::kEncXtea) {
            fEncryption = kEncXtea;
        } else if (S.getEncType() == plEncryptedStream::kEncAES) {
            fEncryption = kEncAes;
        }
        fEncoding = DetectEncoding(&S);
        fEditor->setText(LoadData(&S, fEncoding));
    } else {
        hsFileStream S(PlasmaVer::pvUnknown);
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
        hsFileStream S(PlasmaVer::pvUnknown);
        S.open(filename.toUtf8().data(), fmCreate);
        WriteEncoding(&S, fEncoding);
        SaveData(&S, fEncoding, fEditor->text());
    } else {
        plEncryptedStream S(PlasmaVer::pvUnknown);
        plEncryptedStream::EncryptionType type = plEncryptedStream::kEncNone;
        if (fEncryption == kEncDroid) {
            if ((fFilename != filename) || isZeroKey(fDroidKey)) {
                if (!GetEncryptionKeyFromUser(this, fDroidKey))
                    return false;
            }
            S.setKey(fDroidKey);
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
    fEditor->setReadOnly(false);
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
        fEditor->setLexer(NULL);
        {
            QSettings settings("PlasmaShop", "PlasmaShop");
            QFont textFont(settings.value("SciFont", PLAT_FONT).toString(),
                           settings.value("SciFontSize", 10).toInt(),
                           settings.value("SciFontWeight", QFont::Normal).toInt(),
                           settings.value("SciFontItalic", false).toBool());
            fEditor->setFont(textFont);
        }
    }
}

void QPlasmaTextDoc::setEncoding(EncodingMode type)
{
    fEncoding = type;
    makeDirty();
    fPersistDirty = true;
}

QPlasmaTextDoc::SyntaxMode QPlasmaTextDoc::syntax() const
{ return fSyntax; }

QPlasmaTextDoc::EncodingMode QPlasmaTextDoc::encoding() const
{ return fEncoding; }

void QPlasmaTextDoc::adjustLineNumbers()
{
    if (fDoLineNumbers)
        fEditor->setMarginWidth(MARGIN_LINES, QString(" %1").arg(fEditor->lines()));
}

QPlasmaTextDoc::SyntaxMode QPlasmaTextDoc::GuessIniType()
{
    for (int ln = 0; ln < fEditor->lines(); ln++) {
        if (fEditor->text(ln).startsWith('['))
            return kStxIni;
    }
    return kStxConsole;
}
