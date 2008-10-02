#include "wxSynchedObject.h"
#include <PRP/Object/plSynchedObject.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/checkbox.h>
#include <wx/listbox.h>
#include <wx/stattext.h>

wxSynchedObject::wxSynchedObject(plKey key, plResManager* mgr,
                                 wxTreeCtrl* tree, const wxTreeItemId& tid)
               : wxPrpPlasmaObject(key, mgr, tree, tid)
{ }

void wxSynchedObject::AddPropPages(wxNotebook* nb)
{
    wxPanel* nbpage = new wxPanel(nb);
    plSynchedObject* obj = plSynchedObject::Convert(fKey->getObj());

    wxPanel* pnlFlags = new wxPanel(nbpage, wxID_ANY);
    wxCheckBox* cbDontDirty = new wxCheckBox(pnlFlags, wxID_ANY, wxT("Don't Dirty"));
    cbDontDirty->SetValue((obj->getFlags() & plSynchedObject::kDontDirty) != 0);
    wxCheckBox* cbSendReliably = new wxCheckBox(pnlFlags, wxID_ANY, wxT("Send Reliably"));
    cbSendReliably->SetValue((obj->getFlags() & plSynchedObject::kSendReliably) != 0);
    wxCheckBox* cbHasConstNetGroup = new wxCheckBox(pnlFlags, wxID_ANY, wxT("Has Constant Net Group"));
    cbHasConstNetGroup->SetValue((obj->getFlags() & plSynchedObject::kHasConstantNetGroup) != 0);
    wxCheckBox* cbDontSynchGameMsgs = new wxCheckBox(pnlFlags, wxID_ANY, wxT("Don't Synch Game Messages"));
    cbDontSynchGameMsgs->SetValue((obj->getFlags() & plSynchedObject::kDontSynchGameMessages) != 0);
    wxCheckBox* cbExcludePersistent = new wxCheckBox(pnlFlags, wxID_ANY, wxT("Exclude Persistent State"));
    cbExcludePersistent->SetValue((obj->getFlags() & plSynchedObject::kExcludePersistentState) != 0);
    wxCheckBox* cbExcludeAllPersist = new wxCheckBox(pnlFlags, wxID_ANY, wxT("Exclude ALL Persistent States"));
    cbExcludeAllPersist->SetValue((obj->getFlags() & plSynchedObject::kExcludeAllPersistentState) != 0);
    wxCheckBox* cbHasVolatile = new wxCheckBox(pnlFlags, wxID_ANY, wxT("Has Volatile State"));
    cbHasVolatile->SetValue((obj->getFlags() & plSynchedObject::kHasVolatileState) != 0);
    wxCheckBox* cbAllVolatile = new wxCheckBox(pnlFlags, wxID_ANY, wxT("All States Volatile"));
    cbAllVolatile->SetValue((obj->getFlags() & plSynchedObject::kAllStateIsVolatile) != 0);

    wxGridSizer* szrFlags = new wxGridSizer(4, 2, 4, 10);
    szrFlags->Add(cbDontDirty);
    szrFlags->Add(cbExcludePersistent);
    szrFlags->Add(cbSendReliably);
    szrFlags->Add(cbExcludeAllPersist);
    szrFlags->Add(cbHasConstNetGroup);
    szrFlags->Add(cbHasVolatile);
    szrFlags->Add(cbDontSynchGameMsgs);
    szrFlags->Add(cbAllVolatile);
    pnlFlags->SetSizer(szrFlags);

    wxPanel* pnlStates = new wxPanel(nbpage, wxID_ANY);
    wxStaticText* lbl1 = new wxStaticText(pnlStates, wxID_ANY, wxT("Excludes:"));
    wxStaticText* lbl2 = new wxStaticText(pnlStates, wxID_ANY, wxT("Volatiles:"));
    wxListBox* listExcludes = new wxListBox(pnlStates, wxID_ANY, wxDefaultPosition, wxSize(200, 80));
    wxListBox* listVolatiles = new wxListBox(pnlStates, wxID_ANY, wxDefaultPosition, wxSize(200, 80));
    for (size_t i=0; i<obj->getExcludes().getSize(); i++)
        listExcludes->AppendString(wxString(obj->getExcludes()[i].cstr(), wxConvUTF8));
    for (size_t i=0; i<obj->getVolatiles().getSize(); i++)
        listVolatiles->AppendString(wxString(obj->getVolatiles()[i].cstr(), wxConvUTF8));

    wxFlexGridSizer* szrStates = new wxFlexGridSizer(2, 2, 2, 10);
    szrStates->Add(lbl1);
    szrStates->Add(lbl2);
    szrStates->Add(listExcludes);
    szrStates->Add(listVolatiles);
    pnlStates->SetSizer(szrStates);

    wxFlexGridSizer* split = new wxFlexGridSizer(2, 1, 10, 0);
    split->Add(pnlFlags);
    split->Add(pnlStates);

    wxBoxSizer* border = new wxBoxSizer(0);
    border->Add(split, 0, wxALL, 8);
    nbpage->SetSizerAndFit(border);
    nb->AddPage(nbpage, wxT("Synch State"));
}

void wxSynchedObject::SaveDamage()
{
    wxPrpPlasmaObject::SaveDamage();
    // TODO
}
