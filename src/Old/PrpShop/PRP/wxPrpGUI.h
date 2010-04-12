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

#ifndef _WXPRPGUI_H
#define _WXPRPGUI_H

#include <PRP/KeyedObject/plKey.h>
#include <wx/stattext.h>
#include <wx/listctrl.h>

BEGIN_DECLARE_EVENT_TYPES()
    DECLARE_EVENT_TYPE(wxEVT_COMMAND_LINK, -1)
END_DECLARE_EVENT_TYPES()

class wxLinkText : public wxStaticText {
public:
    wxLinkText();
    wxLinkText(wxWindow* parent, wxWindowID id, const wxString& label,
               const wxPoint& pos = wxDefaultPosition,
               const wxSize& size = wxDefaultSize,
               long style = 0, const wxString& name = wxT("wxLinkText"));

    virtual bool Enable(bool enable = true);

protected:
    DECLARE_DYNAMIC_CLASS(wxLinkText)
    DECLARE_EVENT_TABLE()

    void OnLeftDown(wxMouseEvent& evt);
    void OnLeftUp(wxMouseEvent& evt);

private:
    bool fIsClicking;
};

class wxKeyLabel : public wxLinkText {
public:
    wxKeyLabel(wxWindow* parent, wxWindowID id, plKey key,
               const wxPoint& pos = wxDefaultPosition,
               const wxSize& size = wxDefaultSize,
               long style = 0, const wxString& name = wxT("wxKeyLabel"));
    void SetKey(plKey key);
};

#define wxKEYLIST_DEFAULT_STYLE (wxLC_REPORT | wxLC_NO_HEADER | wxBORDER_SUNKEN)

class wxKeyListCtrl : public wxListCtrl {
public:
    wxKeyListCtrl(wxWindow* parent, wxWindowID id,
                  const wxPoint& pos = wxDefaultPosition,
                  const wxSize& size = wxDefaultSize,
                  long style = wxKEYLIST_DEFAULT_STYLE,
                  const wxValidator& validator = wxDefaultValidator,
                  const wxString& name = wxT("wxKeyList"));
    void AddKey(plKey key);
};

#endif
