// This module implements the QsciStyledText class.
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


#include "QsciPS3/qscistyledtext.h"

#include "QsciPS3/qsciscintillabase.h"
#include "QsciPS3/qscistyle.h"


// A ctor.
QsciStyledText::QsciStyledText(const QString &text, int style)
    : styled_text(text), style_nr(style), explicit_style(0)
{
}


// A ctor.
QsciStyledText::QsciStyledText(const QString &text, const QsciStyle &style)
    : styled_text(text), style_nr(-1)
{
    explicit_style = new QsciStyle(style);
}


// Return the number of the style.
int QsciStyledText::style() const
{
    return explicit_style ? explicit_style->style() : style_nr;
}


// Apply any explicit style to an editor.
void QsciStyledText::apply(QsciScintillaBase *sci) const
{
    if (explicit_style)
        explicit_style->apply(sci);
}
