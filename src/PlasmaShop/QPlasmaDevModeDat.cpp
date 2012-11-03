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
    QGroupBox* grpRecord = new QGroupBox(tr("Device Record"), this);
    QGridLayout* layRecord = new QGridLayout(grpRecord);
    layRecord->setVerticalSpacing(0);
    layRecord->setHorizontalSpacing(8);
    for (size_t i = 0; i < kNumDevRecordEntries; i++) {
        fRecordEntries[i] = new QLineEdit(grpRecord);
        fRecordEntries[i]->setReadOnly(true);
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
    layRecord->addWidget(new QLabel(tr("Num Modes:"), grpRecord), 10, 0);
    layRecord->addWidget(new QLabel(tr("ZBiasRating:"), grpRecord), 11, 0);
    layRecord->addWidget(new QLabel(tr("LODBiasRating:"), grpRecord), 12, 0);
    layRecord->addWidget(new QLabel(tr("FogExpApproxStart:"), grpRecord), 13, 0);
    layRecord->addWidget(new QLabel(tr("FogExp2ApproxStart:"), grpRecord), 14, 0);
    layRecord->addWidget(new QLabel(tr("FogEndBias:"), grpRecord), 15, 0);
    layRecord->addWidget(fRecordEntries[kDevMRecordVersion], 0, 1);
    layRecord->addWidget(fRecordEntries[kDevMFlags], 1, 1);
    layRecord->addWidget(fRecordEntries[kDevMDeviceType], 2, 1);
    layRecord->addWidget(fRecordEntries[kDevMDriverDesc], 3, 1);
    layRecord->addWidget(fRecordEntries[kDevMDriverName], 4, 1);
    layRecord->addWidget(fRecordEntries[kDevMDriverVersion], 5, 1);
    layRecord->addWidget(fRecordEntries[kDevMDeviceDesc], 6, 1);
    layRecord->addWidget(fRecordEntries[kDevMCaps], 7, 1);
    layRecord->addWidget(fRecordEntries[kDevMLayersAtOnce], 8, 1);
    layRecord->addWidget(fRecordEntries[kDevMMemoryBytes], 9, 1);
    layRecord->addWidget(fRecordEntries[kDevMNumModes], 10, 1);
    layRecord->addWidget(fRecordEntries[kDevMZBiasRating], 11, 1);
    layRecord->addWidget(fRecordEntries[kDevMLODBiasRating], 12, 1);
    layRecord->addWidget(fRecordEntries[kDevMFogExpApproxStart], 13, 1);
    layRecord->addWidget(fRecordEntries[kDevMFogExp2ApproxStart], 14, 1);
    layRecord->addWidget(fRecordEntries[kDevMFogEndBias], 15, 1);

    //DeviceMode Group
    QGroupBox* grpMode = new QGroupBox(tr("Device Mode"), this);
    QGridLayout* layMode = new QGridLayout(grpMode);
    layMode->setVerticalSpacing(0);
    layMode->setHorizontalSpacing(8);
    for (size_t i = 0; i < kNumDevModeEntries; i++) {
        fModeEntries[i] = new QLineEdit(grpMode);
        fModeEntries[i]->setReadOnly(true);
    }
    layMode->addWidget(new QLabel(tr("Width:"), grpMode), 0, 0);
    layMode->addWidget(new QLabel(tr("Height:"), grpMode), 1, 0);
    layMode->addWidget(new QLabel(tr("Depth:"), grpMode), 2, 0);
    layMode->addWidget(new QLabel(tr("Can Render To Cubics:"), grpMode), 3, 0);
    layMode->addWidget(fModeEntries[kDevMWidth], 0, 1);
    layMode->addWidget(fModeEntries[kDevMHeight], 1, 1);
    layMode->addWidget(fModeEntries[kDevMDepth], 2, 1);
    layMode->addWidget(fModeEntries[kDevMCanRenderToCubics], 3, 1);

    //TextureQuality Group
    QGroupBox* grpTexQuality = new QGroupBox(tr("Texture Quality"), this);
    QGridLayout* layTexQuality = new QGridLayout(grpTexQuality);
    layTexQuality->setVerticalSpacing(0);
    layTexQuality->setHorizontalSpacing(8);
    fTexQualityEntry = new QLineEdit(grpTexQuality);
    fTexQualityEntry->setReadOnly(true);
    layTexQuality->addWidget(new QLabel(tr("Quality:"), grpTexQuality), 0, 0);
    layTexQuality->addWidget(fTexQualityEntry, 0, 1);

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

            fModeEntries[kDevMWidth]->setText(QString().setNum(loadMode.getWidth()));
            fModeEntries[kDevMHeight]->setText(QString().setNum(loadMode.getHeight()));
            fModeEntries[kDevMDepth]->setText(QString().setNum(loadMode.getDepth()));
            fModeEntries[kDevMCanRenderToCubics]->setText(QString().setNum(loadMode.getCanRenderToCubics()));

            fRecordEntries[kDevMRecordVersion]->setText(QString().setNum(loadRec.getVersion()));
            fRecordEntries[kDevMFlags]->setText(QString().setNum(loadRec.getFlags()));
            fRecordEntries[kDevMDeviceType]->setText(QString().setNum(loadRec.getDeviceType()));
            fRecordEntries[kDevMDriverDesc]->setText(loadRec.getDriverDesc().cstr());
            fRecordEntries[kDevMDriverName]->setText(loadRec.getDriverName().cstr());
            fRecordEntries[kDevMDriverVersion]->setText(loadRec.getDriverVersion().cstr());
            fRecordEntries[kDevMDeviceDesc]->setText(loadRec.getDeviceDesc().cstr());
            fRecordEntries[kDevMCaps]->setText("### TODO ###");
            fRecordEntries[kDevMLayersAtOnce]->setText(QString().setNum(loadRec.getLayersAtOnce()));
            QString recordMemBytes = QString().setNum(loadRec.getMemoryBytes());
            QString recordMemBytesMB = QString().setNum(loadRec.getMemoryBytes() / 1024 / 1024);
            fRecordEntries[kDevMMemoryBytes]->setText(recordMemBytes + " (" + recordMemBytesMB + " MB)");
            fRecordEntries[kDevMNumModes]->setText(QString().setNum(loadRec.getNumModes()));
            fRecordEntries[kDevMZBiasRating]->setText(QString().setNum(loadRec.getZBiasRating()));
            fRecordEntries[kDevMLODBiasRating]->setText(QString().setNum(loadRec.getLODBIasRating()));
            fRecordEntries[kDevMFogExpApproxStart]->setText(QString().setNum(loadRec.getFogExpApproxStart()));
            fRecordEntries[kDevMFogExp2ApproxStart]->setText(QString().setNum(loadRec.getFogExp2ApproxStart()));
            fRecordEntries[kDevMFogEndBias]->setText(QString().setNum(loadRec.getFogEndBias()));

            uint16_t textureQuality = S->readShort();
            fTexQualityEntry->setText(QString().setNum(textureQuality));
        } catch (std::exception &e) {
            plDebug::Error("Error reading dev_mode.dat file %s: %s", fFilename.toUtf8().data(), e.what());
            return false;
        }
    }
    return true;
}
