#include "wxPrpPlasma.h"
#include <ResManager/plFactory.h>

#include "PRP/wxSceneNode.h"
#include "PRP/wxSceneObject.h"

wxPrpPlasmaObject::wxPrpPlasmaObject(plKey key, plResManager* mgr)
                 : fKey(key), fResMgr(mgr)
{ }

void wxPrpPlasmaObject::AddPropPages(wxNotebook* nb)
{ }

wxTreeItemId TreeAddObject(wxTreeCtrl* tree, const wxTreeItemId& parent,
                           plResManager* mgr, plKey key)
{
    switch (key->getType()) {
    case kSceneNode:
        return TreeAddSceneNode(tree, parent, mgr, key);
    case kSceneObject:
        return TreeAddSceneObject(tree, parent, mgr, key);
    case kMipmap:
        return tree->AppendItem(parent, wxString(key->getName().cstr(), wxConvUTF8), ico_img);
    case kCubicEnvironmap:
        return tree->AppendItem(parent, wxString(key->getName().cstr(), wxConvUTF8), ico_img);
    default:
        return tree->AppendItem(parent,
                                wxT("[") + wxString(plFactory::ClassName(key->getType()), wxConvUTF8) + wxT("] ") +
                                wxString(key->getName().cstr(), wxConvUTF8));
    }
}

wxPrpPlasmaObject* AddPropPages(wxNotebook* nb, plResManager* mgr, plKey key)
{
    wxPrpPlasmaObject* obj = NULL;

    switch (key->getType()) {
    case kSceneNode:
        obj = new wxSceneNode(key, mgr);
        break;
    case kSceneObject:
        obj = new wxSceneObject(key, mgr);
        break;
    }

    obj->AddPropPages(nb);
    return obj;
}
