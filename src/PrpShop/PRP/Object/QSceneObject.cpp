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

#include "QSceneObject.h"

#include <QGroupBox>
#include <QTabWidget>
#include <QGridLayout>
#include "../../QPlasmaUtils.h"
#include "../../QKeyDialog.h"
#include "../../Main.h"

QSceneObject::QSceneObject(plCreatable* pCre, QWidget* parent)
            : QCreatable(pCre, kSceneObject, parent)
{
    plSceneObject* obj = plSceneObject::Convert(fCreatable);

    fSynchObjLink = new QCreatableLink(this, false);
    fSynchObjLink->setText(tr("Synch Flags"));
    fSynchObjLink->setCreatable(obj);
    fSynchObjLink->setForceType(kSynchedObject);

    QGroupBox* grpInterfaces = new QGroupBox(tr("Core Interfaces"), this);
    QGridLayout* layInterfaces = new QGridLayout(grpInterfaces);
    layInterfaces->setVerticalSpacing(2);
    fDrawIntfLink = new QCreatableLink(grpInterfaces);
    fDrawIntfLink->setText(pqGetFriendlyClassName(kDrawInterface));
    fDrawIntfLink->setKey(obj->getDrawInterface());
    fSimIntfLink = new QCreatableLink(grpInterfaces);
    fSimIntfLink->setText(pqGetFriendlyClassName(kSimulationInterface));
    fSimIntfLink->setKey(obj->getSimInterface());
    fCoordIntfLink = new QCreatableLink(grpInterfaces);
    fCoordIntfLink->setText(pqGetFriendlyClassName(kCoordinateInterface));
    fCoordIntfLink->setKey(obj->getCoordInterface());
    fAudioIntfLink = new QCreatableLink(grpInterfaces);
    fAudioIntfLink->setText(pqGetFriendlyClassName(kAudioInterface));
    fAudioIntfLink->setKey(obj->getAudioInterface());
    layInterfaces->addWidget(fDrawIntfLink, 0, 0);
    layInterfaces->addWidget(fSimIntfLink, 1, 0);
    layInterfaces->addWidget(fCoordIntfLink, 2, 0);
    layInterfaces->addWidget(fAudioIntfLink, 3, 0);

    QTabWidget* objTab = new QTabWidget(this);
    fInterfaceList = new QKeyList(obj->getKey(), objTab);
    fModifierList = new QKeyList(obj->getKey(), objTab);
    for (size_t i=0; i<obj->getInterfaces().getSize(); i++)
        fInterfaceList->addKey(obj->getInterfaces()[i]);
    for (size_t i=0; i<obj->getModifiers().getSize(); i++)
        fModifierList->addKey(obj->getModifiers()[i]);
    objTab->addTab(fInterfaceList, tr("Interfaces"));
    objTab->addTab(fModifierList, tr("Modifiers"));

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(fSynchObjLink, 0, 0);
    layout->addWidget(grpInterfaces, 1, 0);
    layout->addWidget(objTab, 2, 0);
    fInterfaceList->adjustColumns();
    fModifierList->adjustColumns();

    connect(fDrawIntfLink, SIGNAL(addObject()), this, SLOT(setDraw()));
    connect(fDrawIntfLink, SIGNAL(delObject()), this, SLOT(unsetDraw()));
    connect(fSimIntfLink, SIGNAL(addObject()), this, SLOT(setSim()));
    connect(fSimIntfLink, SIGNAL(delObject()), this, SLOT(unsetSim()));
    connect(fCoordIntfLink, SIGNAL(addObject()), this, SLOT(setCoord()));
    connect(fCoordIntfLink, SIGNAL(delObject()), this, SLOT(unsetCoord()));
    connect(fAudioIntfLink, SIGNAL(addObject()), this, SLOT(setAudio()));
    connect(fAudioIntfLink, SIGNAL(delObject()), this, SLOT(unsetAudio()));
}

void QSceneObject::saveDamage()
{
    plSceneObject* obj = plSceneObject::Convert(fCreatable);

    obj->clearInterfaces();
    QList<plKey> interfaces = fInterfaceList->keys();
    while (!interfaces.isEmpty())
        obj->addInterface(interfaces.takeFirst());

    obj->clearModifiers();
    QList<plKey> modifiers = fModifierList->keys();
    while (!modifiers.isEmpty())
        obj->addModifier(modifiers.takeFirst());
}

void QSceneObject::setDraw()
{
    plSceneObject* obj = plSceneObject::Convert(fCreatable);
    QFindKeyDialog dlg(this);
    if (obj->getDrawInterface().Exists())
        dlg.init(PrpShopMain::ResManager(), obj->getDrawInterface());
    else
        dlg.init(PrpShopMain::ResManager(), obj->getKey()->getLocation(), kDrawInterface);
    if (dlg.exec() == QDialog::Accepted) {
        obj->setDrawInterface(dlg.selection());
        fDrawIntfLink->setKey(obj->getDrawInterface());
    }
}

void QSceneObject::setSim()
{
    plSceneObject* obj = plSceneObject::Convert(fCreatable);
    QFindKeyDialog dlg(this);
    if (obj->getSimInterface().Exists())
        dlg.init(PrpShopMain::ResManager(), obj->getSimInterface());
    else
        dlg.init(PrpShopMain::ResManager(), obj->getKey()->getLocation(), kSimulationInterface);
    if (dlg.exec() == QDialog::Accepted) {
        obj->setSimInterface(dlg.selection());
        fSimIntfLink->setKey(obj->getSimInterface());
    }
}

void QSceneObject::setCoord()
{
    plSceneObject* obj = plSceneObject::Convert(fCreatable);
    QFindKeyDialog dlg(this);
    if (obj->getCoordInterface().Exists())
        dlg.init(PrpShopMain::ResManager(), obj->getCoordInterface());
    else
        dlg.init(PrpShopMain::ResManager(), obj->getKey()->getLocation(), kCoordinateInterface);
    if (dlg.exec() == QDialog::Accepted) {
        obj->setCoordInterface(dlg.selection());
        fCoordIntfLink->setKey(obj->getCoordInterface());
    }
}

void QSceneObject::setAudio()
{
    plSceneObject* obj = plSceneObject::Convert(fCreatable);
    QFindKeyDialog dlg(this);
    if (obj->getAudioInterface().Exists())
        dlg.init(PrpShopMain::ResManager(), obj->getAudioInterface());
    else
        dlg.init(PrpShopMain::ResManager(), obj->getKey()->getLocation(), kAudioInterface);
    if (dlg.exec() == QDialog::Accepted) {
        obj->setAudioInterface(dlg.selection());
        fAudioIntfLink->setKey(obj->getAudioInterface());
    }
}

void QSceneObject::unsetDraw()
{
    plSceneObject* obj = plSceneObject::Convert(fCreatable);
    obj->setDrawInterface(plKey());
    fDrawIntfLink->setCreatable(NULL);
}

void QSceneObject::unsetSim()
{
    plSceneObject* obj = plSceneObject::Convert(fCreatable);
    obj->setSimInterface(plKey());
    fSimIntfLink->setCreatable(NULL);
}

void QSceneObject::unsetCoord()
{
    plSceneObject* obj = plSceneObject::Convert(fCreatable);
    obj->setCoordInterface(plKey());
    fCoordIntfLink->setCreatable(NULL);
}

void QSceneObject::unsetAudio()
{
    plSceneObject* obj = plSceneObject::Convert(fCreatable);
    obj->setAudioInterface(plKey());
    fAudioIntfLink->setCreatable(NULL);
}
