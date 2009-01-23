#include "QLayerAnimation.h"

#include <QLabel>
#include <QGroupBox>
#include <QGridLayout>
#include <QSpacerItem>
#include <ResManager/plFactory.h>
#include "../../Main.h"
#include "../../QKeyDialog.h"

QLayerAnimation::QLayerAnimation(plCreatable* pCre, QWidget* parent)
               : QCreatable(pCre, kLayerAnimation, parent)
{
    plLayerAnimation* lay = (plLayerAnimation*)fCreatable;

    fSynchObjLink = new QCreatableLink(this, false);
    fSynchObjLink->setText(tr("Synch Flags"));
    fSynchObjLink->setCreatable(lay);
    fSynchObjLink->setForceType(kSynchedObject);

    fBaseLayer = new QCreatableLink(this);
    fBaseLayer->setKey(lay->getUnderLay());
    fBaseLayer->setText(lay->getUnderLay().Exists()
                        ? ~lay->getUnderLay()->getName()
                        : "(None)");

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
    fAnimTimeConvert->setText(tr("Animation Time Controller"));
    fAnimTimeConvert->setCreatable(&lay->getTimeConvert());

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(fSynchObjLink, 0, 0, 1, 2);
    layout->addWidget(new QLabel(tr("Base Layer:"), this), 1, 0);
    layout->addWidget(fBaseLayer, 1, 1);
    layout->addWidget(controllerGroup, 2, 0, 1, 2);
    layout->addWidget(fAnimTimeConvert, 3, 0, 1, 2);

    connect(fBaseLayer, SIGNAL(addObject()), this, SLOT(setBaseLayer()));
    connect(fBaseLayer, SIGNAL(delObject()), this, SLOT(unsetBaseLayer()));
    connect(fPreshade, SIGNAL(createObject(short)), this, SLOT(newPreshade(short)));
    connect(fPreshade, SIGNAL(delObject()), this, SLOT(delPreshade()));
    connect(fRuntime, SIGNAL(createObject(short)), this, SLOT(newRuntime(short)));
    connect(fRuntime, SIGNAL(delObject()), this, SLOT(delRuntime()));
    connect(fAmbient, SIGNAL(createObject(short)), this, SLOT(newAmbient(short)));
    connect(fAmbient, SIGNAL(delObject()), this, SLOT(delAmbient()));
    connect(fSpecular, SIGNAL(createObject(short)), this, SLOT(newSpecular(short)));
    connect(fSpecular, SIGNAL(delObject()), this, SLOT(delSpecular()));
    connect(fOpacity, SIGNAL(createObject(short)), this, SLOT(newOpacity(short)));
    connect(fOpacity, SIGNAL(delObject()), this, SLOT(delOpacity()));
    connect(fTransform, SIGNAL(createObject(short)), this, SLOT(newTransform(short)));
    connect(fTransform, SIGNAL(delObject()), this, SLOT(delTransform()));
}

void QLayerAnimation::saveDamage()
{ }

void QLayerAnimation::setBaseLayer()
{
    plLayerAnimation* lay = (plLayerAnimation*)fCreatable;
    QFindKeyDialog dlg(this);
    if (lay->getUnderLay().Exists())
        dlg.init(PrpShopMain::ResManager(), lay->getUnderLay());
    else
        dlg.init(PrpShopMain::ResManager(), lay->getKey()->getLocation(), kLayer);
    if (dlg.exec() == QDialog::Accepted) {
        lay->setUnderLay(dlg.selection());
        fBaseLayer->setKey(lay->getUnderLay());
        fBaseLayer->setText(~lay->getUnderLay()->getName());
    }
}

void QLayerAnimation::unsetBaseLayer()
{
    plLayerAnimation* lay = (plLayerAnimation*)fCreatable;
    lay->setUnderLay(plKey());
    fBaseLayer->setCreatable(NULL);
    fBaseLayer->setText("(None)");
}

void QLayerAnimation::newPreshade(short type)
{
    plLayerAnimation* lay = (plLayerAnimation*)fCreatable;
    plController* pCre = plController::Convert(plFactory::Create(type));
    lay->setPreshadeCtl(pCre);
    fPreshade->setCreatable(pCre);
}

void QLayerAnimation::newRuntime(short type)
{
    plLayerAnimation* lay = (plLayerAnimation*)fCreatable;
    plController* pCre = plController::Convert(plFactory::Create(type));
    lay->setRuntimeCtl(pCre);
    fRuntime->setCreatable(pCre);
}

void QLayerAnimation::newAmbient(short type)
{
    plLayerAnimation* lay = (plLayerAnimation*)fCreatable;
    plController* pCre = plController::Convert(plFactory::Create(type));
    lay->setAmbientCtl(pCre);
    fAmbient->setCreatable(pCre);
}

void QLayerAnimation::newSpecular(short type)
{
    plLayerAnimation* lay = (plLayerAnimation*)fCreatable;
    plController* pCre = plController::Convert(plFactory::Create(type));
    lay->setSpecularCtl(pCre);
    fSpecular->setCreatable(pCre);
}

void QLayerAnimation::newOpacity(short type)
{
    plLayerAnimation* lay = (plLayerAnimation*)fCreatable;
    plController* pCre = plController::Convert(plFactory::Create(type));
    lay->setOpacityCtl(pCre);
    fOpacity->setCreatable(pCre);
}

void QLayerAnimation::newTransform(short type)
{
    plLayerAnimation* lay = (plLayerAnimation*)fCreatable;
    plController* pCre = plController::Convert(plFactory::Create(type));
    lay->setTransformCtl(pCre);
    fTransform->setCreatable(pCre);
}

void QLayerAnimation::delPreshade()
{
    plLayerAnimation* lay = (plLayerAnimation*)fCreatable;
    lay->setPreshadeCtl(NULL);
    fPreshade->setCreatable(NULL);
}

void QLayerAnimation::delRuntime()
{
    plLayerAnimation* lay = (plLayerAnimation*)fCreatable;
    lay->setRuntimeCtl(NULL);
    fRuntime->setCreatable(NULL);
}

void QLayerAnimation::delAmbient()
{
    plLayerAnimation* lay = (plLayerAnimation*)fCreatable;
    lay->setAmbientCtl(NULL);
    fAmbient->setCreatable(NULL);
}

void QLayerAnimation::delSpecular()
{
    plLayerAnimation* lay = (plLayerAnimation*)fCreatable;
    lay->setSpecularCtl(NULL);
    fSpecular->setCreatable(NULL);
}

void QLayerAnimation::delOpacity()
{
    plLayerAnimation* lay = (plLayerAnimation*)fCreatable;
    lay->setOpacityCtl(NULL);
    fOpacity->setCreatable(NULL);
}

void QLayerAnimation::delTransform()
{
    plLayerAnimation* lay = (plLayerAnimation*)fCreatable;
    lay->setTransformCtl(NULL);
    fTransform->setCreatable(NULL);
}
