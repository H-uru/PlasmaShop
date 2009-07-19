#ifndef _QIMAGELIBMOD_H
#define _QIMAGELIBMOD_H

#include "../QCreatable.h"

#include <PRP/GUI/plImageLibMod.h>
#include "../QObjLink.h"
#include "../QKeyList.h"

class QImageLibMod : public QCreatable {
    Q_OBJECT

protected:
    QCreatableLink* fSynchObjLink;
    QKeyList* fImages;

public:
    QImageLibMod(plCreatable* pCre, QWidget* parent = NULL);
    virtual void saveDamage();
};

#endif
