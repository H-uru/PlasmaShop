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

#include "QPlasmaDevModeDat.h"
#include <Debug/plDebug.h>
#include <Util/hs3DDevice.h>
#include <QGridLayout>
#include <QGroupBox>
#include "../QPlasma.h"

/* QPlasmaDevModeDat */
QPlasmaDevModeDat::QPlasmaDevModeDat(QWidget* parent)
              : QPlasmaDocument(kDocDevMode, parent)
{
    //DeviceRecord Group
    QGroupBox* grpRecord = new QGroupBox(tr("Device Record (Read Only)"), this);
    QGridLayout* layRecord = new QGridLayout(grpRecord);
    for (size_t i = 0; i < kNumDevRecordLabels; i++) {
        fRecordLabels[i] = new QLabel(grpRecord);
        fRecordLabels[i]->setTextInteractionFlags(Qt::TextSelectableByMouse);
        layRecord->addWidget(fRecordLabels[i], i, 1);
    }
    layRecord->addWidget(new QLabel(tr("Record Version:"), grpRecord), kDevMRecordVersion, 0);
    layRecord->addWidget(new QLabel(tr("Flags:"), grpRecord), kDevMFlags, 0);
    layRecord->addWidget(new QLabel(tr("Device Type:"), grpRecord), kDevMDeviceType, 0);
    layRecord->addWidget(new QLabel(tr("Driver Description:"), grpRecord), kDevMDriverDesc, 0);
    layRecord->addWidget(new QLabel(tr("Driver Name:"), grpRecord), kDevMDriverName, 0);
    layRecord->addWidget(new QLabel(tr("Driver Version:"), grpRecord), kDevMDriverVersion, 0);
    layRecord->addWidget(new QLabel(tr("Device Description:"), grpRecord), kDevMDeviceDesc, 0);
    layRecord->addWidget(new QLabel(tr("Layers At Once:"), grpRecord), kDevMLayersAtOnce, 0);
    layRecord->addWidget(new QLabel(tr("Memory Bytes:"), grpRecord), kDevMMemoryBytes, 0);
    layRecord->addWidget(new QLabel(tr("ZBiasRating:"), grpRecord), kDevMZBiasRating, 0);
    layRecord->addWidget(new QLabel(tr("LODBiasRating:"), grpRecord), kDevMLODBiasRating, 0);
    layRecord->addWidget(new QLabel(tr("FogExpApproxStart:"), grpRecord), kDevMFogExpApproxStart, 0);
    layRecord->addWidget(new QLabel(tr("FogExp2ApproxStart:"), grpRecord), kDevMFogExp2ApproxStart, 0);
    layRecord->addWidget(new QLabel(tr("FogEndBias:"), grpRecord), kDevMFogEndBias, 0);

    //Capabilities SubGroup
    QGroupBox* grpCaps = new QGroupBox(tr("Capabilities"), grpRecord);
    QGridLayout* layCaps = new QGridLayout(grpCaps);
    for(size_t i = 0; i < kNumCaps; i++) {
        fCapsCheckBoxes[i] = new QCheckBox(getCapName(i), grpCaps);
        fCapsCheckBoxes[i]->setEnabled(false);
        //Add 6 Boxes per row, i.e. Row is i DIV 6 and Column i MOD 6
        layCaps->addWidget(fCapsCheckBoxes[i], (int) i / 6, i % 6);
    }
    layRecord->addWidget(grpCaps, kNumDevRecordLabels, 0, 1, 2);

    //DeviceMode Group
    QGroupBox* grpMode = new QGroupBox(tr("Device Mode"), this);
    QGridLayout* layMode = new QGridLayout(grpMode);
    for (size_t i = 0; i < kNumDevModeLineEdits; i++) {
        fModeLineEdits[i] = new QLineEdit(grpMode);
        layMode->addWidget(fModeLineEdits[i], i, 1);
    }
    fModeCheckBoxWindowed = new QCheckBox(grpMode);
    fModeLabelCanRenderToCubics = new QLabel(grpMode);
    layMode->addWidget(new QLabel(tr("Width:"), grpMode), kDevMWidth, 0);
    layMode->addWidget(new QLabel(tr("Height:"), grpMode), kDevMHeight, 0);
    layMode->addWidget(new QLabel(tr("Depth:"), grpMode), kDevMDepth, 0);
    layMode->addWidget(new QLabel(tr("Windowed:"), grpMode), kNumDevModeLineEdits, 0);
    layMode->addWidget(new QLabel(tr("Can Render To Cubics:"), grpMode), kNumDevModeLineEdits + 1, 0);
    layMode->addWidget(fModeCheckBoxWindowed, kNumDevModeLineEdits, 1);
    layMode->addWidget(fModeLabelCanRenderToCubics, kNumDevModeLineEdits + 1, 1);
    connect(fModeCheckBoxWindowed, SIGNAL(toggled(bool)), fModeLineEdits[kDevMDepth], SLOT(setDisabled(bool)));

    //TextureQuality Group
    QGroupBox* grpTexQuality = new QGroupBox(tr("Texture Quality"), this);
    QGridLayout* layTexQuality = new QGridLayout(grpTexQuality);
    fSliderTextureQuality = new QSlider(Qt::Horizontal, grpTexQuality);
    fSliderTextureQuality->setMinimum(0);
    fSliderTextureQuality->setMaximum(100);
    QLabel* textureQualityValueLabel = new QLabel(grpTexQuality);
    layTexQuality->addWidget(new QLabel(tr("Quality:"), grpTexQuality), 0, 0);
    layTexQuality->addWidget(fSliderTextureQuality, 0, 1);
    layTexQuality->addWidget(textureQualityValueLabel, 0, 2);
    connect(fSliderTextureQuality, SIGNAL(valueChanged(int)), textureQualityValueLabel,
            SLOT(setNum(int)));

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(grpRecord, 0, 0);
    layout->addWidget(grpMode, 1, 0);
    layout->addWidget(grpTexQuality, 2, 0);
    setLayout(layout);
}

