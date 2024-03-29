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

#include "QWinSound.h"

#include <QGroupBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <PRP/Audio/plWin32StaticSound.h>
#include "QKeyDialog.h"
#include "Main.h"

/* QEaxSourceSettings */
QEaxSourceSettings::QEaxSourceSettings(plEAXSourceSettings* eax, QWidget* parent)
    : QWidget(parent), fEaxObject(eax)
{
    fEnabled = new QCheckBox(tr("Enabled"), this);
    fEnabled->setChecked(true);
    fRoomAuto = new QCheckBox(tr("Room Auto"), this);
    fRoomHFAuto = new QCheckBox(tr("Room HF Auto"), this);
    fRoom = new QIntEdit(this);
    fRoom->setRange(-0x8000, 0x7FFF);
    fRoomHF = new QIntEdit(this);
    fRoomHF->setRange(-0x8000, 0x7FFF);
    fOutsideVolHF = new QIntEdit(this);
    fOutsideVolHF->setRange(-0x8000, 0x7FFF);
    fAirAbsorptionFactor = new QFloatEdit(this);
    fRoomRolloffFactor = new QFloatEdit(this);
    fDopplerFactor = new QFloatEdit(this);
    fRolloffFactor = new QFloatEdit(this);
    fOcclusionSoftValue = new QFloatEdit(this);

    QGroupBox* grpSoftStarts = new QGroupBox(tr("Soft Starts"), this);
    QGridLayout* laySoftStarts = new QGridLayout(grpSoftStarts);
    laySoftStarts->setVerticalSpacing(4);
    laySoftStarts->setHorizontalSpacing(8);
    fSoftStarts.fOcclusion = new QIntEdit(grpSoftStarts);
    fSoftStarts.fOcclusion->setRange(-0x8000, 0x7FFF);
    fSoftStarts.fLFRatio = new QFloatEdit(grpSoftStarts);
    fSoftStarts.fRoomRatio = new QFloatEdit(grpSoftStarts);
    fSoftStarts.fDirectRatio = new QFloatEdit(grpSoftStarts);
    laySoftStarts->addWidget(new QLabel(tr("Occlusion:"), grpSoftStarts), 0, 0);
    laySoftStarts->addWidget(fSoftStarts.fOcclusion, 0, 1);
    laySoftStarts->addWidget(new QLabel(tr("LF Ratio:"), grpSoftStarts), 1, 0);
    laySoftStarts->addWidget(fSoftStarts.fLFRatio, 1, 1);
    laySoftStarts->addWidget(new QLabel(tr("Room Ratio:"), grpSoftStarts), 2, 0);
    laySoftStarts->addWidget(fSoftStarts.fRoomRatio, 2, 1);
    laySoftStarts->addWidget(new QLabel(tr("Direct Ratio:"), grpSoftStarts), 3, 0);
    laySoftStarts->addWidget(fSoftStarts.fDirectRatio, 3, 1);

    QGroupBox* grpSoftEnds = new QGroupBox(tr("Soft Ends"), this);
    QGridLayout* laySoftEnds = new QGridLayout(grpSoftEnds);
    laySoftEnds->setVerticalSpacing(4);
    laySoftEnds->setHorizontalSpacing(8);
    fSoftEnds.fOcclusion = new QIntEdit(grpSoftEnds);
    fSoftEnds.fOcclusion->setRange(-0x8000, 0x7FFF);
    fSoftEnds.fLFRatio = new QFloatEdit(grpSoftEnds);
    fSoftEnds.fRoomRatio = new QFloatEdit(grpSoftEnds);
    fSoftEnds.fDirectRatio = new QFloatEdit(grpSoftEnds);
    laySoftEnds->addWidget(new QLabel(tr("Occlusion:"), grpSoftEnds), 0, 0);
    laySoftEnds->addWidget(fSoftEnds.fOcclusion, 0, 1);
    laySoftEnds->addWidget(new QLabel(tr("LF Ratio:"), grpSoftEnds), 1, 0);
    laySoftEnds->addWidget(fSoftEnds.fLFRatio, 1, 1);
    laySoftEnds->addWidget(new QLabel(tr("Room Ratio:"), grpSoftEnds), 2, 0);
    laySoftEnds->addWidget(fSoftEnds.fRoomRatio, 2, 1);
    laySoftEnds->addWidget(new QLabel(tr("Direct Ratio:"), grpSoftEnds), 3, 0);
    laySoftEnds->addWidget(fSoftEnds.fDirectRatio, 3, 1);

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->setVerticalSpacing(4);
    layout->setHorizontalSpacing(8);
    layout->addWidget(fEnabled, 0, 0, 1, 2);
    layout->addWidget(fRoomAuto, 1, 0, 1, 2);
    layout->addWidget(fRoomHFAuto, 2, 0, 1, 2);
    layout->addWidget(new QLabel(tr("Room:"), this), 3, 0);
    layout->addWidget(fRoom, 3, 1);
    layout->addWidget(new QLabel(tr("Room HF:"), this), 4, 0);
    layout->addWidget(fRoomHF, 4, 1);
    layout->addWidget(new QLabel(tr("Outside Vol HF:"), this), 5, 0);
    layout->addWidget(fOutsideVolHF, 5, 1);
    layout->addWidget(new QLabel(tr("Air Absorption:"), this), 1, 2);
    layout->addWidget(fAirAbsorptionFactor, 1, 3);
    layout->addWidget(new QLabel(tr("Room Rolloff:"), this), 2, 2);
    layout->addWidget(fRoomRolloffFactor, 2, 3);
    layout->addWidget(new QLabel(tr("Doppler Factor:"), this), 3, 2);
    layout->addWidget(fDopplerFactor, 3, 3);
    layout->addWidget(new QLabel(tr("Rolloff Factor:"), this), 4, 2);
    layout->addWidget(fRolloffFactor, 4, 3);
    layout->addWidget(new QLabel(tr("Occlusion Soft Val:"), this), 5, 2);
    layout->addWidget(fOcclusionSoftValue, 5, 3);
    layout->addWidget(grpSoftStarts, 6, 0, 1, 2);
    layout->addWidget(grpSoftEnds, 6, 2, 1, 2);

    connect(fEnabled, &QCheckBox::toggled, this,
            [this, grpSoftStarts, grpSoftEnds](bool enabled) {
        fRoomAuto->setEnabled(enabled);
        fRoomHFAuto->setEnabled(enabled);
        fRoom->setEnabled(enabled);
        fRoomHF->setEnabled(enabled);
        fOutsideVolHF->setEnabled(enabled);
        fAirAbsorptionFactor->setEnabled(enabled);
        fRoomRolloffFactor->setEnabled(enabled);
        fDopplerFactor->setEnabled(enabled);
        fRolloffFactor->setEnabled(enabled);
        fOcclusionSoftValue->setEnabled(enabled);
        grpSoftStarts->setEnabled(enabled);
        grpSoftEnds->setEnabled(enabled);
    });

    fRoomAuto->setChecked(fEaxObject->getRoomAuto());
    fRoomHFAuto->setChecked(fEaxObject->getRoomHFAuto());
    fRoom->setValue(fEaxObject->getRoom());
    fRoomHF->setValue(fEaxObject->getRoomHF());
    fOutsideVolHF->setValue(fEaxObject->getOutsideVolHF());
    fAirAbsorptionFactor->setValue(fEaxObject->getAirAbsorptionFactor());
    fRoomRolloffFactor->setValue(fEaxObject->getRoomRolloffFactor());
    fDopplerFactor->setValue(fEaxObject->getDopplerFactor());
    fRolloffFactor->setValue(fEaxObject->getRolloffFactor());
    fOcclusionSoftValue->setValue(fEaxObject->getOcclusionSoftValue());
    fSoftStarts.fOcclusion->setValue(fEaxObject->getSoftStarts().getOcclusion());
    fSoftStarts.fLFRatio->setValue(fEaxObject->getSoftStarts().getLFRatio());
    fSoftStarts.fRoomRatio->setValue(fEaxObject->getSoftStarts().getRoomRatio());
    fSoftStarts.fDirectRatio->setValue(fEaxObject->getSoftStarts().getDirectRatio());
    fSoftEnds.fOcclusion->setValue(fEaxObject->getSoftEnds().getOcclusion());
    fSoftEnds.fLFRatio->setValue(fEaxObject->getSoftEnds().getLFRatio());
    fSoftEnds.fRoomRatio->setValue(fEaxObject->getSoftEnds().getRoomRatio());
    fSoftEnds.fDirectRatio->setValue(fEaxObject->getSoftEnds().getDirectRatio());
    fEnabled->setChecked(fEaxObject->isEnabled());

    connect(fRoomAuto, &QCheckBox::clicked, [this](bool checked) {
        fEaxObject->setRoomAuto(checked);
    });
    connect(fRoomHFAuto, &QCheckBox::clicked, [this](bool checked) {
        fEaxObject->setRoomHFAuto(checked);
    });
}

