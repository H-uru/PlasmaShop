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


#ifndef QSCILEXERFNI_H
#define QSCILEXERFNI_H

#ifdef __APPLE__
extern "C++" {
#endif

#include <QObject>

#include <QsciPS3/qsciglobal.h>
#include <QsciPS3/qscilexer.h>


//! \brief The QsciLexerFni class encapsulates the Scintilla
//! Fni lexer.
class QSCINTILLA_EXPORT QsciLexerFni : public QsciLexer
{
    Q_OBJECT

public:
    //! This enum defines the meanings of the different styles used by the
    //! Properties lexer.
    enum {
        //! The default.
        Default = 0,

        //! A comment.
        Comment = 1,

        //! A command group.
        Group = 2,

        //! A console command.
        Command = 3,

        //! An operator.
        Operator = 4
    };

    //! Construct a QsciLexerFni with parent \a parent.  \a parent is
    //! typically the QsciScintilla instance.
    QsciLexerFni(QObject *parent = 0);

    //! Destroys the QsciLexerFni instance.
    virtual ~QsciLexerFni();

    //! Returns the name of the language.
    const char *language() const;

    //! Returns the name of the lexer.  Some lexers support a number of
    //! languages.
    const char *lexer() const;

    //! \internal Returns the string of characters that comprise a word.
    const char *wordCharacters() const;

    //! Returns the foreground colour of the text for style number \a style.
    //!
    //! \sa defaultPaper()
    QColor defaultColor(int style) const;

    //! Returns the descriptive name for style number \a style.  If the
    //! style is invalid for this language then an empty QString is returned.
    //! This is intended to be used in user preference dialogs.
    QString description(int style) const;

private:
    QsciLexerFni(const QsciLexerFni &);
    QsciLexerFni &operator=(const QsciLexerFni &);
};

#ifdef __APPLE__
}
#endif

#endif
