#ifndef _QMATRIX44_H
#define _QMATRIX44_H

#include <QWidget>
#include <Math/hsMatrix44.h>
#include "../../QNumerics.h"

class QMatrix44 : public QWidget {
    Q_OBJECT

protected:
    QFloatEdit* fMatEdit[4][4];

public:
    QMatrix44(QWidget* parent = NULL);

    hsMatrix44 value() const;
    void setValue(const hsMatrix44& val);
};

#endif
