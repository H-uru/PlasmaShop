#ifndef _QCREATABLE_H
#define _QCREATABLE_H

#include <QMdiSubWindow>
#include <ResManager/plResManager.h>

class QCreatable : public QMdiSubWindow {
protected:
    plCreatable* fCreatable;

public:
    QCreatable(plCreatable* pCre, QWidget* parent = NULL);

    bool isMatch(plCreatable* pCre);
};

#endif
