#ifndef _QGUILISTBOXMOD_H
#define _QGUILISTBOXMOD_H

#include "../QCreatable.h"

#include <PRP/GUI/pfGUIListBoxMod.h>
#include <QCheckBox>
#include "../QObjLink.h"

class QGUIListBoxMod : public QCreatable {
    Q_OBJECT

protected:
    enum {
        kModFlagStart = pfGUIListBoxMod::kSingleSelect,
        kModFlagEnd = pfGUIListBoxMod::kForbidNoSelection,
        kModFlagCount = (kModFlagEnd - kModFlagStart) + 1
    };

    QCreatableLink* fControlModLink;
    QCheckBox* fModFlags[kModFlagCount];
    QCreatableLink* fScrollCtrl;

public:
    QGUIListBoxMod(plCreatable* pCre, QWidget* parent = NULL);
    virtual void saveDamage();

private slots:
    void setScrollCtrl();
    void unsetScrollCtrl();
};

#endif
