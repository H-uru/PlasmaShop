#include <QApplication>
#include <QSettings>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QMdiSubWindow>
#include <Debug/plDebug.h>
#include <ResManager/plFactory.h>
#include <PRP/Surface/plMipmap.h>

#include "Main.h"
#include "../QPlasma.h"
#include "QPlasmaUtils.h"
#include "QKeyDialog.h"
#include "PRP/QCreatable.h"

PrpShopMain* PrpShopMain::sInstance = NULL;
PrpShopMain* PrpShopMain::Instance()
{ return sInstance; }

plResManager* PrpShopMain::ResManager()
{ return &sInstance->fResMgr; }

PrpShopMain::PrpShopMain()
{
    // Set up the "Magic" instance
    if (sInstance != NULL)
        throw hsBadParamException(__FILE__, __LINE__, "PrpShop broke...");
    sInstance = this;

    // Basic Form Settings
    setWindowTitle("PrpShop " PRPSHOP_VERSION);
    setWindowIcon(QIcon(":/res/PrpShop.png"));
    setDockOptions(QMainWindow::AnimatedDocks);

    // Set up actions
    fActions[kFileNewPage] = new QAction(QIcon(":/res/filenew.png"), tr("New &Page"), this);
    fActions[kFileOpen] = new QAction(QIcon(":/res/fileopen.png"), tr("&Open..."), this);
    fActions[kFileSave] = new QAction(QIcon(":/res/filesave.png"), tr("&Save"), this);
    fActions[kFileSaveAs] = new QAction(tr("Sa&ve As..."), this);
    fActions[kFileExit] = new QAction(tr("E&xit"), this);
    fActions[kToolsProperties] = new QAction(tr("Show &Properties Pane"), this);
    fActions[kToolsNewObject] = new QAction(tr("&New Object..."), this);
    fActions[kWindowPrev] = new QAction(tr("&Previous"), this);
    fActions[kWindowNext] = new QAction(tr("&Next"), this);
    fActions[kWindowTile] = new QAction(tr("&Tile"), this);
    fActions[kWindowCascade] = new QAction(tr("&Cascade"), this);
    fActions[kWindowClose] = new QAction(tr("C&lose"), this);
    fActions[kWindowCloseAll] = new QAction(tr("Cl&ose All"), this);

    fActions[kTreeClose] = new QAction(tr("&Close"), this);
    fActions[kTreeEdit] = new QAction(tr("&Edit"), this);
    fActions[kTreePreview] = new QAction(tr("&Preview"), this);
    fActions[kTreeDelete] = new QAction(tr("&Delete"), this);
    fActions[kTreeImport] = new QAction(tr("&Import..."), this);
    fActions[kTreeExport] = new QAction(tr("E&xport..."), this);
    fActions[kTreeExportDDS] = new QAction(tr("Export &DDS..."), this);
    fActions[kTreeExportJPEG] = new QAction(tr("Export &JPEG..."), this);
    fActions[kTreeExportOBJ] = new QAction(tr("Export &OBJ..."), this);

    fActions[kFileOpen]->setShortcut(Qt::CTRL + Qt::Key_O);
    fActions[kFileSave]->setShortcut(Qt::CTRL + Qt::Key_S);
    fActions[kFileExit]->setShortcut(Qt::ALT + Qt::Key_F4);
    fActions[kWindowClose]->setShortcut(Qt::CTRL + Qt::Key_W);
    fActions[kToolsProperties]->setCheckable(true);
    fActions[kToolsProperties]->setChecked(true);

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
    fBrowserTree = new QTreeWidget(fBrowserDock);
    fBrowserDock->setWidget(fBrowserTree);
    fBrowserDock->setAllowedAreas(Qt::LeftDockWidgetArea |
                                  Qt::RightDockWidgetArea);
    fBrowserDock->setFeatures(QDockWidget::DockWidgetMovable |
                              QDockWidget::DockWidgetFloatable);
    fBrowserTree->setUniformRowHeights(true);
    fBrowserTree->setHeaderHidden(true);
    fBrowserTree->setContextMenuPolicy(Qt::CustomContextMenu);
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
    connect(fActions[kFileExit], SIGNAL(activated()), this, SLOT(close()));
    connect(fActions[kFileOpen], SIGNAL(activated()), this, SLOT(openFiles()));
    connect(fActions[kFileSave], SIGNAL(activated()), this, SLOT(performSave()));
    connect(fActions[kFileSaveAs], SIGNAL(activated()), this, SLOT(performSaveAs()));

    connect(fActions[kToolsProperties], SIGNAL(toggled(bool)),
            fPropertyDock, SLOT(setVisible(bool)));
    connect(fPropertyDock, SIGNAL(visibilityChanged(bool)),
            fActions[kToolsProperties], SLOT(setChecked(bool)));
    connect(fActions[kToolsNewObject], SIGNAL(activated()),
            this, SLOT(createNewObject()));

    connect(fActions[kWindowPrev], SIGNAL(activated()),
            fMdiArea, SLOT(activatePreviousSubWindow()));
    connect(fActions[kWindowNext], SIGNAL(activated()),
            fMdiArea, SLOT(activateNextSubWindow()));
    connect(fActions[kWindowTile], SIGNAL(activated()),
            fMdiArea, SLOT(tileSubWindows()));
    connect(fActions[kWindowCascade], SIGNAL(activated()),
            fMdiArea, SLOT(cascadeSubWindows()));
    connect(fActions[kWindowClose], SIGNAL(activated()),
            fMdiArea, SLOT(closeActiveSubWindow()));
    connect(fActions[kWindowCloseAll], SIGNAL(activated()),
            fMdiArea, SLOT(closeAllSubWindows()));

    connect(fActions[kTreeClose], SIGNAL(activated()), this, SLOT(treeClose()));
    connect(fActions[kTreeEdit], SIGNAL(activated()), this, SLOT(treeEdit()));
    connect(fActions[kTreePreview], SIGNAL(activated()), this, SLOT(treePreview()));
    connect(fActions[kTreeDelete], SIGNAL(activated()), this, SLOT(treeDelete()));
    connect(fActions[kTreeImport], SIGNAL(activated()), this, SLOT(treeImport()));
    connect(fActions[kTreeExport], SIGNAL(activated()), this, SLOT(treeExport()));
    connect(fActions[kTreeExportDDS], SIGNAL(activated()), this, SLOT(treeExportDDS()));
    connect(fActions[kTreeExportJPEG], SIGNAL(activated()), this, SLOT(treeExportJPEG()));
    connect(fActions[kTreeExportOBJ], SIGNAL(activated()), this, SLOT(treeExportOBJ()));

    connect(fBrowserTree, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)),
            this, SLOT(treeItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)));
    connect(fBrowserTree, SIGNAL(itemActivated(QTreeWidgetItem*, int)),
            this, SLOT(treeItemActivated(QTreeWidgetItem*, int)));
    connect(fBrowserTree, SIGNAL(customContextMenuRequested(const QPoint&)),
            this, SLOT(treeContextMenu(const QPoint&)));

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
            fSeqPrefix->setRange(-0x10000, 0xFFFF);
            fSeqSuffix = new QSpinBox(locationGrp);
            fSeqSuffix->setRange(-0x10000, 0xFFFF);
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
    if (item == NULL) {
        setPropertyPage(kPropsNone);
    } else if (item->type() == QPlasmaTreeItem::kTypeAge) {
        setPropertyPage(kPropsAge);
        fAgeName->setText(item->age());
    } else if (item->type() == QPlasmaTreeItem::kTypePage) {
        setPropertyPage(kPropsPage);
        fAgeName->setText(~item->page()->getAge());
        fPageName->setText(~item->page()->getPage());
        fReleaseVersion->setValue(item->page()->getReleaseVersion());
        fSeqPrefix->setValue(item->page()->getLocation().getSeqPrefix());
        fSeqSuffix->setValue(item->page()->getLocation().getPageNum());
        fLocationFlags[kLocLocalOnly]->setChecked(item->page()->getLocation().getFlags() & plLocation::kLocalOnly);
        fLocationFlags[kLocVolatile]->setChecked(item->page()->getLocation().getFlags() & plLocation::kVolatile);
        fLocationFlags[kLocItinerant]->setChecked(item->page()->getLocation().getFlags() & plLocation::kItinerant);
        fLocationFlags[kLocReserved]->setChecked(item->page()->getLocation().getFlags() & plLocation::kReserved);
        fLocationFlags[kLocBuiltIn]->setChecked(item->page()->getLocation().getFlags() & plLocation::kBuiltIn);
    } else if (item->type() == QPlasmaTreeItem::kTypeKO) {
        setPropertyPage(kPropsKO);
        fObjName->setText(~item->obj()->getKey()->getName());
        fObjType->setText(item->obj()->ClassName());
        fLoadMaskQ[0]->setValue(item->obj()->getKey()->getLoadMask().getQuality(0));
        fLoadMaskQ[1]->setValue(item->obj()->getKey()->getLoadMask().getQuality(1));
        fCloneId->setValue(item->obj()->getKey()->getCloneID());
        fClonePlayerId->setValue(item->obj()->getKey()->getClonePlayerID());
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
    fBrowserTree->setCurrentItem(fBrowserTree->itemAt(pos));
    QPlasmaTreeItem* item = (QPlasmaTreeItem*)fBrowserTree->currentItem();
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
        menu.addAction(fActions[kTreePreview]);
        menu.addSeparator();
        menu.addAction(fActions[kTreeDelete]);
        menu.addAction(fActions[kTreeImport]);
        menu.addAction(fActions[kTreeExport]);
        if (item->obj()->ClassInstance(kMipmap)) {
            plMipmap* mipmap = plMipmap::Convert(item->obj());
            if (mipmap->getCompressionType() == plMipmap::kJPEGCompression)
                menu.addAction(fActions[kTreeExportJPEG]);
            else
                menu.addAction(fActions[kTreeExportDDS]);
        } else if (item->obj()->ClassInstance(kSceneNode)
                   || item->obj()->ClassInstance(kSceneObject)) {
            menu.addAction(fActions[kTreeExportOBJ]);
        }
        menu.setDefaultAction(fActions[kTreeEdit]);
        fActions[kTreePreview]->setEnabled(pqCanPreviewType(item->obj()->ClassIndex()));
    } else {
        menu.addAction(fActions[kTreeImport]);
    }
    menu.exec(fBrowserTree->viewport()->mapToGlobal(pos));
}

