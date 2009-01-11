#include "QLinkLabel.h"

/* QLinkLabel */
QLinkLabel::QLinkLabel(QWidget* parent)
          : QLabel(parent), fInClick(false)
{ init(); }

QLinkLabel::QLinkLabel(const QString& text, QWidget* parent)
          : QLabel(text, parent), fInClick(false)
{ init(); }

void QLinkLabel::init()
{
    QFont lblFont = font();
    lblFont.setUnderline(true);
    setFont(lblFont);

    QPalette lblPal = palette();
    lblPal.setColor(QPalette::Active, QPalette::WindowText, QColor(0, 0, 255));
    lblPal.setColor(QPalette::Inactive, QPalette::WindowText, QColor(0, 0, 255));
    setPalette(lblPal);

    setCursor(Qt::PointingHandCursor);
}

void QLinkLabel::mousePressEvent(QMouseEvent* evt)
{
    fInClick = (evt->button() == Qt::LeftButton && rect().contains(evt->pos()));
}

void QLinkLabel::mouseReleaseEvent(QMouseEvent* evt)
{
    if (!fInClick) return;
    fInClick = false;

    if (evt->button() == Qt::LeftButton && rect().contains(evt->pos()))
        emit activated();
}
