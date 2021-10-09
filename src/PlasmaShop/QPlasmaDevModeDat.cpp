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
#include <QGridLayout>
#include <QGroupBox>
#include <QScrollArea>
#include "QPlasma.h"

/* QPlasmaDevModeDat */
QPlasmaDevModeDat::QPlasmaDevModeDat(QWidget* parent)
    : QPlasmaDocument(kDocDevMode, parent)
{
    //DeviceRecord Group
    QGroupBox* grpRecord = new QGroupBox(tr("Device Record"), this);
    QGridLayout* layRecord = new QGridLayout(grpRecord);
    for (size_t i = 0; i < kNumDevRecordLabels; i++) {
        fRecordLabels[i] = new QLabel(grpRecord);
        fRecordLabels[i]->setTextInteractionFlags(Qt::TextSelectableByMouse);
        layRecord->addWidget(fRecordLabels[i], i, 1, 1, 2);
    }
    fRecordSliderAASetting = new QSlider(Qt::Horizontal, grpRecord);
    fRecordSliderAASetting->setRange(0, 3);
    fRecordSliderAASetting->setPageStep(1);
    fRecordSliderMaxAniso = new QSlider(Qt::Horizontal, grpRecord);
    fRecordSliderMaxAniso->setRange(0, 16);
    fRecordSliderMaxAniso->setPageStep(2);
    QLabel* AASettingLabel = new QLabel("0", grpRecord);
    QLabel* MaxAnisoLabel = new QLabel("0", grpRecord);
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
    layRecord->addWidget(new QLabel(tr("AASetting:"), grpRecord), kNumDevRecordLabels, 0);
    layRecord->addWidget(new QLabel(tr("MaxAnisotropicSamples:"), grpRecord), kNumDevRecordLabels + 1, 0);
    layRecord->addWidget(fRecordSliderAASetting, kNumDevRecordLabels, 1);
    layRecord->addWidget(fRecordSliderMaxAniso, kNumDevRecordLabels + 1, 1);
    layRecord->addWidget(AASettingLabel, kNumDevRecordLabels, 2);
    layRecord->addWidget(MaxAnisoLabel, kNumDevRecordLabels + 1, 2);
    connect(fRecordSliderAASetting, &QSlider::valueChanged,
            AASettingLabel, QOverload<int>::of(&QLabel::setNum));
    connect(fRecordSliderMaxAniso, &QSlider::valueChanged,
            MaxAnisoLabel, QOverload<int>::of(&QLabel::setNum));

    //Capabilities SubGroup
    QGroupBox* grpCaps = new QGroupBox(tr("Capabilities"), grpRecord);
    QGridLayout* layCaps = new QGridLayout(grpCaps);
    for (size_t i = 0; i < kNumCaps; i++) {
        fCapsCheckBoxes[i] = new QCheckBox(getCapName(i), grpCaps);
        fCapsCheckBoxes[i]->setEnabled(false);
        //Add 6 Boxes per row, i.e. Row is i DIV 6 and Column i MOD 6
        layCaps->addWidget(fCapsCheckBoxes[i], (int) i / 6, i % 6);
    }
    layRecord->addWidget(grpCaps, kNumDevRecordLabels + 2, 0, 1, 3);

    //DeviceMode Group
    QGroupBox* grpMode = new QGroupBox(tr("Device Mode"), this);
    QGridLayout* layMode = new QGridLayout(grpMode);
    fModeLabelFlags = new QLabel(grpMode);
    fModeLineEditWidth = new QLineEdit(grpMode);
    fModeLineEditHeight = new QLineEdit(grpMode);
    fModeComboBoxDepth = new QComboBox(grpMode);
    fModeComboBoxDepth->addItem("Low (16 bit)");
    fModeComboBoxDepth->addItem("High (32 bit)");
    fModeCheckBoxWindowed = new QCheckBox(grpMode);
    fModeLabelCanRenderToCubics = new QLabel(grpMode);
    layMode->addWidget(new QLabel(tr("Flags:"), grpMode), 0, 0);
    layMode->addWidget(new QLabel(tr("Width:"), grpMode), 1, 0);
    layMode->addWidget(new QLabel(tr("Height:"), grpMode), 2, 0);
    layMode->addWidget(new QLabel(tr("Depth:"), grpMode), 3, 0);
    layMode->addWidget(new QLabel(tr("Windowed:"), grpMode), 4, 0);
    layMode->addWidget(new QLabel(tr("Can Render To Cubics:"), grpMode), 5, 0);
    layMode->addWidget(fModeLabelFlags, 0, 1);
    layMode->addWidget(fModeLineEditWidth, 1, 1);
    layMode->addWidget(fModeLineEditHeight, 2, 1);
    layMode->addWidget(fModeComboBoxDepth, 3, 1);
    layMode->addWidget(fModeCheckBoxWindowed, 4, 1);
    layMode->addWidget(fModeLabelCanRenderToCubics, 5, 1);
    connect(fModeCheckBoxWindowed, &QCheckBox::toggled, fModeComboBoxDepth, &QWidget::setDisabled);

    //TextureQuality Group
    QGroupBox* grpTexQuality = new QGroupBox(tr("Texture Quality"), this);
    QGridLayout* layTexQuality = new QGridLayout(grpTexQuality);
    fSliderTextureQuality = new QSlider(Qt::Horizontal, grpTexQuality);
    fSliderTextureQuality->setRange(0, 100);
    QLabel* textureQualityValueLabel = new QLabel(grpTexQuality);
    layTexQuality->addWidget(new QLabel(tr("Quality:"), grpTexQuality), 0, 0);
    layTexQuality->addWidget(fSliderTextureQuality, 0, 1);
    layTexQuality->addWidget(textureQualityValueLabel, 0, 2);
    connect(fSliderTextureQuality, &QSlider::valueChanged, textureQualityValueLabel,
            QOverload<int>::of(&QLabel::setNum));

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(4, 4, 4, 4);
    layout->addWidget(grpRecord, 0, 0);
    layout->addWidget(grpMode, 1, 0);
    layout->addWidget(grpTexQuality, 2, 0);

    //Make everything scrollable
    QWidget* everything = new QWidget(this);
    everything->setLayout(layout);
    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidget(everything);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setWidgetResizable(true);
    QGridLayout* scrollLayout = new QGridLayout(this);
    scrollLayout->setContentsMargins(4, 4, 4, 4);
    scrollLayout->addWidget(scrollArea);
    setLayout(scrollLayout);
}

