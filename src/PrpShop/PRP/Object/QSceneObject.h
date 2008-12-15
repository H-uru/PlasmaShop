#ifndef _QSCENEOBJECT_H
#define _QSCENEOBJECT_H

#include "../QCreatable.h"

#include <PRP/Object/plSceneObject.h>
#include "../QKeyList.h"
#include "../QObjLink.h"

class QSceneObject : public QCreatable {
protected:
    Q_OBJECT
    QCreatableLink* fSynchObjLink;
    QCreatableLink* fDrawIntfLink;
    QCreatableLink* fSimIntfLink;
    QCreatableLink* fCoordIntfLink;
    QCreatableLink* fAudioIntfLink;
    QKeyList* fInterfaceList;
    QKeyList* fModifierList;

public:
    QSceneObject(plCreatable* pCre, QWidget* parent = NULL);
    virtual void saveDamage();

protected slots:
    void setDraw();
    void setSim();
    void setCoord();
    void setAudio();
    void unsetDraw();
    void unsetSim();
    void unsetCoord();
    void unsetAudio();
};

#endif
