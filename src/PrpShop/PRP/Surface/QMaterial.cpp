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

#include "QMaterial.h"

#include <QGroupBox>
#include <QLabel>
#include <QTabWidget>
#include <QGridLayout>

QMaterial::QMaterial(plCreatable* pCre, QWidget* parent)
    : QCreatable(pCre, kGMaterial, parent)
{
    hsGMaterial* mat = hsGMaterial::Convert(fCreatable);

    fSynchObjLink = new QCreatableLink(this, false);
    fSynchObjLink->setCreatable(mat, tr("Synch Flags"));
    fSynchObjLink->setForceType(kSynchedObject);

    QGroupBox* grpCompFlags = new QGroupBox(tr("Composition Flags"), this);
    QGridLayout* layCompFlags = new QGridLayout(grpCompFlags);
    layCompFlags->setVerticalSpacing(0);
    layCompFlags->setHorizontalSpacing(8);
    fCBFlags[kCbShaded] = new QBitmaskCheckBox(hsGMaterial::kCompShaded,
            tr("Shaded"), grpCompFlags);
    fCBFlags[kCbEnvironMap] = new QBitmaskCheckBox(hsGMaterial::kCompEnvironMap,
            tr("EnvironMap"), grpCompFlags);
    fCBFlags[kCbProjectOnto] = new QBitmaskCheckBox(hsGMaterial::kCompProjectOnto,
            tr("Project Onto"), grpCompFlags);
    fCBFlags[kCbSoftShadow] = new QBitmaskCheckBox(hsGMaterial::kCompSoftShadow,
            tr("Soft Shadow"), grpCompFlags);
    fCBFlags[kCbSpecular] = new QBitmaskCheckBox(hsGMaterial::kCompSpecular,
            tr("Specular"), grpCompFlags);
    fCBFlags[kCbTwoSided] = new QBitmaskCheckBox(hsGMaterial::kCompTwoSided,
            tr("Two-Sided"), grpCompFlags);
    fCBFlags[kCbDrawAsSplats] = new QBitmaskCheckBox(hsGMaterial::kCompDrawAsSplats,
            tr("Draw as Splats"), grpCompFlags);
    fCBFlags[kCbAdjusted] = new QBitmaskCheckBox(hsGMaterial::kCompAdjusted,
            tr("Adjusted"), grpCompFlags);
    fCBFlags[kCbNoSoftShadow] = new QBitmaskCheckBox(hsGMaterial::kCompNoSoftShadow,
            tr("No Soft Shadow"), grpCompFlags);
    fCBFlags[kCbDynamic] = new QBitmaskCheckBox(hsGMaterial::kCompDynamic,
            tr("Dynamic"), grpCompFlags);
    fCBFlags[kCbDecal] = new QBitmaskCheckBox(hsGMaterial::kCompDecal,
            tr("Decal"), grpCompFlags);
    fCBFlags[kCbIsEmissive] = new QBitmaskCheckBox(hsGMaterial::kCompIsEmissive,
            tr("Is Emissive"), grpCompFlags);
    fCBFlags[kCbIsLightMapped] = new QBitmaskCheckBox(hsGMaterial::kCompIsLightMapped,
            tr("Is Light Mapped"), grpCompFlags);
    fCBFlags[kCbNeedsBlendChannel] = new QBitmaskCheckBox(hsGMaterial::kCompNeedsBlendChannel,
            tr("Needs Blend Channel"), grpCompFlags);
    layCompFlags->addWidget(fCBFlags[kCbShaded], 0, 0);
    layCompFlags->addWidget(fCBFlags[kCbTwoSided], 0, 1);
    layCompFlags->addWidget(fCBFlags[kCbDrawAsSplats], 0, 2);
    layCompFlags->addWidget(fCBFlags[kCbEnvironMap], 1, 0);
    layCompFlags->addWidget(fCBFlags[kCbAdjusted], 1, 1);
    layCompFlags->addWidget(fCBFlags[kCbNoSoftShadow], 1, 2);
    layCompFlags->addWidget(fCBFlags[kCbProjectOnto], 2, 0);
    layCompFlags->addWidget(fCBFlags[kCbDynamic], 2, 1);
    layCompFlags->addWidget(fCBFlags[kCbIsLightMapped], 2, 2);
    layCompFlags->addWidget(fCBFlags[kCbSoftShadow], 3, 0);
    layCompFlags->addWidget(fCBFlags[kCbDecal], 3, 1);
    layCompFlags->addWidget(fCBFlags[kCbNeedsBlendChannel], 3, 2);
    layCompFlags->addWidget(fCBFlags[kCbSpecular], 4, 0);
    layCompFlags->addWidget(fCBFlags[kCbIsEmissive], 4, 1);

    for (auto cb : fCBFlags) {
        cb->setFrom(mat->getCompFlags());
        connect(cb, &QBitmaskCheckBox::setBits, this, [this](unsigned int mask) {
            hsGMaterial* mat = hsGMaterial::Convert(fCreatable);
            mat->setCompFlags(mat->getCompFlags() | mask);
        });
        connect(cb, &QBitmaskCheckBox::unsetBits, this, [this](unsigned int mask) {
            hsGMaterial* mat = hsGMaterial::Convert(fCreatable);
            mat->setCompFlags(mat->getCompFlags() & ~mask);
        });
    }

    QTabWidget* objTab = new QTabWidget(this);
    fLayers = new QKeyList(mat->getKey(), objTab);
    fPiggyBacks = new QKeyList(mat->getKey(), objTab);
    for (size_t i=0; i<mat->getLayers().size(); i++)
        fLayers->addKey(mat->getLayers()[i]);
    for (size_t i=0; i<mat->getPiggyBacks().size(); i++)
        fPiggyBacks->addKey(mat->getPiggyBacks()[i]);
    objTab->addTab(fLayers, tr("Layers"));
    objTab->addTab(fPiggyBacks, tr("Piggy Backs"));

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(fSynchObjLink, 0, 0);
    layout->addWidget(grpCompFlags, 1, 0);
    layout->addWidget(objTab, 2, 0);
    fLayers->adjustColumns();
    fPiggyBacks->adjustColumns();
}

void QMaterial::saveDamage()
{
    hsGMaterial* mat = hsGMaterial::Convert(fCreatable);

    mat->clearLayers();
    QList<plKey> layers = fLayers->keys();
    while (!layers.isEmpty())
        mat->addLayer(layers.takeFirst());

    mat->clearPiggyBacks();
    QList<plKey> piggyBacks = fPiggyBacks->keys();
    while (!piggyBacks.isEmpty())
        mat->addPiggyBack(piggyBacks.takeFirst());
}
