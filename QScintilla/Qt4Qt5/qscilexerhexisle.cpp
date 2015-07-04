// This defines the interface to the QsciLexerCPP class.
//
// Copyright (c) 2015 Riverbank Computing Limited <info@riverbankcomputing.com>
// 
// This file is part of QScintilla.
// 
// This file may be used under the terms of the GNU General Public License
// version 3.0 as published by the Free Software Foundation and appearing in
// the file LICENSE included in the packaging of this file.  Please review the
// following information to ensure the GNU General Public License version 3.0
// requirements will be met: http://www.gnu.org/copyleft/gpl.html.
// 
// If you do not wish to use this file under the terms of the GPL version 3.0
// then you may purchase a commercial license.  For more information contact
// info@riverbankcomputing.com.
// 
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.


#include "QsciPS3/qscilexerhexisle.h"

#include <qcolor.h>
#include <qfont.h>
#include <qsettings.h>


// The ctor.
QsciLexerHexIsle::QsciLexerHexIsle(QObject *parent)
    : QsciLexer(parent),
      fold_compact(true)
{
}


// The dtor.
QsciLexerHexIsle::~QsciLexerHexIsle()
{
}


// Returns the language name.
const char *QsciLexerHexIsle::language() const
{
    return "HexIsle";
}


// Returns the lexer name.
const char *QsciLexerHexIsle::lexer() const
{
    return "hex";
}


// Return the string of characters that comprise a word.
const char *QsciLexerHexIsle::wordCharacters() const
{
    return "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-";
}


// Returns the foreground colour of the text for a style.
QColor QsciLexerHexIsle::defaultColor(int style) const
{
    switch (style)
    {
    case Comment:
        return QColor(0x00,0x7f,0x00);

    case Section:
        return QColor(0x7f,0x7f,0x00);

    case DefaultValue:
        return QColor(0x7f,0x7f,0x00);

    case Key:
        return QColor(0x00,0x00,0xff);

    case Number:
        return QColor(0x00,0x00,0x7f);

    case String:
        return QColor(0x7f,0x00,0x00);

    case Keyword:
        return QColor(0x00,0x7f,0x7f);
    }

    return QsciLexer::defaultColor(style);
}


// Returns the end-of-line fill for a style.
bool QsciLexerHexIsle::defaultEolFill(int style) const
{
    if (style == Section)
        return true;

    return QsciLexer::defaultEolFill(style);
}


// Returns the font of the text for a style.
QFont QsciLexerHexIsle::defaultFont(int style) const
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


// Returns the user name of a style.
QString QsciLexerHexIsle::description(int style) const
{
    switch (style)
    {
    case Default:
        return tr("Default");

    case Comment:
        return tr("Comment");

    case Section:
        return tr("Section");

    case Assignment:
        return tr("Assignment");

    case DefaultValue:
        return tr("Default value");

    case Key:
        return tr("Key");

    case Number:
        return tr("Number");

    case String:
        return tr("String");

    case Keyword:
        return tr("Keyword");
    }

    return QString();
}


// Refresh all properties.
void QsciLexerHexIsle::refreshProperties()
{
    setCompactProp();
}


// Read properties from the settings.
bool QsciLexerHexIsle::readProperties(QSettings &qs,const QString &prefix)
{
    int rc = true;

    fold_compact = qs.value(prefix + "foldcompact", true).toBool();

    return rc;
}


// Write properties to the settings.
bool QsciLexerHexIsle::writeProperties(QSettings &qs,const QString &prefix) const
{
    int rc = true;

    qs.setValue(prefix + "foldcompact", fold_compact);

    return rc;
}


// Return true if folds are compact.
bool QsciLexerHexIsle::foldCompact() const
{
    return fold_compact;
}


// Set if folds are compact
void QsciLexerHexIsle::setFoldCompact(bool fold)
{
    fold_compact = fold;

    setCompactProp();
}


// Set the "fold.compact" property.
void QsciLexerHexIsle::setCompactProp()
{
    emit propertyChanged("fold.compact",(fold_compact ? "1" : "0"));
}
