#ifndef _QGUITEXTBOXMOD_H
#define _QGUITEXTBOXMOD_H

#include "../QCreatable.h"

#include <PRP/GUI/pfGUITextBoxMod.h>
#include <QCheckBox>
#include <QTextEdit>
#include "../QObjLink.h"

class QGUITextBoxMod : public QCreatable {
    Q_OBJECT

protected:
    enum {
        kModFlagStart = pfGUITextBoxMod::kCenterJustify,
        kModFlagEnd = pfGUITextBoxMod::kRightJustify,
        kModFlagCount = (kModFlagEnd - kModFlagStart) + 1
    };

    QCreatableLink* fControlModLink;
    QCheckBox* fModFlags[kModFlagCount];
    QTextEdit* fText;
    QLineEdit* fLocalizationPath;

public:
    QGUITextBoxMod(plCreatable* pCre, QWidget* parent = NULL);
    virtual void saveDamage();
};

#endif
