#ifndef _WXSCENENODE_H
#define _WXSCENENODE_H

#include "../wxPrpPlasma.h"

class wxSceneNode : public wxPrpPlasmaObject {
public:
    wxSceneNode(plKey key, plResManager* mgr, wxTreeCtrl* tree, const wxTreeItemId& tid);
    virtual void AddPropPages(wxNotebook* nb);
    virtual void SaveDamage();
};

wxTreeItemId TreeAddSceneNode(wxTreeCtrl* tree, const wxTreeItemId& parent, plResManager* mgr, plKey key);

#endif
