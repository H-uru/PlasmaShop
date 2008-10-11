#include <wx/wx.h>
#include <wx/config.h>
#include <wx/imagdds.h>
#include <Debug/plDebug.h>
#include "wxPrpShopFrame.h"

class wxPrpShopApp : public wxApp {
public:
    virtual bool OnInit()
    {
        // Initialize application globals
        plDebug::InitFile(plDebug::kDLAll);
        wxConfigBase::Set(new wxConfig(wxT("PlasmaShop")));

        wxImage::AddHandler(new wxDDSHandler());
        wxImage::AddHandler(new wxJPEGHandler());

        // Create and show the main frame
        wxPrpShopFrame* frame = new wxPrpShopFrame(this);
        frame->Show(true);
        SetTopWindow(frame);

        // Command-line loading
        for (int i=1; i<argc; i++)
            frame->LoadFile(argv[i]);

        return true;
    }
};

IMPLEMENT_APP(wxPrpShopApp)
