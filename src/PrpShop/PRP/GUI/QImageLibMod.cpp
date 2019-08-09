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

#include "QImageLibMod.h"
#include <QGridLayout>

/* QImageLibMod */
QImageLibMod::QImageLibMod(plCreatable* pCre, QWidget* parent)
    : QCreatable(pCre, kImageLibMod, parent)
{
    plImageLibMod* mod = plImageLibMod::Convert(fCreatable);

    fSynchObjLink = new QCreatableLink(this, false);
    fSynchObjLink->setCreatable(mod, tr("Synch Flags"));
    fSynchObjLink->setForceType(kSynchedObject);

    fImages = new QKeyList(mod->getKey(), this);
    for (size_t i=0; i<mod->getImages().size(); i++)
        fImages->addKey(mod->getImages()[i]);

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(fSynchObjLink, 0, 0);
    layout->addWidget(new QLabel(tr("Images:"), this), 1, 0);
    layout->addWidget(fImages, 2, 0);

    fImages->adjustColumns();
}

void QImageLibMod::saveDamage()
{
    plImageLibMod* mod = plImageLibMod::Convert(fCreatable);

    mod->clearImages();
    QList<plKey> imgKeys = fImages->keys();
    while (!imgKeys.isEmpty())
        mod->addImage(imgKeys.takeFirst());
}
