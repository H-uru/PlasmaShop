#include "QGUIDynDisplayCtrl.h"

#include <QTabWidget>
#include <QGridLayout>

/* QGUIDynDisplayCtrl */
QGUIDynDisplayCtrl::QGUIDynDisplayCtrl(plCreatable* pCre, QWidget* parent)
                  : QCreatable(pCre, kGUIDynDisplayCtrl, parent)
{
    pfGUIDynDisplayCtrl* ctrl = (pfGUIDynDisplayCtrl*)fCreatable;

    fControlModLink = new QCreatableLink(this, false);
    fControlModLink->setText(tr("GUI Control Common Properties"));
    fControlModLink->setCreatable(ctrl);
    fControlModLink->setForceType(kGUIControlMod);

    QTabWidget* keyTabs = new QTabWidget(this);
    fTextMaps = new QKeyList(keyTabs);
    for (size_t i=0; i<ctrl->getNumTextMaps(); i++)
        fTextMaps->addKey(ctrl->getTextMap(i));
    fLayers = new QKeyList(keyTabs);
    for (size_t i=0; i<ctrl->getNumLayers(); i++)
        fLayers->addKey(ctrl->getLayer(i));
    fMaterials = new QKeyList(keyTabs);
    for (size_t i=0; i<ctrl->getNumMaterials(); i++)
        fMaterials->addKey(ctrl->getMaterial(i));
    keyTabs->addTab(fTextMaps, tr("Text Maps"));
    keyTabs->addTab(fLayers, tr("Layers"));
    keyTabs->addTab(fMaterials, tr("Materials"));

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(fControlModLink, 0, 0);
    layout->addWidget(keyTabs, 1, 0);

    fTextMaps->adjustColumns();
    fLayers->adjustColumns();
    fMaterials->adjustColumns();
}

void QGUIDynDisplayCtrl::saveDamage()
{
    pfGUIDynDisplayCtrl* ctrl = (pfGUIDynDisplayCtrl*)fCreatable;

    ctrl->clearTextMaps();
    QList<plKey> mapKeys = fTextMaps->keys();
    while (!mapKeys.isEmpty())
        ctrl->addTextMap(mapKeys.takeFirst());

    ctrl->clearLayers();
    QList<plKey> layKeys = fLayers->keys();
    while (!layKeys.isEmpty())
        ctrl->addLayer(layKeys.takeFirst());

    ctrl->clearMaterials();
    QList<plKey> matKeys = fMaterials->keys();
    while (!matKeys.isEmpty())
        ctrl->addMaterial(matKeys.takeFirst());
}