void QEaxSourceSettings::saveDamage()
{
    fEaxObject->setRoom(fRoom->value());
    fEaxObject->setRoomHF(fRoomHF->value());
    fEaxObject->setOutsideVolHF(fOutsideVolHF->value());
    fEaxObject->setAirAbsorptionFactor(fAirAbsorptionFactor->value());
    fEaxObject->setRoomRolloffFactor(fRoomRolloffFactor->value());
    fEaxObject->setDopplerFactor(fDopplerFactor->value());
    fEaxObject->setRolloffFactor(fRolloffFactor->value());
    fEaxObject->setOcclusionSoftValue(fOcclusionSoftValue->value());
    fEaxObject->getSoftStarts().setOcclusion(fSoftStarts.fOcclusion->value());
    fEaxObject->getSoftStarts().setLFRatio(fSoftStarts.fLFRatio->value());
    fEaxObject->getSoftStarts().setRoomRatio(fSoftStarts.fRoomRatio->value());
    fEaxObject->getSoftStarts().setDirectRatio(fSoftStarts.fDirectRatio->value());
    fEaxObject->getSoftEnds().setOcclusion(fSoftEnds.fOcclusion->value());
    fEaxObject->getSoftEnds().setLFRatio(fSoftEnds.fLFRatio->value());
    fEaxObject->getSoftEnds().setRoomRatio(fSoftEnds.fRoomRatio->value());
    fEaxObject->getSoftEnds().setDirectRatio(fSoftEnds.fDirectRatio->value());
    fEaxObject->enable(fEnabled->isChecked());
}

