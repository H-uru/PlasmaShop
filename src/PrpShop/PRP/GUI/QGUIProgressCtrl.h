#ifndef _QGUIPROGRESSCTRL_H
#define _QGUIPROGRESSCTRL_H

#include "../QCreatable.h"

#include <PRP/GUI/pfGUIProgressCtrl.h>
#include <QCheckBox>
#include "../QObjLink.h"
#include "../QKeyList.h"

class QGUIProgressCtrl : public QCreatable {
    Q_OBJECT

protected:
    enum {
        kModFlagStart = pfGUIProgressCtrl::kReverseValues,
        kModFlagEnd = pfGUIProgressCtrl::kMapToAnimationRange,
        kModFlagCount = (kModFlagEnd - kModFlagStart) + 1
    };

    QCreatableLink* fControlModLink;
    QCheckBox* fModFlags[kModFlagCount];
    QKeyList* fAnimationKeys;
    QLineEdit* fAnimName;
    QFloatEdit* fMin;
    QFloatEdit* fMax;
    QFloatEdit* fStep;

public:
    QGUIProgressCtrl(plCreatable* pCre, QWidget* parent = NULL);
    virtual void saveDamage();
};

#endif
