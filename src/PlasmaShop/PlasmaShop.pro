# General Configuration
TEMPLATE = app
DEPENDPATH += .
INCLUDEPATH += . ../3rdParty
CONFIG += release
DESTDIR = ../../bin
RC_FILE = res/PlasmaShop.rc

# Use libPlasma
INCLUDEPATH += ../../../Plasma/core
LIBS += -L../../../Plasma/bin -lPlasma

# QScintilla
win32 {
    debug:LIBS += -L../../QScintilla/Qt4/debug -lqscintilla2-ps3
    release:LIBS += -L../../QScintilla/Qt4/release -lqscintilla2-ps3
    DEFINES += PLAT_FONT="\"\\\"Courier New\\\"\"" PLAT_FONTSIZE=10
} else {
    LIBS += -L../../QScintilla/Qt4 -lqscintilla2-ps3
    DEFINES += PLAT_FONT="\"\\\"Monospace\\\"\"" PLAT_FONTSIZE=10
}
INCLUDEPATH += ../../QScintilla/Qt4

# Make VS shut up a bit
win32:QMAKE_CXXFLAGS += /wd4244 /wd4251 /wd4267 /wd4996

# Use the QtIconLoader wrapper
win32|macx {
    HEADERS += ../3rdParty/qticonloader.h
} else {
    HEADERS += ../3rdParty/qticonloader.h
    SOURCES += ../3rdParty/qticonloader.cpp
}

# PlasmaShop Sources
HEADERS += Main.h \
           QPlasmaDocument.h \
           QPlasmaTextDoc.h \
           ../QPlasma.h
SOURCES += Main.cpp \
           QPlasmaDocument.cpp \
           QPlasmaTextDoc.cpp
RESOURCES += images.qrc
