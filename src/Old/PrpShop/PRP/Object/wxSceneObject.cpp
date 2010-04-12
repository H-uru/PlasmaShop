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

#include "wxSceneObject.h"
#include <wx/statbmp.h>
#include <wx/panel.h>
#include <wx/imaglist.h>
#include <wx/sizer.h>
#include <wx/menu.h>
#include <wx/textdlg.h>

#include <PRP/Object/plSceneObject.h>
#include <PRP/Object/plDrawInterface.h>
#include <PRP/Object/plSimulationInterface.h>
#include <PRP/Object/plCoordinateInterface.h>
#include <PRP/Object/plAudioInterface.h>
#include <PRP/Modifier/plPythonFileMod.h>
#include <PRP/Modifier/plResponderModifier.h>
#include <PRP/Surface/plLayerInterface.h>

#include "../../../../rc/PrpImages.xpm"

wxTreeItemId TreeAddSceneObject(wxTreeCtrl* tree, const wxTreeItemId& parent,
                                plResManager* mgr, plKey key)
{
    wxTreeItemId tid =
    tree->AppendItem(parent, wxString(key->getName().cstr(), wxConvUTF8),
                     GetTypeIcon(key->getType()), -1, new PlasmaTreeItem(key));

    plSceneObject* obj = plSceneObject::Convert(key->getObj());
    if (obj->getDrawInterface().Exists())
        TreeAddObject(tree, tid, mgr, obj->getDrawInterface());
    if (obj->getSimInterface().Exists())
        TreeAddObject(tree, tid, mgr, obj->getSimInterface());
    if (obj->getCoordInterface().Exists())
        TreeAddObject(tree, tid, mgr, obj->getCoordInterface());
    if (obj->getAudioInterface().Exists())
        TreeAddObject(tree, tid, mgr, obj->getAudioInterface());

    if (obj->getNumInterfaces() > 0) {
        wxTreeItemId folder = tree->AppendItem(tid, wxT("Interfaces"), ico_folder);
        for (size_t i=0; i<obj->getNumInterfaces(); i++)
            TreeAddObject(tree, folder, mgr, obj->getInterface(i));
    }
    if (obj->getNumModifiers() > 0) {
        wxTreeItemId folder = tree->AppendItem(tid, wxT("Modifiers"), ico_folder);
        for (size_t i=0; i<obj->getNumModifiers(); i++)
            TreeAddObject(tree, folder, mgr, obj->getModifier(i));
    }

    return tid;
}

wxSceneObject::wxSceneObject(plKey key, plResManager* mgr,
                             wxTreeCtrl* tree, const wxTreeItemId& tid)
             : wxSynchedObject(key, mgr, tree, tid)
{ }