void PrpShopMain::treeClose()
{
    QPlasmaTreeItem* item = (QPlasmaTreeItem*)fBrowserTree->currentItem();
    if (item == NULL)
        return;

    // Make sure no windows are open with objects we want to unload...
    fMdiArea->closeAllSubWindows();

    if (item->type() == QPlasmaTreeItem::kTypeAge) {
        QHash<plLocation, QPlasmaTreeItem*>::Iterator it;
        for (it = fLoadedLocations.begin(); it != fLoadedLocations.end(); ) {
            if (~(*it)->page()->getAge() == item->age()) {
                fResMgr.UnloadPage((*it)->page()->getLocation());
                it = fLoadedLocations.erase(it);
            } else {
                it++;
            }
        }
        delete item;
    } else if (item->type() == QPlasmaTreeItem::kTypePage) {
        plLocation loc = item->page()->getLocation();
        QPlasmaTreeItem* age = (QPlasmaTreeItem*)item->parent();
        delete item;
        QHash<plLocation, QPlasmaTreeItem*>::Iterator it = fLoadedLocations.find(loc);
        fLoadedLocations.erase(it);
        fResMgr.UnloadPage(loc);
        if (age->childCount() == 0)
            delete age;
    }
}

void PrpShopMain::treeEdit()
{
    QPlasmaTreeItem* item = (QPlasmaTreeItem*)fBrowserTree->currentItem();
    if (item == NULL || item->obj() == NULL)
        return;
    editCreatable(item->obj());
}

