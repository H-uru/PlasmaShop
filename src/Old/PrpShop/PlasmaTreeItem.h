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

#ifndef _PLASMATREEITEM_H
#define _PLASMATREEITEM_H

#include <wx/treectrl.h>
#include <PRP/plPageInfo.h>
#include <PRP/KeyedObject/hsKeyedObject.h>

struct TreeAgeInfo {
    wxString fAgeName;
    bool fHasTextures, fHasBuiltIn;

    TreeAgeInfo(const wxString& name = wxEmptyString);
    TreeAgeInfo(const TreeAgeInfo& init);
};

class PlasmaTreeItem : public wxTreeItemData {
public:
    enum Type { kTypeInvalid, kTypeAge, kTypePage, kTypeObject };

protected:
    Type fType;
    TreeAgeInfo fAge;
    plPageInfo* fPage;
    plKey fObject;

public:
    PlasmaTreeItem();
    PlasmaTreeItem(const TreeAgeInfo& age);
    PlasmaTreeItem(plPageInfo* page);
    PlasmaTreeItem(plKey obj);

    TreeAgeInfo* getAge();
    plPageInfo* getPage();
    plKey getObject();
};

#endif
