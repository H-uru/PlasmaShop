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
#include <QTextBlock>
#include <Stream/plEncryptedStream.h>
#include <Stream/hsElfStream.h>
#include "QPlasma.h"

// To preserve settings within a session
static struct
{
    QString text, newText;
    bool regex, cs, wo, reverse;
    class TextFindDialog* current;
} s_findSettings = { QString(), QString(), false, false, false, false, Q_NULLPTR };

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
        fNewText = Q_NULLPTR;
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
    QPushButton* btnReplaceAll = Q_NULLPTR;
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

    connect(btnFind, &QPushButton::clicked, this, &TextFindDialog::handleFind);
    connect(btnCancel, &QPushButton::clicked, this, &QDialog::reject);
    if (replace) {
        connect(btnReplaceAll, &QPushButton::clicked, this, &TextFindDialog::handleReplaceAll);
        connect(fBtnSkip, &QPushButton::clicked, this, [this] { performSearch(); });
    }

    resize(sizeHint().width() * 1.5, sizeHint().height());
}

TextFindDialog::~TextFindDialog()
{
    s_findSettings.text = fFindText->text();
    if (fNewText != Q_NULLPTR)
        s_findSettings.newText = fNewText->text();
    s_findSettings.regex = fRegEx->isChecked();
    s_findSettings.cs = fCaseSensitive->isChecked();
    s_findSettings.wo = fWholeWord->isChecked();
    s_findSettings.reverse = fReverse->isChecked();
    s_findSettings.current = NULL;
}

bool TextFindDialog::performSearch()
{
    s_findSettings.text = fFindText->text();
    if (fNewText != Q_NULLPTR)
        s_findSettings.newText = fNewText->text();
    s_findSettings.regex = fRegEx->isChecked();
    s_findSettings.cs = fCaseSensitive->isChecked();
    s_findSettings.wo = fWholeWord->isChecked();
    s_findSettings.reverse = fReverse->isChecked();
    return fDocument->onFindNext();
}

