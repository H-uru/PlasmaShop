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

#include "QTrackball.h"

#include <cmath>

void QTrackball::push(const QPointF& p)
{
    fLastPos = p;

    fLastPos3D = QVector3D(p.x(), p.y(), 0.0f);
    float sqrZ = 1 - QVector3D::dotProduct(fLastPos3D, fLastPos3D);

    if (sqrZ > 0)
        fLastPos3D.setZ(sqrt(sqrZ));
    else
        fLastPos3D.normalize();

    fLastRotation = fRotation;
}

void QTrackball::move(const QPointF& p)
{
    QVector3D currentPos3D = QVector3D(p.x(), p.y(), 0.0f);
    float sqrZ = 1 - QVector3D::dotProduct(currentPos3D, currentPos3D);
    if (sqrZ > 0)
        currentPos3D.setZ(sqrt(sqrZ));
    else
        currentPos3D.normalize();

    QVector3D axis = QVector3D::crossProduct(fLastPos3D, currentPos3D);
    float angle = (180 / M_PI) * 2 * acos(QVector3D::dotProduct(fLastPos3D, currentPos3D));

    axis.normalize();
    QQuaternion rotation = QQuaternion::fromAxisAndAngle(axis, angle);

    QMatrix4x4 rotationM;
    rotationM.rotate(rotation);

    fRotation = rotationM * fLastRotation;

}
