#ifndef _VAULTSHOP_MAIN_H
#define _VAULTSHOP_MAIN_H

#include <QMainWindow>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QTabWidget>
#include <QAction>

#include <Vault/plVaultStore.h>
#include <SDL/plNetGameServerState.h>
#include <list>

#include "QVaultNode.h"
#include "QVaultNodeEdit.h"
#include "QGameServerState.h"

class VaultShopMain : public QMainWindow {
    Q_OBJECT

public:
    enum TreeRoles {
        kRoleNodeID = Qt::UserRole, kRoleSavFile,
    };

private:
    QString fLastDir;
    QTreeWidget* fVaultTree;
    QTabWidget* fNodeTab;
    QVaultNode* fGenericEditor;
    QVaultNodeEdit* fCustomEditor;
    QGameServerState* fSavEditor;
    int fEditorTabPreference;

    // Menu actions
    enum {
        kFileOpenVault, kFileSaveVault, kFileExit, kVaultOpenNode,
        kNodeUnLink, kNodeLink, kNodeCreate, kNodeUnsubscribe, kNodeRenameVault,
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

    struct SaveInfo {
        plNetGameServerState* fSave;
        QString fSaveFile;

        SaveInfo(QString filename);
        ~SaveInfo();
        void save();
    };
    std::list<SaveInfo*> fLoadedSaves;

    plResManager fResMgr;
    plSDLMgr fSDLMgr;

public:
    VaultShopMain();
    virtual ~VaultShopMain();
    void loadVault(QString filename, QString vaultName, QTreeWidgetItem* parent);
    QTreeWidgetItem* loadNode(const plVaultNode& node, QTreeWidgetItem* parent, VaultInfo* vault);
    void loadSaves(QString path, QTreeWidgetItem* parent);
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
    void subscribe(unsigned int nodeId);
    void tabChanged(int tabIdx);
    void typeModified();
    void treeItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous);
    void treeContextMenu(const QPoint& pos);
    void unlinkNode();
    void linkNode();
    void createNode();
    void closeNode();
    void refreshNode(unsigned int nodeId);
    //void renameVault();
};

#endif
