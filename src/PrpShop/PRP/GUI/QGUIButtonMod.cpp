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

#include "QGUIButtonMod.h"

#include <QTabWidget>
#include <QGridLayout>
#include "Main.h"
#include "QKeyDialog.h"

/* QGUIButtonMod */
QGUIButtonMod::QGUIButtonMod(plCreatable* pCre, QWidget* parent)
    : QCreatable(pCre, kGUIButtonMod, parent)
{
    pfGUIButtonMod* ctrl = pfGUIButtonMod::Convert(fCreatable);

    fControlModLink = new QCreatableLink(this, false);
    fControlModLink->setCreatable(ctrl, tr("GUI Control Common Properties"));
    fControlModLink->setForceType(kGUIControlMod);

    QTabWidget* animTabs = new QTabWidget(this);
    QWidget* animationGrp = new QWidget(animTabs);
    fAnimName = new QLineEdit(animationGrp);
    fAnimName->setText(st2qstr(ctrl->getAnimationName()));
    fAnimationKeys = new QKeyList(ctrl->getKey(), animationGrp);
    for (size_t i=0; i<ctrl->getAnimationKeys().size(); i++)
        fAnimationKeys->addKey(ctrl->getAnimationKeys()[i]);
    QGridLayout* animationLayout = new QGridLayout(animationGrp);
    animationLayout->addWidget(new QLabel(tr("Anim Name:"), animationGrp), 0, 0);
    animationLayout->addWidget(fAnimName, 0, 1);
    animationLayout->addWidget(fAnimationKeys, 1, 0, 1, 2);

    QWidget* mouseOverAnimGrp = new QWidget(animTabs);
    fMouseOverAnimName = new QLineEdit(mouseOverAnimGrp);
    fMouseOverAnimName->setText(st2qstr(ctrl->getMouseOverAnimName()));
    fMouseOverAnimKeys = new QKeyList(ctrl->getKey(), mouseOverAnimGrp);
    for (size_t i=0; i<ctrl->getMouseOverKeys().size(); i++)
        fMouseOverAnimKeys->addKey(ctrl->getMouseOverKeys()[i]);
    QGridLayout* mouseOverAnimLayout = new QGridLayout(mouseOverAnimGrp);
    mouseOverAnimLayout->addWidget(new QLabel(tr("Anim Name:"), mouseOverAnimGrp), 0, 0);
    mouseOverAnimLayout->addWidget(fMouseOverAnimName, 0, 1);
    mouseOverAnimLayout->addWidget(fMouseOverAnimKeys, 1, 0, 1, 2);

    animTabs->addTab(animationGrp, tr("Animation"));
    animTabs->addTab(mouseOverAnimGrp, tr("MouseOver Animation"));

    fNotifyType = new QComboBox(this);
    fNotifyType->addItems(QStringList() << tr("Up Only") << tr("Down Only") << tr("Both"));
    fNotifyType->setCurrentIndex(ctrl->getNotifyType());

    fDraggable = new QCreatableLink(this);
    fDraggable->setKey(ctrl->getDraggable());

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(fControlModLink, 0, 0, 1, 2);
    layout->addWidget(animTabs, 1, 0, 1, 2);
    layout->addWidget(new QLabel(tr("Notify Type:"), this), 2, 0);
    layout->addWidget(fNotifyType, 2, 1);
    layout->addWidget(new QLabel(tr("Draggable:"), this), 3, 0);
    layout->addWidget(fDraggable, 3, 1);

    connect(fDraggable, &QCreatableLink::addObject, this, &QGUIButtonMod::setDraggable);
    connect(fDraggable, &QCreatableLink::delObject, this, &QGUIButtonMod::unsetDraggable);

    fAnimationKeys->adjustColumns();
    fMouseOverAnimKeys->adjustColumns();
}

void QGUIButtonMod::saveDamage()
{
    pfGUIButtonMod* ctrl = pfGUIButtonMod::Convert(fCreatable);

    ctrl->setAnimationName(qstr2st(fAnimName->text()));
    ctrl->setMouseOverAnimName(qstr2st(fMouseOverAnimName->text()));

    ctrl->clearAnimationKeys();
    QList<plKey> animKeys = fAnimationKeys->keys();
    while (!animKeys.isEmpty())
        ctrl->addAnimationKey(animKeys.takeFirst());

    ctrl->clearMouseOverKeys();
    QList<plKey> mouseOverKeys = fMouseOverAnimKeys->keys();
    while (!mouseOverKeys.isEmpty())
        ctrl->addMouseOverKey(mouseOverKeys.takeFirst());

    ctrl->setNotifyType(fNotifyType->currentIndex());
}

void QGUIButtonMod::setDraggable()
{
    pfGUIButtonMod* ctrl = pfGUIButtonMod::Convert(fCreatable);
    QFindKeyDialog dlg(this);
    if (ctrl->getDraggable().Exists())
        dlg.init(PrpShopMain::ResManager(), ctrl->getDraggable());
    else
        dlg.init(PrpShopMain::ResManager(), ctrl->getKey()->getLocation(), kGUIDraggableMod);
    if (dlg.exec() == QDialog::Accepted) {
        ctrl->setDraggable(dlg.selection());
        fDraggable->setKey(ctrl->getDraggable());
    }
}

void QGUIButtonMod::unsetDraggable()
{
    pfGUIButtonMod* ctrl = pfGUIButtonMod::Convert(fCreatable);
    ctrl->setDraggable(plKey());
    fDraggable->setCreatable(NULL, "(None)");
}
