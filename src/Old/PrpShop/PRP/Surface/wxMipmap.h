/* This file is part of PlasmaShop.
 *
 * PlasmaShop is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PlasmaShop is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PlasmaShop.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _WXMIPMAP_H
#define _WXMIPMAP_H

#include "../../wxPrpPlasma.h"
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
