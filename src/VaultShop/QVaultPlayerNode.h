#ifndef _QVAULTPLAYERNODE_H
#define _QVAULTPLAYERNODE_H

#include <QLineEdit>
#include <QComboBox>
#include "QVaultNodeEdit.h"

class QVaultPlayerNode : public QVaultNodeEdit {
    Q_OBJECT

protected:
    QLineEdit* fPlayerName;
    QLineEdit* fAvatarShape;
    QLineEdit* fAccountUUID;

public:
    QVaultPlayerNode(QWidget* parent);
    virtual QString getEditorTitle() const;
    virtual plVaultNode saveNode();

protected:
    virtual void IRefreshNode();
};

#endif
