#include "QVaultNode.h"

#include <QGridLayout>
#include <QSpacerItem>
#include <QIntValidator>
#include <QRegExpValidator>
#include <QDateTime>
#include <Vault/plVaultNodeTypes.h>
#include "QVaultSDLNode.h"

QString QVaultNode::sNodeTypeNames[] = {
    "(Invalid)", "Player", "Age", "Game Server", "Admin", "Vault", "CCR",
    "Folder", "Player Info", "System", "Image", "Text Note", "SDL", "Age Link",
    "Chronicle", "Player Info List", "Marker", "Age Info", "Age Info List",
    "Marker List"
};

int QVaultNode::MapNodeType(unsigned int nodeType)
{
    switch (nodeType) {
    case plVault::kNodePlayer:
        return 1;
    case plVault::kNodeAge:
        return 2;
    case plVault::kNodeGameServer:
        return 3;
    case plVault::kNodeAdmin:
        return 4;
    case plVault::kNodeVaultServer:
        return 5;
    case plVault::kNodeCCR:
        return 6;
    case plVault::kNodeFolder:
        return 7;
    case plVault::kNodePlayerInfo:
        return 8;
    case plVault::kNodeSystem:
        return 9;
    case plVault::kNodeImage:
        return 10;
    case plVault::kNodeTextNote:
        return 11;
    case plVault::kNodeSDL:
        return 12;
    case plVault::kNodeAgeLink:
        return 13;
    case plVault::kNodeChronicle:
        return 14;
    case plVault::kNodePlayerInfoList:
        return 15;
    case plVault::kNodeMarker:
        return 16;
    case plVault::kNodeAgeInfo:
        return 17;
    case plVault::kNodeAgeInfoList:
        return 18;
    case plVault::kNodeMarkerList:
        return 19;
    default:
        return 0;
    }
}

unsigned int QVaultNode::UnmapNodeType(int idx)
{
    static unsigned int sMapping[] = {
        plVault::kNodeInvalid, plVault::kNodePlayer, plVault::kNodeAge,
        plVault::kNodeGameServer, plVault::kNodeAdmin, plVault::kNodeVaultServer,
        plVault::kNodeCCR, plVault::kNodeFolder, plVault::kNodePlayerInfo,
        plVault::kNodeSystem, plVault::kNodeImage, plVault::kNodeTextNote,
        plVault::kNodeSDL, plVault::kNodeAgeLink, plVault::kNodeChronicle,
        plVault::kNodePlayerInfoList, plVault::kNodeMarker, plVault::kNodeAgeInfo,
        plVault::kNodeAgeInfoList, plVault::kNodeMarkerList
    };

    if (idx < 0 || idx > 19)
        return plVault::kNodeInvalid;
    return sMapping[idx];
}

