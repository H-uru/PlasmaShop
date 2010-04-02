#include "QDetectorModifier.h"

#include <QLabel>
#include <QGridLayout>
#include "../../QKeyDialog.h"
#include "../../Main.h"

QDetectorModifier::QDetectorModifier(plCreatable* pCre, QWidget* parent)
                 : QCreatable(pCre, pCre->ClassIndex(), parent)
{
    plDetectorModifier* obj = (plDetectorModifier*)fCreatable;

    fSynchObjLink = new QCreatableLink(this, false);
    fSynchObjLink->setText(tr("Synch Flags"));
    fSynchObjLink->setCreatable(obj);
    fSynchObjLink->setForceType(kSynchedObject);

    fReceivers = new QKeyList(this);
    for (size_t i=0; i<obj->getReceivers().getSize(); i++)
        fReceivers->addKey(obj->getReceivers()[i]);

    fRemoteMod = new QCreatableLink(this);
    fRemoteMod->setKey(obj->getRemoteMod());
    fRemoteMod->setText(obj->getRemoteMod().Exists()
                        ? ~obj->getRemoteMod()->getName()
                        : "(None)");

    fProxy = new QCreatableLink(this);
    fProxy->setKey(obj->getProxy());
    fProxy->setText(obj->getProxy().Exists()
                      ? ~obj->getProxy()->getName()
                      : "(None)");

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

    connect(fRemoteMod, SIGNAL(addObject()), this, SLOT(setRemoteMod()));
    connect(fRemoteMod, SIGNAL(delObject()), this, SLOT(unsetRemoteMod()));
    connect(fProxy, SIGNAL(addObject()), this, SLOT(setProxy()));
    connect(fProxy, SIGNAL(delObject()), this, SLOT(unsetProxy()));
}

void QDetectorModifier::saveDamage()
{
    plDetectorModifier* obj = (plDetectorModifier*)fCreatable;

    obj->clearReceivers();
    QList<plKey> recs = fReceivers->keys();
    while (!recs.isEmpty())
        obj->addReceiver(recs.takeFirst());
}

void QDetectorModifier::setRemoteMod()
{
    plDetectorModifier* obj = (plDetectorModifier*)fCreatable;
    QFindKeyDialog dlg(this);
    if (obj->getRemoteMod().Exists())
        dlg.init(PrpShopMain::ResManager(), obj->getRemoteMod());
    else
        dlg.init(PrpShopMain::ResManager(), obj->getKey()->getLocation());
    if (dlg.exec() == QDialog::Accepted) {
        obj->setRemoteMod(dlg.selection());
        fRemoteMod->setKey(obj->getRemoteMod());
        fRemoteMod->setText(~obj->getRemoteMod()->getName());
    }
}

void QDetectorModifier::setProxy()
{
    plDetectorModifier* obj = (plDetectorModifier*)fCreatable;
    QFindKeyDialog dlg(this);
    if (obj->getProxy().Exists())
        dlg.init(PrpShopMain::ResManager(), obj->getProxy());
    else
        dlg.init(PrpShopMain::ResManager(), obj->getKey()->getLocation());
    if (dlg.exec() == QDialog::Accepted) {
        obj->setProxy(dlg.selection());
        fProxy->setKey(obj->getProxy());
        fProxy->setText(~obj->getProxy()->getName());
    }
}

void QDetectorModifier::unsetRemoteMod()
{
    plDetectorModifier* obj = (plDetectorModifier*)fCreatable;
    obj->setRemoteMod(plKey());
    fRemoteMod->setCreatable(NULL);
    fRemoteMod->setText("(None)");
}

void QDetectorModifier::unsetProxy()
{
    plDetectorModifier* obj = (plDetectorModifier*)fCreatable;
    obj->setProxy(plKey());
    fProxy->setCreatable(NULL);
    fProxy->setText("(None)");
}
