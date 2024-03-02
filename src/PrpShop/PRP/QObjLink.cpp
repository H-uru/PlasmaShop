/* This file is part of PlasmaShop.
 *
 * PlasmaShop is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PlasmaShop is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PlasmaShop.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "QObjLink.h"

#include <QHBoxLayout>
#include <QMouseEvent>
#include <QMenu>
#include "Main.h"

/* QCreatableLink */
QCreatableLink::QCreatableLink(QWidget* parent, bool canEdit)
    : QWidget(parent), fCreatable(), fEditLabel(), fForceType(-1)
{
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(4);

    fObjLabel = new QLinkLabel(this);
    connect(fObjLabel, &QLinkLabel::activated, this, &QCreatableLink::objectActivated);
    layout->addWidget(fObjLabel);
    layout->addStretch();
    if (canEdit) {
        fEditLabel = new QLinkLabel("...", this);
        connect(fEditLabel, &QLinkLabel::activated, this, &QCreatableLink::menuRequested);
        layout->addWidget(fEditLabel);
    }

    fCreatable = NULL;
    fObjLabel->setEnabled(false);
}

void QCreatableLink::setCreatable(plCreatable* pCre, const QString& text)
{
    fCreatable = pCre;
    fObjLabel->setEnabled(fCreatable != NULL);
    if (!text.isEmpty())
        setText(text);
}

void QCreatableLink::setKey(plKey key, bool updateText)
{
    if (key.Exists()) {
        fCreatable = key.isLoaded() ? key->getObj() : NULL;
        fObjLabel->setEnabled(true);
        if (updateText)
            setText(pqFormatKeyName(key));
    } else {
        fCreatable = NULL;
        fObjLabel->setEnabled(false);
        if (updateText)
            setText("(None)");
    }
}

void QCreatableLink::objectActivated()
{
    PrpShopMain::Instance()->editCreatable(fCreatable, fForceType);
}

void QCreatableLink::menuRequested()
{
    QMenu menu(this);

    QAction* setObjItem = menu.addAction(tr("Set Object..."));
    QAction* delObjItem = menu.addAction(tr("Remove Object"));

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
