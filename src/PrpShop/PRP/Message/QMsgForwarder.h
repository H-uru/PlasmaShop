#ifndef _QMSGFORWARDER_H
#define _QMSGFORWARDER_H

#include "../QCreatable.h"

#include <PRP/Message/plMsgForwarder.h>
#include "../QKeyList.h"

class QMsgForwarder : public QCreatable {
protected:
    Q_OBJECT
    QKeyList* fForwardKeys;

public:
    QMsgForwarder(plCreatable* pCre, QWidget* parent = NULL);
    virtual void saveDamage();
};

#endif
