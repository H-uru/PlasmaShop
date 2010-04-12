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

#include "QInterfaceInfoModifier.h"

#include <QLabel>
#include <QGridLayout>

QInterfaceInfoModifier::QInterfaceInfoModifier(plCreatable* pCre, QWidget* parent)
                      : QCreatable(pCre, kInterfaceInfoModifier, parent)
{
    plInterfaceInfoModifier* obj = (plInterfaceInfoModifier*)fCreatable;

    fSynchObjLink = new QCreatableLink(this, false);
    fSynchObjLink->setText(tr("Synch Flags"));
    fSynchObjLink->setCreatable(obj);
    fSynchObjLink->setForceType(kSynchedObject);

    fIntfKeys = new QKeyList(this);
    for (size_t i=0; i<obj->getIntfKeys().getSize(); i++)
        fIntfKeys->addKey(obj->getIntfKeys()[i]);

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(fSynchObjLink, 0, 0);
    layout->addWidget(new QLabel(tr("Interface Keys:"), this), 1, 0);
    layout->addWidget(fIntfKeys, 2, 0);
    fIntfKeys->adjustColumns();
}

void QInterfaceInfoModifier::saveDamage()
{
    plInterfaceInfoModifier* obj = (plInterfaceInfoModifier*)fCreatable;

    obj->clearIntfKeys();
    QList<plKey> keys = fIntfKeys->keys();
    while (!keys.isEmpty())
        obj->addIntfKey(keys.takeFirst());
}
