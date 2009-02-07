#ifndef _PLASMAWIDGETS_H
#define _PLASMAWIDGETS_H

#include <QIcon>
#include <vector>
#include <ResManager/pdUnifiedTypeMap.h>
#include "../QPlasma.h"
#include "../QNumerics.h"

enum {
    kPreviewSceneNode = 0x1000 | kSceneNode,
    kPreviewSceneObject = 0x1000 | kSceneObject,
    kPreviewMipmap = 0x1000 | kMipmap,
    kPreviewCubicEnvironmap = 0x1000 | kCubicEnvironmap,
    kPreviewCoordinateInterface = 0x1000 | kCoordinateInterface,
};

QIcon pqGetTypeIcon(short);
QString pqGetFriendlyClassName(short);

std::vector<short> pqGetValidKOTypes();
bool pqCanPreviewType(short);

#endif
