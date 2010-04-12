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

#include "PlasmaTreeItem.h"

/* TreeAgeInfo */
TreeAgeInfo::TreeAgeInfo(const wxString& name)
           : fAgeName(name), fHasTextures(false), fHasBuiltIn(false)
{ }

TreeAgeInfo::TreeAgeInfo(const TreeAgeInfo& init)
           : fAgeName(init.fAgeName), fHasTextures(init.fHasTextures),
             fHasBuiltIn(init.fHasBuiltIn)
{ }


/* PlasmaTreeItem */
PlasmaTreeItem::PlasmaTreeItem()
              : fType(kTypeInvalid)
{ }

PlasmaTreeItem::PlasmaTreeItem(const TreeAgeInfo& age)
              : fType(kTypeAge), fAge(age)
{ }

PlasmaTreeItem::PlasmaTreeItem(plPageInfo* page)
              : fType(kTypePage), fPage(page)
{ }

PlasmaTreeItem::PlasmaTreeItem(plKey obj)
              : fType(kTypeObject), fObject(obj)
{ }

TreeAgeInfo* PlasmaTreeItem::getAge()
{ return (fType == kTypeAge) ? &fAge : NULL; }

plPageInfo* PlasmaTreeItem::getPage()
{ return (fType == kTypePage) ? fPage : NULL; }

plKey PlasmaTreeItem::getObject()
{ return (fType == kTypeObject) ? fObject : plKey(); }