void PrpShopMain::treePreview()
{
    QPlasmaTreeItem* item = (QPlasmaTreeItem*)fBrowserTree->currentItem();
    if (item == NULL || item->obj() == NULL)
        return;
    editCreatable(item->obj(), 0x1000 | item->obj()->ClassIndex());
}

void PrpShopMain::treeDelete()
{
    QPlasmaTreeItem* item = (QPlasmaTreeItem*)fBrowserTree->currentItem();
    if (item == NULL || item->obj() == NULL)
        return;
    fResMgr.DelObject(item->obj()->getKey());
    QPlasmaTreeItem* folder = (QPlasmaTreeItem*)item->parent();
    delete item;

    if (folder->childCount() == 0)
        delete folder;
}

QPlasmaTreeItem* PrpShopMain::ensurePath(const plLocation& loc, short objType)
{
    plPageInfo* page = fResMgr.FindPage(loc);
    QPlasmaTreeItem* ageItem = NULL;
    QString ageName = ~page->getAge();
    QString pageName = ~page->getPage();
    for (int i=0; i<fBrowserTree->topLevelItemCount(); i++) {
        if (fBrowserTree->topLevelItem(i)->text(0) == ageName) {
            ageItem = (QPlasmaTreeItem*)fBrowserTree->topLevelItem(i);
            break;
        }
    }
    if (ageItem == NULL)
        throw hsBadParamException(__FILE__, __LINE__, "Invalid Age");

    QPlasmaTreeItem* pageItem = NULL;
    for (int i=0; i<ageItem->childCount(); i++) {
        if (ageItem->child(i)->text(0) == pageName) {
            pageItem = (QPlasmaTreeItem*)ageItem->child(i);
            break;
        }
    }
    if (pageItem == NULL)
        throw hsBadParamException(__FILE__, __LINE__, "Invalid Page");

    QPlasmaTreeItem* folderItem = NULL;
    QString typeName = pqGetFriendlyClassName(objType);
    for (int i=0; i<pageItem->childCount(); i++) {
        if (pageItem->child(i)->text(0) == typeName) {
            folderItem = (QPlasmaTreeItem*)pageItem->child(i);
            break;
        }
    }
    if (folderItem == NULL) {
        folderItem = new QPlasmaTreeItem(pageItem);
        folderItem->setText(0, typeName);
    }
    return folderItem;
}

