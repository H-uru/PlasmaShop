#include "QObjLink.h"

#include <QHBoxLayout>
#include <QMouseEvent>
#include <QMenu>
#include "../Main.h"

/* QCreatableLink */
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


/* QControllerLink */
QControllerLink::QControllerLink(const QString& text, QWidget* parent, bool canEdit)
               : QCreatableLink(parent, canEdit)
{
    setText(text);
}

void QControllerLink::menuRequested()
{
    QMenu menu(this);

    QMenu* m_createTypes = new QMenu(tr("Create..."), &menu);
    QAction* newEase = m_createTypes->addAction(tr("Ease Controller"));
    QAction* newMat33 = m_createTypes->addAction(tr("Matrix33 Controller"));
    QAction* newMat44 = m_createTypes->addAction(tr("Matrix44 Controller"));
    QAction* newPoint3 = m_createTypes->addAction(tr("Point3 Controller"));
    QAction* newQuat = m_createTypes->addAction(tr("Quat Controller"));
    QAction* newScalar = m_createTypes->addAction(tr("Scalar Controller"));
    QAction* newScaleValue = m_createTypes->addAction(tr("Scale Value Controller"));
    m_createTypes->addSeparator();
    QAction* newSimpleScale = m_createTypes->addAction(tr("Simple Scale Controller"));
    QAction* newSimpleRot = m_createTypes->addAction(tr("Simple Rot Controller"));
    QAction* newCompoundRot = m_createTypes->addAction(tr("Compound Rot Controller"));
    QAction* newSimplePos = m_createTypes->addAction(tr("Simple Pos Controller"));
    QAction* newCompoundPos = m_createTypes->addAction(tr("Compound Pos Controller"));
    m_createTypes->addSeparator();
    QAction* newTM = m_createTypes->addAction(tr("TM Controller"));
    QAction* newCompound = m_createTypes->addAction(tr("Compound Controller"));
    QAction* newLeaf = m_createTypes->addAction(tr("MOUL/Myst 5 Controller"));

    menu.addMenu(m_createTypes);
    QAction* delObjItem = menu.addAction(tr("Delete"));

    if (fCreatable == NULL)
        delObjItem->setEnabled(false);

    QPoint pos = fEditLabel->mapToGlobal(QPoint(0, fEditLabel->height()));
    QAction* sel = menu.exec(pos);
    if (sel == delObjItem)
        emit delObject();
    else if (sel == newEase)
        emit createObject(kEaseController);
    else if (sel == newMat33)
        emit createObject(kMatrix33Controller);
    else if (sel == newMat44)
        emit createObject(kMatrix44Controller);
    else if (sel == newPoint3)
        emit createObject(kPoint3Controller);
    else if (sel == newQuat)
        emit createObject(kQuatController);
    else if (sel == newScalar)
        emit createObject(kScalarController);
    else if (sel == newScaleValue)
        emit createObject(kScaleValueController);
    else if (sel == newSimpleScale)
        emit createObject(kSimpleScaleController);
    else if (sel == newSimpleRot)
        emit createObject(kSimpleRotController);
    else if (sel == newCompoundRot)
        emit createObject(kCompoundRotController);
    else if (sel == newSimplePos)
        emit createObject(kSimplePosController);
    else if (sel == newCompoundPos)
        emit createObject(kCompoundPosController);
    else if (sel == newTM)
        emit createObject(kTMController);
    else if (sel == newCompound)
        emit createObject(kCompoundController);
    else if (sel == newLeaf)
        emit createObject(kLeafController);
}
