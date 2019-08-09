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

#ifndef _QAVLADDERMOD_H
#define _QAVLADDERMOD_H

#include "PRP/QCreatable.h"

#include <PRP/Avatar/plLadderModifier.h>
#include <QCheckBox>
#include <QComboBox>
#include <QButtonGroup>
#include "PRP/QVector3.h"

class QAvLadderMod : public QCreatable
{
    Q_OBJECT

protected:
    enum { kDirectionDown, kDirectionUp };
    QComboBox* fType;
    QIntEdit* fLoops;
    QCheckBox* fEnabled;
    QButtonGroup* fGoingUp;
    QVector3* fLadderView;

public:
    QAvLadderMod(plCreatable* pCre, QWidget* parent = NULL);
    virtual void saveDamage();
};

#endif
