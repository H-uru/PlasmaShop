#include "QPlasmaTreeItem.h"
#include "QPlasmaUtils.h"

QPlasmaTreeItem::QPlasmaTreeItem()
    : QTreeWidgetItem(kTypeNone)
{
    setIcon(0, QIcon(":/img/folder.png"));
}

QPlasmaTreeItem::QPlasmaTreeItem(hsKeyedObject* obj)
    : QTreeWidgetItem(kTypeKO), fObj(obj)
{
    setText(0, ~obj->getKey()->getName());
    setIcon(0, pqGetTypeIcon(obj->getKey()->getType()));
}

QPlasmaTreeItem::QPlasmaTreeItem(const QString& age)
    : QTreeWidgetItem(kTypeAge), fHasBuiltIn(false), fHasTextures(false),
      fAge(age)
{
    setText(0, age);
    setIcon(0, QIcon(":/img/age.png"));
}

QPlasmaTreeItem::QPlasmaTreeItem(plPageInfo* page)
    : QTreeWidgetItem(kTypePage), fPage(page)
{
    setText(0, ~page->getPage());
    setIcon(0, QIcon(":/img/page.png"));
}

QPlasmaTreeItem::QPlasmaTreeItem(QTreeWidget* parent)
    : QTreeWidgetItem(parent, kTypeNone)
{
    setIcon(0, QIcon(":/img/folder.png"));
}

QPlasmaTreeItem::QPlasmaTreeItem(QTreeWidget* parent, hsKeyedObject* obj)
    : QTreeWidgetItem(parent, kTypeKO), fObj(obj)
{
    setText(0, ~obj->getKey()->getName());
    setIcon(0, pqGetTypeIcon(obj->getKey()->getType()));
}

QPlasmaTreeItem::QPlasmaTreeItem(QTreeWidget* parent, const QString& age)
    : QTreeWidgetItem(parent, kTypeAge), fHasBuiltIn(false),
      fHasTextures(false), fAge(age)
{
    setText(0, age);
    setIcon(0, QIcon(":/img/age.png"));
}

QPlasmaTreeItem::QPlasmaTreeItem(QTreeWidget* parent, plPageInfo* page)
    : QTreeWidgetItem(parent, kTypePage), fPage(page)
{
    setText(0, ~page->getPage());
    setIcon(0, QIcon(":/img/page.png"));
}

QPlasmaTreeItem::QPlasmaTreeItem(QTreeWidgetItem* parent)
    : QTreeWidgetItem(parent, kTypeNone)
{
    setIcon(0, QIcon(":/img/folder.png"));
}

QPlasmaTreeItem::QPlasmaTreeItem(QTreeWidgetItem* parent, hsKeyedObject* obj)
    : QTreeWidgetItem(parent, kTypeKO), fObj(obj)
{
    setText(0, ~obj->getKey()->getName());
    setIcon(0, pqGetTypeIcon(obj->getKey()->getType()));
}

QPlasmaTreeItem::QPlasmaTreeItem(QTreeWidgetItem* parent, const QString& age)
    : QTreeWidgetItem(parent, kTypeAge), fHasBuiltIn(false),
      fHasTextures(false), fAge(age)
{
    setText(0, age);
    setIcon(0, QIcon(":/img/age.png"));
}

QPlasmaTreeItem::QPlasmaTreeItem(QTreeWidgetItem* parent, plPageInfo* page)
    : QTreeWidgetItem(parent, kTypePage), fPage(page)
{
    setText(0, ~page->getPage());
    setIcon(0, QIcon(":/img/page.png"));
}

hsKeyedObject* QPlasmaTreeItem::obj() const
{ return (type() == kTypeKO) ? fObj : NULL; }

QString QPlasmaTreeItem::age() const
{ return (type() == kTypeAge) ? fAge : QString(); }

plPageInfo* QPlasmaTreeItem::page() const
{ return (type() == kTypePage) ? fPage : NULL; }

bool QPlasmaTreeItem::hasBuiltIn() const
{ return (type() == kTypeAge) ? fHasBuiltIn : false; }

bool QPlasmaTreeItem::hasTextures() const
{ return (type() == kTypeAge) ? fHasTextures : false; }

void QPlasmaTreeItem::setHasBuiltIn(bool has)
{ if (type() == kTypeAge) fHasBuiltIn = has; }

void QPlasmaTreeItem::setHasTextures(bool has)
{ if (type() == kTypeAge) fHasTextures = has; }

QString QPlasmaTreeItem::filename() const
{ return fFilename; }

void QPlasmaTreeItem::setFilename(const QString& filename)
{ fFilename = filename; }