QVaultNode::QVaultNode(QWidget* parent)
          : QWidget(parent), fSignalLock(false)
{
    QWidget* top = new QWidget(this);
    QWidget* middle = new QWidget(this);
    QWidget* bottom = new QWidget(this);

    fAutoTime = new QLabel("N/A", top);
    fModifyTime = new QLabel("N/A", top);
    fAgeTime = new QLabel("N/A", top);
    fAgeName = new QLineEdit(top);
    fAgeGUID = new QLineEdit("0000000000000000", top);
    fAgeGUID->setValidator(new QRegExpValidator(QRegExp("[0-9A-Fa-f]{16}"), fAgeGUID));

    fCreatorID = new QLineEdit("0", top);
    fOwnerID = new QLineEdit("0", top);
    fGroupID = new QLineEdit("0", top);
    fCreatorID->setValidator(new QIntValidator(0, 0x7FFFFFFF, fCreatorID));
    fOwnerID->setValidator(new QIntValidator(0, 0x7FFFFFFF, fOwnerID));
    fGroupID->setValidator(new QIntValidator(0, 0x7FFFFFFF, fGroupID));

    QWidget* rProps = new QWidget(top);
    QWidget* permissions = new QWidget(rProps);
    fOwnerRead = new QCheckBox(permissions);
    fOwnerWrite = new QCheckBox(permissions);
    fGroupRead = new QCheckBox(permissions);
    fGroupWrite = new QCheckBox(permissions);
    fOtherRead = new QCheckBox(permissions);
    fOtherWrite = new QCheckBox(permissions);

    QGridLayout* permLayout = new QGridLayout(permissions);
    permLayout->setContentsMargins(0, 0, 0, 0);
    permLayout->setHorizontalSpacing(8);
    permLayout->setVerticalSpacing(2);
    permLayout->addWidget(new QLabel("Owner", permissions), 0, 1);
    permLayout->addWidget(new QLabel("Group", permissions), 0, 2);
    permLayout->addWidget(new QLabel("Other", permissions), 0, 3);
    permLayout->addWidget(new QLabel("Read", permissions), 1, 0);
    permLayout->addWidget(new QLabel("Write", permissions), 2, 0);
    permLayout->addWidget(fOwnerRead, 1, 1);
    permLayout->addWidget(fOwnerWrite, 2, 1);
    permLayout->addWidget(fGroupRead, 1, 2);
    permLayout->addWidget(fGroupWrite, 2, 2);
    permLayout->addWidget(fOtherRead, 1, 3);
    permLayout->addWidget(fOtherWrite, 2, 3);

    fNodeType = new QComboBox(rProps);
    for (int i=0; i<=19; i++)
        fNodeType->addItem(GetNodeTypeIcon(UnmapNodeType(i)), sNodeTypeNames[i]);

    QGridLayout* rLayout = new QGridLayout(rProps);
    rLayout->setContentsMargins(0, 0, 0, 0);
    rLayout->setHorizontalSpacing(8);
    rLayout->setVerticalSpacing(8);
    rLayout->addWidget(permissions, 0, 0, 1, 2);
    rLayout->addWidget(new QLabel("Node Type:", rProps), 1, 0);
    rLayout->addWidget(fNodeType, 1, 1);

    QGridLayout* topLayout = new QGridLayout(top);
    topLayout->setContentsMargins(0, 0, 0, 0);
    topLayout->setHorizontalSpacing(8);
    topLayout->setVerticalSpacing(2);
    topLayout->addWidget(new QLabel("Auto Time:", top), 0, 0);
    topLayout->addWidget(fAutoTime, 0, 1);
    topLayout->addWidget(new QLabel("Modify Time:", top), 1, 0);
    topLayout->addWidget(fModifyTime, 1, 1);
    topLayout->addWidget(new QLabel("Age Time:", top), 2, 0);
    topLayout->addWidget(fAgeTime, 2, 1);
    topLayout->addItem(new QSpacerItem(32, 0), 0, 2);
    topLayout->addWidget(new QLabel("Age Name:", top), 0, 3);
    topLayout->addWidget(fAgeName, 0, 4);
    topLayout->addWidget(new QLabel("Age GUID:", top), 1, 3);
    topLayout->addWidget(fAgeGUID, 1, 4);
    topLayout->addItem(new QSpacerItem(0, 32), 3, 0);
    topLayout->addWidget(new QLabel("Creator:", top), 4, 0);
    topLayout->addWidget(fCreatorID, 4, 1);
    topLayout->addWidget(new QLabel("Owner:", top), 5, 0);
    topLayout->addWidget(fOwnerID, 5, 1);
    topLayout->addWidget(new QLabel("Group:", top), 6, 0);
    topLayout->addWidget(fGroupID, 6, 1);
    topLayout->addWidget(rProps, 4, 3, 3, 2);

    fInt32_1 = new QLineEdit("0", middle);
    fInt32_2 = new QLineEdit("0", middle);
    fInt32_3 = new QLineEdit("0", middle);
    fInt32_4 = new QLineEdit("0", middle);
    fUInt32_1 = new QLineEdit("0", middle);
    fUInt32_2 = new QLineEdit("0", middle);
    fUInt32_3 = new QLineEdit("0", middle);
    fUInt32_4 = new QLineEdit("0", middle);
    fInt32_1->setValidator(new QIntValidator(fInt32_1));
    fInt32_2->setValidator(new QIntValidator(fInt32_2));
    fInt32_3->setValidator(new QIntValidator(fInt32_3));
    fInt32_4->setValidator(new QIntValidator(fInt32_4));
    fUInt32_1->setValidator(new QIntValidator(fUInt32_1));
    fUInt32_2->setValidator(new QIntValidator(fUInt32_2));
    fUInt32_3->setValidator(new QIntValidator(fUInt32_3));
    fUInt32_4->setValidator(new QIntValidator(fUInt32_4));

    QGridLayout* midLayout = new QGridLayout(middle);
    midLayout->setContentsMargins(0, 0, 0, 0);
    midLayout->setHorizontalSpacing(2);
    midLayout->setVerticalSpacing(2);
    midLayout->addWidget(new QLabel("Signed Ints:", middle), 0, 0, 1, 4);
    midLayout->addWidget(fInt32_1, 1, 0);
    midLayout->addWidget(fInt32_2, 1, 1);
    midLayout->addWidget(fInt32_3, 1, 2);
    midLayout->addWidget(fInt32_4, 1, 3);
    midLayout->addItem(new QSpacerItem(0, 16), 2, 0);
    midLayout->addWidget(new QLabel("Unsigned Ints:", middle), 3, 0, 1, 4);
    midLayout->addWidget(fUInt32_1, 4, 0);
    midLayout->addWidget(fUInt32_2, 4, 1);
    midLayout->addWidget(fUInt32_3, 4, 2);
    midLayout->addWidget(fUInt32_4, 4, 3);

    fString64_1 = new QLineEdit(bottom);
    fString64_2 = new QLineEdit(bottom);
    fString64_3 = new QLineEdit(bottom);
    fString64_4 = new QLineEdit(bottom);
    fString64_5 = new QLineEdit(bottom);
    fString64_6 = new QLineEdit(bottom);
    fIString64_1 = new QLineEdit(bottom);
    fIString64_2 = new QLineEdit(bottom);

    fText_1 = new QPlainTextEdit(bottom);
    fText_2 = new QPlainTextEdit(bottom);

    QGridLayout* botLayout = new QGridLayout(bottom);
    botLayout->setContentsMargins(0, 0, 0, 0);
    botLayout->setHorizontalSpacing(2);
    botLayout->setVerticalSpacing(2);
    botLayout->addWidget(new QLabel("Case-Sensitive Strings:", bottom), 0, 0, 1, 2);
    botLayout->addWidget(fString64_1, 1, 0);
    botLayout->addWidget(fString64_2, 1, 1);
    botLayout->addWidget(fString64_3, 2, 0);
    botLayout->addWidget(fString64_4, 2, 1);
    botLayout->addWidget(fString64_5, 3, 0);
    botLayout->addWidget(fString64_6, 3, 1);
    botLayout->addItem(new QSpacerItem(0, 16), 4, 0);
    botLayout->addWidget(new QLabel("Case-Insensitive Strings:", bottom), 5, 0, 1, 2);
    botLayout->addWidget(fIString64_1, 6, 0);
    botLayout->addWidget(fIString64_2, 6, 1);
    botLayout->addItem(new QSpacerItem(0, 16), 7, 0);
    botLayout->addWidget(new QLabel("Texts:", bottom), 8, 0, 1, 2);
    botLayout->addWidget(fText_1, 9, 0);
    botLayout->addWidget(fText_2, 9, 1);

    fBlob1_Info = new QLabel("(empty)", this);
    fBlob2_Info = new QLabel("(empty)", this);

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->setHorizontalSpacing(8);
    layout->setVerticalSpacing(32);
    layout->addWidget(top, 0, 0, 1, 5);
    layout->addWidget(middle, 1, 0, 1, 5);
    layout->addWidget(bottom, 2, 0, 1, 5);
    layout->addWidget(new QLabel("Blob 1:", this), 3, 0);
    layout->addWidget(fBlob1_Info, 3, 1);
    layout->addItem(new QSpacerItem(32, 0), 3, 2);
    layout->addWidget(new QLabel("Blob 2:", this), 3, 3);
    layout->addWidget(fBlob2_Info, 3, 4);

    connect(fNodeType, SIGNAL(currentIndexChanged(int)), this, SLOT(typeChanged(int)));
}

