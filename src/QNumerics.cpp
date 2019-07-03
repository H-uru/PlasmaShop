/* This file is part of PlasmaShop.
 *
 * PlasmaShop is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PlasmaShop is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PlasmaShop.  If not, see <http://www.gnu.org/licenses/>.
 */

// Not ready to depend on Qt 5.11+ yet
#define QT_NO_DEPRECATED_WARNINGS

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
    return QSize(metrics.width("-1234567890") + 2, QLineEdit::sizeHint().height());
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
    return QSize(metrics.width("-1234567890") + 2, QLineEdit::sizeHint().height());
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
