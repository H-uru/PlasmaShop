#ifndef _QCREATABLE_H
#define _QCREATABLE_H

#include <QWidget>
#include <ResManager/plResManager.h>

class QCreatable : public QWidget {
protected:
    plCreatable* fCreatable;
    short fForceType;

public:
    QCreatable(plCreatable* pCre, short type, QWidget* parent = NULL);
    bool isMatch(plCreatable* pCre, short type);
    virtual void saveDamage() = 0;

protected:
    virtual void closeEvent(QCloseEvent* evt);
};

QCreatable* pqMakeCreatableForm(plCreatable* pCre, QWidget* parent, short forceType = -1);

#endif
