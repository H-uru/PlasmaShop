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
#include "QPlasmaUtils.h"

class QCreatable : public QWidget
{
    Q_OBJECT

protected:
    plCreatable* fCreatable;
    int fForceType;

public:
    QCreatable(plCreatable* pCre, int type, QWidget* parent = NULL);
    bool isMatch(plCreatable* pCre, int type);
    bool compareLocation(const plLocation& loc);
    virtual void saveDamage() = 0;

protected:
    void closeEvent(QCloseEvent* evt) override;
};

QCreatable* pqMakeCreatableForm(plCreatable* pCre, QWidget* parent, int forceType = -1);

#endif
