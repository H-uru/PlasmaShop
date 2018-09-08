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

#include "QSeekPointMod.h"

#include <QGridLayout>
#include <PRP/Avatar/plSeekPointMod.h>

QSeekPointMod::QSeekPointMod(plCreatable* pCre, QWidget* parent)
              : QCreatable(pCre, kSeekPointMod, parent)
{
    plSeekPointMod* obj = plSeekPointMod::Convert(fCreatable);

    fSynchObjLink = new QCreatableLink(this, false);
    fSynchObjLink->setCreatable(obj, tr("Synch Flags"));
    fSynchObjLink->setForceType(kSynchedObject);

    fSeekPointName = new QLineEdit(this);
    fSeekPointName->setText(st2qstr(obj->getName()));

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(fSynchObjLink, 0, 0);
    layout->addWidget(new QLabel(tr("Name:"), this), 1, 0);
    layout->addWidget(fSeekPointName, 1, 1);
}

void QSeekPointMod::saveDamage()
{
    plSeekPointMod* obj = plSeekPointMod::Convert(fCreatable);

    obj->setName(qstr2st(fSeekPointName->text()));
}
