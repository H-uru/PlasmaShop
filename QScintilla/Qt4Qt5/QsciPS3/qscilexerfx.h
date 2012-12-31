// This defines the interface to the QsciLexer class.
//
// Copyright (c) 2012 Riverbank Computing Limited <info@riverbankcomputing.com>
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
// If you are unsure which license is appropriate for your use, please
// contact the sales department at sales@riverbankcomputing.com.
// 
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.


#ifndef QSCILEXERFX_H
#define QSCILEXERFX_H

#ifdef __APPLE__
extern "C++" {
#endif

#include <qobject.h>

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
