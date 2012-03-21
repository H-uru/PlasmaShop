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
#include "../../QKeyDialog.h"
#include "../../Main.h"

QCollisionDetector::QCollisionDetector(plCreatable* pCre, QWidget* parent)
                  : QCreatable(pCre, pCre->ClassIndex(), parent)
{
    plCollisionDetector* obj = plCollisionDetector::Convert(fCreatable);

    fDetectorLink = new QCreatableLink(this, false);
    fDetectorLink->setText(tr("Detector Properties"));
    fDetectorLink->setCreatable(obj);
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
        fTypeFlags[kCBTypeEnter] = new QCheckBox(tr("Enter"), grpType);
        fTypeFlags[kCBTypeExit] = new QCheckBox(tr("Exit"), grpType);
        fTypeFlags[kCBTypeAny] = new QCheckBox(tr("Any"), grpType);
        fTypeFlags[kCBTypeUnEnter] = new QCheckBox(tr("Un-Enter"), grpType);
        fTypeFlags[kCBTypeUnExit] = new QCheckBox(tr("Un-Exit"), grpType);
        fTypeFlags[kCBTypeBump] = new QCheckBox(tr("Bump"), grpType);
        layType->addWidget(fTypeFlags[kCBTypeEnter], 0, 0);
        layType->addWidget(fTypeFlags[kCBTypeExit], 0, 1);
        layType->addWidget(fTypeFlags[kCBTypeAny], 0, 2);
        layType->addWidget(fTypeFlags[kCBTypeUnEnter], 1, 0);
        layType->addWidget(fTypeFlags[kCBTypeUnExit], 1, 1);
        layType->addWidget(fTypeFlags[kCBTypeBump], 1, 2);
        fTypeFlags[kCBTypeEnter]->setChecked(obj->getType() & plCollisionDetector::kTypeEnter);
        fTypeFlags[kCBTypeExit]->setChecked(obj->getType() & plCollisionDetector::kTypeExit);
        fTypeFlags[kCBTypeAny]->setChecked(obj->getType() & plCollisionDetector::kTypeAny);
        fTypeFlags[kCBTypeUnEnter]->setChecked(obj->getType() & plCollisionDetector::kTypeUnEnter);
        fTypeFlags[kCBTypeUnExit]->setChecked(obj->getType() & plCollisionDetector::kTypeUnExit);
        fTypeFlags[kCBTypeBump]->setChecked(obj->getType() & plCollisionDetector::kTypeBump);
        layout->addWidget(grpType, 1, 0, 1, 2);
    }

    if (obj->ClassInstance(kSubworldRegionDetector)) {
        plSubworldRegionDetector* subDet = plSubworldRegionDetector::Convert(fCreatable);

        fSubworld = new QCreatableLink(this);
        fSubworld->setKey(subDet->getSubworld());
        fSubworld->setText(subDet->getSubworld().Exists()
                           ? ~subDet->getSubworld()->getName()
                           : "(None)");
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

    if (!obj->ClassInstance(kSubworldRegionDetector))
        obj->setType((fTypeFlags[kCBTypeEnter]->isChecked() ? plCollisionDetector::kTypeEnter : 0)
                   | (fTypeFlags[kCBTypeExit]->isChecked() ? plCollisionDetector::kTypeExit : 0)
                   | (fTypeFlags[kCBTypeAny]->isChecked() ? plCollisionDetector::kTypeAny : 0)
                   | (fTypeFlags[kCBTypeUnEnter]->isChecked() ? plCollisionDetector::kTypeUnEnter : 0)
                   | (fTypeFlags[kCBTypeUnExit]->isChecked() ? plCollisionDetector::kTypeUnExit : 0)
                   | (fTypeFlags[kCBTypeBump]->isChecked() ? plCollisionDetector::kTypeBump : 0));

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
        fSubworld->setText(~obj->getSubworld()->getName());
    }
}

void QCollisionDetector::unsetSubworld()
{
    plSubworldRegionDetector* obj = plSubworldRegionDetector::Convert(fCreatable);
    obj->setSubworld(plKey());
    fSubworld->setCreatable(NULL);
    fSubworld->setText("(None)");
}
