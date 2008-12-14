#include "wxSynchedObject.h"
#include <PRP/Object/plSynchedObject.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/menu.h>
#include <wx/textdlg.h>

wxSynchedObject::wxSynchedObject(plKey key, plResManager* mgr,
                                 wxTreeCtrl* tree, const wxTreeItemId& tid)
               : wxPrpPlasmaObject(key, mgr, tree, tid)
{ }

void wxSynchedObject::AddPropPages(wxNotebook* nb)
{
    wxPrpPlasmaObject::AddPropPages(nb);

    wxPanel* nbpage = new wxPanel(nb);
    plSynchedObject* obj = plSynchedObject::Convert(fKey->getObj());

    cbDontDirty = new wxCheckBox(nbpage, wxID_ANY, wxT("Don't Dirty"));
    cbDontDirty->SetValue((obj->getFlags() & plSynchedObject::kDontDirty) != 0);
    cbSendReliably = new wxCheckBox(nbpage, wxID_ANY, wxT("Send Reliably"));
    cbSendReliably->SetValue((obj->getFlags() & plSynchedObject::kSendReliably) != 0);
    cbHasConstNetGroup = new wxCheckBox(nbpage, wxID_ANY, wxT("Has Constant Net Group"));
    cbHasConstNetGroup->SetValue((obj->getFlags() & plSynchedObject::kHasConstantNetGroup) != 0);
    cbDontSynchGameMsgs = new wxCheckBox(nbpage, wxID_ANY, wxT("Don't Synch Game Messages"));
    cbDontSynchGameMsgs->SetValue((obj->getFlags() & plSynchedObject::kDontSynchGameMessages) != 0);
    cbExcludePersistent = new wxCheckBox(nbpage, wxID_ANY, wxT("Exclude Persistent State"));
    cbExcludePersistent->SetValue((obj->getFlags() & plSynchedObject::kExcludePersistentState) != 0);
    cbExcludeAllPersist = new wxCheckBox(nbpage, wxID_ANY, wxT("Exclude ALL Persistent States"));
    cbExcludeAllPersist->SetValue((obj->getFlags() & plSynchedObject::kExcludeAllPersistentState) != 0);
    cbHasVolatile = new wxCheckBox(nbpage, wxID_ANY, wxT("Has Volatile State"));
    cbHasVolatile->SetValue((obj->getFlags() & plSynchedObject::kHasVolatileState) != 0);
    cbAllVolatile = new wxCheckBox(nbpage, wxID_ANY, wxT("All States Volatile"));
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

    lblExcludes = new wxStaticText(nbpage, wxID_ANY, wxT("Excludes:"));
    lblVolatiles = new wxStaticText(nbpage, wxID_ANY, wxT("Volatiles:"));
    listExcludes = new wxListBox(nbpage, wxID_ANY, wxDefaultPosition, wxSize(200, 72));
    listVolatiles = new wxListBox(nbpage, wxID_ANY, wxDefaultPosition, wxSize(200, 72));
    for (size_t i=0; i<obj->getExcludes().getSize(); i++)
        listExcludes->AppendString(wxString(obj->getExcludes()[i].cstr(), wxConvUTF8));
    for (size_t i=0; i<obj->getVolatiles().getSize(); i++)
        listVolatiles->AppendString(wxString(obj->getVolatiles()[i].cstr(), wxConvUTF8));

    wxFlexGridSizer* szrStates = new wxFlexGridSizer(2, 2, 2, 10);
    szrStates->Add(lblExcludes);
    szrStates->Add(lblVolatiles);
    szrStates->Add(listExcludes);
    szrStates->Add(listVolatiles);

    wxFlexGridSizer* split = new wxFlexGridSizer(2, 1, 10, 0);
    split->Add(szrFlags);
    split->Add(szrStates);

    wxBoxSizer* border = new wxBoxSizer(0);
    border->Add(split, 0, wxALL, 8);
    nbpage->SetSizerAndFit(border);
    nb->AddPage(nbpage, wxT("Synch State"));

    // Event handling
    listExcludes->Connect(wxEVT_CONTEXT_MENU,
                          (wxObjectEventFunction)&wxSynchedObject::OnExcludeContextMenu,
                          NULL, this);
    listVolatiles->Connect(wxEVT_CONTEXT_MENU,
                           (wxObjectEventFunction)&wxSynchedObject::OnVolatileContextMenu,
                           NULL, this);
}

