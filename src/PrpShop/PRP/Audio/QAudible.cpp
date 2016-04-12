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

#include "QAudible.h"

#include <QGridLayout>
#include <PRP/Audio/plAudible.h>
#include "../../QKeyDialog.h"
#include "../../Main.h"

/* QAudible */
QAudible::QAudible(plCreatable* pCre, QWidget* parent)
        : QCreatable(pCre, pCre->ClassIndex(), parent)
{
    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(new QLabel(tr("No editable fields"), this), 0, 0);
}


/* QWinAudible */
QWinAudible::QWinAudible(plCreatable* pCre, QWidget* parent)
           : QCreatable(pCre, pCre->ClassIndex(), parent)
{
    plWinAudible* obj = plWinAudible::Convert(fCreatable);

    fSoundKeys = new QKeyList(obj->getKey(), this);
    for (size_t i=0; i<obj->getSounds().size(); i++)
        fSoundKeys->addKey(obj->getSounds()[i]);

    fSceneNode = new QCreatableLink(this);
    fSceneNode->setKey(obj->getSceneNode(), false);
    fSceneNode->setText(tr("Scene Node"));

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(new QLabel(tr("Sounds:"), this), 0, 0);
    layout->addWidget(fSoundKeys, 1, 0);
    layout->addWidget(fSceneNode, 2, 0);
    fSoundKeys->adjustColumns();

    connect(fSceneNode, SIGNAL(addObject()), this, SLOT(setSceneNode()));
    connect(fSceneNode, SIGNAL(delObject()), this, SLOT(unsetSceneNode()));
}

void QWinAudible::saveDamage()
{
    plWinAudible* obj = plWinAudible::Convert(fCreatable);

    obj->clearSounds();
    QList<plKey> keys = fSoundKeys->keys();
    while (!keys.isEmpty())
        obj->addSound(keys.takeFirst());
}

void QWinAudible::setSceneNode()
{
    plWinAudible* obj = plWinAudible::Convert(fCreatable);
    QFindKeyDialog dlg(this);
    if (obj->getSceneNode().Exists())
        dlg.init(PrpShopMain::ResManager(), obj->getSceneNode());
    else
        dlg.init(PrpShopMain::ResManager(), obj->getKey()->getLocation(), kSceneNode);
    if (dlg.exec() == QDialog::Accepted) {
        obj->setSceneNode(dlg.selection());
        fSceneNode->setKey(obj->getSceneNode(), false);
    }
}

void QWinAudible::unsetSceneNode()
{
    plWinAudible* obj = plWinAudible::Convert(fCreatable);
    obj->setSceneNode(plKey());
    fSceneNode->setCreatable(NULL);
}
