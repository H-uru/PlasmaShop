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

#include "QColorEdit.h"

#include <QLabel>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPaintEngine>
#include <QHBoxLayout>
#include <QColorDialog>

/* QColorBox */
QColorBox::QColorBox(QWidget* parent)
         : QWidget(parent)
{
    setColor(QColor());
}

void QColorBox::setColor(QColor col)
{
    fColor = col;
    update();
}

void QColorBox::mousePressEvent(QMouseEvent* evt)
{
    fInClick = (evt->button() == Qt::LeftButton && rect().contains(evt->pos()));
}

void QColorBox::mouseReleaseEvent(QMouseEvent* evt)
{
    if (!fInClick) return;
    fInClick = false;

    if (evt->button() == Qt::LeftButton && rect().contains(evt->pos()))
        emit activated();
}

void QColorBox::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.fillRect(rect(), QBrush(fColor));
    painter.setPen(QColor(0, 0, 0));
    painter.drawRect(QRect(rect().x(), rect().y(), rect().width()-1, rect().height()-1));
    painter.end();
}


/* QColorEdit */
QColorEdit::QColorEdit(bool doAlpha, QWidget* parent)
          : QWidget(parent)
{
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(4);

    fColorBox = new QColorBox(this);
    fColorBox->setMinimumSize(24, 24);
    //fColorBox->setMaximumSize(24, 24);
    layout->addWidget(fColorBox);
    connect(fColorBox, SIGNAL(activated()), this, SLOT(selectColor()));

    if (doAlpha) {
        fAlphaBox = new QDoubleSpinBox(this);
        fAlphaBox->setDecimals(3);
        fAlphaBox->setRange(0.0, 1.0);
        fAlphaBox->setSingleStep(0.005);
        layout->addWidget(new QLabel(tr("A:"), this));
        layout->addWidget(fAlphaBox);
        connect(fAlphaBox, SIGNAL(valueChanged(double)), this, SLOT(setAlpha(double)));
    } else {
        fAlphaBox = NULL;
    }

    setColor(fColor);
}

void QColorEdit::setColor(hsColorRGBA color)
{
    fColor = color;
    QColor qtcol;
    qtcol.setRgbF(fColor.r, fColor.g, fColor.b);
    fColorBox->setColor(qtcol);
    if (fAlphaBox != NULL)
        fAlphaBox->setValue(fColor.a);
}

void QColorEdit::selectColor()
{
    QColor qtcol;
    qtcol.setRgbF(fColor.r, fColor.g, fColor.b);
    QColor col = QColorDialog::getColor(qtcol, this);
    if (col.isValid())
        setColor(hsColorRGBA(col.redF(), col.greenF(), col.blueF(), fColor.a));
}
