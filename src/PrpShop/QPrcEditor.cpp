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

// Not ready to depend on Qt 5.11+ yet
#define QT_NO_DEPRECATED_WARNINGS

#include "QPrcEditor.h"
#include <QToolBar>
#include <QStatusBar>
#include <QAction>
#include <QVBoxLayout>
#include <QSettings>
#include <QMessageBox>
#include <QCloseEvent>
#include <ResManager/plResManager.h>
#include <Stream/hsRAMStream.h>
#include <PRP/KeyedObject/hsKeyedObject.h>
#include "QPlasma.h"
#include "Main.h"

QPrcEditor::QPrcEditor(plCreatable* pCre, QWidget* parent)
    : QCreatable(pCre, kPRC_Type | pCre->ClassIndex(), parent),
      fLexersInited(false)
{
    fEditor = new QPlasmaTextEdit(this);
    fEditor->setIndentationMode(SyntaxTextEdit::IndentTabs);

    QToolBar* tbar = new QToolBar(this);
    tbar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    fSaveAction = tbar->addAction(qStdIcon("document-save"),
            tr("Compile and &Save"), this, &QPrcEditor::compilePrc);

    fStatusBar = new QStatusBar(this);
    fStatusBar->setSizeGripEnabled(true);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(tbar);
    layout->addWidget(fEditor);
    layout->addWidget(fStatusBar);
    setLayout(layout);

    // Initialize the editor settings, fonts, etc
    updateSettings();

    connect(fEditor->document(), &QTextDocument::modificationChanged,
            this, &QPrcEditor::onModificationChanged);
    connect(fEditor, &QPlainTextEdit::selectionChanged, this, &QPrcEditor::statusChanged);
    connect(fEditor, &QPlainTextEdit::textChanged, this, &QPrcEditor::statusChanged);
    connect(fEditor, &QPlainTextEdit::undoAvailable, [this](bool) { statusChanged(); });
    connect(fEditor, &QPlainTextEdit::redoAvailable, [this](bool) { statusChanged(); });
    connect(fEditor, &QPlainTextEdit::cursorPositionChanged,
            this, &QPrcEditor::showCursorPosition);

    loadPrcData();
}

QSize QPrcEditor::sizeHint() const
{
    QSettings settings("PlasmaShop", "PlasmaShop");
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    // Qt6 changed the values for this enum...
    QFont::Weight weight = static_cast<QFont::Weight>(settings.value("SciFontWeight-qt6", QFont::Normal).toInt());
#else
    int weight = settings.value("SciFontWeight", QFont::Normal).toInt();
#endif

    QFont textFont(settings.value("SciFont", PLAT_FONT).toString(),
                   settings.value("SciFontSize", 10).toInt(), weight,
                   settings.value("SciFontItalic", false).toBool());

    QFontMetrics fm(textFont);
    return QSize(fm.boundingRect(QString(82, QChar('_'))).width(),
                 fm.height() * 25 + 50);
}

void QPrcEditor::loadPrcData()
{
    hsRAMStream* S = new hsRAMStream();
    pfPrcHelper* prc = new pfPrcHelper(S);
    fCreatable->prcWrite(prc);

    S->rewind();
    size_t dataSize = S->size() - S->pos();
    unsigned char* buf = new unsigned char[dataSize];
    S->read(dataSize, buf);

    QString data = QString::fromUtf8((const char*)buf, dataSize);
    delete[] buf;

    fEditor->setPlainText(data);
}

void QPrcEditor::updateSettings()
{
    QSettings settings("PlasmaShop", "PlasmaShop");
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    // Qt6 changed the values for this enum...
    QFont::Weight weight = static_cast<QFont::Weight>(settings.value("SciFontWeight-qt6", QFont::Normal).toInt());
#else
    int weight = settings.value("SciFontWeight", QFont::Normal).toInt();
#endif
    QFont textFont(settings.value("SciFont", PLAT_FONT).toString(),
                   settings.value("SciFontSize", 10).toInt(), weight,
                   settings.value("SciFontItalic", false).toBool());

    fEditor->setFont(textFont);
    fEditor->setSyntax("XML");
    fEditor->setWordWrapMode(QTextOption::NoWrap);

    fEditor->setTabWidth(settings.value("SciTabWidth", 4).toInt());
    fEditor->setAutoIndent(settings.value("SciAutoIndent", true).toBool());

    fEditor->setShowLineNumbers(settings.value("SciLineNumberMargin", true).toBool());
    fEditor->setShowFolding(settings.value("SciFoldMargin", false).toBool());
}

void QPrcEditor::showCursorPosition()
{
    auto cursor = fEditor->textCursor();
    fStatusBar->showMessage(tr("Line: %1    Column: %2")
                            .arg(cursor.blockNumber() + 1)
                            .arg(cursor.positionInBlock() + 1));
}

void QPrcEditor::onModificationChanged(bool changed)
{
    fDirty = changed;
    fSaveAction->setEnabled(fDirty);
}

void QPrcEditor::compilePrc()
{
    pfPrcParser parser;
    hsRAMStream S;
    QByteArray data = fEditor->toPlainText().toUtf8();
    S.write(data.size(), data.data());
    S.rewind();

    try {
        parser.read(&S);
        fCreatable->prcParse(parser.getRoot(), PrpShopMain::ResManager());
    } catch (hsException& e) {
        // TODO: Get file and line from PRC source where the exception occurred
        QMessageBox::critical(this, tr("Compile Error"),
                QString("Error: %1").arg(e.what()));
        return;
    }

    // If we succeeded, replace the editor contents with the compiled source
    loadPrcData();
}

void QPrcEditor::closeEvent(QCloseEvent* event)
{
    if (fDirty) {
        QMessageBox::StandardButton confirm =
                QMessageBox::question(this, tr("Confirmation"),
                        tr("Are you sure you want to close this PRC object without saving?"),
                        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (confirm == QMessageBox::Save) {
            compilePrc();
            if (fDirty)
                event->ignore();
        } else if (confirm == QMessageBox::Cancel) {
            event->ignore();
        }
    }
}
