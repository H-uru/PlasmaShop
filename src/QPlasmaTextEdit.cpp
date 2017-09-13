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

#include "QPlasmaTextEdit.h"

#include <QTextBlock>
#include <QPainter>
#include <KSyntaxHighlighting/Theme>
#include <KSyntaxHighlighting/Definition>
#include <KSyntaxHighlighting/Repository>
#include <KSyntaxHighlighting/SyntaxHighlighter>
#include <cmath>

class LineNumberMargin : public QWidget {
public:
    LineNumberMargin(QPlasmaTextEdit *editor)
        : QWidget(editor), fEditor(editor) { }

    QSize sizeHint() const Q_DECL_OVERRIDE
    {
        return QSize(fEditor->lineMarginWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent* e) Q_DECL_OVERRIDE
    {
        fEditor->paintLineNumbers(e);
    }

private:
    QPlasmaTextEdit* fEditor;
};


class WhitespaceKSyntaxHighlighter : public KSyntaxHighlighting::SyntaxHighlighter
{
public:
    WhitespaceKSyntaxHighlighter(QTextDocument* document)
        : KSyntaxHighlighting::SyntaxHighlighter(document)
    { }

    void highlightBlock(const QString& text) override
    {
        KSyntaxHighlighting::SyntaxHighlighter::highlightBlock(text);

        QRegExp ws_regex("\\s+");
        int index = ws_regex.indexIn(text);
        QTextCharFormat ws_format;
        ws_format.setForeground(QBrush(theme().editorColor(KSyntaxHighlighting::Theme::TabMarker)));
        while (index >= 0) {
            int length = ws_regex.matchedLength();
            setFormat(index, length, ws_format);
            index = ws_regex.indexIn(text, index + length);
        }
    }
};


KSyntaxHighlighting::Repository* QPlasmaTextEdit::SyntaxRepo()
{
    static KSyntaxHighlighting::Repository s_syntaxRepo;
    static bool s_firstInit = true;
    if (s_firstInit) {
        s_firstInit = false;
#ifndef KSYN_NO_CUSTOM_RULES_SUPPORT
        s_syntaxRepo.addCustomSearchPath(":/plasma-syntax");
#endif
    }
    return &s_syntaxRepo;
}

QPlasmaTextEdit::QPlasmaTextEdit(QWidget* parent)
    : QPlainTextEdit(parent), fTabCharSize(8), fLongLineMarker(), fEditorSettings()
{
    fLineMargin = new LineNumberMargin(this);
    fHighlighter = new WhitespaceKSyntaxHighlighter(document());

    connect(this, &QPlainTextEdit::blockCountChanged, this, &QPlasmaTextEdit::updateMargins);
    connect(this, &QPlainTextEdit::updateRequest, this, &QPlasmaTextEdit::updateLineNumbers);
    connect(this, &QPlainTextEdit::cursorPositionChanged, this, &QPlasmaTextEdit::updateCursor);

    updateMargins();
    highlightCurrentLine();

    setTheme((palette().color(QPalette::Base).lightness() < 128)
             ? SyntaxRepo()->defaultTheme(KSyntaxHighlighting::Repository::DarkTheme)
             : SyntaxRepo()->defaultTheme(KSyntaxHighlighting::Repository::LightTheme));
}

bool QPlasmaTextEdit::haveSelection() const
{
    QTextCursor cursor = textCursor();
    return cursor.anchor() != cursor.position();
}

int QPlasmaTextEdit::lineMarginWidth()
{
    if (!showLineNumbers())
        return 0;

    int digits = 1;
    int maxLine = qMax(1, blockCount());
    while (maxLine >= 10) {
        maxLine /= 10;
        ++digits;
    }
    return fontMetrics().width(QString(digits + 1, '0')) + 2;
}

void QPlasmaTextEdit::paintLineNumbers(QPaintEvent* e)
{
    if (!showLineNumbers())
        return;

    QPainter painter(fLineMargin);
    painter.fillRect(e->rect(), fLineMarginBg);

    QTextBlock block = firstVisibleBlock();
    qreal top = blockBoundingGeometry(block).translated(contentOffset()).top();
    qreal bottom = top + blockBoundingRect(block).height();
    const int offset = fontMetrics().width(QLatin1Char('0')) / 2;
    QTextCursor cursor = textCursor();

    while (block.isValid() && top <= e->rect().bottom()) {
        if (block.isVisible() && bottom >= e->rect().top()) {
            const QString lineNum = QString::number(block.blockNumber() + 1);
            if (block.blockNumber() == cursor.blockNumber())
                painter.setPen(fCursorLineNum);
            else
                painter.setPen(fLineMarginFg);
            painter.drawText(0, (int)std::floor(top), fLineMargin->width() - offset,
                             fontMetrics().height(), Qt::AlignRight, lineNum);
        }

        block = block.next();
        top = bottom;
        bottom = top + blockBoundingRect(block).height();
    }
}

void QPlasmaTextEdit::setShowLineNumbers(bool show)
{
    if (show)
        fEditorSettings |= kShowLineNumbers;
    else
        fEditorSettings &= ~kShowLineNumbers;
    updateMargins();
    fLineMargin->update();
}

void QPlasmaTextEdit::setTabWidth(int width)
{
    fTabCharSize = width;
    const qreal tabWidth = QFontMetricsF(font()).width(QString(width, ' '));
    setTabStopWidth(static_cast<int>(std::ceil(tabWidth)));

    // Ensure characters line up evenly so tab stop calculation is correct
    QFont editFont = font();
    const qreal letterSpacing = (std::ceil(tabWidth) - tabWidth) / width;
    editFont.setLetterSpacing(QFont::AbsoluteSpacing, letterSpacing);
    setFont(editFont);
}

void QPlasmaTextEdit::setExpandTabs(bool expand)
{
    if (expand)
        fEditorSettings |= kExpandTabs;
    else
        fEditorSettings &= ~kExpandTabs;
}

void QPlasmaTextEdit::setAutoIndent(bool ai)
{
    if (ai)
        fEditorSettings |= kAutoIndent;
    else
        fEditorSettings &= ~kAutoIndent;
}

void QPlasmaTextEdit::setLongLineMarker(int pos)
{
    fLongLineMarker = pos;
    viewport()->update();
}

void QPlasmaTextEdit::setMatchParentheses(bool match)
{
    if (match)
        fEditorSettings |= kMatchParens;
    else
        fEditorSettings &= ~kMatchParens;
}

void QPlasmaTextEdit::setTheme(const KSyntaxHighlighting::Theme& theme)
{
    QPalette pal = palette();
    pal.setColor(QPalette::Text, theme.textColor(KSyntaxHighlighting::Theme::Normal));
    pal.setColor(QPalette::Base, theme.editorColor(KSyntaxHighlighting::Theme::BackgroundColor));
    pal.setColor(QPalette::Highlight, theme.editorColor(KSyntaxHighlighting::Theme::TextSelection));
    setPalette(pal);

    fLineMarginFg = theme.editorColor(KSyntaxHighlighting::Theme::LineNumbers);
    fLineMarginBg = theme.editorColor(KSyntaxHighlighting::Theme::IconBorder);
    fCursorLineBg = theme.editorColor(KSyntaxHighlighting::Theme::CurrentLine);
    fCursorLineNum = theme.editorColor(KSyntaxHighlighting::Theme::CurrentLineNumber);
    fLongLineColor = theme.editorColor(KSyntaxHighlighting::Theme::WordWrapMarker);
    fParenMatchBg = theme.editorColor(KSyntaxHighlighting::Theme::BracketMatching);

    fHighlighter->setTheme(theme);
}

void QPlasmaTextEdit::setSyntax(const QString& name)
{
    KSyntaxHighlighting::Definition syntaxDef = SyntaxRepo()->definitionForName(name);
    if (!syntaxDef.isValid() && !name.isEmpty())
        qDebug("Warning: Could not find syntax defintion for %s", name.toUtf8().constData());
    fHighlighter->setDefinition(syntaxDef);
}

void QPlasmaTextEdit::updateMargins()
{
    setViewportMargins(lineMarginWidth(), 0, 0, 0);
}

void QPlasmaTextEdit::updateLineNumbers(const QRect& rect, int dy)
{
    if (dy)
        fLineMargin->scroll(0, dy);
    else
        fLineMargin->update(0, rect.y(), fLineMargin->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateMargins();
}

static int findNextParen(QTextBlock block, int position, QChar left, QChar right)
{
    int balance = 0;
    do {
        QString text = block.text();
        while (position < text.size()) {
            QChar ch = text.at(position);
            if (ch == left) {
                ++balance;
            } else if (ch == right) {
                if (balance == 0)
                    return block.position() + position;
                --balance;
            }
            ++position;
        }

        block = block.next();
        position = 0;
    } while (block.isValid());

    // No match found in the document
    return -1;
}

static int findPrevParen(QTextBlock block, int position, QChar left, QChar right)
{
    int balance = 0;
    do {
        QString text = block.text();
        while (position > 0) {
            --position;
            QChar ch = text.at(position);
            if (ch == right) {
                ++balance;
            } else if (ch == left) {
                if (balance == 0)
                    return block.position() + position;
                --balance;
            }
        }

        block = block.previous();
        position = block.text().size();
    } while (block.isValid());

    // No match found in the document
    return -1;
}

void QPlasmaTextEdit::updateCursor()
{
    highlightCurrentLine();

    if (matchParentheses() && !isReadOnly()) {
        QTextCursor cursor = textCursor();
        cursor.clearSelection();
        const QString blockText = cursor.block().text();
        const int blockPos = cursor.positionInBlock();
        const QChar chPrev = (blockPos > 0)
                             ? blockText[cursor.positionInBlock() - 1]
                             : QLatin1Char(0);
        const QChar chNext = (blockPos < blockText.size())
                             ? blockText[cursor.positionInBlock()]
                             : QLatin1Char(0);
        int match = -1;
        if (chNext == '(') {
            match = findNextParen(cursor.block(), blockPos + 1, '(', ')');
        } else if (chNext == '[') {
            match = findNextParen(cursor.block(), blockPos + 1, '[', ']');
        } else if (chNext == '{') {
            match = findNextParen(cursor.block(), blockPos + 1, '{', '}');
        } else if (chPrev == ')') {
            match = findPrevParen(cursor.block(), blockPos - 1, '(', ')');
            cursor.movePosition(QTextCursor::PreviousCharacter);
        } else if (chPrev == ']') {
            match = findPrevParen(cursor.block(), blockPos - 1, '[', ']');
            cursor.movePosition(QTextCursor::PreviousCharacter);
        } else if (chPrev == '}') {
            match = findPrevParen(cursor.block(), blockPos - 1, '{', '}');
            cursor.movePosition(QTextCursor::PreviousCharacter);
        }

        if (match >= 0) {
            auto selections = extraSelections();
            QTextEdit::ExtraSelection selection;
            selection.format.setBackground(fParenMatchBg);
            selection.cursor = cursor;
            selection.cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
            selections.append(selection);
            selection.cursor = textCursor();
            selection.cursor.setPosition(match);
            selection.cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
            selections.append(selection);
            setExtraSelections(selections);
        }
    }
}

void QPlasmaTextEdit::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;
        selection.format.setBackground(fCursorLineBg);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}

void QPlasmaTextEdit::resizeEvent(QResizeEvent* e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect rect = contentsRect();
    rect.setWidth(lineMarginWidth());
    fLineMargin->setGeometry(rect);
}

void QPlasmaTextEdit::indentSelection()
{
    QTextCursor cursor = textCursor();
    cursor.beginEditBlock();

    const int startPos = cursor.selectionStart();
    cursor.setPosition(cursor.selectionEnd());
    const int endBlock = (cursor.position() == cursor.block().position())
                         ? cursor.blockNumber() - 1 : cursor.blockNumber();
    cursor.setPosition(startPos);
    do {
        int leadingIndent = 0;
        int startOfLine = 0;
        for (const auto ch : cursor.block().text()) {
            if (ch == '\t') {
                leadingIndent += (fTabCharSize - (leadingIndent % fTabCharSize));
                startOfLine += 1;
            } else if (ch == ' ') {
                leadingIndent += 1;
                startOfLine += 1;
            } else {
                break;
            }
        }

        const int spaces = fTabCharSize - (leadingIndent % fTabCharSize);
        cursor.movePosition(QTextCursor::StartOfLine);
        cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::MoveAnchor,
                            startOfLine);
        cursor.insertText(QString(spaces, ' '));

        cursor.movePosition(QTextCursor::NextBlock);
    } while (cursor.blockNumber() <= endBlock);

