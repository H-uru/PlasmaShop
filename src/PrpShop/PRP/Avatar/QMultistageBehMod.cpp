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

#include "QMultistageBehMod.h"

#include <QLabel>
#include <QTabWidget>
#include <QGroupBox>
#include <QGridLayout>
#include <QMenu>
#include "../../Main.h"

/* QAnimStage */
static QString s_PlayTypes[] = { "None", "Key", "Auto" };
static QString s_AdvanceTypes[] = { "None", "On Move", "Auto", "Any Key" };

QAnimStage::QAnimStage(plCreatable* pCre, QWidget* parent)
          : QCreatable(pCre, kAnimStage, parent)
{
    plAnimStage* obj = (plAnimStage*)fCreatable;

    QGroupBox* grpNotify = new QGroupBox(tr("Notify On"), this);
    QGridLayout* layNotify = new QGridLayout(grpNotify);
    layNotify->setVerticalSpacing(0);
    layNotify->setHorizontalSpacing(8);
    fNotify[kNotifyEnter] = new QCheckBox(tr("Enter"), grpNotify);
    fNotify[kNotifyLoop] = new QCheckBox(tr("Loop"), grpNotify);
    fNotify[kNotifyAdvance] = new QCheckBox(tr("Advance"), grpNotify);
    fNotify[kNotifyRegress] = new QCheckBox(tr("Regress"), grpNotify);
    fNotify[kNotifyEnter]->setChecked((obj->getNotify() & plAnimStage::kNotifyEnter) != 0);
    fNotify[kNotifyLoop]->setChecked((obj->getNotify() & plAnimStage::kNotifyLoop) != 0);
    fNotify[kNotifyAdvance]->setChecked((obj->getNotify() & plAnimStage::kNotifyAdvance) != 0);
    fNotify[kNotifyRegress]->setChecked((obj->getNotify() & plAnimStage::kNotifyRegress) != 0);
    layNotify->addWidget(fNotify[kNotifyEnter], 0, 0);
    layNotify->addWidget(fNotify[kNotifyLoop], 1, 0);
    layNotify->addWidget(fNotify[kNotifyAdvance], 0, 1);
    layNotify->addWidget(fNotify[kNotifyRegress], 1, 1);

    fForwardType = new QComboBox(this);
    for (int i=0; i<plAnimStage::kPlayMax; i++)
        fForwardType->addItem(s_PlayTypes[i]);
    fForwardType->setCurrentIndex(obj->getForwardType());

    fBackType = new QComboBox(this);
    for (int i=0; i<plAnimStage::kPlayMax; i++)
        fBackType->addItem(s_PlayTypes[i]);
    fBackType->setCurrentIndex(obj->getBackType());

    fAdvanceType = new QComboBox(this);
    for (int i=0; i<plAnimStage::kAdvanceMax; i++)
        fAdvanceType->addItem(s_AdvanceTypes[i]);
    fAdvanceType->setCurrentIndex(obj->getAdvanceType());

    fRegressType = new QComboBox(this);
    for (int i=0; i<plAnimStage::kAdvanceMax; i++)
        fRegressType->addItem(s_AdvanceTypes[i]);
    fRegressType->setCurrentIndex(obj->getRegressType());

    fAnimName = new QLineEdit(this);
    fAnimName->setText(st2qstr(obj->getAnimName()));

    fLoops = new QIntEdit(this);
    fLoops->setValue(obj->getLoops());

    fDoAdvance = new QCheckBox(tr("Advance To:"), this);
    fDoRegress = new QCheckBox(tr("Regress To:"), this);
    fAdvanceTo = new QIntEdit(this);
    fRegressTo = new QIntEdit(this);
    fDoAdvance->setChecked(obj->doAdvanceTo());
    fDoRegress->setChecked(obj->doRegressTo());
    fAdvanceTo->setValue(obj->getAdvanceTo());
    fRegressTo->setValue(obj->getRegressTo());
    fAdvanceTo->setEnabled(obj->doAdvanceTo());
    fRegressTo->setEnabled(obj->doRegressTo());

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(grpNotify, 0, 0, 1, 5);
    layout->addWidget(new QLabel(tr("Forward Type:"), this), 1, 0);
    layout->addWidget(fForwardType, 1, 1);
    layout->addWidget(new QLabel(tr("Backward Type:"), this), 2, 0);
    layout->addWidget(fBackType, 2, 1);
    layout->addItem(new QSpacerItem(8, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum), 1, 2);
    layout->addWidget(new QLabel(tr("Advance Type:"), this), 1, 3);
    layout->addWidget(fAdvanceType, 1, 4);
    layout->addWidget(new QLabel(tr("Regress Type:"), this), 2, 3);
    layout->addWidget(fRegressType, 2, 4);
    layout->addWidget(new QLabel(tr("Animation Name:"), this), 3, 0);
    layout->addWidget(fAnimName, 3, 1, 1, 4);
    layout->addWidget(new QLabel(tr("Loops:"), this), 4, 0);
    layout->addWidget(fLoops, 4, 1, 1, 4);
    layout->addWidget(fDoAdvance, 5, 0);
    layout->addWidget(fAdvanceTo, 5, 1, 1, 4);
    layout->addWidget(fDoRegress, 6, 0);
    layout->addWidget(fRegressTo, 6, 1, 1, 4);

    connect(fDoAdvance, SIGNAL(toggled(bool)), fAdvanceTo, SLOT(setEnabled(bool)));
    connect(fDoRegress, SIGNAL(toggled(bool)), fRegressTo, SLOT(setEnabled(bool)));
}

