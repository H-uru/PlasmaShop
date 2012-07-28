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

#include "QCreatable.h"

#include <PRP/KeyedObject/hsKeyedObject.h>
#include <QMessageBox>
#include "../QPlasmaUtils.h"

QCreatable::QCreatable(plCreatable* pCre, short type, QWidget* parent)
          : QWidget(parent), fCreatable(pCre), fForceType(type)
{
    setAttribute(Qt::WA_DeleteOnClose);
    QIcon ico = pqGetTypeIcon(type);
    if (!ico.isNull())
        setWindowIcon(ico);
    hsKeyedObject* ko = hsKeyedObject::Convert(fCreatable, false);
    if (ko != NULL && ko->getKey().Exists()) {
        setWindowTitle(pqGetFriendlyClassName(type) +
                       ": " + ~ko->getKey()->getName());
    } else {
        setWindowTitle(pqGetFriendlyClassName(type));
    }
}

bool QCreatable::isMatch(plCreatable* pCre, short type)
{
    if (fCreatable == NULL)
        return false;
    if (type == -1) type = pCre->ClassIndex();
    return (fCreatable == pCre) && (fForceType == type);
}

void QCreatable::closeEvent(QCloseEvent*)
{
    saveDamage();
}


/***** Creatable Forms -- think QFactory ;) *****/
#include "QPrcEditor.h"
#include "PRP/QSceneNode.h"
#include "PRP/Animation/QAnimTimeConvert.h"
#include "PRP/Audio/QAudible.h"
#include "PRP/Audio/QSoundBuffer.h"
#include "PRP/Audio/QWinSound.h"
#include "PRP/Avatar/QMultistageBehMod.h"
#include "PRP/GUI/QGUIButtonMod.h"
#include "PRP/GUI/QGUICheckBoxCtrl.h"
#include "PRP/GUI/QGUIClickMapCtrl.h"
#include "PRP/GUI/QGUIControlMod.h"
#include "PRP/GUI/QGUIDialogMod.h"
#include "PRP/GUI/QGUIDraggableMod.h"
#include "PRP/GUI/QGUIDynDisplayCtrl.h"
#include "PRP/GUI/QGUIKnobCtrl.h"
#include "PRP/GUI/QGUIListBoxMod.h"
#include "PRP/GUI/QGUIMenuItem.h"
#include "PRP/GUI/QGUIMultiLineEditCtrl.h"
#include "PRP/GUI/QGUIPopUpMenu.h"
#include "PRP/GUI/QGUIProgressCtrl.h"
#include "PRP/GUI/QGUIRadioGroupCtrl.h"
#include "PRP/GUI/QGUISkin.h"
#include "PRP/GUI/QGUITextBoxMod.h"
#include "PRP/GUI/QGUIUpDownPairMod.h"
#include "PRP/GUI/QImageLibMod.h"
#include "PRP/Light/QShadowMaster.h"
#include "PRP/Message/QMsgForwarder.h"
#include "PRP/Modifier/QInterfaceInfoModifier.h"
#include "PRP/Modifier/QOneShotMod.h"
#include "PRP/Modifier/QPythonFileMod.h"
#include "PRP/Modifier/QSpawnModifier.h"
#include "PRP/Object/QAudioInterface.h"
#include "PRP/Object/QCoordinateInterface.h"
#include "PRP/Object/QDrawInterface.h"
#include "PRP/Object/QSceneObject.h"
#include "PRP/Object/QSimulationInterface.h"
#include "PRP/Object/QSynchedObject.h"
#include "PRP/Physics/QCollisionDetector.h"
#include "PRP/Physics/QDetectorModifier.h"
#include "PRP/Surface/QCubicEnvironmap.h"
#include "PRP/Surface/QDynamicTextMap.h"
#include "PRP/Surface/QFadeOpacityMod.h"
#include "PRP/Surface/QLayer.h"
#include "PRP/Surface/QLayerAnimation.h"
#include "PRP/Surface/QLayerLinkAnimation.h"
#include "PRP/Surface/QLayerMovie.h"
#include "PRP/Surface/QLayerSDLAnimation.h"
#include "PRP/Surface/QMaterial.h"
#include "PRP/Surface/QMipmap.h"
#include "PRP/Render/QSceneObj_Preview.h"

