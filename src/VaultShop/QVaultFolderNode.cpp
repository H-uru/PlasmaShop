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

#include "QVaultFolderNode.h"

#include <QLabel>
#include <QGridLayout>
#include <QSpacerItem>
#include <Vault/plVaultNodeTypes.h>

/* QVaultFolderNode */
QString QVaultFolderNode::sFolderTypeNames[] = {
    "(Generic)", "Age Devices", "Age Journals", "Age Members", "Age Owners",
    "Age Type Journals", "Ages I Can Visit", "Ages I Own",
    "All Age Global SDL Folder", "All Players", "Avatar Closet",
    "Avatar Outfit Folder", "Buddy List", "Can Visit List", "CCR Players",
    "Child Ages", "Chronicle Folder", "Device Inbox", "Global Inbox",
    "Hood Members", "Ignore List", "Inbox", "People I Know About",
    "Player Invites", "Public Ages", "Sub Ages", "Vault Manager Global Data"
};

int QVaultFolderNode::MapFolderType(int type)
{
    switch (type) {
    case plVault::kAgeDevicesFolder:
        return 1;
    case plVault::kAgeJournalsFolder:
        return 2;
    case plVault::kAgeMembersFolder:
        return 3;
    case plVault::kAgeOwnersFolder:
        return 4;
    case plVault::kAgeTypeJournalFolder:
        return 5;
    case plVault::kAgesICanVisitFolder:
        return 6;
    case plVault::kAgesIOwnFolder:
        return 7;
    case plVault::kAllAgeGlobalSDLNodesFolder:
        return 8;
    case plVault::kAllPlayersFolder:
        return 9;
    case plVault::kAvatarClosetFolder:
        return 10;
    case plVault::kAvatarOutfitFolder:
        return 11;
    case plVault::kBuddyListFolder:
        return 12;
    case plVault::kCanVisitFolder:
        return 13;
    case plVault::kCCRPlayersFolder:
        return 14;
    case plVault::kChildAgesFolder:
        return 15;
    case plVault::kChronicleFolder:
        return 16;
    case plVault::kDeviceInboxFolder:
        return 17;
    case plVault::kGlobalInboxFolder:
        return 18;
    case plVault::kHoodMembersFolder:
        return 19;
    case plVault::kIgnoreListFolder:
        return 20;
    case plVault::kInboxFolder:
        return 21;
    case plVault::kPeopleIKnowAboutFolder:
        return 22;
    case plVault::kPlayerInviteFolder:
        return 23;
    case plVault::kPublicAgesFolder:
        return 24;
    case plVault::kSubAgesFolder:
        return 25;
    case plVault::kVaultMgrGlobalDataFolder:
        return 26;
    default:
        /* Set Invalid nodes to being Generic ones */
        return 0;
    }
}

int QVaultFolderNode::UnmapFolderType(int idx)
{
    static int sMapping[] = {
        plVault::kAgeDevicesFolder, plVault::kAgeJournalsFolder,
        plVault::kAgeMembersFolder, plVault::kAgeOwnersFolder,
        plVault::kAgeTypeJournalFolder, plVault::kAgesICanVisitFolder,
        plVault::kAgesIOwnFolder, plVault::kAllAgeGlobalSDLNodesFolder,
        plVault::kAllPlayersFolder, plVault::kAvatarClosetFolder,
        plVault::kAvatarOutfitFolder, plVault::kBuddyListFolder,
        plVault::kCanVisitFolder, plVault::kCCRPlayersFolder,
        plVault::kChildAgesFolder, plVault::kChronicleFolder,
        plVault::kDeviceInboxFolder, plVault::kGlobalInboxFolder,
        plVault::kHoodMembersFolder, plVault::kIgnoreListFolder,
        plVault::kInboxFolder, plVault::kPeopleIKnowAboutFolder,
        plVault::kPlayerInviteFolder, plVault::kPublicAgesFolder,
        plVault::kSubAgesFolder, plVault::kVaultMgrGlobalDataFolder,
    };

    if (idx < 1 || idx > 26)
        // Set Invalid nodes to being Generic ones
        return plVault::kUserDefinedNode;
    return sMapping[idx-1];
}

QVaultFolderNode::QVaultFolderNode(QWidget* parent)
                : QVaultNodeEdit(parent)
{
    fFolderType = new QComboBox(this);
    for (int i=0; i<=26; i++)
        fFolderType->addItem(sFolderTypeNames[i]);

    fFolderName = new QLineEdit(this);

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->setHorizontalSpacing(8);
    layout->setVerticalSpacing(8);
    layout->addWidget(new QLabel(tr("Folder Type:"), this), 0, 0);
    layout->addWidget(fFolderType, 0, 1);
    layout->addWidget(new QLabel(tr("Folder Name:"), this), 1, 0);
    layout->addWidget(fFolderName, 1, 1);
    layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding), 2, 0, 1, 2);
}

plVaultNode QVaultFolderNode::saveNode()
{
    plVaultFolderNode* folder = fNode.upcastToFolderNode();
    if (folder == NULL)
        return plVaultNode();

    folder->setFolderType(UnmapFolderType(fFolderType->currentIndex()));
    folder->setFolderName(~fFolderName->text());
    return fNode;
}

void QVaultFolderNode::IRefreshNode()
{
    plVaultFolderNode* folder = fNode.upcastToFolderNode();
    if (folder == NULL)
        return;

    fFolderType->setCurrentIndex(MapFolderType(folder->getFolderType()));
    fFolderName->setText(~folder->getFolderName());
}
