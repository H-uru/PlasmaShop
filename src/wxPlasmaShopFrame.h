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
#include "PubUtilLib/plResMgr/plResManager.h"

class wxPlasmaShopFrame : public wxFrame {
protected:
    // UI Components
    wxApp* fOwner;
    wxAuiManager* fAuiMgr;
    wxTreeCtrl* fFileTree;
    wxNotebook* fEditorBook;

protected:
    // Plasma Utilities
    plResManager* fResMgr;

public:
    wxPlasmaShopFrame(wxApp* owner);
    virtual ~wxPlasmaShopFrame();

private:
    // Event Handling
    DECLARE_EVENT_TABLE()
};

#endif
