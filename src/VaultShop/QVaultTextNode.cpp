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