void wxSceneObject::AddPropPages(wxNotebook* nb)
{
    wxSynchedObject::AddPropPages(nb);
    plSceneObject* obj = plSceneObject::Convert(fKey->getObj());

    wxPanel* nbpage = new wxPanel(nb);
    wxStaticBitmap* bmpDraw = new wxStaticBitmap(nbpage, wxID_ANY, wxBitmap(XPM_spans2));
    wxStaticBitmap* bmpSim = new wxStaticBitmap(nbpage, wxID_ANY, wxBitmap(XPM_sim));
    wxStaticBitmap* bmpCoord = new wxStaticBitmap(nbpage, wxID_ANY, wxBitmap(XPM_coords));
    wxStaticBitmap* bmpAudio = new wxStaticBitmap(nbpage, wxID_ANY, wxBitmap(XPM_sound));

    lblDraw = new wxKeyLabel(nbpage, wxID_ANY, obj->getDrawInterface());
    lblSim = new wxKeyLabel(nbpage, wxID_ANY, obj->getSimInterface());
    lblCoord = new wxKeyLabel(nbpage, wxID_ANY, obj->getCoordInterface());
    lblAudio = new wxKeyLabel(nbpage, wxID_ANY, obj->getAudioInterface());

    lblADDraw = new wxLinkText(nbpage, wxID_ANY, wxEmptyString);
    lblADSim = new wxLinkText(nbpage, wxID_ANY, wxEmptyString);
    lblADCoord = new wxLinkText(nbpage, wxID_ANY, wxEmptyString);
    lblADAudio = new wxLinkText(nbpage, wxID_ANY, wxEmptyString);

    wxFlexGridSizer* szrProps = new wxFlexGridSizer(4, 4, 4, 4);
    szrProps->Add(bmpDraw);
    szrProps->Add(lblDraw);
    szrProps->Add(8, 0);
    szrProps->Add(lblADDraw);
    szrProps->Add(bmpSim);
    szrProps->Add(lblSim);
    szrProps->Add(0, 0);
    szrProps->Add(lblADSim);
    szrProps->Add(bmpCoord);
    szrProps->Add(lblCoord);
    szrProps->Add(0, 0);
    szrProps->Add(lblADCoord);
    szrProps->Add(bmpAudio);
    szrProps->Add(lblAudio);
    szrProps->Add(0, 0);
    szrProps->Add(lblADAudio);
    UpdateIntfLinks();

    lsInterfaces = new wxKeyListCtrl(nbpage, wxID_ANY, wxDefaultPosition, wxSize(240, 72));
    lsModifiers = new wxKeyListCtrl(nbpage, wxID_ANY, wxDefaultPosition, wxSize(240, 72));
    lsInterfaces->SetImageList(GetTypeImgList(), wxIMAGE_LIST_SMALL);
    lsModifiers->SetImageList(GetTypeImgList(), wxIMAGE_LIST_SMALL);
    for (size_t i=0; i<obj->getNumInterfaces(); i++)
        lsInterfaces->AddKey(obj->getInterface(i));
    for (size_t i=0; i<obj->getNumModifiers(); i++)
        lsModifiers->AddKey(obj->getModifier(i));

    wxFlexGridSizer* szrLists = new wxFlexGridSizer(2, 2, 2, 10);
    szrLists->Add(new wxStaticText(nbpage, wxID_ANY, wxT("Interfaces:")));
    szrLists->Add(new wxStaticText(nbpage, wxID_ANY, wxT("Modifiers:")));
    szrLists->Add(lsInterfaces);
    szrLists->Add(lsModifiers);

    wxFlexGridSizer* szrAll = new wxFlexGridSizer(2, 1, 12, 0);
    szrAll->Add(szrProps);
    szrAll->Add(szrLists);

    wxBoxSizer* border = new wxBoxSizer(0);
    border->Add(szrAll, 0, wxALL, 8);
    nbpage->SetSizerAndFit(border);
    nb->AddPage(nbpage, wxT("Scene Object"));

    // Connect events
    lblDraw->Connect(wxEVT_COMMAND_LINK,
                     (wxObjectEventFunction)&wxSceneObject::OnDrawIntfClick,
                     NULL, this);
    lblSim->Connect(wxEVT_COMMAND_LINK,
                    (wxObjectEventFunction)&wxSceneObject::OnSimIntfClick,
                    NULL, this);
    lblCoord->Connect(wxEVT_COMMAND_LINK,
                      (wxObjectEventFunction)&wxSceneObject::OnCoordIntfClick,
                      NULL, this);
    lblAudio->Connect(wxEVT_COMMAND_LINK,
                      (wxObjectEventFunction)&wxSceneObject::OnAudioIntfClick,
                      NULL, this);

    lblADDraw->Connect(wxEVT_COMMAND_LINK,
                       (wxObjectEventFunction)&wxSceneObject::OnADDrawClick,
                       NULL, this);
    lblADSim->Connect(wxEVT_COMMAND_LINK,
                      (wxObjectEventFunction)&wxSceneObject::OnADSimClick,
                      NULL, this);
    lblADCoord->Connect(wxEVT_COMMAND_LINK,
                        (wxObjectEventFunction)&wxSceneObject::OnADCoordClick,
                        NULL, this);
    lblADAudio->Connect(wxEVT_COMMAND_LINK,
                        (wxObjectEventFunction)&wxSceneObject::OnADAudioClick,
                        NULL, this);

    lsInterfaces->Connect(wxEVT_CONTEXT_MENU,
                          (wxObjectEventFunction)&wxSceneObject::OnIntfContextMenu,
                          NULL, this);
    lsModifiers->Connect(wxEVT_CONTEXT_MENU,
                         (wxObjectEventFunction)&wxSceneObject::OnModContextMenu,
                         NULL, this);
    lsInterfaces->Connect(wxEVT_COMMAND_LIST_ITEM_ACTIVATED,
                          (wxObjectEventFunction)&wxSceneObject::OnIntfItemActivated,
                          NULL, this);
    lsModifiers->Connect(wxEVT_COMMAND_LIST_ITEM_ACTIVATED,
                         (wxObjectEventFunction)&wxSceneObject::OnModItemActivated,
                         NULL, this);
}

