#include "wxPlasmaShopFrame.h"

BEGIN_EVENT_TABLE(wxPlasmaShopFrame, wxFrame)
    //
END_EVENT_TABLE()

wxPlasmaShopFrame::wxPlasmaShopFrame(wxApp* owner)
    : wxFrame(NULL, wxID_ANY, wxT("PlasmaShop 3.0")), fOwner(owner)
{
    fResMgr = new plResManager();
    fAuiMgr = new wxAuiManager(this);

    fFileTree = new wxTreeCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                               wxTR_HAS_BUTTONS | wxTR_HIDE_ROOT);
    fEditorBook = new wxNotebook(this, wxID_ANY, wxDefaultPosition,
                                 wxSize(240, -1), wxNB_TOP);
    fAuiMgr->AddPane(fFileTree, wxLEFT, wxT("File Browser"));
    fAuiMgr->AddPane(fEditorBook, wxTOP, wxEmptyString);

    SetStatusBar(new wxStatusBar(this, wxID_ANY));
}

wxPlasmaShopFrame::~wxPlasmaShopFrame()
{
    delete fResMgr;
}
