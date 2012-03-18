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

#ifndef _QVAULTNODE_H
#define _QVAULTNODE_H

#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QPlainTextEdit>
#include <Vault/plVaultNode.h>

class QVaultNode : public QWidget {
    Q_OBJECT

public:
    static QString sNodeTypeNames[];
    static int MapNodeType(unsigned int nodeType);
    static unsigned int UnmapNodeType(int idx);

private:
    QLabel* fAutoTime;
    QLabel* fModifyTime;
    QLabel* fAgeTime;
    QLineEdit* fAgeName;
    QLineEdit* fAgeGUID;

    QLineEdit* fCreatorID;
    QLineEdit* fOwnerID;
    QLineEdit* fGroupID;

    QCheckBox* fOwnerRead;
    QCheckBox* fOwnerWrite;
    QCheckBox* fGroupRead;
    QCheckBox* fGroupWrite;
    QCheckBox* fOtherRead;
    QCheckBox* fOtherWrite;
    QComboBox* fNodeType;

    QLineEdit* fInt32_1;
    QLineEdit* fInt32_2;
    QLineEdit* fInt32_3;
    QLineEdit* fInt32_4;
    QLineEdit* fUInt32_1;
    QLineEdit* fUInt32_2;
    QLineEdit* fUInt32_3;
    QLineEdit* fUInt32_4;

    QLineEdit* fString64_1;
    QLineEdit* fString64_2;
    QLineEdit* fString64_3;
    QLineEdit* fString64_4;
    QLineEdit* fString64_5;
    QLineEdit* fString64_6;
    QLineEdit* fIString64_1;
    QLineEdit* fIString64_2;

    QPlainTextEdit* fText_1;
    QPlainTextEdit* fText_2;

    QLabel* fBlob1_Info;
    QLabel* fBlob2_Info;

    plVaultNode fNode;
    bool fSignalLock;

public:
    QVaultNode(QWidget* parent = NULL);
    plVaultNode saveNode();

signals:
    void typeModified();

public slots:
    void setNode(const plVaultNode& node);
    void typeChanged(int);
};

QIcon GetNodeTypeIcon(unsigned int nodeType);
QString GetNodeDisplay(const plVaultNode& node);
QString GetFolderName(const plVaultNode& node);

#endif
