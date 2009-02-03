#ifndef _QINTERFACEINFOMODIFIER_H
#define _QINTERFACEINFOMODIFIER_H

#include "../QCreatable.h"

#include <PRP/Modifier/plInterfaceInfoModifier.h>
#include "../QKeyList.h"
#include "../QObjLink.h"

class QInterfaceInfoModifier : public QCreatable {
    Q_OBJECT

protected:
    QCreatableLink* fSynchObjLink;
    QKeyList* fIntfKeys;

public:
    QInterfaceInfoModifier(plCreatable* pCre, QWidget* parent = NULL);
    virtual void saveDamage();
};

#endif