void PrpShopMain::treeImport()
{
    QPlasmaTreeItem* pageItem = findCurrentPageItem();
    if (pageItem == NULL)
        return;

    QStringList files = QFileDialog::getOpenFileNames(this,
                            tr("Import Raw Object(s)"), fDialogDir,
                            "Plasma Objects (*.po *.mof *.uof)");
    QStringList filesIt = files;
    for (QStringList::Iterator it = filesIt.begin(); it != filesIt.end(); it++) {
        try {
            hsFileStream S(fResMgr.getVer());
            S.open(~(*it), fmRead);
            plCreatable* pCre = fResMgr.ReadCreatable(&S);
            hsKeyedObject* ko = hsKeyedObject::Convert(pCre);
            if (pCre != NULL && ko == NULL) {
                delete pCre;
                throw hsBadParamException(__FILE__, __LINE__, "Invalid object class");
            }
            // The key is already added to the ResMgr, but we need to ensure
            // its location is correctly updated
            plLocation loc = pageItem->page()->getLocation();
            fResMgr.MoveKey(ko->getKey(), loc);

            // Now add it to the tree
            QPlasmaTreeItem* folderItem = ensurePath(loc, ko->ClassIndex());
            new QPlasmaTreeItem(folderItem, ko);
            fBrowserTree->sortItems(0, Qt::AscendingOrder);
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
    QPlasmaTreeItem* item = (QPlasmaTreeItem*)fBrowserTree->currentItem();
    if (item == NULL || item->obj() == NULL)
        return;

    QString genPath = tr("%1/[%2]%3.po").arg(fDialogDir)
                                        .arg(item->obj()->ClassName())
                                        .arg(~item->obj()->getKey()->getName());
    QString filename = QFileDialog::getSaveFileName(this,
                            tr("Export Raw Object"), genPath,
                            "Plasma Objects (*.po *.mof *.uof)");
    if (!filename.isEmpty()) {
        try {
            hsFileStream S(fResMgr.getVer());
            S.open(~filename, fmCreate);
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

void PrpShopMain::treeExportDDS()
{
    QMessageBox msgBox(QMessageBox::Warning, tr("Export DDS"),
                       tr("DDS Exporter not yet implemented"),
                       QMessageBox::Ok, this);
    msgBox.exec();
}

void PrpShopMain::treeExportJPEG()
{
    QMessageBox msgBox(QMessageBox::Warning, tr("Export JPEG"),
                       tr("JPEG Exporter not yet implemented"),
                       QMessageBox::Ok, this);
    msgBox.exec();
}

void PrpShopMain::treeExportOBJ()
{
    QMessageBox msgBox(QMessageBox::Warning, tr("Export OBJ"),
                       tr("OBJ Exporter not yet implemented"),
                       QMessageBox::Ok, this);
    msgBox.exec();
}

void PrpShopMain::editCreatable(plCreatable* pCre, short forceType)
{
    QList<QMdiSubWindow*> windows = fMdiArea->subWindowList();
    QList<QMdiSubWindow*>::Iterator it;
    for (it = windows.begin(); it != windows.end(); it++) {
        if (((QCreatable*)(*it)->widget())->isMatch(pCre, forceType)) {
            fMdiArea->setActiveSubWindow(*it);
            break;
        }
    }
    if (it == windows.end()) {
        QCreatable* win = pqMakeCreatableForm(pCre, this, forceType);
        if (win != NULL) {
            QMdiSubWindow* subWin = fMdiArea->addSubWindow(win);
            subWin->setWindowIcon(win->windowIcon());
            subWin->show();
        }
    }
}

void PrpShopMain::openFiles()
{
    QStringList files = QFileDialog::getOpenFileNames(this,
                            tr("Open File(s)"), fDialogDir,
                            "All supported types (*.age *.prp);;"
                            "Age Files (*.age);;"
                            "Page Files (*.prp)");
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
    QDir dir(filename);
    filename = dir.absolutePath();

    if (filename.endsWith(".age", Qt::CaseInsensitive)) {
        try {
            plAgeInfo* age = fResMgr.ReadAge(~filename, true);
            QDir path(filename);
            path.cdUp();
            for (size_t i=0; i<age->getNumPages(); i++) {
                QString prp = path.absoluteFilePath(~age->getPageFilename(i, fResMgr.getVer()));
                if (QFile::exists(prp)) {
                    loadPage(fResMgr.FindPage(age->getPageLoc(i, fResMgr.getVer())), prp);
                } else {
                    plDebug::Warning("Could not find page %s referenced from %s",
                                     age->getPageFilename(i, fResMgr.getVer()).cstr(),
                                     filename.toUtf8().data());
                }
            }
            for (size_t i=0; i<age->getNumCommonPages(fResMgr.getVer()); i++) {
                QString prp = path.absoluteFilePath(~age->getCommonPageFilename(i, fResMgr.getVer()));
                if (QFile::exists(prp)) {
                    loadPage(fResMgr.FindPage(age->getCommonPageLoc(i, fResMgr.getVer())), prp);
                } else {
                    plDebug::Warning("Common page %s was not found",
                                     age->getCommonPageFilename(i, fResMgr.getVer()).cstr());
                }
            }
        } catch (std::exception& ex) {
            QMessageBox msgBox(QMessageBox::Warning, tr("Error"),
                               tr("Error Loading File %1:\n%2").arg(filename).arg(ex.what()),
                               QMessageBox::Ok, this);
            msgBox.exec();
        }
    } else if (filename.endsWith(".prp", Qt::CaseInsensitive)) {
        try {
            plPageInfo* page = fResMgr.ReadPage(~filename);
            QPlasmaTreeItem* pageItem = loadPage(page, filename);
            QPlasmaTreeItem* ageItem = (QPlasmaTreeItem*)pageItem->parent();

            // Manually check for and load the BuiltIn and Textures PRPs
            QDir path(filename);
            path.cdUp(); // Get rid of the filename >.>
            if (ageItem != NULL && !ageItem->hasTextures()) {
                QString texPath = ~page->getAge();
                if (fResMgr.getVer() < pvEoa)
                    texPath += "_District";
                texPath += "_Textures.prp";
                texPath = path.absoluteFilePath(texPath);
                if (QFile::exists(texPath))
                    loadPage(fResMgr.ReadPage(~texPath), texPath);
            }
            if (ageItem != NULL && !ageItem->hasBuiltIn()) {
                QString biPath = ~page->getAge();
                if (fResMgr.getVer() < pvEoa)
                    biPath += "_District";
                biPath += "_BuiltIn.prp";
                biPath = path.absoluteFilePath(biPath);
                if (QFile::exists(biPath))
                    loadPage(fResMgr.ReadPage(~biPath), biPath);
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
    fBrowserTree->sortItems(0, Qt::AscendingOrder);
}

QPlasmaTreeItem* PrpShopMain::findCurrentPageItem(bool isSave)
{
    QPlasmaTreeItem* item = (QPlasmaTreeItem*)fBrowserTree->currentItem();
    if (item == NULL)
        return NULL;

    if (item->type() == QPlasmaTreeItem::kTypeAge) {
        if (isSave) {
            // Save all pages belonging to this age
            for (int i=0; i<item->childCount(); i++) {
                QPlasmaTreeItem* pageItem = (QPlasmaTreeItem*)item->child(i);
                if (pageItem->type() != QPlasmaTreeItem::kTypePage)
                    throw hsBadParamException(__FILE__, __LINE__, "Got non-page child");
                saveFile(pageItem->page(), pageItem->filename());
            }
        }
        return NULL;
    } else if (item->type() == QPlasmaTreeItem::kTypePage) {
        return item;
    } else if (item->type() == QPlasmaTreeItem::kTypeKO) {
        return fLoadedLocations.value(item->obj()->getKey()->getLocation(), NULL);
    } else {
        // Type folder
        QPlasmaTreeItem* pageItem = (QPlasmaTreeItem*)item->parent();
        if (pageItem->type() != QPlasmaTreeItem::kTypePage)
            throw hsBadParamException(__FILE__, __LINE__, "Got non-page parent");
        return pageItem;
    }
}

void PrpShopMain::performSave()
{
    QPlasmaTreeItem* pageItem = findCurrentPageItem(true);
    if (pageItem != NULL)
        saveFile(pageItem->page(), pageItem->filename());
}

void PrpShopMain::performSaveAs()
{
    QPlasmaTreeItem* pageItem = findCurrentPageItem(false);
    if (pageItem == NULL)
        return;

    QString saveDir = pageItem->filename().isEmpty()
                    ? fDialogDir
                    : pageItem->filename();
    QString filename = QFileDialog::getSaveFileName(this,
                            tr("Save PRP"), saveDir,
                            "Page Files (*.prp)");
    if (!filename.isEmpty()) {
        saveFile(pageItem->page(), filename);
        QDir dir = QDir(filename);
        dir.cdUp();
        fDialogDir = dir.absolutePath();
    }
}

void PrpShopMain::saveFile(plPageInfo* page, QString filename)
{
    saveProps((QPlasmaTreeItem*)fBrowserTree->currentItem());
    QList<QMdiSubWindow*> windows = fMdiArea->subWindowList();
    QList<QMdiSubWindow*>::ConstIterator it;
    for (it = windows.constBegin(); it != windows.constEnd(); it++)
        ((QCreatable*)(*it)->widget())->saveDamage();
    fResMgr.WritePage(~filename, page);
}

void PrpShopMain::saveProps(QPlasmaTreeItem* item)
{
    if (item != NULL) {
        bool refreshTree = false;
        if (item->type() == QPlasmaTreeItem::kTypePage) {
            if (fAgeName->text() != ~item->page()->getAge()) {
                item->page()->setAge(~fAgeName->text());
                fLoadedLocations.erase(fLoadedLocations.find(item->page()->getLocation()));
                QPlasmaTreeItem* stale = item;
                item = loadPage(item->page(), item->filename());
                delete stale;
                refreshTree = true;
            }
            if (fPageName->text() != ~item->page()->getPage()) {
                item->page()->setPage(~fPageName->text());
                item->setText(0, fPageName->text());
                refreshTree = true;
            }
            if (fReleaseVersion->value() != (int)item->page()->getReleaseVersion())
                item->page()->setReleaseVersion(fReleaseVersion->value());
            plLocation loc;
            loc.setSeqPrefix(fSeqPrefix->value());
            loc.setPageNum(fSeqSuffix->value());
            loc.setFlags((fLocationFlags[kLocLocalOnly]->isChecked() ? plLocation::kLocalOnly : 0)
                       | (fLocationFlags[kLocVolatile]->isChecked() ? plLocation::kVolatile : 0)
                       | (fLocationFlags[kLocItinerant]->isChecked() ? plLocation::kItinerant : 0)
                       | (fLocationFlags[kLocReserved]->isChecked() ? plLocation::kReserved : 0)
                       | (fLocationFlags[kLocBuiltIn]->isChecked() ? plLocation::kBuiltIn : 0));
            if (loc != item->page()->getLocation()) {
                fLoadedLocations[loc] = fLoadedLocations[item->page()->getLocation()];
                fLoadedLocations.erase(fLoadedLocations.find(item->page()->getLocation()));
                fResMgr.ChangeLocation(item->page()->getLocation(), loc);
            }
        } else if (item->type() == QPlasmaTreeItem::kTypeKO) {
            if (fObjName->text() != ~item->obj()->getKey()->getName()) {
                item->obj()->getKey()->setName(~fObjName->text());
                item->setText(0, fObjName->text());
                refreshTree = true;
            }
            plLoadMask mask = item->obj()->getKey()->getLoadMask();
            mask.setQuality(fLoadMaskQ[0]->value(), fLoadMaskQ[1]->value());
            item->obj()->getKey()->setLoadMask(mask);
            if (fCloneIdBox->isChecked())
                item->obj()->getKey()->setCloneIDs(fCloneId->value(), fClonePlayerId->value());
        }
        if (refreshTree)
            fBrowserTree->sortItems(0, Qt::AscendingOrder);
    }
}

QPlasmaTreeItem* PrpShopMain::loadPage(plPageInfo* page, QString filename)
{
    // See if the page is already loaded -- return that if so
    QPlasmaTreeItem* parent = NULL;
    QPlasmaTreeItem* item = fLoadedLocations.value(page->getLocation(), NULL);
    if (item != NULL) {
        item->setFilename(filename);
        return item;
    }

    // Find or create the Age folder
    QString ageName = ~page->getAge();
    QString pageName = ~page->getPage();
    for (int i=0; i<fBrowserTree->topLevelItemCount(); i++) {
        if (fBrowserTree->topLevelItem(i)->text(0) == ageName) {
            parent = (QPlasmaTreeItem*)fBrowserTree->topLevelItem(i);
            break;
        }
    }
    if (parent == NULL)
        parent = new QPlasmaTreeItem(fBrowserTree, ageName);

    // Treat BuiltIn and Textures PRPs specially:
    if (page->getLocation().getPageNum() == -1)
        parent->setHasTextures();
    else if (page->getLocation().getPageNum() == -2)
        parent->setHasBuiltIn();

    // And now the Page entry
    item = new QPlasmaTreeItem(parent, page);

    // Populate the type folders and actual objects
    std::vector<short> types = fResMgr.getTypes(page->getLocation());
    for (size_t i=0; i<types.size(); i++) {
        QPlasmaTreeItem* folder = new QPlasmaTreeItem(item);
        folder->setText(0, pqGetFriendlyClassName(types[i]));

        std::vector<plKey> keys = fResMgr.getKeys(page->getLocation(), types[i]);
        for (size_t j=0; j<keys.size(); j++) {
            if (!keys[j].Exists() || !keys[j].isLoaded()) {
                plDebug::Debug("Got erroneous key: %s", keys[j]->toString().cstr());
                continue;
            }
            new QPlasmaTreeItem(folder, keys[j]->getObj());
        }
    }

    item->setFilename(filename);
    fLoadedLocations[page->getLocation()] = item;
    return item;
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
    dlg.init(&fResMgr);
    if (dlg.exec() == QDialog::Accepted) {
        plLocation loc = dlg.location();
        short type = dlg.type();

        // Create and register the object
        hsKeyedObject* ko = hsKeyedObject::Convert(plFactory::Create(type));
        if (ko == NULL)
            throw hsBadParamException(__FILE__, __LINE__, "Invalid KeyedObject");
        ko->init(~dlg.name());
        fResMgr.AddObject(loc, ko);

        // Now add it to the tree
        QPlasmaTreeItem* folderItem = ensurePath(loc, type);
        new QPlasmaTreeItem(folderItem, ko);
        fBrowserTree->sortItems(0, Qt::AscendingOrder);
    }
}


int main(int argc, char* argv[])
{
    // Redirect libPlasma's debug stuff to PrpShop.log
    plDebug::InitFile(plDebug::kDLAll, "PrpShop.log");

    QApplication app(argc, argv);
    PrpShopMain mainWnd;
    mainWnd.show();
    for (int i=1; i<argc; i++)
        mainWnd.loadFile(argv[i]);
    return app.exec();
}
