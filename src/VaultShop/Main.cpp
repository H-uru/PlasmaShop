#include <QApplication>
#include <QSettings>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QSplitter>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <Debug/plDebug.h>
#include <ResManager/plFactory.h>

#include "Main.h"
#include "QVaultNode.h"

/* VaultInfo */
VaultShopMain::VaultInfo::VaultInfo(QString filename)
             : fVaultFile(filename), fRootItem(NULL)
{
    fVault = new plVaultStore();
    fVault->ImportFile(filename.toUtf8().data());
}

VaultShopMain::VaultInfo::~VaultInfo()
{
    if (fVault != NULL)
        delete fVault;
    if (fRootItem != NULL)
        delete fRootItem;
}

void VaultShopMain::VaultInfo::save()
{
    fVault->ExportFile(fVaultFile.toUtf8().data());
}


/* VaultShopMain */
VaultShopMain::VaultShopMain()
{
    // Basic Form Settings
    setWindowTitle("VaultShop 1.0");
    //setWindowIcon(QIcon(":/res/VaultShop.png"));

    // Set up actions
    fActions[kFileOpenVault] = new QAction(QIcon(":/res/fileopen.png"), tr("&Load Vault..."), this);
    fActions[kFileSaveVault] = new QAction(QIcon(":/res/filesave.png"), tr("&Save Vault"), this);
    fActions[kFileExit] = new QAction(tr("E&xit"), this);
    fActions[kVaultOpenNode] = new QAction(tr("Subscribe to &Node..."), this);

    fActions[kFileOpenVault]->setShortcut(Qt::CTRL + Qt::Key_O);
    fActions[kFileSaveVault]->setShortcut(Qt::CTRL + Qt::Key_S);
    fActions[kFileExit]->setShortcut(Qt::ALT + Qt::Key_F4);

    // Main Menus
    QMenu* fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(fActions[kFileOpenVault]);
    fileMenu->addAction(fActions[kFileSaveVault]);
    fileMenu->addSeparator();
    fileMenu->addAction(fActions[kFileExit]);

    QMenu* vaultMenu = menuBar()->addMenu(tr("&Vault"));
    vaultMenu->addAction(fActions[kVaultOpenNode]);

    // Toolbars
    QToolBar* fileTbar = addToolBar(tr("File Toolbar"));
    fileTbar->setObjectName("FileToolBar");
    fileTbar->addAction(fActions[kFileOpenVault]);
    fileTbar->addAction(fActions[kFileSaveVault]);
    statusBar();

    // Main Splitter
    QSplitter* splitter = new QSplitter(Qt::Horizontal, this);
    splitter->setObjectName("Splitter");

    // Node Browser
    fVaultTree = new QTreeWidget(splitter);
    fVaultTree->setUniformRowHeights(true);
    fVaultTree->setHeaderHidden(true);

    // Property Editor
    fNodeEditor = new QTabWidget(splitter);
    //fNodeEditor->addTab(..., tr("Node Properties"));

    // Layout
    splitter->addWidget(fVaultTree);
    splitter->addWidget(fNodeEditor);
    setCentralWidget(splitter);
    splitter->setSizes(QList<int>() << 160 << 320);

    // Global UI Signals
    QObject::connect(fActions[kFileExit], SIGNAL(activated()), this, SLOT(close()));
    QObject::connect(fActions[kFileOpenVault], SIGNAL(activated()), this, SLOT(openFile()));
    QObject::connect(fActions[kFileSaveVault], SIGNAL(activated()), this, SLOT(performSave()));
    QObject::connect(fActions[kVaultOpenNode], SIGNAL(activated()), this, SLOT(openNode()));

    QObject::connect(fVaultTree, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)),
                     this, SLOT(treeItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)));

    // Load UI Settings
    QSettings settings("PlasmaShop", "VaultShop");
    resize(settings.value("WinSize", QSize(480, 600)).toSize());
    if (settings.contains("WinPos"))
        move(settings.value("WinPos").toPoint());
    if (settings.value("WinMaximized", false).toBool())
        showMaximized();

    //if (settings.contains("WinState"))
    //    restoreState(settings.value("WinState").toByteArray());
}

