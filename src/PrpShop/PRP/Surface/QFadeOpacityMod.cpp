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

#include "QFadeOpacityMod.h"

#include <QLabel>
#include <QGroupBox>
#include <QGridLayout>
#include "../../QPlasmaUtils.h"

/* QFadeOpacityMod */
QFadeOpacityMod::QFadeOpacityMod(plCreatable* pCre, QWidget* parent)
               : QCreatable(pCre, kFadeOpacityMod, parent)
{
    plFadeOpacityMod* mod = plFadeOpacityMod::Convert(fCreatable);

    fSynchObjLink = new QCreatableLink(this, false);
    fSynchObjLink->setCreatable(mod, tr("Synch Flags"));
    fSynchObjLink->setForceType(kSynchedObject);

    QGroupBox* grpFlags = new QGroupBox(tr("Flags"), this);
    QGridLayout* layFlags = new QGridLayout(grpFlags);
    layFlags->setVerticalSpacing(0);
    layFlags->setHorizontalSpacing(8);
    fFlags[kBoundsCenter] = new QCheckBox(tr("Bounds Centered"), grpFlags);
    layFlags->addWidget(fFlags[kBoundsCenter], 0, 0);
    fFlags[kBoundsCenter]->setChecked(mod->getFlag(plFadeOpacityMod::kBoundsCenter));

    fUp = new QFloatEdit(this);
    fUp->setValue(mod->getFadeUp());

    fDown = new QFloatEdit(this);
    fDown->setValue(mod->getFadeDown());

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(fSynchObjLink, 0, 0, 1, 2);
    layout->addWidget(grpFlags, 1, 0, 1, 2);
    layout->addWidget(new QLabel(tr("Fade Up:"), this), 2, 0);
    layout->addWidget(fUp, 2, 1);
    layout->addWidget(new QLabel(tr("Fade Down:"), this), 3, 0);
    layout->addWidget(fDown, 3, 1);
}

void QFadeOpacityMod::saveDamage()
{
    plFadeOpacityMod* mod = plFadeOpacityMod::Convert(fCreatable);

    mod->setFlag(plFadeOpacityMod::kBoundsCenter, fFlags[kBoundsCenter]->isChecked());
    mod->setFadeUp(fUp->value());
    mod->setFadeDown(fDown->value());
}
