set(QScintilla_MOC_Headers
    ./QsciPS3/qsciscintilla.h
    ./QsciPS3/qsciscintillabase.h
    ./QsciPS3/qsciabstractapis.h
    ./QsciPS3/qsciapis.h
    ./QsciPS3/qscilexer.h
    ./QsciPS3/qscilexercpp.h
    ./QsciPS3/qscilexerfni.h
    ./QsciPS3/qscilexerfx.h
    ./QsciPS3/qscilexerhexisle.h
    ./QsciPS3/qscilexerhtml.h
    ./QsciPS3/qscilexerproperties.h
    ./QsciPS3/qscilexerpython.h
    ./QsciPS3/qscilexersdl.h
    ./QsciPS3/qscilexerxml.h
    ./QsciPS3/qscimacro.h
    SciClasses.h
)

set(QScintilla_Sources
    qsciscintilla.cpp
    qsciscintillabase.cpp
    qsciabstractapis.cpp
    qsciapis.cpp
    qscicommand.cpp
    qscicommandset.cpp
    qscidocument.cpp
    qscilexer.cpp
    qscilexercpp.cpp
    qscilexerfni.cpp
    qscilexerfx.cpp
    qscilexerhexisle.cpp
    qscilexerhtml.cpp
    qscilexerproperties.cpp
    qscilexerpython.cpp
    qscilexersdl.cpp
    qscilexerxml.cpp
    qscimacro.cpp
    qsciprinter.cpp
    qscistyle.cpp
    qscistyledtext.cpp
    SciClasses.cpp
    ListBoxQt.cpp
    PlatQt.cpp
    ScintillaQt.cpp
    ../lexers/LexCPP.cpp
    ../lexers/LexHTML.cpp
    ../lexers/LexOthers.cpp
    ../lexers/LexPython.cpp
    ../lexers/LexSDL.cpp
    ../lexlib/Accessor.cpp
    ../lexlib/CharacterSet.cpp
    ../lexlib/LexerBase.cpp
    ../lexlib/LexerModule.cpp
    ../lexlib/LexerNoExceptions.cpp
    ../lexlib/LexerSimple.cpp
    ../lexlib/PropSetSimple.cpp
    ../lexlib/StyleContext.cpp
    ../lexlib/WordList.cpp
    ../src/AutoComplete.cpp
    ../src/CallTip.cpp
    ../src/Catalogue.cpp
    ../src/CellBuffer.cpp
    ../src/CharClassify.cpp
    ../src/ContractionState.cpp
    ../src/Decoration.cpp
    ../src/Document.cpp
    ../src/Editor.cpp
    ../src/ExternalLexer.cpp
    ../src/Indicator.cpp
    ../src/KeyMap.cpp
    ../src/LineMarker.cpp
    ../src/PerLine.cpp
    ../src/PositionCache.cpp
    ../src/RESearch.cpp
    ../src/RunStyles.cpp
    ../src/ScintillaBase.cpp
    ../src/Selection.cpp
    ../src/Style.cpp
    ../src/UniConversion.cpp
    ../src/ViewStyle.cpp
    ../src/XPM.cpp
)

find_package(Qt4 REQUIRED COMPONENTS QtCore QtGui)
include(${QT_USE_FILE})
qt4_wrap_cpp(QScintilla_MOC ${QScintilla_MOC_Headers})

include_directories("${PROJECT_SOURCE_DIR}/QScintilla/Qt4")
include_directories("${PROJECT_SOURCE_DIR}/QScintilla/include")
include_directories("${PROJECT_SOURCE_DIR}/QScintilla/lexlib")
include_directories("${PROJECT_SOURCE_DIR}/QScintilla/src")

add_definitions(-DQSCINTILLA_MAKE_DLL -DQT -DSCI_LEXER)

add_library(qscintilla2-ps3 STATIC ${QScintilla_Sources} ${QScintilla_MOC})
target_link_libraries(qscintilla2-ps3 ${QT_LIBRARIES})
