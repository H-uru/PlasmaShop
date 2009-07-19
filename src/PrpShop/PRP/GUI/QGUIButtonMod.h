#ifndef _QGUIBUTTONMOD_H
#define _QGUIBUTTONMOD_H

#include "../QCreatable.h"

#include <PRP/GUI/pfGUIButtonMod.h>
#include <QComboBox>
#include "../QObjLink.h"
#include "../QKeyList.h"

class QGUIButtonMod : public QCreatable {
    Q_OBJECT

protected:
    QCreatableLink* fControlModLink;
    QKeyList* fAnimationKeys;
    QKeyList* fMouseOverAnimKeys;
    QLineEdit* fAnimName;
    QLineEdit* fMouseOverAnimName;
    QComboBox* fNotifyType;
    QCreatableLink* fDraggable;

public:
    QGUIButtonMod(plCreatable* pCre, QWidget* parent = NULL);
    virtual void saveDamage();

protected slots:
    void setDraggable();
    void unsetDraggable();
};

#endif
