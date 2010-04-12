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

#include "QGUIMenuItem.h"

#include <QGroupBox>
#include <QGridLayout>

/* QGUIMenuItem */
QGUIMenuItem::QGUIMenuItem(plCreatable* pCre, QWidget* parent)
            : QCreatable(pCre, kGUIMenuItem, parent)
{
    pfGUIMenuItem* ctrl = (pfGUIMenuItem*)fCreatable;

    fButtonModLink = new QCreatableLink(this, false);
    fButtonModLink->setText(tr("GUI Button Common Properties"));
    fButtonModLink->setCreatable(ctrl);
    fButtonModLink->setForceType(kGUIButtonMod);

    QGroupBox* grpFlags = new QGroupBox(tr("Flags"), this);
    fModFlags[pfGUIMenuItem::kDrawSubMenuArrow - kModFlagStart] = new QCheckBox(tr("Draw SubMenu Arrow"), grpFlags);
    fModFlags[pfGUIMenuItem::kReportHovers - kModFlagStart] = new QCheckBox(tr("Report Hovering"), grpFlags);
    QGridLayout* flagLayout = new QGridLayout(grpFlags);
    flagLayout->setContentsMargins(8, 8, 8, 8);
    flagLayout->setVerticalSpacing(0);
    flagLayout->setHorizontalSpacing(8);
    flagLayout->addWidget(fModFlags[pfGUIMenuItem::kDrawSubMenuArrow - kModFlagStart], 0, 0);
    flagLayout->addWidget(fModFlags[pfGUIMenuItem::kReportHovers - kModFlagStart], 1, 0);
    for (size_t i=0; i<kModFlagCount; i++)
        fModFlags[i]->setChecked(ctrl->getFlag(i + kModFlagStart));

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(fButtonModLink, 0, 0, 1, 2);
    layout->addWidget(grpFlags, 1, 0, 1, 2);
}

void QGUIMenuItem::saveDamage()
{
    pfGUIMenuItem* ctrl = (pfGUIMenuItem*)fCreatable;

    for (size_t i=0; i<kModFlagCount; i++)
        ctrl->setFlag(i + kModFlagStart, fModFlags[i]->isChecked());
}
