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

#include "QTargetList.h"

#include <QGridLayout>
#include <PRP/Modifier/plModifier.h>

QTargetList::QTargetList(plCreatable* pCre, QWidget* parent)
    : QCreatable(pCre, kTargets_Type | pCre->ClassIndex(), parent)
{
    plModifier* mod = plModifier::Convert(pCre);

    fTargets = new QKeyList(mod->getKey(), this);
    for (size_t i=0; i < mod->getTargetsCount(); i++)
       fTargets->addKey(mod->getTarget(i));

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(fTargets, 0, 0);
    setLayout(layout);
}

