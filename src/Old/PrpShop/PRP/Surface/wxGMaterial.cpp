#include "wxGMaterial.h"
#include "../../wxObjSelectFrame.h"
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/imaglist.h>

#include <PRP/Surface/hsGMaterial.h>
#include <PRP/Surface/plLayer.h>
#include <PRP/Surface/plLayerAnimation.h>

wxTreeItemId TreeAddMaterial(wxTreeCtrl* tree, const wxTreeItemId& parent,
                             plResManager* mgr, plKey key)
{
    wxTreeItemId tid =
    tree->AppendItem(parent, wxString(key->getName().cstr(), wxConvUTF8),
                     GetTypeIcon(key->getType()), -1, new PlasmaTreeItem(key));

    hsGMaterial* mat = hsGMaterial::Convert(key->getObj());
    if (mat->getNumLayers() > 0) {
        wxTreeItemId folder = tree->AppendItem(tid, wxT("Layers"), ico_folder);
        for (size_t i=0; i<mat->getNumLayers(); i++)
            TreeAddObject(tree, folder, mgr, mat->getLayer(i));
    }
    if (mat->getNumPiggyBacks() > 0) {
        wxTreeItemId folder = tree->AppendItem(tid, wxT("Piggy Backs"), ico_folder);
        for (size_t i=0; i<mat->getNumPiggyBacks(); i++)
            TreeAddObject(tree, folder, mgr, mat->getPiggyBack(i));
    }

    return tid;
}

wxGMaterial::wxGMaterial(plKey key, plResManager* mgr,
                         wxTreeCtrl* tree, const wxTreeItemId& tid)
             : wxSynchedObject(key, mgr, tree, tid)
{ }

