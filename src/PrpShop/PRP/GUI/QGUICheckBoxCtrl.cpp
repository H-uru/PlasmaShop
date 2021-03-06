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

#include "QGUICheckBoxCtrl.h"
#include <QGridLayout>

/* QGUICheckBoxCtrl */
QGUICheckBoxCtrl::QGUICheckBoxCtrl(plCreatable* pCre, QWidget* parent)
    : QCreatable(pCre, kGUICheckBoxCtrl, parent)
{
    pfGUICheckBoxCtrl* ctrl = pfGUICheckBoxCtrl::Convert(fCreatable);

    fControlModLink = new QCreatableLink(this, false);
    fControlModLink->setCreatable(ctrl, tr("GUI Control Common Properties"));
    fControlModLink->setForceType(kGUIControlMod);

    fAnimName = new QLineEdit(this);
    fAnimName->setText(st2qstr(ctrl->getAnimName()));
    fAnimationKeys = new QKeyList(ctrl->getKey(), this);
    for (size_t i=0; i<ctrl->getAnimKeys().size(); i++)
        fAnimationKeys->addKey(ctrl->getAnimKeys()[i]);

    fValue = new QCheckBox(tr("Checked"), this);
    fValue->setChecked(ctrl->isChecked());
    connect(fValue, &QCheckBox::clicked, this, [this](bool checked) {
        pfGUICheckBoxCtrl* ctrl = pfGUICheckBoxCtrl::Convert(fCreatable);
        ctrl->setChecked(checked);
    });

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(fControlModLink, 0, 0, 1, 2);
    layout->addWidget(new QLabel(tr("Anim Name:"), this), 1, 0);
    layout->addWidget(fAnimName, 1, 1);
    layout->addWidget(fAnimationKeys, 2, 0, 1, 2);
    layout->addWidget(fValue, 3, 0, 1, 2);

    fAnimationKeys->adjustColumns();
}

void QGUICheckBoxCtrl::saveDamage()
{
    pfGUICheckBoxCtrl* ctrl = pfGUICheckBoxCtrl::Convert(fCreatable);

    ctrl->setAnimName(qstr2st(fAnimName->text()));
    ctrl->clearAnimKeys();
    QList<plKey> animKeys = fAnimationKeys->keys();
    while (!animKeys.isEmpty())
        ctrl->addAnimKey(animKeys.takeFirst());
}
