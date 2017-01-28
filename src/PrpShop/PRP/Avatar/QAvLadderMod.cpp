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

#include "QAvLadderMod.h"

#include <QRadioButton>
#include <QGridLayout>
#include <QLabel>

/* QAvLadderMod */

QAvLadderMod::QAvLadderMod(plCreatable* pCre, QWidget* parent)
    : QCreatable(pCre, kAvLadderMod, parent)
{
    plAvLadderMod* mod = plAvLadderMod::Convert(fCreatable);

    fType = new QComboBox(this);
    fType->addItem(tr("Big"), plAvLadderMod::kBig);
    fType->addItem(tr("Four Feet"), plAvLadderMod::kFourFeet);
    fType->addItem(tr("Two Feet"), plAvLadderMod::kTwoFeet);

    fLoops = new QIntEdit(this);
    fEnabled = new QCheckBox(tr("Enabled"), this);
    fGoingUp = new QButtonGroup(this);
    fLadderView = new QVector3(this);


    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);

    layout->addWidget(fEnabled, 1, 3, 1, 3, Qt::AlignRight);
    fEnabled->setChecked(mod->isEnabled());

    layout->addWidget(new QLabel(tr("Type:"), this), 1, 0);
    layout->addWidget(fType, 1, 1, 1, 2);
    int typeIdx = fType->findData(mod->getType());
    if (typeIdx < 0) {
        fType->addItem(QString("Unknown Value: %1").arg(mod->getType()), mod->getType());
        fType->setCurrentIndex(fType->count() - 1);
    } else {
        fType->setCurrentIndex(typeIdx);
    }
    connect(fType, QOverload<int>::of(&QComboBox::currentIndexChanged),
        [=](int index) { fLoops->setEnabled(fType->itemData(index) == plAvLadderMod::kBig); });

    layout->addWidget(new QLabel(tr("Loops:"), this), 2, 0);
    layout->addWidget(fLoops, 2, 1, 1, 2);
    fLoops->setValue(mod->getLoops());
    if (fType->currentIndex() != 0)
        fLoops->setEnabled(false);

    layout->addWidget(new QLabel(tr("Direction:"), this), 3, 0);
    QRadioButton* dirUp = new QRadioButton(tr("Up"), this);
    QRadioButton* dirDown = new QRadioButton(tr("Down"), this);
    fGoingUp->addButton(dirUp, kDirectionUp);
    layout->addWidget(dirUp, 3, 1);
    fGoingUp->addButton(dirDown, kDirectionDown);
    layout->addWidget(dirDown, 3, 2);
    fGoingUp->setExclusive(true);
    if (mod->getGoingUp()) {
        dirUp->setChecked(true);
    } else {
        dirDown->setChecked(true);
    }

    layout->addWidget(new QLabel(tr("LadderView:"), this), 4, 0);
    layout->addWidget(fLadderView, 5, 0, 1, 6);
    fLadderView->setValue(mod->getLadderView());
}

void QAvLadderMod::saveDamage()
{
    plAvLadderMod* mod = plAvLadderMod::Convert(fCreatable);

    mod->setEnabled(fEnabled->isChecked());
    mod->setGoingUp(fGoingUp->checkedId() == kDirectionUp);
    mod->setType(fType->currentData().toInt());
    mod->setLoops(fLoops->value());
    mod->setLadderView(fLadderView->value());
}
