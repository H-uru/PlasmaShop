#ifndef _WXPLASMASHOPFRAME_H
#define _WXPLASMASHOPFRAME_H

#include <wx/wxprec.h>
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

class wxPlasmaShopFrame : public wxFrame {
protected:
    wxApp* fOwner;

public:
    wxPlasmaShopFrame(wxApp* owner);
    virtual ~wxPlasmaShopFrame();

private:
    DECLARE_EVENT_TABLE()
};

#endif
