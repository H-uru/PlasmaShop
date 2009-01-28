#include "QCreatable.h"

#include <PRP/KeyedObject/hsKeyedObject.h>
#include <QMessageBox>
#include "../QPlasmaUtils.h"

QCreatable::QCreatable(plCreatable* pCre, short type, QWidget* parent)
          : QWidget(parent), fCreatable(pCre), fForceType(type)
{
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowIcon(pqGetTypeIcon(type));
    if (hsKeyedObject* ko = hsKeyedObject::Convert(fCreatable)) {
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
#include "PRP/QSceneNode.h"
#include "PRP/Audio/QSoundBuffer.h"
#include "PRP/Message/QMsgForwarder.h"
#include "PRP/Modifier/QPythonFileMod.h"
#include "PRP/Object/QAudioInterface.h"
#include "PRP/Object/QCoordinateInterface.h"
#include "PRP/Object/QDrawInterface.h"
#include "PRP/Object/QSceneObject.h"
#include "PRP/Object/QSimulationInterface.h"
#include "PRP/Object/QSynchedObject.h"
#include "PRP/Surface/QLayer.h"
#include "PRP/Surface/QLayerAnimation.h"
#include "PRP/Surface/QLayerSDLAnimation.h"
#include "PRP/Surface/QMaterial.h"
#include "PRP/Surface/QMipmap.h"

QCreatable* pqMakeCreatableForm(plCreatable* pCre, QWidget* parent, short forceType)
{
    short type = (forceType == -1) ? pCre->ClassIndex() : forceType;

    switch (type) {
    case kAudioInterface:
        return new QAudioInterface(pCre, parent);
    case kCoordinateInterface:
        return new QCoordinateInterface(pCre, parent);
    case kDrawInterface:
        return new QDrawInterface(pCre, parent);
    case kGMaterial:
        return new QMaterial(pCre, parent);
    case kLayer:
        return new QLayer(pCre, parent);
    case kLayerAnimation:
        return new QLayerAnimation(pCre, parent);
    case kLayerDepth:
        return new QLayer(pCre, parent);
    case kLayerSDLAnimation:
        return new QLayerSDLAnimation(pCre, parent);
    case kMsgForwarder:
        return new QMsgForwarder(pCre, parent);
    case kPythonFileMod:
        return new QPythonFileMod(pCre, parent);
    case kSceneNode:
        return new QSceneNode(pCre, parent);
    case kSceneObject:
        return new QSceneObject(pCre, parent);
    case kSimulationInterface:
        return new QSimulationInterface(pCre, parent);
    case kSoundBuffer:
        return new QSoundBuffer(pCre, parent);
    case kSynchedObject:
        return new QSynchedObject(pCre, parent);

    case kPreviewMipmap:
        return new QMipmap_Preview(pCre, parent);

    default:
        if ((type & 0x1000) == 0) {
            QMessageBox msgBox(QMessageBox::Information, parent->tr("Oops"),
                            parent->tr("No editor is currently available for %1")
                                      .arg(pqGetFriendlyClassName(type)),
                            QMessageBox::Ok, parent);
            msgBox.exec();
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
