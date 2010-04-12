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

#include "wxCoordinateInterface.h"
#include "../../wxObjSelectFrame.h"
#include <wx/imaglist.h>

#include <PRP/Object/plCoordinateInterface.h>
#include <PRP/plSceneNode.h>

wxCoordinateInterface::wxCoordinateInterface(plKey key, plResManager* mgr,
                                             wxTreeCtrl* tree, const wxTreeItemId& tid)
                     : wxSynchedObject(key, mgr, tree, tid)
{ }

void wxCoordinateInterface::AddPropPages(wxNotebook* nb)
{
    wxSynchedObject::AddPropPages(nb);
    plCoordinateInterface* intf = plCoordinateInterface::Convert(fKey->getObj());

    wxPanel* nbpage = new wxPanel(nb);

    cbDisable = new wxCheckBox(nbpage, wxID_ANY, wxT("Disabled"));
    cbCanEverDelayTransform = new wxCheckBox(nbpage, wxID_ANY, wxT("Can Delay Transform"));
    cbDelayedTransformEval = new wxCheckBox(nbpage, wxID_ANY, wxT("Delayed Transform"));
    cbDisable->SetValue(intf->getProperty(plCoordinateInterface::kDisable));
    cbCanEverDelayTransform->SetValue(intf->getProperty(plCoordinateInterface::kCanEverDelayTransform));
    cbDelayedTransformEval->SetValue(intf->getProperty(plCoordinateInterface::kDelayedTransformEval));

    wxFlexGridSizer* szrProps = new wxFlexGridSizer(3, 1, 4, 0);
    szrProps->Add(cbDisable);
    szrProps->Add(cbCanEverDelayTransform);
    szrProps->Add(cbDelayedTransformEval);

    lsChildren = new wxKeyListCtrl(nbpage, wxID_ANY, wxDefaultPosition, wxSize(240, 72));
    lsChildren->SetImageList(GetTypeImgList(), wxIMAGE_LIST_SMALL);
    for (size_t i=0; i<intf->getNumChildren(); i++)
        lsChildren->AddKey(intf->getChild(i));

    wxFlexGridSizer* szrInterface = new wxFlexGridSizer(2, 2, 8, 32);
    szrInterface->Add(new wxStaticText(nbpage, wxID_ANY, wxT("Properties:")));
    szrInterface->Add(new wxStaticText(nbpage, wxID_ANY, wxT("Children:")));
    szrInterface->Add(szrProps);
    szrInterface->Add(lsChildren);

    wxBoxSizer* border = new wxBoxSizer(0);
    border->Add(szrInterface, 0, wxALL, 8);
    nbpage->SetSizerAndFit(border);
    nb->AddPage(nbpage, wxT("Coordinate Interface"));

    // Events
    lsChildren->Connect(wxEVT_CONTEXT_MENU,
                        (wxObjectEventFunction)&wxCoordinateInterface::OnChildrenMenu,
                        NULL, this);
    lsChildren->Connect(wxEVT_COMMAND_LIST_ITEM_ACTIVATED,
                        (wxObjectEventFunction)&wxCoordinateInterface::OnChildrenActivate,
                        NULL, this);
}

wxWindow* wxCoordinateInterface::MakePreviewPane(wxWindow* parent)
{
    plCoordinateInterface* intf = plCoordinateInterface::Convert(fKey->getObj());
    plSceneObject* obj = GET_KEY_OBJECT(intf->getOwner(), plSceneObject);
    if (obj == NULL)
        return NULL;
    plSceneNode* node = GET_KEY_OBJECT(obj->getSceneNode(), plSceneNode);
    if (node == NULL)
        return NULL;

    fPreviewCanvas = new wxPrpCanvas(parent);
    for (size_t i=0; i<node->getNumSceneObjects(); i++)
        fPreviewCanvas->AddObject(node->getSceneObject(i));
    fPreviewCanvas->Center(obj->getKey(), true);
    fPreviewCanvas->Build(wxPrpCanvas::MODE_MODEL_IN_SCENE);
    
    return fPreviewCanvas;
}

void wxCoordinateInterface::SaveDamage()
{
    wxSynchedObject::SaveDamage();
    plCoordinateInterface* intf = plCoordinateInterface::Convert(fKey->getObj());
    intf->setProperty(plCoordinateInterface::kDisable, cbDisable->GetValue());
    intf->setProperty(plCoordinateInterface::kCanEverDelayTransform, cbCanEverDelayTransform->GetValue());
    intf->setProperty(plCoordinateInterface::kDelayedTransformEval, cbDelayedTransformEval->GetValue());
}

void wxCoordinateInterface::Refresh()
{
    fPreviewCanvas->ReBuild();
    fPreviewCanvas->Refresh();
}

void wxCoordinateInterface::OnChildrenMenu(wxCommandEvent& evt)
{
    wxMenu menu(wxT(""));
    menu.Append(ID_OBJ_ADD, wxT("Add Ref..."));
    menu.Append(ID_OBJ_DELETE, wxT("Delete Ref"));
    if (lsChildren->GetSelectedItemCount() < 1)
        menu.Enable(ID_OBJ_DELETE, false);

    menu.Connect(ID_OBJ_ADD, wxEVT_COMMAND_MENU_SELECTED,
                 (wxObjectEventFunction)&wxCoordinateInterface::OnAddChildClick,
                 NULL, this);
    menu.Connect(ID_OBJ_DELETE, wxEVT_COMMAND_MENU_SELECTED,
                 (wxObjectEventFunction)&wxCoordinateInterface::OnDelChildClick,
                 NULL, this);

    lsChildren->PopupMenu(&menu);
}

void wxCoordinateInterface::OnChildrenActivate(wxListEvent& evt)
{
    plCoordinateInterface* intf = plCoordinateInterface::Convert(fKey->getObj());
    wxTreeItemId folder = fTree->GetItemParent(fTree->GetItemParent(fTreeId));
    wxTreeItemId tid = TreeFindKey(fTree, folder, intf->getChild(evt.GetIndex()));
    if (tid.IsOk())
        fTree->SelectItem(tid);
}

void wxCoordinateInterface::OnAddChildClick(wxMenuEvent& evt)
{
    wxObjSelectFrame addFrame(NULL, wxID_ANY, wxT("Add CoordinateInterface Child"));

    plCoordinateInterface* intf = plCoordinateInterface::Convert(fKey->getObj());
    std::vector<plKey> keys = fResMgr->getKeys(fKey->getLocation(), kSceneObject);
    for (size_t i=0; i<keys.size(); i++)
        addFrame.AddObject(keys[i]);

    int which = addFrame.ShowModal();
    if (which != -1) {
        intf->addChild(keys[which]);
        lsChildren->AddKey(keys[which]);
    }
}

void wxCoordinateInterface::OnDelChildClick(wxMenuEvent& evt)
{
    plCoordinateInterface* intf = plCoordinateInterface::Convert(fKey->getObj());
    for (long i=0; i<lsChildren->GetItemCount(); ) {
        int state = lsChildren->GetItemState(i, wxLIST_STATE_SELECTED);
        if ((state & wxLIST_STATE_SELECTED) != 0) {
            intf->delChild(i);
            lsChildren->DeleteItem(i);
        } else {
            i++;
        }
    }
}
