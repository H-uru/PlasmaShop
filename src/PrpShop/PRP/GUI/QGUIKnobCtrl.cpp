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

#include "QGUIKnobCtrl.h"

#include <QGroupBox>
#include <QGridLayout>

/* QGUIKnobCtrl */
QGUIKnobCtrl::QGUIKnobCtrl(plCreatable* pCre, QWidget* parent)
            : QCreatable(pCre, kGUIKnobCtrl, parent)
{
    pfGUIKnobCtrl* ctrl = pfGUIKnobCtrl::Convert(fCreatable);

    fControlModLink = new QCreatableLink(this, false);
    fControlModLink->setText(tr("GUI Control Common Properties"));
    fControlModLink->setCreatable(ctrl);
    fControlModLink->setForceType(kGUIControlMod);

    QGroupBox* grpFlags = new QGroupBox(tr("Flags"), this);
    fModFlags[pfGUIKnobCtrl::kReverseValues - kModFlagStart] = new QCheckBox(tr("Reverse Values"), grpFlags);
    fModFlags[pfGUIKnobCtrl::kLeftRightOrientation - kModFlagStart] = new QCheckBox(tr("Left-Right Orientation"), grpFlags);
    fModFlags[pfGUIKnobCtrl::kMapToScreenRange - kModFlagStart] = new QCheckBox(tr("Map to Screen Range"), grpFlags);
    fModFlags[pfGUIKnobCtrl::kTriggerOnlyOnMouseUp - kModFlagStart] = new QCheckBox(tr("Trigger Only on MouseUp"), grpFlags);
    fModFlags[pfGUIKnobCtrl::kMapToAnimationRange - kModFlagStart] = new QCheckBox(tr("Map to Anim Range"), grpFlags);
    QGridLayout* flagLayout = new QGridLayout(grpFlags);
    flagLayout->setContentsMargins(8, 8, 8, 8);
    flagLayout->setVerticalSpacing(0);
    flagLayout->setHorizontalSpacing(8);
    flagLayout->addWidget(fModFlags[pfGUIKnobCtrl::kReverseValues - kModFlagStart], 0, 0);
    flagLayout->addWidget(fModFlags[pfGUIKnobCtrl::kLeftRightOrientation - kModFlagStart], 1, 0);
    flagLayout->addWidget(fModFlags[pfGUIKnobCtrl::kMapToScreenRange - kModFlagStart], 2, 0);
    flagLayout->addWidget(fModFlags[pfGUIKnobCtrl::kTriggerOnlyOnMouseUp - kModFlagStart], 3, 0);
    flagLayout->addWidget(fModFlags[pfGUIKnobCtrl::kMapToAnimationRange - kModFlagStart], 4, 0);
    for (size_t i=0; i<kModFlagCount; i++)
        fModFlags[i]->setChecked(ctrl->getFlag(i + kModFlagStart));

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
    fAnimName->setText(~ctrl->getAnimName());
    fAnimationKeys = new QKeyList(ctrl->getKey(), this);
    for (size_t i=0; i<ctrl->getAnimationKeys().size(); i++)
        fAnimationKeys->addKey(ctrl->getAnimationKeys()[i]);

    for (size_t i=0; i<3; i++) {
        fStartPos[i] = new QFloatEdit(this);
        fStartPos[i]->setRange(-2147483648.0, 2147483647.0, 3);
    }
    fStartPos[0]->setValue(ctrl->getAnimStartPos().X);
    fStartPos[1]->setValue(ctrl->getAnimStartPos().Y);
    fStartPos[2]->setValue(ctrl->getAnimStartPos().Z);

    for (size_t i=0; i<3; i++) {
        fEndPos[i] = new QFloatEdit(this);
        fEndPos[i]->setRange(-2147483648.0, 2147483647.0, 3);
    }
    fEndPos[0]->setValue(ctrl->getAnimEndPos().X);
    fEndPos[1]->setValue(ctrl->getAnimEndPos().Y);
    fEndPos[2]->setValue(ctrl->getAnimEndPos().Z);

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(fControlModLink, 0, 0, 1, 4);
    layout->addWidget(grpFlags, 1, 0, 1, 4);
    layout->addWidget(new QLabel(tr("Range:"), this), 2, 0);
    layout->addWidget(fMin, 2, 1);
    layout->addWidget(fMax, 2, 2);
    layout->addWidget(new QLabel(tr("Step:"), this), 3, 0);
    layout->addWidget(fStep, 3, 1);
    layout->addWidget(new QLabel(tr("Anim Name:"), this), 4, 0);
    layout->addWidget(fAnimName, 4, 1, 1, 3);
    layout->addWidget(fAnimationKeys, 5, 0, 1, 4);
    layout->addWidget(new QLabel(tr("Start Pos:"), this), 6, 0);
    layout->addWidget(fStartPos[0], 6, 1);
    layout->addWidget(fStartPos[1], 6, 2);
    layout->addWidget(fStartPos[2], 6, 3);
    layout->addWidget(new QLabel(tr("End Pos:"), this), 7, 0);
    layout->addWidget(fEndPos[0], 7, 1);
    layout->addWidget(fEndPos[1], 7, 2);
    layout->addWidget(fEndPos[2], 7, 3);

    fAnimationKeys->adjustColumns();
}

void QGUIKnobCtrl::saveDamage()
{
    pfGUIKnobCtrl* ctrl = pfGUIKnobCtrl::Convert(fCreatable);

    for (size_t i=0; i<kModFlagCount; i++)
        ctrl->setFlag(i + kModFlagStart, fModFlags[i]->isChecked());

    ctrl->setRange(fMin->value(), fMax->value());
    ctrl->setStep(fStep->value());

    ctrl->setAnimName(~fAnimName->text());
    ctrl->clearAnimationKeys();
    QList<plKey> animKeys = fAnimationKeys->keys();
    while (!animKeys.isEmpty())
        ctrl->addAnimationKey(animKeys.takeFirst());

    ctrl->setAnimStartPos(hsVector3(fStartPos[0]->value(),
                                    fStartPos[1]->value(),
                                    fStartPos[2]->value()));
    ctrl->setAnimEndPos(hsVector3(fEndPos[0]->value(),
                                  fEndPos[1]->value(),
                                  fEndPos[2]->value()));
}
