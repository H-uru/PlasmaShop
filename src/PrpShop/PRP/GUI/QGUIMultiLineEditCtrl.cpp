#include "QGUIMultiLineEditCtrl.h"

#include <QGridLayout>
#include "../../Main.h"
#include "../../QKeyDialog.h"

/* QGUIMultiLineEditCtrl */
QGUIMultiLineEditCtrl::QGUIMultiLineEditCtrl(plCreatable* pCre, QWidget* parent)
                     : QCreatable(pCre, kGUIMultiLineEditCtrl, parent)
{
    pfGUIMultiLineEditCtrl* ctrl = (pfGUIMultiLineEditCtrl*)fCreatable;

    fControlModLink = new QCreatableLink(this, false);
    fControlModLink->setText(tr("GUI Control Common Properties"));
    fControlModLink->setCreatable(ctrl);
    fControlModLink->setForceType(kGUIControlMod);

    fScrollCtrl = new QCreatableLink(this);
    fScrollCtrl->setKey(ctrl->getScrollCtrl());
    fScrollCtrl->setText(ctrl->getScrollCtrl().Exists()
                           ? ~ctrl->getScrollCtrl()->getName()
                           : "(None)");

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(fControlModLink, 0, 0, 1, 2);
    layout->addWidget(new QLabel(tr("Scroll Ctrl:"), this), 1, 0);
    layout->addWidget(fScrollCtrl, 1, 1);

    connect(fScrollCtrl, SIGNAL(addObject()), this, SLOT(setScrollCtrl()));
    connect(fScrollCtrl, SIGNAL(delObject()), this, SLOT(unsetScrollCtrl()));
}

void QGUIMultiLineEditCtrl::saveDamage()
{ }

void QGUIMultiLineEditCtrl::setScrollCtrl()
{
    pfGUIMultiLineEditCtrl* ctrl = (pfGUIMultiLineEditCtrl*)fCreatable;
    QFindKeyDialog dlg(this);
    if (ctrl->getScrollCtrl().Exists())
        dlg.init(PrpShopMain::ResManager(), ctrl->getScrollCtrl());
    else
        dlg.init(PrpShopMain::ResManager(), ctrl->getKey()->getLocation(), kGUIUpDownPairMod);
    if (dlg.exec() == QDialog::Accepted) {
        ctrl->setScrollCtrl(dlg.selection());
        fScrollCtrl->setKey(ctrl->getScrollCtrl());
        fScrollCtrl->setText(~ctrl->getScrollCtrl()->getName());
    }
}

void QGUIMultiLineEditCtrl::unsetScrollCtrl()
{
    pfGUIMultiLineEditCtrl* ctrl = (pfGUIMultiLineEditCtrl*)fCreatable;
    ctrl->setScrollCtrl(plKey());
    fScrollCtrl->setCreatable(NULL);
    fScrollCtrl->setText("(None)");
}
