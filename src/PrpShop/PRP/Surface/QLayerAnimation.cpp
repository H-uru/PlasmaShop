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

#include "QLayerAnimation.h"

#include <QGridLayout>
#include <PRP/Surface/plLayerAnimation.h>

QLayerAnimation::QLayerAnimation(plCreatable* pCre, QWidget* parent)
    : QCreatable(pCre, kLayerAnimation, parent)
{
    plLayerAnimation* lay = plLayerAnimation::Convert(fCreatable);

    fLayerAnimBaseLink = new QCreatableLink(this, false);
    fLayerAnimBaseLink->setCreatable(lay, tr("Layer Animation Properties"));
    fLayerAnimBaseLink->setForceType(kLayerAnimationBase);

    fAnimTimeConvert = new QCreatableLink(this, false);
    fAnimTimeConvert->setCreatable(&lay->getTimeConvert(), tr("Animation Time Properties"));

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(fLayerAnimBaseLink, 0, 0);
    layout->addWidget(fAnimTimeConvert, 1, 0);
}
