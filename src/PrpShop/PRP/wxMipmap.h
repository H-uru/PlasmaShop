#ifndef _WXMIPMAP_H
#define _WXMIPMAP_H

#include "../wxPrpPlasma.h"
#include <wx/checkbox.h>
#include <wx/scrolwin.h>
#include <wx/image.h>

class wxMipmap : public wxPrpPlasmaObject {
protected:
    wxCheckBox* cbAlphaChannel;
    wxCheckBox* cbAlphaBit;
    wxCheckBox* cbBumpEnvMap;
    wxCheckBox* cbForce32Bit;
    wxCheckBox* cbDontThrowAwayImage;
    wxCheckBox* cbForceOneMipLevel;
    wxCheckBox* cbNoMaxSize;
    wxCheckBox* cbIntensityMap;
    wxCheckBox* cbHalfSize;
    wxCheckBox* cbUserOwnsBitmap;
    wxCheckBox* cbForceRewrite;
    wxCheckBox* cbForceNonCompressed;
    wxCheckBox* cbIsTexture;
    wxCheckBox* cbIsOffscreen;
    wxCheckBox* cbIsProjected;
    wxCheckBox* cbIsOrtho;

    wxImage fImage;
    wxScrolledWindow* fImgWindow;

public:
    wxMipmap(plKey key, plResManager* mgr, wxTreeCtrl* tree, const wxTreeItemId& tid);
    virtual void AddPropPages(wxNotebook* nb);
    virtual wxWindow* MakePreviewPane(wxWindow* parent);
    virtual void SaveDamage();

private:
    void OnPreviewPaint(wxPaintEvent& evt);
};

#endif
