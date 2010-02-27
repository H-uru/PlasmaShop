#include "QVaultNodeEdit.h"

#include "QVaultFolderNode.h"
#include "QVaultTextNode.h"
#include "QVaultSDLNode.h"
#include "QVaultChronicleNode.h"
#include "QVaultLinkNode.h"
#include "QVaultPlayerNode.h"
#include "QVaultPlayerInfoNode.h"
#include "QVaultAgeNode.h"
#include "QVaultAgeInfoNode.h"
#include "QVaultImageNode.h"

/* QVaultNodeEdit */
QVaultNodeEdit::QVaultNodeEdit(QWidget* parent)
              : QWidget(parent)
{ }

QVaultNodeEdit* QVaultNodeEdit::MakeEditor(QWidget* parent, const plVaultNode& node,
                                           plResManager* mgr, plSDLMgr* sdl)
{
    if (!node.isValid())
        return NULL;

    QVaultNodeEdit* editor = NULL;
    switch (node.getNodeType()) {
    case plVault::kNodeFolder:
        editor = new QVaultFolderNode(parent);
        break;
    case plVault::kNodePlayerInfoList:
        editor = new QVaultPlayerInfoList(parent);
        break;
    case plVault::kNodeAgeInfoList:
        editor = new QVaultAgeInfoList(parent);
        break;
    case plVault::kNodeTextNote:
        editor = new QVaultTextNode(parent);
        break;
    case plVault::kNodeImage:
        editor = new QVaultImageNode(parent);
        break;
    case plVault::kNodeSDL:
        editor = new QVaultSDLNode(parent);
        break;
    case plVault::kNodeChronicle:
        editor = new QVaultChronicleNode(parent);
        break;
    case plVault::kNodeAgeLink:
        editor = new QVaultLinkNode(parent);
        break;
    case plVault::kNodePlayer:
        editor = new QVaultPlayerNode(parent);
        break;
    case plVault::kNodePlayerInfo:
        editor = new QVaultPlayerInfoNode(parent);
        break;
    case plVault::kNodeAge:
        editor = new QVaultAgeNode(parent);
        break;
    case plVault::kNodeAgeInfo:
        editor = new QVaultAgeInfoNode(parent);
        break;
    }

    if (editor != NULL) {
        editor->setMgrs(mgr, sdl);
        editor->setNode(node);
    }
    return editor;
}

void QVaultNodeEdit::setNode(const plVaultNode& node)
{
    fNode = node;
    IRefreshNode();
}

void QVaultNodeEdit::setMgrs(plResManager*, plSDLMgr*)
{ }
