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

#ifndef _QGUIBUTTONMOD_H
#define _QGUIBUTTONMOD_H

#include "PRP/QCreatable.h"

#include <PRP/GUI/pfGUIButtonMod.h>
#include <QComboBox>
#include "PRP/QObjLink.h"
#include "PRP/QKeyList.h"

class QGUIButtonMod : public QCreatable
{
    Q_OBJECT

protected:
    QCreatableLink* fControlModLink;
    QKeyList* fAnimationKeys;
    QKeyList* fMouseOverAnimKeys;
    QLineEdit* fAnimName;
    QLineEdit* fMouseOverAnimName;
    QComboBox* fNotifyType;
    QCreatableLink* fDraggable;

public:
    QGUIButtonMod(plCreatable* pCre, QWidget* parent = NULL);
    void saveDamage() override;

protected slots:
    void setDraggable();
    void unsetDraggable();
};

#endif
