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

#ifndef _QLINKLABEL_H
#define _QLINKLABEL_H

#include <QLabel>
#include <QMouseEvent>

class QLinkLabel : public QLabel
{
protected:
    Q_OBJECT
    bool fInClick;

public:
    QLinkLabel(QWidget* parent = 0)
        : QLabel(parent), fInClick(false) { init(); }
    QLinkLabel(const QString& text, QWidget* parent = Q_NULLPTR)
        : QLabel(text, parent), fInClick(false) { init(); }

protected:
    void init();
    void mousePressEvent(QMouseEvent* evt) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent* evt) Q_DECL_OVERRIDE;

signals:
    void activated();
};

#endif
