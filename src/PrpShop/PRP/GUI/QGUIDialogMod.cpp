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

#include "QGUIDialogMod.h"

#include <QGroupBox>
#include <QGridLayout>
#include "Main.h"
#include "QKeyDialog.h"

/* QGUIDialogMod */
QGUIDialogMod::QGUIDialogMod(plCreatable* pCre, QWidget* parent)
    : QCreatable(pCre, kGUIDialogMod, parent)
{
    pfGUIDialogMod* dlg = pfGUIDialogMod::Convert(fCreatable);

    fSynchObjLink = new QCreatableLink(this, false);
    fSynchObjLink->setCreatable(dlg, tr("Synch Flags"));
    fSynchObjLink->setForceType(kSynchedObject);

    QGroupBox* grpFlags = new QGroupBox(tr("Flags"), this);
    fModFlagModal = new QCheckBox(tr("Modal"), grpFlags);
    QGridLayout* flagLayout = new QGridLayout(grpFlags);
    flagLayout->setContentsMargins(8, 8, 8, 8);
    flagLayout->addWidget(fModFlagModal, 0, 0);
    fModFlagModal->setChecked(dlg->getFlag(pfGUIDialogMod::kModal));

    connect(fModFlagModal, &QCheckBox::clicked, this, [this](bool checked) {
        pfGUIDialogMod* dlg = pfGUIDialogMod::Convert(fCreatable);
        dlg->setFlag(pfGUIDialogMod::kModal, checked);
    });

    QGroupBox* colorSchemeGrp = new QGroupBox(tr("Color Scheme"), this);
    fColorScheme = new QGUIColorScheme(colorSchemeGrp);
    fColorScheme->setColorScheme(&dlg->getColorScheme());
    QGridLayout* skinLayout = new QGridLayout(colorSchemeGrp);
    skinLayout->setContentsMargins(0, 0, 0, 0);
    skinLayout->addWidget(fColorScheme, 0, 0);

    fName = new QLineEdit(this);
    fName->setText(st2qstr(dlg->getName()));
    fTagID = new QIntEdit(this);
    fTagID->setRange(-0x80000000, 0x7FFFFFFF);
    fTagID->setValue(dlg->getTagID());
    fVersion = new QIntEdit(this);
    fVersion->setRange(-0x80000000, 0x7FFFFFFF);
    fVersion->setValue(dlg->getVersion());

    fControls = new QKeyList(dlg->getKey(), this);
    for (size_t i=0; i<dlg->getControls().size(); i++)
        fControls->addKey(dlg->getControls()[i]);

    fRenderMod = new QCreatableLink(this);
    fRenderMod->setKey(dlg->getRenderMod());

    fProcReceiver = new QCreatableLink(this);
    fProcReceiver->setKey(dlg->getProcReceiver());

    fSceneNode = new QCreatableLink(this);
    fSceneNode->setKey(dlg->getSceneNode());

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(fSynchObjLink, 0, 0, 1, 5);
    layout->addWidget(grpFlags, 1, 0, 1, 5);
    layout->addWidget(colorSchemeGrp, 2, 0, 1, 5);
    layout->addWidget(new QLabel(tr("Name:"), this), 3, 0);
    layout->addWidget(fName, 3, 1, 1, 4);
    layout->addWidget(new QLabel(tr("Tag ID:"), this), 4, 0);
    layout->addWidget(fTagID, 4, 1);
    layout->addWidget(new QLabel(tr("Version:"), this), 4, 3);
    layout->addWidget(fVersion, 4, 4);
    layout->addItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum), 4, 2);
    layout->addWidget(new QLabel(tr("Controls:"), this), 5, 0, 1, 5);
    layout->addWidget(fControls, 6, 0, 1, 5);
    layout->addWidget(new QLabel(tr("Render Mod:"), this), 7, 0);
    layout->addWidget(fRenderMod, 7, 1, 1, 4);
    layout->addWidget(new QLabel(tr("Proc Receiver:"), this), 8, 0);
    layout->addWidget(fProcReceiver, 8, 1, 1, 4);
    layout->addWidget(new QLabel(tr("Scene Node:"), this), 9, 0);
    layout->addWidget(fSceneNode, 9, 1, 1, 4);

    connect(fRenderMod, &QCreatableLink::addObject, this, &QGUIDialogMod::setRenderMod);
    connect(fRenderMod, &QCreatableLink::delObject, this, &QGUIDialogMod::unsetRenderMod);
    connect(fProcReceiver, &QCreatableLink::addObject, this, &QGUIDialogMod::setProcReceiver);
    connect(fProcReceiver, &QCreatableLink::delObject, this, &QGUIDialogMod::unsetProcReceiver);
    connect(fSceneNode, &QCreatableLink::addObject, this, &QGUIDialogMod::setSceneNode);
    connect(fSceneNode, &QCreatableLink::delObject, this, &QGUIDialogMod::unsetSceneNode);
    fControls->adjustColumns();
}

