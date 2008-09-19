#include "PlasmaTreeItem.h"

/* TreeAgeInfo */
TreeAgeInfo::TreeAgeInfo(const wxString& name)
           : fAgeName(name), fHasTextures(false), fHasBuiltIn(false)
{ }

TreeAgeInfo::TreeAgeInfo(const TreeAgeInfo& init)
           : fAgeName(init.fAgeName), fHasTextures(init.fHasTextures),
             fHasBuiltIn(init.fHasBuiltIn)
{ }


/* PlasmaTreeItem */
PlasmaTreeItem::PlasmaTreeItem()
              : fType(kTypeInvalid)
{ }

PlasmaTreeItem::PlasmaTreeItem(const TreeAgeInfo& age)
              : fType(kTypeAge), fAge(age)
{ }

PlasmaTreeItem::PlasmaTreeItem(plPageInfo* page)
              : fType(kTypePage), fPage(page)
{ }

PlasmaTreeItem::PlasmaTreeItem(hsKeyedObject* obj)
              : fType(kTypeObject), fObject(obj)
{ }

TreeAgeInfo* PlasmaTreeItem::getAge()
{ return (fType == kTypeAge) ? &fAge : NULL; }

plPageInfo* PlasmaTreeItem::getPage()
{ return (fType == kTypePage) ? fPage : NULL; }

hsKeyedObject* PlasmaTreeItem::getObject()
{ return (fType == kTypeObject) ? fObject : NULL; }
