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

#include "wxNewFileFrame.h"
#include "../../rc/Games.xpm"
#include "../../rc/FileImages.xpm"

BEGIN_EVENT_TABLE(wxNewFileFrame, wxDialog)
    EVT_BUTTON(wxID_OK, wxNewFileFrame::OnOk)
    EVT_BUTTON(wxID_CANCEL, wxNewFileFrame::OnCancel)
    EVT_LIST_ITEM_SELECTED(ID_PAGE_URU, wxNewFileFrame::OnUruItemSelected)
    EVT_LIST_ITEM_ACTIVATED(ID_PAGE_URU, wxNewFileFrame::OnItemActivated)
    EVT_LIST_ITEM_SELECTED(ID_PAGE_MOUL, wxNewFileFrame::OnMoulItemSelected)
    EVT_LIST_ITEM_ACTIVATED(ID_PAGE_MOUL, wxNewFileFrame::OnItemActivated)
    EVT_LIST_ITEM_SELECTED(ID_PAGE_EOA, wxNewFileFrame::OnEoaItemSelected)
    EVT_LIST_ITEM_ACTIVATED(ID_PAGE_EOA, wxNewFileFrame::OnItemActivated)
    EVT_LIST_ITEM_SELECTED(ID_PAGE_CT, wxNewFileFrame::OnCTItemSelected)
    EVT_LIST_ITEM_ACTIVATED(ID_PAGE_CT, wxNewFileFrame::OnItemActivated)
    EVT_LIST_ITEM_SELECTED(ID_PAGE_HEX, wxNewFileFrame::OnHexItemSelected)
    EVT_LIST_ITEM_ACTIVATED(ID_PAGE_HEX, wxNewFileFrame::OnItemActivated)
END_EVENT_TABLE()

