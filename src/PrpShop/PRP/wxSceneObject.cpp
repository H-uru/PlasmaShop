#include "wxSceneObject.h"
#include <PRP/Object/plSceneObject.h>

wxTreeItemId TreeAddSceneObject(wxTreeCtrl* tree, const wxTreeItemId& parent,
                                plResManager* mgr, plKey key)
{
    wxTreeItemId tid =
    tree->AppendItem(parent, wxString(key->getName().cstr(), wxConvUTF8),
                     ico_sceneobj, -1, new PlasmaTreeItem(key->getObj()));

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

wxSceneObject::wxSceneObject(plKey key, plResManager* mgr)
             : wxPrpPlasmaObject(key, mgr)
{ }

void wxSceneObject::AddPropPages(wxNotebook* nb)
{
    // TODO
}
