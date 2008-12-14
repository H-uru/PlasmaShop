# General Configuration
TEMPLATE = app
DEPENDPATH += .
INCLUDEPATH += .
CONFIG += release
DESTDIR = ../../bin
RC_FILE = res/PrpShop.rc

# Use libPlasma
INCLUDEPATH += ../../../Plasma/core
LIBS += -L../../../Plasma/bin -lPlasma

# PrpShop Sources
HEADERS += Main.h \
           QPlasmaUtils.h \
           QPlasmaTreeItem.h \
           QCreatable.h
SOURCES += Main.cpp \
           QPlasmaUtils.cpp \
           QPlasmaTreeItem.cpp \
           QCreatable.cpp
RESOURCES += images.qrc
