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

#ifndef _QLAYER_H
#define _QLAYER_H

#include "PRP/QCreatable.h"

#include <PRP/Surface/plLayer.h>
#include <QCheckBox>
#include <QSpinBox>
#include <QLineEdit>
#include "PRP/QObjLink.h"
#include "PRP/QMatrix44.h"
#include "QColorEdit.h"

class QLayer : public QCreatable {
    Q_OBJECT

protected:
    QCreatableLink* fSynchObjLink;
    QCreatableLink* fBaseLayer;
    QCreatableLink* fTexture;
    QCreatableLink* fVShader;
    QCreatableLink* fPShader;
    QMatrix44* fUvwTransform;
    QMatrix44* fBumpEnvTransform;
    QColorEdit* fPreshade;
    QColorEdit* fRuntime;
    QColorEdit* fAmbient;
    QColorEdit* fSpecular;
    QSpinBox* fUvwSrc;
    QDoubleSpinBox* fOpacity;
    QFloatEdit* fLodBias;
    QFloatEdit* fSpecPower;

    enum {
        kBlendTest, kBlendAlpha, kBlendMult, kBlendAdd, kBlendAddColorTimesAlpha,
        kBlendAntiAlias, kBlendDetail, kBlendNoColor, kBlendMADD, kBlendDot3,
        kBlendAddSigned, kBlendAddSigned2X, kBlendInvertAlpha, kBlendInvertColor,
        kBlendAlphaMult, kBlendAlphaAdd, kBlendNoVtxAlpha, kBlendNoTexColor,
        kBlendNoTexAlpha, kBlendInvertVtxAlpha, kBlendAlphaAlways,
        kBlendInvertFinalColor, kBlendInvertFinalAlpha, kBlendEnvBumpNext,
        kBlendSubtract, kBlendRevSubtract, kBlendAlphaTestHigh, kNumBlendFlags
    };
    QCheckBox* fBlendFlags[kNumBlendFlags];

    enum {
        kClampTextureU, kClampTextureV, kNumClampFlags
    };
    QCheckBox* fClampFlags[kNumClampFlags];

    enum {
        kShadeSoftShadow, kShadeNoProjectors, kShadeEnvironMap,
        kShadeVertexShade, kShadeBlack, kShadeSpecular, kShadeNoFog,
        kShadeWhite, kShadeSpecularAlpha, kShadeSpecularColor,
        kShadeSpecularHighlight, kShadeVertColShade, kShadeInherit,
        kShadeIgnoreVtxIllum, kShadeEmissive, kShadeReallyNoFog, kNumShadeFlags
    };
    QCheckBox* fShadeFlags[kNumShadeFlags];

    enum {
        kZIncLayer, kZClearZ, kZNoZRead, kZNoZWrite, kZLODBias, kNumZFlags
    };
    QCheckBox* fZFlags[kNumZFlags];

    enum {
        kMiscWireFrame, kMiscDrawMeshOutlines, kMiscTwoSided, kMiscDrawAsSplats,
        kMiscAdjustPlane, kMiscAdjustCylinder, kMiscAdjustSphere,
        kMiscTroubledLoner, kMiscBindSkip, kMiscBindMask, kMiscBindNext,
        kMiscLightMap, kMiscUseReflectionXform, kMiscPerspProjection,
        kMiscOrthoProjection, kMiscRestartPassHere, kMiscBumpLayer, kMiscBumpDu,
        kMiscBumpDv, kMiscBumpDw, kMiscNoShadowAlpha, kMiscUseRefractionXform,
        kMiscCam2Screen, kNumMiscFlags
    };
    QCheckBox* fMiscFlags[kNumMiscFlags];

public:
    QLayer(plCreatable* pCre, QWidget* parent = NULL);
    virtual void saveDamage();

protected:
    void updateFlags(const hsGMatState& state);
    void updateState(hsGMatState& state) const;

protected slots:
    void setBaseLayer();
    void setTexture();
    void setVShader();
    void setPShader();
    void unsetBaseLayer();
    void unsetTexture();
    void unsetVShader();
    void unsetPShader();
};

#endif
