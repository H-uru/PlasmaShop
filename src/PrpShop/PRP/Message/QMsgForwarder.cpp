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

#include "QMsgForwarder.h"

#include <QLabel>
#include <QGridLayout>

QMsgForwarder::QMsgForwarder(plCreatable* pCre, QWidget* parent)
             : QCreatable(pCre, kMsgForwarder, parent)
{
    plMsgForwarder* obj = (plMsgForwarder*)fCreatable;

    fForwardKeys = new QKeyList(this);
    for (size_t i=0; i<obj->getForwardKeys().getSize(); i++)
        fForwardKeys->addKey(obj->getForwardKeys()[i]);

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(new QLabel(tr("Forward Keys:"), this), 0, 0);
    layout->addWidget(fForwardKeys, 1, 0);
    fForwardKeys->adjustColumns();
}

void QMsgForwarder::saveDamage()
{
    plMsgForwarder* obj = (plMsgForwarder*)fCreatable;

    obj->clearForwardKeys();
    QList<plKey> keys = fForwardKeys->keys();
    while (!keys.isEmpty())
        obj->addForwardKey(keys.takeFirst());
}
