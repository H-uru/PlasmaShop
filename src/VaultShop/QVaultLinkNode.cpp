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

#include "QVaultLinkNode.h"

#include <QLabel>
#include <QGridLayout>
#include <QSpacerItem>
#include <Vault/plVaultNodeTypes.h>

QVaultLinkNode::QVaultLinkNode(QWidget* parent)
    : QVaultNodeEdit(parent)
{
    fUnlocked = new QCheckBox(tr("Unlocked"), this);
    fVolatile = new QCheckBox(tr("Volatile"), this);
    fSpawnPoints = new QLineEdit(this);

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->setHorizontalSpacing(8);
    layout->setVerticalSpacing(8);
    layout->addWidget(new QLabel(tr("Link Flags:"), this), 0, 0);
    layout->addWidget(fUnlocked, 0, 1);
    layout->addWidget(fVolatile, 0, 2);
    layout->addWidget(new QLabel(tr("Spawn Points:"), this), 1, 0);
    layout->addWidget(fSpawnPoints, 1, 1, 1, 2);
    layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding), 2, 0, 1, 3);
}

plVaultNode QVaultLinkNode::saveNode()
{
    plVaultAgeLinkNode* link = fNode.upcastToAgeLinkNode();
    if (link == NULL)
        return plVaultNode();

    link->setUnlocked(fUnlocked->isChecked() ? 1 : 0);
    link->setVolatile(fVolatile->isChecked() ? 1 : 0);

    QByteArray contents = fSpawnPoints->text().toUtf8();
    plVaultBlob blob;
    blob.setData((size_t)contents.size(), (const unsigned char*)contents.data());
    link->setSpawnPoints(blob);

    return fNode;
}

void QVaultLinkNode::IRefreshNode()
{
    plVaultAgeLinkNode* link = fNode.upcastToAgeLinkNode();
    if (link == NULL)
        return;

    fUnlocked->setChecked(link->getUnlocked() != 0);
    fVolatile->setChecked(link->getVolatile() != 0);

    plVaultBlob blob = link->getSpawnPoints();
    if (blob.getSize() > 0)
        fSpawnPoints->setText(QString::fromUtf8((const char*)blob.getData(),
                                                blob.getSize()));
}
