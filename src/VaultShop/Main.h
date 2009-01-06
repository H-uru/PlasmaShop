#ifndef _VAULTSHOP_MAIN_H
#define _VAULTSHOP_MAIN_H

#include <QMainWindow>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QTabWidget>
#include <QAction>

#include <Vault/plVaultStore.h>
#include <list>

#include "QVaultNode.h"
#include "QVaultNodeEdit.h"

class VaultShopMain : public QMainWindow {
    Q_OBJECT

public:
    enum TreeRoles {
        kRoleNodeID = Qt::UserRole
    };

private:
    QString fLastDir;
    QTreeWidget* fVaultTree;
    QTabWidget* fNodeTab;
    QVaultNode* fGenericEditor;
    QVaultNodeEdit* fCustomEditor;
    int fEditorTabPreference;

    // Menu actions
    enum {
        kFileOpenVault, kFileSaveVault, kFileExit, kVaultOpenNode,
        kNodeUnLink, kNodeLink, kNodeCreate, kNodeUnsubscribe,
        kNumActions
    };
    QAction* fActions[kNumActions];

    // libPlasma stuff
    struct VaultInfo {
        plVaultStore* fVault;
        QString fVaultFile;
        QTreeWidgetItem* fRootItem;

        VaultInfo(QString filename);
        ~VaultInfo();
        void save();
    };
    std::list<VaultInfo*> fLoadedVaults;
    plResManager fResMgr;
    plSDLMgr fSDLMgr;

public:
    VaultShopMain();
    virtual ~VaultShopMain();
    void loadVault(QString filename, QString vaultName);
    void loadNode(const plVaultNode& node, QTreeWidgetItem* parent, VaultInfo* vault);
    void loadGame(QString path);

signals:
    void nodeChanged(unsigned int nodeId);

protected:
    virtual void closeEvent(QCloseEvent* evt);
    VaultInfo* findCurrentVault(QTreeWidgetItem* item = NULL);
    QList<QTreeWidgetItem*> findNodeItems(unsigned int nodeId, QTreeWidgetItem* parent);
    void saveNode(QTreeWidgetItem* nodeItem, int source);
    void updateNode(QTreeWidgetItem* item, const plVaultNode& node);

public slots:
    void openGame();
    void performSave();
    void openNode();
    void tabChanged(int tabIdx);
    void typeModified();
    void treeItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous);
    void treeContextMenu(const QPoint& pos);
    void unlinkNode();
    void linkNode();
    void createNode();
    void closeNode();
    void refreshNode(unsigned int nodeId);
};

#endif
