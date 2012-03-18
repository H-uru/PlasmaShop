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

#include "QLayerLinkAnimation.h"

#include <QLabel>
#include <QGridLayout>
#include "../../Main.h"
#include "../../QKeyDialog.h"
#include "../../../QPlasma.h"

QLayerLinkAnimation::QLayerLinkAnimation(plCreatable* pCre, QWidget* parent)
                   : QCreatable(pCre, kLayerLinkAnimation, parent)
{
    plLayerLinkAnimation* lay = plLayerLinkAnimation::Convert(fCreatable);

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
    plLayerLinkAnimation* lay = plLayerLinkAnimation::Convert(fCreatable);
    lay->setLeavingAge(fLeavingAge->isChecked());
}

void QLayerLinkAnimation::setLinkKey()
{
    plLayerLinkAnimation* lay = plLayerLinkAnimation::Convert(fCreatable);
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
    plLayerLinkAnimation* lay = plLayerLinkAnimation::Convert(fCreatable);
    lay->setLinkKey(plKey());
    fLinkKey->setCreatable(NULL);
    fLinkKey->setText("(None)");
}
