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
           QVaultNode.h \
           QVaultNodeEdit.h \
           QVaultFolderNode.h \
           QVaultTextNode.h
SOURCES += Main.cpp \
           QVaultNode.cpp \
           QVaultNodeEdit.cpp \
           QVaultFolderNode.cpp \
           QVaultTextNode.cpp
RESOURCES += images.qrc
