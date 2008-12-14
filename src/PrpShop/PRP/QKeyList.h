#ifndef _QKEYLIST_H
#define _QKEYLIST_H

#include <QTreeWidget>
#include <PRP/KeyedObject/plKey.h>

class QKeyList : public QTreeWidget {
protected:
    Q_OBJECT
    QHash<QTreeWidgetItem*, plKey> fKeys;

public:
    QKeyList(QWidget* parent = NULL);
    void addKey(plKey key);
    void delItem(QTreeWidgetItem* item);
    void adjustColumns();

protected slots:
    void activateKeyItem(QTreeWidgetItem* item, int column);
};

#endif
