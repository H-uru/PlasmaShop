/* This file is part of PlasmaShop.
 *
 * PlasmaShop is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PlasmaShop is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PlasmaShop.  If not, see <http://www.gnu.org/licenses/>.
 */

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
#include <Stream/plEncryptedStream.h>

class wxPlasmaTextCtrl : public wxStyledTextCtrl {
public:
    enum SyntaxMode {
        kSynNone, kSynPlasma, kSynPython, kSynSDL_Uru, kSynSDL_Eoa,
        kSynAgeIni, kSynConsole, kSynXML, kSynHex, kSynFX
    };

    enum EncodingType {
        kUniNone, kUniUTF8, kUniUTF16LE, kUniUTF16BE, kUniUTF32LE, kUniUTF32BE
    };

    wxPlasmaTextCtrl(wxWindow* parent, wxWindowID id=wxID_ANY,
                     const wxPoint& pos = wxDefaultPosition,
                     const wxSize& size = wxDefaultSize, long style = 0,
                     const wxString& name = wxSTCNameStr);
    ~wxPlasmaTextCtrl();

    SyntaxMode GetSyntaxMode();
    plEncryptedStream::EncryptionType GetEncryptionType();
    EncodingType GetEncoding();
    wxString GetFilename();

    void SetSyntaxMode(SyntaxMode mode);
    void SetEncryptionType(plEncryptedStream::EncryptionType enc);
    void SetEncoding(EncodingType uni);
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
    EncodingType fEncoding;

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
