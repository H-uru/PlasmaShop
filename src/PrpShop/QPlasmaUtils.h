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

#ifndef _PLASMAWIDGETS_H
#define _PLASMAWIDGETS_H

#include <QIcon>
#include <vector>
#include <ResManager/pdUnifiedTypeMap.h>
#include <PRP/plCreatable.h>
#include "QPlasma.h"
#include "QNumerics.h"

extern bool s_showTypeIDs;

enum
{
    kPreview_Type = 0x1000,
    kPRC_Type = 0x2000,
    kTargets_Type = 0x4000,
    kRESERVED_Type = 0x8000,    // 0x8000 is reserved for null keyed objects
    kHex_Type = 0x10000,
    kRealType_Mask = 0x8FFF,

    kPreviewSceneNode = kPreview_Type | kSceneNode,
    kPreviewSceneObject = kPreview_Type | kSceneObject,
    kPreviewMipmap = kPreview_Type | kMipmap,
    kPreviewCubicEnvironmap = kPreview_Type | kCubicEnvironmap,
    kPreviewCoordinateInterface = kPreview_Type | kCoordinateInterface,
};

QIcon pqGetTypeIcon(int);
QString pqGetFriendlyClassName(int);

std::vector<short> pqGetValidKOTypes();
bool pqCanPreviewType(plCreatable* pCre);
bool pqHasTargets(plCreatable* c);

#endif
