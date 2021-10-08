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

class QColorBox : public QWidget
{
    Q_OBJECT

protected:
    QColor fColor;
    bool fInClick;

public:
    QColorBox(QWidget* parent = Q_NULLPTR);
    void setColor(QColor col);
    QColor color() const { return fColor; }

protected:
    void mousePressEvent(QMouseEvent*) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent*) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent*) Q_DECL_OVERRIDE;

signals:
    void activated();
};

class QColorEdit : public QWidget
{
    Q_OBJECT

protected:
    QColorBox* fColorBox;
    QDoubleSpinBox* fAlphaBox;
    hsColorRGBA fColor;

public:
    QColorEdit(bool doAlpha, QWidget* parent = Q_NULLPTR);
    void setColor(hsColorRGBA color);
    hsColorRGBA color() const { return fColor; }

protected slots:
    void selectColor();
};

#endif
