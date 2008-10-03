#include "wxPrpShopFrame.h"
#include "PlasmaTreeItem.h"
#include "../../rc/PlasmaShop.xpm"
#include "../../rc/PrpImages.xpm"
#include "../../rc/FileImages.xpm"

#include <Debug/plDebug.h>
#include <PRP/plSceneNode.h>
#include <wx/config.h>
#include <wx/filename.h>
#include <wx/sysopt.h>
#include <wx/config.h>
#include <wx/artprov.h>
#include <wx/imaglist.h>

wxLocationInfo::wxLocationInfo(const wxTreeItemId& tid, const wxString& filename)
              : fTreeId(tid), fFilename(filename)
{ }

wxLocationInfo::wxLocationInfo(const wxLocationInfo& init)
              : fTreeId(init.fTreeId), fFilename(init.fFilename)
{ }

wxLocationInfo::wxLocationInfo()
{ }


BEGIN_EVENT_TABLE(wxPrpShopFrame, wxFrame)
    EVT_MENU(wxID_EXIT, wxPrpShopFrame::OnExitClick)
    EVT_MENU(wxID_OPEN, wxPrpShopFrame::OnOpenClick)
    EVT_MENU(wxID_SAVE, wxPrpShopFrame::OnSaveClick)
    EVT_MENU(wxID_SAVEAS, wxPrpShopFrame::OnSaveAsClick)
    EVT_CLOSE(wxPrpShopFrame::OnClose)

    EVT_TREE_SEL_CHANGED(ID_OBJTREE, wxPrpShopFrame::OnTreeChanged)
END_EVENT_TABLE()

