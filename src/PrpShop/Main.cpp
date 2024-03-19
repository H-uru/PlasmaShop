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
#include <QVBoxLayout>
#include <QGridLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QProgressDialog>
#include <QDialogButtonBox>
#include <QMdiSubWindow>
#include <QTextEdit>
#include <QDropEvent>
#include <QUrl>
#include <QMimeData>
#include <QStandardPaths>
#include <QRegularExpression>
#include <Debug/plDebug.h>
#include <ResManager/plFactory.h>
#include <PRP/Surface/plMipmap.h>

#include "Main.h"
#include "QPlasma.h"
#include "QPlasmaUtils.h"
#include "QKeyDialog.h"
#include "PRP/QCreatable.h"
#include "QPrcEditor.h"
#include "QHexViewer.h"

PrpShopMain* PrpShopMain::sInstance = NULL;
PrpShopMain* PrpShopMain::Instance() { return sInstance; }
plResManager* PrpShopMain::ResManager() { return &sInstance->fResMgr; }

PrpShopMain::PrpShopMain()
{
    // Set up the "Magic" instance
    if (sInstance != NULL)
        throw hsBadParamException(__FILE__, __LINE__, "PrpShop broke...");
    sInstance = this;

    // Basic Form Settings
    setWindowTitle("PrpShop " PLASMASHOP_VERSION);
    setWindowIcon(QIcon(":/res/PrpShop.svg"));
    setDockOptions(QMainWindow::AnimatedDocks);

    // Set up actions
    fActions[kFileNewPage] = new QAction(qStdIcon("document-new"), tr("New &Page"), this);
    fActions[kFileOpen] = new QAction(qStdIcon("document-open"), tr("&Open..."), this);
    fActions[kFileSave] = new QAction(qStdIcon("document-save"), tr("&Save"), this);
    fActions[kFileSaveAs] = new QAction(tr("Sa&ve As..."), this);
    fActions[kFileExit] = new QAction(tr("E&xit"), this);
    fActions[kToolsProperties] = new QAction(tr("Show &Properties Pane"), this);
    fActions[kToolsShowAgePageIDs] = new QAction(tr("Show &Age/Page IDs"), this);
    fActions[kToolsShowTypeIDs] = new QAction(tr("Show Type &IDs"), this);
    fActions[kToolsNewObject] = new QAction(tr("&New Object..."), this);
    fActions[kWindowPrev] = new QAction(tr("&Previous"), this);
    fActions[kWindowNext] = new QAction(tr("&Next"), this);
    fActions[kWindowTile] = new QAction(tr("&Tile"), this);
    fActions[kWindowCascade] = new QAction(tr("&Cascade"), this);
    fActions[kWindowClose] = new QAction(tr("C&lose"), this);
    fActions[kWindowCloseAll] = new QAction(tr("Cl&ose All"), this);

    fActions[kTreeClose] = new QAction(tr("&Close"), this);
    fActions[kTreeEdit] = new QAction(tr("&Edit"), this);
    fActions[kTreeEditPRC] = new QAction(tr("Edit P&RC"), this);
    fActions[kTreeEditHex] = new QAction(tr("View He&x"), this);
    fActions[kTreeViewTargets] = new QAction(tr("Show &Targets"), this);
    fActions[kTreePreview] = new QAction(tr("&Preview"), this);
    fActions[kTreeDelete] = new QAction(tr("&Delete"), this);
    fActions[kTreeImport] = new QAction(tr("&Import..."), this);
    fActions[kTreeExport] = new QAction(tr("E&xport..."), this);
    fActions[kTreeNewObject] = new QAction(tr("&New Object..."), this);

    fActions[kFileOpen]->setShortcut(Qt::CTRL | Qt::Key_O);
    fActions[kFileSave]->setShortcut(Qt::CTRL | Qt::Key_S);
    fActions[kFileSaveAs]->setEnabled(false); //Enable if a page is selected
    fActions[kFileSaveAs]->setShortcut(Qt::CTRL | Qt::SHIFT | Qt::Key_S);
    fActions[kFileExit]->setShortcut(Qt::ALT | Qt::Key_F4);
    fActions[kWindowClose]->setShortcut(Qt::CTRL | Qt::Key_W);
    fActions[kWindowClose]->setShortcutContext(Qt::WidgetWithChildrenShortcut);
    fActions[kToolsProperties]->setCheckable(true);
    fActions[kToolsProperties]->setChecked(true);
    fActions[kToolsShowAgePageIDs]->setCheckable(true);
    fActions[kToolsShowAgePageIDs]->setChecked(false);
    fActions[kToolsShowTypeIDs]->setCheckable(true);
    fActions[kToolsShowTypeIDs]->setChecked(false);

    // Main Menus
    QMenu* fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(fActions[kFileNewPage]);
    fileMenu->addSeparator();
    fileMenu->addAction(fActions[kFileOpen]);
    fileMenu->addAction(fActions[kFileSave]);
    fileMenu->addAction(fActions[kFileSaveAs]);
    fileMenu->addSeparator();
    fileMenu->addAction(fActions[kFileExit]);

    QMenu* viewMenu = menuBar()->addMenu(tr("&Tools"));
    viewMenu->addAction(fActions[kToolsProperties]);
    viewMenu->addAction(fActions[kToolsShowAgePageIDs]);
    viewMenu->addAction(fActions[kToolsShowTypeIDs]);
    viewMenu->addSeparator();
    viewMenu->addAction(fActions[kToolsNewObject]);

    QMenu* wndMenu = menuBar()->addMenu(tr("&Window"));
    wndMenu->addAction(fActions[kWindowPrev]);
    wndMenu->addAction(fActions[kWindowNext]);
    wndMenu->addSeparator();
    wndMenu->addAction(fActions[kWindowTile]);
    wndMenu->addAction(fActions[kWindowCascade]);
    wndMenu->addSeparator();
    wndMenu->addAction(fActions[kWindowClose]);
    wndMenu->addAction(fActions[kWindowCloseAll]);

    // Toolbars
    QToolBar* fileTbar = addToolBar(tr("File Toolbar"));
    fileTbar->setObjectName("FileToolBar");
    fileTbar->addAction(fActions[kFileNewPage]);
    fileTbar->addAction(fActions[kFileOpen]);
    fileTbar->addAction(fActions[kFileSave]);
    //statusBar();

    // MDI Area for child editors
    fMdiArea = new QMdiArea(this);
    fMdiArea->setFrameShadow(QFrame::Sunken);
    fMdiArea->setFrameShape(QFrame::StyledPanel);
    fMdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    fMdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    fMdiArea->setActivationOrder(QMdiArea::ActivationHistoryOrder);
    setCentralWidget(fMdiArea);

    // Object Browser
    fBrowserDock = new QDockWidget(tr("Object Browser"), this);
    fBrowserDock->setObjectName("BrowserDock");
    fBrowserTabs = new QTabWidget(fBrowserDock);
    fBrowserTabs->addAction(fActions[kWindowClose]);
    fBrowserDock->setWidget(fBrowserTabs);
    fBrowserDock->setAllowedAreas(Qt::LeftDockWidgetArea |
                                  Qt::RightDockWidgetArea);
    fBrowserDock->setFeatures(QDockWidget::DockWidgetMovable |
                              QDockWidget::DockWidgetFloatable);
    fLastBrowserTabIndex = -1;

    fTypesTree = new QTreeWidget(fBrowserTabs);
    fBrowserTabs->addTab(fTypesTree, "Types");
    fTypesTree->setUniformRowHeights(true);
    fTypesTree->setHeaderHidden(true);
    fTypesTree->setContextMenuPolicy(Qt::CustomContextMenu);

    fStructureTree = new QTreeWidget(fBrowserTabs);
    fBrowserTabs->addTab(fStructureTree, "Structure");
    fStructureTree->setUniformRowHeights(true);
    fStructureTree->setHeaderHidden(true);
    fStructureTree->setContextMenuPolicy(Qt::CustomContextMenu);

    addDockWidget(Qt::LeftDockWidgetArea, fBrowserDock);

    // Property Pane
    fPropertyDock = new QDockWidget(tr("Properties"), this);
    fPropertyDock->setObjectName("PropertyDock");
    fPropertyDock->setAllowedAreas(Qt::LeftDockWidgetArea |
                                   Qt::RightDockWidgetArea);
    fPropertyContainer = new QWidget(fPropertyDock);
    QVBoxLayout* propLayout = new QVBoxLayout(fPropertyContainer);
    propLayout->setContentsMargins(4, 4, 4, 4);
    fPropertyDock->setWidget(fPropertyContainer);
    setPropertyPage(kPropsNone);
    addDockWidget(Qt::LeftDockWidgetArea, fPropertyDock);

    // Global UI Signals
    connect(fActions[kFileNewPage], &QAction::triggered, this, &PrpShopMain::newPage);
    connect(fActions[kFileExit], &QAction::triggered, this, &PrpShopMain::close);
    connect(fActions[kFileOpen], &QAction::triggered, this, &PrpShopMain::openFiles);
    connect(fActions[kFileSave], &QAction::triggered, this, &PrpShopMain::performSave);
    connect(fActions[kFileSaveAs], &QAction::triggered, this, &PrpShopMain::performSaveAs);

    connect(fActions[kToolsProperties], &QAction::toggled,
            fPropertyDock, &QWidget::setVisible);
    connect(fPropertyDock, &QDockWidget::visibilityChanged,
            fActions[kToolsProperties], &QAction::setChecked);
    connect(fActions[kToolsShowAgePageIDs], &QAction::toggled,
            this, &PrpShopMain::showAgePageIDs);
    connect(fActions[kToolsShowTypeIDs], &QAction::toggled,
            this, &PrpShopMain::showTypeIDs);
    connect(fActions[kToolsNewObject], &QAction::triggered,
            this, &PrpShopMain::createNewObject);

    connect(fActions[kWindowPrev], &QAction::triggered,
            fMdiArea, &QMdiArea::activatePreviousSubWindow);
    connect(fActions[kWindowNext], &QAction::triggered,
            fMdiArea, &QMdiArea::activateNextSubWindow);
    connect(fActions[kWindowTile], &QAction::triggered,
            fMdiArea, &QMdiArea::tileSubWindows);
    connect(fActions[kWindowCascade], &QAction::triggered,
            fMdiArea, &QMdiArea::cascadeSubWindows);
    connect(fActions[kWindowClose], &QAction::triggered,
            fMdiArea, &QMdiArea::closeActiveSubWindow);
    connect(fActions[kWindowCloseAll], &QAction::triggered,
            fMdiArea, &QMdiArea::closeAllSubWindows);

    connect(fActions[kTreeClose], &QAction::triggered, this, &PrpShopMain::treeClose);
    connect(fActions[kTreeEdit], &QAction::triggered, this, &PrpShopMain::treeEdit);
    connect(fActions[kTreeEditPRC], &QAction::triggered, this, &PrpShopMain::treeEditPRC);
    connect(fActions[kTreeEditHex], &QAction::triggered, this, &PrpShopMain::treeEditHex);
    connect(fActions[kTreeViewTargets], &QAction::triggered, this, &PrpShopMain::treeShowTargets);
    connect(fActions[kTreePreview], &QAction::triggered, this, &PrpShopMain::treePreview);
    connect(fActions[kTreeDelete], &QAction::triggered, this, &PrpShopMain::treeDelete);
    connect(fActions[kTreeImport], &QAction::triggered, this, &PrpShopMain::treeImport);
    connect(fActions[kTreeExport], &QAction::triggered, this, &PrpShopMain::treeExport);
    connect(fActions[kTreeNewObject], &QAction::triggered, this, &PrpShopMain::createNewObject);

    connect(fBrowserTabs, &QTabWidget::currentChanged, this, [this](auto index) {
        auto previousTree = qobject_cast<QTreeWidget*>(fBrowserTabs->widget(fLastBrowserTabIndex));
        QTreeWidgetItem* previous = previousTree == nullptr ? nullptr : previousTree->currentItem();
        treeItemChanged(currentItem(), previous);
        fLastBrowserTabIndex = index;
    });

    connect(fTypesTree, &QTreeWidget::currentItemChanged,
            this, &PrpShopMain::treeItemChanged);
    connect(fTypesTree, &QTreeWidget::itemActivated,
            this, &PrpShopMain::treeItemActivated);
    connect(fTypesTree, &QTreeWidget::customContextMenuRequested,
            this, &PrpShopMain::treeContextMenu);

    connect(fStructureTree, &QTreeWidget::currentItemChanged,
            this, &PrpShopMain::treeItemChanged);
    connect(fStructureTree, &QTreeWidget::itemActivated,
            this, &PrpShopMain::treeItemActivated);
    connect(fStructureTree, &QTreeWidget::customContextMenuRequested,
            this, &PrpShopMain::treeContextMenu);

    // Load UI Settings
    QSettings settings("PlasmaShop", "PrpShop");
    resize(settings.value("WinSize", QSize(800, 600)).toSize());
    if (settings.contains("WinPos"))
        move(settings.value("WinPos").toPoint());
    if (settings.value("WinMaximized", false).toBool())
        showMaximized();

    if (settings.contains("WinState"))
        restoreState(settings.value("WinState").toByteArray());
    fPropertyDock->setVisible(settings.value("PropertyShow", true).toBool());

    if (settings.contains("DialogDir"))
        fDialogDir = settings.value("DialogDir").toString();

    fActions[kToolsShowAgePageIDs]->setChecked(
            settings.value("ShowAgePageIDs", false).toBool());
    fActions[kToolsShowTypeIDs]->setChecked(
            settings.value("ShowTypeIDs", false).toBool());
}

