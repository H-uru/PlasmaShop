#include "QLayerSDLAnimation.h"

#include <QLabel>
#include <QGroupBox>
#include <QGridLayout>
#include <QSpacerItem>
#include <ResManager/plFactory.h>
#include "../../Main.h"
#include "../../QKeyDialog.h"
#include "../../../QPlasma.h"

QLayerSDLAnimation::QLayerSDLAnimation(plCreatable* pCre, QWidget* parent)
                  : QCreatable(pCre, kLayerSDLAnimation, parent)
{
    plLayerSDLAnimation* lay = (plLayerSDLAnimation*)fCreatable;

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

    fVarName = new QLineEdit(this);
    fVarName->setText(~lay->getVarName());

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(fSynchObjLink, 0, 0, 1, 2);
    layout->addWidget(new QLabel(tr("Base Layer:"), this), 1, 0);
    layout->addWidget(fBaseLayer, 1, 1);
    layout->addWidget(controllerGroup, 2, 0, 1, 2);
    layout->addWidget(new QLabel(tr("SDL Variable:"), this), 3, 0);
    layout->addWidget(fVarName, 3, 1);

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

void QLayerSDLAnimation::saveDamage()
{
    plLayerSDLAnimation* lay = (plLayerSDLAnimation*)fCreatable;
    lay->setVarName(~fVarName->text());
}

void QLayerSDLAnimation::setBaseLayer()
{
    plLayerSDLAnimation* lay = (plLayerSDLAnimation*)fCreatable;
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

void QLayerSDLAnimation::unsetBaseLayer()
{
    plLayerSDLAnimation* lay = (plLayerSDLAnimation*)fCreatable;
    lay->setUnderLay(plKey());
    fBaseLayer->setCreatable(NULL);
    fBaseLayer->setText("(None)");
}

void QLayerSDLAnimation::newPreshade(short type)
{
    plLayerSDLAnimation* lay = (plLayerSDLAnimation*)fCreatable;
    plController* pCre = plController::Convert(plFactory::Create(type));
    lay->setPreshadeCtl(pCre);
    fPreshade->setCreatable(pCre);
}

void QLayerSDLAnimation::newRuntime(short type)
{
    plLayerSDLAnimation* lay = (plLayerSDLAnimation*)fCreatable;
    plController* pCre = plController::Convert(plFactory::Create(type));
    lay->setRuntimeCtl(pCre);
    fRuntime->setCreatable(pCre);
}

void QLayerSDLAnimation::newAmbient(short type)
{
    plLayerSDLAnimation* lay = (plLayerSDLAnimation*)fCreatable;
    plController* pCre = plController::Convert(plFactory::Create(type));
    lay->setAmbientCtl(pCre);
    fAmbient->setCreatable(pCre);
}

void QLayerSDLAnimation::newSpecular(short type)
{
    plLayerSDLAnimation* lay = (plLayerSDLAnimation*)fCreatable;
    plController* pCre = plController::Convert(plFactory::Create(type));
    lay->setSpecularCtl(pCre);
    fSpecular->setCreatable(pCre);
}

void QLayerSDLAnimation::newOpacity(short type)
{
    plLayerSDLAnimation* lay = (plLayerSDLAnimation*)fCreatable;
    plController* pCre = plController::Convert(plFactory::Create(type));
    lay->setOpacityCtl(pCre);
    fOpacity->setCreatable(pCre);
}

void QLayerSDLAnimation::newTransform(short type)
{
    plLayerSDLAnimation* lay = (plLayerSDLAnimation*)fCreatable;
    plController* pCre = plController::Convert(plFactory::Create(type));
    lay->setTransformCtl(pCre);
    fTransform->setCreatable(pCre);
}

void QLayerSDLAnimation::delPreshade()
{
    plLayerSDLAnimation* lay = (plLayerSDLAnimation*)fCreatable;
    lay->setPreshadeCtl(NULL);
    fPreshade->setCreatable(NULL);
}

void QLayerSDLAnimation::delRuntime()
{
    plLayerSDLAnimation* lay = (plLayerSDLAnimation*)fCreatable;
    lay->setRuntimeCtl(NULL);
    fRuntime->setCreatable(NULL);
}

void QLayerSDLAnimation::delAmbient()
{
    plLayerSDLAnimation* lay = (plLayerSDLAnimation*)fCreatable;
    lay->setAmbientCtl(NULL);
    fAmbient->setCreatable(NULL);
}

void QLayerSDLAnimation::delSpecular()
{
    plLayerSDLAnimation* lay = (plLayerSDLAnimation*)fCreatable;
    lay->setSpecularCtl(NULL);
    fSpecular->setCreatable(NULL);
}

void QLayerSDLAnimation::delOpacity()
{
    plLayerSDLAnimation* lay = (plLayerSDLAnimation*)fCreatable;
    lay->setOpacityCtl(NULL);
    fOpacity->setCreatable(NULL);
}

void QLayerSDLAnimation::delTransform()
{
    plLayerSDLAnimation* lay = (plLayerSDLAnimation*)fCreatable;
    lay->setTransformCtl(NULL);
    fTransform->setCreatable(NULL);
}
