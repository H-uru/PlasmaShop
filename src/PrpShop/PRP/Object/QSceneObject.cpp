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
#include "QPlasmaUtils.h"
#include "QKeyDialog.h"
#include "Main.h"

QSceneObject::QSceneObject(plCreatable* pCre, QWidget* parent)
    : QCreatable(pCre, kSceneObject, parent)
{
    plSceneObject* obj = plSceneObject::Convert(fCreatable);

    fSynchObjLink = new QCreatableLink(this, false);
    fSynchObjLink->setCreatable(obj, tr("Synch Flags"));
    fSynchObjLink->setForceType(kSynchedObject);

    QGroupBox* grpInterfaces = new QGroupBox(tr("Core Interfaces"), this);
    QGridLayout* layInterfaces = new QGridLayout(grpInterfaces);
    layInterfaces->setVerticalSpacing(2);
    fDrawIntfLink = new QCreatableLink(grpInterfaces);
    fDrawIntfLink->setKey(obj->getDrawInterface(), false);
    fDrawIntfLink->setText(pqGetFriendlyClassName(kDrawInterface));
    fSimIntfLink = new QCreatableLink(grpInterfaces);
    fSimIntfLink->setKey(obj->getSimInterface(), false);
    fSimIntfLink->setText(pqGetFriendlyClassName(kSimulationInterface));
    fCoordIntfLink = new QCreatableLink(grpInterfaces);
    fCoordIntfLink->setKey(obj->getCoordInterface(), false);
    fCoordIntfLink->setText(pqGetFriendlyClassName(kCoordinateInterface));
    fAudioIntfLink = new QCreatableLink(grpInterfaces);
    fAudioIntfLink->setKey(obj->getAudioInterface(), false);
    fAudioIntfLink->setText(pqGetFriendlyClassName(kAudioInterface));
    layInterfaces->addWidget(fDrawIntfLink, 0, 0);
    layInterfaces->addWidget(fSimIntfLink, 1, 0);
    layInterfaces->addWidget(fCoordIntfLink, 2, 0);
    layInterfaces->addWidget(fAudioIntfLink, 3, 0);

    QTabWidget* objTab = new QTabWidget(this);
    fInterfaceList = new QKeyList(obj->getKey(), objTab);
    fModifierList = new QKeyList(obj->getKey(), objTab);
    for (size_t i=0; i<obj->getInterfaces().size(); i++)
        fInterfaceList->addKey(obj->getInterfaces()[i]);
    for (size_t i=0; i<obj->getModifiers().size(); i++)
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

    connect(fDrawIntfLink, &QCreatableLink::addObject, this, &QSceneObject::setDraw);
    connect(fDrawIntfLink, &QCreatableLink::delObject, this, &QSceneObject::unsetDraw);
    connect(fSimIntfLink, &QCreatableLink::addObject, this, &QSceneObject::setSim);
    connect(fSimIntfLink, &QCreatableLink::delObject, this, &QSceneObject::unsetSim);
    connect(fCoordIntfLink, &QCreatableLink::addObject, this, &QSceneObject::setCoord);
    connect(fCoordIntfLink, &QCreatableLink::delObject, this, &QSceneObject::unsetCoord);
    connect(fAudioIntfLink, &QCreatableLink::addObject, this, &QSceneObject::setAudio);
    connect(fAudioIntfLink, &QCreatableLink::delObject, this, &QSceneObject::unsetAudio);
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
        fDrawIntfLink->setKey(obj->getDrawInterface(), false);
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
        fSimIntfLink->setKey(obj->getSimInterface(), false);
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
        fCoordIntfLink->setKey(obj->getCoordInterface(), false);
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
        fAudioIntfLink->setKey(obj->getAudioInterface(), false);
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