wxNewFileFrame::wxNewFileFrame()
    : wxDialog(NULL, wxID_ANY, wxT("New File..."), wxDefaultPosition,
               wxSize(400, 280), wxDEFAULT_DIALOG_STYLE)
{
    // Image Lists
    imlGames = new wxImageList(48, 48, true, 4);
    imlGames->Add(wxBitmap(XPM_Uru));
    imlGames->Add(wxBitmap(XPM_Eoa));
    imlGames->Add(wxBitmap(XPM_CT));
    imlGames->Add(wxBitmap(XPM_HexIsle));

    imlFiles = new wxImageList(16, 16, true, 10);
    imlFiles->Add(wxBitmap(XPM_age));
    imlFiles->Add(wxBitmap(XPM_curpak));
    imlFiles->Add(wxBitmap(XPM_font));
    imlFiles->Add(wxBitmap(XPM_fontpak));
    imlFiles->Add(wxBitmap(XPM_init));
    imlFiles->Add(wxBitmap(XPM_pak));
    imlFiles->Add(wxBitmap(XPM_python));
    imlFiles->Add(wxBitmap(XPM_sdl));
    imlFiles->Add(wxBitmap(XPM_sum));


    // Interface
    fPager = new wxListbook(this, wxID_ANY);
    wxListCtrl* lsUruFiles = new wxListCtrl(fPager, ID_PAGE_URU, wxDefaultPosition,
                                            wxDefaultSize, wxLC_LIST | wxLC_SINGLE_SEL);
    wxListCtrl* lsMOULFiles = new wxListCtrl(fPager, ID_PAGE_MOUL, wxDefaultPosition,
                                             wxDefaultSize, wxLC_LIST | wxLC_SINGLE_SEL);
    wxListCtrl* lsEoaFiles = new wxListCtrl(fPager, ID_PAGE_EOA, wxDefaultPosition,
                                            wxDefaultSize, wxLC_LIST | wxLC_SINGLE_SEL);
    wxListCtrl* lsCTFiles = new wxListCtrl(fPager, ID_PAGE_CT, wxDefaultPosition,
                                           wxDefaultSize, wxLC_LIST | wxLC_SINGLE_SEL);
    wxListCtrl* lsHexFiles = new wxListCtrl(fPager, ID_PAGE_HEX, wxDefaultPosition,
                                            wxDefaultSize, wxLC_LIST | wxLC_SINGLE_SEL);

    lsUruFiles->SetImageList(imlFiles, wxIMAGE_LIST_SMALL);
    lsMOULFiles->SetImageList(imlFiles, wxIMAGE_LIST_SMALL);
    lsEoaFiles->SetImageList(imlFiles, wxIMAGE_LIST_SMALL);
    lsCTFiles->SetImageList(imlFiles, wxIMAGE_LIST_SMALL);
    lsHexFiles->SetImageList(imlFiles, wxIMAGE_LIST_SMALL);

    lsUruFiles->InsertItem(0, wxT("Age File"), 0);
    lsUruFiles->InsertItem(1, wxT("Fni File"), 4);
    lsUruFiles->InsertItem(2, wxT("Font File"), 2);
    lsUruFiles->InsertItem(3, wxT("Manifest (SUM) File"), 8);
    lsUruFiles->InsertItem(4, wxT("Python File"), 6);
    lsUruFiles->InsertItem(5, wxT("Python Package"), 5);
    lsUruFiles->InsertItem(6, wxT("SDL File"), 7);
    lsUruFiles->InsertItem(7, wxT("VisRegion (CSV) File"), 4);

    lsMOULFiles->InsertItem(0, wxT("Age File"), 0);
    lsMOULFiles->InsertItem(1, wxT("Fni File"), 4);
    lsMOULFiles->InsertItem(2, wxT("Font File"), 2);
    lsMOULFiles->InsertItem(3, wxT("Localization File"), 4);
    lsMOULFiles->InsertItem(4, wxT("Python File"), 6);
    lsMOULFiles->InsertItem(5, wxT("Python Package"), 5);
    lsMOULFiles->InsertItem(6, wxT("SDL File"), 7);
    lsMOULFiles->InsertItem(7, wxT("VisRegion (CSV) File"), 4);

    lsEoaFiles->InsertItem(0, wxT("Age File"), 0);
    lsEoaFiles->InsertItem(1, wxT("Cursor File"), -1);
    lsEoaFiles->InsertItem(2, wxT("Cursor Package"), 1);
    lsEoaFiles->InsertItem(3, wxT("Font File"), 2);
    lsEoaFiles->InsertItem(4, wxT("Font Package"), 3);
    lsEoaFiles->InsertItem(5, wxT("Fni File"), 4);
    lsEoaFiles->InsertItem(6, wxT("Localization File"), 4);
    lsEoaFiles->InsertItem(7, wxT("Manifest (SUM) File"), 8);
    lsEoaFiles->InsertItem(8, wxT("Python File"), 6);
    lsEoaFiles->InsertItem(9, wxT("Python Package"), 5);
    lsEoaFiles->InsertItem(10, wxT("SDL File"), 7);
    lsEoaFiles->InsertItem(11, wxT("Subtitle File"), 4);
    lsEoaFiles->InsertItem(12, wxT("VisRegion (CSV) File"), 4);

    lsCTFiles->InsertItem(0, wxT("Age File"), 0);
    lsCTFiles->InsertItem(1, wxT("Cursor File"), -1);
    lsCTFiles->InsertItem(2, wxT("Cursor Package"), 1);
    lsCTFiles->InsertItem(3, wxT("Font File"), 2);
    lsCTFiles->InsertItem(4, wxT("Font Package"), 3);
    lsCTFiles->InsertItem(5, wxT("Fni File"), 4);
    lsCTFiles->InsertItem(6, wxT("Manifest (SUM) File"), 8);
    lsCTFiles->InsertItem(7, wxT("Python File"), 6);
    lsCTFiles->InsertItem(8, wxT("Python Package"), 5);
    lsCTFiles->InsertItem(9, wxT("SDL File"), 7);

    lsHexFiles->InsertItem(0, wxT("Age File"), 0);
    lsHexFiles->InsertItem(1, wxT("Cursor File"), -1);
    lsHexFiles->InsertItem(2, wxT("Cursor Package"), 1);
    lsHexFiles->InsertItem(3, wxT("Font File"), 2);
    lsHexFiles->InsertItem(4, wxT("Font Package"), 3);
    lsHexFiles->InsertItem(5, wxT("Fni File"), 4);
    lsHexFiles->InsertItem(6, wxT("Level File"), -1);
    lsHexFiles->InsertItem(7, wxT("Localization File"), 4);
    lsHexFiles->InsertItem(8, wxT("Manifest (SUM) File"), 8);
    lsHexFiles->InsertItem(9, wxT("Python File"), 6);
    lsHexFiles->InsertItem(10, wxT("Python Package"), 5);
    lsHexFiles->InsertItem(11, wxT("SDL File"), 7);
    lsHexFiles->InsertItem(12, wxT("Shader File"), 4);
    lsHexFiles->InsertItem(13, wxT("Subtitle File"), 4);

    fPager->SetImageList(imlGames);
    fPager->InsertPage(0, lsUruFiles, wxT("Uru Files"), false, 0);
    fPager->InsertPage(1, lsMOULFiles, wxT("MOUL Files"), false, 0);
    fPager->InsertPage(2, lsEoaFiles, wxT("Myst V Files"), false, 1);
    fPager->InsertPage(3, lsCTFiles, wxT("Crowthistle Files"), false, 2);
    fPager->InsertPage(4, lsHexFiles, wxT("Hex Isle Files"), false, 3);

    wxButton* btnOk = new wxButton(this, wxID_OK, wxT("Create"));
    wxButton* btnCancel = new wxButton(this, wxID_CANCEL, wxT("Cancel"));
    btnOk->SetDefault();

    wxFlexGridSizer* szrButtons = new wxFlexGridSizer(1, 3, 0, 4);
    szrButtons->AddStretchSpacer();
    szrButtons->Add(btnCancel);
    szrButtons->Add(btnOk);
    szrButtons->AddGrowableCol(0);

    wxFlexGridSizer* szrFrame = new wxFlexGridSizer(2, 1, 2, 0);
    szrFrame->Add(fPager, 0, wxEXPAND);
    szrFrame->Add(szrButtons, 0, wxEXPAND);
    szrFrame->AddGrowableCol(0);
    szrFrame->AddGrowableRow(0);
    SetSizer(szrFrame);
}

