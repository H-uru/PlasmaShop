#include "wxPlasmaShopFrame.h"
#include "../../rc/PlasmaShop.xpm"
#include <wx/filename.h>
#include <CoreLib/plDebug.h>

BEGIN_EVENT_TABLE(wxPlasmaShopFrame, wxFrame)
    EVT_MENU(wxID_EXIT, wxPlasmaShopFrame::OnExitClick)
    EVT_MENU(wxID_OPEN, wxPlasmaShopFrame::OnOpenClick)
    EVT_CLOSE(wxPlasmaShopFrame::OnClose)
    EVT_STC_SAVEPOINTLEFT(wxID_ANY, wxPlasmaShopFrame::OnStcDirty)
    EVT_STC_SAVEPOINTREACHED(wxID_ANY, wxPlasmaShopFrame::OnStcClean)
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

    // Miscellaneous
    SetIcon(wxIcon(XPM_PlasmaShop));
}

wxPlasmaShopFrame::~wxPlasmaShopFrame()
{
    delete fResMgr;
}

void wxPlasmaShopFrame::LoadFile(const wxString& filename)
{
    wxFileName fn(filename);

    if (!fn.FileExists()) {
        wxMessageBox(wxString::Format(wxT("Error: File %s not found!"), filename.c_str()),
                     wxT("Error"), wxOK | wxICON_ERROR);
        return;
    }

    wxString ext = fn.GetExt();
    if (ext == wxT("py")) {
        wxPlasmaTextCtrl* ptc = new wxPlasmaTextCtrl(this);
        ptc->DoLoad(filename);
        ptc->SetSyntaxMode(wxPlasmaTextCtrl::kSynPython);
        fEditorBook->AddPage(ptc, fn.GetFullName(), true);
    } else if (ext == wxT("age") || ext == wxT("ini")) {
        wxPlasmaTextCtrl* ptc = new wxPlasmaTextCtrl(this);
        ptc->DoLoad(filename);
        ptc->SetSyntaxMode(wxPlasmaTextCtrl::kSynAgeIni);
        fEditorBook->AddPage(ptc, fn.GetFullName(), true);
    } else if (ext == wxT("fni")) {
        wxPlasmaTextCtrl* ptc = new wxPlasmaTextCtrl(this);
        ptc->DoLoad(filename);
        ptc->SetSyntaxMode(wxPlasmaTextCtrl::kSynConsole);
        fEditorBook->AddPage(ptc, fn.GetFullName(), true);
    } else if (ext == wxT("log") || ext == wxT("txt")) {
        wxPlasmaTextCtrl* ptc = new wxPlasmaTextCtrl(this);
        ptc->DoLoad(filename);
        ptc->SetSyntaxMode(wxPlasmaTextCtrl::kSynNone);
        fEditorBook->AddPage(ptc, fn.GetFullName(), true);
    } else if (ext == wxT("elf")) {
        wxPlasmaTextCtrl* ptc = new wxPlasmaTextCtrl(this);
        ptc->LoadElf(filename);
        ptc->SetSyntaxMode(wxPlasmaTextCtrl::kSynNone);
        ptc->SetReadOnly(true);
        fEditorBook->AddPage(ptc, fn.GetFullName(), true);
    }
}

void wxPlasmaShopFrame::OnExitClick(wxCommandEvent& evt)
{
    Close();
}

void wxPlasmaShopFrame::OnOpenClick(wxCommandEvent& evt)
{
    static const wxString kFilter =
        wxT("All supported files|*.age;*.fni;*.sum;*.ini;*.tga;*.pfp;*.p2f;*.hex;*.elf;*.log*.pak;*.py;*.sdl;*.fx;*.txt|")
        wxT("Age files (*.age, *.fni, *.sum)|*.age;*.fni;*.sum|")
        wxT("Config files (*.ini, *.fni)|*.ini;*.fni|")
        wxT("Cursor files (*.dat, *.tga)|*.dat;*.tga|")
        wxT("Font files (*.pfp, *.p2f)|*.pfp;*.p2f|")
        wxT("Hex Isle levels (*.hex)|*.hex|")
        wxT("Log files (*.elf, *.log)|*.elf;*.log|")
        wxT("Python files (*.pak, *.py)|*.pak;*.py|")
        wxT("SDL files (*.sdl)|*.sdl|")
        wxT("Shader files (*.fx)|*.fx|")
        wxT("Text files (*.txt)|*.txt");

    wxFileDialog fd(this, wxT("Open file"), wxEmptyString,
                    wxEmptyString, kFilter, wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (fd.ShowModal() != wxID_CANCEL)
        LoadFile(fd.GetPath());
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

void wxPlasmaShopFrame::OnStcDirty(wxStyledTextEvent& evt)
{
    int pid = fEditorBook->GetPageIndex((wxWindow*)evt.GetEventObject());
    if (pid == wxNOT_FOUND)
        return;

    wxString pageText = fEditorBook->GetPageText(pid);
    if (pageText.Right(1) != wxT("*"))
        fEditorBook->SetPageText(pid, pageText + wxT(" *"));
}

void wxPlasmaShopFrame::OnStcClean(wxStyledTextEvent& evt)
{
    int pid = fEditorBook->GetPageIndex((wxWindow*)evt.GetEventObject());
    if (pid == wxNOT_FOUND)
        return;

    wxString pageText = fEditorBook->GetPageText(pid);
    if (pageText.Right(1) == wxT("*"))
        fEditorBook->SetPageText(pid, pageText.Left(pageText.size() - 2));
}
