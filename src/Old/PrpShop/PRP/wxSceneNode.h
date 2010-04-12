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

#ifndef _WXSCENENODE_H
#define _WXSCENENODE_H

#include "../wxPrpPlasma.h"
#include "../wxPrpCanvas.h"
#include "wxPrpGUI.h"

class wxSceneNode : public wxPrpPlasmaObject {
protected:
    wxKeyListCtrl* lsSceneObjects;
    wxKeyListCtrl* lsPoolObjects;

    wxPrpCanvas* fPreviewCanvas;

public:
    wxSceneNode(plKey key, plResManager* mgr, wxTreeCtrl* tree, const wxTreeItemId& tid);
    virtual void AddPropPages(wxNotebook* nb);
    virtual wxWindow* MakePreviewPane(wxWindow* parent);
    //virtual void SaveDamage();
    virtual void Refresh();

protected:
    enum {
        ID_SCENEOBJ_ADD, ID_SCENEOBJ_DEL,
    };

    void OnSceneObjectMenu(wxCommandEvent& evt);
    void OnPoolObjectMenu(wxCommandEvent& evt);
    void OnSceneObjectActivated(wxListEvent& evt);
    void OnPoolObjectActivated(wxListEvent& evt);

    void OnSceneObjAddClick(wxMenuEvent& evt);
    void OnSceneObjDelClick(wxMenuEvent& evt);
};

wxTreeItemId TreeAddSceneNode(wxTreeCtrl* tree, const wxTreeItemId& parent, plResManager* mgr, plKey key);

#endif
