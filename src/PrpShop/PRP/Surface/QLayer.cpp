#include "QLayer.h"

#include <QLabel>
#include <QGroupBox>
#include <QTabWidget>
#include <QGridLayout>
#include <QSpacerItem>
#include <QDoubleValidator>

QLayer::QLayer(plCreatable* pCre, QWidget* parent)
      : QCreatable(pCre, kLayer, parent)
{
    plLayer* lay = (plLayer*)fCreatable;

    fSynchObjLink = new QCreatableLink(this, false);
    fSynchObjLink->setText(tr("Synch Flags"));
    fSynchObjLink->setCreatable(lay);
    fSynchObjLink->setForceType(kSynchedObject);

    fBaseLayer = new QCreatableLink(this);
    fBaseLayer->setKey(lay->getUnderLay());
    fBaseLayer->setText(lay->getUnderLay().Exists()
                        ? lay->getUnderLay()->getName().cstr()
                        : "(None)");

    fTexture = new QCreatableLink(this);
    fTexture->setKey(lay->getTexture());
    fTexture->setText(lay->getTexture().Exists()
                      ? lay->getTexture()->getName().cstr()
                      : "(None)");

    fVShader = new QCreatableLink(this);
    fVShader->setKey(lay->getVertexShader());
    fVShader->setText(lay->getVertexShader().Exists()
                      ? lay->getVertexShader()->getName().cstr()
                      : "(None)");

    fPShader = new QCreatableLink(this);
    fPShader->setKey(lay->getPixelShader());
    fPShader->setText(lay->getPixelShader().Exists()
                      ? lay->getPixelShader()->getName().cstr()
                      : "(None)");

    QTabWidget* flagTab = new QTabWidget(this);
    QWidget* blendWidget = new QWidget(flagTab);
    QGridLayout* blendLayout = new QGridLayout(blendWidget);
    blendLayout->setVerticalSpacing(0);
    blendLayout->setHorizontalSpacing(8);
    fBlendFlags[kBlendTest] = new QCheckBox(tr("Test"), blendWidget);
    fBlendFlags[kBlendAlpha] = new QCheckBox(tr("Alpha"), blendWidget);
    fBlendFlags[kBlendMult] = new QCheckBox(tr("Mult"), blendWidget);
    fBlendFlags[kBlendAdd] = new QCheckBox(tr("Add"), blendWidget);
    fBlendFlags[kBlendAddColorTimesAlpha] = new QCheckBox(tr("Add Color × Alpha"), blendWidget);
    fBlendFlags[kBlendAntiAlias] = new QCheckBox(tr("Anti-Alias"), blendWidget);
    fBlendFlags[kBlendDetail] = new QCheckBox(tr("Detail"), blendWidget);
    fBlendFlags[kBlendNoColor] = new QCheckBox(tr("No Color"), blendWidget);
    fBlendFlags[kBlendMADD] = new QCheckBox(tr("MADD"), blendWidget);
    fBlendFlags[kBlendDot3] = new QCheckBox(tr("Dot3"), blendWidget);
    fBlendFlags[kBlendAddSigned] = new QCheckBox(tr("Add Signed"), blendWidget);
    fBlendFlags[kBlendAddSigned2X] = new QCheckBox(tr("Add Signed 2X"), blendWidget);
    fBlendFlags[kBlendInvertAlpha] = new QCheckBox(tr("Invert Alpha"), blendWidget);
    fBlendFlags[kBlendInvertColor] = new QCheckBox(tr("Invert Color"), blendWidget);
    fBlendFlags[kBlendAlphaMult] = new QCheckBox(tr("Alpha Mult"), blendWidget);
    fBlendFlags[kBlendAlphaAdd] = new QCheckBox(tr("Alpha Add"), blendWidget);
    fBlendFlags[kBlendNoVtxAlpha] = new QCheckBox(tr("No Vertex Alpha"), blendWidget);
    fBlendFlags[kBlendNoTexColor] = new QCheckBox(tr("No Texture Color"), blendWidget);
    fBlendFlags[kBlendNoTexAlpha] = new QCheckBox(tr("No Texture Alpha"), blendWidget);
    fBlendFlags[kBlendInvertVtxAlpha] = new QCheckBox(tr("Invert Vertex Alpha"), blendWidget);
    fBlendFlags[kBlendAlphaAlways] = new QCheckBox(tr("Alpha Always"), blendWidget);
    fBlendFlags[kBlendInvertFinalColor] = new QCheckBox(tr("Invert Final Color"), blendWidget);
    fBlendFlags[kBlendInvertFinalAlpha] = new QCheckBox(tr("Invert Final Alpha"), blendWidget);
    fBlendFlags[kBlendEnvBumpNext] = new QCheckBox(tr("Env Bump Next"), blendWidget);
    fBlendFlags[kBlendSubtract] = new QCheckBox(tr("Subtract"), blendWidget);
    fBlendFlags[kBlendRevSubtract] = new QCheckBox(tr("Rev Subtract"), blendWidget);
    fBlendFlags[kBlendAlphaTestHigh] = new QCheckBox(tr("Alpha Test High"), blendWidget);
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

    QWidget* clampZWidget = new QWidget(flagTab);
    QGridLayout* clampZLayout = new QGridLayout(clampZWidget);
    clampZLayout->setVerticalSpacing(0);
    clampZLayout->setHorizontalSpacing(8);
    fClampFlags[kClampTextureU] = new QCheckBox(tr("Clamp Texture U"), clampZWidget);
    fClampFlags[kClampTextureV] = new QCheckBox(tr("Clamp Texture V"), clampZWidget);
    fZFlags[kZIncLayer] = new QCheckBox(tr("Include Layer"), clampZWidget);
    fZFlags[kZClearZ] = new QCheckBox(tr("Clear Z"), clampZWidget);
    fZFlags[kZNoZRead] = new QCheckBox(tr("No Z Read"), clampZWidget);
    fZFlags[kZNoZWrite] = new QCheckBox(tr("No Z Write"), clampZWidget);
    fZFlags[kZLODBias] = new QCheckBox(tr("LOD Bias"), clampZWidget);
    clampZLayout->addWidget(fClampFlags[kClampTextureU], 0, 0);
    clampZLayout->addWidget(fClampFlags[kClampTextureV], 0, 1);
    clampZLayout->addItem(new QSpacerItem(0, 16), 1, 0, 1, 2);
    clampZLayout->addWidget(fZFlags[kZIncLayer], 2, 0);
    clampZLayout->addWidget(fZFlags[kZNoZRead], 2, 1);
    clampZLayout->addWidget(fZFlags[kZClearZ], 3, 0);
    clampZLayout->addWidget(fZFlags[kZNoZWrite], 3, 1);
    clampZLayout->addWidget(fZFlags[kZLODBias], 4, 0);

    QWidget* shadeWidget = new QWidget(flagTab);
    QGridLayout* shadeLayout = new QGridLayout(shadeWidget);
    shadeLayout->setVerticalSpacing(0);
    shadeLayout->setHorizontalSpacing(8);
    fShadeFlags[kShadeSoftShadow] = new QCheckBox(tr("Soft Shadow"), shadeWidget);
    fShadeFlags[kShadeNoProjectors] = new QCheckBox(tr("No Projectors"), shadeWidget);
    fShadeFlags[kShadeEnvironMap] = new QCheckBox(tr("EnvironMap"), shadeWidget);
    fShadeFlags[kShadeVertexShade] = new QCheckBox(tr("Vertex Shade"), shadeWidget);
    fShadeFlags[kShadeBlack] = new QCheckBox(tr("Black"), shadeWidget);
    fShadeFlags[kShadeSpecular] = new QCheckBox(tr("Specular"), shadeWidget);
    fShadeFlags[kShadeNoFog] = new QCheckBox(tr("No Fog"), shadeWidget);
    fShadeFlags[kShadeWhite] = new QCheckBox(tr("White"), shadeWidget);
    fShadeFlags[kShadeSpecularAlpha] = new QCheckBox(tr("Specular Alpha"), shadeWidget);
    fShadeFlags[kShadeSpecularColor] = new QCheckBox(tr("Specular Color"), shadeWidget);
    fShadeFlags[kShadeSpecularHighlight] = new QCheckBox(tr("Specular Highlight"), shadeWidget);
    fShadeFlags[kShadeVertColShade] = new QCheckBox(tr("Vtx Color Shade"), shadeWidget);
    fShadeFlags[kShadeInherit] = new QCheckBox(tr("Inherit"), shadeWidget);
    fShadeFlags[kShadeIgnoreVtxIllum] = new QCheckBox(tr("Ignore Vtx Illum"), shadeWidget);
    fShadeFlags[kShadeEmissive] = new QCheckBox(tr("Emissive"), shadeWidget);
    fShadeFlags[kShadeReallyNoFog] = new QCheckBox(tr("Really No Fog"), shadeWidget);
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

    QWidget* miscWidget = new QWidget(flagTab);
    QGridLayout* miscLayout = new QGridLayout(miscWidget);
    miscLayout->setVerticalSpacing(0);
    miscLayout->setHorizontalSpacing(8);
    fMiscFlags[kMiscWireFrame] = new QCheckBox(tr("Wire Frame"), miscWidget);
    fMiscFlags[kMiscDrawMeshOutlines] = new QCheckBox(tr("Mesh Outlines"), miscWidget);
    fMiscFlags[kMiscTwoSided] = new QCheckBox(tr("Two-Sided"), miscWidget);
    fMiscFlags[kMiscDrawAsSplats] = new QCheckBox(tr("Draw as Splats"), miscWidget);
    fMiscFlags[kMiscAdjustPlane] = new QCheckBox(tr("Adjust Plane"), miscWidget);
    fMiscFlags[kMiscAdjustCylinder] = new QCheckBox(tr("Adjust Cylinder"), miscWidget);
    fMiscFlags[kMiscAdjustSphere] = new QCheckBox(tr("Adjust Sphere"), miscWidget);
    fMiscFlags[kMiscTroubledLoner] = new QCheckBox(tr("Troubled Loner"), miscWidget);
    fMiscFlags[kMiscBindSkip] = new QCheckBox(tr("Bind Skip"), miscWidget);
    fMiscFlags[kMiscBindMask] = new QCheckBox(tr("Bind Mask"), miscWidget);
    fMiscFlags[kMiscBindNext] = new QCheckBox(tr("Bind Next"), miscWidget);
    fMiscFlags[kMiscLightMap] = new QCheckBox(tr("Light Map"), miscWidget);
    fMiscFlags[kMiscUseReflectionXform] = new QCheckBox(tr("Use Reflec Xform"), miscWidget);
    fMiscFlags[kMiscPerspProjection] = new QCheckBox(tr("Persp Projection"), miscWidget);
    fMiscFlags[kMiscOrthoProjection] = new QCheckBox(tr("Ortho Projection"), miscWidget);
    fMiscFlags[kMiscRestartPassHere] = new QCheckBox(tr("Restart Pass Here"), miscWidget);
    fMiscFlags[kMiscBumpLayer] = new QCheckBox(tr("Bump Layer"), miscWidget);
    fMiscFlags[kMiscBumpDu] = new QCheckBox(tr("Bump DU"), miscWidget);
    fMiscFlags[kMiscBumpDv] = new QCheckBox(tr("Bump DV"), miscWidget);
    fMiscFlags[kMiscBumpDw] = new QCheckBox(tr("Bump DW"), miscWidget);
    fMiscFlags[kMiscNoShadowAlpha] = new QCheckBox(tr("No Shadow Alpha"), miscWidget);
    fMiscFlags[kMiscUseRefractionXform] = new QCheckBox(tr("Use Refrac Xform"), miscWidget);
    fMiscFlags[kMiscCam2Screen] = new QCheckBox(tr("Cam 2 Screen"), miscWidget);
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

    flagTab->addTab(blendWidget, tr("Blend Flags"));
    flagTab->addTab(clampZWidget, tr("Clamp/Z Flags"));
    flagTab->addTab(shadeWidget, tr("Shade Flags"));
    flagTab->addTab(miscWidget, tr("Misc Flags"));
    updateFlags(lay->getState());

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
    fPreshade = new QColorEdit(grpColors);
    fRuntime = new QColorEdit(grpColors);
    fAmbient = new QColorEdit(grpColors);
    fSpecular = new QColorEdit(grpColors);
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
    fLodBias = new QDoubleSpinBox(this);
    fSpecPower = new QDoubleSpinBox(this);
    fUvwSrc->setRange(0, 0x7FFFFFFF);
    fOpacity->setDecimals(3);
    fOpacity->setRange(0.0, 1.0);
    fOpacity->setSingleStep(0.005);
    fLodBias->setDecimals(3);
    fLodBias->setRange(-32.0, 32.0);
    fLodBias->setSingleStep(1.0);
    fSpecPower->setDecimals(3);
    fSpecPower->setRange(0.0, 1.0);
    fSpecPower->setSingleStep(0.005);
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

    QObject::connect(fBaseLayer, SIGNAL(addObject()), this, SLOT(setBaseLayer()));
    QObject::connect(fBaseLayer, SIGNAL(delObject()), this, SLOT(unsetBaseLayer()));
    QObject::connect(fTexture, SIGNAL(addObject()), this, SLOT(setTexture()));
    QObject::connect(fTexture, SIGNAL(delObject()), this, SLOT(unsetTexture()));
    QObject::connect(fVShader, SIGNAL(addObject()), this, SLOT(setVShader()));
    QObject::connect(fVShader, SIGNAL(delObject()), this, SLOT(unsetVShader()));
    QObject::connect(fPShader, SIGNAL(addObject()), this, SLOT(setPShader()));
    QObject::connect(fPShader, SIGNAL(delObject()), this, SLOT(unsetPShader()));
}

