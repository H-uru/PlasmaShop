#ifndef _QSCENENODE_H
#define _QSCENENODE_H

#include "QCreatable.h"

#include <PRP/plSceneNode.h>
#include "QKeyList.h"

class QSceneNode : public QCreatable {
    Q_OBJECT

protected:
    QKeyList* fSceneKeys;
    QKeyList* fPoolKeys;

public:
    QSceneNode(plCreatable* pCre, QWidget* parent = NULL);
    virtual void saveDamage();
};

#endif
