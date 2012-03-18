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

#include "QVaultPlayerInfoNode.h"

#include <QGridLayout>
#include <QSpacerItem>
#include <Vault/plVaultNodeTypes.h>

QVaultPlayerInfoNode::QVaultPlayerInfoNode(QWidget* parent)
                    : QVaultNodeEdit(parent)
{
    fPlayerName = new QLineEdit(this);
    fPlayerID = new QSpinBox(this);
    fPlayerID->setRange(0, 0x7FFFFFFF);
    fPlayerLink = new QLinkLabel(tr("Subscribe"), this);

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->setHorizontalSpacing(8);
    layout->setVerticalSpacing(8);
    layout->addWidget(new QLabel(tr("Player Name:"), this), 0, 0);
    layout->addWidget(fPlayerName, 0, 1, 1, 2);
    layout->addWidget(new QLabel(tr("Player ID:"), this), 1, 0);
    layout->addWidget(fPlayerID, 1, 1);
    layout->addWidget(fPlayerLink, 1, 2);
    layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding), 2, 0, 1, 2);

    connect(fPlayerLink, SIGNAL(activated()), this, SLOT(subscribePlayer()));
}

plVaultNode QVaultPlayerInfoNode::saveNode()
{
    plVaultPlayerInfoNode* player = fNode.upcastToPlayerInfoNode();
    if (player == NULL)
        return plVaultNode();

    player->setPlayerName(~fPlayerName->text());
    player->setPlayerID(fPlayerID->value());
    return fNode;
}

void QVaultPlayerInfoNode::IRefreshNode()
{
    plVaultPlayerInfoNode* player = fNode.upcastToPlayerInfoNode();
    if (player == NULL)
        return;

    fPlayerName->setText(~player->getPlayerName());
    fPlayerID->setValue(player->getPlayerID());
}

void QVaultPlayerInfoNode::subscribePlayer()
{
    emit subscribe((unsigned int)fPlayerID->value());
}
