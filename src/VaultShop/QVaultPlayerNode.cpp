#include "QVaultPlayerNode.h"

#include <QLabel>
#include <QGridLayout>
#include <QSpacerItem>
#include <QRegExpValidator>
#include <QCompleter>
#include <Vault/plVaultNodeTypes.h>

QVaultPlayerNode::QVaultPlayerNode(QWidget* parent)
                : QVaultNodeEdit(parent)
{
    fPlayerName = new QLineEdit(this);
    fAvatarShape = new QLineEdit(this);
    fAccountUUID = new QLineEdit(this);
    fAccountUUID->setValidator(new QRegExpValidator(QRegExp("[0-9A-Fa-f]{8}-[0-9A-Fa-f]{4}-[0-9A-Fa-f]{4}-[0-9A-Fa-f]{4}-[0-9A-Fa-f]{12}"), fAccountUUID));

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

QString QVaultPlayerNode::getEditorTitle() const
{ return "Player"; }

plVaultNode QVaultPlayerNode::saveNode()
{
    plVaultPlayerNode* player = fNode.upcastToPlayerNode();
    if (player == NULL)
        return plVaultNode();

    player->setAvatarShape(~fAvatarShape->text());
    player->setPlayerName(~fPlayerName->text());
    player->setAccountUuid(~fAccountUUID->text());
    return fNode;
}

void QVaultPlayerNode::IRefreshNode()
{
    plVaultPlayerNode* player = fNode.upcastToPlayerNode();
    if (player == NULL)
        return;

    fAvatarShape->setText(~player->getAvatarShape());
    fPlayerName->setText(~player->getPlayerName());
    fAccountUUID->setText(~player->getAccountUuid());
}
