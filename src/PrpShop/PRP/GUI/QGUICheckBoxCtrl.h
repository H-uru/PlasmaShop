#ifndef _QGUICHECKBOXCTRL_H
#define _QGUICHECKBOXCTRL_H

#include "../QCreatable.h"

#include <PRP/GUI/pfGUICheckBoxCtrl.h>
#include <QCheckBox>
#include "../QObjLink.h"
#include "../QKeyList.h"

class QGUICheckBoxCtrl : public QCreatable {
    Q_OBJECT

protected:
    QCreatableLink* fControlModLink;
    QKeyList* fAnimationKeys;
    QLineEdit* fAnimName;
    QCheckBox* fValue;

public:
    QGUICheckBoxCtrl(plCreatable* pCre, QWidget* parent = NULL);
    virtual void saveDamage();
};

#endif