wxWindow* wxSceneObject::MakePreviewPane(wxWindow* parent)
{
    fPreviewCanvas = new wxPrpCanvas(parent);
    fPreviewCanvas->AddObject(fKey);
    fPreviewCanvas->Center(fKey, false);
    fPreviewCanvas->Build(wxPrpCanvas::MODE_MODEL);
    
    return fPreviewCanvas;
}

void wxSceneObject::Refresh()
{
    fPreviewCanvas->ReBuild();
    fPreviewCanvas->Refresh();
}

void wxSceneObject::UpdateIntfLinks()
{
    plSceneObject* obj = plSceneObject::Convert(fKey->getObj());
    lblADDraw->SetLabel(obj->getDrawInterface().Exists() ? wxT("Delete") : wxT("Create"));
    lblADSim->SetLabel(obj->getSimInterface().Exists() ? wxT("Delete") : wxT("Create"));
    lblADCoord->SetLabel(obj->getCoordInterface().Exists() ? wxT("Delete") : wxT("Create"));
    lblADAudio->SetLabel(obj->getAudioInterface().Exists() ? wxT("Delete") : wxT("Create"));

    // Re-adjust sizers and update the tree if the Interfaces change
    lblDraw->GetParent()->Fit();
    fTree->Refresh();
}

void wxSceneObject::OnDrawIntfClick(wxCommandEvent& evt)
{
    plSceneObject* obj = plSceneObject::Convert(fKey->getObj());
    if (!obj->getDrawInterface().Exists())
        return;
    
    wxTreeItemId tid = TreeFindKey(fTree, fTreeId, obj->getDrawInterface());
    if (tid.IsOk())
        fTree->SelectItem(tid);
}

void wxSceneObject::OnSimIntfClick(wxCommandEvent& evt)
{
    plSceneObject* obj = plSceneObject::Convert(fKey->getObj());
    if (!obj->getSimInterface().Exists())
        return;
    
    wxTreeItemId tid = TreeFindKey(fTree, fTreeId, obj->getSimInterface());
    if (tid.IsOk())
        fTree->SelectItem(tid);
}

void wxSceneObject::OnCoordIntfClick(wxCommandEvent& evt)
{
    plSceneObject* obj = plSceneObject::Convert(fKey->getObj());
    if (!obj->getCoordInterface().Exists())
        return;
    
    wxTreeItemId tid = TreeFindKey(fTree, fTreeId, obj->getCoordInterface());
    if (tid.IsOk())
        fTree->SelectItem(tid);
}

void wxSceneObject::OnAudioIntfClick(wxCommandEvent& evt)
{
    plSceneObject* obj = plSceneObject::Convert(fKey->getObj());
    if (!obj->getAudioInterface().Exists())
        return;
    
    wxTreeItemId tid = TreeFindKey(fTree, fTreeId, obj->getAudioInterface());
    if (tid.IsOk())
        fTree->SelectItem(tid);
}

void wxSceneObject::OnADDrawClick(wxCommandEvent& evt)
{
    plSceneObject* obj = plSceneObject::Convert(fKey->getObj());
    if (obj->getDrawInterface().Exists()) {
        // Delete
        plKey intf = obj->getDrawInterface();
        wxTreeItemId tid = TreeFindKey(fTree, fTreeId, intf);
        if (tid.IsOk())
            fTree->Delete(tid);

        fResMgr->DelObject(intf);
        obj->setDrawInterface(plKey());
    } else {
        // Add
        plDrawInterface* intf = new plDrawInterface();
        intf->init(obj->getKey()->getName());
        fResMgr->AddObject(obj->getKey()->getLocation(), intf);

        TreeAddObject(fTree, fTreeId, fResMgr, intf->getKey());
        obj->setDrawInterface(intf->getKey());
        intf->setOwner(obj->getKey());
    }
    lblDraw->SetKey(obj->getDrawInterface());
    UpdateIntfLinks();
}

void wxSceneObject::OnADSimClick(wxCommandEvent& evt)
{
    plSceneObject* obj = plSceneObject::Convert(fKey->getObj());
    if (obj->getSimInterface().Exists()) {
        // Delete
        plKey intf = obj->getSimInterface();
        wxTreeItemId tid = TreeFindKey(fTree, fTreeId, intf);
        if (tid.IsOk())
            fTree->Delete(tid);

        fResMgr->DelObject(intf);
        obj->setSimInterface(plKey());
    } else {
        // Add
        plSimulationInterface* intf = new plSimulationInterface();
        intf->init(obj->getKey()->getName());
        fResMgr->AddObject(obj->getKey()->getLocation(), intf);

        TreeAddObject(fTree, fTreeId, fResMgr, intf->getKey());
        obj->setSimInterface(intf->getKey());
        intf->setOwner(obj->getKey());
    }
    lblSim->SetKey(obj->getSimInterface());
    UpdateIntfLinks();
}

