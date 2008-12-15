#include "QSimulationInterface.h"

#include <QGroupBox>
#include <QLabel>
#include <QGridLayout>

QSimulationInterface::QSimulationInterface(plCreatable* pCre, QWidget* parent)
                    : QCreatable(pCre, kSimulationInterface, parent)
{
    plSimulationInterface* intf = (plSimulationInterface*)fCreatable;

    fOwnerLink = new QCreatableLink(this);
    fOwnerLink->setText(tr("Owner Object"));
    fOwnerLink->setKey(intf->getOwner());

    fSynchObjLink = new QCreatableLink(this, false);
    fSynchObjLink->setText(tr("Synch Flags"));
    fSynchObjLink->setCreatable(intf);
    fSynchObjLink->setForceType(kSynchedObject);

    QGroupBox* grpProps = new QGroupBox(tr("Properties"), this);
    QGridLayout* layProps = new QGridLayout(grpProps);
    layProps->setVerticalSpacing(2);
    fCBProperties[plSimulationInterface::kDisable] = new QCheckBox(tr("Disable"), grpProps);
    fCBProperties[plSimulationInterface::kWeightless] = new QCheckBox(tr("Weightless"), grpProps);
    fCBProperties[plSimulationInterface::kPinned] = new QCheckBox(tr("Pinned"), grpProps);
    fCBProperties[plSimulationInterface::kWarp] = new QCheckBox(tr("Warp"), grpProps);
    fCBProperties[plSimulationInterface::kUpright] = new QCheckBox(tr("Upright"), grpProps);
    fCBProperties[plSimulationInterface::kPassive] = new QCheckBox(tr("Passive"), grpProps);
    fCBProperties[plSimulationInterface::kRotationForces] = new QCheckBox(tr("Rotation Forces"), grpProps);
    fCBProperties[plSimulationInterface::kCameraAvoidObject] = new QCheckBox(tr("Camera Avoids Object"), grpProps);
    fCBProperties[plSimulationInterface::kPhysAnim] = new QCheckBox(tr("Physical Animation"), grpProps);
    fCBProperties[plSimulationInterface::kStartInactive] = new QCheckBox(tr("Start Inactive"), grpProps);
    fCBProperties[plSimulationInterface::kNoSynchronize] = new QCheckBox(tr("No Synchronize"), grpProps);
    fCBProperties[plSimulationInterface::kSuppressed] = new QCheckBox(tr("Suppressed"), grpProps);
    fCBProperties[plSimulationInterface::kNoOwnershipChange] = new QCheckBox(tr("No Ownership Change"), grpProps);
    fCBProperties[plSimulationInterface::kAvAnimPushable] = new QCheckBox(tr("Avatar Anim Pushable"), grpProps);
    layProps->addWidget(fCBProperties[plSimulationInterface::kDisable], 0, 0);
    layProps->addWidget(fCBProperties[plSimulationInterface::kRotationForces], 0, 1);
    layProps->addWidget(fCBProperties[plSimulationInterface::kWeightless], 1, 0);
    layProps->addWidget(fCBProperties[plSimulationInterface::kCameraAvoidObject], 1, 1);
    layProps->addWidget(fCBProperties[plSimulationInterface::kPinned], 2, 0);
    layProps->addWidget(fCBProperties[plSimulationInterface::kPhysAnim], 2, 1);
    layProps->addWidget(fCBProperties[plSimulationInterface::kWarp], 3, 0);
    layProps->addWidget(fCBProperties[plSimulationInterface::kStartInactive], 3, 1);
    layProps->addWidget(fCBProperties[plSimulationInterface::kUpright], 4, 0);
    layProps->addWidget(fCBProperties[plSimulationInterface::kNoSynchronize], 4, 1);
    layProps->addWidget(fCBProperties[plSimulationInterface::kPassive], 5, 0);
    layProps->addWidget(fCBProperties[plSimulationInterface::kNoOwnershipChange], 5, 1);
    layProps->addWidget(fCBProperties[plSimulationInterface::kSuppressed], 6, 0);
    layProps->addWidget(fCBProperties[plSimulationInterface::kAvAnimPushable], 6, 1);
    for (size_t i=0; i<plSimulationInterface::kNumProps; i++)
        fCBProperties[i]->setChecked(intf->getProperty(i));

    fPhysicalLink = new QCreatableLink(this);
    fPhysicalLink->setText(intf->getPhysical().Exists() ? intf->getPhysical()->getName().cstr() : "(Null)");
    fPhysicalLink->setKey(intf->getPhysical());

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(fOwnerLink, 0, 0, 1, 2);
    layout->addWidget(fSynchObjLink, 1, 0, 1, 2);
    layout->addWidget(grpProps, 2, 0, 1, 2);
    layout->addWidget(new QLabel(tr("Physical Object: "), this), 3, 0, 1, 1);
    layout->addWidget(fPhysicalLink, 3, 1, 1, 1);

    QObject::connect(fOwnerLink, SIGNAL(addObject()), this, SLOT(setOwner()));
    QObject::connect(fOwnerLink, SIGNAL(delObject()), this, SLOT(unsetOwner()));
    QObject::connect(fPhysicalLink, SIGNAL(addObject()), this, SLOT(setPhysical()));
    QObject::connect(fPhysicalLink, SIGNAL(delObject()), this, SLOT(unsetPhysical()));
}

void QSimulationInterface::saveDamage()
{
    plSimulationInterface* intf = (plSimulationInterface*)fCreatable;
    for (size_t i=0; i<plSimulationInterface::kNumProps; i++)
        intf->setProperty(i, fCBProperties[i]->isChecked());
}

void QSimulationInterface::setOwner()
{
    //...
}

void QSimulationInterface::unsetOwner()
{
    plSimulationInterface* intf = (plSimulationInterface*)fCreatable;
    intf->setOwner(plKey());
    fOwnerLink->setCreatable(NULL);
}

void QSimulationInterface::setPhysical()
{
    //...
}

void QSimulationInterface::unsetPhysical()
{
    plSimulationInterface* intf = (plSimulationInterface*)fCreatable;
    intf->setPhysical(plKey());
    fPhysicalLink->setCreatable(NULL);
    fPhysicalLink->setText("(Null)");
}