wxPrpShopFrame::wxPrpShopFrame(wxApp* owner)
    : wxFrame(NULL, wxID_ANY, wxT("PrpShop 1.0"), wxDefaultPosition, wxSize(800, 600)),
      fOwner(owner), fCurObject(NULL), fCurPage(NULL)
{
    fHSplitter = new wxSplitterWindow(this, wxID_ANY, wxDefaultPosition,
                                      wxDefaultSize, wxSP_3D | wxSP_LIVE_UPDATE);
    fVSplitter = new wxSplitterWindow(fHSplitter, wxID_ANY, wxDefaultPosition,
                                      wxDefaultSize, wxSP_3D | wxSP_LIVE_UPDATE);

    // GUI Elements
    fObjTree = new wxTreeCtrl(fHSplitter, ID_OBJTREE, wxDefaultPosition,
                              wxSize(240, -1),
                              wxTR_HAS_BUTTONS | wxTR_HIDE_ROOT);
    fObjTree->AddRoot(wxT(""));
    wxPanel* pnlBlah = new wxPanel(fVSplitter, wxID_ANY);
    fPropertyBook = new wxNotebook(fVSplitter, ID_PROPERTYBOOK, wxDefaultPosition,
                                   wxSize(-1, 204), wxNB_TOP);

    // Toolbar
    wxSystemOptions::SetOption(wxT("msw.remap"), 0);
    wxToolBar* toolBar = new wxToolBar(this, wxID_ANY, wxDefaultPosition,
                                       wxDefaultSize, wxTB_FLAT);
    toolBar->AddTool(wxID_OPEN, wxT("Open"),
                     wxArtProvider::GetBitmap(wxART_FILE_OPEN, wxART_TOOLBAR),
                     wxNullBitmap, wxITEM_NORMAL, wxT("Open PRP"),
                     wxT("Open a page for editing"));
    toolBar->AddTool(wxID_SAVE, wxT("Save"),
                     wxArtProvider::GetBitmap(wxART_FILE_SAVE, wxART_TOOLBAR),
                     wxNullBitmap, wxITEM_NORMAL, wxT("Save PRP"),
                     wxT("Save changes to active PRP"));
    toolBar->Realize();
    SetToolBar(toolBar);

    // Menu, Status Bar
    wxMenu* mnuFile = new wxMenu();
    mnuFile->Append(wxID_OPEN, wxT("&Open...\tCtrl+O"), wxT("Open a page for editing"));
    mnuFile->Append(wxID_SAVE, wxT("&Save\tCtrl+S"), wxT("Save chages to active PRP"));
    mnuFile->Append(wxID_SAVEAS, wxT("Save &As..."), wxT("Save chages to a different PRP file"));
    mnuFile->AppendSeparator();
    mnuFile->Append(wxID_EXIT, wxT("E&xit\tAlt+F4"), wxT("Exit PrpShop"));

    wxMenuBar* menuBar = new wxMenuBar();
    menuBar->Append(mnuFile, wxT("&File"));

    SetMenuBar(menuBar);
    SetStatusBar(new wxStatusBar(this, wxID_ANY));

    // Layout
    fObjTree->SetMinSize(wxSize(0, 0));
    fPropertyBook->SetMinSize(wxSize(0, 0));
    fVSplitter->SetMinSize(wxSize(0, 0));

    fVSplitter->SetSashGravity(1.0);
    fVSplitter->SplitHorizontally(pnlBlah, fPropertyBook, -204);
    fHSplitter->SetSashGravity(0.0);
    fHSplitter->SplitVertically(fObjTree, fVSplitter, 240);

    // Miscellaneous
    SetIcon(wxIcon(XPM_PlasmaShop));

    wxImageList* prpImages = new wxImageList(16, 16);
    prpImages->Add(wxBitmap(XPM_folder));
    prpImages->Add(wxBitmap(XPM_age));
    prpImages->Add(wxBitmap(XPM_page));
    prpImages->Add(wxBitmap(XPM_img));
    prpImages->Add(wxBitmap(XPM_scenenode));
    prpImages->Add(wxBitmap(XPM_sceneobj));
    prpImages->Add(wxBitmap(XPM_spans));
    prpImages->Add(wxBitmap(XPM_sim));
    prpImages->Add(wxBitmap(XPM_coords));
    prpImages->Add(wxBitmap(XPM_sound));
    prpImages->Add(wxBitmap(XPM_spans2));
    fObjTree->SetImageList(prpImages);

    // Set up the Resource Manager
    fResMgr = new plResManager();

    // User configuration stuff
    wxConfigBase* cfg = new wxConfig(wxT("PlasmaShop"));
    wxConfigBase::Set(cfg);
    cfg->SetPath(wxT("/PrpShop"));

    long width, height, left, top;
    bool maximized;
    cfg->Read(wxT("Width"), &width, -1);
    cfg->Read(wxT("Height"), &height, -1);
    cfg->Read(wxT("Left"), &left, -1);
    cfg->Read(wxT("Top"), &top, -1);
    cfg->Read(wxT("Maximized"), &maximized, false);
    if (height != -1 && width != -1)
        SetSize(width, height);
    if (left != -1 && top != -1)
        SetPosition(wxPoint(left, top));
    Maximize(maximized);

    long vSplitPos, hSplitPos;
    cfg->Read(wxT("VSplit"), &vSplitPos, -1);
    cfg->Read(wxT("HSplit"), &hSplitPos, -1);
    if (vSplitPos != -1)
        fVSplitter->SetSashPosition(vSplitPos);
    if (hSplitPos != -1)
        fHSplitter->SetSashPosition(hSplitPos);
}

wxPrpShopFrame::~wxPrpShopFrame()
{
    // Save the configuration
    wxConfigBase* cfg = wxConfigBase::Get();
    cfg->SetPath(wxT("/PrpShop"));

    if (!IsMaximized()) {
        cfg->Write(wxT("Width"), GetSize().GetWidth());
        cfg->Write(wxT("Height"), GetSize().GetHeight());
        cfg->Write(wxT("Left"), GetPosition().x);
        cfg->Write(wxT("Top"), GetPosition().y);
    }
    cfg->Write(wxT("Maximized"), IsMaximized());

    cfg->Write(wxT("VSplit"), fVSplitter->GetSashPosition());
    cfg->Write(wxT("HSplit"), fHSplitter->GetSashPosition());

    // Now clean up what's left on the form
    if (fCurObject != NULL)
        delete fCurObject;
    delete fResMgr;
}

