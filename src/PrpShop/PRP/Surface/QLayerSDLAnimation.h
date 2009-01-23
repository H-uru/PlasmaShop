#ifndef _QLAYERSDLANIMATION_H
#define _QLAYERSDLANIMATION_H

#include "../QCreatable.h"

#include <PRP/Surface/plLayerAnimation.h>
#include <QLineEdit>
#include "../QObjLink.h"
#include "../QColorEdit.h"
#include "../QMatrix44.h"

class QLayerSDLAnimation : public QCreatable {
    Q_OBJECT

protected:
    QCreatableLink* fSynchObjLink;
    QCreatableLink* fBaseLayer;
    QControllerLink* fPreshade;
    QControllerLink* fRuntime;
    QControllerLink* fAmbient;
    QControllerLink* fSpecular;
    QControllerLink* fOpacity;
    QControllerLink* fTransform;
    QLineEdit* fVarName;

public:
    QLayerSDLAnimation(plCreatable* pCre, QWidget* parent = NULL);
    virtual void saveDamage();

protected slots:
    void setBaseLayer();
    void unsetBaseLayer();

    void newPreshade(short type);
    void newRuntime(short type);
    void newAmbient(short type);
    void newSpecular(short type);
    void newOpacity(short type);
    void newTransform(short type);
    void delPreshade();
    void delRuntime();
    void delAmbient();
    void delSpecular();
    void delOpacity();
    void delTransform();
};

#endif
