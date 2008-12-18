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
           QKeyDialog.h \
           QPlasmaUtils.h \
           QPlasmaTreeItem.h \
           PRP/QColorEdit.h \
           PRP/QCreatable.h \
           PRP/QKeyList.h \
           PRP/QMatrix44.h \
           PRP/QObjLink.h \
           PRP/Modifier/QPythonFileMod.h \
           PRP/Object/QAudioInterface.h \
           PRP/Object/QCoordinateInterface.h \
           PRP/Object/QSceneObject.h \
           PRP/Object/QSimulationInterface.h \
           PRP/Object/QSynchedObject.h \
           PRP/Surface/QLayer.h \
           PRP/Surface/QMaterial.h
SOURCES += Main.cpp \
           QKeyDialog.cpp \
           QPlasmaUtils.cpp \
           QPlasmaTreeItem.cpp \
           PRP/QColorEdit.cpp \
           PRP/QCreatable.cpp \
           PRP/QKeyList.cpp \
           PRP/QMatrix44.cpp \
           PRP/QObjLink.cpp \
           PRP/Modifier/QPythonFileMod.cpp \
           PRP/Object/QAudioInterface.cpp \
           PRP/Object/QCoordinateInterface.cpp \
           PRP/Object/QSceneObject.cpp \
           PRP/Object/QSimulationInterface.cpp \
           PRP/Object/QSynchedObject.cpp \
           PRP/Surface/QLayer.cpp \
           PRP/Surface/QMaterial.cpp
RESOURCES += images.qrc