void wxPrpShopFrame::LoadFile(const wxString& filename)
{
    wxFileName fn(filename);

    if (!fn.FileExists()) {
        wxMessageBox(wxString::Format(wxT("Error: File %s not found!"), filename.wx_str()),
                     wxT("Error"), wxOK | wxICON_ERROR);
        return;
    }

    try {
        wxString ext = fn.GetExt();
        if (ext.CmpNoCase(wxT("age")) == 0) {
            plAgeInfo* age = fResMgr->ReadAge(fn.GetFullPath().mb_str(), true);
            for (size_t i=0; i<age->getNumPages(); i++)
                LoadPage(fResMgr->FindPage(age->getPageLoc(i)),
                                           wxString(age->getPageFilename(i, fResMgr->getVer()), wxConvUTF8));
        } else if (ext.CmpNoCase(wxT("prp")) == 0) {
            plPageInfo* page = fResMgr->ReadPage(fn.GetFullPath().mb_str());
            wxTreeItemId pageId = LoadPage(page, fn.GetFullPath()).fTreeId;
            wxTreeItemId ageId = fObjTree->GetItemParent(pageId);
            PlasmaTreeItem* ageInfo = (PlasmaTreeItem*)fObjTree->GetItemData(ageId);
            if (ageInfo != NULL && ageInfo->getAge() != NULL) {
                if (!ageInfo->getAge()->fHasTextures) {
                    plString texPath = (const char*)fn.GetFullPath().mb_str();
                    texPath = texPath.beforeLast('_') + "_Textures.prp";
                    if (wxFileExists(wxString(texPath, wxConvUTF8)))
                        LoadPage(fResMgr->ReadPage(texPath), wxString(texPath, wxConvUTF8));
                }
                if (!ageInfo->getAge()->fHasBuiltIn) {
                    plString biPath = (const char*)fn.GetFullPath().mb_str();
                    biPath = biPath.beforeLast('_') + "_BuiltIn.prp";
                    if (wxFileExists(wxString(biPath, wxConvUTF8)))
                        LoadPage(fResMgr->ReadPage(biPath), wxString(biPath, wxConvUTF8));
                }
            }
        } else {
            wxMessageBox(wxString::Format(wxT("%s: Unsupported file type!"), filename.wx_str()),
                         wxT("Error"), wxOK | wxICON_ERROR, this);
        }
    } catch (std::exception& e) {
        wxMessageBox(wxString::Format(wxT("Error loading %s: %s"),
                     filename.wx_str(), wxString(e.what(), wxConvUTF8).wx_str()),
                     wxT("Error"), wxOK | wxICON_ERROR, this);
    }

    fObjTree->SortChildren(fObjTree->GetRootItem());
}

