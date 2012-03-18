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
    fSynchObjLink->setText(tr("Synch Flags"));
    fSynchObjLink->setCreatable(mat);
    fSynchObjLink->setForceType(kSynchedObject);

    QGroupBox* grpCompFlags = new QGroupBox(tr("Composition Flags"), this);
    QGridLayout* layCompFlags = new QGridLayout(grpCompFlags);
    layCompFlags->setVerticalSpacing(0);
    layCompFlags->setHorizontalSpacing(8);
    fCBFlags[kCbShaded] = new QCheckBox(tr("Shaded"), grpCompFlags);
    fCBFlags[kCbEnvironMap] = new QCheckBox(tr("EnvironMap"), grpCompFlags);
    fCBFlags[kCbProjectOnto] = new QCheckBox(tr("Project Onto"), grpCompFlags);
    fCBFlags[kCbSoftShadow] = new QCheckBox(tr("Soft Shadow"), grpCompFlags);
    fCBFlags[kCbSpecular] = new QCheckBox(tr("Specular"), grpCompFlags);
    fCBFlags[kCbTwoSided] = new QCheckBox(tr("Two-Sided"), grpCompFlags);
    fCBFlags[kCbDrawAsSplats] = new QCheckBox(tr("Draw as Splats"), grpCompFlags);
    fCBFlags[kCbAdjusted] = new QCheckBox(tr("Adjusted"), grpCompFlags);
    fCBFlags[kCbNoSoftShadow] = new QCheckBox(tr("No Soft Shadow"), grpCompFlags);
    fCBFlags[kCbDynamic] = new QCheckBox(tr("Dynamic"), grpCompFlags);
    fCBFlags[kCbDecal] = new QCheckBox(tr("Decal"), grpCompFlags);
    fCBFlags[kCbIsEmissive] = new QCheckBox(tr("Is Emissive"), grpCompFlags);
    fCBFlags[kCbIsLightMapped] = new QCheckBox(tr("Is Light Mapped"), grpCompFlags);
    fCBFlags[kCbNeedsBlendChannel] = new QCheckBox(tr("Needs Blend Channel"), grpCompFlags);
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
    fCBFlags[kCbShaded]->setChecked(mat->getCompFlags() & hsGMaterial::kCompShaded);
    fCBFlags[kCbEnvironMap]->setChecked(mat->getCompFlags() & hsGMaterial::kCompEnvironMap);
    fCBFlags[kCbProjectOnto]->setChecked(mat->getCompFlags() & hsGMaterial::kCompProjectOnto);
    fCBFlags[kCbSoftShadow]->setChecked(mat->getCompFlags() & hsGMaterial::kCompSoftShadow);
    fCBFlags[kCbSpecular]->setChecked(mat->getCompFlags() & hsGMaterial::kCompSpecular);
    fCBFlags[kCbTwoSided]->setChecked(mat->getCompFlags() & hsGMaterial::kCompTwoSided);
    fCBFlags[kCbDrawAsSplats]->setChecked(mat->getCompFlags() & hsGMaterial::kCompDrawAsSplats);
    fCBFlags[kCbAdjusted]->setChecked(mat->getCompFlags() & hsGMaterial::kCompAdjusted);
    fCBFlags[kCbNoSoftShadow]->setChecked(mat->getCompFlags() & hsGMaterial::kCompNoSoftShadow);
    fCBFlags[kCbDynamic]->setChecked(mat->getCompFlags() & hsGMaterial::kCompDynamic);
    fCBFlags[kCbDecal]->setChecked(mat->getCompFlags() & hsGMaterial::kCompDecal);
    fCBFlags[kCbIsEmissive]->setChecked(mat->getCompFlags() & hsGMaterial::kCompIsEmissive);
    fCBFlags[kCbIsLightMapped]->setChecked(mat->getCompFlags() & hsGMaterial::kCompIsLightMapped);
    fCBFlags[kCbNeedsBlendChannel]->setChecked(mat->getCompFlags() & hsGMaterial::kCompNeedsBlendChannel);

    QTabWidget* objTab = new QTabWidget(this);
    fLayers = new QKeyList(mat->getKey(), objTab);
    fPiggyBacks = new QKeyList(mat->getKey(), objTab);
    for (size_t i=0; i<mat->getLayers().getSize(); i++)
        fLayers->addKey(mat->getLayers()[i]);
    for (size_t i=0; i<mat->getPiggyBacks().getSize(); i++)
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

    mat->setCompFlags((fCBFlags[kCbShaded]->isChecked() ? hsGMaterial::kCompShaded : 0)
                    | (fCBFlags[kCbEnvironMap]->isChecked() ? hsGMaterial::kCompEnvironMap : 0)
                    | (fCBFlags[kCbProjectOnto]->isChecked() ? hsGMaterial::kCompProjectOnto : 0)
                    | (fCBFlags[kCbSoftShadow]->isChecked() ? hsGMaterial::kCompSoftShadow : 0)
                    | (fCBFlags[kCbSpecular]->isChecked() ? hsGMaterial::kCompSpecular : 0)
                    | (fCBFlags[kCbTwoSided]->isChecked() ? hsGMaterial::kCompTwoSided : 0)
                    | (fCBFlags[kCbDrawAsSplats]->isChecked() ? hsGMaterial::kCompDrawAsSplats : 0)
                    | (fCBFlags[kCbAdjusted]->isChecked() ? hsGMaterial::kCompAdjusted : 0)
                    | (fCBFlags[kCbNoSoftShadow]->isChecked() ? hsGMaterial::kCompNoSoftShadow : 0)
                    | (fCBFlags[kCbDynamic]->isChecked() ? hsGMaterial::kCompDynamic : 0)
                    | (fCBFlags[kCbDecal]->isChecked() ? hsGMaterial::kCompDecal : 0)
                    | (fCBFlags[kCbIsEmissive]->isChecked() ? hsGMaterial::kCompIsEmissive : 0)
                    | (fCBFlags[kCbIsLightMapped]->isChecked() ? hsGMaterial::kCompIsLightMapped : 0)
                    | (fCBFlags[kCbNeedsBlendChannel]->isChecked() ? hsGMaterial::kCompNeedsBlendChannel : 0));

    mat->clearLayers();
    QList<plKey> layers = fLayers->keys();
    while (!layers.isEmpty())
        mat->addLayer(layers.takeFirst());

    mat->clearPiggyBacks();
    QList<plKey> piggyBacks = fPiggyBacks->keys();
    while (!piggyBacks.isEmpty())
        mat->addPiggyBack(piggyBacks.takeFirst());
}
