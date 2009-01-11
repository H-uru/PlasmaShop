#include "QVaultAgeNode.h"

#include <QGridLayout>
#include <QSpacerItem>
#include <QRegExpValidator>
#include <Vault/plVaultNodeTypes.h>

QVaultAgeNode::QVaultAgeNode(QWidget* parent)
             : QVaultNodeEdit(parent)
{
    fAgeInstanceGuid = new QLineEdit("0000000000000000", this);
    fAgeInstanceGuid->setValidator(new QRegExpValidator(QRegExp("[0-9A-Fa-f]{16}"), fAgeInstanceGuid));

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->setHorizontalSpacing(8);
    layout->setVerticalSpacing(8);
    layout->addWidget(new QLabel(tr("Age Instance GUID:"), this), 0, 0);
    layout->addWidget(fAgeInstanceGuid, 0, 1);
    layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding), 1, 0);
}

QString QVaultAgeNode::getEditorTitle() const
{ return "Age Instance"; }

plVaultNode QVaultAgeNode::saveNode()
{
    plVaultAgeNode* age = fNode.upcastToAgeNode();
    if (age == NULL)
        return plVaultNode();

    age->setAgeInstanceGuid(plServerGuid::FromString(fAgeInstanceGuid->text().toUtf8().data()));
    return fNode;
}

void QVaultAgeNode::IRefreshNode()
{
    plVaultAgeNode* age = fNode.upcastToAgeNode();
    if (age == NULL)
        return;

    fAgeInstanceGuid->setText(QString::fromUtf8(age->getAgeInstanceGuid().toString().cstr()));
}
