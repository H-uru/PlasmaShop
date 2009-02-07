#ifndef _QDYNAMICTEXTMAP_H
#define _QDYNAMICTEXTMAP_H

#include "QMipmap.h"
#include <PRP/Surface/plDynamicTextMap.h>

class QDynamicTextMap : public QCreatable {
    Q_OBJECT

protected:
    enum {
        kAlphaChannelFlag, kAlphaBitFlag, kBumpEnvMap, kForce32Bit,
        kDontThrowAwayImage, kForceOneMipLevel, kNoMaxSize, kIntensityMap,
        kHalfSize, kUserOwnsBitmap, kForceRewrite, kForceNonCompressed,
        kIsTexture, kIsOffscreen, kIsProjected, kIsOrtho, kNumBitmapFlags
    };
    QCheckBox* fFlags[kNumBitmapFlags];
    QIntEdit* fWidth;
    QIntEdit* fHeight;
    QCheckBox* fHasAlpha;

public:
    QDynamicTextMap(plCreatable* pCre, QWidget* parent = NULL);
    virtual void saveDamage();
};

#endif
