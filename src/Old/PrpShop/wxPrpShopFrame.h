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
#include <wx/splitter.h>
#include <ResManager/plResManager.h>
#include "wxPrpCanvas.h"
#include "wxPrpPlasma.h"

struct wxLocationInfo {
    wxTreeItemId fTreeId;
    wxString fFilename;

    explicit wxLocationInfo(const wxTreeItemId& tid, const wxString& filename);
    wxLocationInfo(const wxLocationInfo& init);
    wxLocationInfo(); // To make std::map happy
};

class wxPrpShopFrame : public wxFrame {
protected:
    // UI Components
    wxApp* fOwner;
    wxTreeCtrl* fObjTree;
    wxNotebook* fPropertyBook;
    wxWindow* fViewerPane;
    wxSplitterWindow* fHSplitter;
    wxSplitterWindow* fVSplitter;

    plResManager* fResMgr;
    std::map<plLocation, wxLocationInfo> fLoadedLocations;
    wxPrpPlasmaObject* fCurObject;
    std::map<size_t, int> fEditorPageMemory;
    int fCachedDrawMode;

    enum {
        ID_OBJTREE, ID_PROPERTYBOOK,
        ID_VIEW_POINTS, ID_VIEW_WIRE, ID_VIEW_FLAT, ID_VIEW_TEXTURED,
        ID_VIEW_FORCE2SIDED
    };

private:
    // Page Editor Components
    plPageInfo* fCurPage;
    wxTextCtrl* txtAge;
    wxTextCtrl* txtPage;
    wxTextCtrl* txtSeqPre;
    wxTextCtrl* txtSeqSuf;
    wxCheckBox* cbLocalOnly;
    wxCheckBox* cbVolatile;
    wxCheckBox* cbReserved;
    wxCheckBox* cbBuiltIn;
    wxCheckBox* cbItinerant;

public:
    wxPrpShopFrame(wxApp* owner);
    virtual ~wxPrpShopFrame();

    void LoadFile(const wxString& filename);
    wxLocationInfo LoadPage(plPageInfo* page, const wxString& filename);

protected:
    // Event Handling
    DECLARE_EVENT_TABLE()

    void OnExitClick(wxCommandEvent& evt);
    void OnNewClick(wxCommandEvent& evt);
    void OnOpenClick(wxCommandEvent& evt);
    void OnSaveClick(wxCommandEvent& evt);
    void OnSaveAsClick(wxCommandEvent& evt);
    void OnViewPointsClick(wxCommandEvent& evt);
    void OnViewWireClick(wxCommandEvent& evt);
    void OnViewFlatClick(wxCommandEvent& evt);
    void OnViewTexturedClick(wxCommandEvent& evt);
    void OnViewForce2SidedClick(wxCommandEvent& evt);
    void OnClose(wxCloseEvent& evt);
    void OnTreeChanged(wxTreeEvent& evt);

    void UpdateMenuAndToolbar();
    plLocation GetActiveLocation();
    void DoDataSave(bool doDelete);
};

#endif
