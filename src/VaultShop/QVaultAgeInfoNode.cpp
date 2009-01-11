#include "QVaultAgeInfoNode.h"

#include <QGridLayout>
#include <QSpacerItem>
#include <QRegExpValidator>
#include <Vault/plVaultNodeTypes.h>

QVaultAgeInfoNode::QVaultAgeInfoNode(QWidget* parent)
                 : QVaultNodeEdit(parent)
{
    fAgeID = new QSpinBox(this);
    fAgeID->setRange(0, 0x7FFFFFFF);
    fAgeLink = new QLinkLabel(tr("Subscribe"), this);
    fAgeCzarID = new QSpinBox(this);
    fAgeCzarID->setRange(0, 0x7FFFFFFF);
    fAgeCzarLink = new QLinkLabel(tr("Subscribe"), this);
    fAgeFilename = new QLineEdit(this);
    fAgeInstanceName = new QLineEdit(this);
    fAgeUserDefinedName = new QLineEdit(this);
    fAgeDescription = new QLineEdit(this);
    fAgeInstanceGuid = new QLineEdit("0000000000000000", this);
    fAgeInstanceGuid->setValidator(new QRegExpValidator(QRegExp("[0-9A-Fa-f]{16}"), fAgeInstanceGuid));

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->setHorizontalSpacing(8);
    layout->setVerticalSpacing(8);
    layout->addWidget(new QLabel(tr("Age ID:"), this), 0, 0);
    layout->addWidget(fAgeID, 0, 1);
    layout->addWidget(fAgeLink, 0, 2);
    layout->addWidget(new QLabel(tr("Age Czar ID:"), this), 1, 0);
    layout->addWidget(fAgeCzarID, 1, 1);
    layout->addWidget(fAgeCzarLink, 1, 2);
    layout->addWidget(new QLabel(tr("Age Filename:"), this), 2, 0);
    layout->addWidget(fAgeFilename, 2, 1, 1, 2);
    layout->addWidget(new QLabel(tr("Age Instance Name:"), this), 3, 0);
    layout->addWidget(fAgeInstanceName, 3, 1, 1, 2);
    layout->addWidget(new QLabel(tr("Age User Defined Name:"), this), 4, 0);
    layout->addWidget(fAgeUserDefinedName, 4, 1, 1, 2);
    layout->addWidget(new QLabel(tr("Age Description:"), this), 5, 0);
    layout->addWidget(fAgeDescription, 5, 1, 1, 2);
    layout->addWidget(new QLabel(tr("Age Instance GUID:"), this), 6, 0);
    layout->addWidget(fAgeInstanceGuid, 6, 1, 1, 2);
    layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding), 7, 0, 1, 2);

    connect(fAgeLink, SIGNAL(activated()), this, SLOT(subscribeAge()));
    connect(fAgeCzarLink, SIGNAL(activated()), this, SLOT(subscribeAgeCzar()));
}

QString QVaultAgeInfoNode::getEditorTitle() const
{ return "Age Info"; }

plVaultNode QVaultAgeInfoNode::saveNode()
{
    plVaultAgeInfoNode* age = fNode.upcastToAgeInfoNode();
    if (age == NULL)
        return plVaultNode();

    age->setAgeID(fAgeID->value());
    age->setAgeCzarID(fAgeCzarID->value());
    age->setAgeFilename(fAgeFilename->text().toUtf8().data());
    age->setAgeInstanceName(fAgeInstanceName->text().toUtf8().data());
    age->setAgeUserDefinedName(fAgeUserDefinedName->text().toUtf8().data());
    age->setAgeDescription(fAgeDescription->text().toUtf8().data());
    age->setAgeInstanceGuid(plServerGuid::FromString(fAgeInstanceGuid->text().toUtf8().data()));
    return fNode;
}

void QVaultAgeInfoNode::IRefreshNode()
{
    plVaultAgeInfoNode* age = fNode.upcastToAgeInfoNode();
    if (age == NULL)
        return;

    fAgeID->setValue(age->getAgeID());
    fAgeCzarID->setValue(age->getAgeCzarID());
    fAgeFilename->setText(QString::fromUtf8(age->getAgeFilename().cstr()));
    fAgeInstanceName->setText(QString::fromUtf8(age->getAgeInstanceName().cstr()));
    fAgeUserDefinedName->setText(QString::fromUtf8(age->getAgeUserDefinedName().cstr()));
    fAgeDescription->setText(QString::fromUtf8(age->getAgeDescription().cstr()));
    fAgeInstanceGuid->setText(QString::fromUtf8(age->getAgeInstanceGuid().toString().cstr()));
}

void QVaultAgeInfoNode::subscribeAge()
{
    emit subscribe((unsigned int)fAgeID->value());
}

void QVaultAgeInfoNode::subscribeAgeCzar()
{
    emit subscribe((unsigned int)fAgeCzarID->value());
}
