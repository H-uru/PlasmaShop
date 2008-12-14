#ifndef _WXPRPPLASMA_H
#define _WXPRPPLASMA_H

#include <wx/treectrl.h>
#include <wx/notebook.h>
#include <ResManager/plResManager.h>
#include "PlasmaTreeItem.h"

enum {
    // Image indices for the PrpShop Object Tree
    ico_folder, ico_age, ico_page, ico_img, ico_scenenode, ico_sceneobj,
    ico_spans, ico_sim, ico_coords, ico_sound, ico_spans2, ico_python,
    ico_layer, ico_material, ico_spotlight, ico_guibutton, ico_guicheckbox,
    ico_guidialog, ico_guiedit, ico_guiedit2, ico_guiprogress, ico_guiradio,
    ico_guitext, ico_guiupdown
};

class wxPrpPlasmaObject : public wxEvtHandler {
protected:
    plKey fKey;
    plResManager* fResMgr;
    wxTreeCtrl* fTree;
    wxTreeItemId fTreeId;

    // Editors
    wxTextCtrl* txtName;
    wxTextCtrl* txtMask;

public:
    wxPrpPlasmaObject(plKey key, plResManager* mgr, wxTreeCtrl* tree,
                      const wxTreeItemId& tid);
    virtual ~wxPrpPlasmaObject();
    virtual void AddPropPages(wxNotebook* nb);
    virtual wxWindow* MakePreviewPane(wxWindow* parent);
    virtual void SaveDamage();
    virtual void Refresh();

    plKey getKey();
    wxTreeItemId getTreeId();
};

wxTreeItemId TreeAddObject(wxTreeCtrl* tree, const wxTreeItemId& parent, plResManager* mgr, plKey key);
wxPrpPlasmaObject* MakeEditor(plResManager* mgr, plKey key,
                              wxTreeCtrl* tree, const wxTreeItemId& tid);

int GetTypeIcon(unsigned short type);
wxImageList* GetTypeImgList();

wxTreeItemId TreeFindKey(wxTreeCtrl* tree, wxTreeItemId parent, plKey key);
wxTreeItemId TreeFindFolder(wxTreeCtrl* tree, wxTreeItemId parent, const wxString& name);

#endif