bool QPlasmaDevModeDat::loadFile(const QString& filename)
{
    hsFileStream S;
    if (!S.open(filename.toUtf8().data(), fmRead))
        return false;
    if (!loadDeviceModeData(&S))
        return false;
    return QPlasmaDocument::loadFile(filename);
}

bool QPlasmaDevModeDat::saveTo(const QString& filename)
{
    hsFileStream S;
    S.open(filename.toUtf8().data(), fmCreate);
    if (!saveDeviceModeData(&S))
        return false;
    return QPlasmaDocument::saveTo(filename);
}

bool QPlasmaDevModeDat::loadDeviceModeData(hsStream* S)
{
    if (S != NULL) {
        try {
            //Read Stream
            fModeRecord.read(S);
            hsG3DDeviceRecord record = fModeRecord.getRecord();
            hsG3DDeviceMode mode = fModeRecord.getMode();

            //Fill Record Group
            fRecordLabels[kDevMRecordVersion]->setText(QString::number(record.getVersion()));
            fRecordLabels[kDevMFlags]->setText(getFlagName(record.getFlags()));
            fRecordLabels[kDevMDeviceType]->setText(getDeviceTypeName(record.getDeviceType()));
            fRecordLabels[kDevMDriverDesc]->setText(st2qstr(record.getDriverDesc()));
            fRecordLabels[kDevMDriverName]->setText(st2qstr(record.getDriverName()));
            fRecordLabels[kDevMDriverVersion]->setText(st2qstr(record.getDriverVersion()));
            fRecordLabels[kDevMDeviceDesc]->setText(st2qstr(record.getDeviceDesc()));
            fRecordLabels[kDevMLayersAtOnce]->setText(QString::number(record.getLayersAtOnce()));
            QString recordMemBytes = QString::number(record.getMemoryBytes());
            QString recordMemBytesMB = QString::number(record.getMemoryBytes() / 1024 / 1024);
            fRecordLabels[kDevMMemoryBytes]->setText(recordMemBytes + " (" + recordMemBytesMB + " MB)");
            fRecordLabels[kDevMZBiasRating]->setText(QString::number(record.getZBiasRating()));
            fRecordLabels[kDevMLODBiasRating]->setText(QString::number(record.getLODBiasRating()));
            fRecordLabels[kDevMFogExpApproxStart]->setText(QString::number(record.getFogExpApproxStart()));
            fRecordLabels[kDevMFogExp2ApproxStart]->setText(QString::number(record.getFogExp2ApproxStart()));
            fRecordLabels[kDevMFogEndBias]->setText(QString::number(record.getFogEndBias()));
            fRecordSliderAASetting->setValue(record.getAASetting());
            fRecordSliderMaxAniso->setValue(record.getMaxAnisotropicSamples());

            hsBitVector caps = record.getCaps();
            for (size_t i = 0; i < kNumCaps; i++)
                fCapsCheckBoxes[i]->setChecked(caps.get(i));

            //Fill Mode Group
            fModeLabelFlags->setText(getFlagName(mode.getFlags()));
            fModeLineEditWidth->setText(QString::number(mode.getWidth()));
            fModeLineEditHeight->setText(QString::number(mode.getHeight()));
            fModeComboBoxDepth->setCurrentIndex(mode.getDepth() == 32 ? 1 : 0);
            if (mode.getDepth() == 0)
                fModeCheckBoxWindowed->setChecked(true);
            fModeLabelCanRenderToCubics->setText(mode.getCanRenderToCubics() ? tr("Yes") : tr("No"));

            //Get Texture Quality
            fSliderTextureQuality->setValue(fModeRecord.getTextureQuality());

            //When to mark the file as dirty?
            connect(fRecordSliderAASetting, &QSlider::valueChanged, this, &QPlasmaDocument::makeDirty);
            connect(fRecordSliderMaxAniso, &QSlider::valueChanged, this, &QPlasmaDocument::makeDirty);
            connect(fModeLineEditWidth, &QLineEdit::textEdited, this, &QPlasmaDocument::makeDirty);
            connect(fModeLineEditHeight, &QLineEdit::textEdited, this, &QPlasmaDocument::makeDirty);
            connect(fModeComboBoxDepth, QOverload<int>::of(&QComboBox::currentIndexChanged),
                    this, &QPlasmaDocument::makeDirty);
            connect(fModeCheckBoxWindowed, &QCheckBox::stateChanged, this, &QPlasmaDocument::makeDirty);
            connect(fSliderTextureQuality, &QSlider::valueChanged, this, &QPlasmaDocument::makeDirty);
        } catch (std::exception &e) {
            plDebug::Error("Error reading dev_mode.dat file {}: {}", fFilename.toUtf8().data(), e.what());
            return false;
        }
    }
    return true;
}

