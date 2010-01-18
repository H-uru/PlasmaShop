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
    DEFINES += PLAT_FONT="\"\\\"Courier New\\\"\""
} else {
    DEFINES += PLAT_FONT="\"\\\"Monospace\\\"\""
}
INCLUDEPATH += ../../QScintilla/Qt4
LIBS += -L../../bin -lqscintilla2-ps3

# Make VS shut up a bit
win32:QMAKE_CXXFLAGS += /wd4244 /wd4251 /wd4267 /wd4996

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
