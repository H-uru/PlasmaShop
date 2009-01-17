#include "QObjLink.h"

#include <QHBoxLayout>
#include <QMouseEvent>
#include <QMenu>
#include "../Main.h"

QCreatableLink::QCreatableLink(QWidget* parent, bool canEdit)
              : QWidget(parent), fCreatable(NULL), fEditLabel(NULL),
                fForceType(-1)
{
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(4);

    fObjLabel = new QLinkLabel(this);
    connect(fObjLabel, SIGNAL(activated()), this, SLOT(objectActivated()));
    layout->addWidget(fObjLabel);
    layout->addStretch();
    if (canEdit) {
        fEditLabel = new QLinkLabel("...", this);
        connect(fEditLabel, SIGNAL(activated()), this, SLOT(menuRequested()));
        layout->addWidget(fEditLabel);
    }

    setCreatable(NULL);
}

void QCreatableLink::setText(const QString& str)
{
    fObjLabel->setText(str);
}

void QCreatableLink::setCreatable(plCreatable* pCre)
{
    fCreatable = pCre;
    fObjLabel->setEnabled(fCreatable != NULL);
}

void QCreatableLink::setKey(plKey key)
{
    fCreatable = (key.Exists() && key.isLoaded()) ? key->getObj() : NULL;
    fObjLabel->setEnabled(fCreatable != NULL);
}

void QCreatableLink::setForceType(short forceType)
{ fForceType = forceType; }

void QCreatableLink::objectActivated()
{
    if (fCreatable != NULL)
        PrpShopMain::Instance()->editCreatable(fCreatable, fForceType);
}

void QCreatableLink::menuRequested()
{
    QMenu menu(this);

    QAction* setObjItem = menu.addAction(tr("Set Object..."));
    QAction* delObjItem = menu.addAction(tr("Remove Object"));

    if (fCreatable == NULL)
        delObjItem->setEnabled(false);

    QPoint pos = fEditLabel->mapToGlobal(QPoint(0, fEditLabel->height()));
    QAction* sel = menu.exec(pos);
    if (sel == setObjItem)
        emit addObject();
    else if (sel == delObjItem)
        emit delObject();
}
