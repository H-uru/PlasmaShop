#ifndef _QGUIMENUITEM_H
#define _QGUIMENUITEM_H

#include "../QCreatable.h"

#include <PRP/GUI/pfGUIButtonMod.h>
#include <QCheckBox>
#include "../QObjLink.h"

class QGUIMenuItem : public QCreatable {
    Q_OBJECT

protected:
    enum {
        kModFlagStart = pfGUIMenuItem::kDrawSubMenuArrow,
        kModFlagEnd = pfGUIMenuItem::kReportHovers,
        kModFlagCount = (kModFlagEnd - kModFlagStart) + 1
    };

    QCreatableLink* fButtonModLink;
    QCheckBox* fModFlags[kModFlagCount];

public:
    QGUIMenuItem(plCreatable* pCre, QWidget* parent = NULL);
    virtual void saveDamage();
};

#endif
