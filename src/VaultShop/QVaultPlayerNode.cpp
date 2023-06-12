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

#include "QVaultPlayerNode.h"

#include <QLabel>
#include <QGridLayout>
#include <QSpacerItem>
#include <QRegularExpressionValidator>
#include <QCompleter>
#include <Vault/plVaultNodeTypes.h>

QVaultPlayerNode::QVaultPlayerNode(QWidget* parent)
    : QVaultNodeEdit(parent)
{
    fPlayerName = new QLineEdit(this);
    fAvatarShape = new QLineEdit(this);
    fAccountUUID = new QLineEdit(this);
    fAccountUUID->setValidator(new QRegularExpressionValidator(
            QRegularExpression("[0-9A-Fa-f]{8}-[0-9A-Fa-f]{4}-[0-9A-Fa-f]{4}-[0-9A-Fa-f]{4}-[0-9A-Fa-f]{12}"),
            fAccountUUID));

    QStringList builtInAvatars;
    builtInAvatars << "Female" << "Male" << "DrWatson" << "Engberg" << "Kodama"
                   << "RandMiller" << "Sutherland" << "Victor" << "Yeesha"
                   << "YeeshaNoGlow" << "Zandi";
    QCompleter* shapeComplete = new QCompleter(builtInAvatars, fAvatarShape);
    shapeComplete->setCaseSensitivity(Qt::CaseInsensitive);
    fAvatarShape->setCompleter(shapeComplete);

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->setHorizontalSpacing(8);
    layout->setVerticalSpacing(8);
    layout->addWidget(new QLabel(tr("Player Name:"), this), 0, 0);
    layout->addWidget(fPlayerName, 0, 1);
    layout->addWidget(new QLabel(tr("Avatar Shape:"), this), 1, 0);
    layout->addWidget(fAvatarShape, 1, 1);
    layout->addWidget(new QLabel(tr("Account UUID:"), this), 2, 0);
    layout->addWidget(fAccountUUID, 2, 1);
    layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding), 3, 0, 1, 2);
}

plVaultNode QVaultPlayerNode::saveNode()
{
    plVaultPlayerNode* player = fNode.upcastToPlayerNode();
    if (player == NULL)
        return plVaultNode();

    player->setAvatarShape(qstr2st(fAvatarShape->text()));
    player->setPlayerName(qstr2st(fPlayerName->text()));
    player->setAccountUuid(qstr2st(fAccountUUID->text()));
    return fNode;
}

void QVaultPlayerNode::IRefreshNode()
{
    plVaultPlayerNode* player = fNode.upcastToPlayerNode();
    if (player == NULL)
        return;

    fAvatarShape->setText(st2qstr(player->getAvatarShape()));
    fPlayerName->setText(st2qstr(player->getPlayerName()));
    fAccountUUID->setText(st2qstr(player->getAccountUuid()));
}