void wxGMaterial::AddPropPages(wxNotebook* nb)
{
    wxSynchedObject::AddPropPages(nb);
    hsGMaterial* mat = hsGMaterial::Convert(fKey->getObj());
    
    wxPanel* nbpage = new wxPanel(nb);
    cbCompShaded = new wxCheckBox(nbpage, wxID_ANY, wxT("Shaded"));
    cbCompEnvironMap = new wxCheckBox(nbpage, wxID_ANY, wxT("EnvironMap"));
    cbCompProjectOnto = new wxCheckBox(nbpage, wxID_ANY, wxT("Project Onto"));
    cbCompSoftShadow = new wxCheckBox(nbpage, wxID_ANY, wxT("Soft Shadow"));
    cbCompSpecular = new wxCheckBox(nbpage, wxID_ANY, wxT("Specular"));
    cbCompTwoSided = new wxCheckBox(nbpage, wxID_ANY, wxT("Two-Sided"));
    cbCompDrawAsSplats = new wxCheckBox(nbpage, wxID_ANY, wxT("Draw as Splats"));
    cbCompAdjusted = new wxCheckBox(nbpage, wxID_ANY, wxT("Adjusted"));
    cbCompNoSoftShadow = new wxCheckBox(nbpage, wxID_ANY, wxT("No Soft Shadow"));
    cbCompDynamic = new wxCheckBox(nbpage, wxID_ANY, wxT("Dynamic"));
    cbCompDecal = new wxCheckBox(nbpage, wxID_ANY, wxT("Decal"));
    cbCompIsEmissive = new wxCheckBox(nbpage, wxID_ANY, wxT("Emissive"));
    cbCompIsLightMapped = new wxCheckBox(nbpage, wxID_ANY, wxT("Light Mapped"));
    cbCompNeedsBlendChannel = new wxCheckBox(nbpage, wxID_ANY, wxT("Needs Blend Channel"));

    cbCompShaded->SetValue((mat->getCompFlags() & hsGMaterial::kCompShaded) != 0);
    cbCompEnvironMap->SetValue((mat->getCompFlags() & hsGMaterial::kCompEnvironMap) != 0);
    cbCompProjectOnto->SetValue((mat->getCompFlags() & hsGMaterial::kCompProjectOnto) != 0);
    cbCompSoftShadow->SetValue((mat->getCompFlags() & hsGMaterial::kCompSoftShadow) != 0);
    cbCompSpecular->SetValue((mat->getCompFlags() & hsGMaterial::kCompSpecular) != 0);
    cbCompTwoSided->SetValue((mat->getCompFlags() & hsGMaterial::kCompTwoSided) != 0);
    cbCompDrawAsSplats->SetValue((mat->getCompFlags() & hsGMaterial::kCompDrawAsSplats) != 0);
    cbCompAdjusted->SetValue((mat->getCompFlags() & hsGMaterial::kCompAdjusted) != 0);
    cbCompNoSoftShadow->SetValue((mat->getCompFlags() & hsGMaterial::kCompNoSoftShadow) != 0);
    cbCompDynamic->SetValue((mat->getCompFlags() & hsGMaterial::kCompDynamic) != 0);
    cbCompDecal->SetValue((mat->getCompFlags() & hsGMaterial::kCompDecal) != 0);
    cbCompIsEmissive->SetValue((mat->getCompFlags() & hsGMaterial::kCompIsEmissive) != 0);
    cbCompIsLightMapped->SetValue((mat->getCompFlags() & hsGMaterial::kCompIsLightMapped) != 0);
    cbCompNeedsBlendChannel->SetValue((mat->getCompFlags() & hsGMaterial::kCompNeedsBlendChannel) != 0);

    wxFlexGridSizer* szrCompFlags = new wxFlexGridSizer(4, 4, 4, 10);
    szrCompFlags->Add(cbCompShaded);
    szrCompFlags->Add(cbCompSpecular);
    szrCompFlags->Add(cbCompNoSoftShadow);
    szrCompFlags->Add(cbCompIsLightMapped);
    szrCompFlags->Add(cbCompEnvironMap);
    szrCompFlags->Add(cbCompTwoSided);
    szrCompFlags->Add(cbCompDynamic);
    szrCompFlags->Add(cbCompNeedsBlendChannel);
    szrCompFlags->Add(cbCompProjectOnto);
    szrCompFlags->Add(cbCompDrawAsSplats);
    szrCompFlags->Add(cbCompDecal);
    szrCompFlags->Add(0, 0);
    szrCompFlags->Add(cbCompSoftShadow);
    szrCompFlags->Add(cbCompAdjusted);
    szrCompFlags->Add(cbCompIsEmissive);
    szrCompFlags->Add(0, 0);

    lsLayers = new wxKeyListCtrl(nbpage, wxID_ANY, wxDefaultPosition, wxSize(240, 72));
    lsPiggyBacks = new wxKeyListCtrl(nbpage, wxID_ANY, wxDefaultPosition, wxSize(240, 72));
    lsLayers->SetImageList(GetTypeImgList(), wxIMAGE_LIST_SMALL);
    lsPiggyBacks->SetImageList(GetTypeImgList(), wxIMAGE_LIST_SMALL);
    for (size_t i=0; i<mat->getNumLayers(); i++)
        lsLayers->AddKey(mat->getLayer(i));
    for (size_t i=0; i<mat->getNumPiggyBacks(); i++)
        lsPiggyBacks->AddKey(mat->getPiggyBack(i));

    wxFlexGridSizer* szrLists = new wxFlexGridSizer(2, 2, 2, 10);
    szrLists->Add(new wxStaticText(nbpage, wxID_ANY, wxT("Layers:")));
    szrLists->Add(new wxStaticText(nbpage, wxID_ANY, wxT("Piggy Backs:")));
    szrLists->Add(lsLayers);
    szrLists->Add(lsPiggyBacks);

    wxFlexGridSizer* szrAll = new wxFlexGridSizer(2, 1, 12, 0);
    szrAll->Add(szrCompFlags);
    szrAll->Add(szrLists);

    wxBoxSizer* border = new wxBoxSizer(0);
    border->Add(szrAll, 0, wxALL, 8);
    nbpage->SetSizerAndFit(border);
    nb->AddPage(nbpage, wxT("Material"));

    // Events
    lsLayers->Connect(wxEVT_CONTEXT_MENU,
                      (wxObjectEventFunction)&wxGMaterial::OnLayerContextMenu,
                      NULL, this);
    lsPiggyBacks->Connect(wxEVT_CONTEXT_MENU,
                          (wxObjectEventFunction)&wxGMaterial::OnPiggyContextMenu,
                          NULL, this);
    lsLayers->Connect(wxEVT_COMMAND_LIST_ITEM_ACTIVATED,
                      (wxObjectEventFunction)&wxGMaterial::OnLayerItemActivated,
                      NULL, this);
    lsPiggyBacks->Connect(wxEVT_COMMAND_LIST_ITEM_ACTIVATED,
                          (wxObjectEventFunction)&wxGMaterial::OnPiggyItemActivated,
                          NULL, this);
}

