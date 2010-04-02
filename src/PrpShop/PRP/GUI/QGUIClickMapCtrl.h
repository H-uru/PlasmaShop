#ifndef _QGUICLICKMAPCTRL_H
#define _QGUICLICKMAPCTRL_H

#include "../QCreatable.h"

#include <PRP/GUI/pfGUIMisc.hpp>
#include <QCheckBox>
#include "../QObjLink.h"

class QGUIClickMapCtrl : public QCreatable {
    Q_OBJECT

protected:
    enum {
        kModFlagStart = pfGUIClickMapCtrl::kReportDragging,
        kModFlagEnd = pfGUIClickMapCtrl::kReportHovering,
        kModFlagCount = (kModFlagEnd - kModFlagStart) + 1
    };

    QCreatableLink* fControlModLink;
    QCheckBox* fModFlags[kModFlagCount];

public:
    QGUIClickMapCtrl(plCreatable* pCre, QWidget* parent = NULL);
    virtual void saveDamage();
};

#endif
