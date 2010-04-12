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
    for (size_t i=0; i<ctrl->getTextMaps().getSize(); i++)
        fTextMaps->addKey(ctrl->getTextMaps()[i]);
    fLayers = new QKeyList(keyTabs);
    for (size_t i=0; i<ctrl->getLayers().getSize(); i++)
        fLayers->addKey(ctrl->getLayers()[i]);
    fMaterials = new QKeyList(keyTabs);
    for (size_t i=0; i<ctrl->getMaterials().getSize(); i++)
        fMaterials->addKey(ctrl->getMaterials()[i]);
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
