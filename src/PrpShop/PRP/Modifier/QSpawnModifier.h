#ifndef _QSPAWNMODIFIER_H
#define _QSPAWNMODIFIER_H

#include "../QCreatable.h"
#include "../QObjLink.h"

class QSpawnModifier : public QCreatable {
    Q_OBJECT

protected:
    QCreatableLink* fSynchObjLink;

public:
    QSpawnModifier(plCreatable* pCre, QWidget* parent = NULL);
    virtual void saveDamage();
};

#endif
