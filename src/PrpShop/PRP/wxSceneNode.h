#ifndef _WXSCENENODE_H
#define _WXSCENENODE_H

#include "../wxPrpPlasma.h"

class wxSceneNode : public wxPrpPlasmaObject {
public:
    wxSceneNode(plKey key, plResManager* mgr);
    virtual void AddPropPages(wxNotebook* nb);
};

wxTreeItemId TreeAddSceneNode(wxTreeCtrl* tree, const wxTreeItemId& parent, plResManager* mgr, plKey key);

#endif
