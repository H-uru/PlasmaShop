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
    virtual QString getEditorTitle() const;
    virtual plVaultNode saveNode();

protected:
    virtual void IRefreshNode();
};

class QVaultPlayerInfoList : public QVaultFolderNode {
protected:
    Q_OBJECT

public:
    QVaultPlayerInfoList(QWidget* parent);
    virtual QString getEditorTitle() const;
};

class QVaultAgeInfoList : public QVaultFolderNode {
protected:
    Q_OBJECT

public:
    QVaultAgeInfoList(QWidget* parent);
    virtual QString getEditorTitle() const;
};

#endif
