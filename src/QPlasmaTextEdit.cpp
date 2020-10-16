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

#include <KSyntaxHighlighting/Theme>
#include <KSyntaxHighlighting/Definition>
#include <KSyntaxHighlighting/Repository>

KSyntaxHighlighting::Repository* QPlasmaTextEdit::SyntaxRepo()
{
    static KSyntaxHighlighting::Repository *s_syntaxRepo = SyntaxTextEdit::syntaxRepo();
#ifndef KSYN_NO_CUSTOM_RULES_SUPPORT
    static bool s_firstInit = true;
    if (s_firstInit) {
        s_firstInit = false;
        s_syntaxRepo->addCustomSearchPath(":/plasma-syntax");
    }
#endif
    return s_syntaxRepo;
}

QPlasmaTextEdit::QPlasmaTextEdit(QWidget* parent)
    : SyntaxTextEdit(parent)
{
    setTabWidth(8);
    setHighlightCurrentLine(true);
}

void QPlasmaTextEdit::setSyntax(const QString& name)
{
    KSyntaxHighlighting::Definition syntaxDef = SyntaxRepo()->definitionForName(name);
    if (!syntaxDef.isValid() && !name.isEmpty())
        qDebug("Warning: Could not find syntax defintion for %s", name.toUtf8().constData());
    SyntaxTextEdit::setSyntax(syntaxDef);
}

void QPlasmaTextEdit::keyPressEvent(QKeyEvent* e)
{
    switch (e->key()) {
    case Qt::Key_Up:
        if ((e->modifiers() & Qt::ControlModifier) && (e->modifiers() & Qt::ShiftModifier)) {
            moveLines(QTextCursor::PreviousBlock);
        } else {
            SyntaxTextEdit::keyPressEvent(e);
        }
        break;

    case Qt::Key_Down:
        if ((e->modifiers() & Qt::ControlModifier) && (e->modifiers() & Qt::ShiftModifier)) {
            moveLines(QTextCursor::NextBlock);
        } else {
            SyntaxTextEdit::keyPressEvent(e);
        }
        break;

    case Qt::Key_D:
        if (e->modifiers() & Qt::ControlModifier) {
            // Default of Ctrl+D is the same as "Del"; This means we can
            // repurpose it for "Delete Line"
            deleteLines();
        } else {
            SyntaxTextEdit::keyPressEvent(e);
        }
        break;

    default:
        SyntaxTextEdit::keyPressEvent(e);
        break;
    }
}
