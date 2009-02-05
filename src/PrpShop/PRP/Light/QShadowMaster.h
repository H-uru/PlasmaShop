#ifndef _QSHADOWMASTER_H
#define _QSHADOWMASTER_H

#include "../QCreatable.h"

#include <PRP/Light/plShadowMaster.h>
#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include "../QObjLink.h"

class QShadowMaster : public QCreatable {
    Q_OBJECT

protected:
    QCreatableLink* fOwnerLink;
    QCreatableLink* fSynchObjLink;
    QCheckBox* fProperties[plShadowMaster::kNumProps];
    QDoubleSpinBox* fAttenDist;
    QDoubleSpinBox* fMinDist;
    QDoubleSpinBox* fMaxDist;
    QDoubleSpinBox* fPower;
    QSpinBox* fMinSize;
    QSpinBox* fMaxSize;

public:
    QShadowMaster(plCreatable* pCre, QWidget* parent = NULL);
    virtual void saveDamage();

protected slots:
    void setOwner();
    void unsetOwner();
};

#endif
