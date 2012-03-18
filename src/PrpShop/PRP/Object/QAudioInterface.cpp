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

#include "QAudioInterface.h"

#include <QGroupBox>
#include <QLabel>
#include <QGridLayout>
#include "../../QKeyDialog.h"
#include "../../Main.h"

QAudioInterface::QAudioInterface(plCreatable* pCre, QWidget* parent)
               : QCreatable(pCre, kAudioInterface, parent)
{
    plAudioInterface* intf = plAudioInterface::Convert(fCreatable);

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
    fAudibleLink->setText(intf->getAudible().Exists() ? ~intf->getAudible()->getName() : "(Null)");
    fAudibleLink->setKey(intf->getAudible());

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(fOwnerLink, 0, 0, 1, 2);
    layout->addWidget(fSynchObjLink, 1, 0, 1, 2);
    layout->addWidget(grpProps, 2, 0, 1, 2);
    layout->addWidget(new QLabel(tr("Audible Object: "), this), 3, 0, 1, 1);
    layout->addWidget(fAudibleLink, 3, 1, 1, 1);

    connect(fOwnerLink, SIGNAL(addObject()), this, SLOT(setOwner()));
    connect(fOwnerLink, SIGNAL(delObject()), this, SLOT(unsetOwner()));
    connect(fAudibleLink, SIGNAL(addObject()), this, SLOT(setAudible()));
    connect(fAudibleLink, SIGNAL(delObject()), this, SLOT(unsetAudible()));
}

void QAudioInterface::saveDamage()
{
    plAudioInterface* intf = plAudioInterface::Convert(fCreatable);
    intf->setProperty(plAudioInterface::kDisable, fDisabled->isChecked());
}

void QAudioInterface::setOwner()
{
    plAudioInterface* intf = plAudioInterface::Convert(fCreatable);
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
    plAudioInterface* intf = plAudioInterface::Convert(fCreatable);
    intf->setOwner(plKey());
    fOwnerLink->setCreatable(NULL);
}

void QAudioInterface::setAudible()
{
    plAudioInterface* intf = plAudioInterface::Convert(fCreatable);
    QFindKeyDialog dlg(this);
    if (intf->getAudible().Exists())
        dlg.init(PrpShopMain::ResManager(), intf->getAudible());
    else
        dlg.init(PrpShopMain::ResManager(), intf->getKey()->getLocation());
    if (dlg.exec() == QDialog::Accepted) {
        intf->setAudible(dlg.selection());
        fAudibleLink->setKey(intf->getAudible());
        fAudibleLink->setText(~intf->getAudible()->getName());
    }
}

void QAudioInterface::unsetAudible()
{
    plAudioInterface* intf = plAudioInterface::Convert(fCreatable);
    intf->setAudible(plKey());
    fAudibleLink->setCreatable(NULL);
    fAudibleLink->setText("(Null)");
}
