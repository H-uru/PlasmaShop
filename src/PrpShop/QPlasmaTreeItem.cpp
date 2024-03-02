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

void QPlasmaTreeItem::reinit()
{
    switch (type()) {
        case kTypeNone:
            setIcon(0, QIcon(":/img/folder.png"));
            break;

        case kTypeKO:
            setText(0, pqFormatKeyName(fObjKey));
            setIcon(0, pqGetTypeIcon(fObjKey->getType()));
            break;

        case kTypeAge:
            setText(0, fAge);
            setIcon(0, QIcon(":/img/age.png"));
            break;

        case kTypePage:
            setText(0, st2qstr(fPage->getPage()));
            setIcon(0, QIcon(":/img/page.png"));
            break;
    }
}
