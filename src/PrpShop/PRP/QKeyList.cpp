#include "QKeyList.h"
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
    fKeys[item] = key;
}

void QKeyList::delItem(QTreeWidgetItem* item)
{
    removeItemWidget(item, 0);
    delete *(fKeys.find(item));
}

void QKeyList::adjustColumns()
{
    resizeColumnToContents(1);
    resizeColumnToContents(0);
}

void QKeyList::activateKeyItem(QTreeWidgetItem* item, int)
{
    PrpShopMain::Instance()->editCreatable(fKeys[item]->getObj());
}