void wxSceneObject::OnADCoordClick(wxCommandEvent& evt)
{
    plSceneObject* obj = plSceneObject::Convert(fKey->getObj());
    if (obj->getCoordInterface().Exists()) {
        // Delete
        plKey intf = obj->getCoordInterface();
        wxTreeItemId tid = TreeFindKey(fTree, fTreeId, intf);
        if (tid.IsOk())
            fTree->Delete(tid);

        fResMgr->DelObject(intf);
        obj->setCoordInterface(plKey());
    } else {
        // Add
        plCoordinateInterface* intf = new plCoordinateInterface();
        intf->init(obj->getKey()->getName());
        fResMgr->AddObject(obj->getKey()->getLocation(), intf);

        TreeAddObject(fTree, fTreeId, fResMgr, intf->getKey());
        obj->setCoordInterface(intf->getKey());
        intf->setOwner(obj->getKey());
    }
    lblCoord->SetKey(obj->getCoordInterface());
    UpdateIntfLinks();
}

void wxSceneObject::OnADAudioClick(wxCommandEvent& evt)
{
    plSceneObject* obj = plSceneObject::Convert(fKey->getObj());
    if (obj->getAudioInterface().Exists()) {
        // Delete
        plKey intf = obj->getAudioInterface();
        wxTreeItemId tid = TreeFindKey(fTree, fTreeId, intf);
        if (tid.IsOk())
            fTree->Delete(tid);

        fResMgr->DelObject(intf);
        obj->setAudioInterface(plKey());
    } else {
        // Add
        plAudioInterface* intf = new plAudioInterface();
        intf->init(obj->getKey()->getName());
        fResMgr->AddObject(obj->getKey()->getLocation(), intf);

        TreeAddObject(fTree, fTreeId, fResMgr, intf->getKey());
        obj->setAudioInterface(intf->getKey());
        intf->setOwner(obj->getKey());
    }
    lblAudio->SetKey(obj->getAudioInterface());
    UpdateIntfLinks();
}

void wxSceneObject::OnIntfContextMenu(wxCommandEvent& evt)
{
    wxMenu* menuAddIntf = new wxMenu(wxT(""));
    menuAddIntf->Append(wxID_ANY, wxT("plBlahBlahInterface"));

    wxMenu menu(wxT(""));
    menu.AppendSubMenu(menuAddIntf, wxT("Add..."));
    menu.Append(ID_OBJ_DELETE, wxT("Delete"));
    if (lsInterfaces->GetSelectedItemCount() < 1)
        menu.Enable(ID_OBJ_DELETE, false);

    menu.Connect(ID_OBJ_DELETE, wxEVT_COMMAND_MENU_SELECTED,
                 (wxObjectEventFunction)&wxSceneObject::OnIntfDeleteClick,
                 NULL, this);

    lsInterfaces->PopupMenu(&menu);
}

void wxSceneObject::OnModContextMenu(wxCommandEvent& evt)
{
    wxMenu* menuAddMod = new wxMenu(wxT(""));
    menuAddMod->Append(ID_MOD_NEW_PYTHON, wxT("plPythonFileMod"));
    menuAddMod->Append(ID_MOD_NEW_RESPONDER, wxT("plResponderModifier"));

    wxMenu menu(wxT(""));
    menu.AppendSubMenu(menuAddMod, wxT("Add..."));
    menu.Append(ID_OBJ_DELETE, wxT("Delete"));
    if (lsModifiers->GetSelectedItemCount() < 1)
        menu.Enable(ID_OBJ_DELETE, false);

    menu.Connect(ID_OBJ_DELETE, wxEVT_COMMAND_MENU_SELECTED,
                 (wxObjectEventFunction)&wxSceneObject::OnModDeleteClick,
                 NULL, this);
    menu.Connect(ID_MOD_NEW_PYTHON, wxEVT_COMMAND_MENU_SELECTED,
                 (wxObjectEventFunction)&wxSceneObject::OnNewPythonClick,
                 NULL, this);
    menu.Connect(ID_MOD_NEW_RESPONDER, wxEVT_COMMAND_MENU_SELECTED,
                 (wxObjectEventFunction)&wxSceneObject::OnNewResponderClick,
                 NULL, this);

    lsModifiers->PopupMenu(&menu);
}