void QAnimStage::saveDamage()
{
    plAnimStage* obj = (plAnimStage*)fCreatable;
    obj->setNotify((fNotify[kNotifyEnter]->isChecked() ? plAnimStage::kNotifyEnter : 0)
                 | (fNotify[kNotifyLoop]->isChecked() ? plAnimStage::kNotifyLoop : 0)
                 | (fNotify[kNotifyAdvance]->isChecked() ? plAnimStage::kNotifyAdvance : 0)
                 | (fNotify[kNotifyRegress]->isChecked() ? plAnimStage::kNotifyRegress : 0));
    obj->setForwardType((plAnimStage::PlayType)fForwardType->currentIndex());
    obj->setBackType((plAnimStage::PlayType)fBackType->currentIndex());
    obj->setAdvanceType((plAnimStage::AdvanceType)fAdvanceType->currentIndex());
    obj->setRegressType((plAnimStage::AdvanceType)fRegressType->currentIndex());
    obj->setAnimName(qstr2st(fAnimName->text()));
    obj->setLoops(fLoops->value());
    obj->setAdvanceTo(fDoAdvance->isChecked(), fAdvanceTo->value());
    obj->setRegressTo(fDoRegress->isChecked(), fRegressTo->value());
}


/* QMultistageBehMod */
QMultistageBehMod::QMultistageBehMod(plCreatable* pCre, QWidget* parent)
                 : QCreatable(pCre, kMultistageBehMod, parent)
{
    plMultistageBehMod* obj = plMultistageBehMod::Convert(fCreatable);

    fSynchObjLink = new QCreatableLink(this, false);
    fSynchObjLink->setCreatable(obj, tr("Synch Flags"));
    fSynchObjLink->setForceType(kSynchedObject);

    QGroupBox* grpFlags = new QGroupBox(tr("Flags"), this);
    QGridLayout* layFlags = new QGridLayout(grpFlags);
    layFlags->setVerticalSpacing(0);
    layFlags->setHorizontalSpacing(8);
    fFreezePhys = new QCheckBox(tr("Freeze Physics"), grpFlags);
    fFreezePhys->setChecked(obj->getFreezePhys());
    fSmartSeek = new QCheckBox(tr("Smart Seek"), grpFlags);
    fSmartSeek->setChecked(obj->getSmartSeek());
    fReverseControls = new QCheckBox(tr("Reverse Controls on Release"), grpFlags);
    fReverseControls->setChecked(obj->getReverseFBControlsOnRelease());
    layFlags->addWidget(fFreezePhys, 0, 0);
    layFlags->addWidget(fSmartSeek, 1, 0);
    layFlags->addWidget(fReverseControls, 2, 0);

    QTabWidget* listTabs = new QTabWidget(this);

    fStages = new QTreeWidget(listTabs);
    fStages->setColumnCount(2);
    fStages->setUniformRowHeights(true);
    fStages->setRootIsDecorated(false);
    fStages->headerItem()->setText(0, tr("Idx"));
    fStages->headerItem()->setText(1, tr("Type"));
    fStages->setContextMenuPolicy(Qt::CustomContextMenu);
    for (size_t i=0; i<obj->getStages().size(); i++) {
        plAnimStage* stage = obj->getStages()[i];
        QTreeWidgetItem* item = new QTreeWidgetItem(fStages,
            QStringList() << QString("%1").arg(i) << pqGetFriendlyClassName(stage->ClassIndex()));
        item->setIcon(0, pqGetTypeIcon(stage->ClassIndex()));
    }

    fReceivers = new QKeyList(obj->getKey(), listTabs);
    for (size_t i=0; i<obj->getReceivers().size(); i++)
        fReceivers->addKey(obj->getReceivers()[i]);

    listTabs->addTab(fStages, tr("Animation Stages"));
    listTabs->addTab(fReceivers, tr("Receivers"));

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(fSynchObjLink, 0, 0);
    layout->addWidget(grpFlags, 1, 0);
    layout->addWidget(listTabs, 2, 0);
    fStages->resizeColumnToContents(1);
    fStages->resizeColumnToContents(0);
    fReceivers->adjustColumns();

    connect(fStages, SIGNAL(customContextMenuRequested(const QPoint&)),
            this, SLOT(stageContextMenu(const QPoint&)));
    connect(fStages, SIGNAL(itemActivated(QTreeWidgetItem*, int)),
            this, SLOT(activateStageItem(QTreeWidgetItem*, int)));
}

