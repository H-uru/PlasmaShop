#include "QCollisionDetector.h"

#include <QLabel>
#include <QGroupBox>
#include <QGridLayout>
#include "../../QKeyDialog.h"
#include "../../Main.h"

QCollisionDetector::QCollisionDetector(plCreatable* pCre, QWidget* parent)
                  : QCreatable(pCre, pCre->ClassIndex(), parent)
{
    plCollisionDetector* obj = (plCollisionDetector*)fCreatable;
    plSubworldRegionDetector* subDet = plSubworldRegionDetector::Convert(fCreatable);
    plPanicLinkRegion* plRgn = plPanicLinkRegion::Convert(fCreatable);

    fDetectorLink = new QCreatableLink(this, false);
    fDetectorLink->setText(tr("Detector Properties"));
    fDetectorLink->setCreatable(obj);
    fDetectorLink->setForceType(kDetectorModifier);

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(fDetectorLink, 0, 0, 1, 2);

    if (subDet == NULL) {
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

    if (subDet != NULL) {
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

    if (plRgn != NULL) {
        fBoolParam = new QCheckBox(tr("Play Link-Out Animation"), this);
        fBoolParam->setChecked(plRgn->getPlayLinkOutAnim());
        layout->addWidget(fBoolParam, 2, 0, 1, 2);
    }
}

void QCollisionDetector::saveDamage()
{
    plCollisionDetector* obj = (plCollisionDetector*)fCreatable;
    plSubworldRegionDetector* subDet = plSubworldRegionDetector::Convert(fCreatable);
    plPanicLinkRegion* plRgn = plPanicLinkRegion::Convert(fCreatable);

    if (subDet == NULL)
        obj->setType((fTypeFlags[kCBTypeEnter]->isChecked() ? plCollisionDetector::kTypeEnter : 0)
                   | (fTypeFlags[kCBTypeExit]->isChecked() ? plCollisionDetector::kTypeExit : 0)
                   | (fTypeFlags[kCBTypeAny]->isChecked() ? plCollisionDetector::kTypeAny : 0)
                   | (fTypeFlags[kCBTypeUnEnter]->isChecked() ? plCollisionDetector::kTypeUnEnter : 0)
                   | (fTypeFlags[kCBTypeUnExit]->isChecked() ? plCollisionDetector::kTypeUnExit : 0)
                   | (fTypeFlags[kCBTypeBump]->isChecked() ? plCollisionDetector::kTypeBump : 0));

    if (subDet != NULL)
        subDet->setOnExit(fBoolParam->isChecked());

    if (plRgn != NULL)
        plRgn->setPlayLinkOutAnim(fBoolParam->isChecked());
}

void QCollisionDetector::setSubworld()
{
    plSubworldRegionDetector* obj = (plSubworldRegionDetector*)fCreatable;
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
    plSubworldRegionDetector* obj = (plSubworldRegionDetector*)fCreatable;
    obj->setSubworld(plKey());
    fSubworld->setCreatable(NULL);
    fSubworld->setText("(None)");
}
