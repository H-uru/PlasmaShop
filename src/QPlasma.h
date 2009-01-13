#ifndef _QPLASMA_H
#define _QPLASMA_H

#include <QString>
#include <Util/plString.h>
#include <PRP/KeyedObject/plLocation.h>

// Shorthand Plasma-Qt string conversion
QString operator~(const plString& str);
plString operator~(const QString& str);

unsigned int qHash(const plLocation& loc);

#endif
