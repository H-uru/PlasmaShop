#ifndef _QVAULTNODEEDIT_H
#define _QVAULTNODEEDIT_H

#include <QWidget>
#include <Vault/plVaultNode.h>
#include <SDL/plSDLMgr.h>
#include <ResManager/plResManager.h>

class QVaultNodeEdit : public QWidget {
    Q_OBJECT

protected:
    plVaultNode fNode;

public:
    QVaultNodeEdit(QWidget* parent);
    static QVaultNodeEdit* MakeEditor(QWidget* parent, const plVaultNode& node,
                                      plResManager* mgr, plSDLMgr* sdl);

    virtual QString getEditorTitle() const = 0;
    virtual plVaultNode saveNode() = 0;
    virtual void setMgrs(plResManager*, plSDLMgr*);

protected:
    virtual void IRefreshNode() = 0;

public slots:
    void setNode(const plVaultNode& node);
};

#endif
