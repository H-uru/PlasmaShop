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
