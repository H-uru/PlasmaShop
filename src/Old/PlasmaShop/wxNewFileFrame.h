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

#ifndef _WXNEWFILEFRAME_H
#define _WXNEWFILEFRAME_H

#include <wx/wxprec.h>
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <wx/listbook.h>
#include <wx/listctrl.h>
#include <wx/imaglist.h>

class wxNewFileFrame : public wxDialog {
public:
    enum {
        ID_NEW_TEXT, ID_NEW_PY, ID_NEW_AGE, ID_NEW_FNI, ID_NEW_INI,
        ID_NEW_SDL, ID_NEW_SDL2, ID_NEW_LOC, ID_NEW_SUB, ID_NEW_HEX,
        ID_NEW_PFP, ID_NEW_PAK, ID_NEW_P2F, ID_NEW_CURS, ID_NEW_CURSDAT,
        ID_NEW_SUM, ID_NEW_CSV, ID_NEW_FX
    };

protected:
    wxImageList* imlGames;
    wxImageList* imlFiles;
    wxListbook* fPager;
    int fSelection;

public:
    wxNewFileFrame();
    virtual ~wxNewFileFrame();

protected:
    enum {
        ID_PAGE_URU, ID_PAGE_MOUL, ID_PAGE_EOA, ID_PAGE_CT, ID_PAGE_HEX
    };

    DECLARE_EVENT_TABLE()

    void OnOk(wxCommandEvent& evt);
    void OnCancel(wxCommandEvent& evt);
    void OnItemActivated(wxListEvent& evt);
    void OnUruItemSelected(wxListEvent& evt);
    void OnMoulItemSelected(wxListEvent& evt);
    void OnEoaItemSelected(wxListEvent& evt);
    void OnCTItemSelected(wxListEvent& evt);
    void OnHexItemSelected(wxListEvent& evt);
};

#endif
