#ifndef _WXPRPPLASMA_H
#define _WXPRPPLASMA_H

#include <wx/treectrl.h>
#include <wx/notebook.h>
#include <ResManager/plResManager.h>
#include "PlasmaTreeItem.h"

enum {
    // Image indices for the PrpShop Object Tree
    ico_folder, ico_age, ico_page, ico_img, ico_scenenode, ico_sceneobj
};

class wxPrpPlasmaObject : public wxEvtHandler {
protected:
    plKey fKey;
    plResManager* fResMgr;

public:
    wxPrpPlasmaObject(plKey key, plResManager* mgr);
    void AddPropPages(wxNotebook* nb);
};

wxTreeItemId TreeAddObject(wxTreeCtrl* tree, const wxTreeItemId& parent, plResManager* mgr, plKey key);
wxPrpPlasmaObject* AddPropPages(wxNotebook* nb, plResManager* mgr, plKey key);

#endif
