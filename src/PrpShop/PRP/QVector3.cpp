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

#include "QVector3.h"

#include <QLabel>
#include <QGridLayout>

QVector3::QVector3(QWidget* parent) : QWidget(parent)
{
    QHBoxLayout * layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    for (size_t i=0; i<3; i++) {
        fVecEdit[i] = new QFloatEdit(this);
        fVecEdit[i]->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed));
    }

    layout->addWidget(new QLabel(tr("X:"), this));
    layout->addWidget(fVecEdit[0]);
    layout->addWidget(new QLabel(tr("Y:"), this));
    layout->addWidget(fVecEdit[1]);
    layout->addWidget(new QLabel(tr("Z:"), this));
    layout->addWidget(fVecEdit[2]);
}

hsVector3 QVector3::value() const
{
    hsVector3 vec;
    vec.X = fVecEdit[0]->value();
    vec.Y = fVecEdit[1]->value();
    vec.Z = fVecEdit[2]->value();
    return vec;
}

void QVector3::setValue(const hsVector3& val)
{
    fVecEdit[0]->setValue(val.X);
    fVecEdit[1]->setValue(val.Y);
    fVecEdit[2]->setValue(val.Z);
}
