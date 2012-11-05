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
    layRecord->setVerticalSpacing(4);
    layRecord->setHorizontalSpacing(8);
    for (size_t i = 0; i < kNumDevRecordLineEdits; i++) {
        fRecordLineEdits[i] = new QLineEdit(grpRecord);
        fRecordLineEdits[i]->setReadOnly(true);
    }
    layRecord->addWidget(new QLabel(tr("Record Version:"), grpRecord), 0, 0);
    layRecord->addWidget(new QLabel(tr("Flags:"), grpRecord), 1, 0);
    layRecord->addWidget(new QLabel(tr("Device Type:"), grpRecord), 2, 0);
    layRecord->addWidget(new QLabel(tr("Driver Description:"), grpRecord), 3, 0);
    layRecord->addWidget(new QLabel(tr("Driver Name:"), grpRecord), 4, 0);
    layRecord->addWidget(new QLabel(tr("Driver Version:"), grpRecord), 5, 0);
    layRecord->addWidget(new QLabel(tr("Device Description:"), grpRecord), 6, 0);
    layRecord->addWidget(new QLabel(tr("Caps:"), grpRecord), 7, 0);
    layRecord->addWidget(new QLabel(tr("Layers At Once:"), grpRecord), 8, 0);
    layRecord->addWidget(new QLabel(tr("Memory Bytes:"), grpRecord), 9, 0);
    layRecord->addWidget(new QLabel(tr("ZBiasRating:"), grpRecord), 10, 0);
    layRecord->addWidget(new QLabel(tr("LODBiasRating:"), grpRecord), 11, 0);
    layRecord->addWidget(new QLabel(tr("FogExpApproxStart:"), grpRecord), 12, 0);
    layRecord->addWidget(new QLabel(tr("FogExp2ApproxStart:"), grpRecord), 13, 0);
    layRecord->addWidget(new QLabel(tr("FogEndBias:"), grpRecord), 14, 0);
    layRecord->addWidget(fRecordLineEdits[kDevMRecordVersion], 0, 1);
    layRecord->addWidget(fRecordLineEdits[kDevMFlags], 1, 1);
    layRecord->addWidget(fRecordLineEdits[kDevMDeviceType], 2, 1);
    layRecord->addWidget(fRecordLineEdits[kDevMDriverDesc], 3, 1);
    layRecord->addWidget(fRecordLineEdits[kDevMDriverName], 4, 1);
    layRecord->addWidget(fRecordLineEdits[kDevMDriverVersion], 5, 1);
    layRecord->addWidget(fRecordLineEdits[kDevMDeviceDesc], 6, 1);
    layRecord->addWidget(fRecordLineEdits[kDevMCaps], 7, 1);
    layRecord->addWidget(fRecordLineEdits[kDevMLayersAtOnce], 8, 1);
    layRecord->addWidget(fRecordLineEdits[kDevMMemoryBytes], 9, 1);
    layRecord->addWidget(fRecordLineEdits[kDevMZBiasRating], 10, 1);
    layRecord->addWidget(fRecordLineEdits[kDevMLODBiasRating], 11, 1);
    layRecord->addWidget(fRecordLineEdits[kDevMFogExpApproxStart], 12, 1);
    layRecord->addWidget(fRecordLineEdits[kDevMFogExp2ApproxStart], 13, 1);
    layRecord->addWidget(fRecordLineEdits[kDevMFogEndBias], 14, 1);

    //DeviceMode Group
    QGroupBox* grpMode = new QGroupBox(tr("Device Mode"), this);
    QGridLayout* layMode = new QGridLayout(grpMode);
    layMode->setVerticalSpacing(4);
    layMode->setHorizontalSpacing(8);
    for (size_t i = 0; i < kNumDevModeLineEdits; i++)
        fModeLineEdits[i] = new QLineEdit(grpMode);
    fModeCheckBoxWindowed = new QCheckBox(grpMode);
    fModeLabelCanRenderToCubics = new QLabel(grpMode);
    layMode->addWidget(new QLabel(tr("Width:"), grpMode), 0, 0);
    layMode->addWidget(new QLabel(tr("Height:"), grpMode), 1, 0);
    layMode->addWidget(new QLabel(tr("Depth:"), grpMode), 2, 0);
    layMode->addWidget(new QLabel(tr("Window Mode:"), grpMode), 3, 0);
    layMode->addWidget(new QLabel(tr("Can Render To Cubics:"), grpMode), 4, 0);
    layMode->addWidget(fModeLineEdits[kDevMWidth], 0, 1);
    layMode->addWidget(fModeLineEdits[kDevMHeight], 1, 1);
    layMode->addWidget(fModeLineEdits[kDevMDepth], 2, 1);
    layMode->addWidget(fModeCheckBoxWindowed, 3, 1);
    layMode->addWidget(fModeLabelCanRenderToCubics, 4, 1);
    connect(fModeCheckBoxWindowed, SIGNAL(toggled(bool)), fModeLineEdits[kDevMDepth], SLOT(setDisabled(bool)));

    //TextureQuality Group
    QGroupBox* grpTexQuality = new QGroupBox(tr("Texture Quality"), this);
    QGridLayout* layTexQuality = new QGridLayout(grpTexQuality);
    layTexQuality->setHorizontalSpacing(8);
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

            fRecordLineEdits[kDevMRecordVersion]->setText(QString().setNum(loadRec.getVersion()));
            fRecordLineEdits[kDevMFlags]->setText(QString().setNum(loadRec.getFlags()));
            fRecordLineEdits[kDevMDeviceType]->setText(QString().setNum(loadRec.getDeviceType()));
            fRecordLineEdits[kDevMDriverDesc]->setText(loadRec.getDriverDesc().cstr());
            fRecordLineEdits[kDevMDriverName]->setText(loadRec.getDriverName().cstr());
            fRecordLineEdits[kDevMDriverVersion]->setText(loadRec.getDriverVersion().cstr());
            fRecordLineEdits[kDevMDeviceDesc]->setText(loadRec.getDeviceDesc().cstr());
            fRecordLineEdits[kDevMCaps]->setText("### TODO ###");
            fRecordLineEdits[kDevMLayersAtOnce]->setText(QString().setNum(loadRec.getLayersAtOnce()));
            QString recordMemBytes = QString().setNum(loadRec.getMemoryBytes());
            QString recordMemBytesMB = QString().setNum(loadRec.getMemoryBytes() / 1024 / 1024);
            fRecordLineEdits[kDevMMemoryBytes]->setText(recordMemBytes + " (" + recordMemBytesMB + " MB)");
            fRecordLineEdits[kDevMZBiasRating]->setText(QString().setNum(loadRec.getZBiasRating()));
            fRecordLineEdits[kDevMLODBiasRating]->setText(QString().setNum(loadRec.getLODBIasRating()));
            fRecordLineEdits[kDevMFogExpApproxStart]->setText(QString().setNum(loadRec.getFogExpApproxStart()));
            fRecordLineEdits[kDevMFogExp2ApproxStart]->setText(QString().setNum(loadRec.getFogExp2ApproxStart()));
            fRecordLineEdits[kDevMFogEndBias]->setText(QString().setNum(loadRec.getFogEndBias()));

            uint16_t textureQuality = S->readShort();
            fSliderTextureQuality->setValue(textureQuality);
        } catch (std::exception &e) {
            plDebug::Error("Error reading dev_mode.dat file %s: %s", fFilename.toUtf8().data(), e.what());
            return false;
        }
    }
    return true;
}
