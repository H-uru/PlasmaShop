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

#include "QGUIPopUpMenu.h"

#include <QGroupBox>
#include <QGridLayout>
#include <QMenu>
#include "Main.h"
#include "QKeyDialog.h"

/* QPopupMenuItemList */
QPopupMenuItemList::QPopupMenuItemList(QWidget* parent)
    : QTreeWidget(parent)
{
    setColumnCount(2);
    setUniformRowHeights(true);
    setRootIsDecorated(false);
    headerItem()->setText(0, tr("Name"));
    headerItem()->setText(1, tr("SubMenu"));

    connect(this, &QTreeWidget::itemActivated,
            this, &QPopupMenuItemList::handleActivate);
}

QSize QPopupMenuItemList::sizeHint() const
{
    QSize treeSize = QTreeWidget::sizeHint();
    return QSize((treeSize.width() * 4) / 3, treeSize.height());
}

void QPopupMenuItemList::addItem(const pfGUIPopUpMenu::pfMenuItem& item)
{
    new QTreeWidgetItem(this, QStringList() << st2qstr(item.fName)
                              << (item.fSubMenu.Exists() ? tr("Yes") : tr("No")));
}

void QPopupMenuItemList::delItem(int idx)
{
    QTreeWidgetItem* item = takeTopLevelItem(idx);
    delete item;
}

void QPopupMenuItemList::adjustColumns()
{
    resizeColumnToContents(0);
    resizeColumnToContents(1);
}

void QPopupMenuItemList::handleUpdate(int idx, QString name, bool hasSubMenu)
{
    topLevelItem(idx)->setText(0, name);
    topLevelItem(idx)->setText(1, hasSubMenu ? tr("Yes") : tr("No"));
}

void QPopupMenuItemList::handleActivate(QTreeWidgetItem* item, int)
{
    emit requestEditItem(indexOfTopLevelItem(item));
}

void QPopupMenuItemList::contextMenuEvent(QContextMenuEvent* evt)
{
    QMenu menu(this);
    QAction* addObjItem = menu.addAction(tr("Add Item..."));
    QAction* editObjItem = menu.addAction(tr("Edit Item..."));
    QAction* delObjItem = menu.addAction(tr("Remove Item"));

    if (currentItem() == NULL)
        delObjItem->setEnabled(false);

    QAction* sel = menu.exec(evt->globalPos());
    if (sel == addObjItem) {
        emit requestNewItem();
    } else if (sel == editObjItem) {
        emit requestEditItem(indexOfTopLevelItem(currentItem()));
    } else if (sel == delObjItem) {
        emit requestDelItem(indexOfTopLevelItem(currentItem()));
        delItem(indexOfTopLevelItem(currentItem()));
    }
}


