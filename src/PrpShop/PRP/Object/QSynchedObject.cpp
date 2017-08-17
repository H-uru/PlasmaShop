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

#include "QSynchedObject.h"

#include <QGroupBox>
#include <QLabel>
#include <QGridLayout>
#include <QTabWidget>
#include "QPlasma.h"

QSynchedObject::QSynchedObject(plCreatable* pCre, QWidget* parent)
              : QCreatable(pCre, kSynchedObject, parent)
{
    plSynchedObject* obj = plSynchedObject::Convert(fCreatable);

    QGroupBox* grpFlags = new QGroupBox(tr("Synched Object Flags"), this);
    QGridLayout* layFlags = new QGridLayout(grpFlags);
    layFlags->setVerticalSpacing(0);
    layFlags->setHorizontalSpacing(8);
    fCBFlags[kCbDontDirty] = new QCheckBox(tr("Don't Dirty"), grpFlags);
    fCBFlags[kCbSendReliably] = new QCheckBox(tr("Send Reliably"), grpFlags);
    fCBFlags[kCbHasConstantNetGroup] = new QCheckBox(tr("Has Constant Net Group"), grpFlags);
    fCBFlags[kCbDontSynchGameMessages] = new QCheckBox(tr("Don't Synch Game Messages"), grpFlags);
    fCBFlags[kCbExcludePersistentState] = new QCheckBox(tr("Exclude Persistent States"), grpFlags);
    fCBFlags[kCbExcludeAllPersistentState] = new QCheckBox(tr("Exclude All Persistent States"), grpFlags);
    fCBFlags[kCbHasVolatileState] = new QCheckBox(tr("Has Volatile States"), grpFlags);
    fCBFlags[kCbAllStateIsVolatile] = new QCheckBox(tr("All States Volatile"), grpFlags);
    layFlags->addWidget(fCBFlags[kCbDontDirty], 0, 0);
    layFlags->addWidget(fCBFlags[kCbDontSynchGameMessages], 0, 1);
    layFlags->addWidget(fCBFlags[kCbSendReliably], 1, 0);
    layFlags->addWidget(fCBFlags[kCbHasConstantNetGroup], 1, 1);
    layFlags->addWidget(fCBFlags[kCbHasVolatileState], 2, 0);
    layFlags->addWidget(fCBFlags[kCbExcludePersistentState], 2, 1);
    layFlags->addWidget(fCBFlags[kCbAllStateIsVolatile], 3, 0);
    layFlags->addWidget(fCBFlags[kCbExcludeAllPersistentState], 3, 1);
    fCBFlags[kCbDontDirty]->setChecked((obj->getSynchFlags() & plSynchedObject::kDontDirty) != 0);
    fCBFlags[kCbSendReliably]->setChecked((obj->getSynchFlags() & plSynchedObject::kSendReliably) != 0);
    fCBFlags[kCbHasConstantNetGroup]->setChecked((obj->getSynchFlags() & plSynchedObject::kHasConstantNetGroup) != 0);
    fCBFlags[kCbDontSynchGameMessages]->setChecked((obj->getSynchFlags() & plSynchedObject::kDontSynchGameMessages) != 0);
    fCBFlags[kCbExcludePersistentState]->setChecked((obj->getSynchFlags() & plSynchedObject::kExcludePersistentState) != 0);
    fCBFlags[kCbExcludeAllPersistentState]->setChecked((obj->getSynchFlags() & plSynchedObject::kExcludeAllPersistentState) != 0);
    fCBFlags[kCbHasVolatileState]->setChecked((obj->getSynchFlags() & plSynchedObject::kHasVolatileState) != 0);
    fCBFlags[kCbAllStateIsVolatile]->setChecked((obj->getSynchFlags() & plSynchedObject::kAllStateIsVolatile) != 0);

    QTabWidget* sdlTab = new QTabWidget(this);
    fExcludeList = new QStringListWidget(sdlTab);
    fVolatileList = new QStringListWidget(sdlTab);
    sdlTab->addTab(fExcludeList, tr("SDL Excludes"));
    sdlTab->addTab(fVolatileList, tr("SDL Volatiles"));

    for (size_t i=0; i<obj->getExcludes().size(); i++)
        fExcludeList->addString(st2qstr(obj->getExcludes()[i]));
    for (size_t i=0; i<obj->getVolatiles().size(); i++)
        fVolatileList->addString(st2qstr(obj->getVolatiles()[i]));

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(grpFlags, 0, 0);
    layout->addWidget(sdlTab, 1, 0);
}

void QSynchedObject::saveDamage()
{
    plSynchedObject* obj = plSynchedObject::Convert(fCreatable);

    obj->setSynchFlags((fCBFlags[kCbDontDirty]->isChecked() ? plSynchedObject::kDontDirty : 0)
                     | (fCBFlags[kCbSendReliably]->isChecked() ? plSynchedObject::kSendReliably : 0)
                     | (fCBFlags[kCbHasConstantNetGroup]->isChecked() ? plSynchedObject::kHasConstantNetGroup : 0)
                     | (fCBFlags[kCbDontSynchGameMessages]->isChecked() ? plSynchedObject::kDontSynchGameMessages : 0)
                     | (fCBFlags[kCbExcludePersistentState]->isChecked() ? plSynchedObject::kExcludePersistentState : 0)
                     | (fCBFlags[kCbExcludeAllPersistentState]->isChecked() ? plSynchedObject::kExcludeAllPersistentState : 0)
                     | (fCBFlags[kCbHasVolatileState]->isChecked() ? plSynchedObject::kHasVolatileState : 0)
                     | (fCBFlags[kCbAllStateIsVolatile]->isChecked() ? plSynchedObject::kAllStateIsVolatile : 0));
    obj->clearExcludes();
    obj->clearVolatiles();
    QStringList excludes = fExcludeList->strings();
    QStringList volatiles = fVolatileList->strings();
    QStringList::Iterator it;
    for (it = excludes.begin(); it != excludes.end(); it++)
        obj->setExclude(qstr2st(*it));
    for (it = volatiles.begin(); it != volatiles.end(); it++)
        obj->setVolatile(qstr2st(*it));
}
