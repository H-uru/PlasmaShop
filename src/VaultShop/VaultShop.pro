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
           QColorEdit.h \
           QVaultNode.h \
           QVaultNodeEdit.h \
           QVaultFolderNode.h \
           QVaultTextNode.h \
           QVaultSDLNode.h \
           QVaultChronicleNode.h \
           QVaultLinkNode.h \
           QVaultPlayerNode.h
SOURCES += Main.cpp \
           QColorEdit.cpp \
           QVaultNode.cpp \
           QVaultNodeEdit.cpp \
           QVaultFolderNode.cpp \
           QVaultTextNode.cpp \
           QVaultSDLNode.cpp \
           QVaultChronicleNode.cpp \
           QVaultLinkNode.cpp \
           QVaultPlayerNode.cpp
RESOURCES += images.qrc