void wxGMaterial::SaveDamage()
{
    wxSynchedObject::SaveDamage();
    hsGMaterial* mat = hsGMaterial::Convert(fKey->getObj());
    
    mat->setCompFlags((cbCompShaded->GetValue() ? hsGMaterial::kCompShaded : 0)
                    | (cbCompEnvironMap->GetValue() ? hsGMaterial::kCompEnvironMap : 0)
                    | (cbCompProjectOnto->GetValue() ? hsGMaterial::kCompProjectOnto : 0)
                    | (cbCompSoftShadow->GetValue() ? hsGMaterial::kCompSoftShadow : 0)
                    | (cbCompSpecular->GetValue() ? hsGMaterial::kCompSpecular : 0)
                    | (cbCompTwoSided->GetValue() ? hsGMaterial::kCompTwoSided : 0)
                    | (cbCompDrawAsSplats->GetValue() ? hsGMaterial::kCompDrawAsSplats : 0)
                    | (cbCompAdjusted->GetValue() ? hsGMaterial::kCompAdjusted : 0)
                    | (cbCompNoSoftShadow->GetValue() ? hsGMaterial::kCompNoSoftShadow : 0)
                    | (cbCompDynamic->GetValue() ? hsGMaterial::kCompDynamic : 0)
                    | (cbCompDecal->GetValue() ? hsGMaterial::kCompDecal : 0)
                    | (cbCompIsEmissive->GetValue() ? hsGMaterial::kCompIsEmissive : 0)
                    | (cbCompIsLightMapped->GetValue() ? hsGMaterial::kCompIsLightMapped : 0)
                    | (cbCompNeedsBlendChannel->GetValue() ? hsGMaterial::kCompNeedsBlendChannel : 0));
}

void wxGMaterial::OnLayerContextMenu(wxCommandEvent& evt)
{
    wxMenu* menuNew = new wxMenu(wxT(""));
    menuNew->Append(ID_NEW_LAYER, wxT("Layer"));
    menuNew->Append(ID_NEW_LAYERANIM, wxT("Layer Animation"));

    wxMenu menu(wxT(""));
    menu.AppendSubMenu(menuNew, wxT("New..."));
    menu.Append(ID_OBJ_ADD, wxT("Add Ref..."));
    menu.Append(ID_OBJ_REMOVE, wxT("Remove"));
    menu.Append(ID_OBJ_DELETE, wxT("Delete"));
    if (lsLayers->GetSelectedItemCount() < 1) {
        menu.Enable(ID_OBJ_REMOVE, false);
        menu.Enable(ID_OBJ_DELETE, false);
    }

    menu.Connect(ID_NEW_LAYER, wxEVT_COMMAND_MENU_SELECTED,
                 (wxObjectEventFunction)&wxGMaterial::OnLayerNewLayerClick,
                 NULL, this);
    menu.Connect(ID_NEW_LAYERANIM, wxEVT_COMMAND_MENU_SELECTED,
                 (wxObjectEventFunction)&wxGMaterial::OnLayerNewLayerAnimClick,
                 NULL, this);
    menu.Connect(ID_OBJ_ADD, wxEVT_COMMAND_MENU_SELECTED,
                 (wxObjectEventFunction)&wxGMaterial::OnLayerAddClick,
                 NULL, this);
    menu.Connect(ID_OBJ_REMOVE, wxEVT_COMMAND_MENU_SELECTED,
                 (wxObjectEventFunction)&wxGMaterial::OnLayerRemoveClick,
                 NULL, this);
    menu.Connect(ID_OBJ_DELETE, wxEVT_COMMAND_MENU_SELECTED,
                 (wxObjectEventFunction)&wxGMaterial::OnLayerDeleteClick,
                 NULL, this);

    lsLayers->PopupMenu(&menu);
}

