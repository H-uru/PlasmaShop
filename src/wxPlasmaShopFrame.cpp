#include "wxPlasmaShopFrame.h"

BEGIN_EVENT_TABLE(wxPlasmaShopFrame, wxFrame)
    //
END_EVENT_TABLE()

wxPlasmaShopFrame::wxPlasmaShopFrame(wxApp* owner)
    : wxFrame(NULL, wxID_ANY, wxT("PlasmaShop 3.0")), fOwner(owner)
{
    //
}

wxPlasmaShopFrame::~wxPlasmaShopFrame()
{
    //
}
