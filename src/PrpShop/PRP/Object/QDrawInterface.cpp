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

#include "QDrawInterface.h"

#include <PRP/Geometry/plDrawableSpans.h>
#include <QGroupBox>
#include <QLabel>
#include <QGridLayout>
#include <QMenu>
#include <QDialogButtonBox>
#include "QKeyDialog.h"
#include "Main.h"

/* QDrawableList */
QDrawableList::QDrawableList(plKey container, QWidget* parent)
    : QKeyList(container, parent)
{
    setColumnCount(2);
    setUniformRowHeights(true);
    setRootIsDecorated(false);
    headerItem()->setText(0, tr("Key"));
    headerItem()->setText(1, tr("Drawable"));
}

void QDrawableList::addKey(plKey key, int dkey)
{
    QTreeWidgetItem* item = new QTreeWidgetItem(this,
        QStringList() << QString("%1").arg(dkey) << pqFormatKeyName(key));
    item->setIcon(1, pqGetTypeIcon(key->getType()));
    fKeys << key;
    fDrawKeys << dkey;
}

void QDrawableList::contextMenuEvent(QContextMenuEvent* evt)
{
    QMenu menu(this);
    QAction* addObjItem = menu.addAction(tr("Add Object"));
    QAction* delObjItem = menu.addAction(tr("Remove Object"));
    menu.addSeparator();
    QAction* openMatItem = menu.addAction(tr("Open Materials"));

    if (currentItem() == NULL)
        delObjItem->setEnabled(false);

    QAction* sel = menu.exec(evt->globalPos());
    if (sel == addObjItem) {
        QFindDrawKeyDialog dlg(this);
        dlg.init(PrpShopMain::ResManager(), fContainer->getLocation());
        if (dlg.exec() == QDialog::Accepted)
            addKey(dlg.selection(), dlg.drawKey());
    } else if (sel == delObjItem) {
        delItem(indexOfTopLevelItem(currentItem()));
    } else if (sel == openMatItem) {
        plResManager* mgr = PrpShopMain::ResManager();
        int idx = indexOfTopLevelItem(currentItem());
        plDrawableSpans* dspans = plDrawableSpans::Convert(mgr->getObject(fKeys[idx]));
        plDISpanIndex dis = dspans->getDIIndex(fDrawKeys[idx]);

        std::vector<plKey> mats = dspans->getMaterials();
        for (size_t i = 0; i < dis.fIndices.size(); i++) {
            plIcicle* ice = dspans->getIcicle(dis.fIndices[i]);
            hsKeyedObject* mat = mgr->getObject(mats[ice->getMaterialIdx()]);
            PrpShopMain::Instance()->editCreatable(mat);
        }
    }
}


/* QFindDrawKeyDialog */
QFindDrawKeyDialog::QFindDrawKeyDialog(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle(tr("Add Drawable..."));

    fKeyBox = new QComboBox(this);
    fDrawKey = new QSpinBox(this);
    fDrawKey->setRange(-0x80000000, 0x7FFFFFFF);
    QDialogButtonBox* buttonBox = new QDialogButtonBox(Qt::Horizontal, this);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel |
                                  QDialogButtonBox::Ok);

    QGridLayout* layout = new QGridLayout(this);
    layout->setHorizontalSpacing(8);
    layout->setVerticalSpacing(8);
    layout->addWidget(new QLabel(tr("Drawable:"), this), 0, 0);
    layout->addWidget(fKeyBox, 0, 1);
    layout->addWidget(new QLabel(tr("DI Key:"), this), 1, 0);
    layout->addWidget(fDrawKey, 1, 1);
    layout->addWidget(buttonBox, 2, 0, 1, 2);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

void QFindDrawKeyDialog::init(plResManager* mgr, const plLocation& loc)
{
    fResMgr = mgr;
    fLocation = loc;

    fKeyBox->clear();
    fKeys = fResMgr->getKeys(fLocation, kDrawableSpans);
    for (size_t i=0; i<fKeys.size(); i++)
        fKeyBox->addItem(pqFormatKeyName(fKeys[i]));
    fDrawKey->setValue(0);
}


