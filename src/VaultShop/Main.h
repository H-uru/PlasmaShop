#ifndef _VAULTSHOP_MAIN_H
#define _VAULTSHOP_MAIN_H

#include <QMainWindow>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QTabWidget>
#include <QAction>

#include <Vault/plVaultStore.h>
#include <list>

class VaultShopMain : public QMainWindow {
public:
    enum TreeRoles {
        kRoleNodeID = Qt::UserRole
    };

private:
    Q_OBJECT
    QTreeWidget* fVaultTree;
    QTabWidget* fNodeEditor;

    // Menu actions
    enum {
        kFileOpenVault, kFileSaveVault, kFileExit, kVaultOpenNode, kNumActions
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

public:
    VaultShopMain();
    void loadVault(QString filename);
    void loadNode(const plVaultNode& node, QTreeWidgetItem* parent, VaultInfo* vault);

protected:
    virtual void closeEvent(QCloseEvent* evt);
    VaultInfo* findCurrentVault();

public slots:
    void openFile();
    void performSave();
    void openNode();
    void treeItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous);
};

#endif
