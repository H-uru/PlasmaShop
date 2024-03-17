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
#include <PRP/KeyedObject/plKey.h>
#include "QPlasma.h"
#include "QNumerics.h"

extern bool s_showAgePageIDs;
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
bool pqIsValidKOType(short);
bool pqCanPreviewType(plCreatable* pCre);
bool pqHasTargets(plCreatable* c);

enum pqRefPriority
{
    // Default priority - include everything that doesn't fall into the categories below.
    kDefault,
    // Also include child objects that are often (but not always) also reachable via another route
    // that makes more sense for the purposes of the structure tree.
    // For example, a scene node has references to all scene objects in the page,
    // but if a scene object has a parent coordinate interface,
    // we would rather display it there instead of under the top-level scene node.
    // Thus, the scene node's scene objects are only included starting with this priority.
    kFlatChildren,
    // Also include backreferences that are rarely interesting.
    // For example, a scene object has a backreference to its scene node,
    // and interfaces and modifiers have backreferences to the scene object to which they belong.
    kBackRefs,
    kMax,
};

std::vector<plKey> pqGetReferencedKeys(plCreatable* c, pqRefPriority priority = pqRefPriority::kDefault);

#endif