void QLayer::saveDamage()
{
    plLayer* lay = (plLayer*)fCreatable;

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
    updateState(lay->getState());
}

void QLayer::updateFlags(const hsGMatState& state)
{
    fBlendFlags[kBlendTest]->setChecked((state.fBlendFlags & hsGMatState::kBlendTest) != 0);
    fBlendFlags[kBlendAlpha]->setChecked((state.fBlendFlags & hsGMatState::kBlendAlpha) != 0);
    fBlendFlags[kBlendMult]->setChecked((state.fBlendFlags & hsGMatState::kBlendMult) != 0);
    fBlendFlags[kBlendAdd]->setChecked((state.fBlendFlags & hsGMatState::kBlendAdd) != 0);
    fBlendFlags[kBlendAddColorTimesAlpha]->setChecked((state.fBlendFlags & hsGMatState::kBlendAddColorTimesAlpha) != 0);
    fBlendFlags[kBlendAntiAlias]->setChecked((state.fBlendFlags & hsGMatState::kBlendAntiAlias) != 0);
    fBlendFlags[kBlendDetail]->setChecked((state.fBlendFlags & hsGMatState::kBlendDetail) != 0);
    fBlendFlags[kBlendNoColor]->setChecked((state.fBlendFlags & hsGMatState::kBlendNoColor) != 0);
    fBlendFlags[kBlendMADD]->setChecked((state.fBlendFlags & hsGMatState::kBlendMADD) != 0);
    fBlendFlags[kBlendDot3]->setChecked((state.fBlendFlags & hsGMatState::kBlendDot3) != 0);
    fBlendFlags[kBlendAddSigned]->setChecked((state.fBlendFlags & hsGMatState::kBlendAddSigned) != 0);
    fBlendFlags[kBlendAddSigned2X]->setChecked((state.fBlendFlags & hsGMatState::kBlendAddSigned2X) != 0);
    fBlendFlags[kBlendInvertAlpha]->setChecked((state.fBlendFlags & hsGMatState::kBlendInvertAlpha) != 0);
    fBlendFlags[kBlendInvertColor]->setChecked((state.fBlendFlags & hsGMatState::kBlendInvertColor) != 0);
    fBlendFlags[kBlendAlphaMult]->setChecked((state.fBlendFlags & hsGMatState::kBlendAlphaMult) != 0);
    fBlendFlags[kBlendAlphaAdd]->setChecked((state.fBlendFlags & hsGMatState::kBlendAlphaAdd) != 0);
    fBlendFlags[kBlendNoVtxAlpha]->setChecked((state.fBlendFlags & hsGMatState::kBlendNoVtxAlpha) != 0);
    fBlendFlags[kBlendNoTexColor]->setChecked((state.fBlendFlags & hsGMatState::kBlendNoTexColor) != 0);
    fBlendFlags[kBlendNoTexAlpha]->setChecked((state.fBlendFlags & hsGMatState::kBlendNoTexAlpha) != 0);
    fBlendFlags[kBlendInvertVtxAlpha]->setChecked((state.fBlendFlags & hsGMatState::kBlendInvertVtxAlpha) != 0);
    fBlendFlags[kBlendAlphaAlways]->setChecked((state.fBlendFlags & hsGMatState::kBlendAlphaAlways) != 0);
    fBlendFlags[kBlendInvertFinalColor]->setChecked((state.fBlendFlags & hsGMatState::kBlendInvertFinalColor) != 0);
    fBlendFlags[kBlendInvertFinalAlpha]->setChecked((state.fBlendFlags & hsGMatState::kBlendInvertFinalAlpha) != 0);
    fBlendFlags[kBlendEnvBumpNext]->setChecked((state.fBlendFlags & hsGMatState::kBlendEnvBumpNext) != 0);
    fBlendFlags[kBlendSubtract]->setChecked((state.fBlendFlags & hsGMatState::kBlendSubtract) != 0);
    fBlendFlags[kBlendRevSubtract]->setChecked((state.fBlendFlags & hsGMatState::kBlendRevSubtract) != 0);
    fBlendFlags[kBlendAlphaTestHigh]->setChecked((state.fBlendFlags & hsGMatState::kBlendAlphaTestHigh) != 0);

    fClampFlags[kClampTextureU]->setChecked((state.fClampFlags & hsGMatState::kClampTextureU) != 0);
    fClampFlags[kClampTextureV]->setChecked((state.fClampFlags & hsGMatState::kClampTextureV) != 0);

    fZFlags[kZIncLayer]->setChecked((state.fZFlags & hsGMatState::kZIncLayer) != 0);
    fZFlags[kZClearZ]->setChecked((state.fZFlags & hsGMatState::kZClearZ) != 0);
    fZFlags[kZNoZRead]->setChecked((state.fZFlags & hsGMatState::kZNoZRead) != 0);
    fZFlags[kZNoZWrite]->setChecked((state.fZFlags & hsGMatState::kZNoZWrite) != 0);
    fZFlags[kZLODBias]->setChecked((state.fZFlags & hsGMatState::kZLODBias) != 0);

    fShadeFlags[kShadeSoftShadow]->setChecked((state.fShadeFlags & hsGMatState::kShadeSoftShadow) != 0);
    fShadeFlags[kShadeNoProjectors]->setChecked((state.fShadeFlags & hsGMatState::kShadeNoProjectors) != 0);
    fShadeFlags[kShadeEnvironMap]->setChecked((state.fShadeFlags & hsGMatState::kShadeEnvironMap) != 0);
    fShadeFlags[kShadeVertexShade]->setChecked((state.fShadeFlags & hsGMatState::kShadeVertexShade) != 0);
    fShadeFlags[kShadeBlack]->setChecked((state.fShadeFlags & hsGMatState::kShadeBlack) != 0);
    fShadeFlags[kShadeSpecular]->setChecked((state.fShadeFlags & hsGMatState::kShadeSpecular) != 0);
    fShadeFlags[kShadeNoFog]->setChecked((state.fShadeFlags & hsGMatState::kShadeNoFog) != 0);
    fShadeFlags[kShadeWhite]->setChecked((state.fShadeFlags & hsGMatState::kShadeWhite) != 0);
    fShadeFlags[kShadeSpecularAlpha]->setChecked((state.fShadeFlags & hsGMatState::kShadeSpecularAlpha) != 0);
    fShadeFlags[kShadeSpecularColor]->setChecked((state.fShadeFlags & hsGMatState::kShadeSpecularColor) != 0);
    fShadeFlags[kShadeSpecularHighlight]->setChecked((state.fShadeFlags & hsGMatState::kShadeSpecularHighlight) != 0);
    fShadeFlags[kShadeVertColShade]->setChecked((state.fShadeFlags & hsGMatState::kShadeVertColShade) != 0);
    fShadeFlags[kShadeInherit]->setChecked((state.fShadeFlags & hsGMatState::kShadeInherit) != 0);
    fShadeFlags[kShadeIgnoreVtxIllum]->setChecked((state.fShadeFlags & hsGMatState::kShadeIgnoreVtxIllum) != 0);
    fShadeFlags[kShadeEmissive]->setChecked((state.fShadeFlags & hsGMatState::kShadeEmissive) != 0);
    fShadeFlags[kShadeReallyNoFog]->setChecked((state.fShadeFlags & hsGMatState::kShadeReallyNoFog) != 0);

    fMiscFlags[kMiscWireFrame]->setChecked((state.fMiscFlags & hsGMatState::kMiscWireFrame) != 0);
    fMiscFlags[kMiscDrawMeshOutlines]->setChecked((state.fMiscFlags & hsGMatState::kMiscDrawMeshOutlines) != 0);
    fMiscFlags[kMiscTwoSided]->setChecked((state.fMiscFlags & hsGMatState::kMiscTwoSided) != 0);
    fMiscFlags[kMiscDrawAsSplats]->setChecked((state.fMiscFlags & hsGMatState::kMiscDrawAsSplats) != 0);
    fMiscFlags[kMiscAdjustPlane]->setChecked((state.fMiscFlags & hsGMatState::kMiscAdjustPlane) != 0);
    fMiscFlags[kMiscAdjustCylinder]->setChecked((state.fMiscFlags & hsGMatState::kMiscAdjustCylinder) != 0);
    fMiscFlags[kMiscAdjustSphere]->setChecked((state.fMiscFlags & hsGMatState::kMiscAdjustSphere) != 0);
    fMiscFlags[kMiscTroubledLoner]->setChecked((state.fMiscFlags & hsGMatState::kMiscTroubledLoner) != 0);
    fMiscFlags[kMiscBindSkip]->setChecked((state.fMiscFlags & hsGMatState::kMiscBindSkip) != 0);
    fMiscFlags[kMiscBindMask]->setChecked((state.fMiscFlags & hsGMatState::kMiscBindMask) != 0);
    fMiscFlags[kMiscBindNext]->setChecked((state.fMiscFlags & hsGMatState::kMiscBindNext) != 0);
    fMiscFlags[kMiscLightMap]->setChecked((state.fMiscFlags & hsGMatState::kMiscLightMap) != 0);
    fMiscFlags[kMiscUseReflectionXform]->setChecked((state.fMiscFlags & hsGMatState::kMiscUseReflectionXform) != 0);
    fMiscFlags[kMiscPerspProjection]->setChecked((state.fMiscFlags & hsGMatState::kMiscPerspProjection) != 0);
    fMiscFlags[kMiscOrthoProjection]->setChecked((state.fMiscFlags & hsGMatState::kMiscOrthoProjection) != 0);
    fMiscFlags[kMiscRestartPassHere]->setChecked((state.fMiscFlags & hsGMatState::kMiscRestartPassHere) != 0);
    fMiscFlags[kMiscBumpLayer]->setChecked((state.fMiscFlags & hsGMatState::kMiscBumpLayer) != 0);
    fMiscFlags[kMiscBumpDu]->setChecked((state.fMiscFlags & hsGMatState::kMiscBumpDu) != 0);
    fMiscFlags[kMiscBumpDv]->setChecked((state.fMiscFlags & hsGMatState::kMiscBumpDv) != 0);
    fMiscFlags[kMiscBumpDw]->setChecked((state.fMiscFlags & hsGMatState::kMiscBumpDw) != 0);
    fMiscFlags[kMiscNoShadowAlpha]->setChecked((state.fMiscFlags & hsGMatState::kMiscNoShadowAlpha) != 0);
    fMiscFlags[kMiscUseRefractionXform]->setChecked((state.fMiscFlags & hsGMatState::kMiscUseRefractionXform) != 0);
    fMiscFlags[kMiscCam2Screen]->setChecked((state.fMiscFlags & hsGMatState::kMiscCam2Screen) != 0);
}

