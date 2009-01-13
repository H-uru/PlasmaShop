#include "QPlasma.h"

QString operator~(const plString& str)
{
    return QString::fromUtf8(str.cstr());
}

plString operator~(const QString& str)
{
    return plString(str.toUtf8().constData());
}

unsigned int qHash(const plLocation& loc)
{ return loc.unparse(); }

/*
unsigned int qHash(const plKey& key)
{
    return (qHash(key->getLocation()) + key->getType())
         ^ (qHash(QString(key->getName().cstr())));
}
*/
