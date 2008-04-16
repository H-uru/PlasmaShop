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
{ }

void wxPlasmaShopFrame::LoadFile(const wxString& filename)
{
    wxFileName fn(filename);

    if (!fn.FileExists()) {
        wxMessageBox(wxString::Format(wxT("Error: File %s not found!"), filename.wx_str()),
                     wxT("Error"), wxOK | wxICON_ERROR);
        return;
    }

    try {
        wxString ext = fn.GetExt();
        if (ext.CmpNoCase(wxT("age")) == 0 || ext.CmpNoCase(wxT("ini")) == 0) {
            wxPlasmaTextCtrl* ptc = new wxPlasmaTextCtrl(this);
            fEditorBook->AddPage(ptc, fn.GetFullName(), true);
            ptc->DoLoad(filename);
            ptc->SetSyntaxMode(wxPlasmaTextCtrl::kSynAgeIni);
        } else if (ext.CmpNoCase(wxT("elf")) == 0) {
            wxPlasmaTextCtrl* ptc = new wxPlasmaTextCtrl(this);
            fEditorBook->AddPage(ptc, fn.GetFullName(), true);
            ptc->LoadElf(filename);
            ptc->SetSyntaxMode(wxPlasmaTextCtrl::kSynNone);
            ptc->SetReadOnly(true);
        } else if (ext.CmpNoCase(wxT("fni")) == 0) {
            wxPlasmaTextCtrl* ptc = new wxPlasmaTextCtrl(this);
            fEditorBook->AddPage(ptc, fn.GetFullName(), true);
            ptc->DoLoad(filename);
            ptc->SetSyntaxMode(wxPlasmaTextCtrl::kSynConsole);
        } else if (ext.CmpNoCase(wxT("fx")) == 0) {
            wxPlasmaTextCtrl* ptc = new wxPlasmaTextCtrl(this);
            fEditorBook->AddPage(ptc, fn.GetFullName(), true);
            ptc->DoLoad(filename);
            ptc->SetSyntaxMode(wxPlasmaTextCtrl::kSynFX);
        } else if (ext.CmpNoCase(wxT("hex")) == 0) {
            wxPlasmaTextCtrl* ptc = new wxPlasmaTextCtrl(this);
            fEditorBook->AddPage(ptc, fn.GetFullName(), true);
            ptc->DoLoad(filename);
            ptc->SetSyntaxMode(wxPlasmaTextCtrl::kSynHex);
        } else if (ext.CmpNoCase(wxT("loc")) == 0 || ext.CmpNoCase(wxT("sub")) == 0) {
            wxPlasmaTextCtrl* ptc = new wxPlasmaTextCtrl(this);
            fEditorBook->AddPage(ptc, fn.GetFullName(), true);
            ptc->DoLoad(filename);
            ptc->SetSyntaxMode(wxPlasmaTextCtrl::kSynXML);
        } else if (ext.CmpNoCase(wxT("log")) == 0 || ext.CmpNoCase(wxT("txt")) == 0) {
            wxPlasmaTextCtrl* ptc = new wxPlasmaTextCtrl(this);
            fEditorBook->AddPage(ptc, fn.GetFullName(), true);
            ptc->DoLoad(filename);
            ptc->SetSyntaxMode(wxPlasmaTextCtrl::kSynNone);
        } else if (ext.CmpNoCase(wxT("py")) == 0) {
            wxPlasmaTextCtrl* ptc = new wxPlasmaTextCtrl(this);
            fEditorBook->AddPage(ptc, fn.GetFullName(), true);
            ptc->DoLoad(filename);
            ptc->SetSyntaxMode(wxPlasmaTextCtrl::kSynPython);
        } else if (ext.CmpNoCase(wxT("sdl")) == 0) {
            wxPlasmaTextCtrl* ptc = new wxPlasmaTextCtrl(this);
            fEditorBook->AddPage(ptc, fn.GetFullName(), true);
            ptc->DoLoad(filename);
            wxString sdlTxt = ptc->GetText();
            if (sdlTxt.Find(wxT("struct")) != wxNOT_FOUND)
                ptc->SetSyntaxMode(wxPlasmaTextCtrl::kSynSDL_Eoa);
            else
                ptc->SetSyntaxMode(wxPlasmaTextCtrl::kSynSDL_Uru);
        } else {
            wxMessageBox(wxString::Format(wxT("%s: Unsupported file type!"), filename.wx_str()),
                         wxT("Error"), wxOK | wxICON_ERROR, this);
        }
    } catch (std::exception& e) {
        wxMessageBox(wxString::Format(wxT("Error loading %s: %s"),
                     filename.wx_str(), wxString(e.what(), wxConvUTF8).wx_str()),
                     wxT("Error"), wxOK | wxICON_ERROR, this);
    }
}

void wxPlasmaShopFrame::OnExitClick(wxCommandEvent& evt)
{
    Close();
}

void wxPlasmaShopFrame::OnOpenClick(wxCommandEvent& evt)
{
    static const wxString kFilter =
        wxT("All supported files|*.age;*.fni;*.sum;*.ini;*.tga;*.pfp;*.p2f;*.hex;*.loc;*.sub;*.elf;*.log*.pak;*.py;*.sdl;*.fx;*.txt|")
        wxT("Age files (*.age, *.fni, *.sum)|*.age;*.fni;*.sum|")
        wxT("Config files (*.ini, *.fni)|*.ini;*.fni|")
        wxT("Cursor files (*.dat, *.tga)|*.dat;*.tga|")
        wxT("Font files (*.pfp, *.p2f)|*.pfp;*.p2f|")
        wxT("Hex Isle levels (*.hex)|*.hex|")
        wxT("Localization files (*.loc, *.sub)|*.loc;*.sub|")
        wxT("Log files (*.elf, *.log)|*.elf;*.log|")
        wxT("Python files (*.pak, *.py)|*.pak;*.py|")
        wxT("SDL files (*.sdl)|*.sdl|")
        wxT("Shader files (*.fx)|*.fx|")
        wxT("Text files (*.txt)|*.txt");

    wxFileDialog fd(this, wxT("Open file"), wxEmptyString,
                    wxEmptyString, kFilter,
                    wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_MULTIPLE);
    if (fd.ShowModal() != wxID_CANCEL) {
        wxArrayString paths;
        fd.GetPaths(paths);
        for (size_t i=0; i<paths.GetCount(); i++)
            LoadFile(paths[i]);
    }
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
