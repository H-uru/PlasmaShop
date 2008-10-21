#ifndef _WXSCENENODE_H
#define _WXSCENENODE_H

#include "../wxPrpPlasma.h"
#include "../wxPrpCanvas.h"
#include "wxPrpGUI.h"

class wxSceneNode : public wxPrpPlasmaObject {
protected:
    wxKeyListCtrl* lsSceneObjects;
    wxKeyListCtrl* lsPoolObjects;

    wxPrpCanvas* fPreviewCanvas;

public:
    wxSceneNode(plKey key, plResManager* mgr, wxTreeCtrl* tree, const wxTreeItemId& tid);
    virtual void AddPropPages(wxNotebook* nb);
    virtual wxWindow* MakePreviewPane(wxWindow* parent);
    //virtual void SaveDamage();
    virtual void Refresh();

protected:
    enum {
        ID_SCENEOBJ_ADD, ID_SCENEOBJ_DEL,
    };

    void OnSceneObjectMenu(wxCommandEvent& evt);
    void OnPoolObjectMenu(wxCommandEvent& evt);
    void OnSceneObjectActivated(wxListEvent& evt);
    void OnPoolObjectActivated(wxListEvent& evt);

    void OnSceneObjAddClick(wxMenuEvent& evt);
    void OnSceneObjDelClick(wxMenuEvent& evt);
};

wxTreeItemId TreeAddSceneNode(wxTreeCtrl* tree, const wxTreeItemId& parent, plResManager* mgr, plKey key);

#endif
