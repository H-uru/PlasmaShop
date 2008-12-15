#ifndef _QKEYLIST_H
#define _QKEYLIST_H

#include <QTreeWidget>
#include <PRP/KeyedObject/plKey.h>

class QKeyList : public QTreeWidget {
protected:
    Q_OBJECT
    QList<plKey> fKeys;

public:
    QKeyList(QWidget* parent = NULL);

    void addKey(plKey key);
    void delItem(int idx);
    QList<plKey> keys() const;

    void adjustColumns();

protected slots:
    void activateKeyItem(QTreeWidgetItem* item, int column);

protected:
    virtual void contextMenuEvent(QContextMenuEvent* evt);
};

#endif
