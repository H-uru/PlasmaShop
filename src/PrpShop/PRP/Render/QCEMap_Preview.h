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

#ifndef _QCEMAP_PREVIEW_H
#define _QCEMAP_PREVIEW_H

#include "PRP/QCreatable.h"
#include "PRP/Surface/QCubicEnvironmap.h"
#include "QPlasmaCubicRender.h"

class plCubicEnvironmap;
class QCEMap_Preview : public QCreatable {
    Q_OBJECT

protected:
    QPlasmaCubicRender* fRender;

public:
    QCEMap_Preview(plCreatable* pCre, QWidget* parent = nullptr);
    virtual void saveDamage() { }

private:
    const QImage buildCubeMapTexture(plCubicEnvironmap*) const;
};

#endif
