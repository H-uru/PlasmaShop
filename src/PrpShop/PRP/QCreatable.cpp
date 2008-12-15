#include "QCreatable.h"

#include <PRP/KeyedObject/hsKeyedObject.h>
#include <QMessageBox>
#include "../QPlasmaUtils.h"

QCreatable::QCreatable(plCreatable* pCre, short type, QWidget* parent)
          : QWidget(parent), fCreatable(pCre), fForceType(type)
{
    if (fForceType == -1) fForceType = fCreatable->ClassIndex();
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowIcon(pqGetTypeIcon(type));
    if (hsKeyedObject* ko = hsKeyedObject::Convert(fCreatable)) {
        setWindowTitle(pqGetFriendlyClassName(type) +
                       ": " + ko->getKey()->getName().cstr());
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
#include "PRP/Object/QAudioInterface.h"
#include "PRP/Object/QCoordinateInterface.h"
#include "PRP/Object/QSceneObject.h"
#include "PRP/Object/QSimulationInterface.h"
#include "PRP/Object/QSynchedObject.h"

QCreatable* pqMakeCreatableForm(plCreatable* pCre, QWidget* parent, short forceType)
{
    short type = (forceType == -1) ? pCre->ClassIndex() : forceType;

    switch (type) {
    case kAudioInterface:
        return new QAudioInterface(pCre, parent);
    case kCoordinateInterface:
        return new QCoordinateInterface(pCre, parent);
    case kSceneObject:
        return new QSceneObject(pCre, parent);
    case kSimulationInterface:
        return new QSimulationInterface(pCre, parent);
    case kSynchedObject:
        return new QSynchedObject(pCre, parent);
    default:
        QMessageBox msgBox(QMessageBox::Information, parent->tr("Oops"),
                           parent->tr("No editor is currently available for %1")
                                     .arg(pqGetFriendlyClassName(type)),
                           QMessageBox::Ok, parent);
        msgBox.exec();
        return NULL;
    }
}
