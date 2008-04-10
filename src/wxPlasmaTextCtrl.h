#ifndef _WXPLASMATEXTCTRL_H
#define _WXPLASMATEXTCTRL_H

#include <wx/wxprec.h>
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include <wx/stc/stc.h>

class wxPlasmaTextCtrl : public wxStyledTextCtrl {
public:
    enum SyntaxMode {
        kSynNone, kSynPlasma, kSynPython, kSynSDL_Uru, kSynSDL_Eoa,
        kSynAgeIni, kSynConsole, kSynXML, kSynHex
    };

    wxPlasmaTextCtrl(wxWindow* parent, wxWindowID id=wxID_ANY,
                     const wxPoint& pos = wxDefaultPosition,
                     const wxSize& size = wxDefaultSize, long style = 0,
                     const wxString& name = wxSTCNameStr);
    ~wxPlasmaTextCtrl();

    void SetSyntaxMode(SyntaxMode mode);
    SyntaxMode GetSyntaxMode();

protected:
    enum { kMarginUNUSED, kMarginLineNumbers, kMarginFolders };
    unsigned int fLineNumberWidth;
    SyntaxMode fSyntaxMode;
    wxString fFontName;
    int fFontSize;

protected:
    DECLARE_EVENT_TABLE()

    void OnMarginClick(wxStyledTextEvent& evt);
    void OnStcPainted(wxStyledTextEvent& evt);
    void OnCharAdded(wxStyledTextEvent& evt);
};

#endif
