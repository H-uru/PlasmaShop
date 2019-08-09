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

#ifndef _QVAULTTEXTNODE_H
#define _QVAULTTEXTNODE_H

#include <QComboBox>
#include <QLineEdit>
#include <QPlainTextEdit>
#include "QVaultNodeEdit.h"

class QVaultTextNode : public QVaultNodeEdit
{
    Q_OBJECT

protected:
    QLineEdit* fNoteTitle;
    QComboBox* fNoteType;
    QPlainTextEdit* fNoteContents;

public:
    QVaultTextNode(QWidget* parent);
    virtual QString getEditorTitle() const { return "Text Note"; }
    virtual plVaultNode saveNode();

protected:
    virtual void IRefreshNode();
};

#endif