bool QPlasmaDevModeDat::loadFile(QString filename)
{
    hsFileStream S(PlasmaVer::pvUnknown);
    if(!S.open(filename.toUtf8().data(), fmRead))
        return false;
    if (!loadDeviceModeData(&S))
        return false;
    return QPlasmaDocument::loadFile(filename);
}

bool QPlasmaDevModeDat::loadDeviceModeData(hsStream* S)
{
    if (S != NULL) {
        try {
            hsG3DDeviceRecord loadRec;
            hsG3DDeviceMode loadMode;
            loadRec.read(S);
            loadMode.read(S);

            fModeLineEdits[kDevMWidth]->setText(QString().setNum(loadMode.getWidth()));
            fModeLineEdits[kDevMHeight]->setText(QString().setNum(loadMode.getHeight()));
            fModeLineEdits[kDevMDepth]->setText(QString().setNum(loadMode.getDepth()));
            fModeLabelCanRenderToCubics->setText(loadMode.getCanRenderToCubics() ? tr("Yes") : tr("No"));

            fRecordLabels[kDevMRecordVersion]->setText(QString().setNum(loadRec.getVersion()));
            fRecordLabels[kDevMFlags]->setText(getFlagName(loadRec.getFlags()));
            fRecordLabels[kDevMDeviceType]->setText(getDeviceTypeName(loadRec.getDeviceType()));
            fRecordLabels[kDevMDriverDesc]->setText(loadRec.getDriverDesc().cstr());
            fRecordLabels[kDevMDriverName]->setText(loadRec.getDriverName().cstr());
            fRecordLabels[kDevMDriverVersion]->setText(loadRec.getDriverVersion().cstr());
            fRecordLabels[kDevMDeviceDesc]->setText(loadRec.getDeviceDesc().cstr());
            fRecordLabels[kDevMLayersAtOnce]->setText(QString().setNum(loadRec.getLayersAtOnce()));
            QString recordMemBytes = QString().setNum(loadRec.getMemoryBytes());
            QString recordMemBytesMB = QString().setNum(loadRec.getMemoryBytes() / 1024 / 1024);
            fRecordLabels[kDevMMemoryBytes]->setText(recordMemBytes + " (" + recordMemBytesMB + " MB)");
            fRecordLabels[kDevMZBiasRating]->setText(QString().setNum(loadRec.getZBiasRating()));
            fRecordLabels[kDevMLODBiasRating]->setText(QString().setNum(loadRec.getLODBIasRating()));
            fRecordLabels[kDevMFogExpApproxStart]->setText(QString().setNum(loadRec.getFogExpApproxStart()));
            fRecordLabels[kDevMFogExp2ApproxStart]->setText(QString().setNum(loadRec.getFogExp2ApproxStart()));
            fRecordLabels[kDevMFogEndBias]->setText(QString().setNum(loadRec.getFogEndBias()));

            hsBitVector caps = loadRec.getCaps();
            for(size_t i = 0; i < kNumCaps; i++)
                fCapsCheckBoxes[i]->setChecked(caps.get(i));

            uint16_t textureQuality = S->readShort();
            fSliderTextureQuality->setValue(textureQuality);
        } catch (std::exception &e) {
            plDebug::Error("Error reading dev_mode.dat file %s: %s", fFilename.toUtf8().data(), e.what());
            return false;
        }
    }
    return true;
}