void QEaxSourceSettings::closeEvent(QCloseEvent*)
{
    saveDamage();
}


/* QWinSound */
QWinSound::QWinSound(plCreatable* pCre, QWidget* parent)
    : QCreatable(pCre, kSound, parent)
{
    plSound* obj = plSound::Convert(fCreatable);

    fSynchObjLink = new QCreatableLink(this, false);
    fSynchObjLink->setCreatable(obj, tr("Synch Flags"));
    fSynchObjLink->setForceType(kSynchedObject);

    fTime = new QFloatEdit(this);
    fMaxFalloff = new QIntEdit(this);
    fMinFalloff = new QIntEdit(this);
    fCurrVolume = new QFloatEdit(this);
    fDesiredVolume = new QFloatEdit(this);
    fSoundType = new QComboBox(this);
    fSoundType->addItems(QStringList() << "Sound FX" << "Ambience"
                         << "Background Music" << "GUI Sound" << "NPC Voices");
    fOuterVol = new QIntEdit(this);
    fInnerCone = new QIntEdit(this);
    fOuterCone = new QIntEdit(this);
    fFadedVolume = new QFloatEdit(this);
    fPriority = new QIntEdit(this);
    fPriority->setRange(0, 0xFF);
    fPlaying = new QCheckBox(tr("Playing"), this);
    fSubtitleId = new QLineEdit(this);

    fTime->setValue(obj->getTime());
    fMaxFalloff->setValue(obj->getMaxFalloff());
    fMinFalloff->setValue(obj->getMinFalloff());
    fCurrVolume->setValue(obj->getCurrVolume());
    fDesiredVolume->setValue(obj->getDesiredVolume());
    fSoundType->setCurrentIndex(obj->getType());
    fOuterVol->setValue(obj->getOuterVol());
    fInnerCone->setValue(obj->getInnerCone());
    fOuterCone->setValue(obj->getOuterCone());
    fFadedVolume->setValue(obj->getFadedVolume());
    fPriority->setValue(obj->getPriority());
    fPlaying->setChecked(obj->isPlaying());
    fSubtitleId->setText(st2qstr(obj->getSubtitleId()));

    connect(fPlaying, &QCheckBox::clicked, [this](bool checked) {
        plSound* obj = plSound::Convert(fCreatable);
        obj->setPlaying(checked);
    });

    QGroupBox* grpProperties = new QGroupBox(tr("Properties"), this);
    QGridLayout* layProperties = new QGridLayout(grpProperties);
    layProperties->setVerticalSpacing(0);
    layProperties->setHorizontalSpacing(8);
    fProperties[kPropIs3DSound] = new QBitmaskCheckBox(plSound::kPropIs3DSound,
            tr("Is 3D Sound"), grpProperties);
    fProperties[kPropDisableLOD] = new QBitmaskCheckBox(plSound::kPropDisableLOD,
            tr("Disable LOD"), grpProperties);
    fProperties[kPropLooping] = new QBitmaskCheckBox(plSound::kPropLooping,
            tr("Looping"), grpProperties);
    fProperties[kPropAutoStart] = new QBitmaskCheckBox(plSound::kPropAutoStart,
            tr("Auto Start"), grpProperties);
    fProperties[kPropLocalOnly] = new QBitmaskCheckBox(plSound::kPropLocalOnly,
            tr("Local Only"), grpProperties);
    fProperties[kPropLoadOnlyOnCall] = new QBitmaskCheckBox(plSound::kPropLoadOnlyOnCall,
            tr("Load Only on Call"), grpProperties);
    fProperties[kPropFullyDisabled] = new QBitmaskCheckBox(plSound::kPropFullyDisabled,
            tr("Fully Disabled"), grpProperties);
    fProperties[kPropDontFade] = new QBitmaskCheckBox(plSound::kPropDontFade,
            tr("Don't Fade"), grpProperties);
    fProperties[kPropIncidental] = new QBitmaskCheckBox(plSound::kPropIncidental,
            tr("Incidental"), grpProperties);
    layProperties->addWidget(fProperties[kPropIs3DSound], 0, 0);
    layProperties->addWidget(fProperties[kPropDisableLOD], 1, 0);
    layProperties->addWidget(fProperties[kPropLooping], 2, 0);
    layProperties->addWidget(fProperties[kPropAutoStart], 0, 1);
    layProperties->addWidget(fProperties[kPropLocalOnly], 1, 1);
    layProperties->addWidget(fProperties[kPropLoadOnlyOnCall], 2, 1);
    layProperties->addWidget(fProperties[kPropFullyDisabled], 0, 2);
    layProperties->addWidget(fProperties[kPropDontFade], 1, 2);
    layProperties->addWidget(fProperties[kPropIncidental], 2, 2);
    for (auto prop : fProperties) {
        prop->setFrom(obj->getProperties());
        connect(prop, &QBitmaskCheckBox::setBits, this, [this](unsigned int mask) {
            plSound* obj = plSound::Convert(fCreatable);
            obj->setProperties(obj->getProperties() | mask);
        });
        connect(prop, &QBitmaskCheckBox::unsetBits, this, [this](unsigned int mask) {
            plSound* obj = plSound::Convert(fCreatable);
            obj->setProperties(obj->getProperties() & ~mask);
        });
    }

    QGroupBox* grpFadeIn = new QGroupBox(tr("Fade In Parameters"), this);
    QGridLayout* layFadeIn = new QGridLayout(grpFadeIn);
    layFadeIn->setVerticalSpacing(4);
    layFadeIn->setHorizontalSpacing(8);
    fFadeInParams.fLength = new QFloatEdit(grpFadeIn);
    fFadeInParams.fVolStart = new QFloatEdit(grpFadeIn);
    fFadeInParams.fVolEnd = new QFloatEdit(grpFadeIn);
    fFadeInParams.fType = new QComboBox(grpFadeIn);
    fFadeInParams.fCurrTime = new QFloatEdit(grpFadeIn);
    fFadeInParams.fStopWhenDone = new QCheckBox(tr("Stop When Done"), grpFadeIn);
    fFadeInParams.fFadeSoftVol = new QCheckBox(tr("Fade Soft Volume"), grpFadeIn);
    fFadeInParams.fType->addItems(QStringList() << "Linear" << "Logarithmic" << "Exponential");
    fFadeInParams.fLength->setValue(obj->getFadeInParams().fLengthInSecs);
    fFadeInParams.fVolStart->setValue(obj->getFadeInParams().fVolStart);
    fFadeInParams.fVolEnd->setValue(obj->getFadeInParams().fVolEnd);
    fFadeInParams.fType->setCurrentIndex(obj->getFadeInParams().fType);
    fFadeInParams.fCurrTime->setValue(obj->getFadeInParams().fCurrTime);
    fFadeInParams.fStopWhenDone->setChecked(obj->getFadeInParams().fStopWhenDone);
    fFadeInParams.fFadeSoftVol->setChecked(obj->getFadeInParams().fFadeSoftVol);
    layFadeIn->addWidget(new QLabel(tr("Length (Secs):"), grpFadeIn), 0, 0);
    layFadeIn->addWidget(fFadeInParams.fLength, 0, 1);
    layFadeIn->addWidget(new QLabel(tr("Volume Start:"), grpFadeIn), 1, 0);
    layFadeIn->addWidget(fFadeInParams.fVolStart, 1, 1);
    layFadeIn->addWidget(new QLabel(tr("Volume End:"), grpFadeIn), 2, 0);
    layFadeIn->addWidget(fFadeInParams.fVolEnd, 2, 1);
    layFadeIn->addWidget(new QLabel(tr("Type:"), grpFadeIn), 3, 0);
    layFadeIn->addWidget(fFadeInParams.fType, 3, 1);
    layFadeIn->addWidget(new QLabel(tr("Current Time:"), grpFadeIn), 4, 0);
    layFadeIn->addWidget(fFadeInParams.fCurrTime, 4, 1);
    layFadeIn->addWidget(fFadeInParams.fStopWhenDone, 5, 0, 1, 2);
    layFadeIn->addWidget(fFadeInParams.fFadeSoftVol, 6, 0, 1, 2);

    connect(fFadeInParams.fStopWhenDone, &QCheckBox::clicked, [this](bool checked) {
        plSound* obj = plSound::Convert(fCreatable);
        obj->getFadeInParams().fStopWhenDone = checked;
    });
    connect(fFadeInParams.fFadeSoftVol, &QCheckBox::clicked, [this](bool checked) {
        plSound* obj = plSound::Convert(fCreatable);
        obj->getFadeInParams().fFadeSoftVol = checked;
    });

    QGroupBox* grpFadeOut = new QGroupBox(tr("Fade Out Parameters"), this);
    QGridLayout* layFadeOut = new QGridLayout(grpFadeOut);
    layFadeOut->setVerticalSpacing(4);
    layFadeOut->setHorizontalSpacing(8);
    fFadeOutParams.fLength = new QFloatEdit(grpFadeOut);
    fFadeOutParams.fVolStart = new QFloatEdit(grpFadeOut);
    fFadeOutParams.fVolEnd = new QFloatEdit(grpFadeOut);
    fFadeOutParams.fType = new QComboBox(grpFadeOut);
    fFadeOutParams.fCurrTime = new QFloatEdit(grpFadeOut);
    fFadeOutParams.fStopWhenDone = new QCheckBox(tr("Stop When Done"), grpFadeOut);
    fFadeOutParams.fFadeSoftVol = new QCheckBox(tr("Fade Soft Volume"), grpFadeOut);
    fFadeOutParams.fType->addItems(QStringList() << "Linear" << "Logarithmic" << "Exponential");
    fFadeOutParams.fLength->setValue(obj->getFadeOutParams().fLengthInSecs);
    fFadeOutParams.fVolStart->setValue(obj->getFadeOutParams().fVolStart);
    fFadeOutParams.fVolEnd->setValue(obj->getFadeOutParams().fVolEnd);
    fFadeOutParams.fType->setCurrentIndex(obj->getFadeOutParams().fType);
    fFadeOutParams.fCurrTime->setValue(obj->getFadeOutParams().fCurrTime);
    fFadeOutParams.fStopWhenDone->setChecked(obj->getFadeOutParams().fStopWhenDone);
    fFadeOutParams.fFadeSoftVol->setChecked(obj->getFadeOutParams().fFadeSoftVol);
    layFadeOut->addWidget(new QLabel(tr("Length (Secs):"), grpFadeOut), 0, 0);
    layFadeOut->addWidget(fFadeOutParams.fLength, 0, 1);
    layFadeOut->addWidget(new QLabel(tr("Volume Start:"), grpFadeOut), 1, 0);
    layFadeOut->addWidget(fFadeOutParams.fVolStart, 1, 1);
    layFadeOut->addWidget(new QLabel(tr("Volume End:"), grpFadeOut), 2, 0);
    layFadeOut->addWidget(fFadeOutParams.fVolEnd, 2, 1);
    layFadeOut->addWidget(new QLabel(tr("Type:"), grpFadeOut), 3, 0);
    layFadeOut->addWidget(fFadeOutParams.fType, 3, 1);
    layFadeOut->addWidget(new QLabel(tr("Current Time:"), grpFadeOut), 4, 0);
    layFadeOut->addWidget(fFadeOutParams.fCurrTime, 4, 1);
    layFadeOut->addWidget(fFadeOutParams.fStopWhenDone, 5, 0, 1, 2);
    layFadeOut->addWidget(fFadeOutParams.fFadeSoftVol, 6, 0, 1, 2);

    connect(fFadeOutParams.fStopWhenDone, &QCheckBox::clicked, [this](bool checked) {
        plSound* obj = plSound::Convert(fCreatable);
        obj->getFadeOutParams().fStopWhenDone = checked;
    });
    connect(fFadeOutParams.fFadeSoftVol, &QCheckBox::clicked, [this](bool checked) {
        plSound* obj = plSound::Convert(fCreatable);
        obj->getFadeOutParams().fFadeSoftVol = checked;
    });

    fSoftRegion = new QCreatableLink(this);
    fSoftRegion->setKey(obj->getSoftRegion());

    fDataBuffer = new QCreatableLink(this);
    fDataBuffer->setKey(obj->getDataBuffer());

    fSoftOcclusionRegion = new QCreatableLink(this);
    fSoftOcclusionRegion->setKey(obj->getSoftOcclusionRegion());

    QWidget* eaxWidget = new QWidget(this);
    QHBoxLayout* eaxLayout = new QHBoxLayout(eaxWidget);
    eaxLayout->setContentsMargins(0, 0, 0, 0);
    eaxLayout->setSpacing(4);
    QLinkLabel* linkEaxSettings = new QLinkLabel(eaxWidget);
    linkEaxSettings->setText(tr("EAX Settings"));
    eaxLayout->addWidget(linkEaxSettings);
    eaxLayout->addStretch();
    connect(linkEaxSettings, &QLinkLabel::activated, this, &QWinSound::editEaxSettings);

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(fSynchObjLink, 0, 0, 1, 4);
    layout->addWidget(new QLabel(tr("Time:"), this), 1, 0);
    layout->addWidget(fTime, 1, 1);
    layout->addWidget(new QLabel(tr("Max Falloff:"), this), 2, 0);
    layout->addWidget(fMaxFalloff, 2, 1);
    layout->addWidget(new QLabel(tr("Min Falloff:"), this), 3, 0);
    layout->addWidget(fMinFalloff, 3, 1);
    layout->addWidget(new QLabel(tr("Current Vol:"), this), 4, 0);
    layout->addWidget(fCurrVolume, 4, 1);
    layout->addWidget(new QLabel(tr("Desired Vol:"), this), 5, 0);
    layout->addWidget(fDesiredVolume, 5, 1);
    layout->addWidget(new QLabel(tr("Type:"), this), 6, 0);
    layout->addWidget(fSoundType, 6, 1);
    layout->addWidget(new QLabel(tr("Outer Volume:"), this), 1, 2);
    layout->addWidget(fOuterVol, 1, 3);
    layout->addWidget(new QLabel(tr("Inner Cone:"), this), 2, 2);
    layout->addWidget(fInnerCone, 2, 3);
    layout->addWidget(new QLabel(tr("Outer Cone:"), this), 3, 2);
    layout->addWidget(fOuterCone, 3, 3);
    layout->addWidget(new QLabel(tr("Faded Volume:"), this), 4, 2);
    layout->addWidget(fFadedVolume, 4, 3);
    layout->addWidget(new QLabel(tr("Priority:"), this), 5, 2);
    layout->addWidget(fPriority, 5, 3);
    layout->addWidget(fPlaying, 6, 2, 1, 2, Qt::AlignRight);
    layout->addWidget(new QLabel(tr("Subtitle ID:"), this), 7, 0);
    layout->addWidget(fSubtitleId, 7, 1, 1, 3);
    layout->addWidget(grpProperties, 8, 0, 1, 4);
    layout->addWidget(grpFadeIn, 9, 0, 1, 2);
    layout->addWidget(grpFadeOut, 9, 2, 1, 2);
    layout->addWidget(new QLabel(tr("Soft Region:"), this), 10, 0);
    layout->addWidget(fSoftRegion, 10, 1, 1, 3);
    layout->addWidget(new QLabel(tr("Data Buffer:"), this), 11, 0);
    layout->addWidget(fDataBuffer, 11, 1, 1, 3);
    layout->addWidget(new QLabel(tr("Soft Occlusion:"), this), 12, 0);
    layout->addWidget(fSoftOcclusionRegion, 12, 1, 1, 3);
    layout->addWidget(eaxWidget, 13, 0, 1, 4);

    connect(fSoftRegion, &QCreatableLink::addObject, this, &QWinSound::setSoftRegion);
    connect(fSoftRegion, &QCreatableLink::delObject, this, &QWinSound::unsetSoftRegion);
    connect(fDataBuffer, &QCreatableLink::addObject, this, &QWinSound::setDataBuffer);
    connect(fDataBuffer, &QCreatableLink::delObject, this, &QWinSound::unsetDataBuffer);
    connect(fSoftOcclusionRegion, &QCreatableLink::addObject, this, &QWinSound::setSoftOcclusionRegion);
    connect(fSoftOcclusionRegion, &QCreatableLink::delObject, this, &QWinSound::unsetSoftOcclusionRegion);
}

