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

#include "QLinkLabel.h"

/* QLinkLabel */
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
    if (!fInClick)
        return;
    fInClick = false;

    if (evt->button() == Qt::LeftButton && rect().contains(evt->pos()))
        emit activated();
}
