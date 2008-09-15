#ifndef _WXNEWFILEFRAME_H
#define _WXNEWFILEFRAME_H

#include <wx/wxprec.h>
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <wx/listctrl.h>
#include <wx/listbook.h>

class wxNewFileFrame : public wxDialog {
protected:
    enum {
        ID_PAGE_URU, ID_PAGE_MOUL, ID_PAGE_EOA, ID_PAGE_HEX,
        ID_NEW_TEXT, ID_NEW_PY, ID_NEW_AGE, ID_NEW_FNI, ID_NEW_INI,
        ID_NEW_SDL, ID_NEW_SDL2, ID_NEW_LOC, ID_NEW_SUB, ID_NEW_HEX,
        ID_NEW_PFP, ID_NEW_PAK, ID_NEW_P2F, ID_NEW_CURS, ID_NEW_CURSDAT
    };

    wxListbook* fPager;
    wxListCtrl* fUruFiles;
    wxListCtrl* fMOULFiles;
    wxListCtrl* fEoaFiles;
    wxListCtrl* fCTFiles;
    wxListCtrl* fHexFiles;
    int fSelection;

public:
    wxNewFileFrame();
    virtual ~wxNewFileFrame();

    int GetSelection();
};

#endif
