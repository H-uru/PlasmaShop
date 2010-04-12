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

#ifndef _WXGMATERIAL_H
#define _WXGMATERIAL_H

#include "../Object/wxSynchedObject.h"
#include "../wxPrpGUI.h"

class wxGMaterial : public wxSynchedObject {
protected:
    wxCheckBox* cbCompShaded;
    wxCheckBox* cbCompEnvironMap;
    wxCheckBox* cbCompProjectOnto;
    wxCheckBox* cbCompSoftShadow;
    wxCheckBox* cbCompSpecular;
    wxCheckBox* cbCompTwoSided;
    wxCheckBox* cbCompDrawAsSplats;
    wxCheckBox* cbCompAdjusted;
    wxCheckBox* cbCompNoSoftShadow;
    wxCheckBox* cbCompDynamic;
    wxCheckBox* cbCompDecal;
    wxCheckBox* cbCompIsEmissive;
    wxCheckBox* cbCompIsLightMapped;
    wxCheckBox* cbCompNeedsBlendChannel;

    wxKeyListCtrl* lsLayers;
    wxKeyListCtrl* lsPiggyBacks;

public:
    wxGMaterial(plKey key, plResManager* mgr, wxTreeCtrl* tree, const wxTreeItemId& tid);
    virtual void AddPropPages(wxNotebook* nb);
    //virtual wxWindow* MakePreviewPane(wxWindow* parent);
    virtual void SaveDamage();

protected:
    enum {
        ID_OBJ_ADD, ID_OBJ_REMOVE, ID_OBJ_DELETE,
        ID_NEW_LAYER, ID_NEW_LAYERANIM
    };

    void OnLayerContextMenu(wxCommandEvent& evt);
    void OnPiggyContextMenu(wxCommandEvent& evt);
    void OnLayerItemActivated(wxListEvent& evt);
    void OnPiggyItemActivated(wxListEvent& evt);
    void OnLayerNewLayerClick(wxMenuEvent& evt);
    void OnLayerNewLayerAnimClick(wxMenuEvent& evt);
    void OnLayerAddClick(wxMenuEvent& evt);
    void OnLayerRemoveClick(wxMenuEvent& evt);
    void OnLayerDeleteClick(wxMenuEvent& evt);
    void OnPiggyNewLayerClick(wxMenuEvent& evt);
    void OnPiggyNewLayerAnimClick(wxMenuEvent& evt);
    void OnPiggyAddClick(wxMenuEvent& evt);
    void OnPiggyRemoveClick(wxMenuEvent& evt);
    void OnPiggyDeleteClick(wxMenuEvent& evt);
};

wxTreeItemId TreeAddMaterial(wxTreeCtrl* tree, const wxTreeItemId& parent, plResManager* mgr, plKey key);

#endif
