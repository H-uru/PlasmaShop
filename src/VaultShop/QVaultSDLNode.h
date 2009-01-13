#ifndef _QVAULTSDLNODE_H
#define _QVAULTSDLNODE_H

#include <QComboBox>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QTreeWidget>
#include <QSpinBox>
#include <QGridLayout>
#include <SDL/plSDLMgr.h>
#include <SDL/plStateVariable.h>
#include <SDL/plStateDataRecord.h>
#include <ResManager/plResManager.h>
#include "QVaultNodeEdit.h"
#include "../QColorEdit.h"

class QSDLEditor : public QWidget {
    Q_OBJECT

public:
    static QString GetVarDisplay(plStateVariable* var);
    static QString GetSDLName(plVaultBlob blob);

private:
    plStateDataRecord fRecord;
    plResManager* fResMgr;
    plSDLMgr* fSDLMgr;
    plString fSDLName;
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

public:
    QSDLEditor(QWidget* parent);
    void setMgrs(plResManager* mgr, plSDLMgr* sdl);

    void loadBlob(plVaultBlob blob);
    plVaultBlob saveBlob();

private:
    void addVar(plStateVariable* var, QTreeWidgetItem* parent);
    void setupVarEditorCommon(plStateVariable* var);
    void setVarCustomEdit(QTreeWidgetItem* item, int which);
    void saveVarCustomEdit(QTreeWidgetItem* item, int which);

private slots:
    void itemSelected(QTreeWidgetItem* current, QTreeWidgetItem* previous);
    void indexChanged(int idx);
};


class QVaultSDLNode : public QVaultNodeEdit {
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
    virtual QString getEditorTitle() const;
    virtual plVaultNode saveNode();
    virtual void setMgrs(plResManager*, plSDLMgr*);

protected:
    virtual void IRefreshNode();
};

#endif
