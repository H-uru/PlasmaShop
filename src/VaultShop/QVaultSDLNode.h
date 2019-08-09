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

#ifndef _QVAULTSDLNODE_H
#define _QVAULTSDLNODE_H

#include <QComboBox>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QTreeWidget>
#include <QSpinBox>
#include <QGridLayout>
#include <QDateTimeEdit>
#include <SDL/plSDLMgr.h>
#include <SDL/plStateVariable.h>
#include <SDL/plStateDataRecord.h>
#include <ResManager/plResManager.h>
#include "QVaultNodeEdit.h"
#include "QColorEdit.h"

class QSDLEditor : public QWidget
{
    Q_OBJECT

public:
    static QString GetVarDisplay(plStateVariable* var);
    static QString GetSDLName(plVaultBlob blob);

private:
    plStateDataRecord* fRecord;
    bool fIOwnRecord;
    plResManager* fResMgr;
    plSDLMgr* fSDLMgr;
    ST::string fSDLName;
    int fSDLVersion;

    QTreeWidget* fSDLList;
    QWidget* fEditorPanel;
    QGridLayout* fEditorLayout;
    QSpinBox* fEditorWhich;
    int fCurrentWhich;
    QComboBox* fComboEdit;
    QSpinBox* fIntEdit;
    QLineEdit* fStringEdit;
    QSpinBox* fLocationEdit[2];
    QColorEdit* fColorEdit;
    QDateTimeEdit* fDTEdit;
    QLineEdit* fGeomEdit[4];

public:
    QSDLEditor(QWidget* parent);
    ~QSDLEditor();
    void setMgrs(plResManager* mgr, plSDLMgr* sdl);

    void loadBlob(plVaultBlob blob);
    plVaultBlob saveBlob();
    void setRecord(plStateDataRecord* rec, bool own=false);

private:
    void addVar(plStateVariable* var, QTreeWidgetItem* parent);
    void setupVarEditorCommon(plStateVariable* var);
    void setVarCustomEdit(QTreeWidgetItem* item, int which);
    void saveVarCustomEdit(QTreeWidgetItem* item, int which);

private slots:
    void itemSelected(QTreeWidgetItem* current, QTreeWidgetItem* previous);
    void indexChanged(int idx);
};


class QVaultSDLNode : public QVaultNodeEdit
{
    Q_OBJECT

public:
    static QString sSDLTypeNames[];
    static int MapSDLType(int type);
    static int UnmapSDLType(int idx);

protected:
    QComboBox* fSDLType;
    QSDLEditor* fSDLEditor;

public:
    QVaultSDLNode(QWidget* parent);
    QString getEditorTitle() const override { return "SDL"; }
    plVaultNode saveNode() override;

    void setMgrs(plResManager* mgr, plSDLMgr* sdl) override
    {
        fSDLEditor->setMgrs(mgr, sdl);
    }

protected:
    void IRefreshNode() override;
};

#endif
