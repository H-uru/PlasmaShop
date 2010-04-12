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

#ifndef _QGUIKNOBCTRL_H
#define _QGUIKNOBCTRL_H

#include "../QCreatable.h"

#include <PRP/GUI/pfGUIKnobCtrl.h>
#include <QCheckBox>
#include "../QObjLink.h"
#include "../QKeyList.h"

class QGUIKnobCtrl : public QCreatable {
    Q_OBJECT

protected:
    enum {
        kModFlagStart = pfGUIKnobCtrl::kReverseValues,
        kModFlagEnd = pfGUIKnobCtrl::kMapToAnimationRange,
        kModFlagCount = (kModFlagEnd - kModFlagStart) + 1
    };

    QCreatableLink* fControlModLink;
    QCheckBox* fModFlags[kModFlagCount];
    QKeyList* fAnimationKeys;
    QLineEdit* fAnimName;
    QFloatEdit* fMin;
    QFloatEdit* fMax;
    QFloatEdit* fStep;
    QFloatEdit* fStartPos[3];
    QFloatEdit* fEndPos[3];

public:
    QGUIKnobCtrl(plCreatable* pCre, QWidget* parent = NULL);
    virtual void saveDamage();
};

#endif
