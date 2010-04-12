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
#include "../../Main.h"
#include "../../QKeyDialog.h"

/* QGUIDialogMod */
QGUIDialogMod::QGUIDialogMod(plCreatable* pCre, QWidget* parent)
             : QCreatable(pCre, kGUIDialogMod, parent)
{
    pfGUIDialogMod* dlg = (pfGUIDialogMod*)fCreatable;

    fSynchObjLink = new QCreatableLink(this, false);
    fSynchObjLink->setText(tr("Synch Flags"));
    fSynchObjLink->setCreatable(dlg);
    fSynchObjLink->setForceType(kSynchedObject);

    QGroupBox* grpFlags = new QGroupBox(tr("Flags"), this);
    fModFlagModal = new QCheckBox(tr("Modal"), grpFlags);
    QGridLayout* flagLayout = new QGridLayout(grpFlags);
    flagLayout->setContentsMargins(8, 8, 8, 8);
    flagLayout->addWidget(fModFlagModal, 0, 0);
    fModFlagModal->setChecked(dlg->getFlag(pfGUIDialogMod::kModal));

    QGroupBox* colorSchemeGrp = new QGroupBox(tr("Color Scheme"), this);
    fColorScheme = new QGUIColorScheme(colorSchemeGrp);
    fColorScheme->setColorScheme(&dlg->getColorScheme());
    QGridLayout* skinLayout = new QGridLayout(colorSchemeGrp);
    skinLayout->setContentsMargins(0, 0, 0, 0);
    skinLayout->addWidget(fColorScheme, 0, 0);

    fName = new QLineEdit(this);
    fName->setText(~dlg->getName());
    fTagID = new QIntEdit(this);
    fTagID->setRange(-0x80000000, 0x7FFFFFFF);
    fTagID->setValue(dlg->getTagID());
    fVersion = new QIntEdit(this);
    fVersion->setRange(-0x80000000, 0x7FFFFFFF);
    fVersion->setValue(dlg->getVersion());

    fControls = new QKeyList(this);
    for (size_t i=0; i<dlg->getControls().getSize(); i++)
        fControls->addKey(dlg->getControls()[i]);

    fRenderMod = new QCreatableLink(this);
    fRenderMod->setKey(dlg->getRenderMod());
    fRenderMod->setText(dlg->getRenderMod().Exists()
                        ? ~dlg->getRenderMod()->getName()
                        : "(None)");

    fProcReceiver = new QCreatableLink(this);
    fProcReceiver->setKey(dlg->getProcReceiver());
    fProcReceiver->setText(dlg->getProcReceiver().Exists()
                           ? ~dlg->getProcReceiver()->getName()
                           : "(None)");

    fSceneNode = new QCreatableLink(this);
    fSceneNode->setKey(dlg->getSceneNode());
    fSceneNode->setText(dlg->getSceneNode().Exists()
                        ? ~dlg->getSceneNode()->getName()
                        : "(None)");

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

    connect(fRenderMod, SIGNAL(addObject()), this, SLOT(setRenderMod()));
    connect(fRenderMod, SIGNAL(delObject()), this, SLOT(unsetRenderMod()));
    connect(fProcReceiver, SIGNAL(addObject()), this, SLOT(setProcReceiver()));
    connect(fProcReceiver, SIGNAL(delObject()), this, SLOT(unsetProcReceiver()));
    connect(fSceneNode, SIGNAL(addObject()), this, SLOT(setSceneNode()));
    connect(fSceneNode, SIGNAL(delObject()), this, SLOT(unsetSceneNode()));
    fControls->adjustColumns();
}

void QGUIDialogMod::saveDamage()
{
    pfGUIDialogMod* dlg = (pfGUIDialogMod*)fCreatable;

    dlg->setFlag(pfGUIDialogMod::kModal, fModFlagModal->isChecked());
    fColorScheme->saveColorScheme(&dlg->getColorScheme());
    dlg->setName(~fName->text());
    dlg->setTagID(fTagID->value());
    dlg->setVersion(fVersion->value());

    dlg->clearControls();
    QList<plKey> ctrlKeys = fControls->keys();
    while (!ctrlKeys.isEmpty())
        dlg->addControl(ctrlKeys.takeFirst());
}

void QGUIDialogMod::setRenderMod()
{
    pfGUIDialogMod* dlgMod = (pfGUIDialogMod*)fCreatable;
    QFindKeyDialog dlg(this);
    if (dlgMod->getRenderMod().Exists())
        dlg.init(PrpShopMain::ResManager(), dlgMod->getRenderMod());
    else
        dlg.init(PrpShopMain::ResManager(), dlgMod->getKey()->getLocation(), kPostEffectMod);
    if (dlg.exec() == QDialog::Accepted) {
        dlgMod->setRenderMod(dlg.selection());
        fRenderMod->setKey(dlgMod->getRenderMod());
        fRenderMod->setText(~dlgMod->getRenderMod()->getName());
    }
}

void QGUIDialogMod::setProcReceiver()
{
    pfGUIDialogMod* dlgMod = (pfGUIDialogMod*)fCreatable;
    QFindKeyDialog dlg(this);
    if (dlgMod->getProcReceiver().Exists())
        dlg.init(PrpShopMain::ResManager(), dlgMod->getProcReceiver());
    else
        dlg.init(PrpShopMain::ResManager(), dlgMod->getKey()->getLocation(), kPythonFileMod);
    if (dlg.exec() == QDialog::Accepted) {
        dlgMod->setProcReceiver(dlg.selection());
        fProcReceiver->setKey(dlgMod->getProcReceiver());
        fProcReceiver->setText(~dlgMod->getProcReceiver()->getName());
    }
}

void QGUIDialogMod::setSceneNode()
{
    pfGUIDialogMod* dlgMod = (pfGUIDialogMod*)fCreatable;
    QFindKeyDialog dlg(this);
    if (dlgMod->getSceneNode().Exists())
        dlg.init(PrpShopMain::ResManager(), dlgMod->getSceneNode());
    else
        dlg.init(PrpShopMain::ResManager(), dlgMod->getKey()->getLocation(), kSceneNode);
    if (dlg.exec() == QDialog::Accepted) {
        dlgMod->setSceneNode(dlg.selection());
        fSceneNode->setKey(dlgMod->getSceneNode());
        fSceneNode->setText(~dlgMod->getSceneNode()->getName());
    }
}

void QGUIDialogMod::unsetRenderMod()
{
    pfGUIDialogMod* dlgMod = (pfGUIDialogMod*)fCreatable;
    dlgMod->setRenderMod(plKey());
    fRenderMod->setCreatable(NULL);
    fRenderMod->setText("(None)");
}

void QGUIDialogMod::unsetProcReceiver()
{
    pfGUIDialogMod* dlgMod = (pfGUIDialogMod*)fCreatable;
    dlgMod->setProcReceiver(plKey());
    fProcReceiver->setCreatable(NULL);
    fProcReceiver->setText("(None)");
}

void QGUIDialogMod::unsetSceneNode()
{
    pfGUIDialogMod* dlgMod = (pfGUIDialogMod*)fCreatable;
    dlgMod->setSceneNode(plKey());
    fSceneNode->setCreatable(NULL);
    fSceneNode->setText("(None)");
}
