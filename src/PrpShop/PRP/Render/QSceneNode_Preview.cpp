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

#include "QSceneNode_Preview.h"

#include <PRP/plSceneNode.h>
#include <PRP/Object/plSceneObject.h>
#include <QToolBar>
#include <QGridLayout>

QSceneNode_Preview::QSceneNode_Preview(plCreatable* pCre, QWidget* parent)
    : QCreatable(pCre, kPreviewSceneNode, parent)
{
    plSceneNode* obj = plSceneNode::Convert(fCreatable);
    fRender = new QPlasmaRender(this);
    bool centered = false;
    for (const auto& soKey : obj->getSceneObjects()) {
        if (soKey->getType() == kSceneObject) {
            fRender->addObject(soKey);
            if (!centered) {
                fRender->center(soKey, false);
                centered = true;
            }
        }
    }
    fRender->build(QPlasmaRender::kNavModel, QPlasmaRender::kDrawTextured);
    QSizePolicy renderPolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    fRender->setSizePolicy(renderPolicy);

    QToolBar* viewToolbar = new QToolBar(tr("View"), this);
    viewToolbar->setFloatable(false);
    QActionGroup* viewActions = fRender->createViewActions();
    viewToolbar->addActions(viewActions->actions());

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setVerticalSpacing(0);
    layout->addWidget(viewToolbar, 0, 0);
    layout->addWidget(fRender, 1, 0);
}
