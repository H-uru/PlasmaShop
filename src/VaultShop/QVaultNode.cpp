#include "QVaultNode.h"
#include <Vault/plVaultNodeTypes.h>

QIcon GetNodeTypeIcon(const plVaultNode& node)
{
    static QIcon s_nodeIcons[] = {
        QIcon(":img/admin.png"), QIcon(":img/age.png"), QIcon(":img/agei.png"),
        QIcon(":img/bfldr.png"), QIcon(":img/data.png"), QIcon(":img/gtext.png"),
        QIcon(":img/img.png"), QIcon(":img/link.png"), QIcon(":img/mrkr.png"),
        QIcon(":img/plr.png"), QIcon(":img/plri.png"), QIcon(":img/rfldr.png"),
        QIcon(":img/save.png"), QIcon(":img/text.png"), QIcon(":img/wfldr.png"),
        QIcon(":img/yfldr.png"), QIcon(":img/gfldr.png"), QIcon(":img/unknown.png")
    };

    switch (node.getNodeType()) {
    case plVault::kNodePlayer:
        return s_nodeIcons[9];
    case plVault::kNodeAge:
        return s_nodeIcons[1];
    case plVault::kNodeGameServer:
        return s_nodeIcons[12];
    case plVault::kNodeAdmin:
        return s_nodeIcons[0];
    case plVault::kNodeVaultServer:
        return s_nodeIcons[11];
    case plVault::kNodeFolder:
        return s_nodeIcons[15];
    case plVault::kNodePlayerInfo:
        return s_nodeIcons[10];
    case plVault::kNodeSystem:
        return s_nodeIcons[16];
    case plVault::kNodeImage:
        return s_nodeIcons[6];
    case plVault::kNodeTextNote:
        return s_nodeIcons[13];
    case plVault::kNodeSDL:
        return s_nodeIcons[4];
    case plVault::kNodeAgeLink:
        return s_nodeIcons[7];
    case plVault::kNodeChronicle:
        return s_nodeIcons[5];
    case plVault::kNodePlayerInfoList:
        return s_nodeIcons[3];
    case plVault::kNodeMarker:
        return s_nodeIcons[8];
    case plVault::kNodeAgeInfo:
        return s_nodeIcons[2];
    case plVault::kNodeAgeInfoList:
        return s_nodeIcons[3];
    case plVault::kNodeMarkerList:
        return s_nodeIcons[14];
    default:
        return s_nodeIcons[17];
    }
}

QString GetNodeDisplay(const plVaultNode& node)
{
    switch (node.getNodeType()) {
    case plVault::kNodePlayer:
        return node.upcastToPlayerNode()->getPlayerName().cstr();
    case plVault::kNodeAge:
        return node.upcastToAgeNode()->getAgeInstanceGuid().toString().cstr();
    case plVault::kNodeGameServer:
        return node.upcastToGameServerNode()->getAgeFilename().cstr();
    case plVault::kNodeAdmin:
        return "Admin";
    case plVault::kNodeVaultServer:
        return "Vault";
    case plVault::kNodeFolder:
        return GetFolderName(node);
    case plVault::kNodePlayerInfo:
        return node.upcastToPlayerInfoNode()->getPlayerName().cstr();
    case plVault::kNodeSystem:
        return "System";
    case plVault::kNodeImage:
        return node.upcastToImageNode()->getImageTitle().cstr();
    case plVault::kNodeTextNote:
        return node.upcastToTextNoteNode()->getNoteTitle().cstr();
    case plVault::kNodeSDL:
        return node.upcastToSDLNode()->getSDLName().cstr();
    case plVault::kNodeAgeLink:
        return "Link";
    case plVault::kNodeChronicle:
        return node.upcastToChronicleNode()->getEntryName().cstr();
    case plVault::kNodePlayerInfoList:
        return GetFolderName(node);
    case plVault::kNodeMarker:
        return (node.upcastToMarkerNode()->getAgeName() + " Marker").cstr();
    case plVault::kNodeAgeInfo:
        return node.upcastToAgeInfoNode()->getAgeFilename().cstr();
    case plVault::kNodeAgeInfoList:
        return GetFolderName(node);
    case plVault::kNodeMarkerList:
        return node.upcastToMarkerListNode()->getGameTitle().cstr();
    default:
        return "Unknown";
    }
}

QString GetFolderName(const plVaultNode& node)
{
    plVaultFolderNode* folder = node.upcastToFolderNode();
    if (!folder->getFolderName().empty())
        return folder->getFolderName().cstr();

    switch (folder->getFolderType()) {
    case plVault::kInboxFolder:
        return "Inbox";
    case plVault::kBuddyListFolder:
        return "Buddy List";
    case plVault::kIgnoreListFolder:
        return "Ignore List";
    case plVault::kPeopleIKnowAboutFolder:
        return "People I Know About";
    case plVault::kVaultMgrGlobalDataFolder:
        return "Vault Manager Global Data";
    case plVault::kChronicleFolder:
        return "Chronicles";
    case plVault::kAvatarOutfitFolder:
        return "Avatar Outfit";
    case plVault::kAgeTypeJournalFolder:
        return "Age Journals";
    case plVault::kSubAgesFolder:
        return "Sub-Ages";
    case plVault::kDeviceInboxFolder:
        return "Device Inbox";
    case plVault::kHoodMembersFolder:
        return "Hood Members";
    case plVault::kAllPlayersFolder:
        return "All Players";
    case plVault::kAgeMembersFolder:
        return "Age Members";
    case plVault::kAgeJournalsFolder:
        return "Age Journals";
    case plVault::kAgeDevicesFolder:
        return "Age Devices";
    case plVault::kAgeInstanceSDLNode:
        return "Age SDL";
    case plVault::kAgeGlobalSDLNode:
        return "Global SDL";
    case plVault::kCanVisitFolder:
        return "Can Visit";
    case plVault::kAgeOwnersFolder:
        return "Age Owners";
    case plVault::kAllAgeGlobalSDLNodesFolder:
        return "All Age Global SDL";
    case plVault::kPlayerInfoNode:
        return "Player Info";
    case plVault::kPublicAgesFolder:
        return "Public Ages";
    case plVault::kAgesIOwnFolder:
        return "Ages I Own";
    case plVault::kAgesICanVisitFolder:
        return "Ages I Can Visit";
    case plVault::kAvatarClosetFolder:
        return "Avatar Closet";
    case plVault::kAgeInfoNode:
        return "Age Info";
    case plVault::kSystemNode:
        return "System";
    case plVault::kPlayerInviteFolder:
        return "Player Invites";
    case plVault::kCCRPlayersFolder:
        return "CCR Players";
    case plVault::kGlobalInboxFolder:
        return "Global Inbox";
    case plVault::kChildAgesFolder:
        return "Child Ages";
    case plVault::kGameScoresFolder:
        return "Game Scores";
    default:
        return "Folder";
    }
}
