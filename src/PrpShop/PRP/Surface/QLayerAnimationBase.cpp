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

#include "QLayerAnimationBase.h"

#include <PRP/Surface/plLayerAnimation.h>
#include <QLabel>
#include <QGroupBox>
#include <QGridLayout>
#include <ResManager/plFactory.h>
#include "Main.h"
#include "QKeyDialog.h"

QLayerAnimationBase::QLayerAnimationBase(plCreatable* pCre, QWidget* parent)
    : QCreatable(pCre, kLayerAnimationBase, parent)
{
    plLayerAnimationBase* lay = plLayerAnimationBase::Convert(fCreatable);

    fSynchObjLink = new QCreatableLink(this, false);
    fSynchObjLink->setCreatable(lay, tr("Synch Flags"));
    fSynchObjLink->setForceType(kSynchedObject);

    fBaseLayer = new QCreatableLink(this);
    fBaseLayer->setKey(lay->getUnderLay());

    QGroupBox* controllerGroup = new QGroupBox(tr("Animation Controllers"), this);
    QGridLayout* controllerLayout = new QGridLayout(controllerGroup);
    controllerLayout->setVerticalSpacing(0);
    controllerLayout->setHorizontalSpacing(8);
    fPreshade = new QControllerLink(tr("Preshade Color"), controllerGroup);
    fRuntime = new QControllerLink(tr("Runtime Color"), controllerGroup);
    fAmbient = new QControllerLink(tr("Ambient Color"), controllerGroup);
    fSpecular = new QControllerLink(tr("Specular Color"), controllerGroup);
    fOpacity = new QControllerLink(tr("Opacity"), controllerGroup);
    fTransform = new QControllerLink(tr("UVW Transform"), controllerGroup);
    controllerLayout->addWidget(fPreshade, 0, 0);
    controllerLayout->addWidget(fRuntime, 1, 0);
    controllerLayout->addWidget(fAmbient, 2, 0);
    controllerLayout->addWidget(fSpecular, 3, 0);
    controllerLayout->addWidget(fOpacity, 4, 0);
    controllerLayout->addWidget(fTransform, 5, 0);

    fPreshade->setCreatable(lay->getPreshadeCtl());
    fRuntime->setCreatable(lay->getRuntimeCtl());
    fAmbient->setCreatable(lay->getAmbientCtl());
    fSpecular->setCreatable(lay->getSpecularCtl());
    fOpacity->setCreatable(lay->getOpacityCtl());
    fTransform->setCreatable(lay->getTransformCtl());

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(fSynchObjLink, 0, 0, 1, 2);
    layout->addWidget(new QLabel(tr("Base Layer:"), this), 1, 0);
    layout->addWidget(fBaseLayer, 1, 1);
    layout->addWidget(controllerGroup, 2, 0, 1, 2);

    connect(fBaseLayer, &QCreatableLink::addObject, this, &QLayerAnimationBase::setBaseLayer);
    connect(fBaseLayer, &QCreatableLink::delObject, this, &QLayerAnimationBase::unsetBaseLayer);
    connect(fPreshade, &QControllerLink::createObject, this, &QLayerAnimationBase::newPreshade);
    connect(fPreshade, &QControllerLink::delObject, this, &QLayerAnimationBase::delPreshade);
    connect(fRuntime, &QControllerLink::createObject, this, &QLayerAnimationBase::newRuntime);
    connect(fRuntime, &QControllerLink::delObject, this, &QLayerAnimationBase::delRuntime);
    connect(fAmbient, &QControllerLink::createObject, this, &QLayerAnimationBase::newAmbient);
    connect(fAmbient, &QControllerLink::delObject, this, &QLayerAnimationBase::delAmbient);
    connect(fSpecular, &QControllerLink::createObject, this, &QLayerAnimationBase::newSpecular);
    connect(fSpecular, &QControllerLink::delObject, this, &QLayerAnimationBase::delSpecular);
    connect(fOpacity, &QControllerLink::createObject, this, &QLayerAnimationBase::newOpacity);
    connect(fOpacity, &QControllerLink::delObject, this, &QLayerAnimationBase::delOpacity);
    connect(fTransform, &QControllerLink::createObject, this, &QLayerAnimationBase::newTransform);
    connect(fTransform, &QControllerLink::delObject, this, &QLayerAnimationBase::delTransform);
}

