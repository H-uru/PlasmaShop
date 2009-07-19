#ifndef _QGUIRADIOGROUPCTRL_H
#define _QGUIRADIOGROUPCTRL_H

#include "../QCreatable.h"

#include <PRP/GUI/pfGUIRadioGroupCtrl.h>
#include <QCheckBox>
#include <QComboBox>
#include "../QObjLink.h"
#include "../QKeyList.h"

class QGUIRadioGroupCtrl : public QCreatable {
    Q_OBJECT

protected:
    QCreatableLink* fControlModLink;
    QCheckBox* fModFlagAllowNoSelection;
    QKeyList* fControls;
    QComboBox* fDefaultValue;

public:
    QGUIRadioGroupCtrl(plCreatable* pCre, QWidget* parent = NULL);
    virtual void saveDamage();

private slots:
    void controlAdded(plKey ctrl);
    void controlRemoved(int which);
};

#endif
