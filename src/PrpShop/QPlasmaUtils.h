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
#include "../QPlasma.h"
#include "../QNumerics.h"

extern bool s_showTypeIDs;

enum {
    kPreview_Type = 0x1000,
    kPRC_Type = 0x2000,

    kPreviewSceneNode = kPreview_Type | kSceneNode,
    kPreviewSceneObject = kPreview_Type | kSceneObject,
    kPreviewMipmap = kPreview_Type | kMipmap,
    kPreviewCubicEnvironmap = kPreview_Type | kCubicEnvironmap,
    kPreviewCoordinateInterface = kPreview_Type | kCoordinateInterface,
};

QIcon pqGetTypeIcon(short);
QString pqGetFriendlyClassName(short);

std::vector<short> pqGetValidKOTypes();
bool pqCanPreviewType(plCreatable* pCre);

#endif
