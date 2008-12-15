#include "QKeyList.h"

#include <QContextMenuEvent>
#include <QMenu>
#include "../QPlasmaUtils.h"
#include "../Main.h"

QKeyList::QKeyList(QWidget* parent)
        : QTreeWidget(parent)
{
    setColumnCount(2);
    setUniformRowHeights(true);
    setRootIsDecorated(false);
    headerItem()->setText(0, tr("Name"));
    headerItem()->setText(1, tr("Type"));

    QObject::connect(this, SIGNAL(itemActivated(QTreeWidgetItem*, int)),
                     this, SLOT(activateKeyItem(QTreeWidgetItem*, int)));
}

void QKeyList::addKey(plKey key)
{
    QTreeWidgetItem* item = new QTreeWidgetItem(this,
        QStringList() << key->getName().cstr() << pqGetFriendlyClassName(key->getType()));
    item->setIcon(0, pqGetTypeIcon(key->getType()));
    fKeys << key;
}

void QKeyList::delItem(int idx)
{
    QTreeWidgetItem* item = takeTopLevelItem(idx);
    delete item;
    fKeys.erase(fKeys.begin() + idx);
}

QList<plKey> QKeyList::keys() const
{ return fKeys; }

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
        // ...
    } else if (sel == delObjItem) {
        delItem(indexOfTopLevelItem(currentItem()));
    }
}