void QLayerAnimationBase::setBaseLayer()
{
    plLayerAnimationBase* lay = plLayerAnimationBase::Convert(fCreatable);
    QFindKeyDialog dlg(this);
    if (lay->getUnderLay().Exists())
        dlg.init(PrpShopMain::ResManager(), lay->getUnderLay());
    else
        dlg.init(PrpShopMain::ResManager(), lay->getKey()->getLocation(), kLayer);
    if (dlg.exec() == QDialog::Accepted) {
        lay->setUnderLay(dlg.selection());
        fBaseLayer->setKey(lay->getUnderLay());
    }
}

void QLayerAnimationBase::unsetBaseLayer()
{
    plLayerAnimationBase* lay = plLayerAnimationBase::Convert(fCreatable);
    lay->setUnderLay(plKey());
    fBaseLayer->setCreatable(nullptr, "(None)");
}

void QLayerAnimationBase::newPreshade(short type)
{
    plLayerAnimationBase* lay = plLayerAnimationBase::Convert(fCreatable);
    plController* pCre = plController::Convert(plFactory::Create(type));
    lay->setPreshadeCtl(pCre);
    fPreshade->setCreatable(pCre);
}

void QLayerAnimationBase::newRuntime(short type)
{
    plLayerAnimationBase* lay = plLayerAnimationBase::Convert(fCreatable);
    plController* pCre = plController::Convert(plFactory::Create(type));
    lay->setRuntimeCtl(pCre);
    fRuntime->setCreatable(pCre);
}

void QLayerAnimationBase::newAmbient(short type)
{
    plLayerAnimationBase* lay = plLayerAnimationBase::Convert(fCreatable);
    plController* pCre = plController::Convert(plFactory::Create(type));
    lay->setAmbientCtl(pCre);
    fAmbient->setCreatable(pCre);
}

void QLayerAnimationBase::newSpecular(short type)
{
    plLayerAnimationBase* lay = plLayerAnimationBase::Convert(fCreatable);
    plController* pCre = plController::Convert(plFactory::Create(type));
    lay->setSpecularCtl(pCre);
    fSpecular->setCreatable(pCre);
}

void QLayerAnimationBase::newOpacity(short type)
{
    plLayerAnimationBase* lay = plLayerAnimationBase::Convert(fCreatable);
    plController* pCre = plController::Convert(plFactory::Create(type));
    lay->setOpacityCtl(pCre);
    fOpacity->setCreatable(pCre);
}

void QLayerAnimationBase::newTransform(short type)
{
    plLayerAnimationBase* lay = plLayerAnimationBase::Convert(fCreatable);
    plController* pCre = plController::Convert(plFactory::Create(type));
    lay->setTransformCtl(pCre);
    fTransform->setCreatable(pCre);
}

void QLayerAnimationBase::delPreshade()
{
    plLayerAnimationBase* lay = plLayerAnimationBase::Convert(fCreatable);
    lay->setPreshadeCtl(nullptr);
    fPreshade->setCreatable(nullptr);
}

void QLayerAnimationBase::delRuntime()
{
    plLayerAnimationBase* lay = plLayerAnimationBase::Convert(fCreatable);
    lay->setRuntimeCtl(nullptr);
    fRuntime->setCreatable(nullptr);
}

void QLayerAnimationBase::delAmbient()
{
    plLayerAnimationBase* lay = plLayerAnimationBase::Convert(fCreatable);
    lay->setAmbientCtl(nullptr);
    fAmbient->setCreatable(nullptr);
}

void QLayerAnimationBase::delSpecular()
{
    plLayerAnimationBase* lay = plLayerAnimationBase::Convert(fCreatable);
    lay->setSpecularCtl(nullptr);
    fSpecular->setCreatable(nullptr);
}

void QLayerAnimationBase::delOpacity()
{
    plLayerAnimationBase* lay = plLayerAnimationBase::Convert(fCreatable);
    lay->setOpacityCtl(nullptr);
    fOpacity->setCreatable(nullptr);
}

void QLayerAnimationBase::delTransform()
{
    plLayerAnimationBase* lay = plLayerAnimationBase::Convert(fCreatable);
    lay->setTransformCtl(nullptr);
    fTransform->setCreatable(nullptr);
}
