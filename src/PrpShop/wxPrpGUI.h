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

class wxKeyListCtrl : public wxListCtrl {
public:
    wxKeyListCtrl(wxWindow* parent, wxWindowID id,
                  const wxPoint& pos = wxDefaultPosition,
                  const wxSize& size = wxDefaultSize,
                  long style = wxLC_REPORT | wxLC_NO_HEADER,
                  const wxValidator& validator = wxDefaultValidator,
                  const wxString& name = wxT("wxKeyList"));
    void AddKey(plKey key);
};

#endif
