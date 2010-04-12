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

#ifndef _WXSCENEOBJECT_H
#define _WXSCENEOBJECT_H

#include "wxSynchedObject.h"
#include "../wxPrpGUI.h"
#include "../../wxPrpCanvas.h"

class wxSceneObject : public wxSynchedObject {
protected:
    wxKeyLabel* lblDraw;
    wxKeyLabel* lblSim;
    wxKeyLabel* lblCoord;
    wxKeyLabel* lblAudio;
    wxLinkText* lblADDraw;
    wxLinkText* lblADSim;
    wxLinkText* lblADCoord;
    wxLinkText* lblADAudio;
    void UpdateIntfLinks();

    wxKeyListCtrl* lsInterfaces;
    wxKeyListCtrl* lsModifiers;

    wxPrpCanvas* fPreviewCanvas;

public:
    wxSceneObject(plKey key, plResManager* mgr, wxTreeCtrl* tree, const wxTreeItemId& tid);
    virtual void AddPropPages(wxNotebook* nb);
    virtual wxWindow* MakePreviewPane(wxWindow* parent);
    //virtual void SaveDamage();
    virtual void Refresh();

protected:
    // Events
    void OnDrawIntfClick(wxCommandEvent& evt);
    void OnSimIntfClick(wxCommandEvent& evt);
    void OnCoordIntfClick(wxCommandEvent& evt);
    void OnAudioIntfClick(wxCommandEvent& evt);

    void OnADDrawClick(wxCommandEvent& evt);
    void OnADSimClick(wxCommandEvent& evt);
    void OnADCoordClick(wxCommandEvent& evt);
    void OnADAudioClick(wxCommandEvent& evt);

    // Interface and Modifier popup items
    enum {
        ID_OBJ_DELETE,
        ID_MOD_NEW_RESPONDER, ID_MOD_NEW_PYTHON,
    };

    void OnIntfContextMenu(wxCommandEvent& evt);
    void OnModContextMenu(wxCommandEvent& evt);

    void OnIntfItemActivated(wxListEvent& evt);
    void OnIntfDeleteClick(wxCommandEvent& evt);

    void OnModItemActivated(wxListEvent& evt);
    void OnModDeleteClick(wxCommandEvent& evt);
    void OnNewPythonClick(wxCommandEvent& evt);
    void OnNewResponderClick(wxCommandEvent& evt);
};

wxTreeItemId TreeAddSceneObject(wxTreeCtrl* tree, const wxTreeItemId& parent, plResManager* mgr, plKey key);

#endif
