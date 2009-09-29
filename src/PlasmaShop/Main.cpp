#include <QApplication>
#include <QSettings>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QMdiSubWindow>
#include <QFileSystemModel>
#include <qticonloader.h>
#include <Debug/plDebug.h>

#include "Main.h"
#include "../QPlasma.h"

PlasmaShopMain* PlasmaShopMain::sInstance = NULL;
PlasmaShopMain* PlasmaShopMain::Instance()
{ return sInstance; }

PlasmaShopMain::PlasmaShopMain()
{
    // Set up the "Magic" instance
    if (sInstance != NULL)
        throw hsBadParamException(__FILE__, __LINE__, "PlasmaShop broke...");
    sInstance = this;

    // Basic Form Settings
    setWindowTitle("PlasmaShop " PLASMASHOP_VERSION);
    setWindowIcon(QIcon(":/res/PlasmaShop.png"));
    setDockOptions(QMainWindow::AnimatedDocks);

    // Set up actions
    fActions[kFileNew] = new QAction(qStdIcon("document-new"), tr("&New..."), this);
    fActions[kFileOpen] = new QAction(qStdIcon("document-open"), tr("&Open..."), this);
    fActions[kFileSave] = new QAction(qStdIcon("document-save"), tr("&Save"), this);
    fActions[kFileSaveAs] = new QAction(tr("Sa&ve As..."), this);
    fActions[kFileOptions] = new QAction(tr("&Options..."), this);
    fActions[kFileExit] = new QAction(tr("E&xit"), this);
    fActions[kWindowPrev] = new QAction(tr("&Previous"), this);
    fActions[kWindowNext] = new QAction(tr("&Next"), this);
    fActions[kWindowTile] = new QAction(tr("&Tile"), this);
    fActions[kWindowCascade] = new QAction(tr("&Cascade"), this);
    fActions[kWindowClose] = new QAction(tr("C&lose"), this);
    fActions[kWindowCloseAll] = new QAction(tr("Cl&ose All"), this);

    fActions[kTextFind] = new QAction(tr("&Find..."), this);
    fActions[kTextReplace] = new QAction(tr("&Replace..."), this);
    fActions[kTextStxNone] = new QAction(tr("&None"), this);
    fActions[kTextStxPython] = new QAction(tr("&Python"), this);
    fActions[kTextStxSDL1] = new QAction(tr("SDL (&Uru / MOUL)"), this);
    fActions[kTextStxSDL2] = new QAction(tr("SDL (Myst &5 / Crowthistle / Hex Isle)"), this);
    fActions[kTextStxIni] = new QAction(tr("&Age / Ini"), this);
    fActions[kTextStxConsole] = new QAction(tr("&Console"), this);
    fActions[kTextStxXML] = new QAction(tr("&XML"), this);
    fActions[kTextStxHex] = new QAction(tr("&Hex Isle Level"), this);
    fActions[kTextStxFX] = new QAction(tr("Shader &FX"), this);
    fActions[kTextEncNone] = new QAction(tr("&None"), this);
    fActions[kTextEncXtea] = new QAction(tr("&Uru Prime / PotS / UU"), this);
    fActions[kTextEncAes] = new QAction(tr("Myst &5 / Crowthistle / Hex Isle"), this);
    fActions[kTextEncDroid] = new QAction(tr("&MOUL (Requires Key)"), this);
    fActions[kTextTypeAnsi] = new QAction(tr("&ANSI"), this);
    fActions[kTextTypeUTF8] = new QAction(tr("UTF-&8"), this);
    fActions[kTextTypeUTF16] = new QAction(tr("UTF-1&6"), this);
    fActions[kTextTypeUTF32] = new QAction(tr("UTF-&32"), this);
    fActions[kTextExpandAll] = new QAction(tr("&Expand All"), this);
    fActions[kTextCollapseAll] = new QAction(tr("&Collapse All"), this);

    fActions[kTreeOpen] = new QAction(tr("&Open"), this);
    fActions[kTreeRename] = new QAction(tr("&Rename"), this);
    fActions[kTreeDelete] = new QAction(tr("&Delete"), this);

    fActions[kFileOpen]->setShortcut(Qt::CTRL + Qt::Key_O);
    fActions[kFileSave]->setShortcut(Qt::CTRL + Qt::Key_S);
    fActions[kFileExit]->setShortcut(Qt::ALT + Qt::Key_F4);
    fActions[kWindowClose]->setShortcut(Qt::CTRL + Qt::Key_W);

    // Main Menus
    QMenu* fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(fActions[kFileNew]);
    fileMenu->addSeparator();
    fileMenu->addAction(fActions[kFileOpen]);
    fileMenu->addAction(fActions[kFileSave]);
    fileMenu->addAction(fActions[kFileSaveAs]);
    fileMenu->addSeparator();
    fileMenu->addAction(fActions[kFileOptions]);
    fileMenu->addSeparator();
    fileMenu->addAction(fActions[kFileExit]);

    QMenu* textMenu = menuBar()->addMenu(tr("&Text"));
    textMenu->addAction(fActions[kTextFind]);
    textMenu->addAction(fActions[kTextReplace]);
    textMenu->addSeparator();
    QMenu* text_stxMenu = textMenu->addMenu(tr("&Syntax"));
    text_stxMenu->addAction(fActions[kTextStxNone]);
    text_stxMenu->addAction(fActions[kTextStxPython]);
    text_stxMenu->addAction(fActions[kTextStxSDL1]);
    text_stxMenu->addAction(fActions[kTextStxSDL2]);
    text_stxMenu->addAction(fActions[kTextStxIni]);
    text_stxMenu->addAction(fActions[kTextStxConsole]);
    text_stxMenu->addAction(fActions[kTextStxXML]);
    text_stxMenu->addAction(fActions[kTextStxHex]);
    text_stxMenu->addAction(fActions[kTextStxFX]);
    QMenu* text_encMenu = textMenu->addMenu(tr("&Encryption"));
    text_encMenu->addAction(fActions[kTextEncNone]);
    text_encMenu->addAction(fActions[kTextEncXtea]);
    text_encMenu->addAction(fActions[kTextEncAes]);
    text_encMenu->addAction(fActions[kTextEncDroid]);
    QMenu* text_typeMenu = textMenu->addMenu(tr("&File Encoding"));
    text_typeMenu->addAction(fActions[kTextTypeAnsi]);
    text_typeMenu->addAction(fActions[kTextTypeUTF8]);
    text_typeMenu->addAction(fActions[kTextTypeUTF16]);
    text_typeMenu->addAction(fActions[kTextTypeUTF32]);
    textMenu->addSeparator();
    textMenu->addAction(fActions[kTextExpandAll]);
    textMenu->addAction(fActions[kTextCollapseAll]);

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
    fileTbar->addAction(fActions[kFileNew]);
    fileTbar->addAction(fActions[kFileOpen]);
    fileTbar->addAction(fActions[kFileSave]);
    statusBar();

    // Tabbed Editor Pane for open documents
    fEditorPane = new QTabWidget(this);
    fEditorPane->setMovable(true);
    fEditorPane->setTabsClosable(true);
    setCentralWidget(fEditorPane);

    // Object Browser
    fBrowserDock = new QDockWidget(tr("File Browser"), this);
    fBrowserDock->setObjectName("BrowserDock");
    fBrowserTree = new QTreeView(fBrowserDock);
    fBrowserDock->setWidget(fBrowserTree);
    fBrowserDock->setAllowedAreas(Qt::LeftDockWidgetArea |
                                  Qt::RightDockWidgetArea);
    fBrowserDock->setFeatures(QDockWidget::DockWidgetMovable |
                              QDockWidget::DockWidgetFloatable);
    fBrowserTree->setUniformRowHeights(true);
    //fBrowserTree->setHeaderHidden(true);
    fBrowserTree->setContextMenuPolicy(Qt::CustomContextMenu);
    addDockWidget(Qt::LeftDockWidgetArea, fBrowserDock);

    // Load UI Settings
    QSettings settings("PlasmaShop", "PlasmaShop");
    resize(settings.value("WinSize", QSize(800, 600)).toSize());
    if (settings.contains("WinPos"))
        move(settings.value("WinPos").toPoint());
    if (settings.value("WinMaximized", false).toBool())
        showMaximized();

    if (settings.contains("WinState"))
        restoreState(settings.value("WinState").toByteArray());

    if (settings.contains("DialogDir"))
        fDialogDir = settings.value("DialogDir").toString();
}

void PlasmaShopMain::closeEvent(QCloseEvent*)
{
    // Save UI Settings
    QSettings settings("PlasmaShop", "PlasmaShop");
    settings.setValue("WinMaximized", (windowState() & Qt::WindowMaximized) != 0);
    showNormal();
    settings.setValue("WinSize", size());
    settings.setValue("WinPos", pos());
    settings.setValue("WinState", saveState());

    settings.setValue("DialogDir", fDialogDir);
}

int main(int argc, char* argv[])
{
    // Redirect libPlasma's debug stuff to PlasmaShop.log
    plDebug::InitFile(plDebug::kDLAll, "PlasmaShop.log");

    QApplication app(argc, argv);
    PlasmaShopMain mainWnd;
    mainWnd.show();
    //for (int i=1; i<argc; i++)
    //    mainWnd.loadFile(argv[i]);
    return app.exec();
}
