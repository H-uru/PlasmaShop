#ifndef _QSOUNDBUFFER_H
#define _QSOUNDBUFFER_H

#include "../QCreatable.h"

#include <QCheckBox>
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
    QIntEdit* fBlockAlign;
    QIntEdit* fBitRate;
    QIntEdit* fSampleRate;
    QIntEdit* fAvgBytesPerSec;

public:
    QSoundBuffer(plCreatable* pCre, QWidget* parent = NULL);
    virtual void saveDamage();
};

#endif