/* QDrawInterface */
QDrawInterface::QDrawInterface(plCreatable* pCre, QWidget* parent)
    : QCreatable(pCre, kDrawInterface, parent)
{
    plDrawInterface* intf = plDrawInterface::Convert(fCreatable);

    fOwnerLink = new QCreatableLink(this);
    fOwnerLink->setKey(intf->getOwner(), false);
    fOwnerLink->setText(tr("Owner Object"));

    fSynchObjLink = new QCreatableLink(this, false);
    fSynchObjLink->setCreatable(intf, tr("Synch Flags"));
    fSynchObjLink->setForceType(kSynchedObject);

    QGroupBox* grpProps = new QGroupBox(tr("Properties"), this);
    QGridLayout* layProps = new QGridLayout(grpProps);
    layProps->setVerticalSpacing(0);
    fDisabled = new QCheckBox(tr("Disable"), grpProps);
    layProps->addWidget(fDisabled, 0, 0);
    fDisabled->setChecked(intf->getProperty(plDrawInterface::kDisable));

    connect(fDisabled, &QCheckBox::clicked, this, [this](bool checked) {
        plDrawInterface* intf = plDrawInterface::Convert(fCreatable);
        intf->setProperty(plDrawInterface::kDisable, checked);
    });

    QTabWidget* childTab = new QTabWidget(this);

    fDrawKeys = new QDrawableList(intf->getKey(), childTab);
    for (size_t i=0; i<intf->getNumDrawables(); i++)
        fDrawKeys->addKey(intf->getDrawable(i), intf->getDrawableKey(i));

    fRegions = new QKeyList(intf->getKey(), childTab);
    for (size_t i=0; i<intf->getRegions().size(); i++)
        fRegions->addKey(intf->getRegions()[i]);

    childTab->addTab(fDrawKeys, tr("Drawables"));
    childTab->addTab(fRegions, tr("Regions"));

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(fOwnerLink, 0, 0);
    layout->addWidget(fSynchObjLink, 1, 0);
    layout->addWidget(grpProps, 2, 0);
    layout->addWidget(childTab, 3, 0);
    fDrawKeys->adjustColumns();
    fRegions->adjustColumns();

    connect(fOwnerLink, &QCreatableLink::addObject, this, &QDrawInterface::setOwner);
    connect(fOwnerLink, &QCreatableLink::delObject, this, &QDrawInterface::unsetOwner);
}

void QDrawInterface::saveDamage()
{
    plDrawInterface* intf = plDrawInterface::Convert(fCreatable);

    intf->clearDrawables();
    QList<plKey> drawables = fDrawKeys->keys();
    QList<int> dkeys = fDrawKeys->drawKeys();
    while (!drawables.isEmpty())
        intf->addDrawable(drawables.takeFirst(), dkeys.takeFirst());

    intf->clearRegions();
    QList<plKey> regions = fRegions->keys();
    while (!regions.isEmpty())
        intf->addRegion(regions.takeFirst());
}

void QDrawInterface::setOwner()
{
    plDrawInterface* intf = plDrawInterface::Convert(fCreatable);
    QFindKeyDialog dlg(this);
    if (intf->getOwner().Exists())
        dlg.init(PrpShopMain::ResManager(), intf->getOwner());
    else
        dlg.init(PrpShopMain::ResManager(), intf->getKey()->getLocation(), kSceneObject);
    if (dlg.exec() == QDialog::Accepted) {
        intf->setOwner(dlg.selection());
        fOwnerLink->setKey(intf->getOwner(), false);
    }
}

void QDrawInterface::unsetOwner()
{
    plDrawInterface* intf = plDrawInterface::Convert(fCreatable);
    intf->setOwner(plKey());
    fOwnerLink->setCreatable(NULL);
}
