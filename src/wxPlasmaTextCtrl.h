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
#include <CoreLib/plEncryptedStream.h>

class wxPlasmaTextCtrl : public wxStyledTextCtrl {
public:
    enum SyntaxMode {
        kSynNone, kSynPlasma, kSynPython, kSynSDL_Uru, kSynSDL_Eoa,
        kSynAgeIni, kSynConsole, kSynXML, kSynHex, kSynFX
    };

    wxPlasmaTextCtrl(wxWindow* parent, wxWindowID id=wxID_ANY,
                     const wxPoint& pos = wxDefaultPosition,
                     const wxSize& size = wxDefaultSize, long style = 0,
                     const wxString& name = wxSTCNameStr);
    ~wxPlasmaTextCtrl();

    SyntaxMode GetSyntaxMode();
    plEncryptedStream::EncryptionType GetEncryptionType();
    wxString GetFilename();

    void SetSyntaxMode(SyntaxMode mode);
    void SetEncryptionType(plEncryptedStream::EncryptionType enc);
    void SetFilename(const wxString& filename);

    void DoLoad(const wxString& filename);
    void DoSave(const wxString& filename = wxEmptyString);
    void LoadElf(const wxString& filename);
    void SaveElf(const wxString& filename = wxEmptyString);

protected:
    enum { kMarginUNUSED, kMarginLineNumbers, kMarginFolders };
    
    unsigned int fLineNumberWidth;
    SyntaxMode fSyntaxMode;
    wxFont fFont;

    wxString fFileName;
    plEncryptedStream::EncryptionType fEncryptionType;

    void ResetSyntax();
    void UpdateLineNumberWidth();

protected:
    DECLARE_EVENT_TABLE()

    void OnMarginClick(wxStyledTextEvent& evt);
    void OnUpdateUI(wxStyledTextEvent& evt);
    void OnCharAdded(wxStyledTextEvent& evt);
    void OnModified(wxStyledTextEvent& evt);
    void OnZoom(wxStyledTextEvent& evt);
};

#endif
