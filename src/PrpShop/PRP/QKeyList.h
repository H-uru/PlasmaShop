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

signals:
    void itemAdded(plKey item);
    void itemRemoved(int idx);

protected slots:
    void activateKeyItem(QTreeWidgetItem* item, int column);

protected:
    virtual void contextMenuEvent(QContextMenuEvent* evt);
};

class QStringListWidget : public QListWidget {
    Q_OBJECT

private:
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

class QIntListWidget : public QListWidget {
    Q_OBJECT

private:
    QList<int> fValues;
    int fMin, fMax;

public:
    QIntListWidget(int min, int max, QWidget* parent = NULL);
    virtual QSize sizeHint() const;

    void addValue(int value);
    void delValue(int idx);
    QList<int> values() const;

protected:
    virtual void contextMenuEvent(QContextMenuEvent* evt);
};

class QDoubleListWidget : public QListWidget {
    Q_OBJECT

protected:
    QList<double> fValues;

public:
    QDoubleListWidget(QWidget* parent = NULL);
    virtual QSize sizeHint() const;

    void addValue(double value);
    void delValue(int idx);
    QList<double> values() const;

protected:
    virtual void contextMenuEvent(QContextMenuEvent* evt);
};

#endif
