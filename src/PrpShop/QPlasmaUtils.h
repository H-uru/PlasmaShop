#ifndef _PLASMAWIDGETS_H
#define _PLASMAWIDGETS_H

#include <QIcon>
#include <PRP/KeyedObject/plLocation.h>
#include <vector>

QIcon pqGetTypeIcon(short);
QString pqGetFriendlyClassName(short);

std::vector<short> pqGetValidKOTypes();

unsigned int qHash(const plLocation& loc);

#endif
