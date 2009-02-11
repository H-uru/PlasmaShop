#include "QLayerSDLAnimation.h"

#include <QLabel>
#include <QGridLayout>
#include <ResManager/plFactory.h>

QLayerSDLAnimation::QLayerSDLAnimation(plCreatable* pCre, QWidget* parent)
                  : QCreatable(pCre, kLayerSDLAnimation, parent)
{
    plLayerSDLAnimation* lay = (plLayerSDLAnimation*)fCreatable;

    fLayerAnimLink = new QCreatableLink(this, false);
    fLayerAnimLink->setText(tr("Layer Animation Properties"));
    fLayerAnimLink->setCreatable(lay);
    fLayerAnimLink->setForceType(kLayerAnimation);

    fVarName = new QLineEdit(this);
    fVarName->setText(~lay->getVarName());

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(fLayerAnimLink, 0, 0, 1, 2);
    layout->addWidget(new QLabel(tr("SDL Variable:"), this), 3, 0);
    layout->addWidget(fVarName, 3, 1);
}

void QLayerSDLAnimation::saveDamage()
{
    plLayerSDLAnimation* lay = (plLayerSDLAnimation*)fCreatable;
    lay->setVarName(~fVarName->text());
}
