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

#ifndef _QSIMULATIONINTERFACE_H
#define _QSIMULATIONINTERFACE_H

#include "PRP/QCreatable.h"

#include <PRP/Object/plSimulationInterface.h>
#include <QCheckBox>
#include "PRP/QObjLink.h"

class QSimulationInterface : public QCreatable
{
    Q_OBJECT

protected:
    QCreatableLink* fOwnerLink;
    QCreatableLink* fSynchObjLink;
    QCheckBox* fCBProperties[plSimulationInterface::kNumProps];
    QCreatableLink* fPhysicalLink;

public:
    QSimulationInterface(plCreatable* pCre, QWidget* parent = NULL);
    void saveDamage() override { }

protected slots:
    void setOwner();
    void unsetOwner();
    void setPhysical();
    void unsetPhysical();
};

#endif
