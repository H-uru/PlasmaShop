#ifndef _QGUISKIN_H
#define _QGUISKIN_H

#include "../QCreatable.h"

#include <PRP/GUI/pfGUISkin.h>
#include <QComboBox>
#include "../QObjLink.h"

class QGUISkin : public QCreatable {
    Q_OBJECT

protected:
    QCreatableLink* fTexture;
    QIntEdit* fItemMargin;
    QIntEdit* fBorderMargin;
    QComboBox* fElements;
    QIntEdit* fX;
    QIntEdit* fY;
    QIntEdit* fWidth;
    QIntEdit* fHeight;
    int fCurElement;

public:
    QGUISkin(plCreatable* pCre, QWidget* parent = NULL);
    virtual void saveDamage();

private slots:
    void setTexture();
    void unsetTexture();
    void setElement(int which);

private:
    void saveElement();
};

#endif