wxNewFileFrame::~wxNewFileFrame()
{
    delete imlGames;
    delete imlFiles;
}

void wxNewFileFrame::OnOk(wxCommandEvent& evt)
{
    EndModal(fSelection);
}

void wxNewFileFrame::OnCancel(wxCommandEvent& evt)
{
    EndModal(-1);
}

void wxNewFileFrame::OnItemActivated(wxListEvent& evt)
{
    EndModal(fSelection);
}

void wxNewFileFrame::OnUruItemSelected(wxListEvent& evt)
{
    switch (evt.GetIndex()) {
    case 0:
        fSelection = ID_NEW_AGE;
        break;
    case 1:
        fSelection = ID_NEW_FNI;
        break;
    case 2:
        fSelection = ID_NEW_P2F;
        break;
    case 3:
        fSelection = ID_NEW_SUM;
        break;
    case 4:
        fSelection = ID_NEW_PY;
        break;
    case 5:
        fSelection = ID_NEW_PAK;
        break;
    case 6:
        fSelection = ID_NEW_SDL;
        break;
    case 7:
        fSelection = ID_NEW_CSV;
        break;
    default:
        fSelection = -1;
    }
}

void wxNewFileFrame::OnMoulItemSelected(wxListEvent& evt)
{
    switch (evt.GetIndex()) {
    case 0:
        fSelection = ID_NEW_AGE;
        break;
    case 1:
        fSelection = ID_NEW_FNI;
        break;
    case 2:
        fSelection = ID_NEW_P2F;
        break;
    case 3:
        fSelection = ID_NEW_LOC;
        break;
    case 4:
        fSelection = ID_NEW_PY;
        break;
    case 5:
        fSelection = ID_NEW_PAK;
        break;
    case 6:
        fSelection = ID_NEW_SDL;
        break;
    case 7:
        fSelection = ID_NEW_SUM;
        break;
    default:
        fSelection = -1;
    }
}

