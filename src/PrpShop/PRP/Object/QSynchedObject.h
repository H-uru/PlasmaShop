#ifndef _QSYNCHEDOBJECT_H
#define _QSYNCHEDOBJECT_H

#include "../QCreatable.h"

#include <PRP/Object/plCoordinateInterface.h>
#include <QCheckBox>
#include "../QKeyList.h"

class QSynchedObject : public QCreatable {
    Q_OBJECT

protected:
    enum {
        kCbDontDirty, kCbSendReliably, kCbHasConstantNetGroup,
        kCbDontSynchGameMessages, kCbExcludePersistentState,
        kCbExcludeAllPersistentState, kCbHasVolatileState,
        kCbAllStateIsVolatile, kNumCbFlags
    };

    QCheckBox* fCBFlags[kNumCbFlags];
    QStringListWidget* fExcludeList;
    QStringListWidget* fVolatileList;

public:
    QSynchedObject(plCreatable* pCre, QWidget* parent = NULL);
    virtual void saveDamage();
};

#endif
