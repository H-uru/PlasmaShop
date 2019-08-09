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

#ifndef _QCOORDINATEINTERFACE_H
#define _QCOORDINATEINTERFACE_H

#include "PRP/QCreatable.h"

#include <PRP/Object/plCoordinateInterface.h>
#include <QCheckBox>
#include "PRP/QKeyList.h"
#include "PRP/QMatrix44.h"
#include "PRP/QObjLink.h"

class QCoordinateInterface : public QCreatable
{
    Q_OBJECT

protected:
    QCreatableLink* fOwnerLink;
    QCreatableLink* fSynchObjLink;
    QCreatableLink* fParentLink;
    QCheckBox* fCBProperties[plCoordinateInterface::kNumProps];
    QKeyList* fChildren;
    QMatrix44* fLocalToParent;
    QMatrix44* fParentToLocal;
    QMatrix44* fLocalToWorld;
    QMatrix44* fWorldToLocal;

public:
    QCoordinateInterface(plCreatable* pCre, QWidget* parent = NULL);
    virtual void saveDamage();

protected slots:
    void setOwner();
    void unsetOwner();
};

#endif
