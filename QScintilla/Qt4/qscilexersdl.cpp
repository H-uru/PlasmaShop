// This module implements the QsciLexerSDL class.
//
// Copyright (c) 2008 Riverbank Computing Limited <info@riverbankcomputing.com>
// 
// This file is part of QScintilla.
// 
// This file may be used under the terms of the GNU General Public
// License versions 2.0 or 3.0 as published by the Free Software
// Foundation and appearing in the files LICENSE.GPL2 and LICENSE.GPL3
// included in the packaging of this file.  Alternatively you may (at
// your option) use any later version of the GNU General Public
// License if such license has been publicly approved by Riverbank
// Computing Limited (or its successors, if any) and the KDE Free Qt
// Foundation. In addition, as a special exception, Riverbank gives you
// certain additional rights. These rights are described in the Riverbank
// GPL Exception version 1.1, which can be found in the file
// GPL_EXCEPTION.txt in this package.
// 
// Please review the following information to ensure GNU General
// Public Licensing requirements will be met:
// http://trolltech.com/products/qt/licenses/licensing/opensource/. If
// you are unsure which license is appropriate for your use, please
// review the following information:
// http://trolltech.com/products/qt/licenses/licensing/licensingoverview
// or contact the sales department at sales@riverbankcomputing.com.
// 
// This file is provided "AS IS" with NO WARRANTY OF ANY KIND,
// INCLUDING THE WARRANTIES OF DESIGN, MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE. Trolltech reserves all rights not expressly
// granted herein.
// 
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.


#include "QsciPS3/qscilexersdl.h"

#include <qcolor.h>
#include <qfont.h>
#include <qsettings.h>


// The ctor.
QsciLexerSDL::QsciLexerSDL(QObject *parent)
    : QsciLexer(parent),
      fold_comments(false), fold_compact(true)
{
}


// The dtor.
QsciLexerSDL::~QsciLexerSDL()
{
}


// Returns the language name.
const char *QsciLexerSDL::language() const
{
    return "SDL";
}


// Returns the lexer name.
const char *QsciLexerSDL::lexer() const
{
    return "sdl";
}


// Return the set of character sequences that can separate auto-completion
// words.
QStringList QsciLexerSDL::autoCompletionWordSeparators() const
{
    QStringList wl;

    wl << ".";

    return wl;
}


// Return the list of keywords that can start a block.
const char *QsciLexerSDL::blockStartKeyword(int *style) const
{
    if (style)
        *style = Keyword;

    return "struct STATEDESC";
}


// Return the list of characters that can start a block.
const char *QsciLexerSDL::blockStart(int *style) const
{
    if (style)
        *style = Operator;

    return "{";
}


// Return the list of characters that can end a block.
const char *QsciLexerSDL::blockEnd(int *style) const
{
    if (style)
        *style = Operator;

    return "}";
}


// Return the style used for braces.
int QsciLexerSDL::braceStyle() const
{
    return Operator;
}


// Return the string of characters that comprise a word.
const char *QsciLexerSDL::wordCharacters() const
{
    return "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_#";
}


// Returns the foreground colour of the text for a style.
QColor QsciLexerSDL::defaultColor(int style) const
{
    switch (style)
    {
    case Default:
        return QColor(0x80,0x80,0x80);

    case Comment:
    case CommentLine:
    case CommentBlock:
        return QColor(0x00,0x7f,0x00);

    case Number:
        return QColor(0x00,0x00,0x7f);

    case Keyword:
        return QColor(0x00,0x00,0x00);

    case DoubleQuotedString:
    case SingleQuotedString:
    case UnclosedString:
        return QColor(0x7f,0x00,0x00);

    case Operator:
        return QColor(0x00,0x00,0x00);

    case Identifier:
        break;

    case KeywordSet2:
        return QColor(0x00,0x00,0xff);

    case KeywordSet3:
        return QColor(0x7f,0x7f,0x00);
    }

    return QsciLexer::defaultColor(style);
}


