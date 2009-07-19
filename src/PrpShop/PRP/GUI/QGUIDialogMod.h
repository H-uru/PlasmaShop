#ifndef _QGUIDIALOGMOD_H
#define _QGUIDIALOGMOD_H

#include "../QCreatable.h"

#include <PRP/GUI/pfGUIDialogMod.h>
#include <QCheckBox>
#include "QGUIControlMod.h"
#include "../QObjLink.h"
#include "../QKeyList.h"

class QGUIDialogMod : public QCreatable {
    Q_OBJECT

protected:
    QCreatableLink* fSynchObjLink;
    QCheckBox* fModFlagModal;
    QIntEdit* fTagID;
    QIntEdit* fVersion;
    QLineEdit* fName;
    QCreatableLink* fRenderMod;
    QCreatableLink* fProcReceiver;
    QCreatableLink* fSceneNode;
    QGUIColorScheme* fColorScheme;
    QKeyList* fControls;

public:
    QGUIDialogMod(plCreatable* pCre, QWidget* parent = NULL);
    virtual void saveDamage();

protected slots:
    void setRenderMod();
    void setProcReceiver();
    void setSceneNode();
    void unsetRenderMod();
    void unsetProcReceiver();
    void unsetSceneNode();
};

#endif