void PrpShopMain::closeEvent(QCloseEvent*)
{
    // Save UI Settings
    QSettings settings("PlasmaShop", "PrpShop");
    settings.setValue("WinMaximized", (windowState() & Qt::WindowMaximized) != 0);
    showNormal();
    settings.setValue("WinSize", size());
    settings.setValue("WinPos", pos());
    settings.setValue("PropertyShow", fPropertyDock->isVisible());
    fPropertyDock->show();
    settings.setValue("WinState", saveState());

    settings.setValue("DialogDir", fDialogDir);
    settings.setValue("ShowAgePageIDs", s_showAgePageIDs);
    settings.setValue("ShowTypeIDs", s_showTypeIDs);
}

void PrpShopMain::dragEnterEvent(QDragEnterEvent* evt)
{
    if (evt->mimeData()->hasUrls())
        evt->acceptProposedAction();
}

void PrpShopMain::dropEvent(QDropEvent* evt)
{
    if (evt->mimeData()->hasUrls()) {
        foreach (QUrl url, evt->mimeData()->urls()) {
            QString filename = url.toLocalFile();
            if (!filename.isEmpty())
                loadFile(filename);
        }
    }
}

QPlasmaTreeItem* PrpShopMain::currentItem() const
{
    auto tree = qobject_cast<QTreeWidget*>(fBrowserTabs->currentWidget());
    if (tree == nullptr) {
        return nullptr;
    }
    return static_cast<QPlasmaTreeItem*>(tree->currentItem());
}

