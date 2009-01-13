#include "QVaultTextNode.h"

#include <QLabel>
#include <QGridLayout>
#include <Vault/plVaultNodeTypes.h>

/* QVaultTextNode */
QVaultTextNode::QVaultTextNode(QWidget* parent)
              : QVaultNodeEdit(parent)
{
    static QString s_NoteTypes[] = {
        "(Generic)", "CCR Petition", "Device Inbox", "Invite",
        "Visit Invitation", "Visit Revokation"
    };

    fNoteTitle = new QLineEdit(this);

    fNoteType = new QComboBox(this);
    for (int i=0; i<=5; i++)
        fNoteType->addItem(s_NoteTypes[i]);

    fNoteContents = new QPlainTextEdit(this);

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->setHorizontalSpacing(8);
    layout->setVerticalSpacing(8);
    layout->addWidget(new QLabel(tr("Title:"), this), 0, 0);
    layout->addWidget(fNoteTitle, 0, 1);
    layout->addWidget(new QLabel(tr("Type:"), this), 1, 0);
    layout->addWidget(fNoteType, 1, 1);
    layout->addWidget(new QLabel(tr("Contents:"), this), 2, 0);
    layout->addWidget(fNoteContents, 2, 1);
}

QString QVaultTextNode::getEditorTitle() const
{ return "Text Note"; }

plVaultNode QVaultTextNode::saveNode()
{
    plVaultTextNoteNode* text = fNode.upcastToTextNoteNode();
    if (text == NULL)
        return plVaultNode();

    text->setNoteTitle(~fNoteTitle->text());
    text->setNoteType(fNoteType->currentIndex());

    QByteArray contents = fNoteContents->toPlainText().toUtf8();
    plVaultBlob blob;
    blob.setData((size_t)contents.size(), (const unsigned char*)contents.data());
    text->setNoteContents(blob);

    return fNode;
}

void QVaultTextNode::IRefreshNode()
{
    plVaultTextNoteNode* text = fNode.upcastToTextNoteNode();
    if (text == NULL)
        return;

    fNoteTitle->setText(~text->getNoteTitle());
    fNoteType->setCurrentIndex(text->getNoteType());

    plVaultBlob blob = text->getNoteContents();
    if (blob.getSize() > 0)
        fNoteContents->setPlainText(QString::fromUtf8((const char*)blob.getData(),
                                                      blob.getSize()));
}
