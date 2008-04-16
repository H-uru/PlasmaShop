#include <wx/wx.h>
#include <wx/config.h>
#include <CoreLib/plDebug.h>
#include "wxPlasmaShopFrame.h"

class wxPlasmaShopApp : public wxApp {
public:
    virtual bool OnInit()
    {
        // Set up libPlasma globals
        plDebug::InitFile(plDebug::kDLAll);

        // Configuration manager
        wxConfigBase::Set(new wxConfig(wxT("PlasmaShop")));

        // Create and show the main frame
        wxPlasmaShopFrame* frame = new wxPlasmaShopFrame(this);
        frame->Show(true);
        SetTopWindow(frame);

        // Command-line loading
        for (int i=1; i<argc; i++)
            frame->LoadFile(argv[i]);

        return true;
    }

    /*
    virtual int OnExit()
    {
        return wxApp::OnExit();
    }*/
};

IMPLEMENT_APP(wxPlasmaShopApp)