    cursor.endEditBlock();
}

void QPlasmaTextEdit::outdentSelection()
{
    QTextCursor cursor = textCursor();
    cursor.beginEditBlock();

    const int startPos = cursor.selectionStart();
    cursor.setPosition(cursor.selectionEnd());
    const int endBlock = (cursor.position() == cursor.block().position())
                         ? cursor.blockNumber() - 1 : cursor.blockNumber();
    cursor.setPosition(startPos);
    do {
        int leadingIndent = 0;
        int startOfLine = 0;
        for (const auto ch : cursor.block().text()) {
            if (ch == '\t') {
                leadingIndent += (fTabCharSize - (leadingIndent % fTabCharSize));
                startOfLine += 1;
            } else if (ch == ' ') {
                leadingIndent += 1;
                startOfLine += 1;
            } else {
                break;
            }
        }
        if ((leadingIndent % fTabCharSize) != 0)
            leadingIndent -= (leadingIndent % fTabCharSize);
        else if (leadingIndent > 0)
            leadingIndent -= fTabCharSize;

        cursor.movePosition(QTextCursor::StartOfLine);
        cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor,
                            startOfLine);
        cursor.removeSelectedText();
        if (expandTabs())
            cursor.insertText(QString(leadingIndent, ' '));
        else
            cursor.insertText(QString(leadingIndent / fTabCharSize, '\t'));

        cursor.movePosition(QTextCursor::NextBlock);
    } while (cursor.blockNumber() <= endBlock);

    cursor.endEditBlock();
}