void PrpShopMain::setPropertyPage(PropWhich which)
{
    QWidget* group = NULL;
    QLayoutItem* item = fPropertyContainer->layout()->itemAt(0);
    if (item != NULL) {
        // Clear the old property page
        fPropertyContainer->layout()->removeItem(item);
        delete item->widget();
        delete item;
    }

    fAgeName = nullptr;
    fPageName = nullptr;
    fReleaseVersion = nullptr;
    fSeqPrefix = nullptr;
    fSeqSuffix = nullptr;
    fObjName = nullptr;
    fObjType = nullptr;
    fLoadMaskQ[0] = nullptr;
    fLoadMaskQ[1] = nullptr;
    fCloneIdBox = nullptr;
    fCloneId = nullptr;
    fClonePlayerId = nullptr;

    switch (which) {
    case kPropsAge:
        {
            // An age is selected -- not much to do here for now
            group = new QGroupBox(tr("Age Properties"), fPropertyContainer);
            QGridLayout* layout = new QGridLayout(group);
            fAgeName = new QLineEdit(group);
            fAgeName->setReadOnly(true);
            layout->addWidget(new QLabel(tr("Name:"), group), 0, 0);
            layout->addWidget(fAgeName, 0, 1);
        }
        break;
    case kPropsPage:
        {
            // PRP file's properties.  Changes to the Location should propagate
            // to all other keys in this PRP.
            group = new QGroupBox(tr("Page Properties"), fPropertyContainer);
            QGridLayout* layout = new QGridLayout(group);
            fAgeName = new QLineEdit(group);
            fPageName = new QLineEdit(group);
            fReleaseVersion = new QSpinBox(group);
            fReleaseVersion->setRange(0, 0x7FFFFFFF);

            QGroupBox* locationGrp = new QGroupBox(tr("Location"), group);
            QGridLayout* locationLayout = new QGridLayout(locationGrp);
            fSeqPrefix = new QSpinBox(locationGrp);
            fSeqPrefix->setRange(-0x00800000, 0x007FFFFF);
            fSeqSuffix = new QSpinBox(locationGrp);
            fSeqSuffix->setRange(-0x00008000, 0x00007FFF);
            fLocationFlags[kLocLocalOnly] = new QCheckBox(tr("Local Only"), locationGrp);
            fLocationFlags[kLocVolatile] = new QCheckBox(tr("Volatile"), locationGrp);
            fLocationFlags[kLocItinerant] = new QCheckBox(tr("Itinerant"), locationGrp);
            fLocationFlags[kLocReserved] = new QCheckBox(tr("Reserved"), locationGrp);
            fLocationFlags[kLocBuiltIn] = new QCheckBox(tr("Built-In"), locationGrp);
            locationLayout->addWidget(new QLabel(tr("Page ID:"), locationGrp), 0, 0);
            locationLayout->addWidget(fSeqPrefix, 0, 1);
            locationLayout->addWidget(fSeqSuffix, 0, 2);
            locationLayout->addWidget(new QLabel(tr("Flags:"), locationGrp), 1, 0);
            locationLayout->addWidget(fLocationFlags[kLocLocalOnly], 1, 1);
            locationLayout->addWidget(fLocationFlags[kLocReserved], 1, 2);
            locationLayout->addWidget(fLocationFlags[kLocVolatile], 2, 1);
            locationLayout->addWidget(fLocationFlags[kLocBuiltIn], 2, 2);
            locationLayout->addWidget(fLocationFlags[kLocItinerant], 3, 1);

            layout->addWidget(new QLabel(tr("Age:"), group), 0, 0);
            layout->addWidget(fAgeName, 0, 1);
            layout->addWidget(new QLabel(tr("Page:"), group), 1, 0);
            layout->addWidget(fPageName, 1, 1);
            layout->addWidget(new QLabel(tr("Release Version:"), group), 2, 0);
            layout->addWidget(fReleaseVersion, 2, 1);
            layout->addWidget(locationGrp, 3, 0, 1, 2);
        }
        break;
    case kPropsKO:
        {
            // A KeyedObject -- just edit the object's key (much easier than
            // throwing Key editors everywhere else they appear)
            group = new QGroupBox(tr("Object Properties"), fPropertyContainer);
            QGridLayout* layout = new QGridLayout(group);
            fObjName = new QLineEdit(group);
            fObjType = new QLabel(group);
            fLoadMaskQ[0] = new QSpinBox(group);
            fLoadMaskQ[1] = new QSpinBox(group);
            fLoadMaskQ[0]->setRange(0, 255);
            fLoadMaskQ[1]->setRange(0, 255);

            fCloneIdBox = new QGroupBox(tr("Clone IDs"), group);
            fCloneIdBox->setCheckable(true);
            QGridLayout* cloneLayout = new QGridLayout(fCloneIdBox);
            fCloneId = new QSpinBox(fCloneIdBox);
            fCloneId->setRange(0, 0x7FFFFFFF);
            fClonePlayerId = new QSpinBox(fCloneIdBox);
            fClonePlayerId->setRange(0, 0x7FFFFFFF);
            cloneLayout->addWidget(new QLabel(tr("Clone ID:"), fCloneIdBox), 0, 0);
            cloneLayout->addWidget(fCloneId, 0, 1);
            cloneLayout->addWidget(new QLabel(tr("Clone Player ID:"), fCloneIdBox), 1, 0);
            cloneLayout->addWidget(fClonePlayerId, 1, 1);

            layout->addWidget(new QLabel(tr("Name:"), group), 0, 0);
            layout->addWidget(fObjName, 0, 1, 1, 2);
            layout->addWidget(new QLabel(tr("Type:"), group), 1, 0);
            layout->addWidget(fObjType, 1, 1, 1, 2);
            layout->addWidget(new QLabel(tr("Load Mask:"), group), 2, 0);
            layout->addWidget(fLoadMaskQ[0], 2, 1);
            layout->addWidget(fLoadMaskQ[1], 2, 2);
            layout->addWidget(fCloneIdBox, 3, 0, 1, 3);
        }
        break;
    default:
        {
            group = new QWidget(fPropertyContainer);
            QGridLayout* layout = new QGridLayout(group);
            layout->addWidget(new QLabel(tr("No Object Selected"), group), 0, 0);
        }
    }
    fPropertyContainer->layout()->addWidget(group);
    fPropertyContainer->setFixedHeight(group->sizeHint().height() + 8);
}

void PrpShopMain::treeItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous)
{
    saveProps((QPlasmaTreeItem*)previous);

    QPlasmaTreeItem* item = (QPlasmaTreeItem*)current;
    fActions[kFileSaveAs]->setEnabled(false);
    if (item == NULL) {
        setPropertyPage(kPropsNone);
    } else if (item->type() == QPlasmaTreeItem::kTypeAge) {
        setPropertyPage(kPropsAge);
        fAgeName->setText(item->age());
    } else if (item->type() == QPlasmaTreeItem::kTypePage) {
        setPropertyPage(kPropsPage);
        fAgeName->setText(st2qstr(item->page()->getAge()));
        fPageName->setText(st2qstr(item->page()->getPage()));
        fReleaseVersion->setValue(item->page()->getReleaseVersion());
        fSeqPrefix->setValue(item->page()->getLocation().getSeqPrefix());
        fSeqSuffix->setValue(item->page()->getLocation().getPageNum());
        fLocationFlags[kLocLocalOnly]->setChecked(item->page()->getLocation().getFlags() & plLocation::kLocalOnly);
        fLocationFlags[kLocVolatile]->setChecked(item->page()->getLocation().getFlags() & plLocation::kVolatile);
        fLocationFlags[kLocItinerant]->setChecked(item->page()->getLocation().getFlags() & plLocation::kItinerant);
        fLocationFlags[kLocReserved]->setChecked(item->page()->getLocation().getFlags() & plLocation::kReserved);
        fLocationFlags[kLocBuiltIn]->setChecked(item->page()->getLocation().getFlags() & plLocation::kBuiltIn);
        fActions[kFileSaveAs]->setEnabled(true);
    } else if (item->type() == QPlasmaTreeItem::kTypeKO) {
        setPropertyPage(kPropsKO);
        fObjName->setText(st2qstr(item->key()->getName()));
        fObjType->setText(item->obj()->ClassName());
        fLoadMaskQ[0]->setValue(item->key()->getLoadMask().getQuality(0));
        fLoadMaskQ[1]->setValue(item->key()->getLoadMask().getQuality(1));
        fCloneId->setValue(item->key()->getCloneID());
        fClonePlayerId->setValue(item->key()->getClonePlayerID());
        fCloneIdBox->setChecked(fCloneId->value() != 0 || fClonePlayerId->value() != 0);
    } else {
        setPropertyPage(kPropsNone);
    }
}

void PrpShopMain::treeItemActivated(QTreeWidgetItem* item, int)
{
    if (item == NULL || ((QPlasmaTreeItem*)item)->obj() == NULL)
        return;
    editCreatable(((QPlasmaTreeItem*)item)->obj());
}

