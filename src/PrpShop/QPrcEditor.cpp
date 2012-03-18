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

#include "QPrcEditor.h"
#include <QGridLayout>
#include <QSettings>
#include "../QPlasma.h"

#define MARGIN_LINES 0
#define MARGIN_FOLDERS 1

QPrcEditor::QPrcEditor(plCreatable* pCre, QWidget* parent)
          : QCreatable(pCre, kPRC_Type | pCre->ClassIndex(), parent),
            fLexersInited(false)
{
    fEditor = new QsciScintilla(this);
    fEditor->setUtf8(true);
    fEditor->SendScintilla(QsciScintillaBase::SCI_SETENDATLASTLINE, 0);
    fEditor->SendScintilla(QsciScintillaBase::SCI_SETSCROLLWIDTHTRACKING, 1);
    fEditor->SendScintilla(QsciScintillaBase::SCI_SETSCROLLWIDTH, 1000);

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(fEditor, 0, 0);
    setLayout(layout);

    // Initialize the editor settings, fonts, etc
    updateSettings();

    connect(fEditor, SIGNAL(linesChanged()), this, SLOT(adjustLineNumbers()));
    connect(fEditor, SIGNAL(selectionChanged()), this, SIGNAL(statusChanged()));
    connect(fEditor, SIGNAL(textChanged()), this, SIGNAL(statusChanged()));

    // Load the creatable
    hsRAMStream* S = new hsRAMStream();
    pfPrcHelper* prc = new pfPrcHelper(S);
    fCreatable->prcWrite(prc);

    S->rewind();
    size_t dataSize = S->size() - S->pos();
    unsigned char* buf = new unsigned char[dataSize];
    S->read(dataSize, buf);

    QString data = QString::fromUtf8((const char*)buf, dataSize);
    delete[] buf;

    fEditor->setText(data);
    fEditor->setReadOnly(true);
    fEditor->SendScintilla(QsciScintillaBase::SCI_SETSAVEPOINT);
}

void QPrcEditor::saveDamage()
{
    //TODO
}

void QPrcEditor::updateSettings()
{
    QSettings settings("PlasmaShop", "PlasmaShop");
    QFont textFont(settings.value("SciFont", PLAT_FONT).toString(),
                   settings.value("SciFontSize", 10).toInt(),
                   settings.value("SciFontWeight", QFont::Normal).toInt(),
                   settings.value("SciFontItalic", false).toBool());

    fEditor->setLexer(NULL);
    if (fLexersInited)
        delete fLexerXML;
    fLexerXML = new QsciLexerXML(fEditor);
    fLexersInited = true;

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

    fEditor->setLexer(fLexerXML);
    fEditor->setMarginsFont(textFont);
    adjustLineNumbers();
}

void QPrcEditor::adjustLineNumbers()
{
    if (fDoLineNumbers)
        fEditor->setMarginWidth(MARGIN_LINES, QString(" %1").arg(fEditor->lines()));
}
