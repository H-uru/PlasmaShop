#include "QLayerLinkAnimation.h"

#include <QLabel>
#include <QGridLayout>
#include "../../Main.h"
#include "../../QKeyDialog.h"
#include "../../../QPlasma.h"

QLayerLinkAnimation::QLayerLinkAnimation(plCreatable* pCre, QWidget* parent)
                   : QCreatable(pCre, kLayerLinkAnimation, parent)
{
    plLayerLinkAnimation* lay = (plLayerLinkAnimation*)fCreatable;

    fLayerAnimLink = new QCreatableLink(this, false);
    fLayerAnimLink->setText(tr("Layer Animation Properties"));
    fLayerAnimLink->setCreatable(lay);
    fLayerAnimLink->setForceType(kLayerAnimation);

    fLinkKey = new QCreatableLink(this);
    fLinkKey->setKey(lay->getLinkKey());
    fLinkKey->setText(lay->getLinkKey().Exists()
                      ? ~lay->getLinkKey()->getName()
                      : "(None)");
    fLeavingAge = new QCheckBox(tr("Leaving Age"));
    fLeavingAge->setChecked(lay->getLeavingAge());

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(fLayerAnimLink, 0, 0, 1, 2);
    layout->addWidget(new QLabel(tr("Link Key:"), this), 3, 0);
    layout->addWidget(fLinkKey, 3, 1);
    layout->addWidget(fLeavingAge, 4, 0, 1, 2);

    connect(fLinkKey, SIGNAL(addObject()), this, SLOT(setLinkKey()));
    connect(fLinkKey, SIGNAL(delObject()), this, SLOT(unsetLinkKey()));
}

void QLayerLinkAnimation::saveDamage()
{
    plLayerLinkAnimation* lay = (plLayerLinkAnimation*)fCreatable;
    lay->setLeavingAge(fLeavingAge->isChecked());
}

void QLayerLinkAnimation::setLinkKey()
{
    plLayerLinkAnimation* lay = (plLayerLinkAnimation*)fCreatable;
    QFindKeyDialog dlg(this);
    if (lay->getLinkKey().Exists())
        dlg.init(PrpShopMain::ResManager(), lay->getLinkKey());
    else
        dlg.init(PrpShopMain::ResManager(), lay->getKey()->getLocation(), kSceneObject);
    if (dlg.exec() == QDialog::Accepted) {
        lay->setLinkKey(dlg.selection());
        fLinkKey->setKey(lay->getLinkKey());
        fLinkKey->setText(~lay->getLinkKey()->getName());
    }
}

void QLayerLinkAnimation::unsetLinkKey()
{
    plLayerLinkAnimation* lay = (plLayerLinkAnimation*)fCreatable;
    lay->setLinkKey(plKey());
    fLinkKey->setCreatable(NULL);
    fLinkKey->setText("(None)");
}
