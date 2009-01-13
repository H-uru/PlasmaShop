#ifndef _PLASMAWIDGETS_H
#define _PLASMAWIDGETS_H

#include <QIcon>
#include <vector>
#include "../QPlasma.h"

QIcon pqGetTypeIcon(short);
QString pqGetFriendlyClassName(short);

std::vector<short> pqGetValidKOTypes();

#endif
