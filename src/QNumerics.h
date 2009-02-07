#ifndef _QNUMERICS_H
#define _QNUMERICS_H

#include <QLineEdit>

class QFloatEdit : public QLineEdit {
    Q_OBJECT

public:
    QFloatEdit(QWidget* parent = 0);
    virtual QSize sizeHint() const;
    virtual QSize minimumSizeHint() const;

    void setRange(double minimum, double maximum, int decimals = 0);
    double value() const;

public slots:
    void setValue(double value);
};

class QIntEdit : public QLineEdit {
    Q_OBJECT

public:
    QIntEdit(QWidget* parent = 0);
    virtual QSize sizeHint() const;
    virtual QSize minimumSizeHint() const;

    void setRange(int minimum, int maximum);
    int value() const;

public slots:
    void setValue(int value);
};

#endif
