# General Configuration
TEMPLATE = app
DEPENDPATH += .
INCLUDEPATH += . ../3rdParty
CONFIG += release
DESTDIR = ../../bin

# Use libPlasma
INCLUDEPATH += ../../../Plasma/core
LIBS += -L../../../Plasma/bin -lPlasma

# Use the QtIconLoader wrapper
win32|macx {
    HEADERS += ../3rdParty/qticonloader.h
} else {
    HEADERS += ../3rdParty/qticonloader.h
    SOURCES += ../3rdParty/qticonloader.cpp
}

# VaultShop Sources
HEADERS += Main.h \
           ../QPlasma.h \
           ../QColorEdit.h \
           ../QLinkLabel.h \
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
           ../QColorEdit.cpp \
           ../QLinkLabel.cpp \
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