void QWinSound::saveDamage()
{
    plSound* obj = plSound::Convert(fCreatable);

    obj->setTime(fTime->value());
    obj->setFalloff(fMinFalloff->value(), fMaxFalloff->value());
    obj->setCurrVolume(fCurrVolume->value());
    obj->setDesiredVolume(fDesiredVolume->value());
    obj->setType(fSoundType->currentIndex());
    obj->setOuterVol(fOuterVol->value());
    obj->setCone(fInnerCone->value(), fOuterCone->value());
    obj->setFadedVolume(fFadedVolume->value());
    obj->setPriority(fPriority->value());
    obj->setSubtitleId(qstr2st(fSubtitleId->text()));
    obj->getFadeInParams().fLengthInSecs = fFadeInParams.fLength->value();
    obj->getFadeInParams().fVolStart = fFadeInParams.fVolStart->value();
    obj->getFadeInParams().fVolEnd = fFadeInParams.fVolEnd->value();
    obj->getFadeInParams().fType = fFadeInParams.fType->currentIndex();
    obj->getFadeInParams().fCurrTime = fFadeInParams.fCurrTime->value();
    obj->getFadeOutParams().fLengthInSecs = fFadeOutParams.fLength->value();
    obj->getFadeOutParams().fVolStart = fFadeOutParams.fVolStart->value();
    obj->getFadeOutParams().fVolEnd = fFadeOutParams.fVolEnd->value();
    obj->getFadeOutParams().fType = fFadeOutParams.fType->currentIndex();
    obj->getFadeOutParams().fCurrTime = fFadeOutParams.fCurrTime->value();
}

