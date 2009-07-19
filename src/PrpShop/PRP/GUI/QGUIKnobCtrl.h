#ifndef _QGUIKNOBCTRL_H
#define _QGUIKNOBCTRL_H

#include "../QCreatable.h"

#include <PRP/GUI/pfGUIKnobCtrl.h>
#include <QCheckBox>
#include "../QObjLink.h"
#include "../QKeyList.h"

class QGUIKnobCtrl : public QCreatable {
    Q_OBJECT

protected:
    enum {
        kModFlagStart = pfGUIKnobCtrl::kReverseValues,
        kModFlagEnd = pfGUIKnobCtrl::kMapToAnimationRange,
        kModFlagCount = (kModFlagEnd - kModFlagStart) + 1
    };

    QCreatableLink* fControlModLink;
    QCheckBox* fModFlags[kModFlagCount];
    QKeyList* fAnimationKeys;
    QLineEdit* fAnimName;
    QFloatEdit* fMin;
    QFloatEdit* fMax;
    QFloatEdit* fStep;
    QFloatEdit* fStartPos[3];
    QFloatEdit* fEndPos[3];

public:
    QGUIKnobCtrl(plCreatable* pCre, QWidget* parent = NULL);
    virtual void saveDamage();
};

#endif
