#include "QSpawnModifier.h"

#include <QGridLayout>
#include <PRP/Modifier/plSpawnModifier.h>

QSpawnModifier::QSpawnModifier(plCreatable* pCre, QWidget* parent)
              : QCreatable(pCre, kSpawnModifier, parent)
{
    plSpawnModifier* obj = (plSpawnModifier*)fCreatable;

    fSynchObjLink = new QCreatableLink(this, false);
    fSynchObjLink->setText(tr("Synch Flags"));
    fSynchObjLink->setCreatable(obj);
    fSynchObjLink->setForceType(kSynchedObject);

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(fSynchObjLink, 0, 0);
}

void QSpawnModifier::saveDamage()
{ }
