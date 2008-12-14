#include "QCreatable.h"
#include "QPlasmaUtils.h"
#include <PRP/KeyedObject/hsKeyedObject.h>

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
{ return fCreatable == pCre; }