void VaultShopMain::closeEvent(QCloseEvent*)
{
    // Save UI Settings
    QSettings settings("PlasmaShop", "VaultShop");
    settings.setValue("WinMaximized", (windowState() & Qt::WindowMaximized) != 0);
    showNormal();
    settings.setValue("WinSize", size());
    settings.setValue("WinPos", pos());
    settings.setValue("WinState", saveState());
}

VaultShopMain::VaultInfo* VaultShopMain::findCurrentVault()
{
    QTreeWidgetItem* item = fVaultTree->currentItem();
    if (item == NULL)
        return NULL;
    while (item->parent() != NULL)
        item = item->parent();
    std::list<VaultInfo*>::iterator it;
    for (it = fLoadedVaults.begin(); it != fLoadedVaults.end(); it++) {
        if ((*it)->fRootItem == item)
            return *it;
    }
    return NULL;
}

void VaultShopMain::treeItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous)
{
    statusBar()->showMessage(current->statusTip(0));
}

void VaultShopMain::openFile()
{
    QString filename = QFileDialog::getOpenFileName(this,
                            tr("Open Vault"), QString(),
                            "Vault Data Files (vault.dat)");
    if (!filename.isEmpty())
        loadVault(filename);
}

void VaultShopMain::loadVault(QString filename)
{
    // Fix filename to contain the absolute path >.<
    QDir dir(filename);
    filename = dir.absolutePath();

    VaultInfo* vault = new VaultInfo(filename);
    vault->fRootItem = new QTreeWidgetItem(fVaultTree, QStringList() << "Vault Root");
    vault->fRootItem->setIcon(0, QIcon(":/img/rfldr.png"));
    vault->fRootItem->setData(0, kRoleNodeID, -1);
    vault->fRootItem->setStatusTip(0, filename);
    fLoadedVaults.push_back(vault);

    plVaultNode node = vault->fVault->getNode(vault->fVault->getFirstNodeID());
    if (node.isValid())
        loadNode(node, vault->fRootItem, vault);
}

void VaultShopMain::loadNode(const plVaultNode& node, QTreeWidgetItem* parent,
                             VaultInfo* vault)
{
    QTreeWidgetItem* item = new QTreeWidgetItem(parent, QStringList() << GetNodeDisplay(node));
    item->setIcon(0, GetNodeTypeIcon(node));
    item->setData(0, kRoleNodeID, node.getNodeID());
    item->setStatusTip(0, QString("%1").arg(node.getNodeID()));

    std::vector<plVaultNode> children = vault->fVault->getChildren(node.getNodeID());
    for (size_t i=0; i<children.size(); i++)
        loadNode(children[i], item, vault);
}

void VaultShopMain::openNode()
{
    VaultInfo* vault = findCurrentVault();
    if (vault == NULL) {
        QMessageBox msgBox(QMessageBox::Critical, tr("Error"),
                           tr("No vault is active"),
                           QMessageBox::Ok, this);
        msgBox.exec();
        return;
    }

    bool ok;
    int nodeId = QInputDialog::getInteger(this, tr("Node ID"),
                                          tr("Enter the ID of the Node to subscribe to"),
                                          0, 0, 0x7FFFFFFF, 1, &ok);
    if (!ok)
        return;
    plVaultNode node = vault->fVault->getNode(nodeId);
    if (node.isValid()) {
        loadNode(node, vault->fRootItem, vault);
    } else {
        QMessageBox msgBox(QMessageBox::Critical, tr("Error"),
                           tr("Vault Node %1 does not exist").arg(nodeId),
                           QMessageBox::Ok, this);
        msgBox.exec();
        return;
    }
}

void VaultShopMain::performSave()
{
    std::list<VaultInfo*>::iterator it;
    for (it = fLoadedVaults.begin(); it != fLoadedVaults.end(); it++)
        (*it)->save();
}

int main(int argc, char* argv[])
{
    // Redirect libPlasma's debug stuff to VaultShop.log
    plDebug::InitFile(plDebug::kDLAll, "VaultShop.log");

    QApplication app(argc, argv);
    VaultShopMain mainWnd;
    mainWnd.show();
    for (int i=1; i<argc; i++)
        mainWnd.loadVault(argv[i]);
    return app.exec();
}
