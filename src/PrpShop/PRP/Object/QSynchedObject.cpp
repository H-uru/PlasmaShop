#include "QSynchedObject.h"

#include <QGroupBox>
#include <QLabel>
#include <QGridLayout>
#include <QTabWidget>

QSynchedObject::QSynchedObject(plCreatable* pCre, QWidget* parent)
              : QCreatable(pCre, kSynchedObject, parent)
{
    plSynchedObject* obj = (plSynchedObject*)fCreatable;

    QGroupBox* grpFlags = new QGroupBox(tr("Synched Object Flags"), this);
    QGridLayout* layFlags = new QGridLayout(grpFlags);
    layFlags->setVerticalSpacing(2);
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
    fCBFlags[kCbDontDirty]->setChecked((obj->getFlags() & plSynchedObject::kDontDirty) != 0);
    fCBFlags[kCbSendReliably]->setChecked((obj->getFlags() & plSynchedObject::kSendReliably) != 0);
    fCBFlags[kCbHasConstantNetGroup]->setChecked((obj->getFlags() & plSynchedObject::kHasConstantNetGroup) != 0);
    fCBFlags[kCbDontSynchGameMessages]->setChecked((obj->getFlags() & plSynchedObject::kDontSynchGameMessages) != 0);
    fCBFlags[kCbExcludePersistentState]->setChecked((obj->getFlags() & plSynchedObject::kExcludePersistentState) != 0);
    fCBFlags[kCbExcludeAllPersistentState]->setChecked((obj->getFlags() & plSynchedObject::kExcludeAllPersistentState) != 0);
    fCBFlags[kCbHasVolatileState]->setChecked((obj->getFlags() & plSynchedObject::kHasVolatileState) != 0);
    fCBFlags[kCbAllStateIsVolatile]->setChecked((obj->getFlags() & plSynchedObject::kAllStateIsVolatile) != 0);

    QTabWidget* sdlTab = new QTabWidget(this);
    fExcludeList = new QStringListWidget(sdlTab);
    fVolatileList = new QStringListWidget(sdlTab);
    sdlTab->addTab(fExcludeList, tr("SDL Excludes"));
    sdlTab->addTab(fVolatileList, tr("SDL Volatiles"));

    for (size_t i=0; i<obj->getExcludes().getSize(); i++)
        fExcludeList->addString(obj->getExcludes()[i].cstr());
    for (size_t i=0; i<obj->getVolatiles().getSize(); i++)
        fVolatileList->addString(obj->getVolatiles()[i].cstr());

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(grpFlags, 0, 0);
    layout->addWidget(sdlTab, 1, 0);
}

void QSynchedObject::saveDamage()
{
    plSynchedObject* obj = (plSynchedObject*)fCreatable;

    obj->setFlags((fCBFlags[kCbDontDirty]->isChecked() ? plSynchedObject::kDontDirty : 0)
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
        obj->setExclude((*it).toUtf8().data());
    for (it = volatiles.begin(); it != volatiles.end(); it++)
        obj->setVolatile((*it).toUtf8().data());
}