void wxGMaterial::OnPiggyContextMenu(wxCommandEvent& evt)
{
    wxMenu* menuNew = new wxMenu(wxT(""));
    menuNew->Append(ID_NEW_LAYER, wxT("Layer"));
    menuNew->Append(ID_NEW_LAYERANIM, wxT("Layer Animation"));

    wxMenu menu(wxT(""));
    menu.AppendSubMenu(menuNew, wxT("New..."));
    menu.Append(ID_OBJ_ADD, wxT("Add Ref..."));
    menu.Append(ID_OBJ_REMOVE, wxT("Remove"));
    menu.Append(ID_OBJ_DELETE, wxT("Delete"));
    if (lsPiggyBacks->GetSelectedItemCount() < 1) {
        menu.Enable(ID_OBJ_REMOVE, false);
        menu.Enable(ID_OBJ_DELETE, false);
    }

    menu.Connect(ID_NEW_LAYER, wxEVT_COMMAND_MENU_SELECTED,
                 (wxObjectEventFunction)&wxGMaterial::OnPiggyNewLayerClick,
                 NULL, this);
    menu.Connect(ID_NEW_LAYERANIM, wxEVT_COMMAND_MENU_SELECTED,
                 (wxObjectEventFunction)&wxGMaterial::OnPiggyNewLayerAnimClick,
                 NULL, this);
    menu.Connect(ID_OBJ_ADD, wxEVT_COMMAND_MENU_SELECTED,
                 (wxObjectEventFunction)&wxGMaterial::OnPiggyAddClick,
                 NULL, this);
    menu.Connect(ID_OBJ_REMOVE, wxEVT_COMMAND_MENU_SELECTED,
                 (wxObjectEventFunction)&wxGMaterial::OnPiggyRemoveClick,
                 NULL, this);
    menu.Connect(ID_OBJ_DELETE, wxEVT_COMMAND_MENU_SELECTED,
                 (wxObjectEventFunction)&wxGMaterial::OnPiggyDeleteClick,
                 NULL, this);

    lsPiggyBacks->PopupMenu(&menu);
}

void wxGMaterial::OnLayerItemActivated(wxListEvent& evt)
{
    hsGMaterial* mat = hsGMaterial::Convert(fKey->getObj());
    wxTreeItemId folder = TreeFindFolder(fTree, fTreeId, wxT("Layers"));
    wxTreeItemId tid = TreeFindKey(fTree, folder, mat->getLayer(evt.GetIndex()));
    if (tid.IsOk())
        fTree->SelectItem(tid);
}

void wxGMaterial::OnPiggyItemActivated(wxListEvent& evt)
{
    hsGMaterial* mat = hsGMaterial::Convert(fKey->getObj());
    wxTreeItemId folder = TreeFindFolder(fTree, fTreeId, wxT("Piggy Backs"));
    wxTreeItemId tid = TreeFindKey(fTree, folder, mat->getPiggyBack(evt.GetIndex()));
    if (tid.IsOk())
        fTree->SelectItem(tid);
}

void wxGMaterial::OnLayerNewLayerClick(wxMenuEvent& evt)
{
    wxString name = wxGetTextFromUser(wxT("Object Name"), wxT("New Layer"));
    if (name == wxEmptyString)
        return;

    hsGMaterial* mat = hsGMaterial::Convert(fKey->getObj());
    plLayer* layer = new plLayer();
    layer->init((const char*)name.mb_str());
    fResMgr->AddObject(fKey->getLocation(), layer);
    mat->addLayer(layer->getKey());

    wxTreeItemId folder = TreeFindFolder(fTree, fTreeId, wxT("Layers"));
    if (!folder.IsOk())
        folder = fTree->AppendItem(fTreeId, wxT("Layers"), ico_folder);
    TreeAddObject(fTree, folder, fResMgr, layer->getKey());
    lsLayers->AddKey(layer->getKey());
}

void wxGMaterial::OnLayerNewLayerAnimClick(wxMenuEvent& evt)
{
    wxString name = wxGetTextFromUser(wxT("Object Name"), wxT("New Layer Animation"));
    if (name == wxEmptyString)
        return;

    hsGMaterial* mat = hsGMaterial::Convert(fKey->getObj());
    plLayerAnimation* layer = new plLayerAnimation();
    layer->init((const char*)name.mb_str());
    fResMgr->AddObject(fKey->getLocation(), layer);
    mat->addLayer(layer->getKey());

    wxTreeItemId folder = TreeFindFolder(fTree, fTreeId, wxT("Layers"));
    if (!folder.IsOk())
        folder = fTree->AppendItem(fTreeId, wxT("Layers"), ico_folder);
    TreeAddObject(fTree, folder, fResMgr, layer->getKey());
    lsLayers->AddKey(layer->getKey());
}

