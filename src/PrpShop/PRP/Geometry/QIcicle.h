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

#ifndef _QICICLE_H
#define _QICICLE_H

#include <QCheckBox>
#include <QComboBox>
#include <PRP/Geometry/plDrawableSpans.h>
#include <PRP/Geometry/plIcicle.h>
#include "PRP/QCreatable.h"
#include "PRP/QKeyList.h"
#include "PRP/QObjLink.h"

class QIcicle : public QCreatable {
    Q_OBJECT

protected:
    enum {
        kPropNoDraw, kPropNoShadowCast, kPropFacesSortable, kPropVolatile,
        kWaterHeight, kPropRunTimeLight, kPropReverseSort, kPropHasPermaLights,
        kPropHasPermaProjs, kPropMatHasSpecular, kPropProjAsVtx,
        kPropSkipProjection, kPropNoShadow, kPropForceShadow,
        kPropDisableNormal, kPropCharacter, kPartialSort, kVisLOS, kNumFlags
    };

    plIcicle* fSpan;

    QComboBox* fLightingType;
    QCheckBox* fFlags[kNumFlags];
    QKeyList* fLights;
    QKeyList* fProjs;
    QCreatableLink* fMatLink;

public:
    QIcicle(plCreatable* pCre, size_t idx, QWidget* parent = NULL);
};

#endif