// Returns the end-of-line fill for a style.
bool QsciLexerSDL::defaultEolFill(int style) const
{
    if (style == UnclosedString)
        return true;

    return QsciLexer::defaultEolFill(style);
}


// Returns the font of the text for a style.
QFont QsciLexerSDL::defaultFont(int style) const
{
    QFont f;

    switch (style)
    {
    case Keyword:
        f = QsciLexer::defaultFont(style);
        f.setBold(true);
        break;

    default:
        f = QsciLexer::defaultFont(style);
    }

    return f;
}


// Returns the set of keywords.
const char *QsciLexerSDL::keywords(int set) const
{
    if (set == 1)
        return
            "DEFAULT DEFAULTOPTION DISPLAYOPTION STATEDESC VAR VERSION "
            "default struct version";

    if (set == 2)
        return
            "AGETIMEOFDAY BOOL BYTE CREATABLE FLOAT INT PLKEY POINT3 "
            "QUATERNION RGB8 SHORT STRING32 TIME VECTOR3 "
            "AgeTimeElapsed AgeTimeOfDay bool buffer byte double float "
            "GameTimeElapsed int matrix44 point3 quaternion string uint "
            "uoid vector3";

    if (set == 3)
        return
            "false true nil VAULT red hidden";

    return 0;
}


// Returns the user name of a style.
QString QsciLexerSDL::description(int style) const
{
    switch (style)
    {
    case Default:
        return tr("Default");

    case CommentLine:
        return tr("Comment line");

    case DoubleQuotedString:
        return tr("Double-quoted string");

    case SingleQuotedString:
        return tr("Single-quoted string");

    case Number:
        return tr("Number");

    case Keyword:
        return tr("Keyword");

    case CommentBlock:
        return tr("Comment block");

    case Operator:
        return tr("Operator");

    case Identifier:
        return tr("Identifier");

    case UnclosedString:
        return tr("Unclosed string");

    case KeywordSet2:
        return tr("Type keywords");

    case Comment:
        return tr("Comment");

    case KeywordSet3:
        return tr("Special built-in keywords");
    }

    return QString();
}


// Returns the background colour of the text for a style.
QColor QsciLexerSDL::defaultPaper(int style) const
{
    if (style == UnclosedString)
        return QColor(0xe0,0xc0,0xe0);

    return QsciLexer::defaultPaper(style);
}


// Refresh all properties.
void QsciLexerSDL::refreshProperties()
{
    setCommentProp();
    setCompactProp();
}


// Read properties from the settings.
bool QsciLexerSDL::readProperties(QSettings &qs,const QString &prefix)
{
    int rc = true;

    fold_comments = qs.value(prefix + "foldcomments", false).toBool();
    fold_compact = qs.value(prefix + "foldcompact", true).toBool();

    return rc;
}


// Write properties to the settings.
bool QsciLexerSDL::writeProperties(QSettings &qs,const QString &prefix) const
{
    int rc = true;

    qs.setValue(prefix + "foldcomments", fold_comments);
    qs.setValue(prefix + "foldcompact", fold_compact);

    return rc;
}


// Return true if comments can be folded.
bool QsciLexerSDL::foldComments() const
{
    return fold_comments;
}


// Set if comments can be folded.
void QsciLexerSDL::setFoldComments(bool fold)
{
    fold_comments = fold;

    setCommentProp();
}


// Set the "fold.comment" property.
void QsciLexerSDL::setCommentProp()
{
    emit propertyChanged("fold.comment",(fold_comments ? "1" : "0"));
}


// Return true if folds are compact.
bool QsciLexerSDL::foldCompact() const
{
    return fold_compact;
}


// Set if folds are compact
void QsciLexerSDL::setFoldCompact(bool fold)
{
    fold_compact = fold;

    setCompactProp();
}


// Set the "fold.compact" property.
void QsciLexerSDL::setCompactProp()
{
    emit propertyChanged("fold.compact",(fold_compact ? "1" : "0"));
}
