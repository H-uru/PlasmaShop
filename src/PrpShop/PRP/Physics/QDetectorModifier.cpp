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

#include "QDetectorModifier.h"

#include <QLabel>
#include <QGridLayout>
#include "QKeyDialog.h"
#include "Main.h"

QDetectorModifier::QDetectorModifier(plCreatable* pCre, QWidget* parent)
    : QCreatable(pCre, pCre->ClassIndex(), parent)
{
    plDetectorModifier* obj = plDetectorModifier::Convert(fCreatable);

    fSynchObjLink = new QCreatableLink(this, false);
    fSynchObjLink->setCreatable(obj, tr("Synch Flags"));
    fSynchObjLink->setForceType(kSynchedObject);

    fReceivers = new QKeyList(obj->getKey(), this);
    for (size_t i=0; i<obj->getReceivers().size(); i++)
        fReceivers->addKey(obj->getReceivers()[i]);

    fRemoteMod = new QCreatableLink(this);
    fRemoteMod->setKey(obj->getRemoteMod());

    fProxy = new QCreatableLink(this);
    fProxy->setKey(obj->getProxy());

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(fSynchObjLink, 0, 0, 1, 2);
    layout->addWidget(new QLabel(tr("Receivers:"), this), 1, 0, 1, 2);
    layout->addWidget(fReceivers, 2, 0, 1, 2);
    layout->addWidget(new QLabel(tr("Remote Mod:"), this), 3, 0, 1, 1);
    layout->addWidget(fRemoteMod, 3, 1, 1, 1);
    layout->addWidget(new QLabel(tr("Proxy:"), this), 4, 0, 1, 1);
    layout->addWidget(fProxy, 4, 1, 1, 1);
    fReceivers->adjustColumns();

    connect(fRemoteMod, &QCreatableLink::addObject, this, &QDetectorModifier::setRemoteMod);
    connect(fRemoteMod, &QCreatableLink::delObject, this, &QDetectorModifier::unsetRemoteMod);
    connect(fProxy, &QCreatableLink::addObject, this, &QDetectorModifier::setProxy);
    connect(fProxy, &QCreatableLink::delObject, this, &QDetectorModifier::unsetProxy);
}

void QDetectorModifier::saveDamage()
{
    plDetectorModifier* obj = plDetectorModifier::Convert(fCreatable);

    obj->clearReceivers();
    QList<plKey> recs = fReceivers->keys();
    while (!recs.isEmpty())
        obj->addReceiver(recs.takeFirst());
}

void QDetectorModifier::setRemoteMod()
{
    plDetectorModifier* obj = plDetectorModifier::Convert(fCreatable);
    QFindKeyDialog dlg(this);
    if (obj->getRemoteMod().Exists())
        dlg.init(PrpShopMain::ResManager(), obj->getRemoteMod());
    else
        dlg.init(PrpShopMain::ResManager(), obj->getKey()->getLocation());
    if (dlg.exec() == QDialog::Accepted) {
        obj->setRemoteMod(dlg.selection());
        fRemoteMod->setKey(obj->getRemoteMod());
    }
}

void QDetectorModifier::setProxy()
{
    plDetectorModifier* obj = plDetectorModifier::Convert(fCreatable);
    QFindKeyDialog dlg(this);
    if (obj->getProxy().Exists())
        dlg.init(PrpShopMain::ResManager(), obj->getProxy());
    else
        dlg.init(PrpShopMain::ResManager(), obj->getKey()->getLocation());
    if (dlg.exec() == QDialog::Accepted) {
        obj->setProxy(dlg.selection());
        fProxy->setKey(obj->getProxy());
    }
}

void QDetectorModifier::unsetRemoteMod()
{
    plDetectorModifier* obj = plDetectorModifier::Convert(fCreatable);
    obj->setRemoteMod(plKey());
    fRemoteMod->setCreatable(NULL, "(None)");
}

void QDetectorModifier::unsetProxy()
{
    plDetectorModifier* obj = plDetectorModifier::Convert(fCreatable);
    obj->setProxy(plKey());
    fProxy->setCreatable(NULL, "(None)");
}
