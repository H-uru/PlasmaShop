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

#ifndef QTRACKBALL_H_
#define QTRACKBALL_H_

#include <QtGui>

#include <QtGui/qvector3d.h>
#include <QtGui/qquaternion.h>

class QTrackball
{
public:
    QTrackball() { }
    virtual ~QTrackball() { }

    void push(const QPointF& p);
    void move(const QPointF& p);
    void release(const QPointF& /*p*/) { }

    QMatrix4x4 rotation() const { return fRotation; }

private:
    QMatrix4x4 fRotation;
    QMatrix4x4 fLastRotation;

    QPointF fLastPos;
    QVector3D fLastPos3D;

};

#endif /* QTRACKBALL_H_ */
