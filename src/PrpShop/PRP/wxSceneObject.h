#ifndef _WXSCENEOBJECT_H
#define _WXSCENEOBJECT_H

#include "wxSynchedObject.h"

class wxSceneObject : public wxSynchedObject {
public:
    wxSceneObject(plKey key, plResManager* mgr, wxTreeCtrl* tree, const wxTreeItemId& tid);
    virtual void AddPropPages(wxNotebook* nb);
    virtual void SaveDamage();

protected:
    void OnButton1(wxCommandEvent& evt);
};

wxTreeItemId TreeAddSceneObject(wxTreeCtrl* tree, const wxTreeItemId& parent, plResManager* mgr, plKey key);

#endif
