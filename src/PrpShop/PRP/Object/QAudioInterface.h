#ifndef _QAUDIOINTERFACE_H
#define _QAUDIOINTERFACE_H

#include "../QCreatable.h"

#include <PRP/Object/plAudioInterface.h>
#include <QCheckBox>
#include "../QObjLink.h"

class QAudioInterface : public QCreatable {
protected:
    Q_OBJECT
    QCreatableLink* fOwnerLink;
    QCreatableLink* fSynchObjLink;
    QCheckBox* fDisabled;
    QCreatableLink* fAudibleLink;

public:
    QAudioInterface(plCreatable* pCre, QWidget* parent = NULL);
    virtual void saveDamage();

protected slots:
    void setOwner();
    void unsetOwner();
    void setAudible();
    void unsetAudible();
};

#endif
