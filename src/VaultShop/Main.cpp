/* This file is part of PlasmaShop.
 *
 * PlasmaShop is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PlasmaShop is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PlasmaShop.  If not, see <http://www.gnu.org/licenses/>.
 */

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
#include <QStandardPaths>
#include <Debug/plDebug.h>
#include <ResManager/plFactory.h>

#include "Main.h"
#include "QPlasma.h"

static QString HexToStr(const QString& hex)
{
    QString str;
    for (int i=0; i<hex.size(); i += 2) {
        int ch = hex.mid(i, 2).toInt(NULL, 16);
        str += QChar(ch);
    }
    return str;
}

/*
static QString StrToHex(const QString& str)
{
    QString hex;
    for (int i=0; i<str.size(); i++) {
        int ch = (int)str[i].toAscii();
        hex += QString("%1").arg(ch, 16);
    }
    return hex;
}
*/


/* VaultInfo */
VaultShopMain::VaultInfo::VaultInfo(QString filename)
    : fVaultFile(filename), fRootItem()
{
    fVault = new plVaultStore();
    fVault->ImportFile(filename.toUtf8().constData());
}

VaultShopMain::VaultInfo::~VaultInfo()
{
    delete fVault;
    delete fRootItem;
}

void VaultShopMain::VaultInfo::save()
{
    fVault->ExportFile(fVaultFile.toUtf8().constData());
}


/* SaveInfo */
VaultShopMain::SaveInfo::SaveInfo(QString filename)
    : fSaveFile(filename)
{
    fSave = new plNetGameServerState();
    hsFileStream FS(PlasmaVer::pvPots);
    FS.open(qstr2st(fSaveFile), fmRead);
    fSave->read(&FS);
}

VaultShopMain::SaveInfo::~SaveInfo()
{
    delete fSave;
}

void VaultShopMain::SaveInfo::save()
{
    hsFileStream FS(PlasmaVer::pvPots);
    FS.open(qstr2st(fSaveFile), fmCreate);
    fSave->write(&FS);
}


