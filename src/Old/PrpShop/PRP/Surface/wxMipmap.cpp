#include "wxMipmap.h"
#include <PRP/Surface/plMipmap.h>
#include <Stream/hsRAMStream.h>
#include "../../../wxPlasmaStream.h"

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

    cbAlphaChannel = new wxCheckBox(nbpage, wxID_ANY, wxT("Alpha Channel"));
    cbAlphaChannel->SetValue((obj->getFlags() & plMipmap::kAlphaChannelFlag) != 0);
    cbAlphaBit = new wxCheckBox(nbpage, wxID_ANY, wxT("Alpha Bit"));
    cbAlphaBit->SetValue((obj->getFlags() & plMipmap::kAlphaBitFlag) != 0);
    cbBumpEnvMap = new wxCheckBox(nbpage, wxID_ANY, wxT("Bump Map"));
    cbBumpEnvMap->SetValue((obj->getFlags() & plMipmap::kBumpEnvMap) != 0);
    cbForce32Bit = new wxCheckBox(nbpage, wxID_ANY, wxT("Force 32-bit"));
    cbForce32Bit->SetValue((obj->getFlags() & plMipmap::kForce32Bit) != 0);
    cbDontThrowAwayImage = new wxCheckBox(nbpage, wxID_ANY, wxT("Don't Throw Away"));
    cbDontThrowAwayImage->SetValue((obj->getFlags() & plMipmap::kDontThrowAwayImage) != 0);
    cbForceOneMipLevel = new wxCheckBox(nbpage, wxID_ANY, wxT("Force 1 Mip Level"));
    cbForceOneMipLevel->SetValue((obj->getFlags() & plMipmap::kForceOneMipLevel) != 0);
    cbNoMaxSize = new wxCheckBox(nbpage, wxID_ANY, wxT("No Max Size"));
    cbNoMaxSize->SetValue((obj->getFlags() & plMipmap::kNoMaxSize) != 0);
    cbIntensityMap = new wxCheckBox(nbpage, wxID_ANY, wxT("Intensity Map"));
    cbIntensityMap->SetValue((obj->getFlags() & plMipmap::kIntensityMap) != 0);
    cbHalfSize = new wxCheckBox(nbpage, wxID_ANY, wxT("Half Size"));
    cbHalfSize->SetValue((obj->getFlags() & plMipmap::kHalfSize) != 0);
    cbUserOwnsBitmap = new wxCheckBox(nbpage, wxID_ANY, wxT("User Owns Bitmap"));
    cbUserOwnsBitmap->SetValue((obj->getFlags() & plMipmap::kUserOwnsBitmap) != 0);
    cbForceRewrite = new wxCheckBox(nbpage, wxID_ANY, wxT("Force Rewrite"));
    cbForceRewrite->SetValue((obj->getFlags() & plMipmap::kForceRewrite) != 0);
    cbForceNonCompressed = new wxCheckBox(nbpage, wxID_ANY, wxT("Force Non-Compressed"));
    cbForceNonCompressed->SetValue((obj->getFlags() & plMipmap::kForceNonCompressed) != 0);
    cbIsTexture = new wxCheckBox(nbpage, wxID_ANY, wxT("Is Texture"));
    cbIsTexture->SetValue((obj->getFlags() & plMipmap::kIsTexture) != 0);
    cbIsOffscreen = new wxCheckBox(nbpage, wxID_ANY, wxT("Is Offscreen"));
    cbIsOffscreen->SetValue((obj->getFlags() & plMipmap::kIsOffscreen) != 0);
    cbIsProjected = new wxCheckBox(nbpage, wxID_ANY, wxT("Is Projected"));
    cbIsProjected->SetValue((obj->getFlags() & plMipmap::kIsProjected) != 0);
    cbIsOrtho = new wxCheckBox(nbpage, wxID_ANY, wxT("Is Orthogonal"));
    cbIsOrtho->SetValue((obj->getFlags() & plMipmap::kIsOrtho) != 0);

    wxFlexGridSizer* szrFlags = new wxFlexGridSizer(5, 4, 4, 8);
    szrFlags->Add(new wxStaticText(nbpage, wxID_ANY, wxT("Flags:")));
    szrFlags->Add(0, 0);
    szrFlags->Add(0, 0);
    szrFlags->Add(0, 0);
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

    wxString strDims = wxString::Format(wxT("%d x %d (%d bpp)"), obj->getWidth(),
                                        obj->getHeight(), obj->getBPP());
    wxString spaceType = wxString(plBitmap::kSpaceNames[obj->getSpace()], wxConvUTF8);
    wxString compType = wxString(plBitmap::kCompressionTypeNames[obj->getCompressionType()], wxConvUTF8);
    wxString mipLevels = wxString::Format(wxT("%d"), obj->getNumLevels());
    wxFlexGridSizer* szrInfo = new wxFlexGridSizer(3, 5, 4, 8);
    szrInfo->Add(new wxStaticText(nbpage, wxID_ANY, wxT("Dimensions:")));
    szrInfo->Add(new wxStaticText(nbpage, wxID_ANY, strDims));
    szrInfo->Add(32, 0);
    szrInfo->Add(new wxStaticText(nbpage, wxID_ANY, wxT("Compression:")));
    szrInfo->Add(new wxStaticText(nbpage, wxID_ANY, compType));
    szrInfo->Add(new wxStaticText(nbpage, wxID_ANY, wxT("Space:")));
    szrInfo->Add(new wxStaticText(nbpage, wxID_ANY, spaceType));
    szrInfo->Add(0, 0);
    if (obj->getCompressionType() == plBitmap::kDirectXCompression) {
        wxString dxType = wxString(plBitmap::kCompressedTypeNames[obj->getDXCompression()], wxConvUTF8);
        wxString blkSize = wxString::Format(wxT("%d"), obj->getDXBlockSize());
        szrInfo->Add(new wxStaticText(nbpage, wxID_ANY, wxT("DXT Type:")));
        szrInfo->Add(new wxStaticText(nbpage, wxID_ANY, dxType));
        szrInfo->Add(new wxStaticText(nbpage, wxID_ANY, wxT("Mipmap Levels:")));
        szrInfo->Add(new wxStaticText(nbpage, wxID_ANY, mipLevels));
        szrInfo->Add(0, 0);
        szrInfo->Add(new wxStaticText(nbpage, wxID_ANY, wxT("Block Size:")));
        szrInfo->Add(new wxStaticText(nbpage, wxID_ANY, blkSize));
    } else {
        wxString rawType = wxString(plBitmap::kUncompressedTypeNames[obj->getARGBType()], wxConvUTF8);
        szrInfo->Add(new wxStaticText(nbpage, wxID_ANY, wxT("ARGB Type:")));
        szrInfo->Add(new wxStaticText(nbpage, wxID_ANY, rawType));
        szrInfo->Add(new wxStaticText(nbpage, wxID_ANY, wxT("Mipmap Levels:")));
        szrInfo->Add(new wxStaticText(nbpage, wxID_ANY, mipLevels));
        szrInfo->Add(0, 0);
        szrInfo->Add(0, 0);
        szrInfo->Add(0, 0);
    }

    wxFlexGridSizer* szBitmap = new wxFlexGridSizer(2, 1, 16, 0);
    szBitmap->Add(szrFlags);
    szBitmap->Add(szrInfo);

    wxBoxSizer* border = new wxBoxSizer(0);
    border->Add(szBitmap, 0, wxALL, 8);
    nbpage->SetSizerAndFit(border);
    nb->AddPage(nbpage, wxT("Bitmap Info"));
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
    plMipmap* obj = plMipmap::Convert(fKey->getObj());

    obj->setFlags((cbAlphaChannel->GetValue() ? plBitmap::kAlphaChannelFlag : 0)
                | (cbAlphaBit->GetValue() ? plBitmap::kAlphaBitFlag : 0)
                | (cbBumpEnvMap->GetValue() ? plBitmap::kBumpEnvMap : 0)
                | (cbForce32Bit->GetValue() ? plBitmap::kForce32Bit : 0)
                | (cbDontThrowAwayImage->GetValue() ? plBitmap::kDontThrowAwayImage : 0)
                | (cbForceOneMipLevel->GetValue() ? plBitmap::kForceOneMipLevel : 0)
                | (cbNoMaxSize->GetValue() ? plBitmap::kNoMaxSize : 0)
                | (cbIntensityMap->GetValue() ? plBitmap::kIntensityMap : 0)
                | (cbHalfSize->GetValue() ? plBitmap::kHalfSize : 0)
                | (cbUserOwnsBitmap->GetValue() ? plBitmap::kUserOwnsBitmap : 0)
                | (cbForceRewrite->GetValue() ? plBitmap::kForceRewrite : 0)
                | (cbForceNonCompressed->GetValue() ? plBitmap::kForceNonCompressed : 0)
                | (cbIsTexture->GetValue() ? plBitmap::kIsTexture : 0)
                | (cbIsOffscreen->GetValue() ? plBitmap::kIsOffscreen : 0)
                | (cbIsProjected->GetValue() ? plBitmap::kIsProjected : 0)
                | (cbIsOrtho->GetValue() ? plBitmap::kIsOrtho : 0));
}

void wxMipmap::OnPreviewPaint(wxPaintEvent& evt)
{
    wxPaintDC dc(fImgWindow);
    fImgWindow->DoPrepareDC(dc);
    dc.DrawBitmap(wxBitmap(fImage), 0, 0, false);
}
