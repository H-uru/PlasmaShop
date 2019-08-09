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

#include "QGUIMultiLineEditCtrl.h"

#include <QGridLayout>
#include "Main.h"
#include "QKeyDialog.h"

/* QGUIMultiLineEditCtrl */
QGUIMultiLineEditCtrl::QGUIMultiLineEditCtrl(plCreatable* pCre, QWidget* parent)
    : QCreatable(pCre, kGUIMultiLineEditCtrl, parent)
{
    pfGUIMultiLineEditCtrl* ctrl = pfGUIMultiLineEditCtrl::Convert(fCreatable);

    fControlModLink = new QCreatableLink(this, false);
    fControlModLink->setCreatable(ctrl, tr("GUI Control Common Properties"));
    fControlModLink->setForceType(kGUIControlMod);

    fScrollCtrl = new QCreatableLink(this);
    fScrollCtrl->setKey(ctrl->getScrollCtrl());

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(fControlModLink, 0, 0, 1, 2);
    layout->addWidget(new QLabel(tr("Scroll Ctrl:"), this), 1, 0);
    layout->addWidget(fScrollCtrl, 1, 1);

    connect(fScrollCtrl, SIGNAL(addObject()), this, SLOT(setScrollCtrl()));
    connect(fScrollCtrl, SIGNAL(delObject()), this, SLOT(unsetScrollCtrl()));
}

void QGUIMultiLineEditCtrl::setScrollCtrl()
{
    pfGUIMultiLineEditCtrl* ctrl = pfGUIMultiLineEditCtrl::Convert(fCreatable);
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

void QGUIMultiLineEditCtrl::unsetScrollCtrl()
{
    pfGUIMultiLineEditCtrl* ctrl = pfGUIMultiLineEditCtrl::Convert(fCreatable);
    ctrl->setScrollCtrl(plKey());
    fScrollCtrl->setCreatable(NULL, "(None)");
}
