#ifndef _QVAULTPLAYERINFONODE_H
#define _QVAULTPLAYERINFONODE_H

#include <QLineEdit>
#include <QSpinBox>
#include "QVaultNodeEdit.h"
#include "QLinkLabel.h"

class QVaultPlayerInfoNode : public QVaultNodeEdit {
    Q_OBJECT

protected:
    QLineEdit* fPlayerName;
    QSpinBox* fPlayerID;
    QLinkLabel* fPlayerLink;

public:
    QVaultPlayerInfoNode(QWidget* parent);
    virtual QString getEditorTitle() const;
    virtual plVaultNode saveNode();

protected:
    virtual void IRefreshNode();

protected slots:
    void subscribePlayer();
};

#endif
