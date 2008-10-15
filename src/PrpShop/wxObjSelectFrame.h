#ifndef _WXOBJSELECTFRAME_H
#define _WXOBJSELECTFRAME_H

#include <wx/wxprec.h>
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "wxPrpGUI.h"
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
