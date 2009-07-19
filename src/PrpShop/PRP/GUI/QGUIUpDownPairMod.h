#ifndef _QGUIUPDOWNPAIRMOD_H
#define _QGUIUPDOWNPAIRMOD_H

#include "../QCreatable.h"

#include <PRP/GUI/pfGUIUpDownPairMod.h>
#include "../QObjLink.h"

class QGUIUpDownPairMod : public QCreatable {
    Q_OBJECT

protected:
    QCreatableLink* fControlModLink;
    QFloatEdit* fMin;
    QFloatEdit* fMax;
    QFloatEdit* fStep;
    QCreatableLink* fUpCtrl;
    QCreatableLink* fDownCtrl;

public:
    QGUIUpDownPairMod(plCreatable* pCre, QWidget* parent = NULL);
    virtual void saveDamage();

private slots:
    void setUpCtrl();
    void setDownCtrl();
    void unsetUpCtrl();
    void unsetDownCtrl();
};

#endif
