#include "wxPrpShopFrame.h"
#include "PlasmaTreeItem.h"
#include "../../rc/PlasmaShop.xpm"
#include "../../rc/PrpImages.xpm"
#include "../../rc/FileImages.xpm"

#include <Debug/plDebug.h>
#include <PRP/plSceneNode.h>
#include <wx/filename.h>
#include <wx/sysopt.h>
#include <wx/config.h>
#include <wx/artprov.h>
#include <wx/splitter.h>
#include <wx/imaglist.h>

BEGIN_EVENT_TABLE(wxPrpShopFrame, wxFrame)
    EVT_MENU(wxID_EXIT, wxPrpShopFrame::OnExitClick)
    EVT_MENU(wxID_OPEN, wxPrpShopFrame::OnOpenClick)
    EVT_CLOSE(wxPrpShopFrame::OnClose)

    EVT_TREE_SEL_CHANGED(ID_OBJTREE, wxPrpShopFrame::OnTreeChanged)
END_EVENT_TABLE()

wxPrpShopFrame::wxPrpShopFrame(wxApp* owner)
    : wxFrame(NULL, wxID_ANY, wxT("PrpShop 1.0"), wxDefaultPosition, wxSize(800, 600)),
      fOwner(owner), fCurObject(NULL), fCurPage(NULL)
{
    wxSplitterWindow* splitter = new wxSplitterWindow(this, wxID_ANY,
            wxDefaultPosition, wxDefaultSize, wxSP_3D | wxSP_LIVE_UPDATE);
    wxSplitterWindow* splitterR = new wxSplitterWindow(splitter, wxID_ANY,
            wxDefaultPosition, wxDefaultSize, wxSP_3D | wxSP_LIVE_UPDATE);

    // GUI Elements
    fObjTree = new wxTreeCtrl(splitter, ID_OBJTREE, wxDefaultPosition,
                              wxSize(240, -1),
                              wxTR_HAS_BUTTONS | wxTR_HIDE_ROOT);
    fObjTree->AddRoot(wxT(""));
    wxPanel* pnlBlah = new wxPanel(splitterR, wxID_ANY);
    fPropertyBook = new wxNotebook(splitterR, ID_PROPERTYBOOK, wxDefaultPosition,
                                   wxSize(-1, 200), wxNB_TOP);

    // Toolbar
    wxSystemOptions::SetOption(wxT("msw.remap"), 0);
    wxToolBar* toolBar = new wxToolBar(this, wxID_ANY, wxDefaultPosition,
                                       wxDefaultSize, wxTB_FLAT);
    toolBar->AddTool(wxID_OPEN, wxT("Open"),
                     wxArtProvider::GetBitmap(wxART_FILE_OPEN, wxART_TOOLBAR),
                     wxNullBitmap, wxITEM_NORMAL, wxT("Open PRP"),
                     wxT("Open a page for editing"));
    toolBar->Realize();
    SetToolBar(toolBar);

    // Menu, Status Bar
    wxMenu* mnuFile = new wxMenu();
    mnuFile->Append(wxID_OPEN, wxT("&Open"), wxT("Open a page for editing"));
    mnuFile->AppendSeparator();
    mnuFile->Append(wxID_EXIT, wxT("E&xit"), wxT("Exit PrpShop"));

    wxMenuBar* menuBar = new wxMenuBar();
    menuBar->Append(mnuFile, wxT("&File"));

    SetMenuBar(menuBar);
    SetStatusBar(new wxStatusBar(this, wxID_ANY));

    // Layout
    fObjTree->SetMinSize(wxSize(0, 0));
    fPropertyBook->SetMinSize(wxSize(0, 0));
    splitterR->SetMinSize(wxSize(0, 0));

    splitterR->SetSashGravity(1.0);
    splitterR->SplitHorizontally(pnlBlah, fPropertyBook, -200);
    splitter->SetSashGravity(0.0);
    splitter->SplitVertically(fObjTree, splitterR, 240);

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
}

wxPrpShopFrame::~wxPrpShopFrame()
{
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
                LoadPage(fResMgr->FindPage(age->getPageLoc(i)));
        } else if (ext.CmpNoCase(wxT("prp")) == 0) {
            plPageInfo* page = fResMgr->ReadPage(fn.GetFullPath().mb_str());
            wxTreeItemId pageId = LoadPage(page);
            wxTreeItemId ageId = fObjTree->GetItemParent(pageId);
            PlasmaTreeItem* ageInfo = (PlasmaTreeItem*)fObjTree->GetItemData(ageId);
            if (ageInfo != NULL && ageInfo->getAge() != NULL) {
                if (!ageInfo->getAge()->fHasTextures) {
                    plString texPath = (const char*)fn.GetFullPath().mb_str();
                    texPath = texPath.beforeLast('_') + "_Textures.prp";
                    if (wxFileExists(wxString(texPath, wxConvUTF8)))
                        LoadPage(fResMgr->ReadPage(texPath));
                }
                if (!ageInfo->getAge()->fHasBuiltIn) {
                    plString biPath = (const char*)fn.GetFullPath().mb_str();
                    biPath = biPath.beforeLast('_') + "_BuiltIn.prp";
                    if (wxFileExists(wxString(biPath, wxConvUTF8)))
                        LoadPage(fResMgr->ReadPage(biPath));
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

wxTreeItemId wxPrpShopFrame::LoadPage(plPageInfo* page)
{
    if (fLoadedLocations[page->getLocation()].IsOk())
        return fLoadedLocations[page->getLocation()];

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
        fLoadedLocations[page->getLocation()] = texFolderId;
        return texFolderId;
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
        fLoadedLocations[page->getLocation()] = biFolderId;
        return biFolderId;
    }

    // Normal page
    wxTreeItemId pageId = fObjTree->AppendItem(ageId, pageName, ico_page, -1,
                                               new PlasmaTreeItem(page));
    fLoadedLocations[page->getLocation()] = pageId;

    // The Scene Node
    TreeAddObject(fObjTree, pageId, fResMgr, fResMgr->getSceneNode(page->getLocation())->getKey());

    return pageId;
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

void wxPrpShopFrame::OnClose(wxCloseEvent& evt)
{
    if (evt.CanVeto()) {
        // Check for unsaved work first
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

    if (fCurObject != NULL) {
        fCurObject->SaveDamage();
        delete fCurObject;
        fCurObject = NULL;
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

        fCurPage = NULL;
    }
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

    if (propPageIdx < fPropertyBook->GetPageCount())
        fPropertyBook->ChangeSelection(propPageIdx);
}
