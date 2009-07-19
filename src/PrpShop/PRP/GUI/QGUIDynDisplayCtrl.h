#ifndef _QGUIDYNDISPLAYCTRL_H
#define _QGUIDYNDISPLAYCTRL_H

#include "../QCreatable.h"

#include <PRP/GUI/pfGUIDynDisplayCtrl.h>
#include "../QObjLink.h"
#include "../QKeyList.h"

class QGUIDynDisplayCtrl : public QCreatable {
    Q_OBJECT

protected:
    QCreatableLink* fControlModLink;
    QKeyList* fTextMaps;
    QKeyList* fLayers;
    QKeyList* fMaterials;

public:
    QGUIDynDisplayCtrl(plCreatable* pCre, QWidget* parent = NULL);
    virtual void saveDamage();
};

#endif
