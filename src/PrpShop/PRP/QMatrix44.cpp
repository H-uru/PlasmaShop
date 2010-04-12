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

#include "QMatrix44.h"

#include <QLabel>
#include <QGridLayout>

QMatrix44::QMatrix44(QWidget* parent) : QWidget(parent)
{
    QGridLayout* layout = new QGridLayout(this);
    layout->addWidget(new QLabel(tr("cX"), this), 0, 1);
    layout->addWidget(new QLabel(tr("cY"), this), 0, 2);
    layout->addWidget(new QLabel(tr("cZ"), this), 0, 3);
    layout->addWidget(new QLabel(tr("Pos"), this), 0, 4);
    layout->addWidget(new QLabel(tr("X"), this), 1, 0);
    layout->addWidget(new QLabel(tr("Y"), this), 2, 0);
    layout->addWidget(new QLabel(tr("Z"), this), 3, 0);

    for (size_t i=0; i<4; i++) {
        for (size_t j=0; j<4; j++) {
            fMatEdit[i][j] = new QFloatEdit(this);
            fMatEdit[i][j]->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed));
            layout->addWidget(fMatEdit[i][j], i+1, j+1);
        }
    }
}

hsMatrix44 QMatrix44::value() const
{
    hsMatrix44 mat;
    for (size_t i=0; i<4; i++)
        for (size_t j=0; j<4; j++)
            mat(i, j) = fMatEdit[i][j]->value();
    return mat;
}

void QMatrix44::setValue(const hsMatrix44& val)
{
    for (size_t i=0; i<4; i++)
        for (size_t j=0; j<4; j++)
            fMatEdit[i][j]->setValue(val(i, j));
}
