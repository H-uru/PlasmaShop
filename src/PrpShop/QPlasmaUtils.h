#ifndef _PLASMAWIDGETS_H
#define _PLASMAWIDGETS_H

#include <QIcon>
#include <vector>
#include <ResManager/pdUnifiedTypeMap.h>
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
bool pqCanPreviewType(short);

#endif
