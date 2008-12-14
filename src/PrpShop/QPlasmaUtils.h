#ifndef _PLASMAWIDGETS_H
#define _PLASMAWIDGETS_H

#include <QIcon>
#include <PRP/KeyedObject/plLocation.h>

QIcon pqGetTypeIcon(short);
QString pqGetFriendlyClassName(short);

unsigned int qHash(const plLocation& loc);

#endif
