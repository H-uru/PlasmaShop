#ifndef _QGUIDRAGGABLEMOD_H
#define _QGUIDRAGGABLEMOD_H

#include "../QCreatable.h"

#include <PRP/GUI/pfGUIDraggableMod.h>
#include <QCheckBox>
#include "../QObjLink.h"

class QGUIDraggableMod : public QCreatable {
    Q_OBJECT

protected:
    enum {
        kModFlagStart = pfGUIDraggableMod::kReportDragging,
        kModFlagEnd = pfGUIDraggableMod::kAlwaysSnapBackToStart,
        kModFlagCount = (kModFlagEnd - kModFlagStart) + 1
    };

    QCreatableLink* fControlModLink;
    QCheckBox* fModFlags[kModFlagCount];

public:
    QGUIDraggableMod(plCreatable* pCre, QWidget* parent = NULL);
    virtual void saveDamage();
};

#endif
