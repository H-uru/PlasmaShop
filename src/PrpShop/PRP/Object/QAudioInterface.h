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

#ifndef _QAUDIOINTERFACE_H
#define _QAUDIOINTERFACE_H

#include "PRP/QCreatable.h"

#include <PRP/Object/plAudioInterface.h>
#include <QCheckBox>
#include "PRP/QObjLink.h"

class QAudioInterface : public QCreatable {
    Q_OBJECT

protected:
    QCreatableLink* fOwnerLink;
    QCreatableLink* fSynchObjLink;
    QCheckBox* fDisabled;
    QCreatableLink* fAudibleLink;

public:
    QAudioInterface(plCreatable* pCre, QWidget* parent = NULL);
    virtual void saveDamage();

protected slots:
    void setOwner();
    void unsetOwner();
    void setAudible();
    void unsetAudible();
};

#endif
