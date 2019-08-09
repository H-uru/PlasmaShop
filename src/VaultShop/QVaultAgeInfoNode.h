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

#ifndef _QVAULTAGEINFONODE_H
#define _QVAULTAGEINFONODE_H

#include <QLineEdit>
#include <QSpinBox>
#include "QVaultNodeEdit.h"
#include "QLinkLabel.h"

class QVaultAgeInfoNode : public QVaultNodeEdit
{
    Q_OBJECT

protected:
    QSpinBox* fAgeID;
    QLinkLabel* fAgeLink;
    QSpinBox* fAgeCzarID;
    QLinkLabel* fAgeCzarLink;
    QLineEdit* fAgeFilename;
    QLineEdit* fAgeInstanceName;
    QLineEdit* fAgeUserDefinedName;
    QLineEdit* fAgeDescription;
    QLineEdit* fAgeInstanceGuid;

public:
    QVaultAgeInfoNode(QWidget* parent);
    virtual QString getEditorTitle() const { return "Age Info"; }
    virtual plVaultNode saveNode();

protected:
    virtual void IRefreshNode();

protected slots:
    void subscribeAge();
    void subscribeAgeCzar();
};

#endif
