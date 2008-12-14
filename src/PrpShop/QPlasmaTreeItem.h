#ifndef _QPLASMATREEITEM_H
#define _QPLASMATREEITEM_H

#include <QTreeWidgetItem>
#include <ResManager/plResManager.h>
#include <PRP/KeyedObject/hsKeyedObject.h>

class QPlasmaTreeItem : public QTreeWidgetItem {
private:
    union {
        hsKeyedObject* fObj;
        plPageInfo* fPage;
        struct { bool fHasBuiltIn, fHasTextures; };
    };
    QString fAge;
    QString fFilename;

public:
    enum ItemType {
        kTypeNone = QTreeWidgetItem::UserType, kTypeAge, kTypePage, kTypeKO,
        kMaxPlasmaTypes
    };

    QPlasmaTreeItem();
    QPlasmaTreeItem(hsKeyedObject* obj);
    QPlasmaTreeItem(const QString& age);
    QPlasmaTreeItem(plPageInfo* page);
    QPlasmaTreeItem(QTreeWidget* parent);
    QPlasmaTreeItem(QTreeWidget* parent, hsKeyedObject* obj);
    QPlasmaTreeItem(QTreeWidget* parent, const QString& age);
    QPlasmaTreeItem(QTreeWidget* parent, plPageInfo* page);
    QPlasmaTreeItem(QTreeWidgetItem* parent);
    QPlasmaTreeItem(QTreeWidgetItem* parent, hsKeyedObject* obj);
    QPlasmaTreeItem(QTreeWidgetItem* parent, const QString& age);
    QPlasmaTreeItem(QTreeWidgetItem* parent, plPageInfo* page);

    hsKeyedObject* obj() const;
    QString age() const;
    plPageInfo* page() const;

    bool hasBuiltIn() const;
    bool hasTextures() const;
    void setHasBuiltIn(bool has=true);
    void setHasTextures(bool has=true);

    QString filename() const;
    void setFilename(const QString& filename);
};

#endif
