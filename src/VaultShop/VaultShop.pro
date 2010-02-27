PLSLIBS = core
include(../../config)

# General Configuration
TEMPLATE = app
DEPENDPATH += .
INCLUDEPATH += . ../3rdParty
CONFIG += release
DESTDIR = ../../bin$${PLAT_BITS}

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
}

# VaultShop Sources
HEADERS += Main.h \
           ../QPlasma.h \
           ../QColorEdit.h \
           ../QLinkLabel.h \
           QGameServerState.h \
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
           QVaultAgeInfoNode.h \
           QVaultImageNode.h
SOURCES += Main.cpp \
           ../QColorEdit.cpp \
           ../QLinkLabel.cpp \
           QGameServerState.cpp \
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
           QVaultAgeInfoNode.cpp \
           QVaultImageNode.cpp
RESOURCES += images.qrc
