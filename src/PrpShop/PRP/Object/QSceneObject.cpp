#include "QSceneObject.h"

#include <QGroupBox>
#include <QTabWidget>
#include <QGridLayout>
#include "../QPlasmaUtils.h"

QSceneObject::QSceneObject(plCreatable* pCre, QWidget* parent)
            : QCreatable(pCre, kSceneObject, parent)
{
    plSceneObject* obj = (plSceneObject*)fCreatable;

    fSynchObjLink = new QCreatableLink(this, false);
    fSynchObjLink->setText(tr("Synch Flags"));
    fSynchObjLink->setCreatable(obj);
    fSynchObjLink->setForceType(kSynchedObject);

    QGroupBox* grpInterfaces = new QGroupBox(tr("Core Interfaces"), this);
    QGridLayout* layInterfaces = new QGridLayout(grpInterfaces);
    layInterfaces->setVerticalSpacing(2);
    fDrawIntfLink = new QCreatableLink(grpInterfaces);
    fDrawIntfLink->setText(pqGetFriendlyClassName(kDrawInterface));
    fDrawIntfLink->setKey(obj->getDrawInterface());
    fSimIntfLink = new QCreatableLink(grpInterfaces);
    fSimIntfLink->setText(pqGetFriendlyClassName(kSimulationInterface));
    fSimIntfLink->setKey(obj->getSimInterface());
    fCoordIntfLink = new QCreatableLink(grpInterfaces);
    fCoordIntfLink->setText(pqGetFriendlyClassName(kCoordinateInterface));
    fCoordIntfLink->setKey(obj->getCoordInterface());
    fAudioIntfLink = new QCreatableLink(grpInterfaces);
    fAudioIntfLink->setText(pqGetFriendlyClassName(kAudioInterface));
    fAudioIntfLink->setKey(obj->getAudioInterface());
    layInterfaces->addWidget(fDrawIntfLink, 0, 0);
    layInterfaces->addWidget(fSimIntfLink, 1, 0);
    layInterfaces->addWidget(fCoordIntfLink, 2, 0);
    layInterfaces->addWidget(fAudioIntfLink, 3, 0);

    QTabWidget* objTab = new QTabWidget(this);
    fInterfaceList = new QKeyList(objTab);
    fModifierList = new QKeyList(objTab);
    for (size_t i=0; i<obj->getNumInterfaces(); i++)
        fInterfaceList->addKey(obj->getInterface(i));
    for (size_t i=0; i<obj->getNumModifiers(); i++)
        fModifierList->addKey(obj->getModifier(i));
    objTab->addTab(fInterfaceList, tr("Interfaces"));
    objTab->addTab(fModifierList, tr("Modifiers"));

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(4, 4, 4, 4);
    layout->addWidget(fSynchObjLink, 0, 0);
    layout->addWidget(grpInterfaces, 1, 0);
    layout->addWidget(objTab, 2, 0);
    fInterfaceList->adjustColumns();
    fModifierList->adjustColumns();

    QObject::connect(fDrawIntfLink, SIGNAL(addObject()), this, SLOT(setDraw()));
    QObject::connect(fDrawIntfLink, SIGNAL(delObject()), this, SLOT(unsetDraw()));
    QObject::connect(fSimIntfLink, SIGNAL(addObject()), this, SLOT(setSim()));
    QObject::connect(fSimIntfLink, SIGNAL(delObject()), this, SLOT(unsetSim()));
    QObject::connect(fCoordIntfLink, SIGNAL(addObject()), this, SLOT(setCoord()));
    QObject::connect(fCoordIntfLink, SIGNAL(delObject()), this, SLOT(unsetCoord()));
    QObject::connect(fAudioIntfLink, SIGNAL(addObject()), this, SLOT(setAudio()));
    QObject::connect(fAudioIntfLink, SIGNAL(delObject()), this, SLOT(unsetAudio()));
}

void QSceneObject::saveDamage()
{
    plSceneObject* obj = (plSceneObject*)fCreatable;

    obj->clearInterfaces();
    QList<plKey> interfaces = fInterfaceList->keys();
    while (!interfaces.isEmpty())
        obj->addInterface(interfaces.takeFirst());

    obj->clearModifiers();
    QList<plKey> modifiers = fModifierList->keys();
    while (!modifiers.isEmpty())
        obj->addModifier(modifiers.takeFirst());
}

void QSceneObject::setDraw()
{
    // ...
}

void QSceneObject::setSim()
{
    // ...
}

void QSceneObject::setCoord()
{
    // ...
}

void QSceneObject::setAudio()
{
    // ...
}

void QSceneObject::unsetDraw()
{
    plSceneObject* obj = (plSceneObject*)fCreatable;
    obj->setDrawInterface(plKey());
    fDrawIntfLink->setCreatable(NULL);
}

void QSceneObject::unsetSim()
{
    plSceneObject* obj = (plSceneObject*)fCreatable;
    obj->setSimInterface(plKey());
    fSimIntfLink->setCreatable(NULL);
}

void QSceneObject::unsetCoord()
{
    plSceneObject* obj = (plSceneObject*)fCreatable;
    obj->setCoordInterface(plKey());
    fCoordIntfLink->setCreatable(NULL);
}

void QSceneObject::unsetAudio()
{
    plSceneObject* obj = (plSceneObject*)fCreatable;
    obj->setAudioInterface(plKey());
    fAudioIntfLink->setCreatable(NULL);
}