void TextFindDialog::handleFind()
{
    if (fNewText != Q_NULLPTR) {
        // Replace
        if (!fBtnSkip->isEnabled()) {
            if (!performSearch()) {
                QMessageBox::critical(this, tr("Not found"),
                                      tr("The pattern '%1' was not found in the current document")
                                      .arg(fFindText->text()), QMessageBox::Ok);
                accept();
            }
            fBtnSkip->setEnabled(true);
        } else {
            fDocument->onReplace(fNewText->text());
            performSearch();
        }
    } else {
        // Find (and close)
        if (!performSearch()) {
            QMessageBox::critical(this, tr("Not found"),
                                  tr("The pattern '%1' was not found in the current document")
                                  .arg(fFindText->text()), QMessageBox::Ok);
        }
        accept();
    }
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
        data = QString::fromLatin1((const char*)buf, dataSize);
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
            QByteArray buf = data.toLatin1();
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
    : QPlasmaDocument(kDocText, parent), fSyntax(kStxNone), fEncoding(kTypeAnsi)
{
    memset(fDroidKey, 0, sizeof(fDroidKey));

    fEditor = new QPlasmaTextEdit(this);

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(4, 4, 4, 4);
    layout->addWidget(fEditor, 0, 0);
    setLayout(layout);

    // Initialize the editor settings, fonts, etc
    updateSettings();

    connect(fEditor->document(), &QTextDocument::modificationChanged, [this](bool changed) {
        if (changed)
            makeDirty();
        else
            maybeClean();
    });
    connect(fEditor, &QPlainTextEdit::selectionChanged, this, &QPlasmaTextDoc::statusChanged);
    connect(fEditor, &QPlainTextEdit::textChanged, this, &QPlasmaTextDoc::statusChanged);
    connect(fEditor, &QPlainTextEdit::undoAvailable, [this](bool) { statusChanged(); });
    connect(fEditor, &QPlainTextEdit::redoAvailable, [this](bool) { statusChanged(); });
}

void QPlasmaTextDoc::updateSettings()
{
    QSettings settings("PlasmaShop", "PlasmaShop");
    QFont textFont(settings.value("SciFont", PLAT_FONT).toString(),
                   settings.value("SciFontSize", 10).toInt(),
                   settings.value("SciFontWeight", QFont::Normal).toInt(),
                   settings.value("SciFontItalic", false).toBool());

    fEditor->setFont(textFont);
    fEditor->setWordWrapMode(QTextOption::NoWrap);
    fEditor->setMatchBraces(true);

    fEditor->setTabWidth(settings.value("SciTabWidth", 4).toInt());
    fEditor->setIndentationMode(settings.value("SciUseSpaces", true).toBool()
                ? SyntaxTextEdit::IndentSpaces : SyntaxTextEdit::IndentTabs);
    fEditor->setAutoIndent(settings.value("SciAutoIndent", true).toBool());

    fEditor->setShowLineNumbers(settings.value("SciLineNumberMargin", true).toBool());
    fEditor->setShowFolding(settings.value("SciFoldMargin", false).toBool());

    QTextOption opt = fEditor->document()->defaultTextOption();
    if (settings.value("SciShowWhitespace", false).toBool())
        opt.setFlags(opt.flags() | QTextOption::ShowTabsAndSpaces);
    else
        opt.setFlags(opt.flags() & ~QTextOption::ShowTabsAndSpaces);
    opt.setFlags(opt.flags() | QTextOption::AddSpaceForLineAndParagraphSeparators);
    fEditor->document()->setDefaultTextOption(opt);

    fEditor->setShowLongLineEdge(settings.value("SciLongLineMark", false).toBool());
    fEditor->setLongLineWidth(settings.value("SciLongLineSize", 80).toInt());
    fEditor->setShowIndentGuides(settings.value("SciIndentGuides", false).toBool());
}

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

bool QPlasmaTextDoc::onFind(const QString& text, bool regex, bool cs,
                            bool wo, bool reverse)
{
    QTextDocument::FindFlags options;
    if (cs)
        options |= QTextDocument::FindCaseSensitively;
    if (wo)
        options |= QTextDocument::FindWholeWords;
    if (reverse)
        options |= QTextDocument::FindBackward;

    if (regex)
        return fEditor->find(QRegExp(text, cs ? Qt::CaseSensitive : Qt::CaseInsensitive), options);
    else
        return fEditor->find(text, options);
}

bool QPlasmaTextDoc::onFindNext()
{
    return onFind(s_findSettings.text, s_findSettings.regex,
                  s_findSettings.cs, s_findSettings.wo,
                  s_findSettings.reverse);
}

void QPlasmaTextDoc::onReplace(const QString& newText)
{
    QTextCursor cursor = fEditor->textCursor();
    cursor.removeSelectedText();
    cursor.insertText(newText);
}

bool QPlasmaTextDoc::onReplaceAll(const QString& text, bool regex, bool cs,
                                  bool wo, const QString& newText)
{
    QTextCursor cursorSave = fEditor->textCursor();

    fEditor->moveCursor(QTextCursor::Start);
    if (!onFind(text, regex, cs, wo, false)) {
        fEditor->setTextCursor(cursorSave);
        return false;
    }

    do {
        onReplace(newText);
    } while (onFind(text, regex, cs, wo, false));

    fEditor->setTextCursor(cursorSave);
    return true;
}

bool QPlasmaTextDoc::loadFile(const QString& filename)
{
    const ST::string stFilename = qstr2st(filename);
    if (filename.right(4).toLower() == ".elf") {
        // Encrypted Log File...  We have to handle it specially
        hsElfStream S;
        if (!S.open(stFilename, fmRead))
            return false;
        QString logAccum;
        logAccum.reserve(static_cast<int>(S.size()));
        while (!S.eof())
            logAccum.append(st2qstr(S.readLine() + '\n'));
        fEditor->setPlainText(logAccum);
        fEditor->setReadOnly(true);
    } else if (plEncryptedStream::IsFileEncrypted(stFilename)) {
        plEncryptedStream S(PlasmaVer::pvUnknown);
        if (!S.open(stFilename, fmRead, plEncryptedStream::kEncAuto))
            return false;
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
        fEditor->setPlainText(LoadData(&S, fEncoding));
    } else {
        hsFileStream S(PlasmaVer::pvUnknown);
        if (!S.open(stFilename, fmRead))
            return false;
        fEncryption = kEncNone;
        fEncoding = DetectEncoding(&S);
        fEditor->setPlainText(LoadData(&S, fEncoding));
    }

    fEditor->document()->clearUndoRedoStacks();
    return QPlasmaDocument::loadFile(filename);
}

static QString unixToWindowsText(QString &&text)
{
    return text.replace("\n", "\r\n").replace("\r\r\n", "\r\n");
}

bool QPlasmaTextDoc::saveTo(const QString& filename)
{
    const ST::string stFilename = qstr2st(filename);
    if (fEncryption == kEncNone) {
        hsFileStream S(PlasmaVer::pvUnknown);
        if (!S.open(stFilename, fmCreate))
            return false;
        WriteEncoding(&S, fEncoding);
        SaveData(&S, fEncoding, unixToWindowsText(fEditor->toPlainText()));
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
        if (!S.open(stFilename, fmCreate, type))
            return false;
        WriteEncoding(&S, fEncoding);
        SaveData(&S, fEncoding, unixToWindowsText(fEditor->toPlainText()));
    }

    fEditor->setReadOnly(false);
    return QPlasmaDocument::saveTo(filename);
}

void QPlasmaTextDoc::setSyntax(SyntaxMode syn)
{
    fSyntax = syn;
    switch (fSyntax) {
    case kStxConsole:
        fEditor->setSyntax("Plasma Console");
        break;
    case kStxFX:
        fEditor->setSyntax("HLSL");
        break;
    case kStxHex:
        fEditor->setSyntax("Hex Isle Level");
        break;
    case kStxIni:
        fEditor->setSyntax("INI Files");
        break;
    case kStxPython:
        fEditor->setSyntax("Python");
        break;
    case kStxSDL:
        fEditor->setSyntax("Plasma SDL");
        break;
    case kStxXML:
        fEditor->setSyntax("XML");
        break;
    default:
        fEditor->setSyntax("");
        break;
    }
}

void QPlasmaTextDoc::setEncoding(EncodingMode type)
{
    fEncoding = type;
    makeDirty();
    fPersistDirty = true;
}

QPlasmaTextDoc::SyntaxMode QPlasmaTextDoc::GuessIniType()
{
    QTextBlock block = fEditor->document()->firstBlock();
    while (block.isValid()) {
        if (block.text().startsWith('['))
            return kStxIni;
        block = block.next();
    }
    return kStxConsole;
}
