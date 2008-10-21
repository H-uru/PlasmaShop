#ifndef _WXPLASMASHOPFRAME_H
#define _WXPLASMASHOPFRAME_H

#include <wx/wxprec.h>
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <wx/aui/aui.h>
#include <wx/treectrl.h>

#include "../wxPlasmaTextCtrl.h"

class wxPlasmaShopFrame : public wxFrame {
protected:
    // UI Components
    wxApp* fOwner;
    wxAuiManager* fAuiMgr;
    wxTreeCtrl* fFileTree;
    wxAuiNotebook* fEditorBook;

public:
    wxPlasmaShopFrame(wxApp* owner);
    virtual ~wxPlasmaShopFrame();

    void LoadFile(const wxString& filename);

protected:
    // Event Handling
    DECLARE_EVENT_TABLE()

    enum {
        // Menus for the text editor
        ID_TEXT_FIND, ID_TEXT_REPLACE,
        ID_TEXT_HL_NONE, ID_TEXT_HL_PYTHON, ID_TEXT_HL_SDL1, ID_TEXT_HL_SDL2,
        ID_TEXT_HL_INI, ID_TEXT_HL_CONSOLE, ID_TEXT_HL_XML, ID_TEXT_HL_FX,
        ID_TEXT_HL_HEX,
        ID_TEXT_ENC_NONE, ID_TEXT_ENC_XTEA, ID_TEXT_ENC_DROID, ID_TEXT_ENC_AES,
        ID_TEXT_U_ANSI, ID_TEXT_U_UTF8, ID_TEXT_U_UTF16LE, ID_TEXT_U_UTF16BE,
        ID_TEXT_U_UTF32LE, ID_TEXT_U_UTF32BE,
        ID_TEXT_EXPAND, ID_TEXT_COLLAPSE
    };

    wxMenu* fEditorMenu;
    void UpdateMenus();

    void OnExitClick(wxCommandEvent& evt);
    void OnNewClick(wxCommandEvent& evt);
    void OnOpenClick(wxCommandEvent& evt);
    void OnSaveClick(wxCommandEvent& evt);
    void OnSaveAsClick(wxCommandEvent& evt);
    void OnClose(wxCloseEvent& evt);

    void SetPageClean(size_t page);
    void SetPageDirty(size_t page);

    void OnPageClose(wxAuiNotebookEvent& evt);
    void OnPageChange(wxAuiNotebookEvent& evt);
    void OnStcDirty(wxStyledTextEvent& evt);
    void OnStcClean(wxStyledTextEvent& evt);

    void OnTextHLNoneClick(wxCommandEvent& evt);
    void OnTextHLPythonClick(wxCommandEvent& evt);
    void OnTextHLSdl1Click(wxCommandEvent& evt);
    void OnTextHLSdl2Click(wxCommandEvent& evt);
    void OnTextHLIniClick(wxCommandEvent& evt);
    void OnTextHLConsoleClick(wxCommandEvent& evt);
    void OnTextHLXmlClick(wxCommandEvent& evt);
    void OnTextHLFxClick(wxCommandEvent& evt);
    void OnTextHLHexClick(wxCommandEvent& evt);
    void OnTextEncNoneClick(wxCommandEvent& evt);
    void OnTextEncXteaClick(wxCommandEvent& evt);
    void OnTextEncDroidClick(wxCommandEvent& evt);
    void OnTextEncAesClick(wxCommandEvent& evt);
    void OnTextUAnsiClick(wxCommandEvent& evt);
    void OnTextUUtf8Click(wxCommandEvent& evt);
    void OnTextUUtf16LEClick(wxCommandEvent& evt);
    void OnTextUUtf16BEClick(wxCommandEvent& evt);
    void OnTextUUtf32LEClick(wxCommandEvent& evt);
    void OnTextUUtf32BEClick(wxCommandEvent& evt);
};

#endif