void QPlasmaTextEdit::keyPressEvent(QKeyEvent* e)
{
    switch (e->key()) {
    case Qt::Key_Tab:
        if (haveSelection()) {
            indentSelection();
        } else if (expandTabs()) {
            QTextCursor cursor = textCursor();
            QString cursorText = cursor.block().text().left(cursor.positionInBlock());
            int vpos = 0;
            for (const auto ch : cursorText) {
                if (ch == '\t')
                    vpos += (fTabCharSize - (vpos % fTabCharSize));
                else
                    vpos += 1;
            }
            const int spaces = fTabCharSize - (vpos % fTabCharSize);
            cursor.insertText(QString(spaces, ' '));
        } else {
            textCursor().insertText(QStringLiteral("\t"));
        }
        e->accept();
        break;

    case Qt::Key_Backtab:
        outdentSelection();
        e->accept();
        break;

    case Qt::Key_Return:
    case Qt::Key_Enter:
        {
            QTextCursor undoCursor = textCursor();
            undoCursor.beginEditBlock();

            // Don't allow QPlainTextEdit to insert a soft break :(
            QKeyEvent retnEvent(e->type(), e->key(),
                                e->modifiers() & ~Qt::ShiftModifier,
                                e->nativeScanCode(), e->nativeVirtualKey(),
                                e->nativeModifiers(), e->text(),
                                e->isAutoRepeat(), e->count());
            QPlainTextEdit::keyPressEvent(&retnEvent);

            // Simple auto-indent: Just copy the previous non-empty line's
            // leading whitespace
            if ((e->modifiers() & Qt::ShiftModifier) == 0 && autoIndent()) {
                QTextCursor scanCursor = textCursor();
                int startOfLine = 0;
                while (scanCursor.blockNumber() > 0 && startOfLine == 0) {
                    scanCursor.movePosition(QTextCursor::PreviousBlock);

                    const QString blockText = scanCursor.block().text();
                    for (const auto ch : blockText) {
                        if (ch == '\t' || ch == ' ')
                            startOfLine += 1;
                        else
                            break;
                    }
                    if (startOfLine == 0 && !blockText.isEmpty()) {
                        // No leading whitespace, but line is not empty.
                        // Therefore, current leading indent level is 0.
                        break;
                    }
                }
                if (startOfLine != 0) {
                    QString leadingIndent = scanCursor.block().text().left(startOfLine);
                    textCursor().insertText(leadingIndent);
                }
            }
            undoCursor.endEditBlock();
        }
        break;

    default:
        QPlainTextEdit::keyPressEvent(e);
        break;
    }

    updateCursor();
}

void QPlasmaTextEdit::paintEvent(QPaintEvent* e)
{
    if (fLongLineMarker > 0) {
        QFontMetricsF fm(currentCharFormat().font());
        const int longLinePos = std::round(fm.averageCharWidth() * fLongLineMarker)
                                + contentOffset().x() + document()->documentMargin();
        QPainter p(viewport());
        p.setPen(fLongLineColor);
        p.drawLine(longLinePos, e->rect().top(), longLinePos, e->rect().bottom());
    }

    QPlainTextEdit::paintEvent(e);
}
