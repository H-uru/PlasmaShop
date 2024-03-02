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

#include "QGUIRadioGroupCtrl.h"

#include <QGroupBox>
#include <QGridLayout>

/* QGUIRadioGroupCtrl */
QGUIRadioGroupCtrl::QGUIRadioGroupCtrl(plCreatable* pCre, QWidget* parent)
    : QCreatable(pCre, kGUIRadioGroupCtrl, parent)
{
    pfGUIRadioGroupCtrl* ctrl = pfGUIRadioGroupCtrl::Convert(fCreatable);

    fControlModLink = new QCreatableLink(this, false);
    fControlModLink->setCreatable(ctrl, tr("GUI Control Common Properties"));
    fControlModLink->setForceType(kGUIControlMod);

    QGroupBox* grpFlags = new QGroupBox(tr("Flags"), this);
    fModFlagAllowNoSelection = new QCheckBox(tr("Allow No Selection"), grpFlags);
    QGridLayout* flagLayout = new QGridLayout(grpFlags);
    flagLayout->setContentsMargins(8, 8, 8, 8);
    flagLayout->setVerticalSpacing(0);
    flagLayout->setHorizontalSpacing(8);
    flagLayout->addWidget(fModFlagAllowNoSelection, 0, 0);
    fModFlagAllowNoSelection->setChecked(ctrl->getFlag(pfGUIRadioGroupCtrl::kAllowNoSelection));

    connect(fModFlagAllowNoSelection, &QCheckBox::clicked, this, [this](bool checked) {
        pfGUIRadioGroupCtrl* ctrl = pfGUIRadioGroupCtrl::Convert(fCreatable);
        ctrl->setFlag(pfGUIRadioGroupCtrl::kAllowNoSelection, checked);
    });

    fControls = new QKeyList(ctrl->getKey(), this);
    fDefaultValue = new QComboBox(this);
    for (size_t i=0; i<ctrl->getControls().size(); i++) {
        fControls->addKey(ctrl->getControls()[i]);
        fDefaultValue->addItem(pqGetTypeIcon(ctrl->getControls()[i]->getType()),
                               pqFormatKeyName(ctrl->getControls()[i]));
    }
    fDefaultValue->setCurrentIndex(ctrl->getDefaultValue());

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(fControlModLink, 0, 0, 1, 2);
    layout->addWidget(grpFlags, 1, 0, 1, 2);
    layout->addItem(new QSpacerItem(0, 8, QSizePolicy::Minimum, QSizePolicy::Minimum), 2, 0, 1, 2);
    layout->addWidget(new QLabel(tr("Controls:"), this), 3, 0, 1, 2);
    layout->addWidget(fControls, 4, 0, 1, 2);
    layout->addWidget(new QLabel(tr("Default:"), this), 5, 0);
    layout->addWidget(fDefaultValue, 5, 1);

    connect(fControls, &QKeyList::itemAdded, this, &QGUIRadioGroupCtrl::controlAdded);
    connect(fControls, &QKeyList::itemRemoved, this, &QGUIRadioGroupCtrl::controlRemoved);
    fControls->adjustColumns();
}

void QGUIRadioGroupCtrl::saveDamage()
{
    pfGUIRadioGroupCtrl* ctrl = pfGUIRadioGroupCtrl::Convert(fCreatable);

    ctrl->clearControls();
    QList<plKey> ctrlKeys = fControls->keys();
    while (!ctrlKeys.isEmpty())
        ctrl->addControl(ctrlKeys.takeFirst());

    ctrl->setDefaultValue(fDefaultValue->currentIndex());
}

void QGUIRadioGroupCtrl::controlAdded(plKey ctrl)
{
    fDefaultValue->addItem(pqGetTypeIcon(ctrl->getType()), pqFormatKeyName(ctrl));
}

void QGUIRadioGroupCtrl::controlRemoved(int which)
{
    fDefaultValue->removeItem(which);
}
