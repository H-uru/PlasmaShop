#ifndef _QCREATABLE_H
#define _QCREATABLE_H

#include <QWidget>
#include <ResManager/plResManager.h>

class QCreatable : public QWidget {
protected:
    plCreatable* fCreatable;

public:
    QCreatable(plCreatable* pCre, QWidget* parent = NULL);
    bool isMatch(plCreatable* pCre, short forceType = -1);
    virtual void saveDamage() = 0;

protected:
    virtual void closeEvent(QCloseEvent* evt);
};

QCreatable* pqMakeCreatableForm(plCreatable* pCre, QWidget* parent, short forceType = -1);

#endif
