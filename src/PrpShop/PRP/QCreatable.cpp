#include "QCreatable.h"

#include <PRP/KeyedObject/hsKeyedObject.h>
#include <QMessageBox>
#include "../QPlasmaUtils.h"

QCreatable::QCreatable(plCreatable* pCre, QWidget* parent)
          : QWidget(parent), fCreatable(pCre)
{
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowIcon(pqGetTypeIcon(fCreatable->ClassIndex()));
    if (hsKeyedObject* ko = hsKeyedObject::Convert(fCreatable)) {
        setWindowTitle(pqGetFriendlyClassName(fCreatable->ClassIndex()) +
                       ": " + ko->getKey()->getName().cstr());
    } else {
        setWindowTitle(pqGetFriendlyClassName(fCreatable->ClassIndex()));
    }
}

bool QCreatable::isMatch(plCreatable* pCre, short forceType)
{
    if (fCreatable == NULL)
        return false;
    if (forceType == -1)
        return fCreatable == pCre;
    return (fCreatable == pCre) && (fCreatable->ClassIndex() == forceType);
}

void QCreatable::closeEvent(QCloseEvent*)
{
    saveDamage();
}


/***** Creatable Forms -- think QFactory ;) *****/
#include "PRP/Object/QCoordinateInterface.h"

QCreatable* pqMakeCreatableForm(plCreatable* pCre, QWidget* parent, short forceType)
{
    short type = (forceType == -1) ? pCre->ClassIndex() : forceType;

    switch (type) {
    case kCoordinateInterface:
        return new QCoordinateInterface(pCre, parent);
    default:
        QMessageBox msgBox(QMessageBox::Information, parent->tr("Oops"),
                           parent->tr("No editor is currently available for %1")
                                     .arg(pqGetFriendlyClassName(type)),
                           QMessageBox::Ok, parent);
        msgBox.exec();
        return NULL;
    }
}
