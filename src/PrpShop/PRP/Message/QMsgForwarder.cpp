#include "QMsgForwarder.h"

#include <QLabel>
#include <QGridLayout>

QMsgForwarder::QMsgForwarder(plCreatable* pCre, QWidget* parent)
             : QCreatable(pCre, kMsgForwarder, parent)
{
    plMsgForwarder* obj = (plMsgForwarder*)fCreatable;

    fForwardKeys = new QKeyList(this);
    for (size_t i=0; i<obj->getForwardKeys().getSize(); i++)
        fForwardKeys->addKey(obj->getForwardKeys()[i]);

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(new QLabel(tr("Forward Keys:"), this), 0, 0);
    layout->addWidget(fForwardKeys, 1, 0);
    fForwardKeys->adjustColumns();
}

void QMsgForwarder::saveDamage()
{
    plMsgForwarder* obj = (plMsgForwarder*)fCreatable;

    obj->clearForwardKeys();
    QList<plKey> keys = fForwardKeys->keys();
    while (!keys.isEmpty())
        obj->addForwardKey(keys.takeFirst());
}