void QGUIDialogMod::saveDamage()
{
    pfGUIDialogMod* dlg = pfGUIDialogMod::Convert(fCreatable);

    fColorScheme->saveColorScheme(&dlg->getColorScheme());
    dlg->setName(fName->text().toUtf8().constData());
    dlg->setTagID(fTagID->value());
    dlg->setVersion(fVersion->value());

    dlg->clearControls();
    QList<plKey> ctrlKeys = fControls->keys();
    while (!ctrlKeys.isEmpty())
        dlg->addControl(ctrlKeys.takeFirst());
}

void QGUIDialogMod::setRenderMod()
{
    pfGUIDialogMod* dlgMod = pfGUIDialogMod::Convert(fCreatable);
    QFindKeyDialog dlg(this);
    if (dlgMod->getRenderMod().Exists())
        dlg.init(PrpShopMain::ResManager(), dlgMod->getRenderMod());
    else
        dlg.init(PrpShopMain::ResManager(), dlgMod->getKey()->getLocation(), kPostEffectMod);
    if (dlg.exec() == QDialog::Accepted) {
        dlgMod->setRenderMod(dlg.selection());
        fRenderMod->setKey(dlgMod->getRenderMod());
    }
}

void QGUIDialogMod::setProcReceiver()
{
    pfGUIDialogMod* dlgMod = pfGUIDialogMod::Convert(fCreatable);
    QFindKeyDialog dlg(this);
    if (dlgMod->getProcReceiver().Exists())
        dlg.init(PrpShopMain::ResManager(), dlgMod->getProcReceiver());
    else
        dlg.init(PrpShopMain::ResManager(), dlgMod->getKey()->getLocation(), kPythonFileMod);
    if (dlg.exec() == QDialog::Accepted) {
        dlgMod->setProcReceiver(dlg.selection());
        fProcReceiver->setKey(dlgMod->getProcReceiver());
    }
}

void QGUIDialogMod::setSceneNode()
{
    pfGUIDialogMod* dlgMod = pfGUIDialogMod::Convert(fCreatable);
    QFindKeyDialog dlg(this);
    if (dlgMod->getSceneNode().Exists())
        dlg.init(PrpShopMain::ResManager(), dlgMod->getSceneNode());
    else
        dlg.init(PrpShopMain::ResManager(), dlgMod->getKey()->getLocation(), kSceneNode);
    if (dlg.exec() == QDialog::Accepted) {
        dlgMod->setSceneNode(dlg.selection());
        fSceneNode->setKey(dlgMod->getSceneNode());
    }
}

void QGUIDialogMod::unsetRenderMod()
{
    pfGUIDialogMod* dlgMod = pfGUIDialogMod::Convert(fCreatable);
    dlgMod->setRenderMod(plKey());
    fRenderMod->setCreatable(NULL, "(None)");
}

void QGUIDialogMod::unsetProcReceiver()
{
    pfGUIDialogMod* dlgMod = pfGUIDialogMod::Convert(fCreatable);
    dlgMod->setProcReceiver(plKey());
    fProcReceiver->setCreatable(NULL, "(None)");
}

void QGUIDialogMod::unsetSceneNode()
{
    pfGUIDialogMod* dlgMod = pfGUIDialogMod::Convert(fCreatable);
    dlgMod->setSceneNode(plKey());
    fSceneNode->setCreatable(NULL, "(None)");
}
