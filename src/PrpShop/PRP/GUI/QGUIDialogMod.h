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

#ifndef _QGUIDIALOGMOD_H
#define _QGUIDIALOGMOD_H

#include "PRP/QCreatable.h"

#include <PRP/GUI/pfGUIDialogMod.h>
#include <QCheckBox>
#include "QGUIControlMod.h"
#include "PRP/QObjLink.h"
#include "PRP/QKeyList.h"

class QGUIDialogMod : public QCreatable
{
    Q_OBJECT

protected:
    QCreatableLink* fSynchObjLink;
    QCheckBox* fModFlagModal;
    QIntEdit* fTagID;
    QIntEdit* fVersion;
    QLineEdit* fName;
    QCreatableLink* fRenderMod;
    QCreatableLink* fProcReceiver;
    QCreatableLink* fSceneNode;
    QGUIColorScheme* fColorScheme;
    QKeyList* fControls;

public:
    QGUIDialogMod(plCreatable* pCre, QWidget* parent = NULL);
    void saveDamage() override;

protected slots:
    void setRenderMod();
    void setProcReceiver();
    void setSceneNode();
    void unsetRenderMod();
    void unsetProcReceiver();
    void unsetSceneNode();
};

#endif
