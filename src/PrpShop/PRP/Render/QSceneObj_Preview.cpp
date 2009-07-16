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