bool QPlasmaDevModeDat::saveDeviceModeData(hsStream* S)
{
    try {
        //Fetch possible changes
        fModeRecord.getRecord().setAASetting(fRecordSliderAASetting->value());
        fModeRecord.getRecord().setMaxAnisotropicSamples(fRecordSliderMaxAniso->value());
        fModeRecord.getMode().setWidth(fModeLineEditWidth->text().toUInt());
        fModeRecord.getMode().setHeight(fModeLineEditHeight->text().toUInt());
        if (fModeCheckBoxWindowed->isChecked())
            fModeRecord.getMode().setDepth(0);
        else
            fModeRecord.getMode().setDepth(fModeComboBoxDepth->currentIndex() == 1 ? 32 : 16);
        fModeRecord.setTextureQuality(fSliderTextureQuality->value());
        //Write to stream
        fModeRecord.write(S);
    } catch (std::exception &e) {
        plDebug::Error("Error writing dev_mode.dat file {}: {}", fFilename.toUtf8().data(), e.what());
        return false;
    }
    return true;
}

QString QPlasmaDevModeDat::getFlagName(uint32_t flag)
{
    if (flag == hsG3DDeviceRecord::kNone)
        return "None";
    QString flagName;
    if (flag & hsG3DDeviceRecord::kDiscarded)
        flagName += flagName.isEmpty() ? "Discarded" : " | Discarded";
    if (flag & hsG3DDeviceRecord::kInvalid)
        flagName += flagName.isEmpty() ? "Invalid" : " | Invalid";
    if (flagName.isEmpty())
        flagName = "UNKNOWN: 0x" + QString::number(flag, 16).toUpper();
    return flagName;
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
