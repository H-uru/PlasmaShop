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

QPlasmaTreeItem::QPlasmaTreeItem(const QString& age)
    : QTreeWidgetItem(kTypeAge), fHasBuiltIn(false), fHasTextures(false),
      fAge(age)
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

QPlasmaTreeItem::QPlasmaTreeItem(QTreeWidget* parent, const QString& age)
    : QTreeWidgetItem(parent, kTypeAge), fHasBuiltIn(false),
      fHasTextures(false), fAge(age)
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

QPlasmaTreeItem::QPlasmaTreeItem(QTreeWidgetItem* parent, const QString& age)
    : QTreeWidgetItem(parent, kTypeAge), fHasBuiltIn(false),
      fHasTextures(false), fAge(age)
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
        return QString("[%1;%2] %3")
            .arg(page->getLocation().getSeqPrefix())
            .arg(page->getLocation().getPageNum())
            .arg(st2qstr(page->getPage()));
    } else {
        return st2qstr(page->getPage());
    }
}

void QPlasmaTreeItem::reinit()
{
    switch (type()) {
        case kTypeNone:
            fSortId = 0;
            setIcon(0, QIcon(":/img/folder.png"));
            break;

        case kTypeKO:
            fSortId = s_showObjectIDs ? static_cast<int32_t>(fObjKey->getID()) : 0;
            setText(0, pqFormatKeyName(fObjKey));
            setIcon(0, pqGetTypeIcon(fObjKey->getType()));
            break;

        case kTypeClassType:
            fSortId = s_showTypeIDs ? fClassType : 0;
            setText(0, pqGetFriendlyClassName(fClassType));
            setIcon(0, QIcon(":/img/folder.png"));
            break;

        case kTypeAge:
            fSortId = 0;
            setText(0, fAge);
            setIcon(0, QIcon(":/img/age.png"));
            break;

        case kTypePage:
            fSortId = s_showAgePageIDs ? static_cast<int32_t>(fPage->getLocation().unparse()) : 0;
            setText(0, pqFormatPageName(fPage));
            setIcon(0, QIcon(":/img/page.png"));
            break;
    }
}

bool QPlasmaTreeItem::operator<(const QTreeWidgetItem& other) const
{
    if (type() >= kTypeNone && type() < kMaxPlasmaTypes) {
        // The other item is also one of ours,
        // so try sorting numerically by ID.
        auto otherPlasma = static_cast<const QPlasmaTreeItem&>(other);
        if (fSortId != otherPlasma.fSortId) {
            return fSortId < otherPlasma.fSortId;
        }
    }
    // If the other item doesn't belong to us or if the two IDs are equal,
    // fall back to default sorting by text.
    return QTreeWidgetItem::operator<(other);
}
