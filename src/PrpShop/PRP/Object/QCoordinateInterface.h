#ifndef _QCOORDINATEINTERFACE_H
#define _QCOORDINATEINTERFACE_H

#include "../QCreatable.h"

#include <PRP/Object/plCoordinateInterface.h>
#include <QCheckBox>
#include "../QKeyList.h"
#include "../QMatrix44.h"
#include "../QObjLink.h"

class QCoordinateInterface : public QCreatable {
    Q_OBJECT

protected:
    QCreatableLink* fOwnerLink;
    QCreatableLink* fSynchObjLink;
    QCheckBox* fCBProperties[plCoordinateInterface::kNumProps];
    QKeyList* fChildren;
    QMatrix44* fLocalToParent;
    QMatrix44* fParentToLocal;
    QMatrix44* fLocalToWorld;
    QMatrix44* fWorldToLocal;

public:
    QCoordinateInterface(plCreatable* pCre, QWidget* parent = NULL);
    virtual void saveDamage();

protected slots:
    void setOwner();
    void unsetOwner();
};

#endif