/* VaultShopMain */
VaultShopMain::VaultShopMain()
{
    // Basic Form Settings
    setWindowTitle("VaultShop " PLASMASHOP_VERSION);
    //setWindowIcon(QIcon(":/res/VaultShop.png"));

    // Set up actions
    fActions[kFileOpenVault] = new QAction(qStdIcon("document-open"), tr("&Load Vault..."), this);
    fActions[kFileSaveVault] = new QAction(qStdIcon("document-save"), tr("&Save Vault"), this);
    fActions[kFileExit] = new QAction(tr("E&xit"), this);
    fActions[kVaultOpenNode] = new QAction(tr("Subscribe to &Node..."), this);

    fActions[kNodeUnLink] = new QAction(tr("Remove"), this);
    fActions[kNodeLink] = new QAction(tr("Add Node..."), this);
    fActions[kNodeCreate] = new QAction(tr("Create Node"), this);
    fActions[kNodeUnsubscribe] = new QAction(tr("Un-subscribe"), this);
    //fActions[kNodeRenameVault] = new QAction(tr("Rename Vault..."), this);

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
    fSavEditor = NULL;
    fEditorTabPreference = 0;

    // Layout
    splitter->addWidget(fVaultTree);
    splitter->addWidget(fNodeTab);
    setCentralWidget(splitter);
    splitter->setSizes(QList<int>() << 160 << 320);

    // Global UI Signals
    connect(fActions[kFileExit], &QAction::triggered, this, &VaultShopMain::close);
    connect(fActions[kFileOpenVault], &QAction::triggered, this, &VaultShopMain::openGame);
    connect(fActions[kFileSaveVault], &QAction::triggered, this, &VaultShopMain::performSave);
    connect(fActions[kVaultOpenNode], &QAction::triggered, this, &VaultShopMain::openNode);
    connect(fActions[kNodeUnLink], &QAction::triggered, this, &VaultShopMain::unlinkNode);
    connect(fActions[kNodeLink], &QAction::triggered, this, &VaultShopMain::linkNode);
    connect(fActions[kNodeCreate], &QAction::triggered, this, &VaultShopMain::createNode);
    connect(fActions[kNodeUnsubscribe], &QAction::triggered, this, &VaultShopMain::closeNode);
    //connect(fActions[kNodeRenameVault], &QAction::triggered, this, &VaultShopMain::renameVault);

    connect(fVaultTree, &QTreeWidget::currentItemChanged,
            this, &VaultShopMain::treeItemChanged);
    connect(fVaultTree, &QTreeWidget::customContextMenuRequested,
            this, &VaultShopMain::treeContextMenu);
    connect(fNodeTab, &QTabWidget::currentChanged, this, &VaultShopMain::tabChanged);
    connect(fGenericEditor, &QVaultNode::typeModified, this, &VaultShopMain::typeModified);
    connect(this, &VaultShopMain::nodeChanged, this, &VaultShopMain::refreshNode);

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
    std::list<VaultInfo*>::iterator vi;
    for (vi = fLoadedVaults.begin(); vi != fLoadedVaults.end(); vi++)
        delete *vi;
    std::list<SaveInfo*>::iterator si;
    for (si = fLoadedSaves.begin(); si != fLoadedSaves.end(); si++)
        delete *si;
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
    while (item->parent() != NULL) {
        item = item->parent();
        std::list<VaultInfo*>::iterator it;
        for (it = fLoadedVaults.begin(); it != fLoadedVaults.end(); it++) {
            if ((*it)->fRootItem == item)
                return *it;
        }
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
            connect(fCustomEditor, &QVaultNodeEdit::subscribe,
                    this, &VaultShopMain::subscribe);
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
    } else if (fSavEditor != NULL) {
        fEditorTabPreference = fNodeTab->currentIndex();
        fNodeTab->removeTab(0);
        disconnect(fSavEditor, 0, 0, 0);
        delete fSavEditor;
        fSavEditor = NULL;
    }

    if (current != NULL && current->data(0, kRoleNodeID).toInt() > 0) {
        VaultInfo* vault = findCurrentVault(current);
        if (vault == NULL) {
            fGenericEditor->setNode(plVaultNode());
            fGenericEditor->hide();
        } else {
            plVaultNode node = vault->fVault->getNode(current->data(0, kRoleNodeID).toInt());
            fGenericEditor->setNode(node);
            fGenericEditor->show();
            fCustomEditor = QVaultNodeEdit::MakeEditor(fNodeTab, node, &fResMgr, &fSDLMgr);
            if (fCustomEditor != NULL) {
                fNodeTab->insertTab(0, fCustomEditor, fCustomEditor->getEditorTitle());
                fNodeTab->setCurrentIndex(fEditorTabPreference);
                connect(fCustomEditor, &QVaultNodeEdit::subscribe,
                        this, &VaultShopMain::subscribe);
            }
        }
    } else {
        fGenericEditor->setNode(plVaultNode());
        fGenericEditor->hide();
        if (current != NULL && !current->data(0, kRoleSavFile).toString().isEmpty()) {
            // Find the sav data
            std::list<SaveInfo*>::iterator it;
            for (it = fLoadedSaves.begin(); it != fLoadedSaves.end(); it++) {
                if ((*it)->fSaveFile == current->data(0, kRoleSavFile).toString())
                    break;
            }
            if (it != fLoadedSaves.end()) {
                fSavEditor = new QGameServerState(fNodeTab);
                fSavEditor->loadSav((*it)->fSave);
                fNodeTab->insertTab(0, fSavEditor, tr("Save Data"));
                fNodeTab->setCurrentIndex(fEditorTabPreference);
            }
        }
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
    std::list<SaveInfo*>::iterator si;
    for (si = fLoadedSaves.begin(); si != fLoadedSaves.end(); si++)
        delete *si;
    fLoadedSaves.clear();
    fVaultTree->clear();

    if (QFile::exists(path + "/sav/vault.dat"))
        loadVault(path + "/sav/vault.dat", "Vault Root", fVaultTree->invisibleRootItem());
    QDir vdir(path + "/sav/");
    if (vdir.exists()) {
        QStringList vaultList = vdir.entryList(
            QDir::Dirs | QDir::NoDotAndDotDot | QDir::Readable | QDir::Hidden);
        QStringList::const_iterator it;
        for (it = vaultList.constBegin(); it != vaultList.constEnd(); it++) {
            QString vaultPath = path + "/sav/" + *it + "/current";
            try {
                if (QFile::exists(vaultPath + "/vault.dat")) {
                    QTreeWidgetItem* dataRoot = new QTreeWidgetItem(fVaultTree, QStringList(HexToStr(*it)));
                    dataRoot->setIcon(0, QIcon(":/img/rfldr.png"));
                    dataRoot->setData(0, kRoleNodeID, -1);
                    dataRoot->setStatusTip(0, *it);

                    loadSaves(vaultPath, dataRoot);
                    loadVault(vaultPath + "/vault.dat", QString("Vault"), dataRoot);
                }
            } catch (const hsException& ex) {
                QMessageBox msgBox(QMessageBox::Critical, tr("Error"),
                                    tr("Error loading vault %1: %2")
                                    .arg(*it).arg(ex.what()),
                                    QMessageBox::Ok, this);
                plDebug::Error("{}:{}: {}", ex.File(), ex.Line(), ex.what());
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
                    fSDLMgr.ReadDescriptors(qstr2st(sdlPath));
            } catch (const hsException& ex) {
                QMessageBox msgBox(QMessageBox::Critical, tr("Error"),
                                    tr("Error parsing %1: %2")
                                    .arg(*it).arg(ex.what()),
                                    QMessageBox::Ok, this);
                plDebug::Error("{}:{}: {}", ex.File(), ex.Line(), ex.what());
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
                                   tr("Select the root game folder for your Uru installation"), fLastDir);
    if (!dirname.isEmpty())
        loadGame(dirname);
}

void VaultShopMain::loadVault(QString filename, QString vaultName, QTreeWidgetItem* parent)
{
    // Fix filename to contain the absolute path >.<
    QDir dir(filename);
    filename = dir.absolutePath();

    VaultInfo* vault = new VaultInfo(filename);
    vault->fRootItem = new QTreeWidgetItem(parent, QStringList() << vaultName);
    vault->fRootItem->setIcon(0, QIcon(":/img/rfldr.png"));
    vault->fRootItem->setData(0, kRoleNodeID, -1);
    vault->fRootItem->setStatusTip(0, filename);
    fLoadedVaults.push_back(vault);

    plVaultNode node = vault->fVault->getNode(vault->fVault->getFirstNodeID());
    if (node.isValid())
        loadNode(node, vault->fRootItem, vault);
}

QTreeWidgetItem* VaultShopMain::loadNode(const plVaultNode& node, QTreeWidgetItem* parent,
                                         VaultInfo* vault)
{
    for (int i=0; i<parent->childCount(); i++) {
        if (parent->child(i)->data(0, kRoleNodeID).toInt() == (int)node.getNodeID())
            return parent->child(i);
    }

    QTreeWidgetItem* item = new QTreeWidgetItem(parent);
    updateNode(item, node);

    std::vector<plVaultNode> children = vault->fVault->getChildren(node.getNodeID());
    for (size_t i=0; i<children.size(); i++)
        loadNode(children[i], item, vault);
    return item;
}

void VaultShopMain::loadSaves(QString path, QTreeWidgetItem* parent)
{
    QDir dir(path);
    QStringList savs = dir.entryList(QStringList("*.sav"), QDir::Files, QDir::Name);
    if (savs.length() == 0)
        return;

    QTreeWidgetItem* savRoot = new QTreeWidgetItem(parent, QStringList("Saves"));
    savRoot->setIcon(0, QIcon(":/img/rfldr.png"));
    savRoot->setData(0, kRoleNodeID, -1);
    savRoot->setStatusTip(0, path);

    foreach (QString sav, savs) {
        sav = dir.absoluteFilePath(sav);

        // Create or find the age folder
        QTreeWidgetItem* ageRoot = NULL;
        QString fn = sav.mid(sav.lastIndexOf(QChar('/')) + 1);
        fn = fn.left(fn.lastIndexOf(QChar('.')));
        QString ageName = fn.left(fn.lastIndexOf(QChar('_')));
        QString ageUuid = fn.mid(fn.lastIndexOf(QChar('_')) + 1);

        for (int i=0; i<parent->childCount(); i++) {
            if (parent->child(i)->text(0) == ageName) {
                ageRoot = parent->child(i);
                break;
            }
        }
        if (ageRoot == NULL) {
            ageRoot = new QTreeWidgetItem(savRoot, QStringList(ageName));
            ageRoot->setIcon(0, QIcon(":/img/age.png"));
            ageRoot->setData(0, kRoleNodeID, -1);
            ageRoot->setStatusTip(0, ageName);
        }

        QTreeWidgetItem* sdlNode = new QTreeWidgetItem(ageRoot, QStringList(ageUuid));
        sdlNode->setIcon(0, QIcon(":/img/data.png"));
        sdlNode->setData(0, kRoleNodeID, -1);
        sdlNode->setData(0, kRoleSavFile, sav);
        sdlNode->setStatusTip(0, QString("%1 %2").arg(ageName).arg(ageUuid));

        SaveInfo* saveInfo = new SaveInfo(sav);
        fLoadedSaves.push_back(saveInfo);
    }
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
    int nodeId = QInputDialog::getInt(this, tr("Node ID"),
                                      tr("Enter the ID of the Node to subscribe to"),
                                      0, 0, 0x7FFFFFFF, 1, &ok);
    if (ok)
        subscribe(nodeId);
}

void VaultShopMain::subscribe(unsigned int nodeId)
{
    if (nodeId == 0)
        return;

    VaultInfo* vault = findCurrentVault();
    if (vault == NULL) {
        QMessageBox msgBox(QMessageBox::Critical, tr("Error"),
                           tr("No vault is active"),
                           QMessageBox::Ok, this);
        msgBox.exec();
        return;
    }

    plVaultNode node = vault->fVault->getNode(nodeId);
    if (node.isValid()) {
        QTreeWidgetItem* item = loadNode(node, vault->fRootItem, vault);
        if (item != NULL)
            fVaultTree->setCurrentItem(item);
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
    std::list<VaultInfo*>::iterator vi;
    for (vi = fLoadedVaults.begin(); vi != fLoadedVaults.end(); vi++)
        (*vi)->save();

    // Save all saves to their files
    std::list<SaveInfo*>::iterator si;
    for (si = fLoadedSaves.begin(); si != fLoadedSaves.end(); si++)
        (*si)->save();
}

void VaultShopMain::treeContextMenu(const QPoint& pos)
{
    fVaultTree->setCurrentItem(fVaultTree->itemAt(pos));
    if (fVaultTree->currentItem() == NULL)
        return;

    QMenu menu(this);
    //menu.addAction(fActions[kNodeRenameVault]);
    menu.addAction(fActions[kNodeUnsubscribe]);
    menu.addAction(fActions[kNodeUnLink]);
    menu.addAction(fActions[kNodeLink]);
    menu.addAction(fActions[kNodeCreate]);
    if (fVaultTree->currentItem()->data(0, kRoleNodeID).toInt() < 0) {
        //fActions[kNodeRenameVault]->setEnabled(true);
        //fActions[kNodeRenameVault]->setVisible(true);
        fActions[kNodeUnsubscribe]->setEnabled(false);
        fActions[kNodeUnsubscribe]->setVisible(false);
        fActions[kNodeUnLink]->setEnabled(false);
        fActions[kNodeLink]->setEnabled(false);
    } else if (fVaultTree->currentItem()->parent() != NULL &&
               fVaultTree->currentItem()->parent()->data(0, kRoleNodeID).toInt() < 0) {
        //fActions[kNodeRenameVault]->setEnabled(false);
        //fActions[kNodeRenameVault]->setVisible(false);
        fActions[kNodeUnsubscribe]->setEnabled(true);
        fActions[kNodeUnsubscribe]->setVisible(true);
        fActions[kNodeUnLink]->setEnabled(false);
        fActions[kNodeLink]->setEnabled(true);
    } else {
        //fActions[kNodeRenameVault]->setEnabled(false);
        //fActions[kNodeRenameVault]->setVisible(false);
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
    int nodeId = QInputDialog::getInt(this, tr("Node ID"),
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

/*
void VaultShopMain::renameVault()
{
    VaultInfo* vault = findCurrentVault();
    if (vault == NULL) {
        QMessageBox msgBox(QMessageBox::Critical, tr("Error"),
                           tr("No vault is active"),
                           QMessageBox::Ok, this);
        msgBox.exec();
        return;
    }

    QString name =
    name = QInputDialog::getText(this, tr("Rename Vault"),
                                 tr("New Vault Name (must match the primary Player Name)"),
                                 QLineEdit::Normal, name);
}*/

int main(int argc, char* argv[])
{
    // Redirect libPlasma's debug stuff to VaultShop.log
    QString logpath = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
    QDir dir;
    dir.mkpath(logpath);
    logpath += "/VaultShop.log";
    plDebug::InitFile(plDebug::kDLAll, logpath.toUtf8().constData());

    QApplication app(argc, argv);
    VaultShopMain mainWnd;
    if (argc > 1 && QFile::exists(argv[1]))
        mainWnd.loadGame(argv[1]);
    mainWnd.show();
    return app.exec();
}
