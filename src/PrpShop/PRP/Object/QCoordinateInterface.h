#ifndef _QCOORDINATEINTERFACE_H
#define _QCOORDINATEINTERFACE_H

#include "../QCreatable.h"

#include <PRP/Object/plCoordinateInterface.h>
#include <QCheckBox>
#include "../QKeyList.h"

class QCoordinateInterface : public QCreatable {
protected:
    Q_OBJECT
    QCheckBox* fCBProperties[plCoordinateInterface::kNumProps];
    QKeyList* fChildren;

public:
    QCoordinateInterface(plCreatable* pCre, QWidget* parent = NULL);
    virtual void saveDamage();
};

#endif