void wxGMaterial::OnLayerAddClick(wxMenuEvent& evt)
{
    wxObjSelectFrame addFrame(NULL, wxID_ANY, wxT("Add Layer"));

    hsGMaterial* mat = hsGMaterial::Convert(fKey->getObj());
    std::vector<plKey> keys = fResMgr->getKeys(fKey->getLocation(), kLayer);
    std::vector<plKey> animKeys = fResMgr->getKeys(fKey->getLocation(), kLayerAnimation);
    
    size_t keyEnd = keys.size();
    keys.resize(keys.size() + animKeys.size());
    std::copy(animKeys.begin(), animKeys.end(), keys.begin() + keyEnd);
    for (size_t i=0; i<keys.size(); i++)
        addFrame.AddObject(keys[i]);

    int which = addFrame.ShowModal();
    if (which != -1) {
        mat->addLayer(keys[which]);
        lsLayers->AddKey(keys[which]);
        wxTreeItemId folder = TreeFindFolder(fTree, fTreeId, wxT("Layers"));
        if (!folder.IsOk())
            folder = fTree->AppendItem(fTreeId, wxT("Layers"), ico_folder);
        TreeAddObject(fTree, folder, fResMgr, keys[which]);
    }
}

void wxGMaterial::OnLayerRemoveClick(wxMenuEvent& evt)
{
    hsGMaterial* mat = hsGMaterial::Convert(fKey->getObj());
    for (long i=0; i<lsLayers->GetItemCount(); ) {
        int state = lsLayers->GetItemState(i, wxLIST_STATE_SELECTED);
        if ((state & wxLIST_STATE_SELECTED) != 0) {
            plKey layer = mat->getLayer(i);
            wxTreeItemId folder = TreeFindFolder(fTree, fTreeId, wxT("Layers"));
            wxTreeItemId tid = TreeFindKey(fTree, folder, layer);
            if (tid.IsOk())
                fTree->Delete(tid);
            if (fTree->GetChildrenCount(folder) == 0)
                fTree->Delete(folder);

            mat->delLayer(i);
            lsLayers->DeleteItem(i);
        } else {
            i++;
        }
    }
}

void wxGMaterial::OnLayerDeleteClick(wxMenuEvent& evt)
{
    hsGMaterial* mat = hsGMaterial::Convert(fKey->getObj());
    for (long i=0; i<lsLayers->GetItemCount(); ) {
        int state = lsLayers->GetItemState(i, wxLIST_STATE_SELECTED);
        if ((state & wxLIST_STATE_SELECTED) != 0) {
            plKey layer = mat->getLayer(i);
            wxTreeItemId folder = TreeFindFolder(fTree, fTreeId, wxT("Layers"));
            wxTreeItemId tid = TreeFindKey(fTree, folder, layer);
            if (tid.IsOk())
                fTree->Delete(tid);
            if (fTree->GetChildrenCount(folder) == 0)
                fTree->Delete(folder);

            mat->delLayer(i);
            fResMgr->DelObject(layer);
            lsLayers->DeleteItem(i);
        } else {
            i++;
        }
    }
}

void wxGMaterial::OnPiggyNewLayerClick(wxMenuEvent& evt)
{
    wxString name = wxGetTextFromUser(wxT("Object Name"), wxT("New Layer"));
    if (name == wxEmptyString)
        return;

    hsGMaterial* mat = hsGMaterial::Convert(fKey->getObj());
    plLayer* layer = new plLayer();
    layer->init((const char*)name.mb_str());
    fResMgr->AddObject(fKey->getLocation(), layer);
    mat->addPiggyBack(layer->getKey());

    wxTreeItemId folder = TreeFindFolder(fTree, fTreeId, wxT("Piggy Backs"));
    if (!folder.IsOk())
        folder = fTree->AppendItem(fTreeId, wxT("Piggy Backs"), ico_folder);
    TreeAddObject(fTree, folder, fResMgr, layer->getKey());
    lsPiggyBacks->AddKey(layer->getKey());
}

