#include "QAudioInterface.h"

#include <QGroupBox>
#include <QLabel>
#include <QGridLayout>
#include "../../QKeyDialog.h"
#include "../../Main.h"

QAudioInterface::QAudioInterface(plCreatable* pCre, QWidget* parent)
               : QCreatable(pCre, kAudioInterface, parent)
{
    plAudioInterface* intf = (plAudioInterface*)fCreatable;

    fOwnerLink = new QCreatableLink(this);
    fOwnerLink->setText(tr("Owner Object"));
    fOwnerLink->setKey(intf->getOwner());

    fSynchObjLink = new QCreatableLink(this, false);
    fSynchObjLink->setText(tr("Synch Flags"));
    fSynchObjLink->setCreatable(intf);
    fSynchObjLink->setForceType(kSynchedObject);

    QGroupBox* grpProps = new QGroupBox(tr("Properties"), this);
    QGridLayout* layProps = new QGridLayout(grpProps);
    layProps->setVerticalSpacing(0);
    fDisabled = new QCheckBox(tr("Disable"), grpProps);
    layProps->addWidget(fDisabled, 0, 0);
    fDisabled->setChecked(intf->getProperty(plAudioInterface::kDisable));

    fAudibleLink = new QCreatableLink(this);
    fAudibleLink->setText(intf->getAudible().Exists() ? intf->getAudible()->getName().cstr() : "(Null)");
    fAudibleLink->setKey(intf->getAudible());

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(fOwnerLink, 0, 0, 1, 2);
    layout->addWidget(fSynchObjLink, 1, 0, 1, 2);
    layout->addWidget(grpProps, 2, 0, 1, 2);
    layout->addWidget(new QLabel(tr("Audible Object: "), this), 3, 0, 1, 1);
    layout->addWidget(fAudibleLink, 3, 1, 1, 1);

    QObject::connect(fOwnerLink, SIGNAL(addObject()), this, SLOT(setOwner()));
    QObject::connect(fOwnerLink, SIGNAL(delObject()), this, SLOT(unsetOwner()));
    QObject::connect(fAudibleLink, SIGNAL(addObject()), this, SLOT(setAudible()));
    QObject::connect(fAudibleLink, SIGNAL(delObject()), this, SLOT(unsetAudible()));
}

void QAudioInterface::saveDamage()
{
    plAudioInterface* intf = (plAudioInterface*)fCreatable;
    intf->setProperty(plAudioInterface::kDisable, fDisabled->isChecked());
}

void QAudioInterface::setOwner()
{
    plAudioInterface* intf = (plAudioInterface*)fCreatable;
    QFindKeyDialog dlg(this);
    if (intf->getOwner().Exists())
        dlg.init(PrpShopMain::ResManager(), intf->getOwner());
    else
        dlg.init(PrpShopMain::ResManager(), intf->getKey()->getLocation(), kSceneObject);
    if (dlg.exec() == QDialog::Accepted) {
        intf->setOwner(dlg.selection());
        fOwnerLink->setKey(intf->getOwner());
    }
}

void QAudioInterface::unsetOwner()
{
    plAudioInterface* intf = (plAudioInterface*)fCreatable;
    intf->setOwner(plKey());
    fOwnerLink->setCreatable(NULL);
}

void QAudioInterface::setAudible()
{
    plAudioInterface* intf = (plAudioInterface*)fCreatable;
    QFindKeyDialog dlg(this);
    if (intf->getAudible().Exists())
        dlg.init(PrpShopMain::ResManager(), intf->getAudible());
    else
        dlg.init(PrpShopMain::ResManager(), intf->getKey()->getLocation());
    if (dlg.exec() == QDialog::Accepted) {
        intf->setAudible(dlg.selection());
        fAudibleLink->setKey(intf->getAudible());
        fAudibleLink->setText(intf->getAudible()->getName().cstr());
    }
}

void QAudioInterface::unsetAudible()
{
    plAudioInterface* intf = (plAudioInterface*)fCreatable;
    intf->setAudible(plKey());
    fAudibleLink->setCreatable(NULL);
    fAudibleLink->setText("(Null)");
}
