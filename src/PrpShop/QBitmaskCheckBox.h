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

#ifndef _QBITMASKCHECKBOX_H
#define _QBITMASKCHECKBOX_H

#include <QCheckBox>

class QBitmaskCheckBox : public QCheckBox
{
    Q_OBJECT

public:
    QBitmaskCheckBox(unsigned int value, const QString& text, QWidget* parent = nullptr);

    void setFrom(unsigned int value)
    {
        setChecked((value & fBitMask) == fBitMask);
    }

signals:
    void setBits(unsigned int mask);
    void unsetBits(unsigned int mask);

private:
    unsigned int fBitMask;
};

#endif // _QBITMASKCHECKBOX_H
