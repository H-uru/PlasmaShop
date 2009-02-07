#include "QNumerics.h"
#include <QDoubleValidator>
#include <QIntValidator>

/* QFloatEdit */
QFloatEdit::QFloatEdit(QWidget* parent)
          : QLineEdit(parent)
{
    setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed, QSizePolicy::LineEdit));
    setValue(0.0);
    setValidator(new QDoubleValidator(this));
}

QSize QFloatEdit::sizeHint() const
{
    QFontMetrics metrics(fontMetrics());
    return QSize(metrics.width("-0123456789") + 2, QLineEdit::sizeHint().height());
}

QSize QFloatEdit::minimumSizeHint() const
{
    QFontMetrics metrics(fontMetrics());
    return QSize(metrics.width("-1234") + 2, QLineEdit::sizeHint().height());
}

void QFloatEdit::setRange(double minimum, double maximum, int decimals)
{
    ((QDoubleValidator*)validator())->setRange(minimum, maximum, decimals);
}

double QFloatEdit::value() const
{
    return text().toDouble();
}

void QFloatEdit::setValue(double value)
{
    setText(QString("%1").arg(value));
}


/* QIntEdit */
QIntEdit::QIntEdit(QWidget* parent)
        : QLineEdit(parent)
{
    setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed, QSizePolicy::LineEdit));
    setValue(0);
    setValidator(new QIntValidator(this));
}

QSize QIntEdit::sizeHint() const
{
    QFontMetrics metrics(fontMetrics());
    return QSize(metrics.width("-0123456789") + 2, QLineEdit::sizeHint().height());
}

QSize QIntEdit::minimumSizeHint() const
{
    QFontMetrics metrics(fontMetrics());
    return QSize(metrics.width("-1234") + 2, QLineEdit::sizeHint().height());
}

void QIntEdit::setRange(int minimum, int maximum)
{
    ((QIntValidator*)validator())->setRange(minimum, maximum);
}

int QIntEdit::value() const
{
    return text().toInt();
}

void QIntEdit::setValue(int value)
{
    setText(QString("%1").arg(value));
}
