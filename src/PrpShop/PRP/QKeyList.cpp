#include "QKeyList.h"

#include <QContextMenuEvent>
#include <QMenu>
#include <QInputDialog>
#include "../QPlasmaUtils.h"
#include "../QKeyDialog.h"
#include "../Main.h"

/* QKeyList */
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
        QStringList() << ~key->getName() << pqGetFriendlyClassName(key->getType()));
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
        QFindKeyDialog dlg(this);
        if (fKeys.size() > 0)
            dlg.init(PrpShopMain::ResManager(), fKeys.back()->getLocation(),
                     fKeys.back()->getType());
        else
            dlg.init(PrpShopMain::ResManager());
        if (dlg.exec() == QDialog::Accepted)
            addKey(dlg.selection());
    } else if (sel == delObjItem) {
        delItem(indexOfTopLevelItem(currentItem()));
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
    if (item != NULL)
        delete item;
    fStrings.erase(fStrings.begin() + idx);
}

QStringList QStringListWidget::strings() const
{ return fStrings; }

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