void QLayer::updateState(hsGMatState& state) const
{
    state.fBlendFlags = (fBlendFlags[kBlendTest]->isChecked() ? hsGMatState::kBlendTest : 0)
                      | (fBlendFlags[kBlendAlpha]->isChecked() ? hsGMatState::kBlendAlpha : 0)
                      | (fBlendFlags[kBlendMult]->isChecked() ? hsGMatState::kBlendMult : 0)
                      | (fBlendFlags[kBlendAdd]->isChecked() ? hsGMatState::kBlendAdd : 0)
                      | (fBlendFlags[kBlendAddColorTimesAlpha]->isChecked() ? hsGMatState::kBlendAddColorTimesAlpha : 0)
                      | (fBlendFlags[kBlendAntiAlias]->isChecked() ? hsGMatState::kBlendAntiAlias : 0)
                      | (fBlendFlags[kBlendDetail]->isChecked() ? hsGMatState::kBlendDetail : 0)
                      | (fBlendFlags[kBlendNoColor]->isChecked() ? hsGMatState::kBlendNoColor : 0)
                      | (fBlendFlags[kBlendMADD]->isChecked() ? hsGMatState::kBlendMADD : 0)
                      | (fBlendFlags[kBlendDot3]->isChecked() ? hsGMatState::kBlendDot3 : 0)
                      | (fBlendFlags[kBlendAddSigned]->isChecked() ? hsGMatState::kBlendAddSigned : 0)
                      | (fBlendFlags[kBlendAddSigned2X]->isChecked() ? hsGMatState::kBlendAddSigned2X : 0)
                      | (fBlendFlags[kBlendInvertAlpha]->isChecked() ? hsGMatState::kBlendInvertAlpha : 0)
                      | (fBlendFlags[kBlendInvertColor]->isChecked() ? hsGMatState::kBlendInvertColor : 0)
                      | (fBlendFlags[kBlendAlphaMult]->isChecked() ? hsGMatState::kBlendAlphaMult : 0)
                      | (fBlendFlags[kBlendAlphaAdd]->isChecked() ? hsGMatState::kBlendAlphaAdd : 0)
                      | (fBlendFlags[kBlendNoVtxAlpha]->isChecked() ? hsGMatState::kBlendNoVtxAlpha : 0)
                      | (fBlendFlags[kBlendNoTexColor]->isChecked() ? hsGMatState::kBlendNoTexColor : 0)
                      | (fBlendFlags[kBlendNoTexAlpha]->isChecked() ? hsGMatState::kBlendNoTexAlpha : 0)
                      | (fBlendFlags[kBlendInvertVtxAlpha]->isChecked() ? hsGMatState::kBlendInvertVtxAlpha : 0)
                      | (fBlendFlags[kBlendAlphaAlways]->isChecked() ? hsGMatState::kBlendAlphaAlways : 0)
                      | (fBlendFlags[kBlendInvertFinalColor]->isChecked() ? hsGMatState::kBlendInvertFinalColor : 0)
                      | (fBlendFlags[kBlendInvertFinalAlpha]->isChecked() ? hsGMatState::kBlendInvertFinalAlpha : 0)
                      | (fBlendFlags[kBlendEnvBumpNext]->isChecked() ? hsGMatState::kBlendEnvBumpNext : 0)
                      | (fBlendFlags[kBlendSubtract]->isChecked() ? hsGMatState::kBlendSubtract : 0)
                      | (fBlendFlags[kBlendRevSubtract]->isChecked() ? hsGMatState::kBlendRevSubtract : 0)
                      | (fBlendFlags[kBlendAlphaTestHigh]->isChecked() ? hsGMatState::kBlendAlphaTestHigh : 0);
    state.fClampFlags = (fClampFlags[kClampTextureU]->isChecked() ? hsGMatState::kClampTextureU : 0)
                      | (fClampFlags[kClampTextureV]->isChecked() ? hsGMatState::kClampTextureV : 0);
    state.fZFlags = (fZFlags[kZIncLayer]->isChecked() ? hsGMatState::kZIncLayer : 0)
                  | (fZFlags[kZClearZ]->isChecked() ? hsGMatState::kZClearZ : 0)
                  | (fZFlags[kZNoZRead]->isChecked() ? hsGMatState::kZNoZRead : 0)
                  | (fZFlags[kZNoZWrite]->isChecked() ? hsGMatState::kZNoZWrite : 0)
                  | (fZFlags[kZLODBias]->isChecked() ? hsGMatState::kZLODBias : 0);
    state.fShadeFlags = (fShadeFlags[kShadeSoftShadow]->isChecked() ? hsGMatState::kShadeSoftShadow : 0)
                      | (fShadeFlags[kShadeNoProjectors]->isChecked() ? hsGMatState::kShadeNoProjectors : 0)
                      | (fShadeFlags[kShadeEnvironMap]->isChecked() ? hsGMatState::kShadeEnvironMap : 0)
                      | (fShadeFlags[kShadeVertexShade]->isChecked() ? hsGMatState::kShadeVertexShade : 0)
                      | (fShadeFlags[kShadeBlack]->isChecked() ? hsGMatState::kShadeBlack : 0)
                      | (fShadeFlags[kShadeSpecular]->isChecked() ? hsGMatState::kShadeSpecular : 0)
                      | (fShadeFlags[kShadeNoFog]->isChecked() ? hsGMatState::kShadeNoFog : 0)
                      | (fShadeFlags[kShadeWhite]->isChecked() ? hsGMatState::kShadeWhite : 0)
                      | (fShadeFlags[kShadeSpecularAlpha]->isChecked() ? hsGMatState::kShadeSpecularAlpha : 0)
                      | (fShadeFlags[kShadeSpecularColor]->isChecked() ? hsGMatState::kShadeSpecularColor : 0)
                      | (fShadeFlags[kShadeSpecularHighlight]->isChecked() ? hsGMatState::kShadeSpecularHighlight : 0)
                      | (fShadeFlags[kShadeVertColShade]->isChecked() ? hsGMatState::kShadeVertColShade : 0)
                      | (fShadeFlags[kShadeInherit]->isChecked() ? hsGMatState::kShadeInherit : 0)
                      | (fShadeFlags[kShadeIgnoreVtxIllum]->isChecked() ? hsGMatState::kShadeIgnoreVtxIllum : 0)
                      | (fShadeFlags[kShadeEmissive]->isChecked() ? hsGMatState::kShadeEmissive : 0)
                      | (fShadeFlags[kShadeReallyNoFog]->isChecked() ? hsGMatState::kShadeReallyNoFog : 0);
    state.fMiscFlags = (fMiscFlags[kMiscWireFrame]->isChecked() ? hsGMatState::kMiscWireFrame : 0)
                     | (fMiscFlags[kMiscDrawMeshOutlines]->isChecked() ? hsGMatState::kMiscDrawMeshOutlines : 0)
                     | (fMiscFlags[kMiscTwoSided]->isChecked() ? hsGMatState::kMiscTwoSided : 0)
                     | (fMiscFlags[kMiscDrawAsSplats]->isChecked() ? hsGMatState::kMiscDrawAsSplats : 0)
                     | (fMiscFlags[kMiscAdjustPlane]->isChecked() ? hsGMatState::kMiscAdjustPlane : 0)
                     | (fMiscFlags[kMiscAdjustCylinder]->isChecked() ? hsGMatState::kMiscAdjustCylinder : 0)
                     | (fMiscFlags[kMiscAdjustSphere]->isChecked() ? hsGMatState::kMiscAdjustSphere : 0)
                     | (fMiscFlags[kMiscTroubledLoner]->isChecked() ? hsGMatState::kMiscTroubledLoner : 0)
                     | (fMiscFlags[kMiscBindSkip]->isChecked() ? hsGMatState::kMiscBindSkip : 0)
                     | (fMiscFlags[kMiscBindMask]->isChecked() ? hsGMatState::kMiscBindMask : 0)
                     | (fMiscFlags[kMiscBindNext]->isChecked() ? hsGMatState::kMiscBindNext : 0)
                     | (fMiscFlags[kMiscLightMap]->isChecked() ? hsGMatState::kMiscLightMap : 0)
                     | (fMiscFlags[kMiscUseReflectionXform]->isChecked() ? hsGMatState::kMiscUseReflectionXform : 0)
                     | (fMiscFlags[kMiscPerspProjection]->isChecked() ? hsGMatState::kMiscPerspProjection : 0)
                     | (fMiscFlags[kMiscOrthoProjection]->isChecked() ? hsGMatState::kMiscOrthoProjection : 0)
                     | (fMiscFlags[kMiscRestartPassHere]->isChecked() ? hsGMatState::kMiscRestartPassHere : 0)
                     | (fMiscFlags[kMiscBumpLayer]->isChecked() ? hsGMatState::kMiscBumpLayer : 0)
                     | (fMiscFlags[kMiscBumpDu]->isChecked() ? hsGMatState::kMiscBumpDu : 0)
                     | (fMiscFlags[kMiscBumpDv]->isChecked() ? hsGMatState::kMiscBumpDv : 0)
                     | (fMiscFlags[kMiscBumpDw]->isChecked() ? hsGMatState::kMiscBumpDw : 0)
                     | (fMiscFlags[kMiscNoShadowAlpha]->isChecked() ? hsGMatState::kMiscNoShadowAlpha : 0)
                     | (fMiscFlags[kMiscUseRefractionXform]->isChecked() ? hsGMatState::kMiscUseRefractionXform : 0)
                     | (fMiscFlags[kMiscCam2Screen]->isChecked() ? hsGMatState::kMiscCam2Screen : 0);
}

void QLayer::setBaseLayer()
{
    //...
}

void QLayer::setTexture()
{
    //...
}

void QLayer::setVShader()
{
    //...
}

void QLayer::setPShader()
{
    //...
}

void QLayer::unsetBaseLayer()
{
    plLayer* lay = (plLayer*)fCreatable;
    lay->setUnderLay(plKey());
    fBaseLayer->setCreatable(NULL);
    fBaseLayer->setText("(None)");
}

void QLayer::unsetTexture()
{
    plLayer* lay = (plLayer*)fCreatable;
    lay->setTexture(plKey());
    fTexture->setCreatable(NULL);
    fTexture->setText("(None)");
}

void QLayer::unsetVShader()
{
    plLayer* lay = (plLayer*)fCreatable;
    lay->setVertexShader(plKey());
    fVShader->setCreatable(NULL);
    fVShader->setText("(None)");
}

void QLayer::unsetPShader()
{
    plLayer* lay = (plLayer*)fCreatable;
    lay->setPixelShader(plKey());
    fPShader->setCreatable(NULL);
    fPShader->setText("(None)");
}