void QVaultNode::typeChanged(int)
{
    if (!fSignalLock)
        emit typeModified();
}

void QVaultNode::setNode(const plVaultNode& node)
{
    fSignalLock = true;
    fNode = node;

    QDateTime dt;
    if (!fNode.getAutoTime().atEpoch()) {
        dt.setTime_t(fNode.getAutoTime().getSecs());
        fAutoTime->setText(dt.toString("yyyy/MM/dd hh:mm:ss"));
    } else {
        fAutoTime->setText("N/A");
    }
    if (!fNode.getAutoTime().atEpoch()) {
        dt.setTime_t(fNode.getModifyTime().getSecs());
        fModifyTime->setText(dt.toString("yyyy/MM/dd hh:mm:ss"));
    } else {
        fModifyTime->setText("N/A");
    }
    if (!fNode.getAutoTime().atEpoch()) {
        dt.setTime_t(fNode.getCreateAgeTime().getSecs());
        fAgeTime->setText(dt.toString("yyyy/MM/dd hh:mm:ss"));
    } else {
        fAgeTime->setText("N/A");
    }

    fAgeName->setText(~fNode.getCreateAgeName());
    fAgeGUID->setText(~fNode.getCreateAgeGuid().toString());

    fCreatorID->setText(QString("%1").arg(fNode.getCreator()));
    fOwnerID->setText(QString("%1").arg(fNode.getOwner()));
    fGroupID->setText(QString("%1").arg(fNode.getGroup()));

    fOwnerRead->setChecked((fNode.getPermissions() & plVault::kOwnerRead) != 0);
    fOwnerWrite->setChecked((fNode.getPermissions() & plVault::kOwnerWrite) != 0);
    fGroupRead->setChecked((fNode.getPermissions() & plVault::kGroupRead) != 0);
    fGroupWrite->setChecked((fNode.getPermissions() & plVault::kGroupWrite) != 0);
    fOtherRead->setChecked((fNode.getPermissions() & plVault::kOtherRead) != 0);
    fOtherWrite->setChecked((fNode.getPermissions() & plVault::kOtherWrite) != 0);
    fNodeType->setCurrentIndex(MapNodeType(fNode.getNodeType()));

    fInt32_1->setText(QString("%1").arg(fNode.getInt32_1()));
    fInt32_2->setText(QString("%1").arg(fNode.getInt32_2()));
    fInt32_3->setText(QString("%1").arg(fNode.getInt32_3()));
    fInt32_4->setText(QString("%1").arg(fNode.getInt32_4()));
    fUInt32_1->setText(QString("%1").arg(fNode.getUInt32_1()));
    fUInt32_2->setText(QString("%1").arg(fNode.getUInt32_2()));
    fUInt32_3->setText(QString("%1").arg(fNode.getUInt32_3()));
    fUInt32_4->setText(QString("%1").arg(fNode.getUInt32_4()));

    fString64_1->setText(~fNode.getString64_1());
    fString64_2->setText(~fNode.getString64_2());
    fString64_3->setText(~fNode.getString64_3());
    fString64_4->setText(~fNode.getString64_4());
    fString64_5->setText(~fNode.getString64_5());
    fString64_6->setText(~fNode.getString64_6());
    fIString64_1->setText(~fNode.getIString64_1());
    fIString64_2->setText(~fNode.getIString64_2());

    fText_1->setPlainText(~fNode.getText_1());
    fText_2->setPlainText(~fNode.getText_2());

    if (fNode.getBlob_1().getSize() > 0)
        fBlob1_Info->setText(QString("%1 bytes").arg(fNode.getBlob_1().getSize()));
    else
        fBlob1_Info->setText(QString("(empty)"));

    if (fNode.getBlob_2().getSize() > 0)
        fBlob2_Info->setText(QString("%1 bytes").arg(fNode.getBlob_2().getSize()));
    else
        fBlob2_Info->setText(QString("(empty)"));

    fSignalLock = false;
}

