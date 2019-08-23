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

#include "QCollisionDetector.h"

#include <QLabel>
#include <QGroupBox>
#include <QGridLayout>
#include "QKeyDialog.h"
#include "Main.h"

QCollisionDetector::QCollisionDetector(plCreatable* pCre, QWidget* parent)
    : QCreatable(pCre, pCre->ClassIndex(), parent)
{
    plCollisionDetector* obj = plCollisionDetector::Convert(fCreatable);

    fDetectorLink = new QCreatableLink(this, false);
    fDetectorLink->setCreatable(obj, tr("Detector Properties"));
    fDetectorLink->setForceType(kDetectorModifier);

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(fDetectorLink, 0, 0, 1, 2);

    if (!obj->ClassInstance(kSubworldRegionDetector)) {
        // Subworld Region Detector skips the Type property
        QGroupBox* grpType = new QGroupBox(tr("Type"), this);
        QGridLayout* layType = new QGridLayout(grpType);
        layType->setVerticalSpacing(0);
        layType->setHorizontalSpacing(8);
        fTypeFlags[kCBTypeEnter] = new QBitmaskCheckBox(plCollisionDetector::kTypeEnter,
                                                        tr("Enter"), grpType);
        fTypeFlags[kCBTypeExit] = new QBitmaskCheckBox(plCollisionDetector::kTypeExit,
                                                       tr("Exit"), grpType);
        fTypeFlags[kCBTypeAny] = new QBitmaskCheckBox(plCollisionDetector::kTypeAny,
                                                      tr("Any"), grpType);
        fTypeFlags[kCBTypeUnEnter] = new QBitmaskCheckBox(plCollisionDetector::kTypeUnEnter,
                                                          tr("Un-Enter"), grpType);
        fTypeFlags[kCBTypeUnExit] = new QBitmaskCheckBox(plCollisionDetector::kTypeUnExit,
                                                         tr("Un-Exit"), grpType);
        fTypeFlags[kCBTypeBump] = new QBitmaskCheckBox(plCollisionDetector::kTypeBump,
                                                       tr("Bump"), grpType);
        layType->addWidget(fTypeFlags[kCBTypeEnter], 0, 0);
        layType->addWidget(fTypeFlags[kCBTypeExit], 0, 1);
        layType->addWidget(fTypeFlags[kCBTypeAny], 0, 2);
        layType->addWidget(fTypeFlags[kCBTypeUnEnter], 1, 0);
        layType->addWidget(fTypeFlags[kCBTypeUnExit], 1, 1);
        layType->addWidget(fTypeFlags[kCBTypeBump], 1, 2);
        layout->addWidget(grpType, 1, 0, 1, 2);

        for (auto cb : fTypeFlags) {
            cb->setFrom(obj->getType());
            connect(cb, &QBitmaskCheckBox::setBits, this, [this](unsigned int mask) {
                plCollisionDetector* obj = plCollisionDetector::Convert(fCreatable);
                obj->setType(obj->getType() | mask);
            });
            connect(cb, &QBitmaskCheckBox::unsetBits, this, [this](unsigned int mask) {
                plCollisionDetector* obj = plCollisionDetector::Convert(fCreatable);
                obj->setType(obj->getType() & ~mask);
            });
        }
    }

    if (obj->ClassInstance(kSubworldRegionDetector)) {
        plSubworldRegionDetector* subDet = plSubworldRegionDetector::Convert(fCreatable);

        fSubworld = new QCreatableLink(this);
        fSubworld->setKey(subDet->getSubworld());
        fBoolParam = new QCheckBox(tr("On Exit"), this);
        fBoolParam->setChecked(subDet->getOnExit());

        layout->addWidget(new QLabel(tr("Subworld:"), this), 1, 0, 1, 1);
        layout->addWidget(fSubworld, 1, 1, 1, 1);
        layout->addWidget(fBoolParam, 2, 0, 1, 2);

        connect(fSubworld, SIGNAL(addObject()), this, SLOT(setSubworld()));
        connect(fSubworld, SIGNAL(delObject()), this, SLOT(unsetSubworld()));
    }

    if (obj->ClassInstance(kPanicLinkRegion)) {
        plPanicLinkRegion* plRgn = plPanicLinkRegion::Convert(fCreatable);
        fBoolParam = new QCheckBox(tr("Play Link-Out Animation"), this);
        fBoolParam->setChecked(plRgn->getPlayLinkOutAnim());
        layout->addWidget(fBoolParam, 2, 0, 1, 2);
    }
}

void QCollisionDetector::saveDamage()
{
    plCollisionDetector* obj = plCollisionDetector::Convert(fCreatable);

    if (obj->ClassInstance(kSubworldRegionDetector)) {
        plSubworldRegionDetector* subDet = plSubworldRegionDetector::Convert(fCreatable);
        subDet->setOnExit(fBoolParam->isChecked());
    }

    if (obj->ClassInstance(kPanicLinkRegion)) {
        plPanicLinkRegion* plRgn = plPanicLinkRegion::Convert(fCreatable);
        plRgn->setPlayLinkOutAnim(fBoolParam->isChecked());
    }
}

void QCollisionDetector::setSubworld()
{
    plSubworldRegionDetector* obj = plSubworldRegionDetector::Convert(fCreatable);
    QFindKeyDialog dlg(this);
    if (obj->getSubworld().Exists())
        dlg.init(PrpShopMain::ResManager(), obj->getSubworld());
    else
        dlg.init(PrpShopMain::ResManager(), obj->getKey()->getLocation(), kSceneObject);
    if (dlg.exec() == QDialog::Accepted) {
        obj->setSubworld(dlg.selection());
        fSubworld->setKey(obj->getSubworld());
    }
}

void QCollisionDetector::unsetSubworld()
{
    plSubworldRegionDetector* obj = plSubworldRegionDetector::Convert(fCreatable);
    obj->setSubworld(plKey());
    fSubworld->setCreatable(NULL, "(None)");
}
