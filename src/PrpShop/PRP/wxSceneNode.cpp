#include "wxSceneNode.h"
#include <PRP/plSceneNode.h>

wxTreeItemId TreeAddSceneNode(wxTreeCtrl* tree, const wxTreeItemId& parent,
                              plResManager* mgr, plKey key)
{
    wxTreeItemId tid =
    tree->AppendItem(parent, wxString(key->getName().cstr(), wxConvUTF8),
                     ico_scenenode, -1, new PlasmaTreeItem(key));

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
    // TODO
}

void wxSceneNode::SaveDamage()
{
    wxPrpPlasmaObject::SaveDamage();
    // TODO
}
