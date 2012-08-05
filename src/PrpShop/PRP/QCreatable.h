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

#ifndef _QCREATABLE_H
#define _QCREATABLE_H

#include <QWidget>
#include <ResManager/plResManager.h>
#include "../QPlasmaUtils.h"

class QCreatable : public QWidget {
    Q_OBJECT

protected:
    plCreatable* fCreatable;
    short fForceType;

public:
    QCreatable(plCreatable* pCre, short type, QWidget* parent = NULL);
    bool isMatch(plCreatable* pCre, short type);
    bool compareLocation(const plLocation& loc);
    virtual void saveDamage() = 0;

protected:
    virtual void closeEvent(QCloseEvent* evt);
};

QCreatable* pqMakeCreatableForm(plCreatable* pCre, QWidget* parent, short forceType = -1);

#endif