void QMultistageBehMod::saveDamage()
{
    plMultistageBehMod* obj = plMultistageBehMod::Convert(fCreatable);

    obj->setFreezePhys(fFreezePhys->isChecked());
    obj->setSmartSeek(fSmartSeek->isChecked());
    obj->setReverseFBControlsOnRelease(fReverseControls->isChecked());

    obj->clearReceivers();
    QList<plKey> keys = fReceivers->keys();
    while (!keys.isEmpty())
        obj->addReceiver(keys.takeFirst());
}

void QMultistageBehMod::stageContextMenu(const QPoint& pos)
{
    plMultistageBehMod* obj = plMultistageBehMod::Convert(fCreatable);

    QMenu menu(this);
    QAction* addObjItem = menu.addAction(tr("Add Stage..."));
    QAction* delObjItem = menu.addAction(tr("Remove Stage"));

    if (fStages->currentItem() == NULL)
        delObjItem->setEnabled(false);

    QAction* sel = menu.exec(fStages->viewport()->mapToGlobal(pos));
    if (sel == addObjItem) {
        plAnimStage* stage = new plAnimStage();
        int id = obj->getStages().size();
        QTreeWidgetItem* item = new QTreeWidgetItem(fStages,
            QStringList() << QString("%1").arg(id) << pqGetFriendlyClassName(stage->ClassIndex()));
        item->setIcon(0, pqGetTypeIcon(stage->ClassIndex()));
        obj->addStage(stage);
    } else if (sel == delObjItem) {
        int idx = fStages->indexOfTopLevelItem(fStages->currentItem());
        QTreeWidgetItem* item = fStages->takeTopLevelItem(idx);
        delete item;
        obj->delStage(idx);
    }
}

void QMultistageBehMod::activateStageItem(QTreeWidgetItem* item, int)
{
    plMultistageBehMod* obj = plMultistageBehMod::Convert(fCreatable);
    PrpShopMain::Instance()->editCreatable(obj->getStages()[fStages->indexOfTopLevelItem(item)]);
}
