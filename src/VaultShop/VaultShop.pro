# General Configuration
TEMPLATE = app
DEPENDPATH += .
INCLUDEPATH += .
DESTDIR = ../../bin

# Use libPlasma
INCLUDEPATH += ../../../Plasma/core
LIBS += -L../../../Plasma/bin -lPlasma

# VaultShop Sources
HEADERS += Main.h \
           QColorEdit.h \
           QLinkLabel.h \
           QVaultNode.h \
           QVaultNodeEdit.h \
           QVaultFolderNode.h \
           QVaultTextNode.h \
           QVaultSDLNode.h \
           QVaultChronicleNode.h \
           QVaultLinkNode.h \
           QVaultPlayerNode.h \
           QVaultPlayerInfoNode.h \
           QVaultAgeNode.h \
           QVaultAgeInfoNode.h
SOURCES += Main.cpp \
           QColorEdit.cpp \
           QLinkLabel.cpp \
           QVaultNode.cpp \
           QVaultNodeEdit.cpp \
           QVaultFolderNode.cpp \
           QVaultTextNode.cpp \
           QVaultSDLNode.cpp \
           QVaultChronicleNode.cpp \
           QVaultLinkNode.cpp \
           QVaultPlayerNode.cpp \
           QVaultPlayerInfoNode.cpp \
           QVaultAgeNode.cpp \
           QVaultAgeInfoNode.cpp
RESOURCES += images.qrc
