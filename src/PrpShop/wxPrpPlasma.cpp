#include "wxPrpPlasma.h"
#include <ResManager/plFactory.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/panel.h>

#include "PRP/wxSceneNode.h"
#include "PRP/wxSceneObject.h"

wxPrpPlasmaObject::wxPrpPlasmaObject(plKey key, plResManager* mgr)
                 : fKey(key), fResMgr(mgr)
{ }

void wxPrpPlasmaObject::AddPropPages(wxNotebook* nb)
{ }

void wxPrpPlasmaObject::AddKeyPage(wxNotebook* nb)
{
    wxPanel* page = new wxPanel(nb);
    wxString objName(fKey->getName(), wxConvUTF8);
    wxString objType(fKey->getObj()->ClassName(), wxConvUTF8);
    wxString loadMask = wxString::Format(wxT("%04X"), fKey->getLoadMask().getMask());
    wxString id = wxString::Format(wxT("%d"), fKey->getID());

    wxStaticText* lbl1 = new wxStaticText(page, wxID_ANY, wxT("Name:"));
    wxStaticText* lbl2 = new wxStaticText(page, wxID_ANY, wxT("Type:"));
    wxStaticText* lbl3 = new wxStaticText(page, wxID_ANY, wxT("Load Mask:"));
    wxTextCtrl* txtName = new wxTextCtrl(page, wxID_ANY, objName, wxDefaultPosition, wxSize(200, -1));
    wxStaticText* lblType = new wxStaticText(page, wxID_ANY, objType);
    wxTextCtrl* txtMask = new wxTextCtrl(page, wxID_ANY, loadMask, wxDefaultPosition, wxSize(40, -1));

    wxFlexGridSizer* props = new wxFlexGridSizer(4, 2, 4, 4);
    props->Add(lbl1);
    props->Add(txtName);
    props->Add(lbl2);
    props->Add(lblType);
    props->Add(0, 10);
    props->Add(0, 10);
    props->Add(lbl3);
    props->Add(txtMask);

    wxBoxSizer* border = new wxBoxSizer(0);
    border->Add(props, 0, wxALL, 8);
    page->SetSizerAndFit(border);
    nb->InsertPage(0, page, wxT("Basic"), true);
}

wxTreeItemId TreeAddObject(wxTreeCtrl* tree, const wxTreeItemId& parent,
                           plResManager* mgr, plKey key)
{
    switch (key->getType()) {
    case kSceneNode:
        return TreeAddSceneNode(tree, parent, mgr, key);
    case kSceneObject:
        return TreeAddSceneObject(tree, parent, mgr, key);
    case kMipmap:
        return tree->AppendItem(parent, wxString(key->getName().cstr(), wxConvUTF8),
                                ico_img, -1, new PlasmaTreeItem(key));
    case kCubicEnvironmap:
        return tree->AppendItem(parent, wxString(key->getName().cstr(), wxConvUTF8),
                                ico_img, -1, new PlasmaTreeItem(key));
    default:
        return tree->AppendItem(parent,
                                wxT("[") + wxString(plFactory::ClassName(key->getType()), wxConvUTF8) + wxT("] ") +
                                wxString(key->getName().cstr(), wxConvUTF8),
                                -1, -1, new PlasmaTreeItem(key));
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
    default:
        obj = new wxPrpPlasmaObject(key, mgr);
        break;
    }

    obj->AddPropPages(nb);
    return obj;
}
