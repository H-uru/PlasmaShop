#ifndef _WXPLASMASHOPFRAME_H
#define _WXPLASMASHOPFRAME_H

#include <wx/wxprec.h>
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <wx/aui/aui.h>
#include <wx/treectrl.h>

#include "../wxPlasmaTextCtrl.h"

class wxPlasmaShopFrame : public wxFrame {
protected:
    // UI Components
    wxApp* fOwner;
    wxAuiManager* fAuiMgr;
    wxTreeCtrl* fFileTree;
    wxAuiNotebook* fEditorBook;

public:
    wxPlasmaShopFrame(wxApp* owner);
    virtual ~wxPlasmaShopFrame();

    void LoadFile(const wxString& filename);

protected:
    // Event Handling
    DECLARE_EVENT_TABLE()

    void OnExitClick(wxCommandEvent& evt);
    void OnNewClick(wxCommandEvent& evt);
    void OnOpenClick(wxCommandEvent& evt);
    void OnClose(wxCloseEvent& evt);

    void OnStcDirty(wxStyledTextEvent& evt);
    void OnStcClean(wxStyledTextEvent& evt);
};

#endif
