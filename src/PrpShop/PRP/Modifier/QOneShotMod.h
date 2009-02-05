#ifndef _QONESHOTMOD_H
#define _QONESHOTMOD_H

#include "../QCreatable.h"

#include <PRP/Modifier/plOneShotMod.h>
#include <QLineEdit>
#include <QCheckBox>
#include <QDoubleSpinBox>
#include "../QObjLink.h"

class QOneShotMod : public QCreatable {
    Q_OBJECT

protected:
    QCreatableLink* fSynchObjLink;
    QLineEdit* fAnimName;
    QCheckBox* fDrivable;
    QCheckBox* fReversable;
    QCheckBox* fSmartSeek;
    QCheckBox* fNoSeek;
    QDoubleSpinBox* fSeekDuration;

public:
    QOneShotMod(plCreatable* pCre, QWidget* parent = NULL);
    virtual void saveDamage();
};

#endif
