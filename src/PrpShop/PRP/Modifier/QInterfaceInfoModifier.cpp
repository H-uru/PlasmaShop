#include "QInterfaceInfoModifier.h"

#include <QLabel>
#include <QGridLayout>

QInterfaceInfoModifier::QInterfaceInfoModifier(plCreatable* pCre, QWidget* parent)
                      : QCreatable(pCre, kInterfaceInfoModifier, parent)
{
    plInterfaceInfoModifier* obj = (plInterfaceInfoModifier*)fCreatable;

    fSynchObjLink = new QCreatableLink(this, false);
    fSynchObjLink->setText(tr("Synch Flags"));
    fSynchObjLink->setCreatable(obj);
    fSynchObjLink->setForceType(kSynchedObject);

    fIntfKeys = new QKeyList(this);
    for (size_t i=0; i<obj->getIntfKeys().getSize(); i++)
        fIntfKeys->addKey(obj->getIntfKeys()[i]);

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(fSynchObjLink, 0, 0);
    layout->addWidget(new QLabel(tr("Interface Keys:"), this), 1, 0);
    layout->addWidget(fIntfKeys, 2, 0);
    fIntfKeys->adjustColumns();
}

void QInterfaceInfoModifier::saveDamage()
{
    plInterfaceInfoModifier* obj = (plInterfaceInfoModifier*)fCreatable;

    obj->clearIntfKeys();
    QList<plKey> keys = fIntfKeys->keys();
    while (!keys.isEmpty())
        obj->addIntfKey(keys.takeFirst());
}
