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

#include "QSceneObj_Preview.h"

#include <PRP/Object/plSceneObject.h>
#include <QGridLayout>

QSceneObj_Preview::QSceneObj_Preview(plCreatable* pCre, QWidget* parent)
                 : QCreatable(pCre, kPreviewSceneObject, parent)
{
    plSceneObject* obj = (plSceneObject*)fCreatable;
    fRender = new QPlasmaRender(this);
    fRender->addObject(obj->getKey());
    fRender->center(obj->getKey(), false);
    fRender->build(QPlasmaRender::kNavModel, QPlasmaRender::kDrawTextured);

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setVerticalSpacing(0);
    layout->addWidget(fRender, 0, 0);
}

void QSceneObj_Preview::saveDamage()
{ }
