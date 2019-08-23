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

#include <QScrollBar>
#include <QTextBlock>
#include <QPainter>
#include <QRegularExpression>
#include <QStack>
#include <KSyntaxHighlighting/Theme>
#include <KSyntaxHighlighting/Definition>
#include <KSyntaxHighlighting/Repository>
#include <KSyntaxHighlighting/SyntaxHighlighter>
#include <cmath>

class LineNumberMargin : public QWidget
{
public:
    explicit LineNumberMargin(QPlasmaTextEdit* editor)
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
    explicit WhitespaceKSyntaxHighlighter(QTextDocument* document)
        : KSyntaxHighlighting::SyntaxHighlighter(document) { }

    void highlightBlock(const QString& text) Q_DECL_OVERRIDE
    {
        KSyntaxHighlighting::SyntaxHighlighter::highlightBlock(text);

        static const QRegularExpression ws_regex("\\s+");
        QRegularExpressionMatchIterator iter = ws_regex.globalMatch(text);
        QTextCharFormat ws_format;
        const QBrush ws_brush(theme().editorColor(KSyntaxHighlighting::Theme::TabMarker));
        ws_format.setForeground(ws_brush);
        while (iter.hasNext()) {
            QRegularExpressionMatch match = iter.next();
            setFormat(match.capturedStart(), match.capturedLength(), ws_format);
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

    setTheme((palette().color(QPalette::Base).lightness() < 128)
             ? SyntaxRepo()->defaultTheme(KSyntaxHighlighting::Repository::DarkTheme)
             : SyntaxRepo()->defaultTheme(KSyntaxHighlighting::Repository::LightTheme));
}

bool QPlasmaTextEdit::haveSelection() const
{
    const QTextCursor cursor = textCursor();
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
    return fontMetrics().boundingRect(QString(digits + 1, '0')).width() + 2;
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
    const QFontMetricsF metrics(font());
    const qreal offset = metrics.width(QLatin1Char('0')) / 2.0;
    QTextCursor cursor = textCursor();

    while (block.isValid() && top <= e->rect().bottom()) {
        if (block.isVisible() && bottom >= e->rect().top()) {
            const QString lineNum = QString::number(block.blockNumber() + 1);
            if (block.blockNumber() == cursor.blockNumber())
                painter.setPen(fCursorLineNum);
            else
                painter.setPen(fLineMarginFg);
            const QRectF numberRect(0, top, fLineMargin->width() - offset, metrics.height());
            painter.drawText(numberRect, Qt::AlignRight, lineNum);
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
    updateTabMetrics();
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

void QPlasmaTextEdit::setMatchBraces(bool match)
{
    if (match)
        fEditorSettings |= kMatchBraces;
    else
        fEditorSettings &= ~kMatchBraces;
    updateCursor();
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
    fBraceMatchBg = theme.editorColor(KSyntaxHighlighting::Theme::BracketMatching);
    fErrorBg = theme.editorColor(KSyntaxHighlighting::Theme::MarkError);

    fHighlighter->setTheme(theme);
    fHighlighter->rehighlight();
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

static bool isQuote(const QChar &ch)
{
    return ch == QLatin1Char('"') || ch == QLatin1Char('\'');
}

static bool braceMatch(const QChar &left, const QChar &right)
{
    switch (left.unicode()) {
    case '{':
        return right == '}';
    case '(':
        return right == ')';
    case '[':
        return right == ']';
    case '}':
        return right == '{';
    case ')':
        return right == '(';
    case ']':
        return right == '[';
    default:
        return false;
    }
}

struct BraceMatchResult
{
    BraceMatchResult() : position(-1), validMatch(false) { }
    BraceMatchResult(int pos, bool valid) : position(pos), validMatch(valid) { }

    int position;
    bool validMatch;
};

static BraceMatchResult findNextBrace(QTextBlock block, int position)
{
    QStack<QChar> balance;
    do {
        QString text = block.text();
        while (position < text.size()) {
            const QChar ch = text.at(position);
            if (isQuote(ch)) {
                if (!balance.isEmpty() && balance.top() == ch)
                    (void) balance.pop();
                else
                    balance.push(ch);
            } else if (!balance.isEmpty() && isQuote(balance.top())) {
                /* Don't look for matching braces until we exit the quote */
            } else if (ch == '(' || ch == '[' || ch == '{') {
                balance.push(ch);
            } else if (ch == ')' || ch == ']' || ch == '}') {
                if (balance.isEmpty())
                    return BraceMatchResult();
                const QChar match = balance.pop();
                if (balance.isEmpty())
                    return BraceMatchResult(block.position() + position, braceMatch(match, ch));
            }
            ++position;
        }

        block = block.next();
        position = 0;
    } while (block.isValid());

    // No match found in the document
    return BraceMatchResult();
}

static BraceMatchResult findPrevBrace(QTextBlock block, int position)
{
    QStack<QChar> balance;
    do {
        QString text = block.text();
        while (position > 0) {
            --position;
            const QChar ch = text.at(position);
            if (isQuote(ch)) {
                if (!balance.isEmpty() && balance.top() == ch)
                    (void) balance.pop();
                else
                    balance.push(ch);
            } else if (!balance.isEmpty() && isQuote(balance.top())) {
                /* Don't look for matching braces until we exit the quote */
            } else if (ch == ')' || ch == ']' || ch == '}') {
                balance.push(ch);
            } else if (ch == '(' || ch == '[' || ch == '{') {
                if (balance.isEmpty())
                    return BraceMatchResult();
                const QChar match = balance.pop();
                if (balance.isEmpty())
                    return BraceMatchResult(block.position() + position, braceMatch(match, ch));
            }
        }

        block = block.previous();
        position = block.text().size();
    } while (block.isValid());

    // No match found in the document
    return BraceMatchResult();
}

void QPlasmaTextEdit::updateCursor()
{
    QList<QTextEdit::ExtraSelection> selections;

    if (matchBraces()) {
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
        BraceMatchResult match;
        if (chNext == '(' || chNext == '[' || chNext == '{') {
            match = findNextBrace(cursor.block(), blockPos);
        } else if (chPrev == ')' || chPrev == ']' || chPrev == '}') {
            match = findPrevBrace(cursor.block(), blockPos);
            cursor.movePosition(QTextCursor::PreviousCharacter);
        }

        if (match.position >= 0) {
            QTextEdit::ExtraSelection selection;
            selection.format.setBackground(match.validMatch ? fBraceMatchBg : fErrorBg);
            selection.cursor = cursor;
            selection.cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
            selections.append(selection);
            selection.cursor = textCursor();
            selection.cursor.setPosition(match.position);
            selection.cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
            selections.append(selection);
        }
    }

    setExtraSelections(selections);

    // Repaint both the viewport and the line margin to ensure both get
    // correctly rendered with the current line change.
    viewport()->update();
    fLineMargin->update();
}

void QPlasmaTextEdit::updateTabMetrics()
{
    // setTabStopWidth only allows int widths, which doesn't line up correctly
    // on many fonts.  Hack from QtCreator: Set it in the text option instead
    const qreal tabWidth = QFontMetricsF(font()).width(QString(fTabCharSize, ' '));
    QTextOption opt = document()->defaultTextOption();
#if (QT_VERSION >= QT_VERSION_CHECK(5, 10, 0))
    opt.setTabStopDistance(tabWidth);
#else
    opt.setTabStop(tabWidth);
#endif
    document()->setDefaultTextOption(opt);
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
            if (ch == QLatin1Char('\t')) {
                leadingIndent += (fTabCharSize - (leadingIndent % fTabCharSize));
                startOfLine += 1;
            } else if (ch == QLatin1Char(' ')) {
                leadingIndent += 1;
                startOfLine += 1;
            } else {
                break;
            }
        }

        cursor.movePosition(QTextCursor::StartOfLine);
        cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor,
                            startOfLine);
        cursor.removeSelectedText();

        const int indent = leadingIndent + fTabCharSize;
        if (expandTabs()) {
            cursor.insertText(QString(indent, ' '));
        } else {
            const int tabs = indent / fTabCharSize;
            const int spaces = indent % fTabCharSize;
            cursor.insertText(QString(tabs, '\t'));
            cursor.insertText(QString(spaces, ' '));
        }

        if (!cursor.movePosition(QTextCursor::NextBlock))
            break;
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
            if (ch == QLatin1Char('\t')) {
                leadingIndent += (fTabCharSize - (leadingIndent % fTabCharSize));
                startOfLine += 1;
            } else if (ch == QLatin1Char(' ')) {
                leadingIndent += 1;
                startOfLine += 1;
            } else {
                break;
            }
        }

        cursor.movePosition(QTextCursor::StartOfLine);
        cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor,
                            startOfLine);
        cursor.removeSelectedText();

        const int indent = leadingIndent - fTabCharSize;
        if (indent > 0) {
            if (expandTabs()) {
                cursor.insertText(QString(indent, ' '));
            } else {
                const int tabs = indent / fTabCharSize;
                const int spaces = indent % fTabCharSize;
                cursor.insertText(QString(tabs, '\t'));
                cursor.insertText(QString(spaces, ' '));
            }
        }

        if (!cursor.movePosition(QTextCursor::NextBlock))
            break;
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
            const QString blockText = cursor.block().text();
            const QStringRef cursorText = blockText.leftRef(cursor.positionInBlock());
            int vpos = 0;
            for (const auto ch : cursorText) {
                if (ch == QLatin1Char('\t'))
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
                        if (ch.isSpace())
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
                    const QString leadingIndent = scanCursor.block().text().left(startOfLine);
                    textCursor().insertText(leadingIndent);
                }
            }
            undoCursor.endEditBlock();
        }
        break;

