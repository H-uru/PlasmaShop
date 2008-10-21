#include "wxPlasmaShopFrame.h"
#include "../../rc/PlasmaShop.xpm"

#include <Debug/plDebug.h>
#include <wx/filename.h>
#include <wx/sysopt.h>
#include <wx/config.h>
#include <wx/artprov.h>

#include "wxNewFileFrame.h"

BEGIN_EVENT_TABLE(wxPlasmaShopFrame, wxFrame)
    EVT_MENU(wxID_EXIT, wxPlasmaShopFrame::OnExitClick)
    EVT_MENU(wxID_NEW, wxPlasmaShopFrame::OnNewClick)
    EVT_MENU(wxID_OPEN, wxPlasmaShopFrame::OnOpenClick)
    EVT_MENU(wxID_SAVE, wxPlasmaShopFrame::OnSaveClick)
    EVT_MENU(wxID_SAVEAS, wxPlasmaShopFrame::OnSaveAsClick)
    EVT_CLOSE(wxPlasmaShopFrame::OnClose)

    EVT_AUINOTEBOOK_PAGE_CLOSE(wxID_ANY, wxPlasmaShopFrame::OnPageClose)
    EVT_AUINOTEBOOK_PAGE_CHANGED(wxID_ANY, wxPlasmaShopFrame::OnPageChange)
    EVT_STC_SAVEPOINTLEFT(wxID_ANY, wxPlasmaShopFrame::OnStcDirty)
    EVT_STC_SAVEPOINTREACHED(wxID_ANY, wxPlasmaShopFrame::OnStcClean)

    EVT_MENU(ID_TEXT_HL_NONE, wxPlasmaShopFrame::OnTextHLNoneClick)
    EVT_MENU(ID_TEXT_HL_PYTHON, wxPlasmaShopFrame::OnTextHLPythonClick)
    EVT_MENU(ID_TEXT_HL_SDL1, wxPlasmaShopFrame::OnTextHLSdl1Click)
    EVT_MENU(ID_TEXT_HL_SDL2, wxPlasmaShopFrame::OnTextHLSdl2Click)
    EVT_MENU(ID_TEXT_HL_INI, wxPlasmaShopFrame::OnTextHLIniClick)
    EVT_MENU(ID_TEXT_HL_CONSOLE, wxPlasmaShopFrame::OnTextHLConsoleClick)
    EVT_MENU(ID_TEXT_HL_XML, wxPlasmaShopFrame::OnTextHLXmlClick)
    EVT_MENU(ID_TEXT_HL_FX, wxPlasmaShopFrame::OnTextHLFxClick)
    EVT_MENU(ID_TEXT_HL_HEX, wxPlasmaShopFrame::OnTextHLHexClick)
    EVT_MENU(ID_TEXT_ENC_NONE, wxPlasmaShopFrame::OnTextEncNoneClick)
    EVT_MENU(ID_TEXT_ENC_XTEA, wxPlasmaShopFrame::OnTextEncXteaClick)
    EVT_MENU(ID_TEXT_ENC_DROID, wxPlasmaShopFrame::OnTextEncDroidClick)
    EVT_MENU(ID_TEXT_ENC_AES, wxPlasmaShopFrame::OnTextEncAesClick)
    EVT_MENU(ID_TEXT_U_ANSI, wxPlasmaShopFrame::OnTextUAnsiClick)
    EVT_MENU(ID_TEXT_U_UTF8, wxPlasmaShopFrame::OnTextUUtf8Click)
    EVT_MENU(ID_TEXT_U_UTF16LE, wxPlasmaShopFrame::OnTextUUtf16LEClick)
    EVT_MENU(ID_TEXT_U_UTF16BE, wxPlasmaShopFrame::OnTextUUtf16BEClick)
    EVT_MENU(ID_TEXT_U_UTF32LE, wxPlasmaShopFrame::OnTextUUtf32LEClick)
    EVT_MENU(ID_TEXT_U_UTF32BE, wxPlasmaShopFrame::OnTextUUtf32BEClick)
END_EVENT_TABLE()

