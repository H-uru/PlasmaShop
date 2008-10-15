#include "wxSceneNode.h"

#include <PRP/plSceneNode.h>
#include <PRP/Object/plCoordinateInterface.h>

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