void QWinSound::setSoftRegion()
{
    plSound* obj = plSound::Convert(fCreatable);
    QFindKeyDialog dlg(this);
    if (obj->getSoftRegion().Exists())
        dlg.init(PrpShopMain::ResManager(), obj->getSoftRegion());
    else
        dlg.init(PrpShopMain::ResManager(), obj->getKey()->getLocation());
    if (dlg.exec() == QDialog::Accepted) {
        obj->setSoftRegion(dlg.selection());
        fSoftRegion->setKey(obj->getSoftRegion());
    }
}

void QWinSound::setDataBuffer()
{
    plSound* obj = plSound::Convert(fCreatable);
    QFindKeyDialog dlg(this);
    if (obj->getDataBuffer().Exists())
        dlg.init(PrpShopMain::ResManager(), obj->getDataBuffer());
    else
        dlg.init(PrpShopMain::ResManager(), obj->getKey()->getLocation(), kSoundBuffer);
    if (dlg.exec() == QDialog::Accepted) {
        obj->setDataBuffer(dlg.selection());
        fDataBuffer->setKey(obj->getDataBuffer());
    }
}

void QWinSound::setSoftOcclusionRegion()
{
    plSound* obj = plSound::Convert(fCreatable);
    QFindKeyDialog dlg(this);
    if (obj->getSoftOcclusionRegion().Exists())
        dlg.init(PrpShopMain::ResManager(), obj->getSoftOcclusionRegion());
    else
        dlg.init(PrpShopMain::ResManager(), obj->getKey()->getLocation());
    if (dlg.exec() == QDialog::Accepted) {
        obj->setSoftOcclusionRegion(dlg.selection());
        fSoftOcclusionRegion->setKey(obj->getSoftOcclusionRegion());
    }
}

