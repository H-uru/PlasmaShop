#ifndef _QSIMULATIONINTERFACE_H
#define _QSIMULATIONINTERFACE_H

#include "../QCreatable.h"

#include <PRP/Object/plSimulationInterface.h>
#include <QCheckBox>
#include "../QObjLink.h"

class QSimulationInterface : public QCreatable {
protected:
    Q_OBJECT
    QCreatableLink* fOwnerLink;
    QCreatableLink* fSynchObjLink;
    QCheckBox* fCBProperties[plSimulationInterface::kNumProps];
    QCreatableLink* fPhysicalLink;

public:
    QSimulationInterface(plCreatable* pCre, QWidget* parent = NULL);
    virtual void saveDamage();

protected slots:
    void setOwner();
    void unsetOwner();
    void setPhysical();
    void unsetPhysical();
};

#endif
