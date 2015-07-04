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


#ifndef QSCILEXERFX_H
#define QSCILEXERFX_H

#ifdef __APPLE__
extern "C++" {
#endif

#include <QObject>

#include <QsciPS3/qsciglobal.h>
#include <QsciPS3/qscilexercpp.h>


//! \brief The QsciLexerFX class defines a lexer for HLSL FX files
//! lexer.
class QSCINTILLA_EXPORT QsciLexerFX : public QsciLexerCPP
{
    Q_OBJECT

public:
    //! Construct a QsciLexerFX with parent \a parent.  \a parent is
    //! typically the QsciScintilla instance.
    QsciLexerFX(QObject *parent = 0);

    //! Destroys the QsciLexerFX instance.
    virtual ~QsciLexerFX();

    //! Returns the name of the language.
    const char *language() const;

    //! Returns the set of keywords for the keyword set \a set recognised
    //! by the lexer as a space separated string.
    const char *keywords(int set) const;

    //! Returns the foreground colour of the text for style number \a style.
    //!
    //! \sa defaultPaper()
    QColor defaultColor(int style) const;

    //! Returns the font for style number \a style.
    QFont defaultFont(int style) const;

private:
    QsciLexerFX(const QsciLexerFX &);
    QsciLexerFX &operator=(const QsciLexerFX &);
};

#ifdef __APPLE__
}
#endif

#endif