void PrpShopMain::treeContextMenu(const QPoint& pos)
{
    auto tree = qobject_cast<QTreeWidget*>(fBrowserTabs->currentWidget());
    if (tree == nullptr) {
        return;
    }
    tree->setCurrentItem(tree->itemAt(pos));
    QPlasmaTreeItem* item = currentItem();
    if (item == NULL)
        return;

    QMenu menu(this);
    if (item->type() == QPlasmaTreeItem::kTypeAge) {
        menu.addAction(fActions[kTreeClose]);
    } else if (item->type() == QPlasmaTreeItem::kTypePage) {
        menu.addAction(fActions[kTreeClose]);
        menu.addAction(fActions[kTreeImport]);
    } else if (item->type() == QPlasmaTreeItem::kTypeKO) {
        menu.addAction(fActions[kTreeEdit]);
        menu.addAction(fActions[kTreeEditPRC]);
        menu.addAction(fActions[kTreeEditHex]);
        menu.addAction(fActions[kTreePreview]);
        menu.addAction(fActions[kTreeViewTargets]);
        menu.addSeparator();
        menu.addAction(fActions[kTreeDelete]);
        menu.addAction(fActions[kTreeImport]);
        menu.addAction(fActions[kTreeExport]);
        menu.setDefaultAction(fActions[kTreeEdit]);
        fActions[kTreePreview]->setEnabled(pqCanPreviewType(item->obj()));
        fActions[kTreeViewTargets]->setEnabled(pqHasTargets(item->obj()));
    } else if (item->type() == QPlasmaTreeItem::kTypeClassType) {
        menu.addAction(fActions[kTreeImport]);
        menu.addAction(fActions[kTreeNewObject]);
        fActions[kTreeNewObject]->setText(tr("New %1...").arg(
            pqGetFriendlyClassName(item->classType()))
        );
        fActions[kTreeNewObject]->setEnabled(pqIsValidKOType(item->classType()));
    }
    menu.exec(tree->viewport()->mapToGlobal(pos));
}

void PrpShopMain::treeClose()
{
    QPlasmaTreeItem* item = currentItem();
    if (item == NULL)
        return;

    // Make sure no windows are open with objects we want to unload...

    if (item->type() == QPlasmaTreeItem::kTypeAge) {
        for (auto it = fLoadedLocations.begin(); it != fLoadedLocations.end();) {
            PrpShopLoadedPage* loadedPage = *it;
            if (st2qstr(loadedPage->fPage->getAge()) == item->age()) {
                const plLocation& loc = loadedPage->fPage->getLocation();
                closeWindows(loc);
                it = fLoadedLocations.erase(it);
                delete loadedPage;
                fResMgr.UnloadPage(loc);
            } else {
                it++;
            }
        }
        PrpShopLoadedAge* loadedAge = fLoadedAges.take(item->age());
        delete loadedAge->fTypesItem;
        delete loadedAge->fStructureItem;
        delete loadedAge;
    } else if (item->type() == QPlasmaTreeItem::kTypePage) {
        plLocation loc = item->page()->getLocation();
        closeWindows(loc);
        PrpShopLoadedPage* loadedPage = fLoadedLocations.take(loc);
        PrpShopLoadedAge* loadedAge = loadedPage->fAge;
        delete loadedPage->fTypesItem;
        delete loadedPage->fStructureItem;
        delete loadedPage;
        fResMgr.UnloadPage(loc);
        if (loadedAge->fTypesItem->childCount() == 0) {
            delete loadedAge->fTypesItem;
            delete loadedAge->fStructureItem;
            delete loadedAge;
        }
    }
}

void PrpShopMain::treeEdit()
{
    QPlasmaTreeItem* item = currentItem();
    if (item == NULL || item->obj() == NULL)
        return;
    editCreatable(item->obj());
}

void PrpShopMain::treeEditPRC()
{
    QPlasmaTreeItem* item = currentItem();
    if (item == NULL || item->obj() == NULL)
        return;
    editCreatable(item->obj(), kPRC_Type | item->obj()->ClassIndex());
}

void PrpShopMain::treeEditHex()
{
    QPlasmaTreeItem* item = currentItem();
    if (item == NULL || item->obj() == NULL)
        return;
    QCreatable *creWin = editCreatable(item->obj(), kHex_Type | item->obj()->ClassIndex());
    if (creWin) {
        QHexViewer *hexWin = qobject_cast<QHexViewer *>(creWin);
        Q_ASSERT(hexWin);

        uint32_t offset = item->key()->getFileOff();
        uint32_t size = item->key()->getObjSize();
        hexWin->loadObject(findPageForItem(item)->fFilename, offset, size);
    }
}

void PrpShopMain::treePreview()
{
    QPlasmaTreeItem* item = currentItem();
    if (item == NULL || item->obj() == NULL)
        return;
    editCreatable(item->obj(), kPreview_Type | item->obj()->ClassIndex());
}

void PrpShopMain::treeShowTargets() {
    QPlasmaTreeItem* item = currentItem();
    if (item == NULL || item->obj() == NULL)
        return;
    editCreatable(item->obj(), kTargets_Type | item->obj()->ClassIndex());
}

void PrpShopMain::treeDelete()
{
    QPlasmaTreeItem* item = currentItem();
    if (item == NULL || item->obj() == NULL)
        return;
    fResMgr.DelObject(item->key());
    PrpShopLoadedPage* loadedPage = findPageForItem(item);

    // Remove the deleted object from the types tree.
    QPlasmaTreeItem* folder = (QPlasmaTreeItem*)loadedPage->fTypesItem->parent();
    delete loadedPage->fTypesItem;

    // If this was the last object of this type in this page,
    // also remove the entire class type folder from the types tree.
    if (folder->childCount() == 0)
        delete folder;

    // Deleting an object might change the structure significantly,
    // so we have to fully rebuild the structure tree for this page.
    rebuildStructureTree(loadedPage);
}

QPlasmaTreeItem* PrpShopMain::findObjectInTypesTree(const plKey& key)
{
    PrpShopLoadedPage* loadedPage = fLoadedLocations.value(key->getLocation());
    QPlasmaTreeItem* pageItem = loadedPage->fTypesItem;

    // Under the found page item, find the class type item for the object's class.
    QPlasmaTreeItem* typeItem = nullptr;
    for (int i = 0; i < pageItem->childCount(); i++) {
        auto child = static_cast<QPlasmaTreeItem*>(pageItem->child(i));
        if (child->classType() == key->getType()) {
            typeItem = child;
            break;
        }
    }
    if (typeItem == nullptr) {
        return nullptr;
    }

    // Under the found class type item, find the object itself.
    for (int i = 0; i < typeItem->childCount(); i++) {
        auto child = static_cast<QPlasmaTreeItem*>(typeItem->child(i));
        if (child->key() == key) {
            return child;
        }
    }
    return nullptr;
}

void PrpShopMain::addNewObjectToTree(const hsKeyedObject* ko)
{
    // Look up the loaded page for the object's location
    PrpShopLoadedPage* loadedPage = fLoadedLocations.value(ko->getKey()->getLocation());
    if (loadedPage == nullptr)
        throw hsBadParamException(__FILE__, __LINE__, "Cannot create path to a location that isn't loaded");

    QPlasmaTreeItem* pageItem = loadedPage->fTypesItem;
    // Under the found page item, find or create class type item for the object's class
    QPlasmaTreeItem* typeItem = nullptr;
    for (int i=0; i<pageItem->childCount(); i++) {
        auto child = static_cast<QPlasmaTreeItem*>(pageItem->child(i));
        if (child->classType() == ko->ClassIndex()) {
            typeItem = child;
            break;
        }
    }
    if (typeItem == nullptr) {
        typeItem = new QPlasmaTreeItem(pageItem, ko->ClassIndex());
    }

    // Add the new object to the class type item
    new QPlasmaTreeItem(typeItem, ko->getKey());
    fTypesTree->sortItems(0, Qt::AscendingOrder);

    // If the object already has references to other objects (e. g. if it was imported),
    // then adding the object might change the structure significantly,
    // so we have to fully rebuild the structure tree for this page.
    rebuildStructureTree(loadedPage);
}