void wxGMaterial::OnPiggyNewLayerAnimClick(wxMenuEvent& evt)
{
    wxString name = wxGetTextFromUser(wxT("Object Name"), wxT("New Layer Animation"));
    if (name == wxEmptyString)
        return;

    hsGMaterial* mat = hsGMaterial::Convert(fKey->getObj());
    plLayerAnimation* layer = new plLayerAnimation();
    layer->init((const char*)name.mb_str());
    fResMgr->AddObject(fKey->getLocation(), layer);
    mat->addPiggyBack(layer->getKey());

    wxTreeItemId folder = TreeFindFolder(fTree, fTreeId, wxT("Piggy Backs"));
    if (!folder.IsOk())
        folder = fTree->AppendItem(fTreeId, wxT("Piggy Backs"), ico_folder);
    TreeAddObject(fTree, folder, fResMgr, layer->getKey());
    lsPiggyBacks->AddKey(layer->getKey());
}

void wxGMaterial::OnPiggyAddClick(wxMenuEvent& evt)
{
    wxObjSelectFrame addFrame(NULL, wxID_ANY, wxT("Add Piggy-Back Layer"));

    hsGMaterial* mat = hsGMaterial::Convert(fKey->getObj());
    std::vector<plKey> keys = fResMgr->getKeys(fKey->getLocation(), kLayer);
    std::vector<plKey> animKeys = fResMgr->getKeys(fKey->getLocation(), kLayerAnimation);
    
    size_t keyEnd = keys.size();
    keys.resize(keys.size() + animKeys.size());
    std::copy(animKeys.begin(), animKeys.end(), keys.begin() + keyEnd);
    for (size_t i=0; i<keys.size(); i++)
        addFrame.AddObject(keys[i]);

    int which = addFrame.ShowModal();
    if (which != -1) {
        mat->addPiggyBack(keys[which]);
        lsPiggyBacks->AddKey(keys[which]);
        wxTreeItemId folder = TreeFindFolder(fTree, fTreeId, wxT("Piggy Backs"));
        if (!folder.IsOk())
            folder = fTree->AppendItem(fTreeId, wxT("Piggy Backs"), ico_folder);
        TreeAddObject(fTree, folder, fResMgr, keys[which]);
    }
}

void wxGMaterial::OnPiggyRemoveClick(wxMenuEvent& evt)
{
    hsGMaterial* mat = hsGMaterial::Convert(fKey->getObj());
    for (long i=0; i<lsPiggyBacks->GetItemCount(); ) {
        int state = lsPiggyBacks->GetItemState(i, wxLIST_STATE_SELECTED);
        if ((state & wxLIST_STATE_SELECTED) != 0) {
            plKey layer = mat->getPiggyBack(i);
            wxTreeItemId folder = TreeFindFolder(fTree, fTreeId, wxT("Piggy Backs"));
            wxTreeItemId tid = TreeFindKey(fTree, folder, layer);
            if (tid.IsOk())
                fTree->Delete(tid);
            if (fTree->GetChildrenCount(folder) == 0)
                fTree->Delete(folder);

            mat->delPiggyBack(i);
            lsPiggyBacks->DeleteItem(i);
        } else {
            i++;
        }
    }
}

void wxGMaterial::OnPiggyDeleteClick(wxMenuEvent& evt)
{
    hsGMaterial* mat = hsGMaterial::Convert(fKey->getObj());
    for (long i=0; i<lsPiggyBacks->GetItemCount(); ) {
        int state = lsPiggyBacks->GetItemState(i, wxLIST_STATE_SELECTED);
        if ((state & wxLIST_STATE_SELECTED) != 0) {
            plKey layer = mat->getPiggyBack(i);
            wxTreeItemId folder = TreeFindFolder(fTree, fTreeId, wxT("Piggy Backs"));
            wxTreeItemId tid = TreeFindKey(fTree, folder, layer);
            if (tid.IsOk())
                fTree->Delete(tid);
            if (fTree->GetChildrenCount(folder) == 0)
                fTree->Delete(folder);

            mat->delPiggyBack(i);
            fResMgr->DelObject(layer);
            lsPiggyBacks->DeleteItem(i);
        } else {
            i++;
        }
    }
}
