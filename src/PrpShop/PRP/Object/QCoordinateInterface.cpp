#include "QCoordinateInterface.h"

#include <QGroupBox>
#include <QLabel>
#include <QTabWidget>
#include <QGridLayout>

QCoordinateInterface::QCoordinateInterface(plCreatable* pCre, QWidget* parent)
                    : QCreatable(pCre, kCoordinateInterface, parent)
{
    plCoordinateInterface* intf = (plCoordinateInterface*)fCreatable;

    fOwnerLink = new QCreatableLink(this);
    fOwnerLink->setText(tr("Owner Object"));
    fOwnerLink->setKey(intf->getOwner());

    fSynchObjLink = new QCreatableLink(this, false);
    fSynchObjLink->setText(tr("Synch Flags"));
    fSynchObjLink->setCreatable(intf);
    fSynchObjLink->setForceType(kSynchedObject);

    QGroupBox* grpProps = new QGroupBox(tr("Coordinate Interface Properties"), this);
    QGridLayout* layProps = new QGridLayout(grpProps);
    layProps->setVerticalSpacing(2);
    fCBProperties[plCoordinateInterface::kDisable] = new QCheckBox(tr("Disable"), grpProps);
    fCBProperties[plCoordinateInterface::kCanEverDelayTransform] = new QCheckBox(tr("Can Ever Delay Transform"), grpProps);
    fCBProperties[plCoordinateInterface::kDelayedTransformEval] = new QCheckBox(tr("Delayed Transform Evaluation"), grpProps);
    layProps->addWidget(fCBProperties[plCoordinateInterface::kDisable], 0, 0);
    layProps->addWidget(fCBProperties[plCoordinateInterface::kCanEverDelayTransform], 1, 0);
    layProps->addWidget(fCBProperties[plCoordinateInterface::kDelayedTransformEval], 2, 0);
    for (size_t i=0; i<plCoordinateInterface::kNumProps; i++)
        fCBProperties[i]->setChecked(intf->getProperty(i));

    QTabWidget* xformTab = new QTabWidget(this);
    fLocalToParent = new QMatrix44(xformTab);
    fParentToLocal = new QMatrix44(xformTab);
    fLocalToWorld = new QMatrix44(xformTab);
    fWorldToLocal = new QMatrix44(xformTab);
    fLocalToParent->setValue(intf->getLocalToParent());
    fParentToLocal->setValue(intf->getParentToLocal());
    fLocalToWorld->setValue(intf->getLocalToWorld());
    fWorldToLocal->setValue(intf->getWorldToLocal());
    xformTab->addTab(fLocalToParent, tr("Local to Parent"));
    xformTab->addTab(fParentToLocal, tr("Parent to Local"));
    xformTab->addTab(fLocalToWorld, tr("Local to World"));
    xformTab->addTab(fWorldToLocal, tr("World to Local"));

    fChildren = new QKeyList(this);
    for (size_t i=0; i<intf->getNumChildren(); i++)
        fChildren->addKey(intf->getChild(i));

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(fOwnerLink, 0, 0);
    layout->addWidget(fSynchObjLink, 1, 0);
    layout->addWidget(grpProps, 2, 0);
    layout->addWidget(new QLabel(tr("Transforms:"), this), 3, 0);
    layout->addWidget(xformTab, 4, 0);
    layout->addWidget(new QLabel(tr("Children:"), this), 5, 0);
    layout->addWidget(fChildren, 6, 0);
    fChildren->adjustColumns();

    QObject::connect(fOwnerLink, SIGNAL(addObject()), this, SLOT(setOwner()));
    QObject::connect(fOwnerLink, SIGNAL(delObject()), this, SLOT(unsetOwner()));
}

void QCoordinateInterface::saveDamage()
{
    plCoordinateInterface* intf = (plCoordinateInterface*)fCreatable;

    for (size_t i=0; i<plCoordinateInterface::kNumProps; i++)
        intf->setProperty(i, fCBProperties[i]->isChecked());

    intf->getLocalToParent() = fLocalToParent->value();
    intf->getParentToLocal() = fParentToLocal->value();
    intf->getLocalToWorld() = fLocalToWorld->value();
    intf->getWorldToLocal() = fWorldToLocal->value();

    intf->clearChildren();
    QList<plKey> children = fChildren->keys();
    while (!children.isEmpty())
        intf->addChild(children.takeFirst());
}

void QCoordinateInterface::setOwner()
{
    //...
}

void QCoordinateInterface::unsetOwner()
{
    plCoordinateInterface* intf = (plCoordinateInterface*)fCreatable;
    intf->setOwner(plKey());
    fOwnerLink->setCreatable(NULL);
}
