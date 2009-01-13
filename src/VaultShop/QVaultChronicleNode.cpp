#include "QVaultChronicleNode.h"

#include <QLabel>
#include <QGridLayout>
#include <QSpacerItem>
#include <Vault/plVaultNodeTypes.h>

QVaultChronicleNode::QVaultChronicleNode(QWidget* parent)
                   : QVaultNodeEdit(parent)
{
    fChronicleType = new QSpinBox(this);
    fChronicleType->setRange(-0x80000000, 0x7FFFFFFF);
    fChronicleName = new QLineEdit(this);
    fChronicleValue = new QLineEdit(this);

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->setHorizontalSpacing(8);
    layout->setVerticalSpacing(8);
    layout->addWidget(new QLabel(tr("Chronicle Type:"), this), 0, 0);
    layout->addWidget(fChronicleType, 0, 1);
    layout->addWidget(new QLabel(tr("Chronicle Name:"), this), 1, 0);
    layout->addWidget(fChronicleName, 1, 1);
    layout->addWidget(new QLabel(tr("Chronicle Value:"), this), 2, 0);
    layout->addWidget(fChronicleValue, 2, 1);
    layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding), 3, 0, 1, 2);
}

QString QVaultChronicleNode::getEditorTitle() const
{ return "Chronicle"; }

plVaultNode QVaultChronicleNode::saveNode()
{
    plVaultChronicleNode* chron = fNode.upcastToChronicleNode();
    if (chron == NULL)
        return plVaultNode();

    chron->setEntryType(fChronicleType->value());
    chron->setEntryName(~fChronicleName->text());
    chron->setEntryValue(~fChronicleValue->text());
    return fNode;
}

void QVaultChronicleNode::IRefreshNode()
{
    plVaultChronicleNode* chron = fNode.upcastToChronicleNode();
    if (chron == NULL)
        return;

    fChronicleType->setValue(chron->getEntryType());
    fChronicleName->setText(~chron->getEntryName());
    fChronicleValue->setText(~chron->getEntryValue());
}
