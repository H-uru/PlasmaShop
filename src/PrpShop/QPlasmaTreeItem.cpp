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

#include "QPlasmaTreeItem.h"

#include <string_theory/format>
#include "QPlasmaUtils.h"

QPlasmaTreeItem::QPlasmaTreeItem()
    : QTreeWidgetItem(kTypeNone)
{
    reinit();
}

QPlasmaTreeItem::QPlasmaTreeItem(plKey obj)
    : QTreeWidgetItem(kTypeKO), fObjKey(std::move(obj))
{
    reinit();
}

QPlasmaTreeItem::QPlasmaTreeItem(short classType)
    : QTreeWidgetItem(kTypeClassType), fClassType(classType)
{
    reinit();
}

QPlasmaTreeItem::QPlasmaTreeItem(const QString& age, int ageSeqPrefix)
    : QTreeWidgetItem(kTypeAge), fAge(age), fAgeSeqPrefix(ageSeqPrefix)
{
    reinit();
}

QPlasmaTreeItem::QPlasmaTreeItem(plPageInfo* page)
    : QTreeWidgetItem(kTypePage), fPage(page)
{
    reinit();
}

QPlasmaTreeItem::QPlasmaTreeItem(QTreeWidget* parent)
    : QTreeWidgetItem(parent, kTypeNone)
{
    reinit();
}

QPlasmaTreeItem::QPlasmaTreeItem(QTreeWidget* parent, plKey obj)
    : QTreeWidgetItem(parent, kTypeKO), fObjKey(std::move(obj))
{
    reinit();
}

QPlasmaTreeItem::QPlasmaTreeItem(QTreeWidget* parent, short classType)
    : QTreeWidgetItem(parent, kTypeClassType), fClassType(classType)
{
    reinit();
}

QPlasmaTreeItem::QPlasmaTreeItem(QTreeWidget* parent, const QString& age, int ageSeqPrefix)
    : QTreeWidgetItem(parent, kTypeAge), fAge(age), fAgeSeqPrefix(ageSeqPrefix)
{
    reinit();
}

QPlasmaTreeItem::QPlasmaTreeItem(QTreeWidget* parent, plPageInfo* page)
    : QTreeWidgetItem(parent, kTypePage), fPage(page)
{
    reinit();
}

QPlasmaTreeItem::QPlasmaTreeItem(QTreeWidgetItem* parent)
    : QTreeWidgetItem(parent, kTypeNone)
{
    reinit();
}

QPlasmaTreeItem::QPlasmaTreeItem(QTreeWidgetItem* parent, plKey obj)
    : QTreeWidgetItem(parent, kTypeKO), fObjKey(std::move(obj))
{
    reinit();
}

QPlasmaTreeItem::QPlasmaTreeItem(QTreeWidgetItem* parent, short classType)
    : QTreeWidgetItem(parent, kTypeClassType), fClassType(classType)
{
    reinit();
}

QPlasmaTreeItem::QPlasmaTreeItem(QTreeWidgetItem* parent, const QString& age, int ageSeqPrefix)
    : QTreeWidgetItem(parent, kTypeAge), fAge(age), fAgeSeqPrefix(ageSeqPrefix)
{
    reinit();
}

QPlasmaTreeItem::QPlasmaTreeItem(QTreeWidgetItem* parent, plPageInfo* page)
    : QTreeWidgetItem(parent, kTypePage), fPage(page)
{
    reinit();
}

static QString pqFormatPageName(const plPageInfo* page)
{
    if (s_showAgePageIDs) {
        return st2qstr(ST::format("{} {}", page->getLocation().toString(), page->getPage()));
    } else {
        return st2qstr(page->getPage());
    }
}

static QString pqFormatAgeName(const QString& name, int seqPrefix)
{
    if (s_showAgePageIDs) {
        return QString("<%1> %2").arg(seqPrefix).arg(name);
    } else {
        return name;
    }
}

void QPlasmaTreeItem::reinit()
{
    switch (type()) {
        case kTypeNone:
            setIcon(0, QIcon(":/img/folder.png"));
            break;

        case kTypeKO:
            setText(0, st2qstr(fObjKey->getName()));
            setIcon(0, pqGetTypeIcon(fObjKey->getType()));
            break;

        case kTypeClassType:
            setText(0, pqGetFriendlyClassName(fClassType));
            setIcon(0, QIcon(":/img/folder.png"));
            break;

        case kTypeAge:
            setText(0, pqFormatAgeName(fAge, fAgeSeqPrefix));
            setIcon(0, QIcon(":/img/age.png"));
            break;

        case kTypePage:
            setText(0, pqFormatPageName(fPage));
            setIcon(0, QIcon(":/img/page.png"));
            break;

        default:
            setText(0, QString("[Error: unhandled item type %1]").arg(type()));
            setIcon(0, QIcon());
    }
}

bool QPlasmaTreeItem::operator<(const QTreeWidgetItem& other) const
{
    if (s_showTypeIDs && type() == kTypeClassType && other.type() == kTypeClassType) {
        // If type IDs are shown,
        // sort class type folders by the numeric type ID.
        auto otherPlasma = static_cast<const QPlasmaTreeItem&>(other);
        return fClassType < otherPlasma.fClassType;
    } else if (s_showAgePageIDs && type() == kTypePage && other.type() == kTypePage) {
        // If age/page IDs are shown,
        // sort pages by their location
        // (i. e. first by age ID, then by page ID).
        auto otherPlasma = static_cast<const QPlasmaTreeItem&>(other);
        return fPage->getLocation() < otherPlasma.fPage->getLocation();
    } else if (s_showAgePageIDs && type() == kTypeAge && other.type() == kTypeAge) {
        // If age/page IDs are shown,
        // sort ages by their ID (sequence prefix).
        auto otherPlasma = static_cast<const QPlasmaTreeItem&>(other);
        return fAgeSeqPrefix < otherPlasma.fAgeSeqPrefix;
    } else if (type() != other.type()) {
        // Sort items of different types by their type,
        // i. e. group items of the same type together.
        // (This ensures a consistent sort order in case items of different types
        // with special sorting behavior somehow end up in the same list.)
        return type() < other.type();
    } else {
        // If both items have the same type, but the type has no special sorting behavior,
        // fall back to default sorting by text.
        return QTreeWidgetItem::operator<(other);
    }
}
