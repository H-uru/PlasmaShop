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

#ifndef _QVAULTAGENODE_H
#define _QVAULTAGENODE_H

#include <QLineEdit>
#include <QSpinBox>
#include "QVaultNodeEdit.h"

class QVaultAgeNode : public QVaultNodeEdit {
    Q_OBJECT

protected:
    QLineEdit* fAgeInstanceGuid;

public:
    QVaultAgeNode(QWidget* parent);
    virtual QString getEditorTitle() const { return "Age Instance"; }
    virtual plVaultNode saveNode();

protected:
    virtual void IRefreshNode();
};

#endif
