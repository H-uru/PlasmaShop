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

    fActions[kNodeUnLink] = new QAction(tr("Remove"), this);
    fActions[kNodeLink] = new QAction(tr("Add Node..."), this);
    fActions[kNodeCreate] = new QAction(tr("Create Node"), this);
    fActions[kNodeUnsubscribe] = new QAction(tr("Un-subscribe"), this);

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
    fVaultTree->setContextMenuPolicy(Qt::CustomContextMenu);

    // Property Editor
    fNodeTab = new QTabWidget(splitter);
    fGenericEditor = new QVaultNode(fNodeTab);
    fNodeTab->addTab(fGenericEditor, tr("Node Properties"));
    fCustomEditor = NULL;
    fEditorTabPreference = 0;

    // Layout
    splitter->addWidget(fVaultTree);
    splitter->addWidget(fNodeTab);
    setCentralWidget(splitter);
    splitter->setSizes(QList<int>() << 160 << 320);

    // Global UI Signals
    connect(fActions[kFileExit], SIGNAL(activated()), this, SLOT(close()));
    connect(fActions[kFileOpenVault], SIGNAL(activated()), this, SLOT(openGame()));
    connect(fActions[kFileSaveVault], SIGNAL(activated()), this, SLOT(performSave()));
    connect(fActions[kVaultOpenNode], SIGNAL(activated()), this, SLOT(openNode()));
    connect(fActions[kNodeUnLink], SIGNAL(activated()), this, SLOT(unlinkNode()));
    connect(fActions[kNodeLink], SIGNAL(activated()), this, SLOT(linkNode()));
    connect(fActions[kNodeCreate], SIGNAL(activated()), this, SLOT(createNode()));
    connect(fActions[kNodeUnsubscribe], SIGNAL(activated()), this, SLOT(closeNode()));

    connect(fVaultTree, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)),
            this, SLOT(treeItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)));
    connect(fVaultTree, SIGNAL(customContextMenuRequested(const QPoint&)),
            this, SLOT(treeContextMenu(const QPoint&)));
    connect(fNodeTab, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));
    connect(fGenericEditor, SIGNAL(typeModified()), this, SLOT(typeModified()));
    connect(this, SIGNAL(nodeChanged(unsigned int)), this, SLOT(refreshNode(unsigned int)));

    // Load UI Settings
    QSettings settings("PlasmaShop", "VaultShop");
    resize(settings.value("WinSize", QSize(480, 600)).toSize());
    if (settings.contains("WinPos"))
        move(settings.value("WinPos").toPoint());
    if (settings.value("WinMaximized", false).toBool())
        showMaximized();

    if (settings.contains("WinState"))
        restoreState(settings.value("WinState").toByteArray());

    fLastDir = settings.value("LastDir").toString();
    if (!fLastDir.isEmpty())
        loadGame(fLastDir);
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
    settings.setValue("LastDir", fLastDir);
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

QList<QTreeWidgetItem*> VaultShopMain::findNodeItems(unsigned int nodeId, QTreeWidgetItem* parent)
{
    QList<QTreeWidgetItem*> items;
    for (int i=0; i<parent->childCount(); i++) {
        items += findNodeItems(nodeId, parent->child(i));
        if (parent->child(i)->data(0, kRoleNodeID).toInt() == (int)nodeId)
            items << parent->child(i);
    }
    return items;
}

void VaultShopMain::saveNode(QTreeWidgetItem* nodeItem, int source)
{
    if (nodeItem == NULL || nodeItem->data(0, kRoleNodeID).toInt() < 0)
        return;

    VaultInfo* vault = findCurrentVault(nodeItem);
    if (vault == NULL)
        return;

    plVaultNode update;
    if (fCustomEditor != NULL && source == 0) {
        update = vault->fVault->addNode(fCustomEditor->saveNode());
        fGenericEditor->setNode(update);
    } else {
        update = vault->fVault->addNode(fGenericEditor->saveNode());
        if (fCustomEditor != NULL)
            fCustomEditor->setNode(update);
    }

    QList<QTreeWidgetItem*> nodeItems = findNodeItems(update.getNodeID(), vault->fRootItem);
    QList<QTreeWidgetItem*>::const_iterator it;
    for (it = nodeItems.constBegin(); it != nodeItems.constEnd(); it++)
        updateNode(*it, update);
}

void VaultShopMain::updateNode(QTreeWidgetItem* item, const plVaultNode& node)
{
    item->setText(0, GetNodeDisplay(node));
    item->setIcon(0, GetNodeTypeIcon(node.getNodeType()));
    item->setData(0, kRoleNodeID, node.getNodeID());
    item->setStatusTip(0, QString("%1").arg(node.getNodeID()));
}

