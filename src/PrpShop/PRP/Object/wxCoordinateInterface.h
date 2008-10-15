#ifndef _WXCOORDINATEINTERFACE_H
#define _WXCOORDINATEINTERFACE_H

#include "wxSynchedObject.h"
#include "../wxPrpGUI.h"
#include "../../wxPrpCanvas.h"

class wxCoordinateInterface : public wxSynchedObject {
protected:
    wxCheckBox* cbDisable;
    wxCheckBox* cbCanEverDelayTransform;
    wxCheckBox* cbDelayedTransformEval;
    wxKeyListCtrl* lsChildren;

    wxPrpCanvas* fPreviewCanvas;

public:
    wxCoordinateInterface(plKey key, plResManager* mgr, wxTreeCtrl* tree, const wxTreeItemId& tid);
    virtual void AddPropPages(wxNotebook* nb);
    virtual wxWindow* MakePreviewPane(wxWindow* parent);
    virtual void SaveDamage();

private:
    enum { ID_OBJ_ADD, ID_OBJ_DELETE };

    void OnChildrenMenu(wxCommandEvent& evt);
    void OnChildrenActivate(wxListEvent& evt);
    void OnAddChildClick(wxMenuEvent& evt);
    void OnDelChildClick(wxMenuEvent& evt);
};

wxTreeItemId TreeAddCoordInterface(wxTreeCtrl* tree, const wxTreeItemId& parent, plResManager* mgr, plKey key);

#endif
