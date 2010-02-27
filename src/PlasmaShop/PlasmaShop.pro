PLSLIBS = core
include(../../config)

# General Configuration
TEMPLATE = app
DEPENDPATH += .
INCLUDEPATH += . ../3rdParty
CONFIG += release
DESTDIR = ../../bin$${PLAT_BITS}
RC_FILE = res/PlasmaShop.rc

# QScintilla
INCLUDEPATH += ../../QScintilla/Qt4
LIBS += -L../../bin$${PLAT_BITS} -lqscintilla2-ps3

# Use shell32.dll for the AppData thing on Windows
win32:LIBS += -lshell32

# Use the QtIconLoader wrapper
win32|macx {
    HEADERS += ../3rdParty/qticonloader.h
} else {
    HEADERS += ../3rdParty/qticonloader.h
    SOURCES += ../3rdParty/qticonloader.cpp
}

!win32 {
    target.path = /usr/local/bin
    INSTALLS += target
    DEFINES += DATA_PATH="\"\\\"/usr/local/share/PlasmaShop\\\"\""
}

# PlasmaShop Sources
HEADERS += Main.h \
           OptionsDialog.h \
           GameBrowser.h \
           GameScanner.h \
           QPlasmaDocument.h \
           QPlasmaTextDoc.h \
           QPlasmaSumFile.h \
           ../QPlasma.h
SOURCES += Main.cpp \
           OptionsDialog.cpp \
           GameBrowser.cpp \
           GameScanner.cpp \
           QPlasmaDocument.cpp \
           QPlasmaTextDoc.cpp \
           QPlasmaSumFile.cpp
RESOURCES += images.qrc
