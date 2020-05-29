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

#ifndef _QGUIMENUITEM_H
#define _QGUIMENUITEM_H

#include "PRP/QCreatable.h"

#include <PRP/GUI/pfGUIButtonMod.h>
#include <QCheckBox>
#include "PRP/QObjLink.h"

class QGUIMenuItem : public QCreatable
{
    Q_OBJECT

protected:
    enum
    {
        kModFlagStart = pfGUIMenuItem::kDrawSubMenuArrow,
        kModFlagEnd = pfGUIMenuItem::kReportHovers,
        kModFlagCount = (kModFlagEnd - kModFlagStart) + 1
    };

    QCreatableLink* fButtonModLink;
    QCheckBox* fModFlags[kModFlagCount];

public:
    QGUIMenuItem(plCreatable* pCre, QWidget* parent = NULL);
};

#endif
