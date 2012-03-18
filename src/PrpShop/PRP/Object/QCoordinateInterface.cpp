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

#include "QCoordinateInterface.h"

#include <QGroupBox>
#include <QLabel>
#include <QTabWidget>
#include <QGridLayout>
#include "../../QKeyDialog.h"
#include "../../Main.h"

QCoordinateInterface::QCoordinateInterface(plCreatable* pCre, QWidget* parent)
                    : QCreatable(pCre, kCoordinateInterface, parent)
{
    plCoordinateInterface* intf = plCoordinateInterface::Convert(fCreatable);

    fOwnerLink = new QCreatableLink(this);
    fOwnerLink->setText(tr("Owner Object"));
    fOwnerLink->setKey(intf->getOwner());

    fSynchObjLink = new QCreatableLink(this, false);
    fSynchObjLink->setText(tr("Synch Flags"));
    fSynchObjLink->setCreatable(intf);
    fSynchObjLink->setForceType(kSynchedObject);

    QGroupBox* grpProps = new QGroupBox(tr("Coordinate Interface Properties"), this);
    QGridLayout* layProps = new QGridLayout(grpProps);
    layProps->setVerticalSpacing(0);
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

    fChildren = new QKeyList(intf->getKey(), this);
    for (size_t i=0; i<intf->getChildren().getSize(); i++)
        fChildren->addKey(intf->getChildren()[i]);

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

    connect(fOwnerLink, SIGNAL(addObject()), this, SLOT(setOwner()));
    connect(fOwnerLink, SIGNAL(delObject()), this, SLOT(unsetOwner()));
}

void QCoordinateInterface::saveDamage()
{
    plCoordinateInterface* intf = plCoordinateInterface::Convert(fCreatable);

    for (size_t i=0; i<plCoordinateInterface::kNumProps; i++)
        intf->setProperty(i, fCBProperties[i]->isChecked());

    intf->setLocalToParent(fLocalToParent->value());
    intf->setParentToLocal(fParentToLocal->value());
    intf->setLocalToWorld(fLocalToWorld->value());
    intf->setWorldToLocal(fWorldToLocal->value());

    intf->clearChildren();
    QList<plKey> children = fChildren->keys();
    while (!children.isEmpty())
        intf->addChild(children.takeFirst());
}

void QCoordinateInterface::setOwner()
{
    plCoordinateInterface* intf = plCoordinateInterface::Convert(fCreatable);
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

void QCoordinateInterface::unsetOwner()
{
    plCoordinateInterface* intf = plCoordinateInterface::Convert(fCreatable);
    intf->setOwner(plKey());
    fOwnerLink->setCreatable(NULL);
}
