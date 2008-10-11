#ifndef _WXSCENENODE_H
#define _WXSCENENODE_H

#include "../wxPrpPlasma.h"
#include "../wxPrpCanvas.h"

class wxSceneNode : public wxPrpPlasmaObject {
protected:
    wxPrpCanvas* fPreviewCanvas;

public:
    wxSceneNode(plKey key, plResManager* mgr, wxTreeCtrl* tree, const wxTreeItemId& tid);
    virtual void AddPropPages(wxNotebook* nb);
    virtual wxWindow* MakePreviewPane(wxWindow* parent);
    //virtual void SaveDamage();
};

wxTreeItemId TreeAddSceneNode(wxTreeCtrl* tree, const wxTreeItemId& parent, plResManager* mgr, plKey key);

#endif
