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

#ifndef _QSHADOWMASTER_H
#define _QSHADOWMASTER_H

#include "PRP/QCreatable.h"

#include <PRP/Light/plShadowMaster.h>
#include <QCheckBox>
#include "PRP/QObjLink.h"

class QShadowMaster : public QCreatable
{
    Q_OBJECT

protected:
    QCreatableLink* fOwnerLink;
    QCreatableLink* fSynchObjLink;
    QCheckBox* fProperties[plShadowMaster::kNumProps];
    QFloatEdit* fAttenDist;
    QFloatEdit* fMinDist;
    QFloatEdit* fMaxDist;
    QFloatEdit* fPower;
    QIntEdit* fMinSize;
    QIntEdit* fMaxSize;

public:
    QShadowMaster(plCreatable* pCre, QWidget* parent = NULL);
    void saveDamage() override;

protected slots:
    void setOwner();
    void unsetOwner();
};

#endif
