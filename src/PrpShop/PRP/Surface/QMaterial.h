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

#ifndef _QMATERIAL_H
#define _QMATERIAL_H

#include "PRP/QCreatable.h"

#include <PRP/Surface/hsGMaterial.h>
#include "PRP/QObjLink.h"
#include "PRP/QKeyList.h"
#include "QBitmaskCheckBox.h"

class QMaterial : public QCreatable
{
    Q_OBJECT

protected:
    enum
    {
        kCbShaded, kCbEnvironMap, kCbProjectOnto, kCbSoftShadow, kCbSpecular,
        kCbTwoSided, kCbDrawAsSplats, kCbAdjusted, kCbNoSoftShadow, kCbDynamic,
        kCbDecal, kCbIsEmissive, kCbIsLightMapped, kCbNeedsBlendChannel,
        kNumCompFlags
    };

    QCreatableLink* fSynchObjLink;
    QBitmaskCheckBox* fCBFlags[kNumCompFlags];
    QKeyList* fLayers;
    QKeyList* fPiggyBacks;

public:
    QMaterial(plCreatable* pCre, QWidget* parent = NULL);
    void saveDamage() override;
};

#endif
