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

#ifndef _QMULTISTAGEBEHMOD_H
#define _QMULTISTAGEBEHMOD_H

#include "PRP/QCreatable.h"

#include <PRP/Avatar/plMultistageBehMod.h>
#include <QComboBox>
#include <QLineEdit>
#include "QBitmaskCheckBox.h"
#include "PRP/QObjLink.h"
#include "PRP/QKeyList.h"

class QAnimStage : public QCreatable
{
    Q_OBJECT

protected:
    enum
    {
        kNotifyEnter, kNotifyLoop, kNotifyAdvance, kNotifyRegress,
        kNumNotifyFlags
    };
    QBitmaskCheckBox* fNotify[kNumNotifyFlags];

    QComboBox* fForwardType;
    QComboBox* fBackType;
    QComboBox* fAdvanceType;
    QComboBox* fRegressType;
    QLineEdit* fAnimName;
    QIntEdit* fLoops;
    QCheckBox* fDoAdvance;
    QCheckBox* fDoRegress;
    QIntEdit* fAdvanceTo;
    QIntEdit* fRegressTo;

public:
    QAnimStage(plCreatable* pCre, QWidget* parent = NULL);
    void saveDamage() override;
};

class QMultistageBehMod : public QCreatable
{
    Q_OBJECT

protected:
    QCreatableLink* fSynchObjLink;
    QTreeWidget* fStages;
    QCheckBox* fFreezePhys;
    QCheckBox* fSmartSeek;
    QCheckBox* fReverseControls;
    QKeyList* fReceivers;

public:
    QMultistageBehMod(plCreatable* pCre, QWidget* parent = NULL);
    void saveDamage() override;

protected slots:
    void stageContextMenu(const QPoint& pos);
    void activateStageItem(QTreeWidgetItem* item, int column);
};

#endif
