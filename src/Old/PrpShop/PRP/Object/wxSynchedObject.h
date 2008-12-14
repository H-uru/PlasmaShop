#ifndef _WXSYNCHEDOBJECT_H
#define _WXSYNCHEDOBJECT_H

#include "../../wxPrpPlasma.h"
#include <wx/checkbox.h>
#include <wx/stattext.h>
#include <wx/listbox.h>

class wxSynchedObject : public wxPrpPlasmaObject {
protected:
    wxCheckBox* cbDontDirty;
    wxCheckBox* cbSendReliably;
    wxCheckBox* cbHasConstNetGroup;
    wxCheckBox* cbDontSynchGameMsgs;
    wxCheckBox* cbExcludePersistent;
    wxCheckBox* cbExcludeAllPersist;
    wxCheckBox* cbHasVolatile;
    wxCheckBox* cbAllVolatile;
    wxStaticText* lblExcludes;
    wxStaticText* lblVolatiles;
    wxListBox* listExcludes;
    wxListBox* listVolatiles;

public:
    wxSynchedObject(plKey key, plResManager* mgr, wxTreeCtrl* tree, const wxTreeItemId& tid);
    virtual void AddPropPages(wxNotebook* nb);
    virtual void SaveDamage();

protected:
    enum { ID_MENU_ADD, ID_MENU_DELETE };

    void OnExcludeContextMenu(wxCommandEvent& evt);
    void OnVolatileContextMenu(wxCommandEvent& evt);
    void OnExcludeAddClick(wxCommandEvent& evt);
    void OnExcludeDeleteClick(wxCommandEvent& evt);
    void OnVolatileAddClick(wxCommandEvent& evt);
    void OnVolatileDeleteClick(wxCommandEvent& evt);
    void OnExcludePersistCheck(wxCommandEvent& evt);
    void OnHasVolatileCheck(wxCommandEvent& evt);
};

#endif
