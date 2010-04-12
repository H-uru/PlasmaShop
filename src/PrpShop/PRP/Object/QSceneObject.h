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

#ifndef _QSCENEOBJECT_H
#define _QSCENEOBJECT_H

#include "../QCreatable.h"

#include <PRP/Object/plSceneObject.h>
#include "../QKeyList.h"
#include "../QObjLink.h"

class QSceneObject : public QCreatable {
    Q_OBJECT

protected:
    QCreatableLink* fSynchObjLink;
    QCreatableLink* fDrawIntfLink;
    QCreatableLink* fSimIntfLink;
    QCreatableLink* fCoordIntfLink;
    QCreatableLink* fAudioIntfLink;
    QKeyList* fInterfaceList;
    QKeyList* fModifierList;

public:
    QSceneObject(plCreatable* pCre, QWidget* parent = NULL);
    virtual void saveDamage();

protected slots:
    void setDraw();
    void setSim();
    void setCoord();
    void setAudio();
    void unsetDraw();
    void unsetSim();
    void unsetCoord();
    void unsetAudio();
};

#endif