QCreatable* pqMakeCreatableForm(plCreatable* pCre, QWidget* parent, short forceType)
{
    short type = (forceType == -1) ? pCre->ClassIndex() : forceType;

    if ((type & kPRC_Type) != 0)
        return new QPrcEditor(pCre, parent);

    switch (type) {
    // Keyed Object types
    case k2WayWinAudible:
        return new QWinAudible(pCre, parent);
    case kAudible:
        return new QAudible(pCre, parent);
    case kAudibleNull:
        return new QAudible(pCre, parent);
    case kAudioInterface:
        return new QAudioInterface(pCre, parent);
    case kCollisionDetector:
        return new QCollisionDetector(pCre, parent);
    case kCoordinateInterface:
        return new QCoordinateInterface(pCre, parent);
    case kCubicEnvironmap:
        return new QCubicEnvironmap(pCre, parent);
    case kDetectorModifier:
        return new QDetectorModifier(pCre, parent);
    case kDirectShadowMaster:
        return new QShadowMaster(pCre, parent);
    case kDrawInterface:
        return new QDrawInterface(pCre, parent);
    case kDynamicTextMap:
        return new QDynamicTextMap(pCre, parent);
    case kFadeOpacityMod:
        return new QFadeOpacityMod(pCre, parent);
    case kInterfaceInfoModifier:
        return new QInterfaceInfoModifier(pCre, parent);
    case kGMaterial:
        return new QMaterial(pCre, parent);
    case kGUIButtonMod:
        return new QGUIButtonMod(pCre, parent);
    case kGUICheckBoxCtrl:
        return new QGUICheckBoxCtrl(pCre, parent);
    case kGUIClickMapCtrl:
        return new QGUIClickMapCtrl(pCre, parent);
    case kGUIControlMod:
        return new QGUIControlMod(pCre, parent);
    case kGUIDialogMod:
        return new QGUIDialogMod(pCre, parent);
    case kGUIDragBarCtrl:
        return new QGUIControlMod(pCre, parent);
    case kGUIDraggableMod:
        return new QGUIDraggableMod(pCre, parent);
    case kGUIDynDisplayCtrl:
        return new QGUIDynDisplayCtrl(pCre, parent);
    case kGUIEditBoxMod:
        return new QGUIControlMod(pCre, parent);
    case kGUIKnobCtrl:
        return new QGUIKnobCtrl(pCre, parent);
    case kGUIListBoxMod:
        return new QGUIListBoxMod(pCre, parent);
    case kGUIMenuItem:
        return new QGUIMenuItem(pCre, parent);
    case kGUIMultiLineEditCtrl:
        return new QGUIMultiLineEditCtrl(pCre, parent);
    case kGUIPopUpMenu:
        return new QGUIPopUpMenu(pCre, parent);
    case kGUIProgressCtrl:
        return new QGUIProgressCtrl(pCre, parent);
    case kGUIRadioGroupCtrl:
        return new QGUIRadioGroupCtrl(pCre, parent);
    case kGUISkin:
        return new QGUISkin(pCre, parent);
    case kGUITextBoxMod:
        return new QGUITextBoxMod(pCre, parent);
    case kGUIUpDownPairMod:
        return new QGUIUpDownPairMod(pCre, parent);
    case kImageLibMod:
        return new QImageLibMod(pCre, parent);
    case kLayer:
        return new QLayer(pCre, parent);
    case kLayerAnimation:
        return new QLayerAnimation(pCre, parent);
    case kLayerAVI:
        return new QLayerMovie(pCre, parent);
    case kLayerBink:
        return new QLayerMovie(pCre, parent);
    case kLayerDepth:
        return new QLayer(pCre, parent);
    case kLayerLinkAnimation:
        return new QLayerLinkAnimation(pCre, parent);
    case kLayerMovie:
        return new QLayerMovie(pCre, parent);
    case kLayerSDLAnimation:
        return new QLayerSDLAnimation(pCre, parent);
    case kMipmap:
        return new QMipmap(pCre, parent);
    case kMsgForwarder:
        return new QMsgForwarder(pCre, parent);
    case kMultistageBehMod:
        return new QMultistageBehMod(pCre, parent);
    case kOneShotMod:
        return new QOneShotMod(pCre, parent);
    case kPanicLinkRegion:
        return new QCollisionDetector(pCre, parent);
    case kPickingDetector:
        return new QDetectorModifier(pCre, parent);
    case kPointShadowMaster:
        return new QShadowMaster(pCre, parent);
    case kPythonFileMod:
        return new QPythonFileMod(pCre, parent);
    case kSceneNode:
        return new QSceneNode(pCre, parent);
    case kSceneObject:
        return new QSceneObject(pCre, parent);
    case kShadowMaster:
        return new QShadowMaster(pCre, parent);
    case kSimulationInterface:
        return new QSimulationInterface(pCre, parent);
    case kSound:
        return new QWinSound(pCre, parent);
    case kSoundBuffer:
        return new QSoundBuffer(pCre, parent);
    case kSpawnModifier:
        return new QSpawnModifier(pCre, parent);
    case kSubworldRegionDetector:
        return new QCollisionDetector(pCre, parent);
    case kSynchedObject:
        return new QSynchedObject(pCre, parent);
    case kWinAudible:
        return new QWinAudible(pCre, parent);
    case kWin32LinkSound:
        return new QWin32Sound(pCre, parent);
    case kWin32Sound:
        return new QWin32Sound(pCre, parent);
    case kWin32StaticSound:
        return new QWin32Sound(pCre, parent);
    case kWin32StreamingSound:
        return new QWin32Sound(pCre, parent);

    // Non-Keyed Object types
    case kAnimStage:
        return new QAnimStage(pCre, parent);
    case kAnimTimeConvert:
        return new QAnimTimeConvert(pCre, parent);
    case kATCEaseCurve:
        return new QATCEaseCurve(pCre, parent);
    case kConstAccelEaseCurve:
        return new QATCEaseCurve(pCre, parent);
    case kSplineEaseCurve:
        return new QATCEaseCurve(pCre, parent);

    // Preview meta-types
    case kPreviewMipmap:
        return new QMipmap_Preview(pCre, parent);
    case kPreviewSceneObject:
        return new QSceneObj_Preview(pCre, parent);

    default:
        if ((type & 0x1000) == 0) {
            QMessageBox msgBox(QMessageBox::Information, parent->tr("Oops"),
                            parent->tr("No editor is currently available for %1")
                                      .arg(pqGetFriendlyClassName(type)),
                            QMessageBox::Ok, parent);
            msgBox.exec();
            return new QPrcEditor(pCre, parent);
        } else {
            QMessageBox msgBox(QMessageBox::Information, parent->tr("Oops"),
                            parent->tr("No previewer is currently available for %1")
                                      .arg(pqGetFriendlyClassName(type)),
                            QMessageBox::Ok, parent);
            msgBox.exec();
        }
        return NULL;
    }
}
