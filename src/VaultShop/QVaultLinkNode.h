#ifndef _QVAULTLINKNODE_H
#define _QVAULTLINKNODE_H

#include <QCheckBox>
#include <QLineEdit>
#include "QVaultNodeEdit.h"

class QVaultLinkNode : public QVaultNodeEdit {
    Q_OBJECT

protected:
    QCheckBox* fUnlocked;
    QCheckBox* fVolatile;
    QLineEdit* fSpawnPoints;

public:
    QVaultLinkNode(QWidget* parent);
    virtual QString getEditorTitle() const;
    virtual plVaultNode saveNode();

protected:
    virtual void IRefreshNode();
};

#endif