plVaultNode QVaultNode::saveNode()
{
    fNode.setCreateAgeName(~fAgeName->text());
    try {
        fNode.setCreateAgeGuid(plServerGuid::FromString(~fAgeGUID->text()));
    } catch (...) { }

    fNode.setCreator(fCreatorID->text().toUInt());
    fNode.setOwner(fOwnerID->text().toUInt());
    fNode.setGroup(fGroupID->text().toUInt());

    fNode.setPermissions(
        (fOwnerRead->isChecked() ? plVault::kOwnerRead : 0) |
        (fOwnerWrite->isChecked() ? plVault::kOwnerWrite : 0) |
        (fGroupRead->isChecked() ? plVault::kGroupRead : 0) |
        (fGroupWrite->isChecked() ? plVault::kGroupWrite : 0) |
        (fOtherRead->isChecked() ? plVault::kOtherRead : 0) |
        (fOtherWrite->isChecked() ? plVault::kOtherWrite : 0));
    fNode.setNodeType(UnmapNodeType(fNodeType->currentIndex()));

    fNode.setInt32_1(fInt32_1->text().toInt());
    fNode.setInt32_2(fInt32_2->text().toInt());
    fNode.setInt32_3(fInt32_3->text().toInt());
    fNode.setInt32_4(fInt32_4->text().toInt());
    fNode.setUInt32_1(fUInt32_1->text().toUInt());
    fNode.setUInt32_2(fUInt32_2->text().toUInt());
    fNode.setUInt32_3(fUInt32_3->text().toUInt());
    fNode.setUInt32_4(fUInt32_4->text().toUInt());

    fNode.setString64_1(~fString64_1->text());
    fNode.setString64_2(~fString64_2->text());
    fNode.setString64_3(~fString64_3->text());
    fNode.setString64_4(~fString64_4->text());
    fNode.setString64_5(~fString64_5->text());
    fNode.setString64_6(~fString64_6->text());
    fNode.setIString64_1(~fIString64_1->text());
    fNode.setIString64_2(~fIString64_2->text());

    fNode.setText_1(~fText_1->toPlainText());
    fNode.setText_2(~fText_2->toPlainText());

    return fNode;
}


