#ifndef _WXPRPSHOPFRAME_H
#define _WXPRPSHOPFRAME_H

#include <wx/wxprec.h>
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <list>
#include <wx/treectrl.h>
#include <wx/notebook.h>
#include <ResManager/plResManager.h>
#include "wxPrpCanvas.h"
#include "wxPrpPlasma.h"

class wxPrpShopFrame : public wxFrame {
protected:
    // UI Components
    wxApp* fOwner;
    wxTreeCtrl* fObjTree;
    wxNotebook* fPropertyBook;
    plResManager* fResMgr;
    std::map<plLocation, wxTreeItemId> fLoadedLocations;
    wxPrpPlasmaObject* fCurObject;

    enum { ID_OBJTREE, ID_PROPERTYBOOK };

public:
    wxPrpShopFrame(wxApp* owner);
    virtual ~wxPrpShopFrame();

    void LoadFile(const wxString& filename);
    wxTreeItemId LoadPage(plPageInfo* page);

protected:
    // Event Handling
    DECLARE_EVENT_TABLE()

    void OnExitClick(wxCommandEvent& evt);
    void OnNewClick(wxCommandEvent& evt);
    void OnOpenClick(wxCommandEvent& evt);
    void OnClose(wxCloseEvent& evt);
    void OnTreeChanged(wxTreeEvent& evt);
};

#endif
