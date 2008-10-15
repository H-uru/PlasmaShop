#include "wxObjSelectFrame.h"
#include "wxPrpPlasma.h"
#include <wx/sizer.h>

BEGIN_EVENT_TABLE(wxObjSelectFrame, wxDialog)
    EVT_BUTTON(wxID_OK, wxObjSelectFrame::OnOk)
    EVT_BUTTON(wxID_CANCEL, wxObjSelectFrame::OnCancel)
    EVT_LIST_ITEM_ACTIVATED(ID_OBJLIST, wxObjSelectFrame::OnItemActivated)
END_EVENT_TABLE()

wxObjSelectFrame::wxObjSelectFrame(wxWindow* parent, wxWindowID id,
                                   const wxString& title, const wxPoint& pos,
                                   const wxSize& size, long style,
                                   const wxString& name)
                : wxDialog(parent, id, title, pos, size, style, name)
{
    lsObjects = new wxKeyListCtrl(this, ID_OBJLIST, wxDefaultPosition, wxSize(240, 400),
                                  wxKEYLIST_DEFAULT_STYLE | wxLC_SINGLE_SEL);
    wxButton* btnOk = new wxButton(this, wxID_OK, wxT("Select"));
    wxButton* btnCancel = new wxButton(this, wxID_CANCEL, wxT("Cancel"));
    btnOk->SetDefault();

    lsObjects->SetImageList(GetTypeImgList(), wxIMAGE_LIST_SMALL);

    wxFlexGridSizer* szrButtons = new wxFlexGridSizer(1, 3, 0, 8);
    szrButtons->Add(btnOk);
    szrButtons->AddStretchSpacer();
    szrButtons->Add(btnCancel);
    //szrButtons->AddGrowableCol(1);

    wxFlexGridSizer* szrFrame = new wxFlexGridSizer(2, 1, 4, 0);
    szrFrame->Add(lsObjects);
    szrFrame->Add(szrButtons, 0, 0, wxEXPAND);

    wxBoxSizer* border = new wxBoxSizer(0);
    border->Add(szrFrame, 0, wxALL, 4);
    SetSizerAndFit(border);
}

void wxObjSelectFrame::AddObject(plKey key)
{
    fKeys.append(key);
    lsObjects->AddKey(key);
}

void wxObjSelectFrame::OnOk(wxCommandEvent& evt)
{
    int sel = -1;
    for (long i=0; i<lsObjects->GetItemCount(); i++) {
        int state = lsObjects->GetItemState(i, wxLIST_STATE_SELECTED);
        if ((state & wxLIST_STATE_SELECTED) != 0) {
            sel = (int)i;
            break;
        }
    }
    EndModal(sel);
}

void wxObjSelectFrame::OnCancel(wxCommandEvent& evt)
{
    EndModal(-1);
}

void wxObjSelectFrame::OnItemActivated(wxListEvent& evt)
{
    EndModal(evt.GetIndex());
}
