#ifndef _QSOUNDBUFFER_H
#define _QSOUNDBUFFER_H

#include "../QCreatable.h"

#include <QCheckBox>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <PRP/Audio/plSoundBuffer.h>

class QSoundBuffer : public QCreatable {
    Q_OBJECT

protected:
    enum {
        kIsExternal, kAlwaysExternal, kOnlyLeftChannel, kOnlyRightChannel,
        kStreamCompressed, kNumFlags
    };
    QCheckBox* fFlags[kNumFlags];

    QLineEdit* fFilename;
    QComboBox* fFormat;
    QSpinBox* fNumChannels;
    QSpinBox* fBlockAlign;
    QSpinBox* fBitRate;
    QSpinBox* fSampleRate;
    QSpinBox* fAvgBytesPerSec;

public:
    QSoundBuffer(plCreatable* pCre, QWidget* parent = NULL);
    virtual void saveDamage();
};

#endif
