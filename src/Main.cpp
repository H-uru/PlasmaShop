#include <wx/wx.h>
#include <wx/imagdds.h>
#include "wxPlasmaShopFrame.h"

class wxPlasmaShopApp : public wxApp {
public:
    virtual bool OnInit()
    {
        // Add various image format support to wxImage
        wxImage::AddHandler(new wxJPEGHandler());
        wxImage::AddHandler(new wxDDSHandler());
        wxImage::AddHandler(new wxPNGHandler());
        wxImage::AddHandler(new wxXPMHandler());

        // Create and show the main frame
        wxPlasmaShopFrame* frame = new wxPlasmaShopFrame(this);
        frame->Show(true);
        SetTopWindow(frame);
        return true;
    }
};

IMPLEMENT_APP(wxPlasmaShopApp)
