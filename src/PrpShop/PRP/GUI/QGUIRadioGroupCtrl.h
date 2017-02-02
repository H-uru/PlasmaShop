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

#ifndef _QGUIRADIOGROUPCTRL_H
#define _QGUIRADIOGROUPCTRL_H

#include "PRP/QCreatable.h"

#include <PRP/GUI/pfGUIRadioGroupCtrl.h>
#include <QCheckBox>
#include <QComboBox>
#include "PRP/QObjLink.h"
#include "PRP/QKeyList.h"

class QGUIRadioGroupCtrl : public QCreatable {
    Q_OBJECT

protected:
    QCreatableLink* fControlModLink;
    QCheckBox* fModFlagAllowNoSelection;
    QKeyList* fControls;
    QComboBox* fDefaultValue;

public:
    QGUIRadioGroupCtrl(plCreatable* pCre, QWidget* parent = NULL);
    virtual void saveDamage();

private slots:
    void controlAdded(plKey ctrl);
    void controlRemoved(int which);
};

#endif
