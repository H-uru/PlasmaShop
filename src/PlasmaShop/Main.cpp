#include <wx/wx.h>
#include <wx/config.h>
#include <Debug/plDebug.h>
#include "wxPlasmaShopFrame.h"

class wxPlasmaShopApp : public wxApp {
public:
    virtual bool OnInit()
    {
        // Initialize application globals
        plDebug::InitFile(plDebug::kDLAll);
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
