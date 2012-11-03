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

#ifndef _QPLASMADEVMODEDAT_H
#define _QPLASMADEVMODEDAT_H

#include "QPlasmaDocument.h"
#include <Stream/hsStream.h>
#include <QLabel>
#include <QLineEdit>

class QPlasmaDevModeDat : public QPlasmaDocument {
    Q_OBJECT

public:
    QPlasmaDevModeDat(QWidget* parent);

    virtual bool loadFile(QString filename);

private:
    enum {
        //DeviceRecord
        kDevMRecordVersion, kDevMFlags, kDevMDeviceType,
        kDevMDriverDesc, kDevMDriverName, kDevMDriverVersion,
        kDevMDeviceDesc, kDevMCaps, kDevMLayersAtOnce,
        kDevMMemoryBytes, kDevMNumModes, kDevMZBiasRating,
        kDevMLODBiasRating, kDevMFogExpApproxStart,
        kDevMFogExp2ApproxStart, kDevMFogEndBias,
        kNumDevRecordEntries
    };
    enum {
        //DeviceMode
        kDevMWidth, kDevMHeight, kDevMDepth,
        kDevMCanRenderToCubics, kNumDevModeEntries
    };
    QLineEdit* fRecordEntries[kNumDevRecordEntries];
    QLineEdit* fModeEntries[kNumDevModeEntries];
    QLineEdit* fTexQualityEntry;

    bool loadDeviceModeData(hsStream* S);
};

#endif