/* QGUIPopUpMenu */
QGUIPopUpMenu::QGUIPopUpMenu(plCreatable* pCre, QWidget* parent)
    : QCreatable(pCre, kGUIPopUpMenu, parent)
{
    pfGUIPopUpMenu* ctrl = pfGUIPopUpMenu::Convert(fCreatable);

    fDialogModLink = new QCreatableLink(this, false);
    fDialogModLink->setCreatable(ctrl, tr("GUI Dialog Common Properties"));
    fDialogModLink->setForceType(kGUIDialogMod);

    QGroupBox* grpFlags = new QGroupBox(tr("Flags"), this);
    fModFlags[pfGUIPopUpMenu::kStayOpenAfterClick - kModFlagStart] = new QCheckBox(tr("Stay Open After Click"), grpFlags);
    fModFlags[pfGUIPopUpMenu::kModalOutsideMenus - kModFlagStart] = new QCheckBox(tr("Modal Outside Menus"), grpFlags);
    fModFlags[pfGUIPopUpMenu::kOpenSubMenusOnHover - kModFlagStart] = new QCheckBox(tr("Open Submenus on Hover"), grpFlags);
    fModFlags[pfGUIPopUpMenu::kScaleWithResolution - kModFlagStart] = new QCheckBox(tr("Scale With Resolution"), grpFlags);
    QGridLayout* flagLayout = new QGridLayout(grpFlags);
    flagLayout->setContentsMargins(8, 8, 8, 8);
    flagLayout->setVerticalSpacing(0);
    flagLayout->setHorizontalSpacing(8);
    flagLayout->addWidget(fModFlags[pfGUIPopUpMenu::kStayOpenAfterClick - kModFlagStart], 0, 0);
    flagLayout->addWidget(fModFlags[pfGUIPopUpMenu::kModalOutsideMenus - kModFlagStart], 1, 0);
    flagLayout->addWidget(fModFlags[pfGUIPopUpMenu::kOpenSubMenusOnHover - kModFlagStart], 2, 0);
    flagLayout->addWidget(fModFlags[pfGUIPopUpMenu::kScaleWithResolution - kModFlagStart], 3, 0);

    for (size_t i = 0; i < kModFlagCount; ++i) {
        fModFlags[i]->setChecked(ctrl->getFlag(i + kModFlagStart));
        connect(fModFlags[i], &QCheckBox::clicked, this, [this, i](bool checked) {
            pfGUIPopUpMenu* ctrl = pfGUIPopUpMenu::Convert(fCreatable);
            ctrl->setFlag(i + kModFlagStart, checked);
        });
    }

    fAlignment = new QComboBox(this);
    fAlignment->addItems(QStringList() << tr("Upper Left") << tr("Upper Right")
                                       << tr("Lower Left") << tr("Lower Right"));
    fAlignment->setCurrentIndex(ctrl->getAlignment());

    fMargin = new QIntEdit(this);
    fMargin->setRange(0, 65535);
    fMargin->setValue(ctrl->getMargin());

    fSkin = new QCreatableLink(this);
    fSkin->setKey(ctrl->getSkin());

    fOriginContext = new QCreatableLink(this);
    fOriginContext->setKey(ctrl->getOriginContext());

    fOriginAnchor = new QCreatableLink(this);
    fOriginAnchor->setKey(ctrl->getOriginAnchor());

    fMenuItems = new QPopupMenuItemList(this);
    for (size_t i=0; i<ctrl->getNumItems(); i++)
        fMenuItems->addItem(ctrl->getItem(i));

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(fDialogModLink, 0, 0, 1, 5);
    layout->addWidget(grpFlags, 1, 0, 1, 5);
    layout->addWidget(new QLabel(tr("Alignment:"), this), 2, 0);
    layout->addWidget(fAlignment, 2, 1);
    layout->addWidget(new QLabel(tr("Margin"), this), 2, 3);
    layout->addWidget(fMargin, 2, 4);
    layout->addItem(new QSpacerItem(8, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum), 2, 2);
    layout->addWidget(new QLabel(tr("Skin:"), this), 3, 0);
    layout->addWidget(fSkin, 3, 1, 1, 4);
    layout->addWidget(new QLabel(tr("Origin Context:"), this), 4, 0);
    layout->addWidget(fOriginContext, 4, 1, 1, 4);
    layout->addWidget(new QLabel(tr("Origin Anchor:"), this), 5, 0);
    layout->addWidget(fOriginAnchor, 5, 1, 1, 4);
    layout->addWidget(new QLabel(tr("Menu Items:"), this), 6, 0, 1, 5);
    layout->addWidget(fMenuItems, 7, 0, 1, 5);

    connect(fSkin, &QCreatableLink::addObject, this, &QGUIPopUpMenu::setSkin);
    connect(fSkin, &QCreatableLink::delObject, this, &QGUIPopUpMenu::unsetSkin);
    connect(fOriginContext, &QCreatableLink::addObject, this, &QGUIPopUpMenu::setOriginContext);
    connect(fOriginContext, &QCreatableLink::delObject, this, &QGUIPopUpMenu::unsetOriginContext);
    connect(fOriginAnchor, &QCreatableLink::addObject, this, &QGUIPopUpMenu::setOriginAnchor);
    connect(fOriginAnchor, &QCreatableLink::delObject, this, &QGUIPopUpMenu::unsetOriginAnchor);
    connect(fMenuItems, &QPopupMenuItemList::requestNewItem, this, &QGUIPopUpMenu::addMenuItem);
    connect(fMenuItems, &QPopupMenuItemList::requestEditItem, this, &QGUIPopUpMenu::editMenuItem);
    connect(fMenuItems, &QPopupMenuItemList::requestDelItem, this, &QGUIPopUpMenu::delMenuItem);
}

void QGUIPopUpMenu::saveDamage()
{
    pfGUIPopUpMenu* ctrl = pfGUIPopUpMenu::Convert(fCreatable);

    ctrl->setAlignment((pfGUIPopUpMenu::Alignment)fAlignment->currentIndex());
    ctrl->setMargin(fMargin->value());
}

void QGUIPopUpMenu::setSkin()
{
    pfGUIPopUpMenu* ctrl = pfGUIPopUpMenu::Convert(fCreatable);
    QFindKeyDialog dlg(this);
    if (ctrl->getSkin().Exists())
        dlg.init(PrpShopMain::ResManager(), ctrl->getSkin());
    else
        dlg.init(PrpShopMain::ResManager(), ctrl->getKey()->getLocation(), kGUISkin);
    if (dlg.exec() == QDialog::Accepted) {
        ctrl->setSkin(dlg.selection());
        fSkin->setKey(ctrl->getSkin());
    }
}