wxPlasmaShopFrame::wxPlasmaShopFrame(wxApp* owner)
    : wxFrame(NULL, wxID_ANY, wxT("PlasmaShop 3.0"), wxDefaultPosition, wxSize(800, 600)),
      fOwner(owner), fEditorMenu(NULL)
{
    // GUI Elements
    fEditorBook = new wxAuiNotebook(this, wxID_ANY, wxDefaultPosition,
                                    wxSize(400, 400),
                                    wxAUI_NB_TOP | wxAUI_NB_TAB_SPLIT |
                                    wxAUI_NB_TAB_MOVE | wxAUI_NB_SCROLL_BUTTONS |
                                    wxAUI_NB_CLOSE_ON_ACTIVE_TAB);
    fFileTree = new wxTreeCtrl(this, wxID_ANY, wxDefaultPosition,
                               wxSize(240, 400),
                               wxTR_HAS_BUTTONS | wxTR_HIDE_ROOT);

    // Toolbar
    wxSystemOptions::SetOption(wxT("msw.remap"), 0);
    wxToolBar* toolBar = new wxToolBar(this, wxID_ANY, wxDefaultPosition,
                                       wxDefaultSize, wxTB_FLAT);
    toolBar->AddTool(wxID_NEW, wxT("New"),
                     wxArtProvider::GetBitmap(wxART_NEW, wxART_TOOLBAR),
                     wxNullBitmap, wxITEM_NORMAL, wxT("New..."),
                     wxT("Create a new file..."));
    toolBar->AddTool(wxID_OPEN, wxT("Open"),
                     wxArtProvider::GetBitmap(wxART_FILE_OPEN, wxART_TOOLBAR),
                     wxNullBitmap, wxITEM_NORMAL, wxT("Open File"),
                     wxT("Open a file for editing"));
    toolBar->AddTool(wxID_SAVE, wxT("Save"),
                     wxArtProvider::GetBitmap(wxART_FILE_SAVE, wxART_TOOLBAR),
                     wxNullBitmap, wxITEM_NORMAL, wxT("Save File"),
                     wxT("Save the current file to disk"));
    toolBar->Realize();
    SetToolBar(toolBar);

    // Menu, Status Bar
    wxMenu* mnuFile = new wxMenu();
    mnuFile->Append(wxID_NEW, wxT("&New..."), wxT("Create a new file..."));
    mnuFile->Append(wxID_OPEN, wxT("&Open"), wxT("Open a file for editing"));
    mnuFile->Append(wxID_SAVE, wxT("&Save"), wxT("Save the current file to disk"));
    mnuFile->Append(wxID_SAVEAS, wxT("Save &As..."), wxT("Save the current file to another location"));
    mnuFile->AppendSeparator();
    mnuFile->Append(wxID_EXIT, wxT("E&xit"), wxT("Exit PlasmaShop"));

    wxMenuBar* menuBar = new wxMenuBar();
    menuBar->Append(mnuFile, wxT("&File"));

    SetMenuBar(menuBar);
    SetStatusBar(new wxStatusBar(this, wxID_ANY));

    // The AUI Manager / Layout
    fAuiMgr = new wxAuiManager(this);
    fAuiMgr->AddPane(fEditorBook, wxAuiPaneInfo().CentrePane().Dockable(false));
    fAuiMgr->AddPane(fFileTree, wxAuiPaneInfo().Layer(0).Left().
                     TopDockable(false).BottomDockable(false).
                     Caption(wxT("File Browser")));
    //fAuiMgr->AddPane(toolBar, wxAuiPaneInfo().ToolbarPane());
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
        } else if (ext.CmpNoCase(wxT("log")) == 0 || ext.CmpNoCase(wxT("txt")) == 0 ||
                   ext.CmpNoCase(wxT("csv")) == 0) {
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
            if (ptc->GetEncryptionType() == plEncryptedStream::kEncAES)
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

void wxPlasmaShopFrame::UpdateMenus()
{
    if (fEditorMenu != NULL) {
        GetMenuBar()->Remove(1);
        delete fEditorMenu;
        fEditorMenu = NULL;
    }

    wxWindow* page = fEditorBook->GetPage(fEditorBook->GetSelection());
    if (page == NULL)
        return;

    if (wxPlasmaTextCtrl* stcPage = wxDynamicCast(page, wxPlasmaTextCtrl)) {
        wxMenu* mnuHilight = new wxMenu();
        mnuHilight->AppendRadioItem(ID_TEXT_HL_NONE, wxT("&None"));
        mnuHilight->AppendRadioItem(ID_TEXT_HL_PYTHON, wxT("&Python"));
        mnuHilight->AppendRadioItem(ID_TEXT_HL_SDL1, wxT("SDL (&Uru / MOUL)"));
        mnuHilight->AppendRadioItem(ID_TEXT_HL_SDL2, wxT("SDL (&Myst 5 / Crowthistle / Hex Isle)"));
        mnuHilight->AppendRadioItem(ID_TEXT_HL_INI, wxT("&Age / INI"));
        mnuHilight->AppendRadioItem(ID_TEXT_HL_CONSOLE, wxT("Plasma &Console"));
        mnuHilight->AppendRadioItem(ID_TEXT_HL_XML, wxT("&XML"));
        mnuHilight->AppendRadioItem(ID_TEXT_HL_HEX, wxT("&Hex Isle Level"));
        mnuHilight->AppendRadioItem(ID_TEXT_HL_FX, wxT("Shader &FX"));

        wxMenu* mnuEncryption = new wxMenu();
        mnuEncryption->AppendRadioItem(ID_TEXT_ENC_NONE, wxT("&None"));
        mnuEncryption->AppendRadioItem(ID_TEXT_ENC_XTEA, wxT("&Uru"));
        mnuEncryption->AppendRadioItem(ID_TEXT_ENC_DROID, wxT("&MOUL (Requires key)"));
        mnuEncryption->AppendRadioItem(ID_TEXT_ENC_AES, wxT("Myst &5 / Crowthistle / Hex Isle"));

        wxMenu* mnuEncoding = new wxMenu();
        mnuEncoding->AppendRadioItem(ID_TEXT_U_ANSI, wxT("&ANSI"));
        mnuEncoding->AppendRadioItem(ID_TEXT_U_UTF8, wxT("UTF-&8"));
        mnuEncoding->AppendRadioItem(ID_TEXT_U_UTF16LE, wxT("&UTF-16"));
        mnuEncoding->AppendRadioItem(ID_TEXT_U_UTF16BE, wxT("UTF-16 (&Big Endian)"));
        mnuEncoding->AppendRadioItem(ID_TEXT_U_UTF32LE, wxT("UTF-&32"));
        mnuEncoding->AppendRadioItem(ID_TEXT_U_UTF32BE, wxT("UTF-32 (Big &Endian)"));

        fEditorMenu = new wxMenu();
        fEditorMenu->Append(ID_TEXT_FIND, wxT("&Find"), wxT("Search for text"));
        fEditorMenu->Append(ID_TEXT_REPLACE, wxT("&Replace"), wxT("Search for and replace text"));
        fEditorMenu->AppendSeparator();
        fEditorMenu->AppendSubMenu(mnuHilight, wxT("&Syntax Highlighting"));
        fEditorMenu->AppendSubMenu(mnuEncryption, wxT("File &Encryption"));
        fEditorMenu->AppendSubMenu(mnuEncoding, wxT("En&coding"));
        fEditorMenu->AppendSeparator();
        fEditorMenu->Append(ID_TEXT_EXPAND, wxT("&Expand All"), wxT("Expand all fold points"));
        fEditorMenu->Append(ID_TEXT_COLLAPSE, wxT("&Collapse All"), wxT("Collapse all fold points"));

        GetMenuBar()->Append(fEditorMenu, wxT("&Text"));

        switch (stcPage->GetSyntaxMode()) {
        case wxPlasmaTextCtrl::kSynAgeIni:
            mnuHilight->Check(ID_TEXT_HL_INI, true);
            break;
        case wxPlasmaTextCtrl::kSynConsole:
            mnuHilight->Check(ID_TEXT_HL_CONSOLE, true);
            break;
        case wxPlasmaTextCtrl::kSynFX:
            mnuHilight->Check(ID_TEXT_HL_FX, true);
            break;
        case wxPlasmaTextCtrl::kSynHex:
            mnuHilight->Check(ID_TEXT_HL_HEX, true);
            break;
        case wxPlasmaTextCtrl::kSynPython:
            mnuHilight->Check(ID_TEXT_HL_PYTHON, true);
            break;
        case wxPlasmaTextCtrl::kSynSDL_Uru:
            mnuHilight->Check(ID_TEXT_HL_SDL1, true);
            break;
        case wxPlasmaTextCtrl::kSynSDL_Eoa:
            mnuHilight->Check(ID_TEXT_HL_SDL2, true);
            break;
        case wxPlasmaTextCtrl::kSynXML:
            mnuHilight->Check(ID_TEXT_HL_XML, true);
            break;
        default:
            mnuHilight->Check(ID_TEXT_HL_NONE, true);
            break;
        }

        switch (stcPage->GetEncryptionType()) {
        case plEncryptedStream::kEncAES:
            mnuEncryption->Check(ID_TEXT_ENC_AES, true);
            break;
        case plEncryptedStream::kEncDroid:
            mnuEncryption->Check(ID_TEXT_ENC_DROID, true);
            break;
        case plEncryptedStream::kEncXtea:
            mnuEncryption->Check(ID_TEXT_ENC_XTEA, true);
            break;
        default:
            mnuEncryption->Check(ID_TEXT_ENC_NONE, true);
            break;
        }

        switch (stcPage->GetEncoding()) {
        case wxPlasmaTextCtrl::kUniUTF8:
            mnuEncoding->Check(ID_TEXT_U_UTF8, true);
            break;
        case wxPlasmaTextCtrl::kUniUTF16BE:
            mnuEncoding->Check(ID_TEXT_U_UTF16BE, true);
            break;
        case wxPlasmaTextCtrl::kUniUTF16LE:
            mnuEncoding->Check(ID_TEXT_U_UTF16LE, true);
            break;
        case wxPlasmaTextCtrl::kUniUTF32BE:
            mnuEncoding->Check(ID_TEXT_U_UTF32BE, true);
            break;
        case wxPlasmaTextCtrl::kUniUTF32LE:
            mnuEncoding->Check(ID_TEXT_U_UTF32LE, true);
            break;
        default:
            mnuEncoding->Check(ID_TEXT_U_ANSI, true);
            break;
        }
    }
}

void wxPlasmaShopFrame::OnExitClick(wxCommandEvent& evt)
{
    Close();
}

void wxPlasmaShopFrame::OnNewClick(wxCommandEvent& evt)
{
    int sel = wxNewFileFrame().ShowModal();
    if (sel == -1)
        return;

    switch (sel) {
    case wxNewFileFrame::ID_NEW_AGE:
    case wxNewFileFrame::ID_NEW_INI:
        {
            wxPlasmaTextCtrl* ptc = new wxPlasmaTextCtrl(this);
            fEditorBook->AddPage(ptc, wxT("Untitled"), true);
            ptc->SetSyntaxMode(wxPlasmaTextCtrl::kSynAgeIni);
        }
        break;
    case wxNewFileFrame::ID_NEW_CSV:
    case wxNewFileFrame::ID_NEW_TEXT:
        {
            wxPlasmaTextCtrl* ptc = new wxPlasmaTextCtrl(this);
            fEditorBook->AddPage(ptc, wxT("Untitled"), true);
            ptc->SetSyntaxMode(wxPlasmaTextCtrl::kSynNone);
        }
        break;
    case wxNewFileFrame::ID_NEW_CURS:
        {
            wxMessageBox(wxT("Cursor"), wxT("ToDo"), wxOK | wxICON_INFORMATION);
        }
        break;
    case wxNewFileFrame::ID_NEW_CURSDAT:
        {
            wxMessageBox(wxT("Cursors.dat"), wxT("ToDo"), wxOK | wxICON_INFORMATION);
        }
        break;
    case wxNewFileFrame::ID_NEW_FNI:
        {
            wxPlasmaTextCtrl* ptc = new wxPlasmaTextCtrl(this);
            fEditorBook->AddPage(ptc, wxT("Untitled"), true);
            ptc->SetSyntaxMode(wxPlasmaTextCtrl::kSynConsole);
        }
        break;
    case wxNewFileFrame::ID_NEW_FX:
        {
            wxPlasmaTextCtrl* ptc = new wxPlasmaTextCtrl(this);
            fEditorBook->AddPage(ptc, wxT("Untitled"), true);
            ptc->SetSyntaxMode(wxPlasmaTextCtrl::kSynFX);
        }
        break;
    case wxNewFileFrame::ID_NEW_HEX:
        {
            wxPlasmaTextCtrl* ptc = new wxPlasmaTextCtrl(this);
            fEditorBook->AddPage(ptc, wxT("Untitled"), true);
            ptc->SetSyntaxMode(wxPlasmaTextCtrl::kSynHex);
        }
        break;
    case wxNewFileFrame::ID_NEW_LOC:
    case wxNewFileFrame::ID_NEW_SUB:
        {
            wxPlasmaTextCtrl* ptc = new wxPlasmaTextCtrl(this);
            fEditorBook->AddPage(ptc, wxT("Untitled"), true);
            ptc->SetSyntaxMode(wxPlasmaTextCtrl::kSynXML);
        }
        break;
    case wxNewFileFrame::ID_NEW_P2F:
        {
            wxMessageBox(wxT("Font"), wxT("ToDo"), wxOK | wxICON_INFORMATION);
        }
        break;
    case wxNewFileFrame::ID_NEW_PAK:
        {
            wxMessageBox(wxT("Python.pak"), wxT("ToDo"), wxOK | wxICON_INFORMATION);
        }
        break;
    case wxNewFileFrame::ID_NEW_PFP:
        {
            wxMessageBox(wxT("Fonts.pfp"), wxT("ToDo"), wxOK | wxICON_INFORMATION);
        }
        break;
    case wxNewFileFrame::ID_NEW_PY:
        {
            wxPlasmaTextCtrl* ptc = new wxPlasmaTextCtrl(this);
            fEditorBook->AddPage(ptc, wxT("Untitled"), true);
            ptc->SetSyntaxMode(wxPlasmaTextCtrl::kSynPython);
        }
        break;
    case wxNewFileFrame::ID_NEW_SDL:
        {
            wxPlasmaTextCtrl* ptc = new wxPlasmaTextCtrl(this);
            fEditorBook->AddPage(ptc, wxT("Untitled"), true);
            ptc->SetSyntaxMode(wxPlasmaTextCtrl::kSynSDL_Uru);
        }
        break;
    case wxNewFileFrame::ID_NEW_SDL2:
        {
            wxPlasmaTextCtrl* ptc = new wxPlasmaTextCtrl(this);
            fEditorBook->AddPage(ptc, wxT("Untitled"), true);
            ptc->SetSyntaxMode(wxPlasmaTextCtrl::kSynSDL_Eoa);
        }
        break;
    case wxNewFileFrame::ID_NEW_SUM:
        {
            wxMessageBox(wxT("Sum"), wxT("ToDo"), wxOK | wxICON_INFORMATION);
        }
        break;
    }
    UpdateMenus();
}

void wxPlasmaShopFrame::OnOpenClick(wxCommandEvent& evt)
{
    static const wxString kFilter =
        wxT("All supported files|*.age;*.fni;*.csv;*.sum;*.ini;*.tga;*.pfp;*.p2f;*.hex;*.loc;*.sub;*.elf;*.log*.pak;*.py;*.sdl;*.fx;*.txt|")
        wxT("Age files (*.age, *.fni, *.csv, *.sum)|*.age;*.fni;*.csv;*.sum|")
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
    UpdateMenus();
}

void wxPlasmaShopFrame::OnSaveClick(wxCommandEvent& evt)
{
    wxWindow* page = fEditorBook->GetPage(fEditorBook->GetSelection());

    if (wxPlasmaTextCtrl* stcPage = wxDynamicCast(page, wxPlasmaTextCtrl)) {
        if (stcPage->GetFilename() == wxEmptyString) {
            OnSaveAsClick(evt);
            return;
        } else {
            stcPage->DoSave();
        }
    }
}

void wxPlasmaShopFrame::OnSaveAsClick(wxCommandEvent& evt)
{
    wxWindow* page = fEditorBook->GetPage(fEditorBook->GetSelection());

    if (wxPlasmaTextCtrl* stcPage = wxDynamicCast(page, wxPlasmaTextCtrl)) {
        static const wxString kFilter =
            wxT("Age files (*.age)|*.age|")
            wxT("Console files (*.fni)|*.fni|")
            wxT("Relevance files (*.csv)|*.csv|")
            wxT("Config files (*.ini)|*.ini|")
            wxT("Hex Isle levels (*.hex)|*.hex|")
            wxT("Localization files (*.loc)|*.loc|")
            wxT("Subtitle files (*.sub)|*.sub|")
            wxT("Python files (*.py)|*.py|")
            wxT("SDL files (*.sdl)|*.sdl|")
            wxT("Shader files (*.fx)|*.fx|")
            wxT("Text files (*.txt)|*.txt");

        wxFileName fn(stcPage->GetFilename());
        wxFileDialog fd(this, wxT("Save file"), fn.GetPath(),
                        fn.GetFullName(), kFilter,
                        wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
        
        wxString curExt = fn.GetExt().Lower();
        if (curExt == wxT("age"))
            fd.SetFilterIndex(0);
        else if (curExt == wxT("fni"))
            fd.SetFilterIndex(1);
        else if (curExt == wxT("csv"))
            fd.SetFilterIndex(2);
        else if (curExt == wxT("ini"))
            fd.SetFilterIndex(3);
        else if (curExt == wxT("hex"))
            fd.SetFilterIndex(4);
        else if (curExt == wxT("loc"))
            fd.SetFilterIndex(5);
        else if (curExt == wxT("sub"))
            fd.SetFilterIndex(6);
        else if (curExt == wxT("py"))
            fd.SetFilterIndex(7);
        else if (curExt == wxT("sdl"))
            fd.SetFilterIndex(8);
        else if (curExt == wxT("fx"))
            fd.SetFilterIndex(9);
        else
            fd.SetFilterIndex(10);

        if (fd.ShowModal() != wxID_CANCEL) {
            wxFileName fn(fd.GetPath());
            stcPage->DoSave(fd.GetPath());
            fEditorBook->SetPageText(fEditorBook->GetSelection(), fn.GetFullName());
        }
    }

    wxString pageText = fEditorBook->GetPageText(fEditorBook->GetSelection());
    if (pageText.Right(1) == wxT("*"))
        fEditorBook->SetPageText(fEditorBook->GetSelection(),
                                 pageText.Left(pageText.size() - 2));
}

void wxPlasmaShopFrame::OnClose(wxCloseEvent& evt)
{
    if (evt.CanVeto()) {
        while (fEditorBook->GetPageCount() > 0 && !evt.GetVeto()) {
            wxAuiNotebookEvent nbEvt;
            nbEvt.SetSelection(0);
            OnPageClose(nbEvt);
            if (!nbEvt.IsAllowed())
                evt.Veto();
            else
                fEditorBook->DeletePage(0);
        }
    }
    if (!evt.GetVeto())
        Destroy();
}

void wxPlasmaShopFrame::SetPageClean(size_t page)
{
    wxString pageText = fEditorBook->GetPageText(page);
    if (pageText.Right(1) == wxT("*"))
        fEditorBook->SetPageText(page, pageText.Left(pageText.size() - 2));
}

void wxPlasmaShopFrame::SetPageDirty(size_t page)
{
    wxString pageText = fEditorBook->GetPageText(page);
    if (pageText.Right(1) != wxT("*"))
        fEditorBook->SetPageText(page, pageText + wxT(" *"));
}

void wxPlasmaShopFrame::OnPageClose(wxAuiNotebookEvent& evt)
{
    wxWindow* page = fEditorBook->GetPage(evt.GetSelection());

    int save = wxNO;
    if (wxPlasmaTextCtrl* stcPage = wxDynamicCast(page, wxPlasmaTextCtrl)) {
        if (stcPage->GetModify()) {
            save = wxMessageBox(wxT("Would you like to save changes to ") +
                                stcPage->GetFilename() + wxT(" before closing?"),
                                wxT("Confirm"), wxYES_NO | wxCANCEL | wxICON_QUESTION);
            if (save == wxYES) {
                OnSaveClick(evt);
            } else if (save == wxCANCEL) {
                evt.Veto();
                return;
            }
        }
    }

    if (fEditorMenu != NULL) {
        GetMenuBar()->Remove(1);
        delete fEditorMenu;
        fEditorMenu = NULL;
    }
}

void wxPlasmaShopFrame::OnPageChange(wxAuiNotebookEvent& evt)
{
    UpdateMenus();
}

void wxPlasmaShopFrame::OnStcDirty(wxStyledTextEvent& evt)
{
    int pid = fEditorBook->GetPageIndex((wxWindow*)evt.GetEventObject());
    if (pid == wxNOT_FOUND)
        return;
    SetPageDirty(pid);
}

void wxPlasmaShopFrame::OnStcClean(wxStyledTextEvent& evt)
{
    int pid = fEditorBook->GetPageIndex((wxWindow*)evt.GetEventObject());
    if (pid == wxNOT_FOUND)
        return;
    SetPageClean(pid);
}

void wxPlasmaShopFrame::OnTextHLNoneClick(wxCommandEvent& evt)
{
    wxPlasmaTextCtrl* page = wxDynamicCast(fEditorBook->GetPage(fEditorBook->GetSelection()), wxPlasmaTextCtrl);
    if (page == NULL)
        return;
    page->SetSyntaxMode(wxPlasmaTextCtrl::kSynNone);
}

void wxPlasmaShopFrame::OnTextHLPythonClick(wxCommandEvent& evt)
{
    wxPlasmaTextCtrl* page = wxDynamicCast(fEditorBook->GetPage(fEditorBook->GetSelection()), wxPlasmaTextCtrl);
    if (page == NULL)
        return;
    page->SetSyntaxMode(wxPlasmaTextCtrl::kSynPython);
}

void wxPlasmaShopFrame::OnTextHLSdl1Click(wxCommandEvent& evt)
{
    wxPlasmaTextCtrl* page = wxDynamicCast(fEditorBook->GetPage(fEditorBook->GetSelection()), wxPlasmaTextCtrl);
    if (page == NULL)
        return;
    page->SetSyntaxMode(wxPlasmaTextCtrl::kSynSDL_Uru);
}

void wxPlasmaShopFrame::OnTextHLSdl2Click(wxCommandEvent& evt)
{
    wxPlasmaTextCtrl* page = wxDynamicCast(fEditorBook->GetPage(fEditorBook->GetSelection()), wxPlasmaTextCtrl);
    if (page == NULL)
        return;
    page->SetSyntaxMode(wxPlasmaTextCtrl::kSynSDL_Eoa);
}

void wxPlasmaShopFrame::OnTextHLIniClick(wxCommandEvent& evt)
{
    wxPlasmaTextCtrl* page = wxDynamicCast(fEditorBook->GetPage(fEditorBook->GetSelection()), wxPlasmaTextCtrl);
    if (page == NULL)
        return;
    page->SetSyntaxMode(wxPlasmaTextCtrl::kSynAgeIni);
}

void wxPlasmaShopFrame::OnTextHLConsoleClick(wxCommandEvent& evt)
{
    wxPlasmaTextCtrl* page = wxDynamicCast(fEditorBook->GetPage(fEditorBook->GetSelection()), wxPlasmaTextCtrl);
    if (page == NULL)
        return;
    page->SetSyntaxMode(wxPlasmaTextCtrl::kSynConsole);
}

void wxPlasmaShopFrame::OnTextHLXmlClick(wxCommandEvent& evt)
{
    wxPlasmaTextCtrl* page = wxDynamicCast(fEditorBook->GetPage(fEditorBook->GetSelection()), wxPlasmaTextCtrl);
    if (page == NULL)
        return;
    page->SetSyntaxMode(wxPlasmaTextCtrl::kSynXML);
}

void wxPlasmaShopFrame::OnTextHLFxClick(wxCommandEvent& evt)
{
    wxPlasmaTextCtrl* page = wxDynamicCast(fEditorBook->GetPage(fEditorBook->GetSelection()), wxPlasmaTextCtrl);
    if (page == NULL)
        return;
    page->SetSyntaxMode(wxPlasmaTextCtrl::kSynFX);
}

void wxPlasmaShopFrame::OnTextHLHexClick(wxCommandEvent& evt)
{
    wxPlasmaTextCtrl* page = wxDynamicCast(fEditorBook->GetPage(fEditorBook->GetSelection()), wxPlasmaTextCtrl);
    if (page == NULL)
        return;
    page->SetSyntaxMode(wxPlasmaTextCtrl::kSynHex);
}

void wxPlasmaShopFrame::OnTextEncNoneClick(wxCommandEvent& evt)
{
    wxPlasmaTextCtrl* page = wxDynamicCast(fEditorBook->GetPage(fEditorBook->GetSelection()), wxPlasmaTextCtrl);
    if (page == NULL)
        return;
    page->SetEncryptionType(plEncryptedStream::kEncNone);
}

void wxPlasmaShopFrame::OnTextEncXteaClick(wxCommandEvent& evt)
{
    wxPlasmaTextCtrl* page = wxDynamicCast(fEditorBook->GetPage(fEditorBook->GetSelection()), wxPlasmaTextCtrl);
    if (page == NULL)
        return;
    page->SetEncryptionType(plEncryptedStream::kEncXtea);
}

void wxPlasmaShopFrame::OnTextEncDroidClick(wxCommandEvent& evt)
{
    wxPlasmaTextCtrl* page = wxDynamicCast(fEditorBook->GetPage(fEditorBook->GetSelection()), wxPlasmaTextCtrl);
    if (page == NULL)
        return;
    page->SetEncryptionType(plEncryptedStream::kEncDroid);
}

void wxPlasmaShopFrame::OnTextEncAesClick(wxCommandEvent& evt)
{
    wxPlasmaTextCtrl* page = wxDynamicCast(fEditorBook->GetPage(fEditorBook->GetSelection()), wxPlasmaTextCtrl);
    if (page == NULL)
        return;
    page->SetEncryptionType(plEncryptedStream::kEncAES);
}

void wxPlasmaShopFrame::OnTextUAnsiClick(wxCommandEvent& evt)
{
    wxPlasmaTextCtrl* page = wxDynamicCast(fEditorBook->GetPage(fEditorBook->GetSelection()), wxPlasmaTextCtrl);
    if (page == NULL)
        return;
    page->SetEncoding(wxPlasmaTextCtrl::kUniNone);
}

void wxPlasmaShopFrame::OnTextUUtf8Click(wxCommandEvent& evt)
{
    wxPlasmaTextCtrl* page = wxDynamicCast(fEditorBook->GetPage(fEditorBook->GetSelection()), wxPlasmaTextCtrl);
    if (page == NULL)
        return;
    page->SetEncoding(wxPlasmaTextCtrl::kUniUTF8);
}

void wxPlasmaShopFrame::OnTextUUtf16LEClick(wxCommandEvent& evt)
{
    wxPlasmaTextCtrl* page = wxDynamicCast(fEditorBook->GetPage(fEditorBook->GetSelection()), wxPlasmaTextCtrl);
    if (page == NULL)
        return;
    page->SetEncoding(wxPlasmaTextCtrl::kUniUTF16LE);
}

void wxPlasmaShopFrame::OnTextUUtf16BEClick(wxCommandEvent& evt)
{
    wxPlasmaTextCtrl* page = wxDynamicCast(fEditorBook->GetPage(fEditorBook->GetSelection()), wxPlasmaTextCtrl);
    if (page == NULL)
        return;
    page->SetEncoding(wxPlasmaTextCtrl::kUniUTF16BE);
}

void wxPlasmaShopFrame::OnTextUUtf32LEClick(wxCommandEvent& evt)
{
    wxPlasmaTextCtrl* page = wxDynamicCast(fEditorBook->GetPage(fEditorBook->GetSelection()), wxPlasmaTextCtrl);
    if (page == NULL)
        return;
    page->SetEncoding(wxPlasmaTextCtrl::kUniUTF32LE);
}

void wxPlasmaShopFrame::OnTextUUtf32BEClick(wxCommandEvent& evt)
{
    wxPlasmaTextCtrl* page = wxDynamicCast(fEditorBook->GetPage(fEditorBook->GetSelection()), wxPlasmaTextCtrl);
    if (page == NULL)
        return;
    page->SetEncoding(wxPlasmaTextCtrl::kUniUTF32BE);
}
