#ifndef _QMULTISTAGEBEHMOD_H
#define _QMULTISTAGEBEHMOD_H

#include "../QCreatable.h"

#include <PRP/Avatar/plMultistageBehMod.h>
#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include "../QObjLink.h"
#include "../QKeyList.h"

class QAnimStage : public QCreatable {
    Q_OBJECT

protected:
    enum {
        kNotifyEnter, kNotifyLoop, kNotifyAdvance, kNotifyRegress,
        kNumNotifyFlags
    };
    QCheckBox* fNotify[kNumNotifyFlags];

    QComboBox* fForwardType;
    QComboBox* fBackType;
    QComboBox* fAdvanceType;
    QComboBox* fRegressType;
    QLineEdit* fAnimName;
    QIntEdit* fLoops;
    QCheckBox* fDoAdvance;
    QCheckBox* fDoRegress;
    QIntEdit* fAdvanceTo;
    QIntEdit* fRegressTo;

public:
    QAnimStage(plCreatable* pCre, QWidget* parent = NULL);
    virtual void saveDamage();
};

class QMultistageBehMod : public QCreatable {
    Q_OBJECT

protected:
    QCreatableLink* fSynchObjLink;
    QTreeWidget* fStages;
    QCheckBox* fFreezePhys;
    QCheckBox* fSmartSeek;
    QCheckBox* fReverseControls;
    QKeyList* fReceivers;

public:
    QMultistageBehMod(plCreatable* pCre, QWidget* parent = NULL);
    virtual void saveDamage();

protected slots:
    void stageContextMenu(const QPoint& pos);
    void activateStageItem(QTreeWidgetItem* item, int column);
};

#endif
