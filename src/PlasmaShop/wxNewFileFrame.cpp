#include "wxNewFileFrame.h"
#include "../../rc/Games.xpm"
#include "../../rc/FileImages.xpm"

#include <wx/imaglist.h>

wxNewFileFrame::wxNewFileFrame()
    : wxDialog(NULL, wxID_ANY, wxT("New File..."), wxDefaultPosition,
               wxSize(400, 264), wxDEFAULT_DIALOG_STYLE)
{
    // Image Lists
    wxImageList* gameIList = new wxImageList(48, 48, true, 4);
    gameIList->Add(wxBitmap(XPM_Uru));
    gameIList->Add(wxBitmap(XPM_Eoa));
    gameIList->Add(wxBitmap(XPM_CT));
    gameIList->Add(wxBitmap(XPM_HexIsle));

    wxImageList* fileIList = new wxImageList(16, 16, true, 10);
    fileIList->Add(wxBitmap(XPM_age));
    fileIList->Add(wxBitmap(XPM_curpak));
    fileIList->Add(wxBitmap(XPM_font));
    fileIList->Add(wxBitmap(XPM_fontpak));
    fileIList->Add(wxBitmap(XPM_init));
    fileIList->Add(wxBitmap(XPM_pak));
    fileIList->Add(wxBitmap(XPM_python));
    fileIList->Add(wxBitmap(XPM_sdl));
    fileIList->Add(wxBitmap(XPM_sum));


    // Interface
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    fPager = new wxListbook(this, wxID_ANY);

    fUruFiles = new wxListCtrl(fPager, wxID_ANY, wxDefaultPosition,
                               wxDefaultSize, wxLC_LIST | wxLC_SINGLE_SEL);
    fMOULFiles = new wxListCtrl(fPager, wxID_ANY, wxDefaultPosition,
                                wxDefaultSize, wxLC_LIST | wxLC_SINGLE_SEL);
    fEoaFiles = new wxListCtrl(fPager, wxID_ANY, wxDefaultPosition,
                               wxDefaultSize, wxLC_LIST | wxLC_SINGLE_SEL);
    fCTFiles = new wxListCtrl(fPager, wxID_ANY, wxDefaultPosition,
                              wxDefaultSize, wxLC_LIST | wxLC_SINGLE_SEL);
    fHexFiles = new wxListCtrl(fPager, wxID_ANY, wxDefaultPosition,
                               wxDefaultSize, wxLC_LIST | wxLC_SINGLE_SEL);

    fUruFiles->SetImageList(fileIList, wxIMAGE_LIST_SMALL);
    fMOULFiles->SetImageList(fileIList, wxIMAGE_LIST_SMALL);
    fEoaFiles->SetImageList(fileIList, wxIMAGE_LIST_SMALL);
    fCTFiles->SetImageList(fileIList, wxIMAGE_LIST_SMALL);
    fHexFiles->SetImageList(fileIList, wxIMAGE_LIST_SMALL);

    fUruFiles->InsertItem(0, wxT("Age File"), 0);
    fUruFiles->InsertItem(1, wxT("Fni File"), 4);
    fUruFiles->InsertItem(2, wxT("Font File"), 2);
    fUruFiles->InsertItem(3, wxT("Manifest (SUM) File"), 8);
    fUruFiles->InsertItem(4, wxT("Python File"), 6);
    fUruFiles->InsertItem(5, wxT("Python Package"), 5);
    fUruFiles->InsertItem(6, wxT("SDL File"), 7);
    fUruFiles->InsertItem(7, wxT("VisRegion (CSV) File"), 4);

    fMOULFiles->InsertItem(0, wxT("Age File"), 0);
    fMOULFiles->InsertItem(1, wxT("Fni File"), 4);
    fMOULFiles->InsertItem(2, wxT("Font File"), 2);
    fMOULFiles->InsertItem(3, wxT("Localization File"), 4);
    fMOULFiles->InsertItem(4, wxT("Python File"), 6);
    fMOULFiles->InsertItem(5, wxT("Python Package"), 5);
    fMOULFiles->InsertItem(6, wxT("SDL File"), 7);
    fMOULFiles->InsertItem(7, wxT("VisRegion (CSV) File"), 4);

    fEoaFiles->InsertItem(0, wxT("Age File"), 0);
    fEoaFiles->InsertItem(1, wxT("Cursor File"), -1);
    fEoaFiles->InsertItem(2, wxT("Cursor Package"), 1);
    fEoaFiles->InsertItem(3, wxT("Font File"), 2);
    fEoaFiles->InsertItem(4, wxT("Font Package"), 3);
    fEoaFiles->InsertItem(5, wxT("Fni File"), 4);
    fEoaFiles->InsertItem(6, wxT("Localization File"), 4);
    fEoaFiles->InsertItem(7, wxT("Manifest (SUM) File"), 8);
    fEoaFiles->InsertItem(8, wxT("Python File"), 6);
    fEoaFiles->InsertItem(9, wxT("Python Package"), 5);
    fEoaFiles->InsertItem(10, wxT("SDL File"), 7);
    fEoaFiles->InsertItem(11, wxT("Subtitle File"), 4);
    fEoaFiles->InsertItem(12, wxT("VisRegion (CSV) File"), 4);

    fCTFiles->InsertItem(0, wxT("Age File"), 0);
    fCTFiles->InsertItem(1, wxT("Cursor File"), -1);
    fCTFiles->InsertItem(2, wxT("Cursor Package"), 1);
    fCTFiles->InsertItem(3, wxT("Font File"), 2);
    fCTFiles->InsertItem(4, wxT("Font Package"), 3);
    fCTFiles->InsertItem(5, wxT("Fni File"), 4);
    fCTFiles->InsertItem(6, wxT("Manifest (SUM) File"), 8);
    fCTFiles->InsertItem(7, wxT("Python File"), 6);
    fCTFiles->InsertItem(8, wxT("Python Package"), 5);
    fCTFiles->InsertItem(9, wxT("SDL File"), 7);

    fHexFiles->InsertItem(0, wxT("Age File"), 0);
    fHexFiles->InsertItem(1, wxT("Cursor File"), -1);
    fHexFiles->InsertItem(2, wxT("Cursor Package"), 1);
    fHexFiles->InsertItem(3, wxT("Font File"), 2);
    fHexFiles->InsertItem(4, wxT("Font Package"), 3);
    fHexFiles->InsertItem(5, wxT("Fni File"), 4);
    fHexFiles->InsertItem(6, wxT("Level File"), -1);
    fHexFiles->InsertItem(7, wxT("Localization File"), 4);
    fHexFiles->InsertItem(8, wxT("Manifest (SUM) File"), 8);
    fHexFiles->InsertItem(9, wxT("Python File"), 6);
    fHexFiles->InsertItem(10, wxT("Python Package"), 5);
    fHexFiles->InsertItem(11, wxT("SDL File"), 7);
    fHexFiles->InsertItem(12, wxT("Shader File"), 4);
    fHexFiles->InsertItem(13, wxT("Subtitle File"), 4);

    fPager->SetImageList(gameIList);
    fPager->InsertPage(0, fUruFiles, wxT("Uru Files"), false, 0);
    fPager->InsertPage(1, fMOULFiles, wxT("MOUL Files"), false, 0);
    fPager->InsertPage(2, fEoaFiles, wxT("Myst V Files"), false, 1);
    fPager->InsertPage(3, fCTFiles, wxT("Crowthistle Files"), false, 2);
    fPager->InsertPage(4, fHexFiles, wxT("Hex Isle Files"), false, 3);

    sizer->Add(fPager, 1, wxEXPAND | wxALL, 4);
    SetSizer(sizer);
}

wxNewFileFrame::~wxNewFileFrame()
{
    delete fPager->GetImageList();
    delete fUruFiles->GetImageList(wxIMAGE_LIST_SMALL);
}

int wxNewFileFrame::GetSelection()
{
    return fSelection;
}
