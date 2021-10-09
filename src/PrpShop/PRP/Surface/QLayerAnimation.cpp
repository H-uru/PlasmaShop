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

#include "QLayerAnimation.h"

#include <QLabel>
#include <QGroupBox>
#include <QGridLayout>
#include <QSpacerItem>
#include <ResManager/plFactory.h>
#include "Main.h"
#include "QKeyDialog.h"

QLayerAnimation::QLayerAnimation(plCreatable* pCre, QWidget* parent)
    : QCreatable(pCre, kLayerAnimation, parent)
{
    plLayerAnimation* lay = plLayerAnimation::Convert(fCreatable);

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

    fAnimTimeConvert = new QCreatableLink(this, false);
    fAnimTimeConvert->setCreatable(&lay->getTimeConvert(), tr("Animation Time Properties"));

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(fSynchObjLink, 0, 0, 1, 2);
    layout->addWidget(new QLabel(tr("Base Layer:"), this), 1, 0);
    layout->addWidget(fBaseLayer, 1, 1);
    layout->addWidget(controllerGroup, 2, 0, 1, 2);
    layout->addWidget(fAnimTimeConvert, 3, 0, 1, 2);

    connect(fBaseLayer, &QCreatableLink::addObject, this, &QLayerAnimation::setBaseLayer);
    connect(fBaseLayer, &QCreatableLink::delObject, this, &QLayerAnimation::unsetBaseLayer);
    connect(fPreshade, &QControllerLink::createObject, this, &QLayerAnimation::newPreshade);
    connect(fPreshade, &QControllerLink::delObject, this, &QLayerAnimation::delPreshade);
    connect(fRuntime, &QControllerLink::createObject, this, &QLayerAnimation::newRuntime);
    connect(fRuntime, &QControllerLink::delObject, this, &QLayerAnimation::delRuntime);
    connect(fAmbient, &QControllerLink::createObject, this, &QLayerAnimation::newAmbient);
    connect(fAmbient, &QControllerLink::delObject, this, &QLayerAnimation::delAmbient);
    connect(fSpecular, &QControllerLink::createObject, this, &QLayerAnimation::newSpecular);
    connect(fSpecular, &QControllerLink::delObject, this, &QLayerAnimation::delSpecular);
    connect(fOpacity, &QControllerLink::createObject, this, &QLayerAnimation::newOpacity);
    connect(fOpacity, &QControllerLink::delObject, this, &QLayerAnimation::delOpacity);
    connect(fTransform, &QControllerLink::createObject, this, &QLayerAnimation::newTransform);
    connect(fTransform, &QControllerLink::delObject, this, &QLayerAnimation::delTransform);
}

void QLayerAnimation::setBaseLayer()
{
    plLayerAnimation* lay = plLayerAnimation::Convert(fCreatable);
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

void QLayerAnimation::unsetBaseLayer()
{
    plLayerAnimation* lay = plLayerAnimation::Convert(fCreatable);
    lay->setUnderLay(plKey());
    fBaseLayer->setCreatable(NULL, "(None)");
}

void QLayerAnimation::newPreshade(short type)
{
    plLayerAnimation* lay = plLayerAnimation::Convert(fCreatable);
    plController* pCre = plController::Convert(plFactory::Create(type));
    lay->setPreshadeCtl(pCre);
    fPreshade->setCreatable(pCre);
}

void QLayerAnimation::newRuntime(short type)
{
    plLayerAnimation* lay = plLayerAnimation::Convert(fCreatable);
    plController* pCre = plController::Convert(plFactory::Create(type));
    lay->setRuntimeCtl(pCre);
    fRuntime->setCreatable(pCre);
}

void QLayerAnimation::newAmbient(short type)
{
    plLayerAnimation* lay = plLayerAnimation::Convert(fCreatable);
    plController* pCre = plController::Convert(plFactory::Create(type));
    lay->setAmbientCtl(pCre);
    fAmbient->setCreatable(pCre);
}

void QLayerAnimation::newSpecular(short type)
{
    plLayerAnimation* lay = plLayerAnimation::Convert(fCreatable);
    plController* pCre = plController::Convert(plFactory::Create(type));
    lay->setSpecularCtl(pCre);
    fSpecular->setCreatable(pCre);
}

void QLayerAnimation::newOpacity(short type)
{
    plLayerAnimation* lay = plLayerAnimation::Convert(fCreatable);
    plController* pCre = plController::Convert(plFactory::Create(type));
    lay->setOpacityCtl(pCre);
    fOpacity->setCreatable(pCre);
}

void QLayerAnimation::newTransform(short type)
{
    plLayerAnimation* lay = plLayerAnimation::Convert(fCreatable);
    plController* pCre = plController::Convert(plFactory::Create(type));
    lay->setTransformCtl(pCre);
    fTransform->setCreatable(pCre);
}

void QLayerAnimation::delPreshade()
{
    plLayerAnimation* lay = plLayerAnimation::Convert(fCreatable);
    lay->setPreshadeCtl(NULL);
    fPreshade->setCreatable(NULL);
}

void QLayerAnimation::delRuntime()
{
    plLayerAnimation* lay = plLayerAnimation::Convert(fCreatable);
    lay->setRuntimeCtl(NULL);
    fRuntime->setCreatable(NULL);
}

void QLayerAnimation::delAmbient()
{
    plLayerAnimation* lay = plLayerAnimation::Convert(fCreatable);
    lay->setAmbientCtl(NULL);
    fAmbient->setCreatable(NULL);
}

void QLayerAnimation::delSpecular()
{
    plLayerAnimation* lay = plLayerAnimation::Convert(fCreatable);
    lay->setSpecularCtl(NULL);
    fSpecular->setCreatable(NULL);
}

void QLayerAnimation::delOpacity()
{
    plLayerAnimation* lay = plLayerAnimation::Convert(fCreatable);
    lay->setOpacityCtl(NULL);
    fOpacity->setCreatable(NULL);
}

void QLayerAnimation::delTransform()
{
    plLayerAnimation* lay = plLayerAnimation::Convert(fCreatable);
    lay->setTransformCtl(NULL);
    fTransform->setCreatable(NULL);
}
