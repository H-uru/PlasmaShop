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

#ifndef _QWINSOUND_H
#define _QWINSOUND_H

#include "PRP/QCreatable.h"

#include <PRP/Audio/plEAXEffects.h>
#include <QComboBox>
#include "QBitmaskCheckBox.h"
#include "PRP/QObjLink.h"

class QEaxSourceSettings : public QWidget
{
    Q_OBJECT

protected:
    QCheckBox* fEnabled;
    QIntEdit* fRoom;
    QIntEdit* fRoomHF;
    QCheckBox* fRoomAuto;
    QCheckBox* fRoomHFAuto;
    QIntEdit* fOutsideVolHF;
    QFloatEdit* fAirAbsorptionFactor;
    QFloatEdit* fRoomRolloffFactor;
    QFloatEdit* fDopplerFactor;
    QFloatEdit* fRolloffFactor;
    QFloatEdit* fOcclusionSoftValue;
    struct
    {
        QIntEdit* fOcclusion;
        QFloatEdit* fLFRatio;
        QFloatEdit* fRoomRatio;
        QFloatEdit* fDirectRatio;
    } fSoftStarts, fSoftEnds;

    plEAXSourceSettings* fEaxObject;

public:
    QEaxSourceSettings(plEAXSourceSettings* eax, QWidget* parent = NULL);
    void saveDamage();

protected:
    void closeEvent(QCloseEvent* evt) override;
};

class QWinSound : public QCreatable {
    Q_OBJECT

protected:
    QCreatableLink* fSynchObjLink;
    QComboBox* fSoundType;
    QIntEdit* fPriority;
    QCheckBox* fPlaying;
    QFloatEdit* fTime;
    QIntEdit* fMinFalloff;
    QIntEdit* fMaxFalloff;
    QIntEdit* fOuterVol;
    QIntEdit* fInnerCone;
    QIntEdit* fOuterCone;
    QFloatEdit* fCurrVolume;
    QFloatEdit* fDesiredVolume;
    QFloatEdit* fFadedVolume;
    QCreatableLink* fSoftRegion;
    QCreatableLink* fSoftOcclusionRegion;
    QCreatableLink* fDataBuffer;
    QLineEdit* fSubtitleId;

    // Properties
    enum
    {
        kPropIs3DSound, kPropDisableLOD, kPropLooping, kPropAutoStart,
        kPropLocalOnly, kPropLoadOnlyOnCall, kPropFullyDisabled, kPropDontFade,
        kPropIncidental, kNumProps
    };
    QBitmaskCheckBox* fProperties[kNumProps];

    // Fade Params
    struct
    {
        QFloatEdit* fLength;
        QFloatEdit* fVolStart;
        QFloatEdit* fVolEnd;
        QComboBox* fType;
        QFloatEdit* fCurrTime;
        QCheckBox* fStopWhenDone;
        QCheckBox* fFadeSoftVol;
    } fFadeInParams, fFadeOutParams;

public:
    QWinSound(plCreatable* pCre, QWidget* parent = NULL);
    void saveDamage() override;

protected slots:
    void setSoftRegion();
    void setSoftOcclusionRegion();
    void setDataBuffer();
    void unsetSoftRegion();
    void unsetSoftOcclusionRegion();
    void unsetDataBuffer();

    void editEaxSettings();
};

class QWin32Sound : public QCreatable
{
    Q_OBJECT

protected:
    QCreatableLink* fSoundLink;
    QComboBox* fChannel;

public:
    QWin32Sound(plCreatable* pCre, QWidget* parent = NULL);
    void saveDamage() override;
};

#endif
