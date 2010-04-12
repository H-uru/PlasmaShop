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

#ifndef _WXCOORDINATEINTERFACE_H
#define _WXCOORDINATEINTERFACE_H

#include "wxSynchedObject.h"
#include "../wxPrpGUI.h"
#include "../../wxPrpCanvas.h"

class wxCoordinateInterface : public wxSynchedObject {
protected:
    wxCheckBox* cbDisable;
    wxCheckBox* cbCanEverDelayTransform;
    wxCheckBox* cbDelayedTransformEval;
    wxKeyListCtrl* lsChildren;

    wxPrpCanvas* fPreviewCanvas;

public:
    wxCoordinateInterface(plKey key, plResManager* mgr, wxTreeCtrl* tree, const wxTreeItemId& tid);
    virtual void AddPropPages(wxNotebook* nb);
    virtual wxWindow* MakePreviewPane(wxWindow* parent);
    virtual void SaveDamage();
    virtual void Refresh();

private:
    enum { ID_OBJ_ADD, ID_OBJ_DELETE };

    void OnChildrenMenu(wxCommandEvent& evt);
    void OnChildrenActivate(wxListEvent& evt);
    void OnAddChildClick(wxMenuEvent& evt);
    void OnDelChildClick(wxMenuEvent& evt);
};

wxTreeItemId TreeAddCoordInterface(wxTreeCtrl* tree, const wxTreeItemId& parent, plResManager* mgr, plKey key);

#endif
