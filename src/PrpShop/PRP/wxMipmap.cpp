#include "wxMipmap.h"
#include <PRP/Surface/plMipmap.h>
#include <Stream/hsRAMStream.h>
#include "../../wxPlasmaStream.h"

#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/dcclient.h>
#include <wx/imagdds.h>

wxMipmap::wxMipmap(plKey key, plResManager* mgr,
                   wxTreeCtrl* tree, const wxTreeItemId& tid)
        : wxPrpPlasmaObject(key, mgr, tree, tid)
{ }

void wxMipmap::AddPropPages(wxNotebook* nb)
{
    wxPrpPlasmaObject::AddPropPages(nb);

    wxPanel* nbpage = new wxPanel(nb);
    plMipmap* obj = plMipmap::Convert(fKey->getObj());

    wxPanel* pnlFlags = new wxPanel(nbpage);
    cbAlphaChannel = new wxCheckBox(pnlFlags, wxID_ANY, wxT("Alpha Channel"));
    cbAlphaChannel->SetValue((obj->getFlags() & plMipmap::kAlphaChannelFlag) != 0);
    cbAlphaBit = new wxCheckBox(pnlFlags, wxID_ANY, wxT("Alpha Bit"));
    cbAlphaBit->SetValue((obj->getFlags() & plMipmap::kAlphaBitFlag) != 0);
    cbBumpEnvMap = new wxCheckBox(pnlFlags, wxID_ANY, wxT("Bump Map"));
    cbBumpEnvMap->SetValue((obj->getFlags() & plMipmap::kBumpEnvMap) != 0);
    cbForce32Bit = new wxCheckBox(pnlFlags, wxID_ANY, wxT("Force 32-bit"));
    cbForce32Bit->SetValue((obj->getFlags() & plMipmap::kForce32Bit) != 0);
    cbDontThrowAwayImage = new wxCheckBox(pnlFlags, wxID_ANY, wxT("Don't Throw Away"));
    cbDontThrowAwayImage->SetValue((obj->getFlags() & plMipmap::kDontThrowAwayImage) != 0);
    cbForceOneMipLevel = new wxCheckBox(pnlFlags, wxID_ANY, wxT("Force 1 Mip Level"));
    cbForceOneMipLevel->SetValue((obj->getFlags() & plMipmap::kForceOneMipLevel) != 0);
    cbNoMaxSize = new wxCheckBox(pnlFlags, wxID_ANY, wxT("No Max Size"));
    cbNoMaxSize->SetValue((obj->getFlags() & plMipmap::kNoMaxSize) != 0);
    cbIntensityMap = new wxCheckBox(pnlFlags, wxID_ANY, wxT("Intensity Map"));
    cbIntensityMap->SetValue((obj->getFlags() & plMipmap::kIntensityMap) != 0);
    cbHalfSize = new wxCheckBox(pnlFlags, wxID_ANY, wxT("Half Size"));
    cbHalfSize->SetValue((obj->getFlags() & plMipmap::kHalfSize) != 0);
    cbUserOwnsBitmap = new wxCheckBox(pnlFlags, wxID_ANY, wxT("User Owns Bitmap"));
    cbUserOwnsBitmap->SetValue((obj->getFlags() & plMipmap::kUserOwnsBitmap) != 0);
    cbForceRewrite = new wxCheckBox(pnlFlags, wxID_ANY, wxT("Force Rewrite"));
    cbForceRewrite->SetValue((obj->getFlags() & plMipmap::kForceRewrite) != 0);
    cbForceNonCompressed = new wxCheckBox(pnlFlags, wxID_ANY, wxT("Force Non-Compressed"));
    cbForceNonCompressed->SetValue((obj->getFlags() & plMipmap::kForceNonCompressed) != 0);
    cbIsTexture = new wxCheckBox(pnlFlags, wxID_ANY, wxT("Is Texture"));
    cbIsTexture->SetValue((obj->getFlags() & plMipmap::kIsTexture) != 0);
    cbIsOffscreen = new wxCheckBox(pnlFlags, wxID_ANY, wxT("Is Offscreen"));
    cbIsOffscreen->SetValue((obj->getFlags() & plMipmap::kIsOffscreen) != 0);
    cbIsProjected = new wxCheckBox(pnlFlags, wxID_ANY, wxT("Is Projected"));
    cbIsProjected->SetValue((obj->getFlags() & plMipmap::kIsProjected) != 0);
    cbIsOrtho = new wxCheckBox(pnlFlags, wxID_ANY, wxT("Is Orthogonal"));
    cbIsOrtho->SetValue((obj->getFlags() & plMipmap::kIsOrtho) != 0);

    wxFlexGridSizer* szrFlags = new wxFlexGridSizer(4, 4, 4, 8);
    szrFlags->Add(cbAlphaChannel);
    szrFlags->Add(cbDontThrowAwayImage);
    szrFlags->Add(cbHalfSize);
    szrFlags->Add(cbIsTexture);
    szrFlags->Add(cbAlphaBit);
    szrFlags->Add(cbForceOneMipLevel);
    szrFlags->Add(cbUserOwnsBitmap);
    szrFlags->Add(cbIsOffscreen);
    szrFlags->Add(cbBumpEnvMap);
    szrFlags->Add(cbNoMaxSize);
    szrFlags->Add(cbForceRewrite);
    szrFlags->Add(cbIsProjected);
    szrFlags->Add(cbForce32Bit);
    szrFlags->Add(cbIntensityMap);
    szrFlags->Add(cbForceNonCompressed);
    szrFlags->Add(cbIsOrtho);
    pnlFlags->SetSizer(szrFlags);

    wxFlexGridSizer* szBitmap = new wxFlexGridSizer(2, 1, 8, 0);
    szBitmap->Add(new wxStaticText(nbpage, wxID_ANY, wxT("Flags:")));
    szBitmap->Add(pnlFlags);

    wxBoxSizer* border = new wxBoxSizer(0);
    border->Add(szBitmap, 0, wxALL, 8);
    nbpage->SetSizerAndFit(border);
    nb->AddPage(nbpage, wxT("Bitmap Flags"));
}

