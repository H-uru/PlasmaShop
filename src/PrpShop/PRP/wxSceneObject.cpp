#include "wxSceneObject.h"
#include <PRP/Object/plSceneObject.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/msgdlg.h>

wxTreeItemId TreeAddSceneObject(wxTreeCtrl* tree, const wxTreeItemId& parent,
                                plResManager* mgr, plKey key)
{
    wxTreeItemId tid =
    tree->AppendItem(parent, wxString(key->getName().cstr(), wxConvUTF8),
                     ico_sceneobj, -1, new PlasmaTreeItem(key));

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

    wxPanel* pnl = new wxPanel(nb);
    wxButton* btn = new wxButton(pnl, wxID_ANY, wxT("Blah"));
    wxGridSizer* szr = new wxGridSizer(1, 1);
    szr->Add(btn);
    pnl->SetSizer(szr);
    nb->AddPage(pnl, wxT("Test"));
    btn->Connect(wxID_ANY, wxEVT_COMMAND_BUTTON_CLICKED,
                 (wxObjectEventFunction)&wxSceneObject::OnButton1);
}

void wxSceneObject::SaveDamage()
{
    wxSynchedObject::SaveDamage();
    // TODO
}

void wxSceneObject::OnButton1(wxCommandEvent& evt)
{
    wxMessageBox(wxT("Clicking"), wxT("Test"), wxID_OK);
}