void wxSceneObject::OnIntfItemActivated(wxListEvent& evt)
{
    plSceneObject* obj = plSceneObject::Convert(fKey->getObj());
    wxTreeItemId folder = TreeFindFolder(fTree, fTreeId, wxT("Interfaces"));
    wxTreeItemId tid = TreeFindKey(fTree, folder, obj->getInterface(evt.GetIndex()));
    if (tid.IsOk())
        fTree->SelectItem(tid);
}

void wxSceneObject::OnIntfDeleteClick(wxCommandEvent& evt)
{
    plSceneObject* obj = plSceneObject::Convert(fKey->getObj());

    for (long i=0; i<lsInterfaces->GetItemCount(); ) {
        int state = lsInterfaces->GetItemState(i, wxLIST_STATE_SELECTED);
        if ((state & wxLIST_STATE_SELECTED) != 0) {
            plKey intf = obj->getInterface(i);
            wxTreeItemId folder = TreeFindFolder(fTree, fTreeId, wxT("Interfaces"));
            wxTreeItemId tid = TreeFindKey(fTree, folder, intf);
            if (tid.IsOk())
                fTree->Delete(tid);
            if (fTree->GetChildrenCount(folder) == 0)
                fTree->Delete(folder);

            obj->delInterface(i);
            fResMgr->DelObject(intf);
            lsInterfaces->DeleteItem(i);
        } else {
            i++;
        }
    }
}

void wxSceneObject::OnModItemActivated(wxListEvent& evt)
{
    plSceneObject* obj = plSceneObject::Convert(fKey->getObj());
    wxTreeItemId folder = TreeFindFolder(fTree, fTreeId, wxT("Modifiers"));
    wxTreeItemId tid = TreeFindKey(fTree, folder, obj->getModifier(evt.GetIndex()));
    if (tid.IsOk())
        fTree->SelectItem(tid);
}

void wxSceneObject::OnModDeleteClick(wxCommandEvent& evt)
{
    plSceneObject* obj = plSceneObject::Convert(fKey->getObj());

    for (long i=0; i<lsModifiers->GetItemCount(); ) {
        int state = lsModifiers->GetItemState(i, wxLIST_STATE_SELECTED);
        if ((state & wxLIST_STATE_SELECTED) != 0) {
            plKey mod = obj->getModifier(i);
            wxTreeItemId folder = TreeFindFolder(fTree, fTreeId, wxT("Modifiers"));
            wxTreeItemId tid = TreeFindKey(fTree, folder, mod);
            if (tid.IsOk())
                fTree->Delete(tid);
            if (fTree->GetChildrenCount(folder) == 0)
                fTree->Delete(folder);

            obj->delModifier(i);
            fResMgr->DelObject(mod);
            lsModifiers->DeleteItem(i);
        } else {
            i++;
        }
    }
}

void wxSceneObject::OnNewPythonClick(wxCommandEvent& evt)
{
    wxString name = wxGetTextFromUser(wxT("Object Name"), wxT("Add plPythonFileMod"));
    if (name == wxEmptyString)
        return;

    plSceneObject* obj = plSceneObject::Convert(fKey->getObj());
    plPythonFileMod* mod = new plPythonFileMod();
    mod->init((const char*)name.mb_str());
    fResMgr->AddObject(fKey->getLocation(), mod);
    obj->addModifier(mod->getKey());

    wxTreeItemId folder = TreeFindFolder(fTree, fTreeId, wxT("Modifiers"));
    if (!folder.IsOk())
        folder = fTree->AppendItem(fTreeId, wxT("Modifiers"), ico_folder);
    TreeAddObject(fTree, folder, fResMgr, mod->getKey());
    lsModifiers->AddKey(mod->getKey());
}

void wxSceneObject::OnNewResponderClick(wxCommandEvent& evt)
{
    wxString name = wxGetTextFromUser(wxT("Object Name"), wxT("Add plResponderModifier"));
    if (name == wxEmptyString)
        return;

    plSceneObject* obj = plSceneObject::Convert(fKey->getObj());
    plResponderModifier* mod = new plResponderModifier();
    mod->init((const char*)name.mb_str());
    fResMgr->AddObject(fKey->getLocation(), mod);
    obj->addModifier(mod->getKey());

    wxTreeItemId folder = TreeFindFolder(fTree, fTreeId, wxT("Modifiers"));
    if (!folder.IsOk())
        folder = fTree->AppendItem(fTreeId, wxT("Modifiers"), ico_folder);
    TreeAddObject(fTree, folder, fResMgr, mod->getKey());
    lsModifiers->AddKey(mod->getKey());
}
