#ifndef _QCUBICENVIRONMAP_H
#define _QCUBICENVIRONMAP_H

#include "QMipmap.h"
#include <PRP/Surface/plCubicEnvironmap.h>

class QCubicEnvironmap : public QCreatable {
    Q_OBJECT

protected:
    enum {
        kAlphaChannelFlag, kAlphaBitFlag, kBumpEnvMap, kForce32Bit,
        kDontThrowAwayImage, kForceOneMipLevel, kNoMaxSize, kIntensityMap,
        kHalfSize, kUserOwnsBitmap, kForceRewrite, kForceNonCompressed,
        kIsTexture, kIsOffscreen, kIsProjected, kIsOrtho, kNumBitmapFlags
    };
    QCheckBox* fFlags[kNumBitmapFlags];
    QCreatableLink* fFaces[plCubicEnvironmap::kNumFaces];
    QCreatableLink* fPreviewLink;

public:
    QCubicEnvironmap(plCreatable* pCre, QWidget* parent = NULL);
    virtual void saveDamage();
};

#endif
