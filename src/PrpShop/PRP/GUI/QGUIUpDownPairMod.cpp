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

#include "QGUIUpDownPairMod.h"

#include <QGridLayout>
#include "Main.h"
#include "QKeyDialog.h"

/* QGUIUpDownPairMod */
QGUIUpDownPairMod::QGUIUpDownPairMod(plCreatable* pCre, QWidget* parent)
    : QCreatable(pCre, kGUIUpDownPairMod, parent)
{
    pfGUIUpDownPairMod* ctrl = pfGUIUpDownPairMod::Convert(fCreatable);

    fControlModLink = new QCreatableLink(this, false);
    fControlModLink->setCreatable(ctrl, tr("GUI Control Common Properties"));
    fControlModLink->setForceType(kGUIControlMod);

    fMin = new QFloatEdit(this);
    fMin->setRange(-2147483648.0, 2147483647.0, 3);
    fMin->setValue(ctrl->getMin());
    fMax = new QFloatEdit(this);
    fMax->setRange(-2147483648.0, 2147483647.0, 3);
    fMax->setValue(ctrl->getMax());
    fStep = new QFloatEdit(this);
    fStep->setRange(-2147483648.0, 2147483647.0, 3);
    fStep->setValue(ctrl->getStep());

    fUpCtrl = new QCreatableLink(this);
    fUpCtrl->setKey(ctrl->getUpControl());

    fDownCtrl = new QCreatableLink(this);
    fDownCtrl->setKey(ctrl->getDownControl());

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(fControlModLink, 0, 0, 1, 3);
    layout->addWidget(new QLabel(tr("Range:"), this), 1, 0);
    layout->addWidget(fMin, 1, 1);
    layout->addWidget(fMax, 1, 2);
    layout->addWidget(new QLabel(tr("Step:"), this), 2, 0);
    layout->addWidget(fStep, 2, 1);
    layout->addWidget(new QLabel(tr("Up Control:"), this), 3, 0);
    layout->addWidget(fUpCtrl, 3, 1, 1, 2);
    layout->addWidget(new QLabel(tr("Down Control:"), this), 4, 0);
    layout->addWidget(fDownCtrl, 4, 1, 1, 2);

    connect(fUpCtrl, SIGNAL(addObject()), this, SLOT(setUpCtrl()));
    connect(fUpCtrl, SIGNAL(delObject()), this, SLOT(unsetUpCtrl()));
    connect(fDownCtrl, SIGNAL(addObject()), this, SLOT(setDownCtrl()));
    connect(fDownCtrl, SIGNAL(delObject()), this, SLOT(unsetDownCtrl()));
}

void QGUIUpDownPairMod::saveDamage()
{
    pfGUIUpDownPairMod* ctrl = pfGUIUpDownPairMod::Convert(fCreatable);

    ctrl->setRange(fMin->value(), fMax->value());
    ctrl->setStep(fStep->value());
}

void QGUIUpDownPairMod::setUpCtrl()
{
    pfGUIUpDownPairMod* ctrl = pfGUIUpDownPairMod::Convert(fCreatable);
    QFindKeyDialog dlg(this);
    if (ctrl->getUpControl().Exists())
        dlg.init(PrpShopMain::ResManager(), ctrl->getUpControl());
    else
        dlg.init(PrpShopMain::ResManager(), ctrl->getKey()->getLocation(), kGUIButtonMod);
    if (dlg.exec() == QDialog::Accepted) {
        ctrl->setUpControl(dlg.selection());
        fUpCtrl->setKey(ctrl->getUpControl());
    }
}

void QGUIUpDownPairMod::setDownCtrl()
{
    pfGUIUpDownPairMod* ctrl = pfGUIUpDownPairMod::Convert(fCreatable);
    QFindKeyDialog dlg(this);
    if (ctrl->getDownControl().Exists())
        dlg.init(PrpShopMain::ResManager(), ctrl->getDownControl());
    else
        dlg.init(PrpShopMain::ResManager(), ctrl->getKey()->getLocation(), kGUIButtonMod);
    if (dlg.exec() == QDialog::Accepted) {
        ctrl->setDownControl(dlg.selection());
        fDownCtrl->setKey(ctrl->getDownControl());
    }
}

void QGUIUpDownPairMod::unsetUpCtrl()
{
    pfGUIUpDownPairMod* ctrl = pfGUIUpDownPairMod::Convert(fCreatable);
    ctrl->setUpControl(plKey());
    fUpCtrl->setCreatable(NULL, "(None)");
}

void QGUIUpDownPairMod::unsetDownCtrl()
{
    pfGUIUpDownPairMod* ctrl = pfGUIUpDownPairMod::Convert(fCreatable);
    ctrl->setDownControl(plKey());
    fDownCtrl->setCreatable(NULL, "(None)");
}
