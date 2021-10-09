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

#include "QLayer.h"

#include <QLabel>
#include <QGroupBox>
#include <QTabWidget>
#include <QGridLayout>
#include <QSpacerItem>
#include "QKeyDialog.h"
#include "Main.h"

QLayer::QLayer(plCreatable* pCre, QWidget* parent)
    : QCreatable(pCre, kLayer, parent)
{
    plLayer* lay = plLayer::Convert(fCreatable);

    fSynchObjLink = new QCreatableLink(this, false);
    fSynchObjLink->setCreatable(lay, tr("Synch Flags"));
    fSynchObjLink->setForceType(kSynchedObject);

    fBaseLayer = new QCreatableLink(this);
    fBaseLayer->setKey(lay->getUnderLay());

    fTexture = new QCreatableLink(this);
    fTexture->setKey(lay->getTexture());

    fVShader = new QCreatableLink(this);
    fVShader->setKey(lay->getVertexShader());

    fPShader = new QCreatableLink(this);
    fPShader->setKey(lay->getPixelShader());

    QTabWidget* flagTab = new QTabWidget(this);
    QWidget* blendWidget = new QWidget(flagTab);
    QGridLayout* blendLayout = new QGridLayout(blendWidget);
    blendLayout->setVerticalSpacing(0);
    blendLayout->setHorizontalSpacing(8);
    fBlendFlags[kBlendTest] = new QBitmaskCheckBox(hsGMatState::kBlendTest,
            tr("Test"), blendWidget);
    fBlendFlags[kBlendAlpha] = new QBitmaskCheckBox(hsGMatState::kBlendAlpha,
            tr("Alpha"), blendWidget);
    fBlendFlags[kBlendMult] = new QBitmaskCheckBox(hsGMatState::kBlendMult,
            tr("Mult"), blendWidget);
    fBlendFlags[kBlendAdd] = new QBitmaskCheckBox(hsGMatState::kBlendAdd,
            tr("Add"), blendWidget);
    fBlendFlags[kBlendAddColorTimesAlpha] = new QBitmaskCheckBox(hsGMatState::kBlendAddColorTimesAlpha,
            tr("Add Color \xc3\x97 Alpha"), blendWidget);
    fBlendFlags[kBlendAntiAlias] = new QBitmaskCheckBox(hsGMatState::kBlendAntiAlias,
            tr("Anti-Alias"), blendWidget);
    fBlendFlags[kBlendDetail] = new QBitmaskCheckBox(hsGMatState::kBlendDetail,
            tr("Detail"), blendWidget);
    fBlendFlags[kBlendNoColor] = new QBitmaskCheckBox(hsGMatState::kBlendNoColor,
            tr("No Color"), blendWidget);
    fBlendFlags[kBlendMADD] = new QBitmaskCheckBox(hsGMatState::kBlendMADD,
            tr("MADD"), blendWidget);
    fBlendFlags[kBlendDot3] = new QBitmaskCheckBox(hsGMatState::kBlendDot3,
            tr("Dot3"), blendWidget);
    fBlendFlags[kBlendAddSigned] = new QBitmaskCheckBox(hsGMatState::kBlendAddSigned,
            tr("Add Signed"), blendWidget);
    fBlendFlags[kBlendAddSigned2X] = new QBitmaskCheckBox(hsGMatState::kBlendAddSigned2X,
            tr("Add Signed 2X"), blendWidget);
    fBlendFlags[kBlendInvertAlpha] = new QBitmaskCheckBox(hsGMatState::kBlendInvertAlpha,
            tr("Invert Alpha"), blendWidget);
    fBlendFlags[kBlendInvertColor] = new QBitmaskCheckBox(hsGMatState::kBlendInvertColor,
            tr("Invert Color"), blendWidget);
    fBlendFlags[kBlendAlphaMult] = new QBitmaskCheckBox(hsGMatState::kBlendAlphaMult,
            tr("Alpha Mult"), blendWidget);
    fBlendFlags[kBlendAlphaAdd] = new QBitmaskCheckBox(hsGMatState::kBlendAlphaAdd,
            tr("Alpha Add"), blendWidget);
    fBlendFlags[kBlendNoVtxAlpha] = new QBitmaskCheckBox(hsGMatState::kBlendNoVtxAlpha,
            tr("No Vertex Alpha"), blendWidget);
    fBlendFlags[kBlendNoTexColor] = new QBitmaskCheckBox(hsGMatState::kBlendNoTexColor,
            tr("No Texture Color"), blendWidget);
    fBlendFlags[kBlendNoTexAlpha] = new QBitmaskCheckBox(hsGMatState::kBlendNoTexAlpha,
            tr("No Texture Alpha"), blendWidget);
    fBlendFlags[kBlendInvertVtxAlpha] = new QBitmaskCheckBox(hsGMatState::kBlendInvertVtxAlpha,
            tr("Invert Vertex Alpha"), blendWidget);
    fBlendFlags[kBlendAlphaAlways] = new QBitmaskCheckBox(hsGMatState::kBlendAlphaAlways,
            tr("Alpha Always"), blendWidget);
    fBlendFlags[kBlendInvertFinalColor] = new QBitmaskCheckBox(hsGMatState::kBlendInvertFinalColor,
            tr("Invert Final Color"), blendWidget);
    fBlendFlags[kBlendInvertFinalAlpha] = new QBitmaskCheckBox(hsGMatState::kBlendInvertFinalAlpha,
            tr("Invert Final Alpha"), blendWidget);
    fBlendFlags[kBlendEnvBumpNext] = new QBitmaskCheckBox(hsGMatState::kBlendEnvBumpNext,
            tr("Env Bump Next"), blendWidget);
    fBlendFlags[kBlendSubtract] = new QBitmaskCheckBox(hsGMatState::kBlendSubtract,
            tr("Subtract"), blendWidget);
    fBlendFlags[kBlendRevSubtract] = new QBitmaskCheckBox(hsGMatState::kBlendRevSubtract,
            tr("Rev Subtract"), blendWidget);
    fBlendFlags[kBlendAlphaTestHigh] = new QBitmaskCheckBox(hsGMatState::kBlendAlphaTestHigh,
            tr("Alpha Test High"), blendWidget);
    blendLayout->addWidget(fBlendFlags[kBlendTest], 0, 0);
    blendLayout->addWidget(fBlendFlags[kBlendAddColorTimesAlpha], 0, 1);
    blendLayout->addWidget(fBlendFlags[kBlendInvertAlpha], 0, 2);
    blendLayout->addWidget(fBlendFlags[kBlendNoVtxAlpha], 0, 3);
    blendLayout->addWidget(fBlendFlags[kBlendAlpha], 1, 0);
    blendLayout->addWidget(fBlendFlags[kBlendAntiAlias], 1, 1);
    blendLayout->addWidget(fBlendFlags[kBlendInvertColor], 1, 2);
    blendLayout->addWidget(fBlendFlags[kBlendNoTexColor], 1, 3);
    blendLayout->addWidget(fBlendFlags[kBlendMult], 2, 0);
    blendLayout->addWidget(fBlendFlags[kBlendNoColor], 2, 1);
    blendLayout->addWidget(fBlendFlags[kBlendAlphaMult], 2, 2);
    blendLayout->addWidget(fBlendFlags[kBlendNoTexAlpha], 2, 3);
    blendLayout->addWidget(fBlendFlags[kBlendAdd], 3, 0);
    blendLayout->addWidget(fBlendFlags[kBlendMADD], 3, 1);
    blendLayout->addWidget(fBlendFlags[kBlendAlphaAdd], 3, 2);
    blendLayout->addWidget(fBlendFlags[kBlendInvertVtxAlpha], 3, 3);
    blendLayout->addWidget(fBlendFlags[kBlendDetail], 4, 0);
    blendLayout->addWidget(fBlendFlags[kBlendAddSigned], 4, 1);
    blendLayout->addWidget(fBlendFlags[kBlendSubtract], 4, 2);
    blendLayout->addWidget(fBlendFlags[kBlendInvertFinalColor], 4, 3);
    blendLayout->addWidget(fBlendFlags[kBlendDot3], 5, 0);
    blendLayout->addWidget(fBlendFlags[kBlendAddSigned2X], 5, 1);
    blendLayout->addWidget(fBlendFlags[kBlendRevSubtract], 5, 2);
    blendLayout->addWidget(fBlendFlags[kBlendInvertFinalAlpha], 5, 3);
    blendLayout->addWidget(fBlendFlags[kBlendAlphaTestHigh], 6, 1);
    blendLayout->addWidget(fBlendFlags[kBlendAlphaAlways], 6, 2);
    blendLayout->addWidget(fBlendFlags[kBlendEnvBumpNext], 6, 3);
    blendLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding), 7, 0, 1, 4);

    const hsGMatState &state = lay->getState();
    for (auto cb : fBlendFlags) {
        cb->setFrom(state.fBlendFlags);
        connect(cb, &QBitmaskCheckBox::setBits, this, [this](unsigned int mask) {
            plLayer* lay = plLayer::Convert(fCreatable);
            lay->getState().fBlendFlags |= mask;
        });
        connect(cb, &QBitmaskCheckBox::unsetBits, this, [this](unsigned int mask) {
            plLayer* lay = plLayer::Convert(fCreatable);
            lay->getState().fBlendFlags &= ~mask;
        });
    }

    QWidget* clampZWidget = new QWidget(flagTab);
    QGridLayout* clampZLayout = new QGridLayout(clampZWidget);
    clampZLayout->setVerticalSpacing(0);
    clampZLayout->setHorizontalSpacing(8);
    fClampFlags[kClampTextureU] = new QBitmaskCheckBox(hsGMatState::kClampTextureU,
            tr("Clamp Texture U"), clampZWidget);
    fClampFlags[kClampTextureV] = new QBitmaskCheckBox(hsGMatState::kClampTextureV,
            tr("Clamp Texture V"), clampZWidget);
    fZFlags[kZIncLayer] = new QBitmaskCheckBox(hsGMatState::kZIncLayer,
            tr("Increment Z Layer"), clampZWidget);
    fZFlags[kZClearZ] = new QBitmaskCheckBox(hsGMatState::kZClearZ,
            tr("Clear Z"), clampZWidget);
    fZFlags[kZNoZRead] = new QBitmaskCheckBox(hsGMatState::kZNoZRead,
            tr("No Z Read"), clampZWidget);
    fZFlags[kZNoZWrite] = new QBitmaskCheckBox(hsGMatState::kZNoZWrite,
            tr("No Z Write"), clampZWidget);
    fZFlags[kZLODBias] = new QBitmaskCheckBox(hsGMatState::kZLODBias,
            tr("LOD Bias"), clampZWidget);
    clampZLayout->addWidget(fClampFlags[kClampTextureU], 0, 0);
    clampZLayout->addWidget(fClampFlags[kClampTextureV], 0, 1);
    clampZLayout->addItem(new QSpacerItem(0, 16), 1, 0, 1, 2);
    clampZLayout->addWidget(fZFlags[kZIncLayer], 2, 0);
    clampZLayout->addWidget(fZFlags[kZNoZRead], 2, 1);
    clampZLayout->addWidget(fZFlags[kZClearZ], 3, 0);
    clampZLayout->addWidget(fZFlags[kZNoZWrite], 3, 1);
    clampZLayout->addWidget(fZFlags[kZLODBias], 4, 0);
    clampZLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding), 5, 0, 1, 2);

    for (auto cb : fClampFlags) {
        cb->setFrom(state.fClampFlags);
        connect(cb, &QBitmaskCheckBox::setBits, this, [this](unsigned int mask) {
            plLayer* lay = plLayer::Convert(fCreatable);
            lay->getState().fClampFlags |= mask;
        });
        connect(cb, &QBitmaskCheckBox::unsetBits, this, [this](unsigned int mask) {
            plLayer* lay = plLayer::Convert(fCreatable);
            lay->getState().fClampFlags &= ~mask;
        });
    }

    for (auto cb : fZFlags) {
        cb->setFrom(state.fZFlags);
        connect(cb, &QBitmaskCheckBox::setBits, this, [this](unsigned int mask) {
            plLayer* lay = plLayer::Convert(fCreatable);
            lay->getState().fZFlags |= mask;
        });
        connect(cb, &QBitmaskCheckBox::unsetBits, this, [this](unsigned int mask) {
            plLayer* lay = plLayer::Convert(fCreatable);
            lay->getState().fZFlags &= ~mask;
        });
    }

    QWidget* shadeWidget = new QWidget(flagTab);
    QGridLayout* shadeLayout = new QGridLayout(shadeWidget);
    shadeLayout->setVerticalSpacing(0);
    shadeLayout->setHorizontalSpacing(8);
    fShadeFlags[kShadeSoftShadow] = new QBitmaskCheckBox(hsGMatState::kShadeSoftShadow,
            tr("Soft Shadow"), shadeWidget);
    fShadeFlags[kShadeNoProjectors] = new QBitmaskCheckBox(hsGMatState::kShadeNoProjectors,
            tr("No Projectors"), shadeWidget);
    fShadeFlags[kShadeEnvironMap] = new QBitmaskCheckBox(hsGMatState::kShadeEnvironMap,
            tr("EnvironMap"), shadeWidget);
    fShadeFlags[kShadeVertexShade] = new QBitmaskCheckBox(hsGMatState::kShadeVertexShade,
            tr("Vertex Shade"), shadeWidget);
    fShadeFlags[kShadeBlack] = new QBitmaskCheckBox(hsGMatState::kShadeBlack,
            tr("Black"), shadeWidget);
    fShadeFlags[kShadeSpecular] = new QBitmaskCheckBox(hsGMatState::kShadeSpecular,
            tr("Specular"), shadeWidget);
    fShadeFlags[kShadeNoFog] = new QBitmaskCheckBox(hsGMatState::kShadeNoFog,
            tr("No Fog"), shadeWidget);
    fShadeFlags[kShadeWhite] = new QBitmaskCheckBox(hsGMatState::kShadeWhite,
            tr("White"), shadeWidget);
    fShadeFlags[kShadeSpecularAlpha] = new QBitmaskCheckBox(hsGMatState::kShadeSpecularAlpha,
            tr("Specular Alpha"), shadeWidget);
    fShadeFlags[kShadeSpecularColor] = new QBitmaskCheckBox(hsGMatState::kShadeSpecularColor,
            tr("Specular Color"), shadeWidget);
    fShadeFlags[kShadeSpecularHighlight] = new QBitmaskCheckBox(hsGMatState::kShadeSpecularHighlight,
            tr("Specular Highlight"), shadeWidget);
    fShadeFlags[kShadeVertColShade] = new QBitmaskCheckBox(hsGMatState::kShadeVertColShade,
            tr("Vtx Color Shade"), shadeWidget);
    fShadeFlags[kShadeInherit] = new QBitmaskCheckBox(hsGMatState::kShadeInherit,
            tr("Inherit"), shadeWidget);
    fShadeFlags[kShadeIgnoreVtxIllum] = new QBitmaskCheckBox(hsGMatState::kShadeIgnoreVtxIllum,
            tr("Ignore Vtx Illum"), shadeWidget);
    fShadeFlags[kShadeEmissive] = new QBitmaskCheckBox(hsGMatState::kShadeEmissive,
            tr("Emissive"), shadeWidget);
    fShadeFlags[kShadeReallyNoFog] = new QBitmaskCheckBox(hsGMatState::kShadeReallyNoFog,
            tr("Really No Fog"), shadeWidget);
    shadeLayout->addWidget(fShadeFlags[kShadeSoftShadow], 0, 0);
    shadeLayout->addWidget(fShadeFlags[kShadeBlack], 0, 1);
    shadeLayout->addWidget(fShadeFlags[kShadeSpecularAlpha], 0, 2);
    shadeLayout->addWidget(fShadeFlags[kShadeInherit], 0, 3);
    shadeLayout->addWidget(fShadeFlags[kShadeNoProjectors], 1, 0);
    shadeLayout->addWidget(fShadeFlags[kShadeSpecular], 1, 1);
    shadeLayout->addWidget(fShadeFlags[kShadeSpecularColor], 1, 2);
    shadeLayout->addWidget(fShadeFlags[kShadeIgnoreVtxIllum], 1, 3);
    shadeLayout->addWidget(fShadeFlags[kShadeEnvironMap], 2, 0);
    shadeLayout->addWidget(fShadeFlags[kShadeNoFog], 2, 1);
    shadeLayout->addWidget(fShadeFlags[kShadeSpecularHighlight], 2, 2);
    shadeLayout->addWidget(fShadeFlags[kShadeEmissive], 2, 3);
    shadeLayout->addWidget(fShadeFlags[kShadeVertexShade], 3, 0);
    shadeLayout->addWidget(fShadeFlags[kShadeWhite], 3, 1);
    shadeLayout->addWidget(fShadeFlags[kShadeVertColShade], 3, 2);
    shadeLayout->addWidget(fShadeFlags[kShadeReallyNoFog], 3, 3);
    shadeLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding), 4, 0, 1, 4);

    for (auto cb : fShadeFlags) {
        cb->setFrom(state.fShadeFlags);
        connect(cb, &QBitmaskCheckBox::setBits, this, [this](unsigned int mask) {
            plLayer* lay = plLayer::Convert(fCreatable);
            lay->getState().fShadeFlags |= mask;
        });
        connect(cb, &QBitmaskCheckBox::unsetBits, this, [this](unsigned int mask) {
            plLayer* lay = plLayer::Convert(fCreatable);
            lay->getState().fShadeFlags &= ~mask;
        });
    }

    QWidget* miscWidget = new QWidget(flagTab);
    QGridLayout* miscLayout = new QGridLayout(miscWidget);
    miscLayout->setVerticalSpacing(0);
    miscLayout->setHorizontalSpacing(8);
    fMiscFlags[kMiscWireFrame] = new QBitmaskCheckBox(hsGMatState::kMiscWireFrame,
            tr("Wire Frame"), miscWidget);
    fMiscFlags[kMiscDrawMeshOutlines] = new QBitmaskCheckBox(hsGMatState::kMiscDrawMeshOutlines,
            tr("Mesh Outlines"), miscWidget);
    fMiscFlags[kMiscTwoSided] = new QBitmaskCheckBox(hsGMatState::kMiscTwoSided,
            tr("Two-Sided"), miscWidget);
    fMiscFlags[kMiscDrawAsSplats] = new QBitmaskCheckBox(hsGMatState::kMiscDrawAsSplats,
            tr("Draw as Splats"), miscWidget);
    fMiscFlags[kMiscAdjustPlane] = new QBitmaskCheckBox(hsGMatState::kMiscAdjustPlane,
            tr("Adjust Plane"), miscWidget);
    fMiscFlags[kMiscAdjustCylinder] = new QBitmaskCheckBox(hsGMatState::kMiscAdjustCylinder,
            tr("Adjust Cylinder"), miscWidget);
    fMiscFlags[kMiscAdjustSphere] = new QBitmaskCheckBox(hsGMatState::kMiscAdjustSphere,
            tr("Adjust Sphere"), miscWidget);
    fMiscFlags[kMiscTroubledLoner] = new QBitmaskCheckBox(hsGMatState::kMiscTroubledLoner,
            tr("Troubled Loner"), miscWidget);
    fMiscFlags[kMiscBindSkip] = new QBitmaskCheckBox(hsGMatState::kMiscBindSkip,
            tr("Bind Skip"), miscWidget);
    fMiscFlags[kMiscBindMask] = new QBitmaskCheckBox(hsGMatState::kMiscBindMask,
            tr("Bind Mask"), miscWidget);
    fMiscFlags[kMiscBindNext] = new QBitmaskCheckBox(hsGMatState::kMiscBindNext,
            tr("Bind Next"), miscWidget);
    fMiscFlags[kMiscLightMap] = new QBitmaskCheckBox(hsGMatState::kMiscLightMap,
            tr("Light Map"), miscWidget);
    fMiscFlags[kMiscUseReflectionXform] = new QBitmaskCheckBox(hsGMatState::kMiscUseReflectionXform,
            tr("Use Reflec Xform"), miscWidget);
    fMiscFlags[kMiscPerspProjection] = new QBitmaskCheckBox(hsGMatState::kMiscPerspProjection,
            tr("Persp Projection"), miscWidget);
    fMiscFlags[kMiscOrthoProjection] = new QBitmaskCheckBox(hsGMatState::kMiscOrthoProjection,
            tr("Ortho Projection"), miscWidget);
    fMiscFlags[kMiscRestartPassHere] = new QBitmaskCheckBox(hsGMatState::kMiscRestartPassHere,
            tr("Restart Pass Here"), miscWidget);
    fMiscFlags[kMiscBumpLayer] = new QBitmaskCheckBox(hsGMatState::kMiscBumpLayer,
            tr("Bump Layer"), miscWidget);
    fMiscFlags[kMiscBumpDu] = new QBitmaskCheckBox(hsGMatState::kMiscBumpDu,
            tr("Bump DU"), miscWidget);
    fMiscFlags[kMiscBumpDv] = new QBitmaskCheckBox(hsGMatState::kMiscBumpDv,
            tr("Bump DV"), miscWidget);
    fMiscFlags[kMiscBumpDw] = new QBitmaskCheckBox(hsGMatState::kMiscBumpDw,
            tr("Bump DW"), miscWidget);
    fMiscFlags[kMiscNoShadowAlpha] = new QBitmaskCheckBox(hsGMatState::kMiscNoShadowAlpha,
            tr("No Shadow Alpha"), miscWidget);
    fMiscFlags[kMiscUseRefractionXform] = new QBitmaskCheckBox(hsGMatState::kMiscUseRefractionXform,
            tr("Use Refrac Xform"), miscWidget);
    fMiscFlags[kMiscCam2Screen] = new QBitmaskCheckBox(hsGMatState::kMiscCam2Screen,
            tr("Cam 2 Screen"), miscWidget);
    miscLayout->addWidget(fMiscFlags[kMiscWireFrame], 0, 0);
    miscLayout->addWidget(fMiscFlags[kMiscDrawAsSplats], 0, 1);
    miscLayout->addWidget(fMiscFlags[kMiscBumpLayer], 0, 2);
    miscLayout->addWidget(fMiscFlags[kMiscPerspProjection], 0, 3);
    miscLayout->addWidget(fMiscFlags[kMiscTwoSided], 1, 0);
    miscLayout->addWidget(fMiscFlags[kMiscAdjustPlane], 1, 1);
    miscLayout->addWidget(fMiscFlags[kMiscBumpDu], 1, 2);
    miscLayout->addWidget(fMiscFlags[kMiscOrthoProjection], 1, 3);
    miscLayout->addWidget(fMiscFlags[kMiscBindSkip], 2, 0);
    miscLayout->addWidget(fMiscFlags[kMiscAdjustCylinder], 2, 1);
    miscLayout->addWidget(fMiscFlags[kMiscBumpDv], 2, 2);
    miscLayout->addWidget(fMiscFlags[kMiscUseReflectionXform], 2, 3);
    miscLayout->addWidget(fMiscFlags[kMiscBindMask], 3, 0);
    miscLayout->addWidget(fMiscFlags[kMiscAdjustSphere], 3, 1);
    miscLayout->addWidget(fMiscFlags[kMiscBumpDw], 3, 2);
    miscLayout->addWidget(fMiscFlags[kMiscUseRefractionXform], 3, 3);
    miscLayout->addWidget(fMiscFlags[kMiscBindNext], 4, 0);
    miscLayout->addWidget(fMiscFlags[kMiscDrawMeshOutlines], 4, 1);
    miscLayout->addWidget(fMiscFlags[kMiscCam2Screen], 4, 2);
    miscLayout->addWidget(fMiscFlags[kMiscRestartPassHere], 4, 3);
    miscLayout->addWidget(fMiscFlags[kMiscLightMap], 5, 0);
    miscLayout->addWidget(fMiscFlags[kMiscTroubledLoner], 5, 1);
    miscLayout->addWidget(fMiscFlags[kMiscNoShadowAlpha], 5, 2, 1, 2);
    miscLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding), 6, 0, 1, 4);

    for (auto cb : fMiscFlags) {
        cb->setFrom(state.fMiscFlags);
        connect(cb, &QBitmaskCheckBox::setBits, this, [this](unsigned int mask) {
            plLayer* lay = plLayer::Convert(fCreatable);
            lay->getState().fMiscFlags |= mask;
        });
        connect(cb, &QBitmaskCheckBox::unsetBits, this, [this](unsigned int mask) {
            plLayer* lay = plLayer::Convert(fCreatable);
            lay->getState().fMiscFlags &= ~mask;
        });
    }

    flagTab->addTab(blendWidget, tr("Blend Flags"));
    flagTab->addTab(clampZWidget, tr("Clamp/Z Flags"));
    flagTab->addTab(shadeWidget, tr("Shade Flags"));
    flagTab->addTab(miscWidget, tr("Misc Flags"));

    QTabWidget* xformTab = new QTabWidget(this);
    fUvwTransform = new QMatrix44(xformTab);
    fBumpEnvTransform = new QMatrix44(xformTab);
    fUvwTransform->setValue(lay->getTransform());
    fBumpEnvTransform->setValue(lay->getBumpEnvTransform());
    xformTab->addTab(fUvwTransform, tr("UVW Transform"));
    xformTab->addTab(fBumpEnvTransform, tr("Bump Env Transform"));

    QGroupBox* grpColors = new QGroupBox(tr("Colors"), this);
    QGridLayout* layColors = new QGridLayout(grpColors);
    layColors->setVerticalSpacing(8);
    layColors->setHorizontalSpacing(8);
    fPreshade = new QColorEdit(true, grpColors);
    fRuntime = new QColorEdit(true, grpColors);
    fAmbient = new QColorEdit(true, grpColors);
    fSpecular = new QColorEdit(true, grpColors);
    fPreshade->setColor(lay->getPreshade());
    fRuntime->setColor(lay->getRuntime());
    fAmbient->setColor(lay->getAmbient());
    fSpecular->setColor(lay->getSpecular());
    layColors->addWidget(new QLabel(tr("Preshade:"), grpColors), 0, 0);
    layColors->addWidget(fPreshade, 0, 1);
    layColors->addItem(new QSpacerItem(8, 0, QSizePolicy::MinimumExpanding), 0, 2);
    layColors->addWidget(new QLabel(tr("Runtime:"), grpColors), 0, 3);
    layColors->addWidget(fRuntime, 0, 4);
    layColors->addWidget(new QLabel(tr("Ambient:"), grpColors), 1, 0);
    layColors->addWidget(fAmbient, 1, 1);
    layColors->addItem(new QSpacerItem(8, 0, QSizePolicy::MinimumExpanding), 1, 2);
    layColors->addWidget(new QLabel(tr("Specular:"), grpColors), 1, 3);
    layColors->addWidget(fSpecular, 1, 4);

    fUvwSrc = new QSpinBox(this);
    fOpacity = new QDoubleSpinBox(this);
    fLodBias = new QFloatEdit(this);
    fSpecPower = new QFloatEdit(this);
    fUvwSrc->setRange(0, 0x7FFFFFFF);
    fOpacity->setDecimals(3);
    fOpacity->setRange(0.0, 1.0);
    fOpacity->setSingleStep(0.005);
    fUvwSrc->setValue(lay->getUVWSrc());
    fOpacity->setValue(lay->getOpacity());
    fLodBias->setValue(lay->getLODBias());
    fSpecPower->setValue(lay->getSpecularPower());

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(fSynchObjLink, 0, 0, 1, 5);
    layout->addWidget(new QLabel(tr("Base Layer:"), this), 1, 0, 1, 1);
    layout->addWidget(fBaseLayer, 1, 1, 1, 4);
    layout->addWidget(flagTab, 2, 0, 1, 5);
    layout->addWidget(xformTab, 3, 0, 1, 5);
    layout->addWidget(grpColors, 4, 0, 1, 5);
    layout->addWidget(new QLabel(tr("UVW Source:"), this), 5, 0, 1, 1);
    layout->addWidget(fUvwSrc, 5, 1, 1, 1);
    layout->addItem(new QSpacerItem(8, 0, QSizePolicy::MinimumExpanding), 5, 2);
    layout->addWidget(new QLabel(tr("LOD Bias:"), this), 5, 3, 1, 1);
    layout->addWidget(fLodBias, 5, 4, 1, 1);
    layout->addWidget(new QLabel(tr("Opacity:"), this), 6, 0, 1, 1);
    layout->addWidget(fOpacity, 6, 1, 1, 1);
    layout->addItem(new QSpacerItem(8, 0, QSizePolicy::MinimumExpanding), 6, 2);
    layout->addWidget(new QLabel(tr("Specular Power:"), this), 6, 3, 1, 1);
    layout->addWidget(fSpecPower, 6, 4, 1, 1);
    layout->addWidget(new QLabel(tr("Texture:"), this), 7, 0, 1, 1);
    layout->addWidget(fTexture, 7, 1, 1, 4);
    layout->addWidget(new QLabel(tr("Vertex Shader:"), this), 8, 0, 1, 1);
    layout->addWidget(fVShader, 8, 1, 1, 4);
    layout->addWidget(new QLabel(tr("Pixel Shader:"), this), 9, 0, 1, 1);
    layout->addWidget(fPShader, 9, 1, 1, 4);

    connect(fBaseLayer, &QCreatableLink::addObject, this, &QLayer::setBaseLayer);
    connect(fBaseLayer, &QCreatableLink::delObject, this, &QLayer::unsetBaseLayer);
    connect(fTexture, &QCreatableLink::addObject, this, &QLayer::setTexture);
    connect(fTexture, &QCreatableLink::delObject, this, &QLayer::unsetTexture);
    connect(fVShader, &QCreatableLink::addObject, this, &QLayer::setVShader);
    connect(fVShader, &QCreatableLink::delObject, this, &QLayer::unsetVShader);
    connect(fPShader, &QCreatableLink::addObject, this, &QLayer::setPShader);
    connect(fPShader, &QCreatableLink::delObject, this, &QLayer::unsetPShader);
}

