#ifndef _QSYNCHEDOBJECT_H
#define _QSYNCHEDOBJECT_H

#include "../QCreatable.h"

#include <PRP/Object/plCoordinateInterface.h>
#include <QCheckBox>
#include "../QKeyList.h"

class QSynchedObject : public QCreatable {
protected:
    enum {
        kCbDontDirty, kCbSendReliably, kCbHasConstantNetGroup,
        kCbDontSynchGameMessages, kCbExcludePersistentState,
        kCbExcludeAllPersistentState, kCbHasVolatileState,
        kCbAllStateIsVolatile, kNumCbFlags
    };

    Q_OBJECT
    QCheckBox* fCBFlags[kNumCbFlags];
    QStringListWidget* fExcludeList;
    QStringListWidget* fVolatileList;

public:
    QSynchedObject(plCreatable* pCre, QWidget* parent = NULL);
    virtual void saveDamage();
};

#endif
