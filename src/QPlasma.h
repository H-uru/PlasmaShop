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

#ifndef _QPLASMA_H
#define _QPLASMA_H

#include <QString>
#include <Util/plString.h>
#include <PRP/KeyedObject/plLocation.h>

// APIs between Qt4 and Qt5 are incompatible for getting these paths
#if QT_VERSION >= 0x050000
    #include <QStandardPaths>

    inline QString QStandardPaths_DataLocation()
    { return QStandardPaths::writableLocation(QStandardPaths::DataLocation); }

    inline QString QStandardPaths_HomeLocation()
    { return QStandardPaths::writableLocation(QStandardPaths::HomeLocation); }

    inline QString QStandardPaths_DocumentsLocation()
    { return QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation); }
#else
    #include <QDesktopServices>

    inline QString QStandardPaths_DataLocation()
    { return QDesktopServices::storageLocation(QDesktopServices::DataLocation); }

    inline QString QStandardPaths_HomeLocation()
    { return QDesktopServices::storageLocation(QDesktopServices::HomeLocation); }

    inline QString QStandardPaths_DocumentsLocation()
    { return QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation); }
#endif

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
