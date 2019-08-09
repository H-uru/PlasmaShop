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

#ifndef _QGUISKIN_H
#define _QGUISKIN_H

#include "PRP/QCreatable.h"

#include <PRP/GUI/pfGUISkin.h>
#include <QComboBox>
#include "PRP/QObjLink.h"

class QGUISkin : public QCreatable
{
    Q_OBJECT

protected:
    QCreatableLink* fTexture;
    QIntEdit* fItemMargin;
    QIntEdit* fBorderMargin;
    QComboBox* fElements;
    QIntEdit* fX;
    QIntEdit* fY;
    QIntEdit* fWidth;
    QIntEdit* fHeight;
    int fCurElement;

public:
    QGUISkin(plCreatable* pCre, QWidget* parent = NULL);
    void saveDamage() override;

private slots:
    void setTexture();
    void unsetTexture();
    void setElement(int which);

private:
    void saveElement();
};

#endif