    case Qt::Key_Up:
        if (e->modifiers() & Qt::ControlModifier) {
            auto scrollBar = verticalScrollBar();
            scrollBar->setValue(scrollBar->value() - 1);
        } else {
            QPlainTextEdit::keyPressEvent(e);
        }
        break;

    case Qt::Key_Down:
        if (e->modifiers() & Qt::ControlModifier) {
            auto scrollBar = verticalScrollBar();
            scrollBar->setValue(scrollBar->value() + 1);
        } else {
            QPlainTextEdit::keyPressEvent(e);
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
    const QRect eventRect = e->rect();
    const QRect viewRect = viewport()->rect();
    QRectF cursorBlockRect;

    // Highlight current line first, so the long line marker will draw over it
    // Unlike setExtraSelections(), we paint the entire line past even the
    // document margins.
    QTextCursor cursor = textCursor();
    cursorBlockRect = blockBoundingGeometry(cursor.block());
    cursorBlockRect.translate(contentOffset());
    cursorBlockRect.setLeft(eventRect.left());
    cursorBlockRect.setWidth(eventRect.width());
    if (eventRect.intersects(cursorBlockRect.toAlignedRect())) {
        QPainter p(viewport());
        p.fillRect(cursorBlockRect, fCursorLineBg);
    }

    if (fLongLineMarker > 0) {
        QFontMetricsF fm(font());
        const qreal longLinePos = fm.width(QString(fLongLineMarker, 'x'))
                                  + contentOffset().x() + document()->documentMargin();
        if (longLinePos < viewRect.width()) {
            QPainter p(viewport());
            p.setPen(fLongLineColor);
            p.drawLine(longLinePos, eventRect.top(), longLinePos, eventRect.bottom());
        }
    }

    QPlainTextEdit::paintEvent(e);
}