void QLayer::saveDamage()
{
    plLayer* lay = plLayer::Convert(fCreatable);

    lay->setTransform(fUvwTransform->value());
    lay->setBumpEnvTransform(fBumpEnvTransform->value());
    lay->setPreshade(fPreshade->color());
    lay->setRuntime(fRuntime->color());
    lay->setAmbient(fAmbient->color());
    lay->setSpecular(fSpecular->color());
    lay->setUVWSrc(fUvwSrc->value());
    lay->setOpacity(fOpacity->value());
    lay->setLODBias(fLodBias->value());
    lay->setSpecularPower(fSpecPower->value());
}

void QLayer::setBaseLayer()
{
    plLayer* lay = plLayer::Convert(fCreatable);
    QFindKeyDialog dlg(this);
    if (lay->getUnderLay().Exists())
        dlg.init(PrpShopMain::ResManager(), lay->getUnderLay());
    else
        dlg.init(PrpShopMain::ResManager(), lay->getKey()->getLocation(), kLayer);
    if (dlg.exec() == QDialog::Accepted) {
        lay->setUnderLay(dlg.selection());
        fBaseLayer->setKey(lay->getUnderLay());
    }
}

void QLayer::setTexture()
{
    plLayer* lay = plLayer::Convert(fCreatable);
    QFindKeyDialog dlg(this);
    if (lay->getTexture().Exists())
        dlg.init(PrpShopMain::ResManager(), lay->getTexture());
    else
        dlg.init(PrpShopMain::ResManager(), lay->getKey()->getLocation(), kMipmap);
    if (dlg.exec() == QDialog::Accepted) {
        lay->setTexture(dlg.selection());
        fTexture->setKey(lay->getTexture());
    }
}

