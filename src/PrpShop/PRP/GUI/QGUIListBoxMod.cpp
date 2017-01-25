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

#include "QGUIListBoxMod.h"

#include <QGroupBox>
#include <QGridLayout>
#include "../../Main.h"
#include "../../QKeyDialog.h"

/* QGUIListBoxMod */
QGUIListBoxMod::QGUIListBoxMod(plCreatable* pCre, QWidget* parent)
              : QCreatable(pCre, kGUIListBoxMod, parent)
{
    pfGUIListBoxMod* ctrl = pfGUIListBoxMod::Convert(fCreatable);

    fControlModLink = new QCreatableLink(this, false);
    fControlModLink->setCreatable(ctrl, tr("GUI Control Common Properties"));
    fControlModLink->setForceType(kGUIControlMod);

    QGroupBox* grpFlags = new QGroupBox(tr("Flags"), this);
    fModFlags[pfGUIListBoxMod::kSingleSelect - kModFlagStart] = new QCheckBox(tr("Single Select"), grpFlags);
    fModFlags[pfGUIListBoxMod::kDragAndDropCapable - kModFlagStart] = new QCheckBox(tr("Drag & Drop"), grpFlags);
    fModFlags[pfGUIListBoxMod::kDisableSelection - kModFlagStart] = new QCheckBox(tr("Disable Selection"), grpFlags);
    fModFlags[pfGUIListBoxMod::kDisableKeyActions - kModFlagStart] = new QCheckBox(tr("Disable Key Actions"), grpFlags);
    fModFlags[pfGUIListBoxMod::kScrollLeftToRight - kModFlagStart] = new QCheckBox(tr("Scroll Horizontally"), grpFlags);
    fModFlags[pfGUIListBoxMod::kAllowMultipleElementsPerRow - kModFlagStart] = new QCheckBox(tr("Multiple Elements per Row"), grpFlags);
    fModFlags[pfGUIListBoxMod::kAllowMousePassThrough - kModFlagStart] = new QCheckBox(tr("Allow Mouse Pass-Through"), grpFlags);
    fModFlags[pfGUIListBoxMod::kGrowLeavesAndProcessOxygen - kModFlagStart] = new QCheckBox(tr("Grow Leaves and Process Oxygen"), grpFlags);
    fModFlags[pfGUIListBoxMod::kHandsOffMultiSelect - kModFlagStart] = new QCheckBox(tr("Hands-Off Multi-Select"), grpFlags);
    fModFlags[pfGUIListBoxMod::kForbidNoSelection - kModFlagStart] = new QCheckBox(tr("Forbid No Selection"), grpFlags);
    QGridLayout* flagLayout = new QGridLayout(grpFlags);
    flagLayout->setContentsMargins(8, 8, 8, 8);
    flagLayout->setVerticalSpacing(0);
    flagLayout->setHorizontalSpacing(8);
    flagLayout->addWidget(fModFlags[pfGUIListBoxMod::kSingleSelect - kModFlagStart], 0, 0);
    flagLayout->addWidget(fModFlags[pfGUIListBoxMod::kDragAndDropCapable - kModFlagStart], 1, 0);
    flagLayout->addWidget(fModFlags[pfGUIListBoxMod::kDisableSelection - kModFlagStart], 2, 0);
    flagLayout->addWidget(fModFlags[pfGUIListBoxMod::kDisableKeyActions - kModFlagStart], 3, 0);
    flagLayout->addWidget(fModFlags[pfGUIListBoxMod::kScrollLeftToRight - kModFlagStart], 4, 0);
    flagLayout->addWidget(fModFlags[pfGUIListBoxMod::kAllowMultipleElementsPerRow - kModFlagStart], 0, 1);
    flagLayout->addWidget(fModFlags[pfGUIListBoxMod::kAllowMousePassThrough - kModFlagStart], 1, 1);
    flagLayout->addWidget(fModFlags[pfGUIListBoxMod::kGrowLeavesAndProcessOxygen - kModFlagStart], 2, 1);
    flagLayout->addWidget(fModFlags[pfGUIListBoxMod::kHandsOffMultiSelect - kModFlagStart], 3, 1);
    flagLayout->addWidget(fModFlags[pfGUIListBoxMod::kForbidNoSelection - kModFlagStart], 4, 1);
    for (size_t i=0; i<kModFlagCount; i++)
        fModFlags[i]->setChecked(ctrl->getFlag(i + kModFlagStart));

    fScrollCtrl = new QCreatableLink(this);
    fScrollCtrl->setKey(ctrl->getScrollCtrl());

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(fControlModLink, 0, 0, 1, 2);
    layout->addWidget(grpFlags, 1, 0, 1, 2);
    layout->addWidget(new QLabel(tr("Scroll Ctrl:"), this), 2, 0);
    layout->addWidget(fScrollCtrl, 2, 1);

    connect(fScrollCtrl, SIGNAL(addObject()), this, SLOT(setScrollCtrl()));
    connect(fScrollCtrl, SIGNAL(delObject()), this, SLOT(unsetScrollCtrl()));
}

void QGUIListBoxMod::saveDamage()
{
    pfGUIListBoxMod* ctrl = pfGUIListBoxMod::Convert(fCreatable);

    for (size_t i=0; i<kModFlagCount; i++)
        ctrl->setFlag(i + kModFlagStart, fModFlags[i]->isChecked());
}

void QGUIListBoxMod::setScrollCtrl()
{
    pfGUIListBoxMod* ctrl = pfGUIListBoxMod::Convert(fCreatable);
    QFindKeyDialog dlg(this);
    if (ctrl->getScrollCtrl().Exists())
        dlg.init(PrpShopMain::ResManager(), ctrl->getScrollCtrl());
    else
        dlg.init(PrpShopMain::ResManager(), ctrl->getKey()->getLocation(), kGUIUpDownPairMod);
    if (dlg.exec() == QDialog::Accepted) {
        ctrl->setScrollCtrl(dlg.selection());
        fScrollCtrl->setKey(ctrl->getScrollCtrl());
    }
}

void QGUIListBoxMod::unsetScrollCtrl()
{
    pfGUIListBoxMod* ctrl = pfGUIListBoxMod::Convert(fCreatable);
    ctrl->setScrollCtrl(plKey());
    fScrollCtrl->setCreatable(NULL, "(None)");
}