void QWinSound::unsetSoftRegion()
{
    plSound* obj = plSound::Convert(fCreatable);
    obj->setSoftRegion(plKey());
    fSoftRegion->setCreatable(NULL, "(None)");
}

void QWinSound::unsetDataBuffer()
{
    plSound* obj = plSound::Convert(fCreatable);
    obj->setDataBuffer(plKey());
    fDataBuffer->setCreatable(NULL, "(None)");
}

void QWinSound::unsetSoftOcclusionRegion()
{
    plSound* obj = plSound::Convert(fCreatable);
    obj->setSoftOcclusionRegion(plKey());
    fSoftOcclusionRegion->setCreatable(NULL, "(None)");
}

void QWinSound::editEaxSettings()
{
    QEaxSourceSettings* eaxDlg = new QEaxSourceSettings(&(plSound::Convert(fCreatable))->getEAXSettings());
    eaxDlg->setWindowTitle(tr("EAX Settings: %1")
                           .arg(st2qstr(plSound::Convert(fCreatable)->getKey()->getName())));
    eaxDlg->show();
}


/* QWin32Sound */
QWin32Sound::QWin32Sound(plCreatable* pCre, QWidget* parent)
    : QCreatable(pCre, pCre->ClassIndex(), parent)
{
    plWin32Sound* obj = plWin32Sound::Convert(fCreatable);

    fSoundLink = new QCreatableLink(this, false);
    fSoundLink->setCreatable(obj, tr("Sound Properties"));
    fSoundLink->setForceType(kSound);

    fChannel = new QComboBox(this);
    fChannel->addItem("Left", plWin32Sound::kLeftChannel);
    fChannel->addItem("Right", plWin32Sound::kRightChannel);
    int channelIdx = fChannel->findData(obj->getChannel());
    if (channelIdx < 0) {
        fChannel->addItem(QString("Unknown Value: %1").arg(obj->getChannel()),
                          obj->getChannel());
        fChannel->setCurrentIndex(fChannel->count() - 1);
    } else {
        fChannel->setCurrentIndex(channelIdx);
    }

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(fSoundLink, 0, 0, 1, 2);
    layout->addWidget(new QLabel(tr("Channel:"), this), 1, 0);
    layout->addWidget(fChannel, 1, 1);
}

void QWin32Sound::saveDamage()
{
    plWin32Sound* obj = plWin32Sound::Convert(fCreatable);
    obj->setChannel(fChannel->itemData(fChannel->currentIndex()).toUInt());
}
