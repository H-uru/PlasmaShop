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
#include <string_theory/formatter>
#include <PRP/KeyedObject/plLocation.h>

// Shorthand Plasma-Qt string conversion
inline QString st2qstr(const ST::string& str)
{
    return QString::fromUtf8(str.c_str());
}

inline ST::string qstr2st(const QString& str)
{
    QByteArray utf8 = str.toUtf8();
    return ST::string::from_utf8(utf8.constData(), utf8.size(), ST::assume_valid);
}

// ST::format support
inline ST_FORMAT_TYPE(const QString&)
{
    QByteArray utf8 = value.toUtf8();
    ST::format_string(format, output, utf8.constData(), utf8.size());
}

// qHash functions for QHashMaps
inline unsigned int qHash(const plLocation& loc)
{
    return loc.unparse();
}

/*
inline unsigned int qHash(const plKey& key)
{
    return (qHash(key->getLocation()) + key->getType())
         ^ (qHash(QString(key->getName().cstr())));
}
*/

// Simplified backport of QOverload
#if (QT_VERSION < QT_VERSION_CHECK(5, 7, 0))
template <typename... Args>
struct QOverload
{
    template <typename R, typename T>
    static Q_DECL_CONSTEXPR auto of(R(T::*ptr)(Args...)) Q_DECL_NOTHROW -> decltype(ptr)
    {
        return ptr;
    }
};
#endif

#if defined(Q_OS_WIN)
#  define qStdIcon(name) QIcon(":/res/win/" name ".png")
#elif defined(Q_OS_MAC)
#  define qStdIcon(name) QIcon(":/res/mac/" name ".png")
#else
#  define qStdIcon(name) QIcon::fromTheme(name, QIcon(":/res/def/" name ".png"))
#endif

#endif
