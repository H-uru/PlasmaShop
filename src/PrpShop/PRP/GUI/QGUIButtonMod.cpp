#include "QGUIButtonMod.h"

#include <QTabWidget>
#include <QGridLayout>
#include "../../Main.h"
#include "../../QKeyDialog.h"

/* QGUIButtonMod */
QGUIButtonMod::QGUIButtonMod(plCreatable* pCre, QWidget* parent)
             : QCreatable(pCre, kGUIButtonMod, parent)
{
    pfGUIButtonMod* ctrl = (pfGUIButtonMod*)fCreatable;

    fControlModLink = new QCreatableLink(this, false);
    fControlModLink->setText(tr("GUI Control Common Properties"));
    fControlModLink->setCreatable(ctrl);
    fControlModLink->setForceType(kGUIControlMod);

    QTabWidget* animTabs = new QTabWidget(this);
    QWidget* animationGrp = new QWidget(animTabs);
    fAnimName = new QLineEdit(animationGrp);
    fAnimName->setText(~ctrl->getAnimationName());
    fAnimationKeys = new QKeyList(animationGrp);
    for (size_t i=0; i<ctrl->getNumAnimationKeys(); i++)
        fAnimationKeys->addKey(ctrl->getAnimationKey(i));
    QGridLayout* animationLayout = new QGridLayout(animationGrp);
    animationLayout->addWidget(new QLabel(tr("Anim Name:"), animationGrp), 0, 0);
    animationLayout->addWidget(fAnimName, 0, 1);
    animationLayout->addWidget(fAnimationKeys, 1, 0, 1, 2);

    QWidget* mouseOverAnimGrp = new QWidget(animTabs);
    fMouseOverAnimName = new QLineEdit(mouseOverAnimGrp);
    fMouseOverAnimName->setText(~ctrl->getMouseOverAnimName());
    fMouseOverAnimKeys = new QKeyList(mouseOverAnimGrp);
    for (size_t i=0; i<ctrl->getNumMouseOverKeys(); i++)
        fMouseOverAnimKeys->addKey(ctrl->getMouseOverKey(i));
    QGridLayout* mouseOverAnimLayout = new QGridLayout(mouseOverAnimGrp);
    mouseOverAnimLayout->addWidget(new QLabel(tr("Anim Name:"), mouseOverAnimGrp), 0, 0);
    mouseOverAnimLayout->addWidget(fMouseOverAnimName, 0, 1);
    mouseOverAnimLayout->addWidget(fMouseOverAnimKeys, 1, 0, 1, 2);

    animTabs->addTab(animationGrp, tr("Animation"));
    animTabs->addTab(mouseOverAnimGrp, tr("MouseOver Animation"));

    fNotifyType = new QComboBox(this);
    fNotifyType->addItems(QStringList() << tr("Up Only") << tr("Down Only") << tr("Both"));
    fNotifyType->setCurrentIndex(ctrl->getNotifyType());

    fDraggable = new QCreatableLink(this);
    fDraggable->setKey(ctrl->getDraggable());
    fDraggable->setText(ctrl->getDraggable().Exists()
                        ? ~ctrl->getDraggable()->getName()
                        : "(None)");

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(fControlModLink, 0, 0, 1, 2);
    layout->addWidget(animTabs, 1, 0, 1, 2);
    layout->addWidget(new QLabel(tr("Notify Type:"), this), 2, 0);
    layout->addWidget(fNotifyType, 2, 1);
    layout->addWidget(new QLabel(tr("Draggable:"), this), 3, 0);
    layout->addWidget(fDraggable, 3, 1);

    connect(fDraggable, SIGNAL(addObject()), this, SLOT(setDraggable()));
    connect(fDraggable, SIGNAL(delObject()), this, SLOT(unsetDraggable()));

    fAnimationKeys->adjustColumns();
    fMouseOverAnimKeys->adjustColumns();
}

void QGUIButtonMod::saveDamage()
{
    pfGUIButtonMod* ctrl = (pfGUIButtonMod*)fCreatable;

    ctrl->setAnimationName(~fAnimName->text());
    ctrl->setMouseOverAnimName(~fMouseOverAnimName->text());

    ctrl->clearAnimationKeys();
    QList<plKey> animKeys = fAnimationKeys->keys();
    while (!animKeys.isEmpty())
        ctrl->addAnimationKey(animKeys.takeFirst());

    ctrl->clearMouseOverKeys();
    QList<plKey> mouseOverKeys = fMouseOverAnimKeys->keys();
    while (!mouseOverKeys.isEmpty())
        ctrl->addMouseOverKey(mouseOverKeys.takeFirst());

    ctrl->setNotifyType(fNotifyType->currentIndex());
}

void QGUIButtonMod::setDraggable()
{
    pfGUIButtonMod* ctrl = (pfGUIButtonMod*)fCreatable;
    QFindKeyDialog dlg(this);
    if (ctrl->getDraggable().Exists())
        dlg.init(PrpShopMain::ResManager(), ctrl->getDraggable());
    else
        dlg.init(PrpShopMain::ResManager(), ctrl->getKey()->getLocation(), kGUIDraggableMod);
    if (dlg.exec() == QDialog::Accepted) {
        ctrl->setDraggable(dlg.selection());
        fDraggable->setKey(ctrl->getDraggable());
        fDraggable->setText(~ctrl->getDraggable()->getName());
    }
}

void QGUIButtonMod::unsetDraggable()
{
    pfGUIButtonMod* ctrl = (pfGUIButtonMod*)fCreatable;
    ctrl->setDraggable(plKey());
    fDraggable->setCreatable(NULL);
    fDraggable->setText("(None)");
}