QString QPlasmaDevModeDat::getFlagName(uint32_t flag)
{
    switch(flag) {
    case hsG3DDeviceRecord::kNone:
        return "None";
    case hsG3DDeviceRecord::kDiscarded:
        return "Discarded";
    case hsG3DDeviceRecord::kInvalid:
        return "Invalid";
    default:
        return "UNKNOWN";
    }
}

QString QPlasmaDevModeDat::getDeviceTypeName(uint32_t type)
{
    switch(type) {
    case hsG3DDeviceRecord::kGlide:
        return "Glide";
    case hsG3DDeviceRecord::kDirect3D:
        return "Direct3D";
    case hsG3DDeviceRecord::kOpenGL:
        return "OpenGL";
    case hsG3DDeviceRecord::kDirect3D_TnL:
        return "Direct3D_TnL";
    default:
        return "UNKNOWN";
    }
}

QString QPlasmaDevModeDat::getCapName(size_t cap)
{
    switch(cap) {
        case kCapsNone: return "None";
        case kCapsNoWindow: return "NoWindow";
        case kCapsMipmap: return "Mipmap";
        case kCapsPerspective: return "Perspective";
        case kCapsHardware: return "Hardware";
        case kCapsWBuffer: return "WBuffer";
        case kCapsCompressTextures: return "CompressTextures";
        case kCapsHWTransform: return "HWTransform";
        case kCapsDither: return "Dither";
        case kCapsFogLinear: return "FogLinear";
        case kCapsFogExp: return "FogExp";
        case kCapsFogExp2: return "FogExp2";
        case kCapsFogRange: return "FogRange";
        case kCapsLODWatch: return "LODWatch";
        case kCapsUNUSED: return "UNUSED";
        case kCapsDoesSmallTextures: return "DoesSmallTextures";
        case kCapsPixelFog: return "PixelFog";
        case kCapsBadYonStuff: return "BadYonStuff";
        case kCapsNoKindaSmallTexs: return "NoKindaSmallTexs";
        case kCapsCubicTextures: return "CubicTextures";
        case kCapsCubicMipmap: return "CubicMap";
        case kCapsZBias: return "ZBias";
        case kCapsPixelShader: return "PixelShader";
        case kCapsNoAA: return "NoAA";
        case kCapsDoubleFlush: return "DoubleFlush";
        case kCapsSingleFlush: return "SingleFlush";
        case kCapsCantShadow: return "CantShadow";
        case kCapsMaxUVWSrc2: return "CapsMaxUVWSrc2";
        case kCapsCantProj: return "CantProj";
        case kCapsLimitedProj: return "LimitedProj";
        case kCapsShareDepth: return "ShareDepth";
        case kCapsBadManaged: return "BadManaged";
        case kCapsNoAniso: return "NoAniso";
        default: return "UNKNOWN";
    };
}
