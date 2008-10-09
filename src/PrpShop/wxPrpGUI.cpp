#include "wxPrpGUI.h"
#include "wxPrpPlasma.h"
#include <ResManager/plFactory.h>

/* wxLinkText */
DEFINE_EVENT_TYPE(wxEVT_COMMAND_LINK)
IMPLEMENT_DYNAMIC_CLASS(wxLinkText, wxStaticText)

BEGIN_EVENT_TABLE(wxLinkText, wxStaticText)
    EVT_LEFT_DOWN(wxLinkText::OnLeftDown)
    EVT_LEFT_UP(wxLinkText::OnLeftUp)
END_EVENT_TABLE()

wxLinkText::wxLinkText()
{ }

wxLinkText::wxLinkText(wxWindow* parent, wxWindowID id, const wxString& label,
                       const wxPoint& pos, const wxSize& size, long style,
                       const wxString& name)
          : wxStaticText(parent, id, label, pos, size, style, name)
{
    SetForegroundColour(0xFF0000);
    Enable(true);
}

bool wxLinkText::Enable(bool enable)
{
    wxFont fnt = GetFont();
    fnt.SetUnderlined(enable);
    SetFont(fnt);
    SetCursor(wxCursor(enable ? wxCURSOR_HAND : wxCURSOR_DEFAULT));

    return wxStaticText::Enable(enable);
}

void wxLinkText::OnLeftDown(wxMouseEvent& evt)
{
    fIsClicking = true;
}

void wxLinkText::OnLeftUp(wxMouseEvent& evt)
{
    if (!fIsClicking)
        return;
    fIsClicking = false;

    wxCommandEvent cmdEvent(wxEVT_COMMAND_LINK, GetId());
    cmdEvent.SetEventObject(this);
    GetEventHandler()->ProcessEvent(cmdEvent);
}


/* wxKeyLabel */
wxKeyLabel::wxKeyLabel(wxWindow* parent, wxWindowID id, plKey key,
                       const wxPoint& pos, const wxSize& size, long style,
                       const wxString& name)
          : wxLinkText(parent, id, wxT("(none)"), pos, size, style, name)
{
    SetKey(key);
}

void wxKeyLabel::SetKey(plKey key)
{
    if (key.Exists()) {
        SetLabel(wxString(key->getName().cstr(), wxConvUTF8));
        Enable(true);
    } else {
        SetLabel(wxT("(None)"));
        Enable(false);
    }
}


/* wxKeyListCtrl */
wxKeyListCtrl::wxKeyListCtrl(wxWindow* parent, wxWindowID id, const wxPoint& pos,
                             const wxSize& size, long style,
                             const wxValidator& validator, const wxString& name)
         : wxListCtrl(parent, id, pos, size, style, validator, name)
{
    long width = GetClientSize().GetWidth() - 20;
    InsertColumn(0, wxT("Name"), wxLIST_FORMAT_LEFT, (3 * width) / 5);
    InsertColumn(1, wxT("Type"), wxLIST_FORMAT_LEFT, (2 * width) / 5);
}

void wxKeyListCtrl::AddKey(plKey key)
{
    long idx = GetItemCount();
    long which = InsertItem(idx, wxString(key->getName().cstr(), wxConvUTF8));
    SetItem(which, 1, wxString(pdUnifiedTypeMap::ClassName(key->getType()), wxConvUTF8));
    SetItemColumnImage(which, 0, GetTypeIcon(key->getType()));
}