wxLocationInfo wxPrpShopFrame::LoadPage(plPageInfo* page, const wxString& filename)
{
    if (fLoadedLocations[page->getLocation()].fTreeId.IsOk()) {
        fLoadedLocations[page->getLocation()].fFilename = filename;
        return fLoadedLocations[page->getLocation()];
    }

    wxString ageName = wxString(page->getAge().cstr(), wxConvUTF8);
    wxString pageName = wxString(page->getPage().cstr(), wxConvUTF8);
    wxTreeItemIdValue scookie;
    wxTreeItemId ageId = fObjTree->GetFirstChild(fObjTree->GetRootItem(), scookie);
    while (ageId.IsOk()) {
        if (fObjTree->GetItemText(ageId) == ageName)
            break;
        ageId = fObjTree->GetNextChild(fObjTree->GetRootItem(), scookie);
    }
    if (!ageId.IsOk())
        ageId = fObjTree->AppendItem(fObjTree->GetRootItem(), ageName, 1, -1,
                                     new PlasmaTreeItem(TreeAgeInfo(ageName)));

    std::vector<plKey> keys;

    // Special cases: Textures and BuiltIn:
    if (page->getLocation().getPageNum() == -1) {
        wxTreeItemId texFolderId = fObjTree->InsertItem(ageId, 0, wxT("Textures"), ico_folder,
                                                        -1, new PlasmaTreeItem(page));
        wxTreeItemId envmapId = fObjTree->AppendItem(texFolderId, wxT("Environment Maps"), ico_folder);
        wxTreeItemId mipmapId = fObjTree->AppendItem(texFolderId, wxT("Mipmaps"), ico_folder);

        keys = fResMgr->getKeys(page->getLocation(), kCubicEnvironmap);
        for (size_t i=0; i<keys.size(); i++)
            TreeAddObject(fObjTree, envmapId, fResMgr, keys[i]);
        keys = fResMgr->getKeys(page->getLocation(), kMipmap);
        for (size_t i=0; i<keys.size(); i++)
            TreeAddObject(fObjTree, mipmapId, fResMgr, keys[i]);

        ((PlasmaTreeItem*)fObjTree->GetItemData(ageId))->getAge()->fHasTextures = true;
        fLoadedLocations[page->getLocation()] = wxLocationInfo(texFolderId, filename);
    } else if (page->getLocation().getPageNum() == -2) {
        keys = fResMgr->getKeys(page->getLocation(), kSceneObject);
        wxTreeItemId biFolderId;
        if (keys.size() > 1)
            plDebug::Warning("Got multiple Scene Objects in a BuiltIn PRP");
        if (keys.size() >= 1) {
            if (((PlasmaTreeItem*)fObjTree->GetItemData(ageId))->getAge()->fHasTextures)
                biFolderId = fObjTree->InsertItem(ageId, 1, wxT("Built-In"), ico_folder,
                                                  -1, new PlasmaTreeItem(page));
            else
                biFolderId = fObjTree->InsertItem(ageId, 0, wxT("Built-In"), ico_folder,
                                                  -1, new PlasmaTreeItem(page));
            TreeAddObject(fObjTree, biFolderId, fResMgr, keys[0]);
        }

        ((PlasmaTreeItem*)fObjTree->GetItemData(ageId))->getAge()->fHasBuiltIn = true;
        fLoadedLocations[page->getLocation()] = wxLocationInfo(biFolderId, filename);
    } else {
        // Normal page
        wxTreeItemId pageId = fObjTree->AppendItem(ageId, pageName, ico_page, -1,
                                                   new PlasmaTreeItem(page));
        fLoadedLocations[page->getLocation()] = wxLocationInfo(pageId, filename);

        // The Scene Node
        TreeAddObject(fObjTree, pageId, fResMgr, fResMgr->getSceneNode(page->getLocation())->getKey());
    }

    return fLoadedLocations[page->getLocation()];
}

void wxPrpShopFrame::OnExitClick(wxCommandEvent& evt)
{
    Close();
}

