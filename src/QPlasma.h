#ifndef _QPLASMA_H
#define _QPLASMA_H

#include <QString>
#include <Util/plString.h>
#include <PRP/KeyedObject/plLocation.h>

// Shorthand Plasma-Qt string conversion
inline QString operator~(const plString& str)
{ return QString::fromUtf8(str.cstr()); }

inline plString operator~(const QString& str)
{ return plString(str.toUtf8().constData()); }

// qHash functions for QHashMaps
inline unsigned int qHash(const plLocation& loc)
{ return loc.unparse(); }

/*
inline unsigned int qHash(const plKey& key)
{
    return (qHash(key->getLocation()) + key->getType())
         ^ (qHash(QString(key->getName().cstr())));
}
*/

#endif
