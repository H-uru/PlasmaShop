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

#include "QIcicle.h"

#include <QGroupBox>
#include <QGridLayout>

QIcicle::QIcicle(plCreatable* pCre, size_t idx, QWidget* parent)
    : QCreatable(pCre, kSpan_Type|idx, parent)
{
    plDrawableSpans* dspan = plDrawableSpans::Convert(fCreatable);
    fSpan = dspan->getIcicle(idx);

    setWindowTitle(pqGetFriendlyClassName(dspan->ClassIndex()) +
                   ": " + st2qstr(dspan->getKey()->getName()));

    QIcon ico = pqGetTypeIcon(dspan->ClassIndex());
    if (!ico.isNull())
        setWindowIcon(ico);

    QGroupBox* grpFlags = new QGroupBox(tr("Flags"), this);
    fFlags[kPropNoDraw] = new QCheckBox(tr("No Draw"), grpFlags);
    fFlags[kPropNoShadowCast] = new QCheckBox(tr("No Shadow Cast"), grpFlags);
    fFlags[kPropFacesSortable] = new QCheckBox(tr("Faces Sortable"), grpFlags);
    fFlags[kPropVolatile] = new QCheckBox(tr("Volatile"), grpFlags);
    fFlags[kWaterHeight] = new QCheckBox(tr("Water Height"), grpFlags);
    fFlags[kPropRunTimeLight] = new QCheckBox(tr("Run-Time Light"), grpFlags);
    fFlags[kPropReverseSort] = new QCheckBox(tr("Reverse Sort"), grpFlags);
    fFlags[kPropHasPermaLights] = new QCheckBox(tr("PermaLights"), grpFlags);
    fFlags[kPropHasPermaProjs] = new QCheckBox(tr("PermaProjs"), grpFlags);
    fFlags[kPropMatHasSpecular] = new QCheckBox(tr("Has Specular"), grpFlags);
    fFlags[kPropProjAsVtx] = new QCheckBox(tr("ProjAsVtx"), grpFlags);
    fFlags[kPropSkipProjection] = new QCheckBox(tr("Skip Projection"), grpFlags);
    fFlags[kPropNoShadow] = new QCheckBox(tr("No Shadow"), grpFlags);
    fFlags[kPropForceShadow] = new QCheckBox(tr("Force Shadow"), grpFlags);
    fFlags[kPropDisableNormal] = new QCheckBox(tr("Disable Normal"), grpFlags);
    fFlags[kPropCharacter] = new QCheckBox(tr("Character"), grpFlags);
    fFlags[kPartialSort] = new QCheckBox(tr("Partial Sort"), grpFlags);
    fFlags[kVisLOS] = new QCheckBox(tr("Vis LOS"), grpFlags);

    fLightingType = new QComboBox(grpFlags);
    fLightingType->addItems(QStringList() << "Material Lighting" << "Vertex Preshaded Lighting" << "Vertex Non-Preshaded Lighting" << "Projection" << "Shadow Erase" << "Shadow");
    fLightingType->setCurrentIndex(0);

    int lighting = (fSpan->getProps() & 0x3E00) >> 8;
    for (int i = 0; i < 6; i++) {
        if (lighting & (1 << i)) {
            fLightingType->setCurrentIndex(i);
        }
    }

    fFlags[kPropNoDraw]->setChecked((fSpan->getProps() & plSpan::kPropNoDraw) != 0);
    fFlags[kPropNoShadowCast]->setChecked((fSpan->getProps() & plSpan::kPropNoShadowCast) != 0);
    fFlags[kPropFacesSortable]->setChecked((fSpan->getProps() & plSpan::kPropFacesSortable) != 0);
    fFlags[kPropVolatile]->setChecked((fSpan->getProps() & plSpan::kPropVolatile) != 0);
    fFlags[kWaterHeight]->setChecked((fSpan->getProps() & plSpan::kWaterHeight) != 0);
    fFlags[kPropRunTimeLight]->setChecked((fSpan->getProps() & plSpan::kPropRunTimeLight) != 0);
    fFlags[kPropReverseSort]->setChecked((fSpan->getProps() & plSpan::kPropReverseSort) != 0);
    fFlags[kPropHasPermaLights]->setChecked((fSpan->getProps() & plSpan::kPropHasPermaLights) != 0);
    fFlags[kPropHasPermaProjs]->setChecked((fSpan->getProps() & plSpan::kPropHasPermaProjs) != 0);
    fFlags[kPropMatHasSpecular]->setChecked((fSpan->getProps() & plSpan::kPropMatHasSpecular) != 0);
    fFlags[kPropProjAsVtx]->setChecked((fSpan->getProps() & plSpan::kPropProjAsVtx) != 0);
    fFlags[kPropSkipProjection]->setChecked((fSpan->getProps() & plSpan::kPropSkipProjection) != 0);
    fFlags[kPropNoShadow]->setChecked((fSpan->getProps() & plSpan::kPropNoShadow) != 0);
    fFlags[kPropForceShadow]->setChecked((fSpan->getProps() & plSpan::kPropForceShadow) != 0);
    fFlags[kPropDisableNormal]->setChecked((fSpan->getProps() & plSpan::kPropDisableNormal) != 0);
    fFlags[kPropCharacter]->setChecked((fSpan->getProps() & plSpan::kPropCharacter) != 0);
    fFlags[kPartialSort]->setChecked((fSpan->getProps() & plSpan::kPartialSort) != 0);
    fFlags[kVisLOS]->setChecked((fSpan->getProps() & plSpan::kVisLOS) != 0);

    QGridLayout* layFlags = new QGridLayout(grpFlags);
    layFlags->setVerticalSpacing(0);
    layFlags->setHorizontalSpacing(8);
    layFlags->addWidget(fFlags[kPropNoDraw], 0, 0);
    layFlags->addWidget(fFlags[kPropNoShadowCast], 0, 1);
    layFlags->addWidget(fFlags[kPropFacesSortable], 0, 2);
    layFlags->addWidget(fFlags[kPropVolatile], 0, 3);
    layFlags->addWidget(fFlags[kWaterHeight], 1, 0);
    layFlags->addWidget(fFlags[kPropRunTimeLight], 1, 1);
    layFlags->addWidget(fFlags[kPropReverseSort], 1, 2);
    layFlags->addWidget(fFlags[kPropHasPermaLights], 1, 3);
    layFlags->addWidget(fFlags[kPropHasPermaProjs], 2, 0);
    layFlags->addWidget(fFlags[kPropMatHasSpecular], 2, 1);
    layFlags->addWidget(fFlags[kPropProjAsVtx], 2, 2);
    layFlags->addWidget(fFlags[kPropSkipProjection], 2, 3);
    layFlags->addWidget(fFlags[kPropNoShadow], 3, 0);
    layFlags->addWidget(fFlags[kPropForceShadow], 3, 1);
    layFlags->addWidget(fFlags[kPropDisableNormal], 3, 2);
    layFlags->addWidget(fFlags[kPropCharacter], 3, 3);
    layFlags->addWidget(fLightingType, 4, 0, 1, 2);
    layFlags->addWidget(fFlags[kPartialSort], 4, 2);
    layFlags->addWidget(fFlags[kVisLOS], 4, 3);

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(grpFlags, 0, 0, 1, 2);
}
