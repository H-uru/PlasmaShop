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
                       ": " + ko->getKey()->getName().cstr());
    } else {
        setWindowTitle(pqGetFriendlyClassName(type));
    }
}

bool QCreatable::isMatch(plCreatable* pCre, short type)
{
    if (fCreatable == NULL)
        return false;
    return (fCreatable == pCre) && (fForceType == type);
}

void QCreatable::closeEvent(QCloseEvent*)
{
    saveDamage();
}


/***** Creatable Forms -- think QFactory ;) *****/
#include "PRP/Object/QCoordinateInterface.h"
#include "PRP/Object/QSynchedObject.h"

QCreatable* pqMakeCreatableForm(plCreatable* pCre, QWidget* parent, short forceType)
{
    short type = (forceType == -1) ? pCre->ClassIndex() : forceType;

    switch (type) {
    case kCoordinateInterface:
        return new QCoordinateInterface(pCre, parent);
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
