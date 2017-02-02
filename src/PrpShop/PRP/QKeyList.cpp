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

#include "QKeyList.h"

#include <QContextMenuEvent>
#include <QMenu>
#include <QInputDialog>
#include "QPlasmaUtils.h"
#include "QKeyDialog.h"
#include "Main.h"

/* QKeyList */
QKeyList::QKeyList(plKey container, QWidget* parent)
        : QTreeWidget(parent), fContainer(container)
{
    setColumnCount(2);
    setUniformRowHeights(true);
    setRootIsDecorated(false);
    headerItem()->setText(0, tr("Name"));
    headerItem()->setText(1, tr("Type"));

    connect(this, SIGNAL(itemActivated(QTreeWidgetItem*, int)),
            this, SLOT(activateKeyItem(QTreeWidgetItem*, int)));
}

void QKeyList::addKey(plKey key)
{
    QTreeWidgetItem* item = new QTreeWidgetItem(this,
        QStringList() << st2qstr(key->getName()) << pqGetFriendlyClassName(key->getType()));
    item->setIcon(0, pqGetTypeIcon(key->getType()));
    fKeys << key;
}

void QKeyList::delItem(int idx)
{
    QTreeWidgetItem* item = takeTopLevelItem(idx);
    delete item;
    fKeys.erase(fKeys.begin() + idx);
}

void QKeyList::adjustColumns()
{
    resizeColumnToContents(1);
    resizeColumnToContents(0);
}

void QKeyList::activateKeyItem(QTreeWidgetItem* item, int)
{
    PrpShopMain::Instance()->editCreatable(fKeys[indexOfTopLevelItem(item)]->getObj());
}

void QKeyList::contextMenuEvent(QContextMenuEvent* evt)
{
    QMenu menu(this);
    QAction* addObjItem = menu.addAction(tr("Add Object"));
    QAction* delObjItem = menu.addAction(tr("Remove Object"));

    if (currentItem() == NULL)
        delObjItem->setEnabled(false);

    QAction* sel = menu.exec(evt->globalPos());
    if (sel == addObjItem) {
        QFindKeyDialog dlg(this);
        if (fKeys.size() > 0)
            dlg.init(PrpShopMain::ResManager(), fKeys.back()->getLocation(),
                     fKeys.back()->getType());
        else
            dlg.init(PrpShopMain::ResManager(), fContainer->getLocation());
        if (dlg.exec() == QDialog::Accepted) {
            addKey(dlg.selection());
            emit itemAdded(dlg.selection());
        }
    } else if (sel == delObjItem) {
        int idx = indexOfTopLevelItem(currentItem());
        delItem(idx);
        emit itemRemoved(idx);
    }
}


/* QStringListWidget */
QStringListWidget::QStringListWidget(QWidget* parent)
                 : QListWidget(parent)
{ }

QSize QStringListWidget::sizeHint() const
{
    QSize listSize = QListWidget::sizeHint();
    return QSize((listSize.width() * 2) / 3, (listSize.height() * 1) / 2);
}

void QStringListWidget::addString(const QString& str)
{
    fStrings << str;
    addItem(str);
}

void QStringListWidget::delString(int idx)
{
    QListWidgetItem* item = takeItem(idx);
    delete item;
    fStrings.erase(fStrings.begin() + idx);
}

void QStringListWidget::contextMenuEvent(QContextMenuEvent* evt)
{
    QMenu menu(this);
    QAction* addObjItem = menu.addAction(tr("Add Item"));
    QAction* delObjItem = menu.addAction(tr("Remove Item"));

    if (currentItem() == NULL)
        delObjItem->setEnabled(false);

    QAction* sel = menu.exec(evt->globalPos());
    if (sel == addObjItem) {
        bool ok;
        QString str = QInputDialog::getText(this, tr("Add Item"),
                          tr("Enter a string"), QLineEdit::Normal,
                          QString(), &ok);
        if (ok) addString(str);
    } else if (sel == delObjItem) {
        delString(currentRow());
    }
}


/* QIntListWidget */
QIntListWidget::QIntListWidget(int min, int max, QWidget* parent)
              : QListWidget(parent), fMin(min), fMax(max)
{ }

QSize QIntListWidget::sizeHint() const
{
    QSize listSize = QListWidget::sizeHint();
    return QSize((listSize.width() * 2) / 3, (listSize.height() * 1) / 2);
}

void QIntListWidget::addValue(int value)
{
    fValues << value;
    addItem(QString("%1").arg(value));
}

void QIntListWidget::delValue(int idx)
{
    QListWidgetItem* item = takeItem(idx);
    delete item;
    fValues.erase(fValues.begin() + idx);
}

void QIntListWidget::contextMenuEvent(QContextMenuEvent* evt)
{
    QMenu menu(this);
    QAction* addObjItem = menu.addAction(tr("Add Item"));
    QAction* delObjItem = menu.addAction(tr("Remove Item"));

    if (currentItem() == NULL)
        delObjItem->setEnabled(false);

    QAction* sel = menu.exec(evt->globalPos());
    if (sel == addObjItem) {
        bool ok;
        double val = QInputDialog::getInt(this, tr("Add Item"),
                            tr("Enter a value"), 0, fMin, fMax,
                            1, &ok);
        if (ok) addValue(val);
    } else if (sel == delObjItem) {
        delValue(currentRow());
    }
}


/* QDoubleListWidget */
QDoubleListWidget::QDoubleListWidget(QWidget* parent)
                 : QListWidget(parent)
{ }

QSize QDoubleListWidget::sizeHint() const
{
    QSize listSize = QListWidget::sizeHint();
    return QSize((listSize.width() * 2) / 3, (listSize.height() * 1) / 2);
}

void QDoubleListWidget::addValue(double value)
{
    fValues << value;
    addItem(QString("%1").arg(value));
}

void QDoubleListWidget::delValue(int idx)
{
    QListWidgetItem* item = takeItem(idx);
    delete item;
    fValues.erase(fValues.begin() + idx);
}

void QDoubleListWidget::contextMenuEvent(QContextMenuEvent* evt)
{
    QMenu menu(this);
    QAction* addObjItem = menu.addAction(tr("Add Item"));
    QAction* delObjItem = menu.addAction(tr("Remove Item"));

    if (currentItem() == NULL)
        delObjItem->setEnabled(false);

    QAction* sel = menu.exec(evt->globalPos());
    if (sel == addObjItem) {
        bool ok;
        double val = QInputDialog::getDouble(this, tr("Add Item"),
                            tr("Enter a value"), 0.0, 0.0, 2147483647.0,
                            3, &ok);
        if (ok) addValue(val);
    } else if (sel == delObjItem) {
        delValue(currentRow());
    }
}