void wxNewFileFrame::OnEoaItemSelected(wxListEvent& evt)
{
    switch (evt.GetIndex()) {
    case 0:
        fSelection = ID_NEW_AGE;
        break;
    case 1:
        fSelection = ID_NEW_CURS;
        break;
    case 2:
        fSelection = ID_NEW_CURSDAT;
        break;
    case 3:
        fSelection = ID_NEW_P2F;
        break;
    case 4:
        fSelection = ID_NEW_PFP;
        break;
    case 5:
        fSelection = ID_NEW_FNI;
        break;
    case 6:
        fSelection = ID_NEW_LOC;
        break;
    case 7:
        fSelection = ID_NEW_SUM;
        break;
    case 8:
        fSelection = ID_NEW_PY;
        break;
    case 9:
        fSelection = ID_NEW_PAK;
        break;
    case 10:
        fSelection = ID_NEW_SDL2;
        break;
    case 11:
        fSelection = ID_NEW_SUB;
        break;
    case 12:
        fSelection = ID_NEW_CSV;
        break;
    default:
        fSelection = -1;
    }
}

void wxNewFileFrame::OnCTItemSelected(wxListEvent& evt)
{
    switch (evt.GetIndex()) {
    case 0:
        fSelection = ID_NEW_AGE;
        break;
    case 1:
        fSelection = ID_NEW_CURS;
        break;
    case 2:
        fSelection = ID_NEW_CURSDAT;
        break;
    case 3:
        fSelection = ID_NEW_P2F;
        break;
    case 4:
        fSelection = ID_NEW_PFP;
        break;
    case 5:
        fSelection = ID_NEW_FNI;
        break;
    case 6:
        fSelection = ID_NEW_SUM;
        break;
    case 7:
        fSelection = ID_NEW_PY;
        break;
    case 8:
        fSelection = ID_NEW_PAK;
        break;
    case 9:
        fSelection = ID_NEW_SDL2;
        break;
    default:
        fSelection = -1;
    }
}

void wxNewFileFrame::OnHexItemSelected(wxListEvent& evt)
{
    switch (evt.GetIndex()) {
    case 0:
        fSelection = ID_NEW_AGE;
        break;
    case 1:
        fSelection = ID_NEW_CURS;
        break;
    case 2:
        fSelection = ID_NEW_CURSDAT;
        break;
    case 3:
        fSelection = ID_NEW_P2F;
        break;
    case 4:
        fSelection = ID_NEW_PFP;
        break;
    case 5:
        fSelection = ID_NEW_FNI;
        break;
    case 6:
        fSelection = ID_NEW_HEX;
        break;
    case 7:
        fSelection = ID_NEW_LOC;
        break;
    case 8:
        fSelection = ID_NEW_SUM;
        break;
    case 9:
        fSelection = ID_NEW_PY;
        break;
    case 10:
        fSelection = ID_NEW_PAK;
        break;
    case 11:
        fSelection = ID_NEW_SDL2;
        break;
    case 12:
        fSelection = ID_NEW_FX;
        break;
    case 13:
        fSelection = ID_NEW_SUB;
        break;
    default:
        fSelection = -1;
    }
}
