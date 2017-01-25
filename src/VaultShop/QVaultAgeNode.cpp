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

#include "QVaultAgeNode.h"

#include <QLabel>
#include <QGridLayout>
#include <QSpacerItem>
#include <QRegExpValidator>
#include <Vault/plVaultNodeTypes.h>

QVaultAgeNode::QVaultAgeNode(QWidget* parent)
             : QVaultNodeEdit(parent)
{
    fAgeInstanceGuid = new QLineEdit("0000000000000000", this);
    fAgeInstanceGuid->setValidator(new QRegExpValidator(QRegExp("[0-9A-Fa-f]{16}"), fAgeInstanceGuid));

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->setHorizontalSpacing(8);
    layout->setVerticalSpacing(8);
    layout->addWidget(new QLabel(tr("Age Instance GUID:"), this), 0, 0);
    layout->addWidget(fAgeInstanceGuid, 0, 1);
    layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding), 1, 0);
}

plVaultNode QVaultAgeNode::saveNode()
{
    plVaultAgeNode* age = fNode.upcastToAgeNode();
    if (age == NULL)
        return plVaultNode();

    age->setAgeInstanceGuid(plServerGuid::FromString(qstr2st(fAgeInstanceGuid->text())));
    return fNode;
}

void QVaultAgeNode::IRefreshNode()
{
    plVaultAgeNode* age = fNode.upcastToAgeNode();
    if (age == NULL)
        return;

    fAgeInstanceGuid->setText(st2qstr(age->getAgeInstanceGuid().toString()));
}
