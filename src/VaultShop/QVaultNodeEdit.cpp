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
