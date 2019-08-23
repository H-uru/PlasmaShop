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

#include "QGUIProgressCtrl.h"

#include <QGroupBox>
#include <QGridLayout>

/* QGUIProgressCtrl */
QGUIProgressCtrl::QGUIProgressCtrl(plCreatable* pCre, QWidget* parent)
    : QCreatable(pCre, kGUIProgressCtrl, parent)
{
    pfGUIProgressCtrl* ctrl = pfGUIProgressCtrl::Convert(fCreatable);

    fControlModLink = new QCreatableLink(this, false);
    fControlModLink->setCreatable(ctrl, tr("GUI Control Common Properties"));
    fControlModLink->setForceType(kGUIControlMod);

    QGroupBox* grpFlags = new QGroupBox(tr("Flags"), this);
    fModFlags[pfGUIProgressCtrl::kReverseValues - kModFlagStart] = new QCheckBox(tr("Reverse Values"), grpFlags);
    fModFlags[pfGUIProgressCtrl::kLeftRightOrientation - kModFlagStart] = new QCheckBox(tr("Left-Right Orientation"), grpFlags);
    fModFlags[pfGUIProgressCtrl::kMapToScreenRange - kModFlagStart] = new QCheckBox(tr("Map to Screen Range"), grpFlags);
    fModFlags[pfGUIProgressCtrl::kTriggerOnlyOnMouseUp - kModFlagStart] = new QCheckBox(tr("Trigger Only on MouseUp"), grpFlags);
    fModFlags[pfGUIProgressCtrl::kMapToAnimationRange - kModFlagStart] = new QCheckBox(tr("Map to Anim Range"), grpFlags);
    QGridLayout* flagLayout = new QGridLayout(grpFlags);
    flagLayout->setContentsMargins(8, 8, 8, 8);
    flagLayout->setVerticalSpacing(0);
    flagLayout->setHorizontalSpacing(8);
    flagLayout->addWidget(fModFlags[pfGUIProgressCtrl::kReverseValues - kModFlagStart], 0, 0);
    flagLayout->addWidget(fModFlags[pfGUIProgressCtrl::kLeftRightOrientation - kModFlagStart], 1, 0);
    flagLayout->addWidget(fModFlags[pfGUIProgressCtrl::kMapToScreenRange - kModFlagStart], 2, 0);
    flagLayout->addWidget(fModFlags[pfGUIProgressCtrl::kTriggerOnlyOnMouseUp - kModFlagStart], 3, 0);
    flagLayout->addWidget(fModFlags[pfGUIProgressCtrl::kMapToAnimationRange - kModFlagStart], 4, 0);

    for (size_t i = 0; i < kModFlagCount; ++i) {
        fModFlags[i]->setChecked(ctrl->getFlag(i + kModFlagStart));
        connect(fModFlags[i], &QCheckBox::clicked, this, [this, i](bool checked) {
            pfGUIProgressCtrl* ctrl = pfGUIProgressCtrl::Convert(fCreatable);
            ctrl->setFlag(i + kModFlagStart, checked);
        });
    }

    fMin = new QFloatEdit(this);
    fMin->setRange(-2147483648.0, 2147483647.0, 3);
    fMin->setValue(ctrl->getMin());
    fMax = new QFloatEdit(this);
    fMax->setRange(-2147483648.0, 2147483647.0, 3);
    fMax->setValue(ctrl->getMax());
    fStep = new QFloatEdit(this);
    fStep->setRange(-2147483648.0, 2147483647.0, 3);
    fStep->setValue(ctrl->getStep());

    fAnimName = new QLineEdit(this);
    fAnimName->setText(st2qstr(ctrl->getAnimName()));
    fAnimationKeys = new QKeyList(ctrl->getKey(), this);
    for (size_t i=0; i<ctrl->getAnimKeys().size(); i++)
        fAnimationKeys->addKey(ctrl->getAnimKeys()[i]);

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(fControlModLink, 0, 0, 1, 3);
    layout->addWidget(grpFlags, 1, 0, 1, 3);
    layout->addWidget(new QLabel(tr("Range:"), this), 2, 0);
    layout->addWidget(fMin, 2, 1);
    layout->addWidget(fMax, 2, 2);
    layout->addWidget(new QLabel(tr("Step:"), this), 3, 0);
    layout->addWidget(fStep, 3, 1);
    layout->addWidget(new QLabel(tr("Anim Name:"), this), 4, 0);
    layout->addWidget(fAnimName, 4, 1, 1, 2);
    layout->addWidget(fAnimationKeys, 5, 0, 1, 3);

    fAnimationKeys->adjustColumns();
}

void QGUIProgressCtrl::saveDamage()
{
    pfGUIProgressCtrl* ctrl = pfGUIProgressCtrl::Convert(fCreatable);

    ctrl->setRange(fMin->value(), fMax->value());
    ctrl->setStep(fStep->value());

    ctrl->setAnimName(qstr2st(fAnimName->text()));
    ctrl->clearAnimKeys();
    QList<plKey> animKeys = fAnimationKeys->keys();
    while (!animKeys.isEmpty())
        ctrl->addAnimKey(animKeys.takeFirst());
}