void wxSynchedObject::SaveDamage()
{
    wxPrpPlasmaObject::SaveDamage();
    plSynchedObject* obj = plSynchedObject::Convert(fKey->getObj());

    obj->setFlags((cbDontDirty->GetValue() ? plSynchedObject::kDontDirty : 0)
                | (cbSendReliably->GetValue() ? plSynchedObject::kSendReliably : 0)
                | (cbHasConstNetGroup->GetValue() ? plSynchedObject::kHasConstantNetGroup : 0)
                | (cbDontSynchGameMsgs->GetValue() ? plSynchedObject::kDontSynchGameMessages : 0)
                | (cbExcludePersistent->GetValue() ? plSynchedObject::kExcludePersistentState : 0)
                | (cbExcludeAllPersist->GetValue() ? plSynchedObject::kExcludeAllPersistentState : 0)
                | (cbHasVolatile->GetValue() ? plSynchedObject::kHasVolatileState : 0)
                | (cbAllVolatile->GetValue() ? plSynchedObject::kAllStateIsVolatile : 0));
    obj->clearExcludes();
    obj->clearVolatiles();
    for (size_t i=0; i<listExcludes->GetCount(); i++)
        obj->setExclude((const char*)listExcludes->GetString(i).mb_str());
    for (size_t i=0; i<listVolatiles->GetCount(); i++)
        obj->setVolatile((const char*)listVolatiles->GetString(i).mb_str());
}

void wxSynchedObject::OnExcludeContextMenu(wxCommandEvent& evt)
{
    wxMenu menu(wxT(""));
    menu.Append(ID_MENU_ADD, wxT("Add Exclusion..."));
    menu.Append(ID_MENU_DELETE, wxT("Delete"));
    if (listExcludes->GetSelection() < 0)
        menu.Enable(ID_MENU_DELETE, false);
    menu.Connect(ID_MENU_ADD, wxEVT_COMMAND_MENU_SELECTED,
                 (wxObjectEventFunction)&wxSynchedObject::OnExcludeAddClick,
                 NULL, this);
    menu.Connect(ID_MENU_DELETE, wxEVT_COMMAND_MENU_SELECTED,
                 (wxObjectEventFunction)&wxSynchedObject::OnExcludeDeleteClick,
                 NULL, this);
    listExcludes->PopupMenu(&menu);
}

void wxSynchedObject::OnVolatileContextMenu(wxCommandEvent& evt)
{
    wxMenu menu(wxT(""));
    menu.Append(ID_MENU_ADD, wxT("Add Volatile..."));
    menu.Append(ID_MENU_DELETE, wxT("Delete"));
    if (listVolatiles->GetSelection() < 0)
        menu.Enable(ID_MENU_DELETE, false);
    menu.Connect(ID_MENU_ADD, wxEVT_COMMAND_MENU_SELECTED,
                 (wxObjectEventFunction)&wxSynchedObject::OnVolatileAddClick,
                 NULL, this);
    menu.Connect(ID_MENU_DELETE, wxEVT_COMMAND_MENU_SELECTED,
                 (wxObjectEventFunction)&wxSynchedObject::OnVolatileDeleteClick,
                 NULL, this);
    listVolatiles->PopupMenu(&menu);
}

void wxSynchedObject::OnExcludeAddClick(wxCommandEvent& evt)
{
    wxString var = wxGetTextFromUser(wxT("Enter Exclude Variable"), wxT("Add SDL Exclusion"));
    if (var != wxEmptyString)
        listExcludes->AppendString(var);
}

void wxSynchedObject::OnExcludeDeleteClick(wxCommandEvent& evt)
{
    if (listExcludes->GetSelection() >= 0)
        listExcludes->Delete(listExcludes->GetSelection());
}

void wxSynchedObject::OnVolatileAddClick(wxCommandEvent& evt)
{
    wxString var = wxGetTextFromUser(wxT("Enter Volatile Variable"), wxT("Add SDL Volatile"));
    if (var != wxEmptyString)
        listVolatiles->AppendString(var);
}

void wxSynchedObject::OnVolatileDeleteClick(wxCommandEvent& evt)
{
    if (listVolatiles->GetSelection() >= 0)
        listVolatiles->Delete(listVolatiles->GetSelection());
}
