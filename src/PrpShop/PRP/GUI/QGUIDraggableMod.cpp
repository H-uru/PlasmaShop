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

#include "QGUIDraggableMod.h"

#include <QGroupBox>
#include <QGridLayout>

/* QGUIDraggableMod */
QGUIDraggableMod::QGUIDraggableMod(plCreatable* pCre, QWidget* parent)
    : QCreatable(pCre, kGUIDraggableMod, parent)
{
    pfGUIDraggableMod* ctrl = pfGUIDraggableMod::Convert(fCreatable);

    fControlModLink = new QCreatableLink(this, false);
    fControlModLink->setCreatable(ctrl, tr("GUI Control Common Properties"));
    fControlModLink->setForceType(kGUIControlMod);

    QGroupBox* grpFlags = new QGroupBox(tr("Flags"), this);
    fModFlags[pfGUIDraggableMod::kReportDragging - kModFlagStart] = new QCheckBox(tr("Report Dragging"), grpFlags);
    fModFlags[pfGUIDraggableMod::kHideCursorWhileDragging - kModFlagStart] = new QCheckBox(tr("Hide Cursor While Dragging"), grpFlags);
    fModFlags[pfGUIDraggableMod::kAlwaysSnapBackToStart - kModFlagStart] = new QCheckBox(tr("Always Snap Back to Start"), grpFlags);
    QGridLayout* flagLayout = new QGridLayout(grpFlags);
    flagLayout->setContentsMargins(8, 8, 8, 8);
    flagLayout->setVerticalSpacing(0);
    flagLayout->setHorizontalSpacing(8);
    flagLayout->addWidget(fModFlags[pfGUIDraggableMod::kReportDragging - kModFlagStart], 0, 0);
    flagLayout->addWidget(fModFlags[pfGUIDraggableMod::kHideCursorWhileDragging - kModFlagStart], 1, 0);
    flagLayout->addWidget(fModFlags[pfGUIDraggableMod::kAlwaysSnapBackToStart - kModFlagStart], 2, 0);
    for (size_t i=0; i<kModFlagCount; i++)
        fModFlags[i]->setChecked(ctrl->getFlag(i + kModFlagStart));

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(fControlModLink, 0, 0, 1, 2);
    layout->addWidget(grpFlags, 1, 0, 1, 2);
}

void QGUIDraggableMod::saveDamage()
{
    pfGUIDraggableMod* ctrl = pfGUIDraggableMod::Convert(fCreatable);

    for (size_t i=0; i<kModFlagCount; i++)
        ctrl->setFlag(i + kModFlagStart, fModFlags[i]->isChecked());
}
