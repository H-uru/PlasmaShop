#ifndef _PLASMATREEITEM_H
#define _PLASMATREEITEM_H

#include <wx/treectrl.h>
#include <PRP/plPageInfo.h>
#include <PRP/KeyedObject/hsKeyedObject.h>

struct TreeAgeInfo {
    wxString fAgeName;
    bool fHasTextures, fHasBuiltIn;

    TreeAgeInfo(const wxString& name = wxEmptyString);
    TreeAgeInfo(const TreeAgeInfo& init);
};

class PlasmaTreeItem : public wxTreeItemData {
public:
    enum Type { kTypeInvalid, kTypeAge, kTypePage, kTypeObject };

protected:
    Type fType;
    TreeAgeInfo fAge;
    plPageInfo* fPage;
    plKey fObject;

public:
    PlasmaTreeItem();
    PlasmaTreeItem(const TreeAgeInfo& age);
    PlasmaTreeItem(plPageInfo* page);
    PlasmaTreeItem(plKey obj);

    TreeAgeInfo* getAge();
    plPageInfo* getPage();
    plKey getObject();
};

#endif
