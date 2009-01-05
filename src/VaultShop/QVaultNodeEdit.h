#ifndef _QVAULTNODEEDIT_H
#define _QVAULTNODEEDIT_H

#include <QWidget>
#include <Vault/plVaultNode.h>

class QVaultNodeEdit : public QWidget {
    Q_OBJECT

protected:
    plVaultNode fNode;

public:
    QVaultNodeEdit(QWidget* parent);
    static QVaultNodeEdit* MakeEditor(QWidget* parent, const plVaultNode& node);

    virtual QString getEditorTitle() const = 0;
    virtual plVaultNode saveNode() = 0;

protected:
    virtual void IRefreshNode() = 0;

public slots:
    void setNode(const plVaultNode& node);
};

#endif
