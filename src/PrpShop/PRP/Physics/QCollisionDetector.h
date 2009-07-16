#ifndef _QCOLLISIONDETECTOR_H
#define _QCOLLISIONDETECTOR_H

#include "../QCreatable.h"

#include <PRP/Physics/plCollisionDetector.h>
#include <QCheckBox>
#include "../QObjLink.h"

class QCollisionDetector : public QCreatable {
    Q_OBJECT

protected:
    QCreatableLink* fDetectorLink;
    QCheckBox* fBoolParam;
    QCreatableLink* fSubworld;

    enum {
        kCBTypeEnter, kCBTypeExit, kCBTypeAny, kCBTypeUnEnter, kCBTypeUnExit,
        kCBTypeBump, kNumTypeFlags
    };
    QCheckBox* fTypeFlags[kNumTypeFlags];

public:
    QCollisionDetector(plCreatable* pCre, QWidget* parent = NULL);
    virtual void saveDamage();

protected slots:
    void setSubworld();
    void unsetSubworld();
};

#endif
