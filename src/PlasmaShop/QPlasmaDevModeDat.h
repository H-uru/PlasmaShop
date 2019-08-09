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
#include <Util/hs3DDevice.h>
#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QSlider>

class QPlasmaDevModeDat : public QPlasmaDocument
{
    Q_OBJECT

public:
    QPlasmaDevModeDat(QWidget* parent);

    bool loadFile(QString filename) override;
    bool saveTo(QString filename) override;

private:
    enum //DeviceRecord Labels
    {
        kDevMRecordVersion, kDevMFlags, kDevMDeviceType,
        kDevMDriverDesc, kDevMDriverName, kDevMDriverVersion,
        kDevMDeviceDesc, kDevMLayersAtOnce, kDevMMemoryBytes,
        kDevMZBiasRating, kDevMLODBiasRating, kDevMFogExpApproxStart,
        kDevMFogExp2ApproxStart, kDevMFogEndBias,
        kNumDevRecordLabels
    };
    enum //Capabilities, Name Strings in getCapName()
    {
        kCapsNone, kCapsNoWindow, kCapsMipmap,
        kCapsPerspective, kCapsHardware, kCapsWBuffer,
        kCapsCompressTextures, kCapsHWTransform, kCapsDither,
        kCapsFogLinear, kCapsFogExp, kCapsFogExp2, kCapsFogRange,
        kCapsLODWatch, kCapsUNUSED, kCapsDoesSmallTextures,
        kCapsPixelFog, kCapsBadYonStuff, kCapsNoKindaSmallTexs,
        kCapsCubicTextures, kCapsCubicMipmap, kCapsZBias,
        kCapsPixelShader, kCapsNoAA, kCapsDoubleFlush,
        kCapsSingleFlush, kCapsCantShadow, kCapsMaxUVWSrc2,
        kCapsCantProj, kCapsLimitedProj, kCapsShareDepth,
        kCapsBadManaged, kCapsNoAniso, kNumCaps
    };
    QLabel* fRecordLabels[kNumDevRecordLabels];
    QSlider* fRecordSliderAASetting, *fRecordSliderMaxAniso;
    QCheckBox* fCapsCheckBoxes[kNumCaps];
    QLabel* fModeLabelFlags;
    QLineEdit* fModeLineEditWidth, *fModeLineEditHeight;
    QComboBox* fModeComboBoxDepth;
    QCheckBox* fModeCheckBoxWindowed;
    QLabel* fModeLabelCanRenderToCubics;
    QSlider* fSliderTextureQuality;
    hsG3DDeviceModeRecord fModeRecord;

    bool loadDeviceModeData(hsStream* S);
    bool saveDeviceModeData(hsStream* S);
    QString getFlagName(uint32_t flag);
    QString getDeviceTypeName(uint32_t type);
    QString getCapName(size_t cap);
};

#endif
