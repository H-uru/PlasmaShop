#ifndef _QWINSOUND_H
#define _QWINSOUND_H

#include "../QCreatable.h"

#include <PRP/Audio/plEAXEffects.h>
#include <QComboBox>
#include <QCheckBox>
#include "../QObjLink.h"

class QEaxSourceSettings : public QWidget {
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
    struct {
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
    virtual void closeEvent(QCloseEvent* evt);
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
    enum {
        kPropIs3DSound, kPropDisableLOD, kPropLooping, kPropAutoStart,
        kPropLocalOnly, kPropLoadOnlyOnCall, kPropFullyDisabled, kPropDontFade,
        kPropIncidental, kNumProps
    };
    QCheckBox* fProperties[kNumProps];

    // Fade Params
    struct {
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
    virtual void saveDamage();

protected slots:
    void setSoftRegion();
    void setSoftOcclusionRegion();
    void setDataBuffer();
    void unsetSoftRegion();
    void unsetSoftOcclusionRegion();
    void unsetDataBuffer();

    void editEaxSettings();
};

class QWin32Sound : public QCreatable {
    Q_OBJECT

protected:
    QCreatableLink* fSoundLink;
    QComboBox* fChannel;

public:
    QWin32Sound(plCreatable* pCre, QWidget* parent = NULL);
    virtual void saveDamage();
};

#endif
