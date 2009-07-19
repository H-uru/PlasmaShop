#ifndef _QGUICONTROLMOD_H
#define _QGUICONTROLMOD_H

#include "../QCreatable.h"

#include <PRP/GUI/pfGUIControlMod.h>
#include <QCheckBox>
#include <QComboBox>
#include <QGroupBox>
#include "../QObjLink.h"
#include "../QKeyList.h"
#include "../QColorEdit.h"

class QGUIColorScheme : public QWidget {
    Q_OBJECT

private:
    QColorEdit* fForeColor;
    QColorEdit* fBackColor;
    QColorEdit* fSelForeColor;
    QColorEdit* fSelBackColor;
    QCheckBox* fTransparent;
    QIntEdit* fFontSize;
    QLineEdit* fFontFace;

    enum { kCBBold, kCBItalic, kCBShadowed, kNumFontFlags };
    QCheckBox* fFontFlags[kNumFontFlags];

public:
    QGUIColorScheme(QWidget* parent);
    void setColorScheme(pfGUIColorScheme* scheme);
    void saveColorScheme(pfGUIColorScheme* scheme);
};

class QGUIControlMod : public QCreatable {
    Q_OBJECT

protected:
    QCreatableLink* fSynchObjLink;
    QCheckBox* fModFlags[pfGUIControlMod::kBetterHitTesting + 1];
    QIntEdit* fTagID;
    QCheckBox* fVisible;
    QCreatableLink* fDynTextMap;
    QCreatableLink* fDynTextLayer;
    QIntListWidget* fSoundIndices;
    QCreatableLink* fProxy;
    QCreatableLink* fSkin;
    QComboBox* fProcType;
    QLineEdit* fProcCommand;
    QLabel* fProcCommandLabel;
    QGroupBox* fColorSchemeGrp;
    QGUIColorScheme* fColorScheme;

public:
    QGUIControlMod(plCreatable* pCre, QWidget* parent = NULL);
    virtual void saveDamage();

protected slots:
    void setDynTextMap();
    void setDynTextLayer();
    void setProxy();
    void setSkin();
    void unsetDynTextMap();
    void unsetDynTextLayer();
    void unsetProxy();
    void unsetSkin();

    void setProcType(int typeIdx);
};

#endif
