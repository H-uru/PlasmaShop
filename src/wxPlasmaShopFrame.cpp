#include "wxPlasmaShopFrame.h"

BEGIN_EVENT_TABLE(wxPlasmaShopFrame, wxFrame)
    //
END_EVENT_TABLE()

wxPlasmaShopFrame::wxPlasmaShopFrame(wxApp* owner)
    : wxFrame(NULL, wxID_ANY, wxT("PlasmaShop 3.0"), wxDefaultPosition, wxSize(640, 480)),
      fOwner(owner)
{
    fResMgr = new plResManager();
    fAuiMgr = new wxAuiManager(this);

    // GUI Elements
    fEditorBook = new wxAuiNotebook(this, wxID_ANY, wxDefaultPosition,
                                    wxSize(400, 400));
    fFileTree = new wxTreeCtrl(this, wxID_ANY, wxDefaultPosition,
                               wxSize(240, 400),
                               wxTR_HAS_BUTTONS | wxTR_HIDE_ROOT);

    // Menu, Status Bar
    wxMenu* mnuFile = new wxMenu();
    mnuFile->Append(wxID_OPEN, wxT("&Open"), wxT("Open a file for editing"));
    mnuFile->AppendSeparator();
    mnuFile->Append(wxID_EXIT, wxT("E&xit"), wxT("Exit PlasmaShop"));

    wxMenuBar* menuBar = new wxMenuBar();
    menuBar->Append(mnuFile, wxT("&File"));

    SetMenuBar(menuBar);
    SetStatusBar(new wxStatusBar(this, wxID_ANY));

    // The AUI Manager
    fAuiMgr->AddPane(fEditorBook, wxCENTER, wxEmptyString);
    fAuiMgr->AddPane(fFileTree, wxLEFT, wxT("File Browser"));
    fAuiMgr->Update();

    // Test
    fEditorBook->AddPage(new wxTextCtrl(fEditorBook, wxID_ANY, wxEmptyString,
                                        wxDefaultPosition, wxDefaultSize,
                                        wxTE_MULTILINE),
                         wxT("Test Editor"), true);
    fEditorBook->AddPage(new wxPanel(fEditorBook, wxID_ANY),
                         wxT("Blah"), false);
}

wxPlasmaShopFrame::~wxPlasmaShopFrame()
{
    delete fResMgr;
}
