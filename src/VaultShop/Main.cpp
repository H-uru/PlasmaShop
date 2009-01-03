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
    fActions[kVaultOpenNode]->setShortcut(Qt::Key_F2);

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
    fVaultNodeEditor = new QVaultNode(fNodeEditor);
    fNodeEditor->addTab(fVaultNodeEditor, tr("Node Properties"));

    // Layout
    splitter->addWidget(fVaultTree);
    splitter->addWidget(fNodeEditor);
    setCentralWidget(splitter);
    splitter->setSizes(QList<int>() << 160 << 320);

    // Global UI Signals
    QObject::connect(fActions[kFileExit], SIGNAL(activated()), this, SLOT(close()));
    QObject::connect(fActions[kFileOpenVault], SIGNAL(activated()), this, SLOT(openGame()));
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

    if (settings.contains("WinState"))
        restoreState(settings.value("WinState").toByteArray());
}

VaultShopMain::~VaultShopMain()
{
    std::list<VaultInfo*>::iterator it;
    for (it = fLoadedVaults.begin(); it != fLoadedVaults.end(); it++)
        delete *it;
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

VaultShopMain::VaultInfo* VaultShopMain::findCurrentVault(QTreeWidgetItem* item)
{
    if (item == NULL)
        item = fVaultTree->currentItem();
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

plVaultNode VaultShopMain::saveNode(QTreeWidgetItem* nodeItem)
{
    if (nodeItem == NULL || nodeItem->data(0, kRoleNodeID).toInt() < 0)
        return plVaultNode();

    VaultInfo* vault = findCurrentVault(nodeItem);
    if (vault == NULL) {
        fVaultNodeEditor->setNode(plVaultNode());
        return plVaultNode();
    }
    plVaultNode update = vault->fVault->addNode(fVaultNodeEditor->saveNode());
    updateNode(nodeItem, update);
    return update;
}

void VaultShopMain::updateNode(QTreeWidgetItem* item, const plVaultNode& node)
{
    item->setText(0, GetNodeDisplay(node));
    item->setIcon(0, GetNodeTypeIcon(node.getNodeType()));
    item->setData(0, kRoleNodeID, node.getNodeID());
    item->setStatusTip(0, QString("%1").arg(node.getNodeID()));
}

void VaultShopMain::treeItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous)
{
    if (current != NULL)
        statusBar()->showMessage(current->statusTip(0));
    saveNode(previous);

    if (current != NULL && current->data(0, kRoleNodeID).toInt() > 0) {
        VaultInfo* vault = findCurrentVault(current);
        if (vault == NULL)
            fVaultNodeEditor->setNode(plVaultNode());
        else
            fVaultNodeEditor->setNode(vault->fVault->getNode(current->data(0, kRoleNodeID).toInt()));
    } else {
        fVaultNodeEditor->setNode(plVaultNode());
    }
}

void VaultShopMain::openGame()
{
    static QString s_gameDir;

    QString dirname = QFileDialog::getExistingDirectory(this,
                                   tr("Open Game"), s_gameDir);
    if (!dirname.isEmpty()) {
        fSDLMgr.ClearDescriptors();
        std::list<VaultInfo*>::iterator it;
        for (it = fLoadedVaults.begin(); it != fLoadedVaults.end(); it++)
            delete *it;
        fLoadedVaults.clear();

        if (QFile::exists(dirname + "/sav/vault.dat"))
            loadVault(dirname + "/sav/vault.dat", "Vault Root");
        QDir vdir(dirname + "/sav/");
        if (vdir.exists()) {
            QStringList vaultList = vdir.entryList(
                QDir::Dirs | QDir::NoDotAndDotDot | QDir::Readable | QDir::Hidden);
            QStringList::const_iterator it;
            for (it = vaultList.constBegin(); it != vaultList.constEnd(); it++) {
                QString vaultPath = dirname + "/sav/" + *it + "/current/vault.dat";
                try {
                    if (QFile::exists(vaultPath))
                        loadVault(vaultPath, QString("Vault %1").arg(*it));
                } catch (hsException ex) {
                    QMessageBox msgBox(QMessageBox::Critical, tr("Error"),
                                       tr("Error loading vault %1: %2")
                                       .arg(*it).arg(ex.what()),
                                       QMessageBox::Ok, this);
                    plDebug::Error("%s:%d: %s", ex.File(), ex.Line(), ex.what());
                    msgBox.exec();
                }
            }
        }

        QDir sdldir(dirname + "/SDL/");
        if (sdldir.exists()) {
            QStringList sdlList = sdldir.entryList(QStringList() << "*.sdl",
                QDir::Files | QDir::Readable | QDir::Hidden);
            QStringList::const_iterator it;
            for (it = sdlList.constBegin(); it != sdlList.constEnd(); it++) {
                QString sdlPath = dirname + "/SDL/" + *it;
                try {
                    if (QFile::exists(sdlPath))
                        fSDLMgr.ReadDescriptors(sdlPath.toUtf8().data());
                } catch (hsException ex) {
                    QMessageBox msgBox(QMessageBox::Critical, tr("Error"),
                                       tr("Error parsing %1: %2")
                                       .arg(*it).arg(ex.what()),
                                       QMessageBox::Ok, this);
                    plDebug::Error("%s:%d: %s", ex.File(), ex.Line(), ex.what());
                    msgBox.exec();
                }
            }
        }

        if (fVaultTree->topLevelItemCount() == 0) {
            QMessageBox msgBox(QMessageBox::Critical, tr("Error"),
                               tr("No vaults were found to load"),
                               QMessageBox::Ok, this);
            msgBox.exec();
        }

        s_gameDir = dirname;
    }
}

void VaultShopMain::loadVault(QString filename, QString vaultName)
{
    // Fix filename to contain the absolute path >.<
    QDir dir(filename);
    filename = dir.absolutePath();

    VaultInfo* vault = new VaultInfo(filename);
    vault->fRootItem = new QTreeWidgetItem(fVaultTree, QStringList() << vaultName);
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
    QTreeWidgetItem* item = new QTreeWidgetItem(parent);
    updateNode(item, node);

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
                           tr("Node %1 does not exist in %2")
                           .arg(nodeId).arg(vault->fRootItem->text(0)),
                           QMessageBox::Ok, this);
        msgBox.exec();
        return;
    }
}

void VaultShopMain::performSave()
{
    // Save the current node:
    plVaultNode update = saveNode(fVaultTree->currentItem());
    fVaultNodeEditor->setNode(update);

    // Save all vaults to their files
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
    return app.exec();
}
