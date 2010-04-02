#include "QAudible.h"

#include <QGridLayout>
#include <PRP/Audio/plAudible.h>
#include "../../QKeyDialog.h"
#include "../../Main.h"

/* QAudible */
QAudible::QAudible(plCreatable* pCre, QWidget* parent)
        : QCreatable(pCre, pCre->ClassIndex(), parent)
{
    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(new QLabel(tr("No editable fields"), this), 0, 0);
}

void QAudible::saveDamage()
{ }


/* QWinAudible */
QWinAudible::QWinAudible(plCreatable* pCre, QWidget* parent)
           : QCreatable(pCre, pCre->ClassIndex(), parent)
{
    plWinAudible* obj = (plWinAudible*)fCreatable;

    fSoundKeys = new QKeyList(this);
    for (size_t i=0; i<obj->getSounds().getSize(); i++)
        fSoundKeys->addKey(obj->getSounds()[i]);

    fSceneNode = new QCreatableLink(this);
    fSceneNode->setText(tr("Scene Node"));
    fSceneNode->setKey(obj->getSceneNode());

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(new QLabel(tr("Sounds:"), this), 0, 0);
    layout->addWidget(fSoundKeys, 1, 0);
    layout->addWidget(fSceneNode, 2, 0);
    fSoundKeys->adjustColumns();

    connect(fSceneNode, SIGNAL(addObject()), this, SLOT(setSceneNode()));
    connect(fSceneNode, SIGNAL(delObject()), this, SLOT(unsetSceneNode()));
}

void QWinAudible::saveDamage()
{
    plWinAudible* obj = (plWinAudible*)fCreatable;

    obj->clearSounds();
    QList<plKey> keys = fSoundKeys->keys();
    while (!keys.isEmpty())
        obj->addSound(keys.takeFirst());
}

void QWinAudible::setSceneNode()
{
    plWinAudible* obj = (plWinAudible*)fCreatable;
    QFindKeyDialog dlg(this);
    if (obj->getSceneNode().Exists())
        dlg.init(PrpShopMain::ResManager(), obj->getSceneNode());
    else
        dlg.init(PrpShopMain::ResManager(), obj->getKey()->getLocation(), kSceneNode);
    if (dlg.exec() == QDialog::Accepted) {
        obj->setSceneNode(dlg.selection());
        fSceneNode->setKey(obj->getSceneNode());
    }
}

void QWinAudible::unsetSceneNode()
{
    plWinAudible* obj = (plWinAudible*)fCreatable;
    obj->setSceneNode(plKey());
    fSceneNode->setCreatable(NULL);
}
