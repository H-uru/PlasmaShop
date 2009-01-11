#ifndef _QMATERIAL_H
#define _QMATERIAL_H

#include "../QCreatable.h"

#include <PRP/Surface/hsGMaterial.h>
#include <QCheckBox>
#include "../QObjLink.h"
#include "../QKeyList.h"

class QMaterial : public QCreatable {
    Q_OBJECT

protected:
    enum {
        kCbShaded, kCbEnvironMap, kCbProjectOnto, kCbSoftShadow, kCbSpecular,
        kCbTwoSided, kCbDrawAsSplats, kCbAdjusted, kCbNoSoftShadow, kCbDynamic,
        kCbDecal, kCbIsEmissive, kCbIsLightMapped, kCbNeedsBlendChannel,
        kNumCompFlags
    };

    QCreatableLink* fSynchObjLink;
    QCheckBox* fCBFlags[kNumCompFlags];
    QKeyList* fLayers;
    QKeyList* fPiggyBacks;

public:
    QMaterial(plCreatable* pCre, QWidget* parent = NULL);
    virtual void saveDamage();
};

#endif