void QLayer::setVShader()
{
    plLayer* lay = plLayer::Convert(fCreatable);
    QFindKeyDialog dlg(this);
    if (lay->getVertexShader().Exists())
        dlg.init(PrpShopMain::ResManager(), lay->getVertexShader());
    else
        dlg.init(PrpShopMain::ResManager(), lay->getKey()->getLocation(), kShader);
    if (dlg.exec() == QDialog::Accepted) {
        lay->setVertexShader(dlg.selection());
        fVShader->setKey(lay->getVertexShader());
    }
}

void QLayer::setPShader()
{
    plLayer* lay = plLayer::Convert(fCreatable);
    QFindKeyDialog dlg(this);
    if (lay->getPixelShader().Exists())
        dlg.init(PrpShopMain::ResManager(), lay->getPixelShader());
    else
        dlg.init(PrpShopMain::ResManager(), lay->getKey()->getLocation(), kShader);
    if (dlg.exec() == QDialog::Accepted) {
        lay->setPixelShader(dlg.selection());
        fPShader->setKey(lay->getPixelShader());
    }
}

void QLayer::unsetBaseLayer()
{
    plLayer* lay = plLayer::Convert(fCreatable);
    lay->setUnderLay(plKey());
    fBaseLayer->setCreatable(NULL, "(None)");
}

void QLayer::unsetTexture()
{
    plLayer* lay = plLayer::Convert(fCreatable);
    lay->setTexture(plKey());
    fTexture->setCreatable(NULL, "(None)");
}

void QLayer::unsetVShader()
{
    plLayer* lay = plLayer::Convert(fCreatable);
    lay->setVertexShader(plKey());
    fVShader->setCreatable(NULL, "(None)");
}

void QLayer::unsetPShader()
{
    plLayer* lay = plLayer::Convert(fCreatable);
    lay->setPixelShader(plKey());
    fPShader->setCreatable(NULL, "(None)");
}
