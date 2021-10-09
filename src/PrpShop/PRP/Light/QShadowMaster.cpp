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

#include "QShadowMaster.h"

#include <limits>
#include <QGroupBox>
#include <QLabel>
#include <QGridLayout>
#include "QKeyDialog.h"
#include "Main.h"

QShadowMaster::QShadowMaster(plCreatable* pCre, QWidget* parent)
    : QCreatable(pCre, pCre->ClassIndex(), parent)
{
    plShadowMaster* obj = plShadowMaster::Convert(fCreatable);

    fOwnerLink = new QCreatableLink(this);
    fOwnerLink->setKey(obj->getOwner(), false);
    fOwnerLink->setText(tr("Owner Object"));

    fSynchObjLink = new QCreatableLink(this, false);
    fSynchObjLink->setCreatable(obj, tr("Synch Flags"));
    fSynchObjLink->setForceType(kSynchedObject);

    QGroupBox* grpProps = new QGroupBox(tr("Properties"), this);
    QGridLayout* layProps = new QGridLayout(grpProps);
    layProps->setVerticalSpacing(0);
    fProperties[plShadowMaster::kDisable] = new QCheckBox(tr("Disable"), grpProps);
    fProperties[plShadowMaster::kSelfShadow] = new QCheckBox(tr("Self Shadow"), grpProps);
    layProps->addWidget(fProperties[plShadowMaster::kDisable], 0, 0);
    layProps->addWidget(fProperties[plShadowMaster::kSelfShadow], 0, 1);

    for (size_t i = 0; i < plShadowMaster::kNumProps; ++i) {
        fProperties[i]->setChecked(obj->getProperty(i));
        connect(fProperties[i], &QCheckBox::clicked, this, [this, i](bool checked) {
            plShadowMaster* obj = plShadowMaster::Convert(fCreatable);
            obj->setProperty(i, checked);
        });
    }

    fMinDist = new QFloatEdit(this);
    fMinDist->setValue(obj->getMinDist());

    fMaxDist = new QFloatEdit(this);
    fMaxDist->setValue(obj->getMaxDist());

    fMinSize = new QIntEdit(this);
    fMinSize->setRange(0, 0x7FFFFFFF);
    fMinSize->setValue(obj->getMinSize());

    fMaxSize = new QIntEdit(this);
    fMaxSize->setRange(0, 0x7FFFFFFF);
    fMaxSize->setValue(obj->getMaxSize());

    fAttenDist = new QFloatEdit(this);
    fAttenDist->setValue(obj->getAttenDist());

    fPower = new QFloatEdit(this);
    fPower->setValue(obj->getPower());

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(fOwnerLink, 0, 0, 1, 3);
    layout->addWidget(fSynchObjLink, 1, 0, 1, 3);
    layout->addWidget(grpProps, 2, 0, 1, 3);
    layout->addWidget(new QLabel(tr("Min"), this), 3, 1);
    layout->addWidget(new QLabel(tr("Max"), this), 3, 2);
    layout->addWidget(new QLabel(tr("Distance:"), this), 4, 0);
    layout->addWidget(fMinDist, 4, 1);
    layout->addWidget(fMaxDist, 4, 2);
    layout->addWidget(new QLabel(tr("Size:"), this), 5, 0);
    layout->addWidget(fMinSize, 5, 1);
    layout->addWidget(fMaxSize, 5, 2);
    layout->addWidget(new QLabel(tr("Attensity Dist:"), this), 6, 0);
    layout->addWidget(fAttenDist, 6, 1, 1, 2);
    layout->addWidget(new QLabel(tr("Power:"), this), 7, 0);
    layout->addWidget(fPower, 7, 1, 1, 2);

    connect(fOwnerLink, &QCreatableLink::addObject, this, &QShadowMaster::setOwner);
    connect(fOwnerLink, &QCreatableLink::delObject, this, &QShadowMaster::unsetOwner);
}

void QShadowMaster::saveDamage()
{
    plShadowMaster* obj = plShadowMaster::Convert(fCreatable);

    obj->setAttenDist(fAttenDist->value());
    obj->setDist(fMinDist->value(), fMaxDist->value());
    obj->setSize(fMinSize->value(), fMaxSize->value());
    obj->setPower(fPower->value());
}

void QShadowMaster::setOwner()
{
    plShadowMaster* obj = plShadowMaster::Convert(fCreatable);
    QFindKeyDialog dlg(this);
    if (obj->getOwner().Exists())
        dlg.init(PrpShopMain::ResManager(), obj->getOwner());
    else
        dlg.init(PrpShopMain::ResManager(), obj->getKey()->getLocation(), kSceneObject);
    if (dlg.exec() == QDialog::Accepted) {
        obj->setOwner(dlg.selection());
        fOwnerLink->setKey(obj->getOwner(), false);
    }
}

void QShadowMaster::unsetOwner()
{
    plShadowMaster* obj = plShadowMaster::Convert(fCreatable);
    obj->setOwner(plKey());
    fOwnerLink->setCreatable(NULL);
}