void QGUIPopUpMenu::setOriginContext()
{
    pfGUIPopUpMenu* ctrl = pfGUIPopUpMenu::Convert(fCreatable);
    QFindKeyDialog dlg(this);
    if (ctrl->getOriginContext().Exists())
        dlg.init(PrpShopMain::ResManager(), ctrl->getOriginContext());
    else
        dlg.init(PrpShopMain::ResManager(), ctrl->getKey()->getLocation(), kGUIDialogMod);
    if (dlg.exec() == QDialog::Accepted) {
        ctrl->setOriginContext(dlg.selection());
        fOriginContext->setKey(ctrl->getOriginContext());
    }
}

void QGUIPopUpMenu::setOriginAnchor()
{
    pfGUIPopUpMenu* ctrl = pfGUIPopUpMenu::Convert(fCreatable);
    QFindKeyDialog dlg(this);
    if (ctrl->getOriginAnchor().Exists())
        dlg.init(PrpShopMain::ResManager(), ctrl->getOriginAnchor());
    else
        dlg.init(PrpShopMain::ResManager(), ctrl->getKey()->getLocation(), kSceneObject);
    if (dlg.exec() == QDialog::Accepted) {
        ctrl->setOriginAnchor(dlg.selection());
        fOriginAnchor->setKey(ctrl->getOriginAnchor());
    }
}

void QGUIPopUpMenu::unsetSkin()
{
    pfGUIPopUpMenu* ctrl = pfGUIPopUpMenu::Convert(fCreatable);
    ctrl->setSkin(plKey());
    fSkin->setCreatable(NULL, "(None)");
}

void QGUIPopUpMenu::unsetOriginContext()
{
    pfGUIPopUpMenu* ctrl = pfGUIPopUpMenu::Convert(fCreatable);
    ctrl->setOriginContext(plKey());
    fOriginContext->setCreatable(NULL, "(None)");
}

void QGUIPopUpMenu::unsetOriginAnchor()
{
    pfGUIPopUpMenu* ctrl = pfGUIPopUpMenu::Convert(fCreatable);
    ctrl->setOriginAnchor(plKey());
    fOriginAnchor->setCreatable(NULL, "(None)");
}

void QGUIPopUpMenu::addMenuItem()
{
    pfGUIPopUpMenu* ctrl = pfGUIPopUpMenu::Convert(fCreatable);
    ctrl->addItem("(New Item)", NULL, plKey(), 0.0f);
    pfGUIPopUpMenu::pfMenuItem& item = ctrl->getItem(ctrl->getNumItems() - 1);
    fMenuItems->addItem(item);
    QPopUpMenuItemDialog dlg(this);
    dlg.init(&item, ctrl->getNumItems() - 1);
    connect(&dlg, &QPopUpMenuItemDialog::updateItem,
            fMenuItems, &QPopupMenuItemList::handleUpdate);
    dlg.exec();
}

void QGUIPopUpMenu::editMenuItem(int idx)
{
    pfGUIPopUpMenu* ctrl = pfGUIPopUpMenu::Convert(fCreatable);
    QPopUpMenuItemDialog dlg(this);
    dlg.init(&ctrl->getItem(idx), idx);
    connect(&dlg, &QPopUpMenuItemDialog::updateItem,
            fMenuItems, &QPopupMenuItemList::handleUpdate);
    dlg.exec();
}

void QGUIPopUpMenu::delMenuItem(int idx)
{
    pfGUIPopUpMenu* ctrl = pfGUIPopUpMenu::Convert(fCreatable);
    ctrl->delItem(idx);
}


