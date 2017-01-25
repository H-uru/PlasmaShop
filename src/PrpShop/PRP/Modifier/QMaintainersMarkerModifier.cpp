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

#include "QMaintainersMarkerModifier.h"

#include <QGridLayout>
#include <PRP/Modifier/plMaintainersMarkerModifier.h>

QMaintainersMarkerModifier::QMaintainersMarkerModifier(plCreatable* pCre, QWidget* parent)
              : QCreatable(pCre, kMaintainersMarkerModifier, parent)
{
    plMaintainersMarkerModifier* obj = plMaintainersMarkerModifier::Convert(fCreatable);

    fSynchObjLink = new QCreatableLink(this, false);
    fSynchObjLink->setCreatable(obj, tr("Synch Flags"));
    fSynchObjLink->setForceType(kSynchedObject);

    fMarkerCalibration = new QComboBox();
    fMarkerCalibration->addItem("Broken", plMaintainersMarkerModifier::kBroken);
    fMarkerCalibration->addItem("Repaired", plMaintainersMarkerModifier::kRepaired);
    fMarkerCalibration->addItem("Calibrated", plMaintainersMarkerModifier::kCalibrated);
    int calibrationIdx = fMarkerCalibration->findData(obj->getCalibration());
    if (calibrationIdx < 0) {
        fMarkerCalibration->addItem(QString("Unknown Value: %1").arg(obj->getCalibration()),
                                    obj->getCalibration());
        fMarkerCalibration->setCurrentIndex(fMarkerCalibration->count() - 1);
    }
    else {
        fMarkerCalibration->setCurrentIndex(calibrationIdx);
    }

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(fSynchObjLink, 0, 0);
    layout->addWidget(new QLabel(tr("Calibration State:"), this), 2, 0);
    layout->addWidget(fMarkerCalibration, 2, 1);
}

void QMaintainersMarkerModifier::saveDamage()
{
    plMaintainersMarkerModifier* obj = plMaintainersMarkerModifier::Convert(fCreatable);

    obj->setCalibration(fMarkerCalibration->itemData(fMarkerCalibration->currentIndex()).toUInt());
}
