#ifndef _QLAYERLINKANIMATION_H
#define _QLAYERLINKANIMATION_H

#include "../QCreatable.h"

#include <PRP/Surface/plLayerAnimation.h>
#include <QCheckBox>
#include "../QObjLink.h"

class QLayerLinkAnimation : public QCreatable {
    Q_OBJECT

protected:
    QCreatableLink* fLayerAnimLink;
    QCreatableLink* fLinkKey;
    QCheckBox* fLeavingAge;

public:
    QLayerLinkAnimation(plCreatable* pCre, QWidget* parent = NULL);
    virtual void saveDamage();

protected slots:
    void setLinkKey();
    void unsetLinkKey();
};

#endif