void VaultShopMain::tabChanged(int tabIdx)
{
    if (tabIdx == 0)
        saveNode(fVaultTree->currentItem(), 1);
    else
        saveNode(fVaultTree->currentItem(), 0);
}

void VaultShopMain::typeModified()
{
    saveNode(fVaultTree->currentItem(), fNodeTab->currentIndex());
    if (fCustomEditor != NULL) {
        fEditorTabPreference = fNodeTab->currentIndex();
        fNodeTab->removeTab(0);
        disconnect(fCustomEditor, 0, 0, 0);
        delete fCustomEditor;
        fCustomEditor = NULL;
    }

    QTreeWidgetItem* current = fVaultTree->currentItem();
    VaultInfo* vault = findCurrentVault(current);
    if (vault != NULL) {
        plVaultNode node = vault->fVault->getNode(current->data(0, kRoleNodeID).toInt());
        fCustomEditor = QVaultNodeEdit::MakeEditor(fNodeTab, node, &fResMgr, &fSDLMgr);
        if (fCustomEditor != NULL) {
            fNodeTab->insertTab(0, fCustomEditor, fCustomEditor->getEditorTitle());
            fNodeTab->setCurrentIndex(fEditorTabPreference);
        }
    }
}

void VaultShopMain::treeItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous)
{
    saveNode(previous, fNodeTab->currentIndex());

    if (current != NULL)
        statusBar()->showMessage(current->statusTip(0));
    if (fCustomEditor != NULL) {
        fEditorTabPreference = fNodeTab->currentIndex();
        fNodeTab->removeTab(0);
        disconnect(fCustomEditor, 0, 0, 0);
        delete fCustomEditor;
        fCustomEditor = NULL;
    }

    if (current != NULL && current->data(0, kRoleNodeID).toInt() > 0) {
        VaultInfo* vault = findCurrentVault(current);
        if (vault == NULL) {
            fGenericEditor->setNode(plVaultNode());
        } else {
            plVaultNode node = vault->fVault->getNode(current->data(0, kRoleNodeID).toInt());
            fGenericEditor->setNode(node);
            fCustomEditor = QVaultNodeEdit::MakeEditor(fNodeTab, node, &fResMgr, &fSDLMgr);
            if (fCustomEditor != NULL) {
                fNodeTab->insertTab(0, fCustomEditor, fCustomEditor->getEditorTitle());
                fNodeTab->setCurrentIndex(fEditorTabPreference);
            }
        }
    } else {
        fGenericEditor->setNode(plVaultNode());
    }
}

