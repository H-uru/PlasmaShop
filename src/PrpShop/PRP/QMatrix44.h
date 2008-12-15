#ifndef _QMATRIX44_H
#define _QMATRIX44_H

#include <QWidget>
#include <QLineEdit>
#include <Math/hsMatrix44.h>

class QMatrix44 : public QWidget {
protected:
    Q_OBJECT
    QLineEdit* fMatEdit[4][4];

public:
    QMatrix44(QWidget* parent = NULL);
    virtual QSize sizeHint() const;

    hsMatrix44 value() const;
    void setValue(const hsMatrix44& val);
};

#endif
