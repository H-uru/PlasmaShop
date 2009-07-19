#ifndef _QGUIMULTILINEEDITCTRL_H
#define _QGUIMULTILINEEDITCTRL_H

#include "../QCreatable.h"

#include <PRP/GUI/pfGUIMultiLineEditCtrl.h>
#include "../QObjLink.h"

class QGUIMultiLineEditCtrl : public QCreatable {
    Q_OBJECT

protected:
    QCreatableLink* fControlModLink;
    QCreatableLink* fScrollCtrl;

public:
    QGUIMultiLineEditCtrl(plCreatable* pCre, QWidget* parent = NULL);
    virtual void saveDamage();

private slots:
    void setScrollCtrl();
    void unsetScrollCtrl();
};

#endif
