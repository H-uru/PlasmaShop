#ifndef _QFADEOPACITYMOD_H
#define _QFADEOPACITYMOD_H

#include "../QCreatable.h"
#include "../QObjLink.h"
#include <QImage>
#include <QCheckBox>
#include <QSpinBox>
#include <PRP/Surface/plFadeOpacityMod.h>

class QFadeOpacityMod : public QCreatable {
    Q_OBJECT

protected:
    QCreatableLink* fSynchObjLink;
    enum {
        kBoundsCenter, kNumBitmapFlags
    };
    QCheckBox* fFlags[kNumBitmapFlags];
    QFloatEdit* fUp;
    QFloatEdit* fDown;

public:
    QFadeOpacityMod(plCreatable* pCre, QWidget* parent = NULL);
    virtual void saveDamage();
};

#endif