QIcon GetNodeTypeIcon(unsigned int nodeType)
{
    static QIcon s_nodeIcons[] = {
        QIcon(":img/admin.png"), QIcon(":img/age.png"), QIcon(":img/agei.png"),
        QIcon(":img/bfldr.png"), QIcon(":img/data.png"), QIcon(":img/gtext.png"),
        QIcon(":img/img.png"), QIcon(":img/link.png"), QIcon(":img/mrkr.png"),
        QIcon(":img/plr.png"), QIcon(":img/plri.png"), QIcon(":img/rfldr.png"),
        QIcon(":img/save.png"), QIcon(":img/text.png"), QIcon(":img/wfldr.png"),
        QIcon(":img/yfldr.png"), QIcon(":img/gfldr.png"), QIcon(":img/unknown.png")
    };

    switch (nodeType) {
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
        return ~node.upcastToPlayerNode()->getPlayerName();
    case plVault::kNodeAge:
        return ~node.getCreateAgeName();
    case plVault::kNodeGameServer:
        return ~node.upcastToGameServerNode()->getAgeFilename();
    case plVault::kNodeAdmin:
        return "Admin";
    case plVault::kNodeVaultServer:
        return "Vault";
    case plVault::kNodeFolder:
        return GetFolderName(node);
    case plVault::kNodePlayerInfo:
        return ~node.upcastToPlayerInfoNode()->getPlayerName();
    case plVault::kNodeSystem:
        return "System";
    case plVault::kNodeImage:
        return ~node.upcastToImageNode()->getImageTitle();
    case plVault::kNodeTextNote:
        return ~node.upcastToTextNoteNode()->getNoteTitle();
    case plVault::kNodeSDL:
        return QSDLEditor::GetSDLName(node.upcastToSDLNode()->getSDLData());
    case plVault::kNodeAgeLink:
        return "Link";
    case plVault::kNodeChronicle:
        return ~node.upcastToChronicleNode()->getEntryName();
    case plVault::kNodePlayerInfoList:
        return GetFolderName(node);
    case plVault::kNodeMarker:
        return QString("%1 Marker").arg(~node.upcastToMarkerNode()->getAgeName());
    case plVault::kNodeAgeInfo:
        return ~node.upcastToAgeInfoNode()->getAgeInstanceName();
    case plVault::kNodeAgeInfoList:
        return GetFolderName(node);
    case plVault::kNodeMarkerList:
        return ~node.upcastToMarkerListNode()->getGameTitle();
    default:
        return "Unknown";
    }
}

QString GetFolderName(const plVaultNode& node)
{
    plVaultFolderNode* folder = node.upcastToFolderNode();
    if (!folder->getFolderName().empty())
        return ~folder->getFolderName();

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
