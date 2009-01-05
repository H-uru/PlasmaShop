#include "QVaultNodeEdit.h"

#include "QVaultFolderNode.h"

/* QVaultNodeEdit */
QVaultNodeEdit::QVaultNodeEdit(QWidget* parent)
              : QWidget(parent)
{ }

QVaultNodeEdit* QVaultNodeEdit::MakeEditor(QWidget* parent, const plVaultNode& node)
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
    }

    if (editor != NULL)
        editor->setNode(node);
    return editor;
}

void QVaultNodeEdit::setNode(const plVaultNode& node)
{
    fNode = node;
    IRefreshNode();
}
