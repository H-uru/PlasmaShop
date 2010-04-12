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

#ifndef _WXOBJSELECTFRAME_H
#define _WXOBJSELECTFRAME_H

#include <wx/wxprec.h>
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "PRP/wxPrpGUI.h"
#include <ResManager/plResManager.h>

class wxObjSelectFrame : public wxDialog {
protected:
    wxKeyListCtrl* lsObjects;
    hsTArray<plKey> fKeys;

public:
    wxObjSelectFrame(wxWindow* parent, wxWindowID id, const wxString& title,
                     const wxPoint& pos = wxDefaultPosition,
                     const wxSize& size = wxDefaultSize,
                     long style = wxDEFAULT_DIALOG_STYLE,
                     const wxString& name = wxDialogNameStr);
    void AddObject(plKey key);

protected:
    enum { ID_OBJLIST };
    DECLARE_EVENT_TABLE()

    void OnOk(wxCommandEvent& evt);
    void OnCancel(wxCommandEvent& evt);
    void OnItemActivated(wxListEvent& evt);
};

#endif