void PrpShopMain::closeWindows(const plLocation& loc)
{
    QList<QMdiSubWindow*> windows = fMdiArea->subWindowList(); 
    for (auto it = windows.begin(); it != windows.end(); it++) {
        QCreatable* creWin = qobject_cast<QCreatable*>((*it)->widget());
        if (creWin && creWin->compareLocation(loc))
            fMdiArea->removeSubWindow(*it);
    }
}

void PrpShopMain::treeImport()
{
    PrpShopLoadedPage* loadedPage = findPageForItem(currentItem());
    if (loadedPage == nullptr)
        return;

    QStringList files = QFileDialog::getOpenFileNames(this,
                            tr("Import Raw Object(s)"), fDialogDir,
                            "Plasma Objects (*.po *.mof *.uof)");
    QStringList filesIt = files;
    for (QStringList::Iterator it = filesIt.begin(); it != filesIt.end(); it++) {
        try {
            hsFileStream S((int)fResMgr.getVer());
            S.open(qstr2st(*it), fmRead);
            plCreatable* pCre = fResMgr.ReadCreatable(&S);
            hsKeyedObject* ko = hsKeyedObject::Convert(pCre);
            if (pCre != NULL && ko == NULL) {
                delete pCre;
                throw hsBadParamException(__FILE__, __LINE__, "Invalid object class");
            }
            // The key is already added to the ResMgr, but we need to ensure
            // its location is correctly updated
            plLocation loc = loadedPage->fPage->getLocation();
            fResMgr.MoveKey(ko->getKey(), loc);

            // Now add it to the tree
            addNewObjectToTree(ko);
        } catch (std::exception& ex) {
            QMessageBox msgBox(QMessageBox::Critical, tr("Error"),
                               tr("Error Importing File %1:\n%2").arg(*it).arg(ex.what()),
                               QMessageBox::Ok, this);
            msgBox.exec();
        }
        QDir dir = QDir(*it);
        dir.cdUp();
        fDialogDir = dir.absolutePath();
    }
}

void PrpShopMain::treeExport()
{
    QPlasmaTreeItem* item = currentItem();
    if (item == NULL || item->obj() == NULL)
        return;

    QString fnfix = st2qstr(item->key()->getName())
                    .replace(QRegularExpression("[?:/\\*\"<>|]"), "_");
    QString genPath = tr("%1/[%2]%3.po").arg(fDialogDir)
                                        .arg(item->obj()->ClassName())
                                        .arg(fnfix);
    QString filename = QFileDialog::getSaveFileName(this,
                            tr("Export Raw Object"), genPath,
                            "Plasma Objects (*.po *.mof *.uof)");
    if (!filename.isEmpty()) {
        try {
            hsFileStream S((int)fResMgr.getVer());
            S.open(qstr2st(filename), fmCreate);
            fResMgr.WriteCreatable(&S, item->obj());
        } catch (std::exception& ex) {
            QMessageBox msgBox(QMessageBox::Critical, tr("Error"),
                               tr("Error Exporting File %1:\n%2").arg(filename).arg(ex.what()),
                               QMessageBox::Ok, this);
            msgBox.exec();
        }
        QDir dir = QDir(filename);
        dir.cdUp();
        fDialogDir = dir.absolutePath();
    }
}

QCreatable* PrpShopMain::editCreatable(plCreatable* pCre, int forceType)
{
    if (pCre == Q_NULLPTR) {
        QMessageBox::critical(this, tr("NULL Object"),
                tr("The requested object is not currently loaded"));
        return Q_NULLPTR;
    }

    QList<QMdiSubWindow*> windows = fMdiArea->subWindowList();
    QList<QMdiSubWindow*>::Iterator it;
    for (it = windows.begin(); it != windows.end(); it++) {
        QCreatable* creWin = qobject_cast<QCreatable*>((*it)->widget());
        if (creWin && creWin->isMatch(pCre, forceType)) {
            fMdiArea->setActiveSubWindow(*it);
            return creWin;
        }
    }
    if (it == windows.end()) {
        QCreatable* win = pqMakeCreatableForm(pCre, this, forceType);
        if (win != Q_NULLPTR) {
            QMdiSubWindow* subWin = fMdiArea->addSubWindow(win);
            subWin->setWindowIcon(win->windowIcon());
            subWin->show();
            return win;
        }
    }
    return Q_NULLPTR;
}

