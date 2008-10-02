#ifndef _WXSYNCHEDOBJECT_H
#define _WXSYNCHEDOBJECT_H

#include "../wxPrpPlasma.h"

class wxSynchedObject : public wxPrpPlasmaObject {
public:
    wxSynchedObject(plKey key, plResManager* mgr, wxTreeCtrl* tree, const wxTreeItemId& tid);
    virtual void AddPropPages(wxNotebook* nb);
    virtual void SaveDamage();
};

#endif
