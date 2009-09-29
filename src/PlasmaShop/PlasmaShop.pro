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
           ../QPlasma.h
SOURCES += Main.cpp
RESOURCES += images.qrc
