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

QColor QColorBox::color() const
{ return fColor; }

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
    painter.end();
}


/* QColorEdit */
QColorEdit::QColorEdit(QWidget* parent)
          : QWidget(parent)
{
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(4);

    fColorBox = new QColorBox(this);
    fColorBox->setMinimumSize(24, 24);
    fColorBox->setMaximumSize(24, 24);
    fAlphaBox = new QDoubleSpinBox(this);
    fAlphaBox->setDecimals(3);
    fAlphaBox->setRange(0.0, 1.0);
    fAlphaBox->setSingleStep(0.005);
    layout->addWidget(fColorBox);
    layout->addWidget(new QLabel(tr("A:"), this));
    layout->addWidget(fAlphaBox);

    QObject::connect(fColorBox, SIGNAL(activated()), this, SLOT(selectColor()));
    QObject::connect(fAlphaBox, SIGNAL(valueChanged(double)), this, SLOT(setAlpha(double)));
    setColor(fColor);
}

void QColorEdit::setColor(hsColorRGBA color)
{
    fColor = color;
    QColor qtcol;
    qtcol.setRgbF(fColor.r, fColor.g, fColor.b);
    fColorBox->setColor(qtcol);
    fAlphaBox->setValue(fColor.a);
}

hsColorRGBA QColorEdit::color() const
{ return fColor; }

void QColorEdit::selectColor()
{
    QColor qtcol;
    qtcol.setRgbF(fColor.r, fColor.g, fColor.b);
    QColor col = QColorDialog::getColor(qtcol, this);
    if (col.isValid())
        setColor(hsColorRGBA(col.redF(), col.greenF(), col.blueF(), fColor.a));
}

void QColorEdit::setAlpha(double alpha)
{
    fColor.a = alpha;
}
