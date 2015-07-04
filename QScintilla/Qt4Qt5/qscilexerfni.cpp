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


#include "QsciPS3/qscilexerfni.h"

#include <qcolor.h>
#include <qfont.h>
#include <qsettings.h>


// The ctor.
QsciLexerFni::QsciLexerFni(QObject *parent)
    : QsciLexer(parent)
{
}


// The dtor.
QsciLexerFni::~QsciLexerFni()
{
}


// Returns the language name.
const char *QsciLexerFni::language() const
{
    return "Fni";
}


// Returns the lexer name.
const char *QsciLexerFni::lexer() const
{
    return "fni";
}


// Return the string of characters that comprise a word.
const char *QsciLexerFni::wordCharacters() const
{
    return "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_";
}


// Returns the foreground colour of the text for a style.
QColor QsciLexerFni::defaultColor(int style) const
{
    switch (style)
    {
    case Default:
        return QColor(0x00,0x00,0x00);

    case Comment:
        return QColor(0x00,0x7f,0x00);

    case Group:
        return QColor(0x7f,0x00,0x7f);

    case Command:
        return QColor(0x00,0x00,0xff);

    case Operator:
        return QColor(0x00,0x00,0x00);
    }

    return QsciLexer::defaultColor(style);
}


// Returns the user name of a style.
QString QsciLexerFni::description(int style) const
{
    switch (style)
    {
    case Default:
        return tr("Default");

    case Comment:
        return tr("Comment");

    case Group:
        return tr("Command group");

    case Command:
        return tr("Console command");

    case Operator:
        return tr("Operator");
    }

    return QString();
}
