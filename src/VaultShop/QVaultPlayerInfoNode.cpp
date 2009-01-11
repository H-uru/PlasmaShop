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

QString QVaultPlayerInfoNode::getEditorTitle() const
{ return "Player Info"; }

plVaultNode QVaultPlayerInfoNode::saveNode()
{
    plVaultPlayerInfoNode* player = fNode.upcastToPlayerInfoNode();
    if (player == NULL)
        return plVaultNode();

    player->setPlayerName(fPlayerName->text().toUtf8().data());
    player->setPlayerID(fPlayerID->value());
    return fNode;
}

void QVaultPlayerInfoNode::IRefreshNode()
{
    plVaultPlayerInfoNode* player = fNode.upcastToPlayerInfoNode();
    if (player == NULL)
        return;

    fPlayerName->setText(QString::fromUtf8(player->getPlayerName().cstr()));
    fPlayerID->setValue(player->getPlayerID());
}

void QVaultPlayerInfoNode::subscribePlayer()
{
    emit subscribe((unsigned int)fPlayerID->value());
}
