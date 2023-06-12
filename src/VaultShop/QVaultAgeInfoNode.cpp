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

#include "QVaultAgeInfoNode.h"

#include <QGridLayout>
#include <QSpacerItem>
#include <QRegularExpressionValidator>
#include <Vault/plVaultNodeTypes.h>

QVaultAgeInfoNode::QVaultAgeInfoNode(QWidget* parent)
    : QVaultNodeEdit(parent)
{
    fAgeID = new QSpinBox(this);
    fAgeID->setRange(0, 0x7FFFFFFF);
    fAgeLink = new QLinkLabel(tr("Subscribe"), this);
    fAgeCzarID = new QSpinBox(this);
    fAgeCzarID->setRange(0, 0x7FFFFFFF);
    fAgeCzarLink = new QLinkLabel(tr("Subscribe"), this);
    fAgeFilename = new QLineEdit(this);
    fAgeInstanceName = new QLineEdit(this);
    fAgeUserDefinedName = new QLineEdit(this);
    fAgeDescription = new QLineEdit(this);
    fAgeInstanceGuid = new QLineEdit("0000000000000000", this);
    fAgeInstanceGuid->setValidator(new QRegularExpressionValidator(
            QRegularExpression("[0-9A-Fa-f]{16}"), fAgeInstanceGuid));

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->setHorizontalSpacing(8);
    layout->setVerticalSpacing(8);
    layout->addWidget(new QLabel(tr("Age ID:"), this), 0, 0);
    layout->addWidget(fAgeID, 0, 1);
    layout->addWidget(fAgeLink, 0, 2);
    layout->addWidget(new QLabel(tr("Age Czar ID:"), this), 1, 0);
    layout->addWidget(fAgeCzarID, 1, 1);
    layout->addWidget(fAgeCzarLink, 1, 2);
    layout->addWidget(new QLabel(tr("Age Filename:"), this), 2, 0);
    layout->addWidget(fAgeFilename, 2, 1, 1, 2);
    layout->addWidget(new QLabel(tr("Age Instance Name:"), this), 3, 0);
    layout->addWidget(fAgeInstanceName, 3, 1, 1, 2);
    layout->addWidget(new QLabel(tr("Age User Defined Name:"), this), 4, 0);
    layout->addWidget(fAgeUserDefinedName, 4, 1, 1, 2);
    layout->addWidget(new QLabel(tr("Age Description:"), this), 5, 0);
    layout->addWidget(fAgeDescription, 5, 1, 1, 2);
    layout->addWidget(new QLabel(tr("Age Instance GUID:"), this), 6, 0);
    layout->addWidget(fAgeInstanceGuid, 6, 1, 1, 2);
    layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding), 7, 0, 1, 2);

    connect(fAgeLink, &QLinkLabel::activated, this, &QVaultAgeInfoNode::subscribeAge);
    connect(fAgeCzarLink, &QLinkLabel::activated, this, &QVaultAgeInfoNode::subscribeAgeCzar);
}

plVaultNode QVaultAgeInfoNode::saveNode()
{
    plVaultAgeInfoNode* age = fNode.upcastToAgeInfoNode();
    if (age == NULL)
        return plVaultNode();

    age->setAgeID(fAgeID->value());
    age->setAgeCzarID(fAgeCzarID->value());
    age->setAgeFilename(qstr2st(fAgeFilename->text()));
    age->setAgeInstanceName(qstr2st(fAgeInstanceName->text()));
    age->setAgeUserDefinedName(qstr2st(fAgeUserDefinedName->text()));
    age->setAgeDescription(qstr2st(fAgeDescription->text()));
    age->setAgeInstanceGuid(plServerGuid::FromString(qstr2st(fAgeInstanceGuid->text())));
    return fNode;
}

void QVaultAgeInfoNode::IRefreshNode()
{
    plVaultAgeInfoNode* age = fNode.upcastToAgeInfoNode();
    if (age == NULL)
        return;

    fAgeID->setValue(age->getAgeID());
    fAgeCzarID->setValue(age->getAgeCzarID());
    fAgeFilename->setText(st2qstr(age->getAgeFilename()));
    fAgeInstanceName->setText(st2qstr(age->getAgeInstanceName()));
    fAgeUserDefinedName->setText(st2qstr(age->getAgeUserDefinedName()));
    fAgeDescription->setText(st2qstr(age->getAgeDescription()));
    fAgeInstanceGuid->setText(st2qstr(age->getAgeInstanceGuid().toString()));
}

void QVaultAgeInfoNode::subscribeAge()
{
    emit subscribe((unsigned int)fAgeID->value());
}

void QVaultAgeInfoNode::subscribeAgeCzar()
{
    emit subscribe((unsigned int)fAgeCzarID->value());
}
