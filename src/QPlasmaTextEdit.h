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

#ifndef _QPLASMATEXTEDIT_H
#define _QPLASMATEXTEDIT_H

#include <QPlainTextEdit>
#include <SyntaxHighlighter>

namespace KSyntaxHighlighting
{
    class Theme;
    class Repository;
}
class WhitespaceKSyntaxHighlighter;

class QPlasmaTextEdit : public QPlainTextEdit
{
    Q_OBJECT

    enum EditorSettings
    {
        kShowLineNumbers = (1<<0),
        kExpandTabs = (1<<1),
        kAutoIndent = (1<<2),
        kMatchBraces = (1<<3),
    };

public:
    explicit QPlasmaTextEdit(QWidget* parent);

    bool haveSelection() const;

    int lineMarginWidth();
    void paintLineNumbers(QPaintEvent* e);
    bool showLineNumbers() const { return (fEditorSettings & kShowLineNumbers) != 0; }
    void setShowLineNumbers(bool show);

    void setTabWidth(int width);
    int tabWidth() const { return fTabCharSize; }
    void setExpandTabs(bool expand);
    bool expandTabs() const { return (fEditorSettings & kExpandTabs) != 0; }

    void setAutoIndent(bool ai);
    bool autoIndent() const { return (fEditorSettings & kAutoIndent) != 0; }

    void setLongLineMarker(int pos);
    int longLineMarker() const { return fLongLineMarker; }

    void setMatchBraces(bool match);
    bool matchBraces() const { return (fEditorSettings & kMatchBraces) != 0; }

    static KSyntaxHighlighting::Repository* SyntaxRepo();

    void setTheme(const KSyntaxHighlighting::Theme& theme);
    void setSyntax(const QString& name);

    void moveLines(QTextCursor::MoveOperation op);
    void smartHome(QTextCursor::MoveMode mode);
    void deleteLines();

protected:
    void resizeEvent(QResizeEvent* e) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent* e) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent* e) Q_DECL_OVERRIDE;

public slots:
    void indentSelection();
    void outdentSelection();

private slots:
    void updateMargins();
    void updateLineNumbers(const QRect& rect, int dy);
    void updateCursor();
    void updateTabMetrics();

private:
    QWidget* fLineMargin;
    WhitespaceKSyntaxHighlighter* fHighlighter;
    QColor fLineMarginBg, fLineMarginFg;
    QColor fCursorLineBg, fCursorLineNum;
    QColor fLongLineColor;
    QColor fBraceMatchBg;
    QColor fErrorBg;
    int fTabCharSize;
    int fLongLineMarker;
    unsigned int fEditorSettings;
};

#endif