/* QPopUpMenuItemDialog */
QPopUpMenuItemDialog::QPopUpMenuItemDialog(QWidget* parent)
    : QDialog(parent), fItem(), fItemIdx()
{
    fName = new QLineEdit(this);
    fProcType = new QComboBox(this);
    fProcType->addItems(QStringList() << "(NULL)" << tr("Console Command")
                                      << tr("Python Script") << tr("Close Dialog"));
    fProcCommand = new QLineEdit(this);
    fProcCommandLabel = new QLabel(tr("Command:"), this);
    fProcCommand->setEnabled(false);
    fProcCommandLabel->setEnabled(false);
    fSubMenuKey = new QLinkLabel("(Null)", this);
    fYOffsetToNext = new QFloatEdit(this);
    fYOffsetToNext->setRange(-2147483647.0, 2147483647.0, 3);

    QGridLayout* layout = new QGridLayout(this);
    layout->addWidget(new QLabel(tr("Name:"), this), 0, 0);
    layout->addWidget(fName, 0, 1);
    layout->addWidget(new QLabel(tr("Proc Type:"), this), 1, 0);
    layout->addWidget(fProcType, 1, 1);
    layout->addWidget(fProcCommandLabel, 2, 0);
    layout->addWidget(fProcCommand, 2, 1);
    layout->addItem(new QSpacerItem(0, 10, QSizePolicy::Minimum, QSizePolicy::Minimum), 3, 0, 1, 2);
    layout->addWidget(new QLabel(tr("Submenu:"), this), 4, 0);
    layout->addWidget(fSubMenuKey, 4, 1);
    layout->addWidget(new QLabel(tr("Height Offset:"), this), 5, 0);
    layout->addWidget(fYOffsetToNext, 5, 1);

    connect(fProcType, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &QPopUpMenuItemDialog::setProcType);
    connect(fSubMenuKey, &QLinkLabel::activated, this, &QPopUpMenuItemDialog::selectKey);
    connect(fName, &QLineEdit::textChanged, this, &QPopUpMenuItemDialog::nameChanged);
    connect(fProcCommand, &QLineEdit::textChanged, this, &QPopUpMenuItemDialog::cmdChanged);
    connect(fYOffsetToNext, &QLineEdit::textChanged, this, &QPopUpMenuItemDialog::offsetChanged);
}

void QPopUpMenuItemDialog::init(pfGUIPopUpMenu::pfMenuItem* item, int idx)
{
    fItem = item;
    if (fItem->fHandler != NULL && fItem->fHandler->getType() == pfGUICtrlProcWriteableObject::kConsoleCmd)
        fProcCommand->setText(st2qstr(((pfGUIConsoleCmdProc*)fItem->fHandler)->getCommand()));
    fItemIdx = idx;
    fName->setText(st2qstr(fItem->fName));
    fProcType->setCurrentIndex(fItem->fHandler == NULL ? 0 : fItem->fHandler->getType());
    fSubMenuKey->setText(fItem->fSubMenu.Exists() ? st2qstr(fItem->fSubMenu->getName()) : tr("(Null)"));
    fYOffsetToNext->setValue(fItem->fYOffsetToNext);
}

void QPopUpMenuItemDialog::setProcType(int typeIdx)
{
    if (typeIdx == pfGUICtrlProcWriteableObject::kConsoleCmd)
        fProcCommand->setEnabled(true);
    else
        fProcCommand->setEnabled(false);
    fProcCommandLabel->setEnabled(fProcCommand->isEnabled());

    if (fItem == NULL)
        return;

    delete fItem->fHandler;

    switch (typeIdx) {
    case pfGUICtrlProcWriteableObject::kNull:
        fItem->fHandler = NULL;
        break;
    case pfGUICtrlProcWriteableObject::kConsoleCmd:
        fItem->fHandler = new pfGUIConsoleCmdProc();
        ((pfGUIConsoleCmdProc*)fItem->fHandler)->setCommand(qstr2st(fProcCommand->text()));
        break;
    case pfGUICtrlProcWriteableObject::kPythonScript:
        fItem->fHandler = new pfGUIPythonScriptProc();
        break;
    case pfGUICtrlProcWriteableObject::kCloseDlg:
        fItem->fHandler = new pfGUICloseDlgProc();
        break;
    }
}

void QPopUpMenuItemDialog::selectKey()
{
    if (fItem == NULL)
        return;

    QFindKeyDialog dlg(this);
    dlg.init(PrpShopMain::ResManager(), fItem->fSubMenu);
    if (dlg.exec() == QDialog::Accepted) {
        fItem->fSubMenu = dlg.selection();
        fSubMenuKey->setText(st2qstr(fItem->fSubMenu->getName()));
    } else {
        fItem->fSubMenu = plKey();
        fSubMenuKey->setText(tr("(Null)"));
    }
    emit updateItem(fItemIdx, st2qstr(fItem->fName), fItem->fSubMenu.Exists());
}

void QPopUpMenuItemDialog::nameChanged(QString name)
{
    if (fItem == NULL)
        return;

    fItem->fName = qstr2st(name);
    emit updateItem(fItemIdx, name, fItem->fSubMenu.Exists());
}

void QPopUpMenuItemDialog::cmdChanged(QString cmd)
{
    if (fItem == NULL || fItem->fHandler == NULL
        || fItem->fHandler->getType() != pfGUICtrlProcWriteableObject::kConsoleCmd)
        return;

    ((pfGUIConsoleCmdProc*)fItem->fHandler)->setCommand(qstr2st(cmd));
}

void QPopUpMenuItemDialog::offsetChanged(QString)
{
    if (fItem == NULL)
        return;

    fItem->fYOffsetToNext = fYOffsetToNext->value();
}
