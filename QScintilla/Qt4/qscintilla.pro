# The project file for the QScintilla library.
#
# Copyright (c) 2008 Riverbank Computing Limited <info@riverbankcomputing.com>
# 
# This file is part of QScintilla.
# 
# This file may be used under the terms of the GNU General Public
# License versions 2.0 or 3.0 as published by the Free Software
# Foundation and appearing in the files LICENSE.GPL2 and LICENSE.GPL3
# included in the packaging of this file.  Alternatively you may (at
# your option) use any later version of the GNU General Public
# License if such license has been publicly approved by Riverbank
# Computing Limited (or its successors, if any) and the KDE Free Qt
# Foundation. In addition, as a special exception, Riverbank gives you
# certain additional rights. These rights are described in the Riverbank
# GPL Exception version 1.1, which can be found in the file
# GPL_EXCEPTION.txt in this package.
# 
# Please review the following information to ensure GNU General
# Public Licensing requirements will be met:
# http://trolltech.com/products/qt/licenses/licensing/opensource/. If
# you are unsure which license is appropriate for your use, please
# review the following information:
# http://trolltech.com/products/qt/licenses/licensing/licensingoverview
# or contact the sales department at sales@riverbankcomputing.com.
# 
# This file is provided "AS IS" with NO WARRANTY OF ANY KIND,
# INCLUDING THE WARRANTIES OF DESIGN, MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE. Trolltech reserves all rights not expressly
# granted herein.
# 
# This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
# WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.


# This must be kept in sync with configure.py.
!win32:VERSION = 5.1.0

TEMPLATE = lib
TARGET = qscintilla2-ps3
DESTDIR = ../../bin
CONFIG += qt warn_off release dll thread
INCLUDEPATH = . ../include ../src
DEFINES = QSCINTILLA_MAKE_DLL QT SCI_LEXER

# Handle both Qt v4 and v3.
target.path = $$[QT_INSTALL_LIBS]
isEmpty(target.path) {
    target.path = $(QTDIR)/lib
}

header.path = $$[QT_INSTALL_HEADERS]
header.files = QsciPS3
isEmpty(header.path) {
    header.path = $(QTDIR)/include/QsciPS3
    header.files = QsciPS3/qsci*.h
}

trans.path = $$[QT_INSTALL_TRANSLATIONS]
trans.files = qscintilla_*.qm
isEmpty(trans.path) {
    trans.path = $(QTDIR)/translations
}

qsci.path = $$[QT_INSTALL_DATA]
qsci.files = ../qsci
isEmpty(qsci.path) {
    qsci.path = $(QTDIR)
}

INSTALLS += header trans qsci target

HEADERS = \
    ./QsciPS3/qsciglobal.h \
    ./QsciPS3/qsciscintilla.h \
    ./QsciPS3/qsciscintillabase.h \
    ./QsciPS3/qsciabstractapis.h \
    ./QsciPS3/qsciapis.h \
    ./QsciPS3/qscicommand.h \
    ./QsciPS3/qscicommandset.h \
    ./QsciPS3/qscidocument.h \
    ./QsciPS3/qscilexer.h \
    ./QsciPS3/qscilexercpp.h \
    ./QsciPS3/qscilexerfni.h \
    ./QsciPS3/qscilexerfx.h \
    ./QsciPS3/qscilexerhexisle.h \
    ./QsciPS3/qscilexerhtml.h \
    ./QsciPS3/qscilexerproperties.h \
    ./QsciPS3/qscilexerpython.h \
    ./QsciPS3/qscilexersdl.h \
    ./QsciPS3/qscilexerxml.h \
    ./QsciPS3/qscimacro.h \
    ./QsciPS3/qsciprinter.h \
    ./QsciPS3/qscistyle.h \
    ./QsciPS3/qscistyledtext.h \
    ListBoxQt.h \
    SciClasses.h \
    ScintillaQt.h \
    ../include/Accessor.h \
    ../include/KeyWords.h \
    ../include/Platform.h \
    ../include/PropSet.h \
    ../include/SString.h \
    ../include/SciLexer.h \
    ../include/Scintilla.h \
    ../include/ScintillaWidget.h \
    ../src/AutoComplete.h \
    ../src/CallTip.h \
    ../src/CellBuffer.h \
    ../src/CharClassify.h \
    ../src/ContractionState.h \
    ../src/Decoration.h \
    ../src/Document.h \
    ../src/DocumentAccessor.h \
    ../src/Editor.h \
    ../src/ExternalLexer.h \
    ../src/Indicator.h \
    ../src/KeyMap.h \
    ../src/LineMarker.h \
    ../src/Partitioning.h \
    ../src/PerLine.h \
    ../src/PositionCache.h \
    ../src/RESearch.h \
    ../src/RunStyles.h \
    ../src/SVector.h \
    ../src/ScintillaBase.h \
    ../src/SplitVector.h \
    ../src/Style.h \
    ../src/StyleContext.h \
    ../src/ViewStyle.h \
    ../src/XPM.h

SOURCES = \
    qsciscintilla.cpp \
    qsciscintillabase.cpp \
    qsciabstractapis.cpp \
    qsciapis.cpp \
    qscicommand.cpp \
    qscicommandset.cpp \
    qscidocument.cpp \
    qscilexer.cpp \
    qscilexercpp.cpp \
    qscilexerfni.cpp \
    qscilexerfx.cpp \
    qscilexerhexisle.cpp \
    qscilexerhtml.cpp \
    qscilexerproperties.cpp \
    qscilexerpython.cpp \
    qscilexersdl.cpp \
    qscilexerxml.cpp \
    qscimacro.cpp \
    qsciprinter.cpp \
    qscistyle.cpp \
    qscistyledtext.cpp \
    SciClasses.cpp \
    ListBoxQt.cpp \
    PlatQt.cpp \
    ScintillaQt.cpp \
    ../src/AutoComplete.cpp \
    ../src/CallTip.cpp \
    ../src/CellBuffer.cpp \
    ../src/CharClassify.cpp \
    ../src/ContractionState.cpp \
    ../src/Decoration.cpp \
    ../src/Document.cpp \
    ../src/DocumentAccessor.cpp \
    ../src/Editor.cpp \
    ../src/ExternalLexer.cpp \
    ../src/Indicator.cpp \
    ../src/KeyMap.cpp \
    ../src/KeyWords.cpp \
    ../src/LexCPP.cpp \
    ../src/LexHTML.cpp \
    ../src/LexOthers.cpp \
    ../src/LexPython.cpp \
    ../src/LexSDL.cpp \
    ../src/LineMarker.cpp \
    ../src/PerLine.cpp \
    ../src/PositionCache.cpp \
    ../src/PropSet.cpp \
    ../src/RESearch.cpp \
    ../src/RunStyles.cpp \
    ../src/ScintillaBase.cpp \
    ../src/Style.cpp \
    ../src/StyleContext.cpp \
    ../src/ViewStyle.cpp \
    ../src/XPM.cpp

TRANSLATIONS = \
    qscintilla_cs.ts \
    qscintilla_de.ts \
    qscintilla_es.ts \
    qscintilla_fr.ts \
    qscintilla_pt_br.ts \
    qscintilla_ru.ts
