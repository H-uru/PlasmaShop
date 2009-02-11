#ifndef _QLAYERSDLANIMATION_H
#define _QLAYERSDLANIMATION_H

#include "../QCreatable.h"

#include <PRP/Surface/plLayerAnimation.h>
#include <QLineEdit>
#include "../QObjLink.h"

class QLayerSDLAnimation : public QCreatable {
    Q_OBJECT

protected:
    QCreatableLink* fLayerAnimLink;
    QLineEdit* fVarName;

public:
    QLayerSDLAnimation(plCreatable* pCre, QWidget* parent = NULL);
    virtual void saveDamage();
};

#endif
