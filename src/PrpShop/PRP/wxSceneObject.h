#ifndef _WXSCENEOBJECT_H
#define _WXSCENEOBJECT_H

#include "../wxPrpPlasma.h"

class wxSceneObject : public wxPrpPlasmaObject {
public:
    wxSceneObject(plKey key, plResManager* mgr);
    virtual void AddPropPages(wxNotebook* nb);

protected:
    void OnButton1(wxCommandEvent& evt);
};

wxTreeItemId TreeAddSceneObject(wxTreeCtrl* tree, const wxTreeItemId& parent, plResManager* mgr, plKey key);

#endif
