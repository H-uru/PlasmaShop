/* This file is part of PlasmaShop.
 *
 * PlasmaShop is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PlasmaShop is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PlasmaShop.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "wxSceneNode.h"
#include <wx/imaglist.h>

#include <PRP/plSceneNode.h>
#include <PRP/Object/plCoordinateInterface.h>

wxTreeItemId TreeAddSceneNode(wxTreeCtrl* tree, const wxTreeItemId& parent,
                              plResManager* mgr, plKey key)
{
    wxTreeItemId tid =
    tree->AppendItem(parent, wxString(key->getName().cstr(), wxConvUTF8),
                     GetTypeIcon(key->getType()), -1, new PlasmaTreeItem(key));

    plSceneNode* node = plSceneNode::Convert(key->getObj());
    if (node->getNumSceneObjects() > 0) {
        wxTreeItemId folder = tree->AppendItem(tid, wxT("Scene Objects"), ico_folder);
        for (size_t i=0; i<node->getNumSceneObjects(); i++)
            TreeAddObject(tree, folder, mgr, node->getSceneObject(i));
        tree->SortChildren(folder);
    }
    if (node->getNumPoolObjects() > 0) {
        wxTreeItemId folder = tree->AppendItem(tid, wxT("Pool Objects"), ico_folder);
        for (size_t i=0; i<node->getNumPoolObjects(); i++)
            TreeAddObject(tree, folder, mgr, node->getPoolObject(i));
        tree->SortChildren(folder);
    }

    return tid;
}

wxSceneNode::wxSceneNode(plKey key, plResManager* mgr,
                         wxTreeCtrl* tree, const wxTreeItemId& tid)
           : wxPrpPlasmaObject(key, mgr, tree, tid)
{ }

void wxSceneNode::AddPropPages(wxNotebook* nb)
{
    wxPrpPlasmaObject::AddPropPages(nb);
    plSceneNode* node = plSceneNode::Convert(fKey->getObj());
    
    wxPanel* nbpage = new wxPanel(nb);
    lsSceneObjects = new wxKeyListCtrl(nbpage, wxID_ANY, wxDefaultPosition, wxSize(240, 160));
    lsPoolObjects = new wxKeyListCtrl(nbpage, wxID_ANY, wxDefaultPosition, wxSize(240, 160));
    lsSceneObjects->SetImageList(GetTypeImgList(), wxIMAGE_LIST_SMALL);
    lsPoolObjects->SetImageList(GetTypeImgList(), wxIMAGE_LIST_SMALL);
    for (size_t i=0; i<node->getNumSceneObjects(); i++)
        lsSceneObjects->AddKey(node->getSceneObject(i));
    for (size_t i=0; i<node->getNumPoolObjects(); i++)
        lsPoolObjects->AddKey(node->getPoolObject(i));
    
    wxFlexGridSizer* szrLists = new wxFlexGridSizer(2, 2, 2, 10);
    szrLists->Add(new wxStaticText(nbpage, wxID_ANY, wxT("Scene Objects:")));
    szrLists->Add(new wxStaticText(nbpage, wxID_ANY, wxT("Pool Objects:")));
    szrLists->Add(lsSceneObjects);
    szrLists->Add(lsPoolObjects);

    wxBoxSizer* border = new wxBoxSizer(0);
    border->Add(szrLists, 0, wxALL, 8);
    nbpage->SetSizerAndFit(border);
    nb->AddPage(nbpage, wxT("Scene Node"));

    // Events
    lsSceneObjects->Connect(wxEVT_CONTEXT_MENU,
                            (wxObjectEventFunction)&wxSceneNode::OnSceneObjectMenu,
                            NULL, this);
    lsPoolObjects->Connect(wxEVT_CONTEXT_MENU,
                           (wxObjectEventFunction)&wxSceneNode::OnPoolObjectMenu,
                           NULL, this);
    lsSceneObjects->Connect(wxEVT_COMMAND_LIST_ITEM_ACTIVATED,
                            (wxObjectEventFunction)&wxSceneNode::OnSceneObjectActivated,
                            NULL, this);
    lsPoolObjects->Connect(wxEVT_COMMAND_LIST_ITEM_ACTIVATED,
                           (wxObjectEventFunction)&wxSceneNode::OnPoolObjectActivated,
                           NULL, this);
}

wxWindow* wxSceneNode::MakePreviewPane(wxWindow* parent)
{
    plSceneNode* node = plSceneNode::Convert(fKey->getObj());
    fPreviewCanvas = new wxPrpCanvas(parent);

    hsVector3 spawn;
    float spawnAngle = 0.0f;
    bool haveSpawn = false;
    for (size_t i=0; i<node->getNumSceneObjects(); i++) {
        fPreviewCanvas->AddObject(node->getSceneObject(i));
        plSceneObject* obj = plSceneObject::Convert(node->getSceneObject(i)->getObj());
        for (size_t j=0; j<obj->getNumModifiers(); j++) {
            if (obj->getModifier(j)->getType() == kSpawnModifier) {
                if (!haveSpawn || obj->getKey()->getName() == "LinkInPointDefault") {
                    plCoordinateInterface* coord = plCoordinateInterface::Convert(obj->getCoordInterface()->getObj());
                    hsMatrix44 mat = coord->getLocalToWorld();
                    spawn = hsVector3(mat(0, 3), mat(1, 3), mat(2, 3) + 5.0f);
                    spawnAngle = atan2f(mat(1, 0), mat(0, 0));
                    haveSpawn = true;
                }
            }
        }
    }

    fPreviewCanvas->Build(wxPrpCanvas::MODE_SCENE);
    fPreviewCanvas->SetView(spawn, spawnAngle);

    return fPreviewCanvas;
}

void wxSceneNode::Refresh()
{
    fPreviewCanvas->ReBuild();
    fPreviewCanvas->Refresh();
}

void wxSceneNode::OnSceneObjectMenu(wxCommandEvent& evt)
{
    wxMenu menu(wxT(""));
    menu.Append(ID_SCENEOBJ_ADD, wxT("New Scene Object"));
    menu.Append(ID_SCENEOBJ_DEL, wxT("Delete"));
    if (lsSceneObjects->GetSelectedItemCount() < 1)
        menu.Enable(ID_SCENEOBJ_DEL, false);

    menu.Connect(ID_SCENEOBJ_ADD, wxEVT_COMMAND_MENU_SELECTED,
                 (wxObjectEventFunction)&wxSceneNode::OnSceneObjAddClick,
                 NULL, this);
    menu.Connect(ID_SCENEOBJ_DEL, wxEVT_COMMAND_MENU_SELECTED,
                 (wxObjectEventFunction)&wxSceneNode::OnSceneObjDelClick,
                 NULL, this);

    lsSceneObjects->PopupMenu(&menu);
}

void wxSceneNode::OnPoolObjectMenu(wxCommandEvent& evt)
{
}

void wxSceneNode::OnSceneObjectActivated(wxListEvent& evt)
{
    plSceneNode* node = plSceneNode::Convert(fKey->getObj());
    wxTreeItemId folder = TreeFindFolder(fTree, fTreeId, wxT("Scene Objects"));
    wxTreeItemId tid = TreeFindKey(fTree, folder, node->getSceneObject(evt.GetIndex()));
    if (tid.IsOk())
        fTree->SelectItem(tid);
}

void wxSceneNode::OnPoolObjectActivated(wxListEvent& evt)
{
    plSceneNode* node = plSceneNode::Convert(fKey->getObj());
    wxTreeItemId folder = TreeFindFolder(fTree, fTreeId, wxT("Pool Objects"));
    wxTreeItemId tid = TreeFindKey(fTree, folder, node->getPoolObject(evt.GetIndex()));
    if (tid.IsOk())
        fTree->SelectItem(tid);
}

void wxSceneNode::OnSceneObjAddClick(wxMenuEvent& evt)
{
    wxString name = wxGetTextFromUser(wxT("Object Name"), wxT("Add Scene Object"));
    if (name == wxEmptyString)
        return;

    plSceneNode* node = plSceneNode::Convert(fKey->getObj());
    plSceneObject* obj = new plSceneObject();
    obj->init((const char*)name.mb_str());
    fResMgr->AddObject(fKey->getLocation(), obj);
    node->getSceneObjects().append(obj->getKey());
    obj->setSceneNode(fKey);

    wxTreeItemId folder = TreeFindFolder(fTree, fTreeId, wxT("Scene Objects"));
    if (!folder.IsOk())
        folder = fTree->AppendItem(fTreeId, wxT("Scene Objects"), ico_folder);
    TreeAddObject(fTree, folder, fResMgr, obj->getKey());
    lsSceneObjects->AddKey(obj->getKey());
}

void wxSceneNode::OnSceneObjDelClick(wxMenuEvent& evt)
{
    plSceneNode* node = plSceneNode::Convert(fKey->getObj());

    for (long i=0; i<lsSceneObjects->GetItemCount(); ) {
        int state = lsSceneObjects->GetItemState(i, wxLIST_STATE_SELECTED);
        if ((state & wxLIST_STATE_SELECTED) != 0) {
            plKey obj = node->getSceneObject(i);
            wxTreeItemId folder = TreeFindFolder(fTree, fTreeId, wxT("Scene Objects"));
            wxTreeItemId tid = TreeFindKey(fTree, folder, obj);
            if (tid.IsOk())
                fTree->Delete(tid);
            if (fTree->GetChildrenCount(folder) == 0)
                fTree->Delete(folder);

            node->getSceneObjects().remove(i);
            fResMgr->DelObject(obj);
            lsSceneObjects->DeleteItem(i);
        } else {
            i++;
        }
    }
}
