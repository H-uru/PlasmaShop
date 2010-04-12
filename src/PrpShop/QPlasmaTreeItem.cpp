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
    setIcon(0, QIcon(":/img/folder.png"));
}

QPlasmaTreeItem::QPlasmaTreeItem(plKey obj)
    : QTreeWidgetItem(kTypeKO), fObjKey(obj)
{
    setText(0, ~obj->getName());
    setIcon(0, pqGetTypeIcon(obj->getType()));
}

QPlasmaTreeItem::QPlasmaTreeItem(const QString& age)
    : QTreeWidgetItem(kTypeAge), fHasBuiltIn(false), fHasTextures(false),
      fAge(age)
{
    setText(0, age);
    setIcon(0, QIcon(":/img/age.png"));
}

QPlasmaTreeItem::QPlasmaTreeItem(plPageInfo* page)
    : QTreeWidgetItem(kTypePage), fPage(page)
{
    setText(0, ~page->getPage());
    setIcon(0, QIcon(":/img/page.png"));
}

QPlasmaTreeItem::QPlasmaTreeItem(QTreeWidget* parent)
    : QTreeWidgetItem(parent, kTypeNone)
{
    setIcon(0, QIcon(":/img/folder.png"));
}

QPlasmaTreeItem::QPlasmaTreeItem(QTreeWidget* parent, plKey obj)
    : QTreeWidgetItem(parent, kTypeKO), fObjKey(obj)
{
    setText(0, ~obj->getName());
    setIcon(0, pqGetTypeIcon(obj->getType()));
}

QPlasmaTreeItem::QPlasmaTreeItem(QTreeWidget* parent, const QString& age)
    : QTreeWidgetItem(parent, kTypeAge), fHasBuiltIn(false),
      fHasTextures(false), fAge(age)
{
    setText(0, age);
    setIcon(0, QIcon(":/img/age.png"));
}

QPlasmaTreeItem::QPlasmaTreeItem(QTreeWidget* parent, plPageInfo* page)
    : QTreeWidgetItem(parent, kTypePage), fPage(page)
{
    setText(0, ~page->getPage());
    setIcon(0, QIcon(":/img/page.png"));
}

QPlasmaTreeItem::QPlasmaTreeItem(QTreeWidgetItem* parent)
    : QTreeWidgetItem(parent, kTypeNone)
{
    setIcon(0, QIcon(":/img/folder.png"));
}

QPlasmaTreeItem::QPlasmaTreeItem(QTreeWidgetItem* parent, plKey obj)
    : QTreeWidgetItem(parent, kTypeKO), fObjKey(obj)
{
    setText(0, ~obj->getName());
    setIcon(0, pqGetTypeIcon(obj->getType()));
}

QPlasmaTreeItem::QPlasmaTreeItem(QTreeWidgetItem* parent, const QString& age)
    : QTreeWidgetItem(parent, kTypeAge), fHasBuiltIn(false),
      fHasTextures(false), fAge(age)
{
    setText(0, age);
    setIcon(0, QIcon(":/img/age.png"));
}

QPlasmaTreeItem::QPlasmaTreeItem(QTreeWidgetItem* parent, plPageInfo* page)
    : QTreeWidgetItem(parent, kTypePage), fPage(page)
{
    setText(0, ~page->getPage());
    setIcon(0, QIcon(":/img/page.png"));
}

hsKeyedObject* QPlasmaTreeItem::obj() const
{ return (type() == kTypeKO) ? fObjKey->getObj() : NULL; }

QString QPlasmaTreeItem::age() const
{ return (type() == kTypeAge) ? fAge : QString(); }

plPageInfo* QPlasmaTreeItem::page() const
{ return (type() == kTypePage) ? fPage : NULL; }

bool QPlasmaTreeItem::hasBuiltIn() const
{ return (type() == kTypeAge) ? fHasBuiltIn : false; }

bool QPlasmaTreeItem::hasTextures() const
{ return (type() == kTypeAge) ? fHasTextures : false; }

void QPlasmaTreeItem::setHasBuiltIn(bool has)
{ if (type() == kTypeAge) fHasBuiltIn = has; }

void QPlasmaTreeItem::setHasTextures(bool has)
{ if (type() == kTypeAge) fHasTextures = has; }

QString QPlasmaTreeItem::filename() const
{ return fFilename; }

void QPlasmaTreeItem::setFilename(const QString& filename)
{ fFilename = filename; }
