#include "QDrawInterface.h"

#include <QGroupBox>
#include <QLabel>
#include <QGridLayout>
#include <QMenu>
#include <QDialogButtonBox>
#include "../../QKeyDialog.h"
#include "../../Main.h"

/* QDrawableList */
QDrawableList::QDrawableList(const plLocation& loc, QWidget* parent)
             : QKeyList(parent)
{
    fLocation = loc;

    setColumnCount(2);
    setUniformRowHeights(true);
    setRootIsDecorated(false);
    headerItem()->setText(0, tr("Key"));
    headerItem()->setText(1, tr("Drawable"));
}

void QDrawableList::addKey(plKey key, int dkey)
{
    QTreeWidgetItem* item = new QTreeWidgetItem(this,
        QStringList() << QString("%1").arg(dkey) << ~key->getName());
    item->setIcon(1, pqGetTypeIcon(key->getType()));
    fKeys << key;
    fDrawKeys << dkey;
}

QList<int> QDrawableList::drawKeys() const
{ return fDrawKeys; }

void QDrawableList::contextMenuEvent(QContextMenuEvent* evt)
{
    QMenu menu(this);
    QAction* addObjItem = menu.addAction(tr("Add Object"));
    QAction* delObjItem = menu.addAction(tr("Remove Object"));

    if (currentItem() == NULL)
        delObjItem->setEnabled(false);

    QAction* sel = menu.exec(evt->globalPos());
    if (sel == addObjItem) {
        QFindDrawKeyDialog dlg(this);
        dlg.init(PrpShopMain::ResManager(), fLocation);
        if (dlg.exec() == QDialog::Accepted)
            addKey(dlg.selection(), dlg.drawKey());
    } else if (sel == delObjItem) {
        delItem(indexOfTopLevelItem(currentItem()));
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

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

void QFindDrawKeyDialog::init(plResManager* mgr, const plLocation& loc)
{
    fResMgr = mgr;
    fLocation = loc;

    fKeyBox->clear();
    fKeys = fResMgr->getKeys(fLocation, kDrawableSpans);
    for (size_t i=0; i<fKeys.size(); i++)
        fKeyBox->addItem(~fKeys[i]->getName());
    fDrawKey->setValue(0);
}

plKey QFindDrawKeyDialog::selection() const
{ return fKeys[fKeyBox->currentIndex()]; }

int QFindDrawKeyDialog::drawKey() const
{ return fDrawKey->value(); }


/* QDrawInterface */
QDrawInterface::QDrawInterface(plCreatable* pCre, QWidget* parent)
              : QCreatable(pCre, kDrawInterface, parent)
{
    plDrawInterface* intf = (plDrawInterface*)fCreatable;

    fOwnerLink = new QCreatableLink(this);
    fOwnerLink->setText(tr("Owner Object"));
    fOwnerLink->setKey(intf->getOwner());

    fSynchObjLink = new QCreatableLink(this, false);
    fSynchObjLink->setText(tr("Synch Flags"));
    fSynchObjLink->setCreatable(intf);
    fSynchObjLink->setForceType(kSynchedObject);

    QGroupBox* grpProps = new QGroupBox(tr("Properties"), this);
    QGridLayout* layProps = new QGridLayout(grpProps);
    layProps->setVerticalSpacing(0);
    fDisabled = new QCheckBox(tr("Disable"), grpProps);
    layProps->addWidget(fDisabled, 0, 0);
    fDisabled->setChecked(intf->getProperty(plDrawInterface::kDisable));

    QTabWidget* childTab = new QTabWidget(this);

    fDrawKeys = new QDrawableList(intf->getKey()->getLocation(), childTab);
    for (size_t i=0; i<intf->getNumDrawables(); i++)
        fDrawKeys->addKey(intf->getDrawable(i), intf->getDrawableKey(i));

    fRegions = new QKeyList(childTab);
    for (size_t i=0; i<intf->getRegions().getSize(); i++)
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

    connect(fOwnerLink, SIGNAL(addObject()), this, SLOT(setOwner()));
    connect(fOwnerLink, SIGNAL(delObject()), this, SLOT(unsetOwner()));
}

void QDrawInterface::saveDamage()
{
    plDrawInterface* intf = (plDrawInterface*)fCreatable;
    intf->setProperty(plDrawInterface::kDisable, fDisabled->isChecked());

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
    plDrawInterface* intf = (plDrawInterface*)fCreatable;
    QFindKeyDialog dlg(this);
    if (intf->getOwner().Exists())
        dlg.init(PrpShopMain::ResManager(), intf->getOwner());
    else
        dlg.init(PrpShopMain::ResManager(), intf->getKey()->getLocation(), kSceneObject);
    if (dlg.exec() == QDialog::Accepted) {
        intf->setOwner(dlg.selection());
        fOwnerLink->setKey(intf->getOwner());
    }
}

void QDrawInterface::unsetOwner()
{
    plDrawInterface* intf = (plDrawInterface*)fCreatable;
    intf->setOwner(plKey());
    fOwnerLink->setCreatable(NULL);
}
