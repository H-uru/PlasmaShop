# General Configuration
TEMPLATE = app
DEPENDPATH += .
INCLUDEPATH += .
CONFIG += release
DESTDIR = ../../bin

# Use libPlasma
INCLUDEPATH += ../../../Plasma/core
LIBS += -L../../../Plasma/bin -lPlasma

# VaultShop Sources
HEADERS += Main.h \
           QVaultNode.h
SOURCES += Main.cpp \
           QVaultNode.cpp
RESOURCES += images.qrc