void VaultShopMain::loadGame(QString path)
{
    if (fCustomEditor != NULL) {
        fEditorTabPreference = fNodeTab->currentIndex();
        fNodeTab->removeTab(0);
        disconnect(fCustomEditor, 0, 0, 0);
        delete fCustomEditor;
        fCustomEditor = NULL;
    }

    fSDLMgr.ClearDescriptors();
    std::list<VaultInfo*>::iterator it;
    for (it = fLoadedVaults.begin(); it != fLoadedVaults.end(); it++)
        delete *it;
    fLoadedVaults.clear();

    if (QFile::exists(path + "/sav/vault.dat"))
        loadVault(path + "/sav/vault.dat", "Vault Root");
    QDir vdir(path + "/sav/");
    if (vdir.exists()) {
        QStringList vaultList = vdir.entryList(
            QDir::Dirs | QDir::NoDotAndDotDot | QDir::Readable | QDir::Hidden);
        QStringList::const_iterator it;
        for (it = vaultList.constBegin(); it != vaultList.constEnd(); it++) {
            QString vaultPath = path + "/sav/" + *it + "/current/vault.dat";
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

    QDir sdldir(path + "/SDL/");
    if (sdldir.exists()) {
        QStringList sdlList = sdldir.entryList(QStringList() << "*.sdl",
            QDir::Files | QDir::Readable | QDir::Hidden);
        QStringList::const_iterator it;
        for (it = sdlList.constBegin(); it != sdlList.constEnd(); it++) {
            QString sdlPath = path + "/SDL/" + *it;
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

    fLastDir = path;
}

void VaultShopMain::openGame()
{
    QString dirname = QFileDialog::getExistingDirectory(this,
                                   tr("Open Game"), fLastDir);
    if (!dirname.isEmpty())
        loadGame(dirname);
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
    for (int i=0; i<parent->childCount(); i++) {
        if (parent->child(i)->data(0, kRoleNodeID).toInt() == (int)node.getNodeID())
            return;
    }

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
    saveNode(fVaultTree->currentItem(), fNodeTab->currentIndex());

    // Save all vaults to their files
    std::list<VaultInfo*>::iterator it;
    for (it = fLoadedVaults.begin(); it != fLoadedVaults.end(); it++)
        (*it)->save();
}

void VaultShopMain::treeContextMenu(const QPoint& pos)
{
    fVaultTree->setCurrentItem(fVaultTree->itemAt(pos));
    if (fVaultTree->currentItem() == NULL)
        return;

    QMenu menu(this);
    menu.addAction(fActions[kNodeUnsubscribe]);
    menu.addAction(fActions[kNodeUnLink]);
    menu.addAction(fActions[kNodeLink]);
    menu.addAction(fActions[kNodeCreate]);
    if (fVaultTree->currentItem()->data(0, kRoleNodeID).toInt() < 0) {
        fActions[kNodeUnsubscribe]->setEnabled(false);
        fActions[kNodeUnsubscribe]->setVisible(false);
        fActions[kNodeUnLink]->setEnabled(false);
        fActions[kNodeLink]->setEnabled(false);
    } else if (fVaultTree->currentItem()->parent() != NULL &&
               fVaultTree->currentItem()->parent()->data(0, kRoleNodeID).toInt() < 0) {
        fActions[kNodeUnsubscribe]->setEnabled(true);
        fActions[kNodeUnsubscribe]->setVisible(true);
        fActions[kNodeUnLink]->setEnabled(false);
        fActions[kNodeLink]->setEnabled(true);
    } else {
        fActions[kNodeUnsubscribe]->setEnabled(false);
        fActions[kNodeUnsubscribe]->setVisible(false);
        fActions[kNodeUnLink]->setEnabled(true);
        fActions[kNodeLink]->setEnabled(true);
    }
    menu.exec(fVaultTree->viewport()->mapToGlobal(pos));
}

void VaultShopMain::unlinkNode()
{
    VaultInfo* vault = findCurrentVault();
    if (vault == NULL) {
        QMessageBox msgBox(QMessageBox::Critical, tr("Error"),
                           tr("No vault is active"),
                           QMessageBox::Ok, this);
        msgBox.exec();
        return;
    }

    unsigned int nodeId = fVaultTree->currentItem()->data(0, kRoleNodeID).toInt();
    unsigned int parent = fVaultTree->currentItem()->parent()->data(0, kRoleNodeID).toInt();
    vault->fVault->delRef(parent, nodeId);
    emit nodeChanged(parent);
}

void VaultShopMain::linkNode()
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
                                          tr("Enter the ID of the Node to link"),
                                          0, 0, 0x7FFFFFFF, 1, &ok);
    if (!ok)
        return;
    plVaultNode node = vault->fVault->getNode(nodeId);
    if (node.isValid()) {
        unsigned int parent = fVaultTree->currentItem()->data(0, kRoleNodeID).toInt();
        vault->fVault->addRef(parent, nodeId);
        emit nodeChanged(parent);
    } else {
        QMessageBox msgBox(QMessageBox::Critical, tr("Error"),
                           tr("Node %1 does not exist in %2")
                           .arg(nodeId).arg(vault->fRootItem->text(0)),
                           QMessageBox::Ok, this);
        msgBox.exec();
        return;
    }
}

void VaultShopMain::createNode()
{
    VaultInfo* vault = findCurrentVault();
    if (vault == NULL) {
        QMessageBox msgBox(QMessageBox::Critical, tr("Error"),
                           tr("No vault is active"),
                           QMessageBox::Ok, this);
        msgBox.exec();
        return;
    }

    plVaultNode node = vault->fVault->addNode(plVaultNode());
    unsigned int parent = fVaultTree->currentItem()->data(0, kRoleNodeID).toInt();
    if (parent > 0) {
        vault->fVault->addRef(parent, node.getNodeID());
        emit nodeChanged(parent);
    } else {
        loadNode(node, fVaultTree->currentItem(), vault);
    }
}

void VaultShopMain::closeNode()
{
    delete fVaultTree->currentItem();
}

void VaultShopMain::refreshNode(unsigned int nodeId)
{
    VaultInfo* vault = findCurrentVault();
    if (vault == NULL) {
        QMessageBox msgBox(QMessageBox::Critical, tr("Error"),
                           tr("No vault is active"),
                           QMessageBox::Ok, this);
        msgBox.exec();
        return;
    }

    QList<QTreeWidgetItem*> nodeItems = findNodeItems(nodeId, vault->fRootItem);
    QList<QTreeWidgetItem*>::const_iterator it;
    plVaultNode node = vault->fVault->getNode(nodeId);
    for (it = nodeItems.constBegin(); it != nodeItems.constEnd(); it++) {
        updateNode(*it, node);
        while ((*it)->childCount() > 0)
            delete (*it)->child(0);
        std::vector<plVaultNode> children = vault->fVault->getChildren(node.getNodeID());
        for (size_t i=0; i<children.size(); i++)
            loadNode(children[i], *it, vault);
    }
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
