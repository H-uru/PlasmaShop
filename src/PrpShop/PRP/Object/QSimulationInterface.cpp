/* This file is part of PlasmaShop.
 *
 * PlasmaShop is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PlasmaShop is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PlasmaShop.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "QSimulationInterface.h"

#include <QGroupBox>
#include <QLabel>
#include <QGridLayout>
#include "QKeyDialog.h"
#include "Main.h"

QSimulationInterface::QSimulationInterface(plCreatable* pCre, QWidget* parent)
                    : QCreatable(pCre, kSimulationInterface, parent)
{
    plSimulationInterface* intf = plSimulationInterface::Convert(fCreatable);

    fOwnerLink = new QCreatableLink(this);
    fOwnerLink->setKey(intf->getOwner(), false);
    fOwnerLink->setText(tr("Owner Object"));

    fSynchObjLink = new QCreatableLink(this, false);
    fSynchObjLink->setCreatable(intf, tr("Synch Flags"));
    fSynchObjLink->setForceType(kSynchedObject);

    QGroupBox* grpProps = new QGroupBox(tr("Properties"), this);
    QGridLayout* layProps = new QGridLayout(grpProps);
    layProps->setVerticalSpacing(0);
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
    fPhysicalLink->setKey(intf->getPhysical());

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(fOwnerLink, 0, 0, 1, 2);
    layout->addWidget(fSynchObjLink, 1, 0, 1, 2);
    layout->addWidget(grpProps, 2, 0, 1, 2);
    layout->addWidget(new QLabel(tr("Physical Object: "), this), 3, 0, 1, 1);
    layout->addWidget(fPhysicalLink, 3, 1, 1, 1);

    connect(fOwnerLink, SIGNAL(addObject()), this, SLOT(setOwner()));
    connect(fOwnerLink, SIGNAL(delObject()), this, SLOT(unsetOwner()));
    connect(fPhysicalLink, SIGNAL(addObject()), this, SLOT(setPhysical()));
    connect(fPhysicalLink, SIGNAL(delObject()), this, SLOT(unsetPhysical()));
}

void QSimulationInterface::saveDamage()
{
    plSimulationInterface* intf = plSimulationInterface::Convert(fCreatable);
    for (size_t i=0; i<plSimulationInterface::kNumProps; i++)
        intf->setProperty(i, fCBProperties[i]->isChecked());
}

void QSimulationInterface::setOwner()
{
    plSimulationInterface* intf = plSimulationInterface::Convert(fCreatable);
    QFindKeyDialog dlg(this);
    if (intf->getOwner().Exists())
        dlg.init(PrpShopMain::ResManager(), intf->getOwner());
    else
        dlg.init(PrpShopMain::ResManager(), intf->getKey()->getLocation(), kSceneObject);
    if (dlg.exec() == QDialog::Accepted) {
        intf->setOwner(dlg.selection());
        fOwnerLink->setKey(intf->getOwner(), false);
    }
}

void QSimulationInterface::unsetOwner()
{
    plSimulationInterface* intf = plSimulationInterface::Convert(fCreatable);
    intf->setOwner(plKey());
    fOwnerLink->setCreatable(NULL);
}

void QSimulationInterface::setPhysical()
{
    plSimulationInterface* intf = plSimulationInterface::Convert(fCreatable);
    QFindKeyDialog dlg(this);
    if (intf->getPhysical().Exists())
        dlg.init(PrpShopMain::ResManager(), intf->getPhysical());
    else
        dlg.init(PrpShopMain::ResManager(), intf->getKey()->getLocation(), kGenericPhysical);
    if (dlg.exec() == QDialog::Accepted) {
        intf->setPhysical(dlg.selection());
        fPhysicalLink->setKey(intf->getPhysical());
    }
}

void QSimulationInterface::unsetPhysical()
{
    plSimulationInterface* intf = plSimulationInterface::Convert(fCreatable);
    intf->setPhysical(plKey());
    fPhysicalLink->setCreatable(NULL, "(None)");
}
