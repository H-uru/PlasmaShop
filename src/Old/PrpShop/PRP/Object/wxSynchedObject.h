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

#ifndef _WXSYNCHEDOBJECT_H
#define _WXSYNCHEDOBJECT_H

#include "../../wxPrpPlasma.h"
#include <wx/checkbox.h>
#include <wx/stattext.h>
#include <wx/listbox.h>

class wxSynchedObject : public wxPrpPlasmaObject {
protected:
    wxCheckBox* cbDontDirty;
    wxCheckBox* cbSendReliably;
    wxCheckBox* cbHasConstNetGroup;
    wxCheckBox* cbDontSynchGameMsgs;
    wxCheckBox* cbExcludePersistent;
    wxCheckBox* cbExcludeAllPersist;
    wxCheckBox* cbHasVolatile;
    wxCheckBox* cbAllVolatile;
    wxStaticText* lblExcludes;
    wxStaticText* lblVolatiles;
    wxListBox* listExcludes;
    wxListBox* listVolatiles;

public:
    wxSynchedObject(plKey key, plResManager* mgr, wxTreeCtrl* tree, const wxTreeItemId& tid);
    virtual void AddPropPages(wxNotebook* nb);
    virtual void SaveDamage();

protected:
    enum { ID_MENU_ADD, ID_MENU_DELETE };

    void OnExcludeContextMenu(wxCommandEvent& evt);
    void OnVolatileContextMenu(wxCommandEvent& evt);
    void OnExcludeAddClick(wxCommandEvent& evt);
    void OnExcludeDeleteClick(wxCommandEvent& evt);
    void OnVolatileAddClick(wxCommandEvent& evt);
    void OnVolatileDeleteClick(wxCommandEvent& evt);
    void OnExcludePersistCheck(wxCommandEvent& evt);
    void OnHasVolatileCheck(wxCommandEvent& evt);
};

#endif
