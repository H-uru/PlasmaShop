#include "QObjLink.h"

#include <QHBoxLayout>
#include <QMouseEvent>
#include <QMenu>
#include "../Main.h"

/* QLinkLabel */
QLinkLabel::QLinkLabel(QWidget* parent)
          : QLabel(parent), fInClick(false)
{ init(); }

QLinkLabel::QLinkLabel(const QString& text, QWidget* parent)
          : QLabel(text, parent), fInClick(false)
{ init(); }

void QLinkLabel::init()
{
    QFont lblFont = font();
    lblFont.setUnderline(true);
    setFont(lblFont);

    QPalette lblPal = palette();
    lblPal.setColor(QPalette::Active, QPalette::WindowText, QColor(0, 0, 255));
    lblPal.setColor(QPalette::Inactive, QPalette::WindowText, QColor(0, 0, 255));
    setPalette(lblPal);

    setCursor(Qt::PointingHandCursor);
}

void QLinkLabel::mousePressEvent(QMouseEvent* evt)
{
    fInClick = rect().contains(evt->pos());
}

void QLinkLabel::mouseReleaseEvent(QMouseEvent* evt)
{
    if (!fInClick) return;
    fInClick = false;

    if (rect().contains(evt->pos()))
        emit activated();
}


/* QCreatableLink */
QCreatableLink::QCreatableLink(QWidget* parent, bool canEdit)
              : QWidget(parent), fCreatable(NULL), fEditLabel(NULL),
                fForceType(-1)
{
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(4);

    fObjLabel = new QLinkLabel(this);
    QObject::connect(fObjLabel, SIGNAL(activated()), this, SLOT(objectActivated()));
    layout->addWidget(fObjLabel);
    layout->addStretch();
    if (canEdit) {
        fEditLabel = new QLinkLabel("...", this);
        QObject::connect(fEditLabel, SIGNAL(activated()), this, SLOT(menuRequested()));
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
