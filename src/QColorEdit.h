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

#ifndef _QCOLOREDIT_H
#define _QCOLOREDIT_H

#include <QWidget>
#include <QDoubleSpinBox>
#include <Sys/hsColor.h>

class QColorBox : public QWidget {
    Q_OBJECT

protected:
    QColor fColor;
    bool fInClick;

public:
    QColorBox(QWidget* parent = NULL);
    void setColor(QColor col);
    QColor color() const { return fColor; }

protected:
    virtual void mousePressEvent(QMouseEvent*);
    virtual void mouseReleaseEvent(QMouseEvent*);
    virtual void paintEvent(QPaintEvent*);

signals:
    void activated();
};

class QColorEdit : public QWidget {
    Q_OBJECT

protected:
    QColorBox* fColorBox;
    QDoubleSpinBox* fAlphaBox;
    hsColorRGBA fColor;

public:
    QColorEdit(bool doAlpha, QWidget* parent = NULL);
    void setColor(hsColorRGBA color);
    hsColorRGBA color() const { return fColor; }

protected slots:
    void selectColor();
    void setAlpha(double alpha) { fColor.a = alpha; }
};

#endif