void wxPrpShopFrame::OnOpenClick(wxCommandEvent& evt)
{
    static const wxString kFilter =
        wxT("All supported files|*.age;*.prp|")
        wxT("Age files (*.age)|*.age|")
        wxT("Page files (*.prp)|*.prp");

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

void wxPrpShopFrame::OnSaveClick(wxCommandEvent& evt)
{
    DoDataSave(false);
    plLocation loc = GetActiveLocation();
    if (!loc.isValid()) {
        wxMessageBox(wxT("No PRP file or object selected!"), wxT("Error"),
                     wxOK | wxICON_ERROR);
        return;
    }
    if (fLoadedLocations[loc].fFilename == wxEmptyString) {
        OnSaveAsClick(evt);
        return;
    }
    fResMgr->WritePage((const char*)fLoadedLocations[loc].fFilename.mb_str(),
                       fResMgr->FindPage(loc));
}

void wxPrpShopFrame::OnSaveAsClick(wxCommandEvent& evt)
{
    static const wxString kFilter =
        wxT("Page files (*.prp)|*.prp");

    DoDataSave(false);
    plLocation loc = GetActiveLocation();
    if (!loc.isValid()) {
        wxMessageBox(wxT("No PRP file or object selected!"), wxT("Error"),
                     wxOK | wxICON_ERROR);
        return;
    }
    wxFileName name(fLoadedLocations[loc].fFilename);
    wxFileDialog fd(this, wxT("Save PRP"), name.GetPath(),
                    name.GetName(), kFilter,
                    wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    if (fd.ShowModal() != wxID_CANCEL) {
        fResMgr->WritePage((const char*)fd.GetPath().mb_str(), fResMgr->FindPage(loc));
        fLoadedLocations[loc].fFilename = fd.GetPath();
    }
}

void wxPrpShopFrame::OnClose(wxCloseEvent& evt)
{
    if (evt.CanVeto()) {
        int confirm = wxMessageBox(wxT("Any unsaved changes will be lost.  Are you sure you want to quit?"),
                                   wxT("Close PrpShop"), wxYES | wxNO | wxICON_QUESTION);
        if (confirm == wxYES)
            Destroy();
    } else {
        Destroy();
    }
}

void wxPrpShopFrame::OnTreeChanged(wxTreeEvent& evt)
{
    wxTreeItemId itm = evt.GetItem();
    PlasmaTreeItem* data = (PlasmaTreeItem*)fObjTree->GetItemData(itm);
    if (data == NULL)
        return;

    int propPageIdx = fPropertyBook->GetSelection();
    if (propPageIdx < 0)
        propPageIdx = 0;

    DoDataSave(true);
    fPropertyBook->DeleteAllPages();

    if (data->getObject().Exists()) {
        fCurObject = AddPropPages(fPropertyBook, fResMgr, data->getObject(), fObjTree, itm);
        fCurObject->AddKeyPage(fPropertyBook);
    } else if (data->getPage() != NULL) {
        fCurPage = data->getPage();
        wxPanel* nbpage = new wxPanel(fPropertyBook);
        wxString ageName(fCurPage->getAge().cstr(), wxConvUTF8);
        wxString pageName(fCurPage->getPage().cstr(), wxConvUTF8);
        wxString seqPrefix(wxString::Format(wxT("%d"), fCurPage->getLocation().getSeqPrefix()));
        wxString seqSuffix(wxString::Format(wxT("%d"), fCurPage->getLocation().getPageNum()));

        wxStaticText* lbl1 = new wxStaticText(nbpage, wxID_ANY, wxT("Age:"));
        wxStaticText* lbl2 = new wxStaticText(nbpage, wxID_ANY, wxT("Page:"));
        wxStaticText* lbl3 = new wxStaticText(nbpage, wxID_ANY, wxT("Sequence Prefix:"));
        wxStaticText* lbl4 = new wxStaticText(nbpage, wxID_ANY, wxT("Page Number:"));
        wxStaticText* lbl5 = new wxStaticText(nbpage, wxID_ANY, wxT("Flags:"));
        txtAge = new wxTextCtrl(nbpage, wxID_ANY, ageName, wxDefaultPosition, wxSize(200, -1));
        txtPage = new wxTextCtrl(nbpage, wxID_ANY, pageName, wxDefaultPosition, wxSize(200, -1));
        txtSeqPre = new wxTextCtrl(nbpage, wxID_ANY, seqPrefix, wxDefaultPosition, wxSize(40, -1));
        txtSeqSuf = new wxTextCtrl(nbpage, wxID_ANY, seqSuffix, wxDefaultPosition, wxSize(40, -1));
        wxPanel* pnlFlags = new wxPanel(nbpage, wxID_ANY);
        cbLocalOnly = new wxCheckBox(pnlFlags, wxID_ANY, wxT("Local Only"));
        cbLocalOnly->SetValue((fCurPage->getLocation().getFlags() & plLocation::kLocalOnly) != 0);
        cbVolatile = new wxCheckBox(pnlFlags, wxID_ANY, wxT("Volatile"));
        cbVolatile->SetValue((fCurPage->getLocation().getFlags() & plLocation::kVolatile) != 0);
        cbReserved = new wxCheckBox(pnlFlags, wxID_ANY, wxT("Reserved"));
        cbReserved->SetValue((fCurPage->getLocation().getFlags() & plLocation::kReserved) != 0);
        cbBuiltIn = new wxCheckBox(pnlFlags, wxID_ANY, wxT("Built-In"));
        cbBuiltIn->SetValue((fCurPage->getLocation().getFlags() & plLocation::kBuiltIn) != 0);
        cbItinerant = new wxCheckBox(pnlFlags, wxID_ANY, wxT("Itinerant"));
        cbItinerant->SetValue((fCurPage->getLocation().getFlags() & plLocation::kItinerant) != 0);

        wxGridSizer* szrFlags = new wxGridSizer(3, 2, 4, 16);
        szrFlags->Add(cbLocalOnly);
        szrFlags->Add(cbVolatile);
        szrFlags->Add(cbReserved);
        szrFlags->Add(cbBuiltIn);
        szrFlags->Add(cbItinerant);
        szrFlags->Add(0, 0);
        pnlFlags->SetSizer(szrFlags);

        wxFlexGridSizer* props = new wxFlexGridSizer(4, 5, 4, 4);
        props->Add(lbl1);
        props->Add(txtAge);
        props->Add(8, 0);
        props->Add(lbl3);
        props->Add(txtSeqPre);
        props->Add(lbl2);
        props->Add(txtPage);
        props->Add(8, 0);
        props->Add(lbl4);
        props->Add(txtSeqSuf);
        props->Add(0, 10);
        props->Add(0, 10);
        props->Add(0, 10);
        props->Add(0, 10);
        props->Add(0, 10);
        props->Add(lbl5);
        props->Add(pnlFlags);
        props->Add(0, 0);
        props->Add(0, 0);
        props->Add(0, 0);

        wxBoxSizer* border = new wxBoxSizer(0);
        border->Add(props, 0, wxALL, 8);
        nbpage->SetSizerAndFit(border);
        fPropertyBook->AddPage(nbpage, wxT("PRP Settings"));
    }

    if (propPageIdx < (int)fPropertyBook->GetPageCount())
        fPropertyBook->ChangeSelection(propPageIdx);
}

plLocation wxPrpShopFrame::GetActiveLocation()
{
    if (fCurPage != NULL)
        return fCurPage->getLocation();
    else if (fCurObject != NULL)
        return fCurObject->getKey()->getLocation();
    else
        return plLocation();
}

void wxPrpShopFrame::DoDataSave(bool doDelete)
{
    if (fCurObject != NULL) {
        fCurObject->SaveDamage();
        if (doDelete) {
            delete fCurObject;
            fCurObject = NULL;
        }
    }
    if (fCurPage != NULL) {
        fCurPage->setAge((const char*)txtAge->GetValue().mb_str());
        fCurPage->setPage((const char*)txtPage->GetValue().mb_str());

        plLocation newLoc;
        long out;
        txtSeqPre->GetValue().ToLong(&out);
        newLoc.setSeqPrefix(out);
        txtSeqSuf->GetValue().ToLong(&out);
        newLoc.setPageNum(out);
        unsigned short pgFlags = cbLocalOnly->GetValue() ? plLocation::kLocalOnly : 0
                               | cbVolatile->GetValue() ? plLocation::kVolatile : 0
                               | cbReserved->GetValue() ? plLocation::kReserved : 0
                               | cbBuiltIn->GetValue() ? plLocation::kBuiltIn : 0
                               | cbItinerant->GetValue() ? plLocation::kItinerant : 0;
        newLoc.setFlags(pgFlags);
        if (newLoc != fCurPage->getLocation()) {
            fLoadedLocations[newLoc] = fLoadedLocations[fCurPage->getLocation()];
            fLoadedLocations.erase(fLoadedLocations.find(fCurPage->getLocation()));
            fResMgr->ChangeLocation(fCurPage->getLocation(), newLoc);
        }

        if (doDelete)
            fCurPage = NULL;
    }
}
