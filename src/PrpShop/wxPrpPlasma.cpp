#include "wxPrpPlasma.h"
#include <ResManager/plFactory.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/panel.h>
#include <wx/imaglist.h>

#include "PRP/wxSceneNode.h"
#include "PRP/Object/wxSceneObject.h"
#include "PRP/Object/wxCoordinateInterface.h"
#include "PRP/Surface/wxMipmap.h"

#include "../../rc/PrpImages.xpm"
#include "../../rc/FileImages.xpm"

wxPrpPlasmaObject::wxPrpPlasmaObject(plKey key, plResManager* mgr,
                                     wxTreeCtrl* tree, const wxTreeItemId& tid)
                 : fKey(key), fResMgr(mgr), fTree(tree), fTreeId(tid)
{ }

wxPrpPlasmaObject::~wxPrpPlasmaObject()
{ }

void wxPrpPlasmaObject::AddPropPages(wxNotebook* nb)
{
    wxPanel* page = new wxPanel(nb);
    wxString objName(fKey->getName(), wxConvUTF8);
    wxString objType(fKey->getObj()->ClassName(), wxConvUTF8);
    wxString loadMask = wxString::Format(wxT("%04X"), fKey->getLoadMask().getMask());
    wxString id = wxString::Format(wxT("%d"), fKey->getID());

    wxStaticText* lbl1 = new wxStaticText(page, wxID_ANY, wxT("Name:"));
    wxStaticText* lbl2 = new wxStaticText(page, wxID_ANY, wxT("Type:"));
    wxStaticText* lbl3 = new wxStaticText(page, wxID_ANY, wxT("Load Mask:"));
    txtName = new wxTextCtrl(page, wxID_ANY, objName, wxDefaultPosition, wxSize(200, -1));
    wxStaticText* lblType = new wxStaticText(page, wxID_ANY, objType);
    txtMask = new wxTextCtrl(page, wxID_ANY, loadMask, wxDefaultPosition, wxSize(40, -1));

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
    nb->AddPage(page, wxT("Basic"));
}

wxWindow* wxPrpPlasmaObject::MakePreviewPane(wxWindow* parent)
{
    return NULL;
}

void wxPrpPlasmaObject::SaveDamage()
{
    fKey->setName((const char*)txtName->GetValue().mb_str());
    plLoadMask loadMask;
    unsigned long mask;
    txtMask->GetValue().ToULong(&mask, 16);
    loadMask.setMask(mask);
    fKey->setLoadMask(loadMask);

    fTree->SetItemText(fTreeId, txtName->GetValue());
}

plKey wxPrpPlasmaObject::getKey()
{ return fKey; }

wxTreeItemId wxPrpPlasmaObject::getTreeId()
{ return fTreeId; }

wxTreeItemId TreeAddObject(wxTreeCtrl* tree, const wxTreeItemId& parent,
                           plResManager* mgr, plKey key)
{
    switch (key->getType()) {
    case kSceneNode:
        return TreeAddSceneNode(tree, parent, mgr, key);
    case kSceneObject:
        return TreeAddSceneObject(tree, parent, mgr, key);
    default:
        return tree->AppendItem(parent, wxString(key->getName().cstr(), wxConvUTF8),
                                GetTypeIcon(key->getType()), -1, new PlasmaTreeItem(key));
    }
}

wxPrpPlasmaObject* MakeEditor(plResManager* mgr, plKey key,
                              wxTreeCtrl* tree, const wxTreeItemId& tid)
{
    wxPrpPlasmaObject* obj = NULL;

    switch (key->getType()) {
    case kSceneNode:
        obj = new wxSceneNode(key, mgr, tree, tid);
        break;
    case kSceneObject:
        obj = new wxSceneObject(key, mgr, tree, tid);
        break;
    case kMipmap:
        obj = new wxMipmap(key, mgr, tree, tid);
        break;
    case kCoordinateInterface:
        obj = new wxCoordinateInterface(key, mgr, tree, tid);
        break;
    default:
        obj = new wxPrpPlasmaObject(key, mgr, tree, tid);
        break;
    }
    return obj;
}

int GetTypeIcon(unsigned short type)
{
    switch (type) {
    case kMipmap: return ico_img;
    case kCubicEnvironmap: return ico_img;
    case kSceneNode: return ico_scenenode;
    case kSceneObject: return ico_sceneobj;
    case kDrawableSpans: return ico_spans;
    case kSimulationInterface: return ico_sim;
    case kCoordinateInterface: return ico_coords;
    case kAudioInterface: return ico_sound;
    case kDrawInterface: return ico_spans2;
    case kPythonFileMod: return ico_python;
    case kLayer: return ico_layer;
    case kGMaterial: return ico_material;
    default: return -1;
    }
}

static wxImageList* s_typeImgList = NULL;

void DestroyTypeImgList() {
    if (s_typeImgList != NULL)
        delete s_typeImgList;
}

wxImageList* GetTypeImgList()
{
    if (s_typeImgList == NULL) {
        s_typeImgList = new wxImageList(16, 16);
        s_typeImgList->Add(wxBitmap(XPM_folder));
        s_typeImgList->Add(wxBitmap(XPM_age));
        s_typeImgList->Add(wxBitmap(XPM_page));
        s_typeImgList->Add(wxBitmap(XPM_img));
        s_typeImgList->Add(wxBitmap(XPM_scenenode));
        s_typeImgList->Add(wxBitmap(XPM_sceneobj));
        s_typeImgList->Add(wxBitmap(XPM_spans));
        s_typeImgList->Add(wxBitmap(XPM_sim));
        s_typeImgList->Add(wxBitmap(XPM_coords));
        s_typeImgList->Add(wxBitmap(XPM_sound));
        s_typeImgList->Add(wxBitmap(XPM_spans2));
        s_typeImgList->Add(wxBitmap(XPM_python));
        s_typeImgList->Add(wxBitmap(XPM_layer));
        s_typeImgList->Add(wxBitmap(XPM_material));
        atexit(&DestroyTypeImgList);
    }
    return s_typeImgList;
}

wxTreeItemId TreeFindKey(wxTreeCtrl* tree, wxTreeItemId parent, plKey key)
{
    wxTreeItemIdValue cookie;
    wxTreeItemId tid = tree->GetFirstChild(parent, cookie);
    while (tid.IsOk()) {
        PlasmaTreeItem* data = (PlasmaTreeItem*)tree->GetItemData(tid);
        if ((data != NULL) && (data->getObject() == key))
            return tid;
        tid = tree->GetNextChild(parent, cookie);
    }
    return wxTreeItemId();
}

wxTreeItemId TreeFindFolder(wxTreeCtrl* tree, wxTreeItemId parent, const wxString& name)
{
    wxTreeItemIdValue cookie;
    wxTreeItemId tid = tree->GetFirstChild(parent, cookie);
    while (tid.IsOk()) {
        PlasmaTreeItem* data = (PlasmaTreeItem*)tree->GetItemData(tid);
        if ((data == NULL) && (tree->GetItemText(tid) == name))
            return tid;
        tid = tree->GetNextChild(parent, cookie);
    }
    return wxTreeItemId();
}