wxWindow* wxMipmap::MakePreviewPane(wxWindow* parent)
{
    plMipmap* obj = plMipmap::Convert(fKey->getObj());
    hsRAMStream RS;
    obj->writeToStream(&RS);
    RS.rewind();
    wxPlasmaInputStream WS(&RS);

    if (obj->getCompressionType() == plBitmap::kJPEGCompression) {
        fImage.LoadFile(WS, obj->isImageJPEG() ? wxBITMAP_TYPE_JPEG
                                               : wxDDSHandler::wxBITMAP_TYPE_DDS);

        // Load Alpha
        RS.rewind();
        obj->writeAlphaToStream(&RS);
        RS.rewind();
        wxPlasmaInputStream WSa(&RS);
        wxImage aImage;
        aImage.LoadFile(WSa, obj->isAlphaJPEG() ? wxBITMAP_TYPE_JPEG
                                                : wxDDSHandler::wxBITMAP_TYPE_DDS);
        aImage.ConvertColourToAlpha(255, 255, 255);
        size_t aSize = aImage.GetWidth() * aImage.GetHeight();
        unsigned char* alpha = (unsigned char*)malloc(aSize);
        memcpy(alpha, aImage.GetAlpha(), aSize);
        if (!fImage.HasAlpha())
            fImage.InitAlpha();
        fImage.SetAlpha(alpha);
        // Don't delete alpha -- wxImage takes ownership of the buffer...
    } else {
        // Automatically supports alpha :)
        fImage.LoadFile(WS, wxDDSHandler::wxBITMAP_TYPE_DDS);
    }

    fImgWindow = new wxScrolledWindow(parent);
    fImgWindow->SetVirtualSize(obj->getWidth(), obj->getHeight());
    fImgWindow->SetScrollRate(1, 1);
    fImgWindow->Connect(wxEVT_PAINT, (wxObjectEventFunction)&wxMipmap::OnPreviewPaint,
                        NULL, this);
    fImgWindow->Refresh();

    return fImgWindow;
}

void wxMipmap::SaveDamage()
{
    wxPrpPlasmaObject::SaveDamage();
    
    // ...
}

void wxMipmap::OnPreviewPaint(wxPaintEvent& evt)
{
    wxPaintDC dc(fImgWindow);
    fImgWindow->DoPrepareDC(dc);
    dc.DrawBitmap(wxBitmap(fImage), 0, 0, false);
}
