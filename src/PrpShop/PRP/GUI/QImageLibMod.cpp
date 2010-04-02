#include "QImageLibMod.h"
#include <QGridLayout>

/* QImageLibMod */
QImageLibMod::QImageLibMod(plCreatable* pCre, QWidget* parent)
            : QCreatable(pCre, kImageLibMod, parent)
{
    plImageLibMod* mod = (plImageLibMod*)fCreatable;

    fSynchObjLink = new QCreatableLink(this, false);
    fSynchObjLink->setText(tr("Synch Flags"));
    fSynchObjLink->setCreatable(mod);
    fSynchObjLink->setForceType(kSynchedObject);

    fImages = new QKeyList(this);
    for (size_t i=0; i<mod->getImages().getSize(); i++)
        fImages->addKey(mod->getImages()[i]);

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(fSynchObjLink, 0, 0);
    layout->addWidget(new QLabel(tr("Images:"), this), 1, 0);
    layout->addWidget(fImages, 2, 0);

    fImages->adjustColumns();
}

void QImageLibMod::saveDamage()
{
    plImageLibMod* mod = (plImageLibMod*)fCreatable;

    mod->clearImages();
    QList<plKey> imgKeys = fImages->keys();
    while (!imgKeys.isEmpty())
        mod->addImage(imgKeys.takeFirst());
}
