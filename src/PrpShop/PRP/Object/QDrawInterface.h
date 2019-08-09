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

#ifndef _QDRAWINTERFACE_H
#define _QDRAWINTERFACE_H

#include "PRP/QCreatable.h"

#include <PRP/Object/plDrawInterface.h>
#include <QCheckBox>
#include <QSpinBox>
#include <QComboBox>
#include <QDialog>
#include "PRP/QKeyList.h"
#include "PRP/QObjLink.h"

class QDrawableList : public QKeyList
{
    Q_OBJECT

protected:
    QList<int> fDrawKeys;

public:
    QDrawableList(plKey container, QWidget* parent = NULL);

    void addKey(plKey key, int dkey);
    QList<int> drawKeys() const { return fDrawKeys; }

protected:
    virtual void contextMenuEvent(QContextMenuEvent* evt);
};

class QFindDrawKeyDialog : public QDialog
{
    Q_OBJECT

protected:
    QComboBox* fKeyBox;
    QSpinBox* fDrawKey;
    plResManager* fResMgr;
    plLocation fLocation;
    std::vector<plKey> fKeys;

public:
    QFindDrawKeyDialog(QWidget* parent = NULL);
    void init(plResManager* mgr, const plLocation& loc);

    plKey selection() const { return fKeys[fKeyBox->currentIndex()]; }
    int drawKey() const { return fDrawKey->value(); }
};

class QDrawInterface : public QCreatable
{
    Q_OBJECT

protected:
    QCreatableLink* fOwnerLink;
    QCreatableLink* fSynchObjLink;
    QCheckBox* fDisabled;
    QDrawableList* fDrawKeys;
    QKeyList* fRegions;

public:
    QDrawInterface(plCreatable* pCre, QWidget* parent = NULL);
    virtual void saveDamage();

protected slots:
    void setOwner();
    void unsetOwner();
};

#endif
