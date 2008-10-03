#ifndef _WXPRPPLASMA_H
#define _WXPRPPLASMA_H

#include <wx/treectrl.h>
#include <wx/notebook.h>
#include <ResManager/plResManager.h>
#include "PlasmaTreeItem.h"

enum {
    // Image indices for the PrpShop Object Tree
    ico_folder, ico_age, ico_page, ico_img, ico_scenenode, ico_sceneobj,
    ico_spans, ico_sim, ico_coords, ico_sound, ico_spans2
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
    virtual void AddPropPages(wxNotebook* nb);
    void AddKeyPage(wxNotebook* nb);
    virtual void SaveDamage();

    plKey getKey();
    wxTreeItemId getTreeId();
};

wxTreeItemId TreeAddObject(wxTreeCtrl* tree, const wxTreeItemId& parent, plResManager* mgr, plKey key);
wxPrpPlasmaObject* AddPropPages(wxNotebook* nb, plResManager* mgr, plKey key,
                                wxTreeCtrl* tree, const wxTreeItemId& tid);

#endif
