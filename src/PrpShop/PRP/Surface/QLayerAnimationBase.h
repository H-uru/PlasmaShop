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

#ifndef _QLAYERANIMATIONBASE_H
#define _QLAYERANIMATIONBASE_H

#include "PRP/QCreatable.h"

#include "PRP/QObjLink.h"

class QLayerAnimationBase : public QCreatable
{
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

public:
    QLayerAnimationBase(plCreatable* pCre, QWidget* parent = nullptr);

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
