#ifndef _WXSCENEOBJECT_H
#define _WXSCENEOBJECT_H

#include "../wxPrpPlasma.h"

class wxSceneObject : public wxPrpPlasmaObject {
public:
    wxSceneObject(plKey key, plResManager* mgr);
    void AddPropPages(wxNotebook* nb);
};

wxTreeItemId TreeAddSceneObject(wxTreeCtrl* tree, const wxTreeItemId& parent, plResManager* mgr, plKey key);

#endif
