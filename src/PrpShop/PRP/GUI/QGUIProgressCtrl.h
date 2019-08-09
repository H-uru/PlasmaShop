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

#ifndef _QGUIPROGRESSCTRL_H
#define _QGUIPROGRESSCTRL_H

#include "PRP/QCreatable.h"

#include <PRP/GUI/pfGUIProgressCtrl.h>
#include <QCheckBox>
#include "PRP/QObjLink.h"
#include "PRP/QKeyList.h"

class QGUIProgressCtrl : public QCreatable
{
    Q_OBJECT

protected:
    enum
    {
        kModFlagStart = pfGUIProgressCtrl::kReverseValues,
        kModFlagEnd = pfGUIProgressCtrl::kMapToAnimationRange,
        kModFlagCount = (kModFlagEnd - kModFlagStart) + 1
    };

    QCreatableLink* fControlModLink;
    QCheckBox* fModFlags[kModFlagCount];
    QKeyList* fAnimationKeys;
    QLineEdit* fAnimName;
    QFloatEdit* fMin;
    QFloatEdit* fMax;
    QFloatEdit* fStep;

public:
    QGUIProgressCtrl(plCreatable* pCre, QWidget* parent = NULL);
    virtual void saveDamage();
};

#endif
