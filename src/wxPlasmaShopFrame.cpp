#include "wxPlasmaShopFrame.h"

BEGIN_EVENT_TABLE(wxPlasmaShopFrame, wxFrame)
    EVT_MENU(wxID_EXIT, wxPlasmaShopFrame::OnExitClick)
    EVT_CLOSE(wxPlasmaShopFrame::OnClose)
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

    // Test editors
    wxPlasmaTextCtrl* stcPlasma = new wxPlasmaTextCtrl(this, wxID_ANY);
    stcPlasma->SetSyntaxMode(wxPlasmaTextCtrl::kSynPlasma);
    fEditorBook->AddPage(stcPlasma, wxT("Plasma"), true);
    wxPlasmaTextCtrl* stcPython = new wxPlasmaTextCtrl(this, wxID_ANY);
    stcPython->SetSyntaxMode(wxPlasmaTextCtrl::kSynPython);
    fEditorBook->AddPage(stcPython, wxT("Python"), true);
    wxPlasmaTextCtrl* stcSDL1 = new wxPlasmaTextCtrl(this, wxID_ANY);
    stcSDL1->SetSyntaxMode(wxPlasmaTextCtrl::kSynSDL_Uru);
    fEditorBook->AddPage(stcSDL1, wxT("SDL (Uru)"), true);
    wxPlasmaTextCtrl* stcSDL2 = new wxPlasmaTextCtrl(this, wxID_ANY);
    stcSDL2->SetSyntaxMode(wxPlasmaTextCtrl::kSynSDL_Eoa);
    fEditorBook->AddPage(stcSDL2, wxT("SDL (Eoa)"), true);
    wxPlasmaTextCtrl* stcAge = new wxPlasmaTextCtrl(this, wxID_ANY);
    stcAge->SetSyntaxMode(wxPlasmaTextCtrl::kSynAgeIni);
    fEditorBook->AddPage(stcAge, wxT("Age/Ini"), true);
    wxPlasmaTextCtrl* stcConsole = new wxPlasmaTextCtrl(this, wxID_ANY);
    stcConsole->SetSyntaxMode(wxPlasmaTextCtrl::kSynConsole);
    fEditorBook->AddPage(stcConsole, wxT("Console"), true);

    // The AUI Manager
    fAuiMgr->AddPane(fEditorBook, wxCENTER, wxEmptyString);
    fAuiMgr->AddPane(fFileTree, wxLEFT, wxT("File Browser"));
    fAuiMgr->Update();
}

wxPlasmaShopFrame::~wxPlasmaShopFrame()
{
    delete fResMgr;
}

void wxPlasmaShopFrame::OnExitClick(wxCommandEvent& evt)
{
    Close();
}

void wxPlasmaShopFrame::OnClose(wxCloseEvent& evt)
{
    if (evt.CanVeto()) {
        // Check for unsaved work first
        Destroy();
    } else {
        Destroy();
    }
}
