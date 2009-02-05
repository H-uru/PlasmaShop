#ifndef _QAUDIBLE_H
#define _QAUDIBLE_H

#include "../QCreatable.h"
#include "../QKeyList.h"
#include "../QObjLink.h"

class QAudible : public QCreatable {
    Q_OBJECT

public:
    QAudible(plCreatable* pCre, QWidget* parent = NULL);
    virtual void saveDamage();
};

class QWinAudible : public QCreatable {
    Q_OBJECT

protected:
    QKeyList* fSoundKeys;
    QCreatableLink* fSceneNode;

public:
    QWinAudible(plCreatable* pCre, QWidget* parent = NULL);
    virtual void saveDamage();

protected slots:
    void setSceneNode();
    void unsetSceneNode();
};

#endif
