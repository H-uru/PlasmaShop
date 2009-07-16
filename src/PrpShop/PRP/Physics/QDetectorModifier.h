#ifndef _QDETECTORMODIFIER_H
#define _QDETECTORMODIFIER_H

#include "../QCreatable.h"

#include <PRP/Physics/plDetectorModifier.h>
#include "../QObjLink.h"
#include "../QKeyList.h"

class QDetectorModifier : public QCreatable {
    Q_OBJECT

protected:
    QCreatableLink* fSynchObjLink;
    QKeyList* fReceivers;
    QCreatableLink* fRemoteMod;
    QCreatableLink* fProxy;

public:
    QDetectorModifier(plCreatable* pCre, QWidget* parent = NULL);
    virtual void saveDamage();

protected slots:
    void setRemoteMod();
    void setProxy();
    void unsetRemoteMod();
    void unsetProxy();
};

#endif