void PrpShopMain::newPage()
{
    static PlasmaVer s_pvMap[] = {
        PlasmaVer::pvPrime, PlasmaVer::pvPots, PlasmaVer::pvMoul,
        PlasmaVer::pvEoa, PlasmaVer::pvHex, PlasmaVer::pvUniversal,
    };

    QDialog dlg;
    dlg.setWindowTitle(tr("New Page..."));
    dlg.setWindowIcon(qStdIcon("document-new"));

    QComboBox* dlg_gameVer = new QComboBox(&dlg);
    dlg_gameVer->addItems(QStringList() << "UU / Prime" << "Uru CC / PotS / Alcugs"
                                        << "Uru Live / MQO" << "Myst 5 / Crowthistle"
                                        << "Hex Isle" << "Universal (unsupported)");
    dlg_gameVer->setCurrentIndex(1);
    QLineEdit* dlg_ageName = new QLineEdit(&dlg);
    QLineEdit* dlg_pageName = new QLineEdit(&dlg);

    QGroupBox* locationGrp = new QGroupBox(tr("Location"), &dlg);
    QGridLayout* locationLayout = new QGridLayout(locationGrp);
    QSpinBox* dlg_seqPrefix = new QSpinBox(locationGrp);
    dlg_seqPrefix->setRange(-0x00800000, 0x007FFFFF);
    QSpinBox* dlg_seqSuffix = new QSpinBox(locationGrp);
    dlg_seqSuffix->setRange(-0x00008000, 0x00007FFF);
    locationLayout->addWidget(new QLabel(tr("Page ID:"), locationGrp), 0, 0);
    locationLayout->addWidget(dlg_seqPrefix, 0, 1);
    locationLayout->addWidget(dlg_seqSuffix, 0, 2);

    QDialogButtonBox* btnBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                                                    Qt::Horizontal, &dlg);
    dlg.connect(btnBox, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    dlg.connect(btnBox, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    QGridLayout* layout = new QGridLayout(&dlg);
    layout->addWidget(new QLabel(tr("Version:"), &dlg), 0, 0);
    layout->addWidget(dlg_gameVer, 0, 1);
    layout->addWidget(new QLabel(tr("Age:"), &dlg), 1, 0);
    layout->addWidget(dlg_ageName, 1, 1);
    layout->addWidget(new QLabel(tr("Page:"), &dlg), 2, 0);
    layout->addWidget(dlg_pageName, 2, 1);
    layout->addWidget(locationGrp, 3, 0, 1, 2);
    layout->addItem(new QSpacerItem(0, 16, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding), 4, 0, 1, 2);
    layout->addWidget(btnBox, 5, 0, 1, 2);

    if (dlg.exec() != QDialog::Accepted)
        return;

    PlasmaVer selPVer = s_pvMap[dlg_gameVer->currentIndex()];
    fResMgr.setVer(selPVer, true);

    plPageInfo* page = new plPageInfo(qstr2st(dlg_ageName->text()),
                                      qstr2st(dlg_pageName->text()));
    plLocation loc;
    loc.setSeqPrefix(dlg_seqPrefix->value());
    loc.setPageNum(dlg_seqSuffix->value());
    page->setLocation(loc);
    fResMgr.AddPage(page);

    QString filename = dlg_ageName->text();
    if (selPVer.isUru())
        filename += "_District";
    filename += "_" + dlg_pageName->text();
    loadPage(page, filename);
    fTypesTree->sortItems(0, Qt::AscendingOrder);
    fStructureTree->sortItems(0, Qt::AscendingOrder);
}

void PrpShopMain::openFiles()
{
    QStringList files = QFileDialog::getOpenFileNames(this,
                            tr("Open File(s)"), fDialogDir,
                            "All supported types (*.age *.prp *.prx);;"
                            "Age Files (*.age);;"
                            "Page Files (*.prp *.prx)");
    QStringList filesIt = files;
    for (QStringList::Iterator it = filesIt.begin(); it != filesIt.end(); it++) {
        loadFile(*it);
        QDir dir = QDir(*it);
        dir.cdUp();
        fDialogDir = dir.absolutePath();
    }
}

void PrpShopMain::loadFile(QString filename)
{
    // Fix filename to contain the absolute path >.<
    // Note: On Win32, Qt will use '/' as a path separator
    //       But libHSPlasma wants '\', so we must fix this!
    QDir dir(filename);
    filename = QDir::toNativeSeparators(dir.absolutePath());

    PageUnloadCallback prevCallback = fResMgr.SetPageUnloadFunc([this, &prevCallback](const plLocation& loc) {
        closeWindows(loc);
        if (prevCallback != NULL)
            prevCallback(loc);
    });

    QProgressDialog progress("Please Wait...", QString(), 0, 0);
    progress.setWindowModality(Qt::WindowModal);
    progress.setAutoClose(false);
    plPageInfo* lastPage = 0;
    fResMgr.SetProgressFunc([&progress, &lastPage](plPageInfo *page, size_t curObj, size_t maxObjs) {
        if (page != lastPage) {
            progress.setLabelText(QString("Loading %1...")
                    .arg(page ? st2qstr(page->getPage()) : "<Unknown Page>"));
            progress.setMaximum(maxObjs);
            lastPage = page;
        }
        progress.setValue(curObj);
    });
    progress.show();
    qApp->processEvents();

    if (filename.endsWith(".age", Qt::CaseInsensitive)) {
        try {
            plAgeInfo* age = fResMgr.ReadAge(qstr2st(filename), true);
            QDir path(filename);
            path.cdUp();
            for (size_t i=0; i<age->getNumPages(); i++) {
                QString prp = QDir::toNativeSeparators(path.absoluteFilePath(st2qstr(age->getPageFilename(i, fResMgr.getVer()))));
                if (QFile::exists(prp)) {
                    loadPage(fResMgr.FindPage(age->getPageLoc(i, fResMgr.getVer())), prp);
                } else {
                    plDebug::Warning("Could not find page {} referenced from {}",
                                     age->getPageFilename(i, fResMgr.getVer()),
                                     filename.toUtf8().data());
                }
            }
            for (size_t i=0; i<age->getNumCommonPages(fResMgr.getVer()); i++) {
                QString prp = QDir::toNativeSeparators(path.absoluteFilePath(st2qstr(age->getCommonPageFilename(i, fResMgr.getVer()))));
                if (QFile::exists(prp))
                    loadPage(fResMgr.FindPage(age->getCommonPageLoc(i, fResMgr.getVer())), prp);
            }
        } catch (std::exception& ex) {
            QMessageBox msgBox(QMessageBox::Warning, tr("Error"),
                               tr("Error Loading File %1:\n%2").arg(filename).arg(ex.what()),
                               QMessageBox::Ok, this);
            msgBox.exec();
        }
    } else if (filename.endsWith(".prp", Qt::CaseInsensitive) || filename.endsWith(".prx", Qt::CaseInsensitive)) {
        try {
            plPageInfo* page = fResMgr.ReadPage(qstr2st(filename));
            PrpShopLoadedPage* loadedPage = loadPage(page, filename);

            // Manually check for and load the BuiltIn and Textures PRPs
            QDir path(filename);
            path.cdUp(); // Get rid of the filename >.>
            if (!loadedPage->fAge->fHasTextures) {
                QString texPath = st2qstr(page->getAge());
                if (fResMgr.getVer().isUru())
                    texPath += "_District";
                texPath += "_Textures.prp";
                texPath = QDir::toNativeSeparators(path.absoluteFilePath(texPath));
                if (QFile::exists(texPath))
                    loadPage(fResMgr.ReadPage(qstr2st(texPath)), texPath);
            }
            if (!loadedPage->fAge->fHasBuiltIn) {
                QString biPath = st2qstr(page->getAge());
                if (fResMgr.getVer().isUru())
                    biPath += "_District";
                biPath += "_BuiltIn.prp";
                biPath = QDir::toNativeSeparators(path.absoluteFilePath(biPath));
                if (QFile::exists(biPath))
                    loadPage(fResMgr.ReadPage(qstr2st(biPath)), biPath);
            }
        } catch (std::exception& ex) {
            QMessageBox msgBox(QMessageBox::Critical, tr("Error"),
                               tr("Error Loading File %1:\n%2").arg(filename).arg(ex.what()),
                               QMessageBox::Ok, this);
            msgBox.exec();
        }
    } else {
        QMessageBox msgBox(QMessageBox::Warning, tr("Error"),
                           tr("%1: Unsupported File Type").arg(filename),
                           QMessageBox::Ok, this);
        msgBox.exec();
    }

    fResMgr.SetPageUnloadFunc(prevCallback);
    fTypesTree->sortItems(0, Qt::AscendingOrder);
    fStructureTree->sortItems(0, Qt::AscendingOrder);
}

PrpShopLoadedPage* PrpShopMain::findPageForItem(QPlasmaTreeItem* item)
{
    if (item == NULL)
        return NULL;

    if (item->type() == QPlasmaTreeItem::kTypePage) {
        return fLoadedLocations.value(item->page()->getLocation());
    } else if (item->type() == QPlasmaTreeItem::kTypeKO) {
        return fLoadedLocations.value(item->key()->getLocation(), NULL);
    } else if (item->type() == QPlasmaTreeItem::kTypeClassType) {
        QPlasmaTreeItem* pageItem = (QPlasmaTreeItem*)item->parent();
        if (pageItem->type() != QPlasmaTreeItem::kTypePage)
            throw hsBadParamException(__FILE__, __LINE__, "Got non-page parent");
        return fLoadedLocations.value(pageItem->page()->getLocation());
    } else {
        return nullptr;
    }
}

void PrpShopMain::performSave()
{
    QPlasmaTreeItem* item = currentItem();
    if (item == nullptr) {
        return;
    }

    if (item->type() == QPlasmaTreeItem::kTypeAge) {
        // Save all pages belonging to this age
        for (int i = 0; i < item->childCount(); i++) {
            auto pageItem = static_cast<QPlasmaTreeItem*>(item->child(i));
            if (pageItem->type() != QPlasmaTreeItem::kTypePage)
                throw hsBadParamException(__FILE__, __LINE__, "Got non-page child");
            PrpShopLoadedPage* loadedPage = findPageForItem(item);
            if (loadedPage != nullptr) {
                saveFile(loadedPage->fPage, loadedPage->fFilename);
            }
        }
    } else {
        // Find the page corresponding to this item and save it
        PrpShopLoadedPage* loadedPage = findPageForItem(item);
        if (loadedPage != nullptr) {
            saveFile(loadedPage->fPage, loadedPage->fFilename);
        }
    }
}

void PrpShopMain::performSaveAs()
{
    static QString s_formats[] = {
        "Uru Prime/UU Pages (*.prp)",
        "Path of the Shell Pages (*.prp)",
        "MOUL Pages (*.prp)",
        "Myst 5 Pages (*.prp)",
        "Hex Isle Pages (*.prp)"
    };

    QPlasmaTreeItem* pageItem = currentItem();
    if (pageItem == nullptr || pageItem->type() != QPlasmaTreeItem::kTypePage)
        return;

    PrpShopLoadedPage* loadedPage = fLoadedLocations.value(pageItem->page()->getLocation());

    QString saveDir = loadedPage->fFilename.isEmpty()
                    ? fDialogDir
                    : loadedPage->fFilename;
    QString selFormat;
    switch (fResMgr.getVer()) {
    case PlasmaVer::pvPrime:
        selFormat = s_formats[0];
        break;
    case PlasmaVer::pvPots:
        selFormat = s_formats[1];
        break;
    case PlasmaVer::pvMoul:
        selFormat = s_formats[2];
        break;
    case PlasmaVer::pvEoa:
        selFormat = s_formats[3];
        break;
    case PlasmaVer::pvHex:
        selFormat = s_formats[4];
        break;
    default:
        selFormat = s_formats[1];
        break;
    }
    QString filename = QFileDialog::getSaveFileName(this,
                            tr("Save PRP"), saveDir,
                            tr("%1;;%2;;%3;;%4;;%5")
                               .arg(s_formats[0]).arg(s_formats[1])
                               .arg(s_formats[2]).arg(s_formats[3])
                               .arg(s_formats[4]),
                            &selFormat);

    if (!filename.isEmpty()) {
        if (selFormat == s_formats[0])
            fResMgr.setVer(PlasmaVer::pvPrime, true);
        else if (selFormat == s_formats[1])
            fResMgr.setVer(PlasmaVer::pvPots, true);
        else if (selFormat == s_formats[2])
            fResMgr.setVer(PlasmaVer::pvMoul, true);
        else if (selFormat == s_formats[3])
            fResMgr.setVer(PlasmaVer::pvEoa, true);
        else if (selFormat == s_formats[4])
            fResMgr.setVer(PlasmaVer::pvHex, true);

        saveFile(pageItem->page(), filename);
        QDir dir = QDir(filename);
        dir.cdUp();
        fDialogDir = dir.absolutePath();
    }
}

void PrpShopMain::saveFile(plPageInfo* page, QString filename)
{
    saveProps(currentItem());
    QList<QMdiSubWindow*> windows = fMdiArea->subWindowList();
    QList<QMdiSubWindow*>::ConstIterator it;
    for (it = windows.constBegin(); it != windows.constEnd(); it++) {
        QCreatable* creWin = qobject_cast<QCreatable*>((*it)->widget());
        if (creWin)
            creWin->saveDamage();
    }
    fResMgr.WritePage(qstr2st(filename), page);
}

void PrpShopMain::saveProps(QPlasmaTreeItem* item)
{
    if (item != NULL) {
        bool reinit = false;
        bool refreshTree = false;
        if (item->type() == QPlasmaTreeItem::kTypePage) {
            PrpShopLoadedPage* loadedPage = fLoadedLocations.value(item->page()->getLocation());
            if (loadedPage == nullptr) {
                // This generally happens while closing a page.
                // A currentItemChanged signal is emitted when the page item is removed from the tree,
                // which happens after the corresponding fLoadedLocations entry has already been removed.
                return;
            }
            if (fAgeName->text() != st2qstr(loadedPage->fPage->getAge())) {
                loadedPage->fPage->setAge(qstr2st(fAgeName->text()));
                loadPage(loadedPage->fPage, loadedPage->fFilename);
                refreshTree = true;
            }
            if (fPageName->text() != st2qstr(loadedPage->fPage->getPage())) {
                loadedPage->fPage->setPage(qstr2st(fPageName->text()));
                reinit = true;
                refreshTree = true;
            }
            if (fReleaseVersion->value() != (int)loadedPage->fPage->getReleaseVersion())
                loadedPage->fPage->setReleaseVersion(fReleaseVersion->value());
            plLocation loc;
            loc.setSeqPrefix(fSeqPrefix->value());
            loc.setPageNum(fSeqSuffix->value());
            loc.setFlags((fLocationFlags[kLocLocalOnly]->isChecked() ? plLocation::kLocalOnly : 0)
                       | (fLocationFlags[kLocVolatile]->isChecked() ? plLocation::kVolatile : 0)
                       | (fLocationFlags[kLocItinerant]->isChecked() ? plLocation::kItinerant : 0)
                       | (fLocationFlags[kLocReserved]->isChecked() ? plLocation::kReserved : 0)
                       | (fLocationFlags[kLocBuiltIn]->isChecked() ? plLocation::kBuiltIn : 0));
            if (loc != loadedPage->fPage->getLocation()) {
                fLoadedLocations.insert(loc, fLoadedLocations.take(loadedPage->fPage->getLocation()));
                fResMgr.ChangeLocation(loadedPage->fPage->getLocation(), loc);
                reinit = true;
            }
            if (reinit) {
                loadedPage->fTypesItem->reinit();
                loadedPage->fTypesItem->parent()->sortChildren(0, Qt::AscendingOrder);
                loadedPage->fStructureItem->reinit();
                loadedPage->fStructureItem->parent()->sortChildren(0, Qt::AscendingOrder);
            }
        } else if (item->type() == QPlasmaTreeItem::kTypeKO) {
            if (item->obj() != NULL) {
                plKey key = item->key();
                if (fObjName->text() != st2qstr(key->getName())) {
                    key->setName(qstr2st(fObjName->text()));
                    reinit = true;
                    refreshTree = true;
                }
                plLoadMask mask = key->getLoadMask();
                mask.setQuality(fLoadMaskQ[0]->value(), fLoadMaskQ[1]->value());
                key->setLoadMask(mask);
                if (fCloneIdBox->isChecked())
                    key->setCloneIDs(fCloneId->value(), fClonePlayerId->value());

                if (reinit) {
                    // We need to reinit the item in both trees,
                    // so it's not enough to just do item->reinit().
                    QPlasmaTreeItem* typesObjectItem = findObjectInTypesTree(key);
                    if (typesObjectItem != nullptr) {
                        typesObjectItem->reinit();
                    }
                    // Finding the object in the structure tree is too annoying,
                    // so just rebuild the entire thing, lol.
                    rebuildStructureTree(findPageForItem(item));
                }
            }
        }
        if (refreshTree) {
            fTypesTree->sortItems(0, Qt::AscendingOrder);
            fStructureTree->sortItems(0, Qt::AscendingOrder);
        }
    }
}

void PrpShopMain::rebuildStructureTree(PrpShopLoadedPage* loadedPage)
{
    qDeleteAll(loadedPage->fStructureItem->takeChildren());

    // Collect all objects in this page and create tree items for them,
    // but don't actually add them to the tree yet.
    std::map<plKey, QPlasmaTreeItem*> objectItems;
    plLocation loc = loadedPage->fPage->getLocation();
    for (short type : fResMgr.getTypes(loc, true)) {
        for (const plKey& key : fResMgr.getKeys(loc, type, true)) {
            objectItems.emplace(key, new QPlasmaTreeItem(key, QPlasmaTreeItem::kObjectStructure));
        }
    }

    // Find all objects referenced by other objects
    // and move their items under the item of the referencing object.
    // Start with the default priority and add lower-priority references later
    // so that objects are placed under high-priority referring objects where possible.
    for (int prio = pqRefPriority::kDefault; prio < pqRefPriority::kMax; prio++) {
        for (const auto& pair : objectItems) {
            QPlasmaTreeItem* item = pair.second;
            for (const plKey& contained : pqGetReferencedKeys(item->obj(), static_cast<pqRefPriority>(prio))) {
                // Ignore null keys.
                if (!contained.Exists()) {
                    continue;
                }
                auto it = objectItems.find(contained);
                // Ignore keys from other pages for now.
                if (it == objectItems.end()) {
                    continue;
                }
                QPlasmaTreeItem* containedItem = it->second;
                bool repeated = false;
                // Check if the contained object already has a place elsewhere in the tree.
                if (containedItem->parent() != nullptr) {
                    repeated = true;
                } else {
                    // Check if the contained object is a (possibly indirect) parent of the containing object.
                    // (For example, scene objects have a reference back to their scene node.)
                    for (QTreeWidgetItem* parent = item; parent != nullptr; parent = parent->parent()) {
                        if (parent == containedItem) {
                            repeated = true;
                            break;
                        }
                    }
                }
                if (repeated) {
                    // The contained object already appears elsewhere in the tree.
                    // so only add a placeholder here that has no further children.
                    // Except if it's a lower-priority reference, don't add it at all,
                    // because they're generally not interesting enough.
                    if (prio == pqRefPriority::kDefault) {
                        new QPlasmaTreeItem(item, contained, QPlasmaTreeItem::kObjectStructureRepeated);
                    }
                } else {
                    // The contained object doesn't have a place in the tree yet,
                    // so move it under this object.
                    item->addChild(containedItem);
                }
            }
        }
    }

    // Find all objects that still don't have a place
    // and add them as "top-level objects" under the page item.
    for (const auto& pair : objectItems) {
        QPlasmaTreeItem* item = pair.second;
        if (item->parent() == nullptr) {
            loadedPage->fStructureItem->addChild(item);
        }
    }
}

PrpShopLoadedPage* PrpShopMain::loadPage(plPageInfo* page, QString filename)
{
    // See if the page is already loaded -- if so, then we have reloaded it, and need to rebuild
    PrpShopLoadedPage* loadedPage = fLoadedLocations.value(page->getLocation());
    if (loadedPage != nullptr) {
        // Reuse the loaded page object and the tree item for the page,
        // but remove all existing children of the tree item
        qDeleteAll(loadedPage->fTypesItem->takeChildren());
        qDeleteAll(loadedPage->fStructureItem->takeChildren());
        loadedPage->fFilename = filename;
        loadedPage->fPage = page;
    } else {
        // Find or create the Age folder
        QString ageName = st2qstr(page->getAge());
        PrpShopLoadedAge* loadedAge = fLoadedAges.value(ageName);
        if (loadedAge == nullptr) {
            int seqPrefix = page->getLocation().getSeqPrefix();
            auto typesAgeItem = new QPlasmaTreeItem(fTypesTree, ageName, seqPrefix);
            auto structureAgeItem = new QPlasmaTreeItem(fStructureTree, ageName, seqPrefix);
            loadedAge = new PrpShopLoadedAge {ageName, seqPrefix, false, false, typesAgeItem, structureAgeItem};
            fLoadedAges.insert(ageName, loadedAge);
        }

        // Treat BuiltIn and Textures PRPs specially:
        if (page->getLocation().getPageNum() == -1)
            loadedAge->fHasTextures = true;
        else if (page->getLocation().getPageNum() == -2)
            loadedAge->fHasBuiltIn = true;

        // And now the Page entry
        auto typesPageItem = new QPlasmaTreeItem(loadedAge->fTypesItem, page);
        auto structurePageItem = new QPlasmaTreeItem(loadedAge->fStructureItem, page);
        loadedPage = new PrpShopLoadedPage {loadedAge, filename, page, typesPageItem, structurePageItem};
        fLoadedLocations.insert(page->getLocation(), loadedPage);
    }

    // Populate the type folders and actual objects
    std::vector<short> types = fResMgr.getTypes(page->getLocation(), true);
    for (size_t i=0; i<types.size(); i++) {
        auto typeItem = new QPlasmaTreeItem(loadedPage->fTypesItem, types[i]);

        std::vector<plKey> keys = fResMgr.getKeys(page->getLocation(), types[i], true);
        for (size_t j=0; j<keys.size(); j++)
            new QPlasmaTreeItem(typeItem, keys[j]);
    }

    // Build the structure tree
    rebuildStructureTree(loadedPage);

    return loadedPage;
}

void PrpShopMain::createNewObject()
{
    if (fResMgr.getLocations().size() < 1) {
        QMessageBox msgBox(QMessageBox::Critical, tr("Error"),
                           tr("You must have at least one page loaded to create objects"),
                           QMessageBox::Ok, this);
        msgBox.exec();
        return;
    }

    QNewKeyDialog dlg(this);
    QPlasmaTreeItem* item = currentItem();
    if (item != NULL) {
        if (item->type() == QPlasmaTreeItem::kTypeAge) {
            if (item->childCount() != 0) {
                QPlasmaTreeItem* child = (QPlasmaTreeItem*)item->child(0);
                if (child->type() != QPlasmaTreeItem::kTypePage)
                    throw hsBadParamException(__FILE__, __LINE__, "Got non-page child");
                dlg.init(&fResMgr, plLocation(child->page()->getLocation()));
            } else {
                dlg.init(&fResMgr);
            }
        } else if (item->type() == QPlasmaTreeItem::kTypePage) {
            dlg.init(&fResMgr, item->page()->getLocation());
        } else if (item->type() == QPlasmaTreeItem::kTypeClassType) {
            if (item->parent()->type() != QPlasmaTreeItem::kTypePage)
                throw hsBadParamException(__FILE__, __LINE__, "Got non-page parent");
            dlg.init(&fResMgr, ((QPlasmaTreeItem*)item->parent())->page()->getLocation(), item->classType());
        } else if (item->type() == QPlasmaTreeItem::kTypeKO) {
            dlg.init(&fResMgr, item->key()->getLocation(), item->key()->getType());
        } else {
            dlg.init(&fResMgr);
        }
    } else {
        dlg.init(&fResMgr);
    }

    if (dlg.exec() == QDialog::Accepted) {
        plLocation loc = dlg.location();
        short type = dlg.type();

        // Create and register the object
        hsKeyedObject* ko = hsKeyedObject::Convert(plFactory::Create(type));
        if (ko == NULL)
            throw hsBadParamException(__FILE__, __LINE__, "Invalid KeyedObject");
        ko->init(qstr2st(dlg.name()));
        fResMgr.AddObject(loc, ko);

        // Now add it to the tree
        addNewObjectToTree(ko);

        // And open it for convenience
        editCreatable(ko);
    }
}

void PrpShopMain::showAgePageIDs(bool show)
{
    s_showAgePageIDs = show;

    // Refresh currently loaded ages/pages
    for (int w = 0; w < fBrowserTabs->count(); w++) {
        auto tree = qobject_cast<QTreeWidget*>(fBrowserTabs->widget(w));
        for (int i = 0; i < tree->topLevelItemCount(); i++) {
            QPlasmaTreeItem* ageNode = (QPlasmaTreeItem*)tree->topLevelItem(i);
            for (int j = 0; j < ageNode->childCount(); j++) {
                QPlasmaTreeItem* pageNode = (QPlasmaTreeItem*)ageNode->child(j);
                pageNode->reinit();
            }
            ageNode->sortChildren(0, Qt::AscendingOrder);
            ageNode->reinit();
        }
        tree->sortItems(0, Qt::AscendingOrder);
    }
}

static void recursivelyReinitChildren(QTreeWidgetItem* item)
{
    for (int i = 0; i < item->childCount(); i++) {
        auto child = static_cast<QPlasmaTreeItem*>(item->child(i));
        child->reinit();
        recursivelyReinitChildren(child);
    }
}

void PrpShopMain::showTypeIDs(bool show)
{
    s_showTypeIDs = show;

    // Refresh the folder display for currently loaded pages
    for (int i=0; i<fTypesTree->topLevelItemCount(); i++) {
        QPlasmaTreeItem* ageNode = (QPlasmaTreeItem*)fTypesTree->topLevelItem(i);
        for (int j=0; j<ageNode->childCount(); j++) {
            QPlasmaTreeItem* pageNode = (QPlasmaTreeItem*)ageNode->child(j);
            for (int t=0; t<pageNode->childCount(); t++) {
                QPlasmaTreeItem* typeNode = (QPlasmaTreeItem*)pageNode->child(t);
                typeNode->reinit();
            }
            pageNode->sortChildren(0, Qt::AscendingOrder);
        }
    }

    // Objects in the structure tree contain type names, so recursively reinit them all...
    for (int i = 0; i < fStructureTree->topLevelItemCount(); i++) {
        auto ageNode = static_cast<QPlasmaTreeItem*>(fStructureTree->topLevelItem(i));
        for (int j = 0; j < ageNode->childCount(); j++) {
            QTreeWidgetItem* child = ageNode->child(j);
            recursivelyReinitChildren(child);
            child->sortChildren(0, Qt::AscendingOrder);
        }
    }
}

int main(int argc, char* argv[])
{
    // Redirect libPlasma's debug stuff to PrpShop.log
    QString logpath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir;
    dir.mkpath(logpath);
    logpath += "/PrpShop.log";
    plDebug::InitFile(plDebug::kDLAll, logpath.toUtf8().constData());

    QApplication app(argc, argv);
    PrpShopMain mainWnd;
    mainWnd.show();
    for (int i=1; i<argc; i++)
        mainWnd.loadFile(argv[i]);
    mainWnd.setAcceptDrops(true);
    return app.exec();
}
