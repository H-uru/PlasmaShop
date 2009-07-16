#ifndef _QSCENEOBJ_PREVIEW_H
#define _QSCENEOBJ_PREVIEW_H

#include "../QCreatable.h"
#include "QPlasmaRender.h"

class QSceneObj_Preview : public QCreatable {
    Q_OBJECT

protected:
    QPlasmaRender* fRender;

public:
    QSceneObj_Preview(plCreatable* pCre, QWidget* parent = NULL);
    virtual void saveDamage();
};

#endif
