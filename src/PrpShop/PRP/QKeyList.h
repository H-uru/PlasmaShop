/* This file is part of PlasmaShop.
 *
 * PlasmaShop is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PlasmaShop is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PlasmaShop.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _QKEYLIST_H
#define _QKEYLIST_H

#include <QTreeWidget>
#include <QListWidget>
#include <PRP/KeyedObject/plKey.h>

class QKeyList : public QTreeWidget {
    Q_OBJECT

protected:
    QList<plKey> fKeys;
    plKey fContainer;

public:
    QKeyList(plKey container, QWidget* parent = NULL);

    void addKey(plKey key);
    void delItem(int idx);
    QList<plKey> keys() const { return fKeys; }

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
    QStringList strings() const { return fStrings; }

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
    QList<int> values() const { return fValues; }

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
    QList<double> values() const { return fValues; }

protected:
    virtual void contextMenuEvent(QContextMenuEvent* evt);
};

#endif
