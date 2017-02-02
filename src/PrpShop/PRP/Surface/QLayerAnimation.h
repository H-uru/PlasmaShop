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

#ifndef _QLAYERANIMATION_H
#define _QLAYERANIMATION_H

#include "PRP/QCreatable.h"

#include <PRP/Surface/plLayerAnimation.h>
#include "PRP/QObjLink.h"
#include "PRP/QMatrix44.h"
#include "QColorEdit.h"

class QLayerAnimation : public QCreatable {
    Q_OBJECT

protected:
    QCreatableLink* fSynchObjLink;
    QCreatableLink* fBaseLayer;
    QControllerLink* fPreshade;
    QControllerLink* fRuntime;
    QControllerLink* fAmbient;
    QControllerLink* fSpecular;
    QControllerLink* fOpacity;
    QControllerLink* fTransform;
    QCreatableLink* fAnimTimeConvert;

public:
    QLayerAnimation(plCreatable* pCre, QWidget* parent = NULL);
    virtual void saveDamage() { }

protected slots:
    void setBaseLayer();
    void unsetBaseLayer();

    void newPreshade(short type);
    void newRuntime(short type);
    void newAmbient(short type);
    void newSpecular(short type);
    void newOpacity(short type);
    void newTransform(short type);
    void delPreshade();
    void delRuntime();
    void delAmbient();
    void delSpecular();
    void delOpacity();
    void delTransform();
};

#endif
