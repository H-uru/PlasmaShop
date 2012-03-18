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

#ifndef _QVAULTLINKNODE_H
#define _QVAULTLINKNODE_H

#include <QCheckBox>
#include <QLineEdit>
#include "QVaultNodeEdit.h"

class QVaultLinkNode : public QVaultNodeEdit {
    Q_OBJECT

protected:
    QCheckBox* fUnlocked;
    QCheckBox* fVolatile;
    QLineEdit* fSpawnPoints;

public:
    QVaultLinkNode(QWidget* parent);
    virtual QString getEditorTitle() const { return "Age Link"; }
    virtual plVaultNode saveNode();

protected:
    virtual void IRefreshNode();
};

#endif
