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

#ifndef _QVAULTFOLDERNODE_H
#define _QVAULTFOLDERNODE_H

#include <QComboBox>
#include <QLineEdit>
#include "QVaultNodeEdit.h"

class QVaultFolderNode : public QVaultNodeEdit {
    Q_OBJECT

public:
    static QString sFolderTypeNames[];
    static int MapFolderType(int type);
    static int UnmapFolderType(int idx);

protected:
    QComboBox* fFolderType;
    QLineEdit* fFolderName;

public:
    QVaultFolderNode(QWidget* parent);
    virtual QString getEditorTitle() const { return "Folder"; }
    virtual plVaultNode saveNode();

protected:
    virtual void IRefreshNode();
};

class QVaultPlayerInfoList : public QVaultFolderNode {
protected:
    Q_OBJECT

public:
    QVaultPlayerInfoList(QWidget* parent)
        : QVaultFolderNode(parent) { }
    virtual QString getEditorTitle() const { return "Player Info List"; }
};

class QVaultAgeInfoList : public QVaultFolderNode {
protected:
    Q_OBJECT

public:
    QVaultAgeInfoList(QWidget* parent)
        : QVaultFolderNode(parent) { }
    virtual QString getEditorTitle() const { return "Age Info List"; }
};

#endif
