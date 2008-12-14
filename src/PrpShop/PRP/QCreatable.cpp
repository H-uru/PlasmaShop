#include "QCreatable.h"

#include <PRP/KeyedObject/hsKeyedObject.h>
#include <QMessageBox>
#include "../QPlasmaUtils.h"

QCreatable::QCreatable(plCreatable* pCre, QWidget* parent)
          : QMdiSubWindow(parent), fCreatable(pCre)
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

bool QCreatable::isMatch(plCreatable* pCre)
{ return (fCreatable == NULL) ? false : fCreatable == pCre; }

void QCreatable::closeEvent(QCloseEvent* evt)
{
    saveDamage();
}


/***** Creatable Forms -- think QFactory ;) *****/
#include "PRP/Object/QCoordinateInterface.h"

QCreatable* pqMakeCreatableForm(plCreatable* pCre, QWidget* parent)
{
    switch (pCre->ClassIndex()) {
    case kCoordinateInterface:
        return new QCoordinateInterface(pCre, parent);
    default:
        QMessageBox msgBox(QMessageBox::Information, parent->tr("Oops"),
                           parent->tr("No editor is currently available for %1")
                                     .arg(pqGetFriendlyClassName(pCre->ClassIndex())),
                           QMessageBox::Ok, parent);
        msgBox.exec();
        return NULL;
    }
}
