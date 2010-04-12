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

#ifndef _QVAULTPLAYERINFONODE_H
#define _QVAULTPLAYERINFONODE_H

#include <QLineEdit>
#include <QSpinBox>
#include "QVaultNodeEdit.h"
#include "../QLinkLabel.h"

class QVaultPlayerInfoNode : public QVaultNodeEdit {
    Q_OBJECT

protected:
    QLineEdit* fPlayerName;
    QSpinBox* fPlayerID;
    QLinkLabel* fPlayerLink;

public:
    QVaultPlayerInfoNode(QWidget* parent);
    virtual QString getEditorTitle() const;
    virtual plVaultNode saveNode();

protected:
    virtual void IRefreshNode();

protected slots:
    void subscribePlayer();
};

#endif
