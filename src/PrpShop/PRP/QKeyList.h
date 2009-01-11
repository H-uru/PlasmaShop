#ifndef _QKEYLIST_H
#define _QKEYLIST_H

#include <QTreeWidget>
#include <QListWidget>
#include <PRP/KeyedObject/plKey.h>

class QKeyList : public QTreeWidget {
    Q_OBJECT

protected:
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

class QStringListWidget : public QListWidget {
    Q_OBJECT

protected:
    QStringList fStrings;

public:
    QStringListWidget(QWidget* parent = NULL);
    virtual QSize sizeHint() const;

    void addString(const QString& str);
    void delString(int idx);
    QStringList strings() const;

protected:
    virtual void contextMenuEvent(QContextMenuEvent* evt);
};

#endif
