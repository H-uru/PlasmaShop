#include <QApplication>
#include <QSettings>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include <qticonloader.h>
#include <Debug/plDebug.h>

#include "Main.h"
#include "../QPlasma.h"

PlasmaShopMain::PlasmaShopMain()
{
    // Basic Form Settings
    setWindowTitle("PlasmaShop " PLASMASHOP_VERSION);
    setWindowIcon(QIcon(":/res/PlasmaShop.png"));
    setDockOptions(QMainWindow::AnimatedDocks);

    // Set up actions
    fActions[kFileNew] = new QAction(qStdIcon("document-new"), tr("&New..."), this);
    fActions[kFileOpen] = new QAction(qStdIcon("document-open"), tr("&Open..."), this);
    fActions[kFileSave] = new QAction(qStdIcon("document-save"), tr("&Save"), this);
    fActions[kFileSaveAs] = new QAction(qStdIcon("document-save-as"), tr("Sa&ve As..."), this);
    fActions[kFileOptions] = new QAction(tr("&Options..."), this);
    fActions[kFileExit] = new QAction(qStdIcon("application-exit"), tr("E&xit"), this);
    fActions[kEditUndo] = new QAction(qStdIcon("edit-undo"), tr("&Undo"), this);
    fActions[kEditRedo] = new QAction(qStdIcon("edit-redo"), tr("&Redo"), this);
    fActions[kEditCut] = new QAction(qStdIcon("edit-cut"), tr("Cu&t"), this);
    fActions[kEditCopy] = new QAction(qStdIcon("edit-copy"), tr("&Copy"), this);
    fActions[kEditPaste] = new QAction(qStdIcon("edit-paste"), tr("&Paste"), this);
    fActions[kEditRevert] = new QAction(qStdIcon("document-revert"), tr("Re&vert"), this);
    fActions[kHelpAbout] = new QAction(qStdIcon("help-about"), tr("&About PlasmaShop"), this);

    fActions[kTextFind] = new QAction(qStdIcon("edit-find"), tr("&Find..."), this);
    fActions[kTextFindNext] = new QAction(tr("Find &Next"), this);
    fActions[kTextFindPrev] = new QAction(tr("Find &Previous"), this);
    fActions[kTextReplace] = new QAction(qStdIcon("edit-find-replace"), tr("&Replace..."), this);
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

    fActions[kFileNew]->setShortcut(Qt::CTRL + Qt::Key_N);
    fActions[kFileOpen]->setShortcut(Qt::CTRL + Qt::Key_O);
    fActions[kFileSave]->setShortcut(Qt::CTRL + Qt::Key_S);
    fActions[kFileSaveAs]->setShortcut(Qt::SHIFT + Qt::CTRL + Qt::Key_S);
    fActions[kFileExit]->setShortcut(Qt::ALT + Qt::Key_F4);
    fActions[kEditUndo]->setShortcut(Qt::CTRL + Qt::Key_Z);
    fActions[kEditRedo]->setShortcut(Qt::SHIFT + Qt::CTRL + Qt::Key_Z);
    fActions[kEditCut]->setShortcut(Qt::CTRL + Qt::Key_X);
    fActions[kEditCopy]->setShortcut(Qt::CTRL + Qt::Key_C);
    fActions[kEditPaste]->setShortcut(Qt::CTRL + Qt::Key_V);
    fActions[kTextFind]->setShortcut(Qt::CTRL + Qt::Key_F);
    fActions[kTextFindNext]->setShortcut(Qt::Key_F3);
    fActions[kTextFindPrev]->setShortcut(Qt::SHIFT + Qt::Key_F3);
    fActions[kTextReplace]->setShortcut(Qt::CTRL + Qt::Key_R);

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

    QMenu* editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(fActions[kEditUndo]);
    editMenu->addAction(fActions[kEditRedo]);
    editMenu->addSeparator();
    editMenu->addAction(fActions[kEditCut]);
    editMenu->addAction(fActions[kEditCopy]);
    editMenu->addAction(fActions[kEditPaste]);
    editMenu->addSeparator();
    editMenu->addAction(fActions[kEditRevert]);

    // Editor-specific menus
    QMenu* textMenu = menuBar()->addMenu(tr("&Text"));
    textMenu->addAction(fActions[kTextFind]);
    textMenu->addAction(fActions[kTextFindNext]);
    textMenu->addAction(fActions[kTextFindPrev]);
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

    // Help menu is always at the end
    QMenu* helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(fActions[kHelpAbout]);

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

    // Signals
    connect(fActions[kFileOpen], SIGNAL(triggered()), this, SLOT(onOpenFile()));
    connect(fActions[kFileSave], SIGNAL(triggered()), this, SLOT(onSaveFile()));
    connect(fActions[kFileSaveAs], SIGNAL(triggered()), this, SLOT(onSaveAs()));
    connect(fActions[kFileExit], SIGNAL(triggered()), this, SLOT(close()));

    connect(fEditorPane, SIGNAL(tabCloseRequested(int)),
            this, SLOT(onCloseTab(int)));
}

void PlasmaShopMain::loadFile(QString filename)
{
    // Guess the filetype based on its extension
    QString ext, fnameNoPath, fnameDisplay = filename;
    QRegExp re;
    re.setPattern(".*\\.([^.]*)");
    if (re.indexIn(filename) >= 0)
        ext = re.cap(1).toLower();
    re.setPattern(".*[\\/]([^\\/]*)");
    if (re.indexIn(filename) >= 0) {
        fnameNoPath = re.cap(1).toLower();
        fnameDisplay = re.cap(1);
    }

    DocumentType dtype = kDocUnknown;
    if (ext == "age" || ext == "cfg" || ext == "csv" || ext == "elf" ||
        ext == "fni" || ext == "fx"  || ext == "hex" || ext == "ini" ||
        ext == "loc" || ext == "log" || ext == "mfs" || ext == "py"  ||
        ext == "sdl" || ext == "sub" || ext == "txt" || ext == "xml" ||
        ext == "mfold" || ext == "mlist") {
        dtype = kDocText;
    } else if (ext == "p2f") {
        dtype = kDocFont;
    } else if (ext == "pfp" || ext == "pak" || fnameNoPath == "cursors.dat") {
        dtype = kDocPackage;
    } else if (ext == "sum") {
        dtype = kDocManifest;
    } else if (ext == "prp") {
        // TODO: Launch PRP editor...
        return;
    } else if (ext == "pyc") {
        // TODO: Launch Decompyler...
    } else if (ext == "tga") {
        // TODO: Launch Image editor...
    } else if (fnameNoPath == "dev_mode.dat") {
        // TODO: Open Dev Mode editor
        return;
    } else {
        QMessageBox::critical(this, tr("Unsupported File Type"),
                              tr("Error: File %1 has an unsupported file type (%2)")
                              .arg(fnameNoPath).arg(ext),
                              QMessageBox::Ok);
        return;
    }

    // Create an appropriate editor and add it to the document panel
    QPlasmaDocument* plDoc = QPlasmaDocument::GetEditor(dtype, this);
    if (plDoc != NULL) {
        fEditorPane->addTab(plDoc, QPlasmaDocument::GetDocIcon(filename), fnameDisplay);
        plDoc->loadFile(filename);
    }
}

void PlasmaShopMain::closeEvent(QCloseEvent* evt)
{
    // Cycle through open documents and save work if necessary
    for (int i=0; i<fEditorPane->count(); i++) {
        QPlasmaDocument* doc = (QPlasmaDocument*)fEditorPane->widget(i);
        if (doc->isDirty()) {
            QMessageBox::StandardButton result =
                QMessageBox::question(this, tr("Save Changes?"),
                                      tr("You have unsaved changes in %1.\n"
                                         "Would you like to save before closing?")
                                      .arg(doc->filename()),
                                      QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
            if (result == QMessageBox::Yes)
                doc->saveDefault();
            else if (result == QMessageBox::Cancel)
                // Don't continue and don't close anything
                return;
        }
    }

    // Save UI Settings
    QSettings settings("PlasmaShop", "PlasmaShop");
    settings.setValue("WinMaximized", (windowState() & Qt::WindowMaximized) != 0);
    showNormal();
    settings.setValue("WinSize", size());
    settings.setValue("WinPos", pos());
    settings.setValue("WinState", saveState());

    settings.setValue("DialogDir", fDialogDir);
}

void PlasmaShopMain::onOpenFile()
{
    QStringList files = QFileDialog::getOpenFileNames(this,
                            tr("Open File(s)"), fDialogDir,
                            "All supported Plasma Files (*.age *.cfg *.ini *.fni *.hex *.loc *.sub *.log *.elf *.p2f *.pfp *.pak *.csv *.sdl *.sum *.fx Cursors.dat);;"
                            "Age Files (*.age);;"
                            "Config Files (*.cfg *.ini);;"
                            "Console Files (*.fni);;"
                            "Cursor Packages (Cursors.dat);;"
                            "Hex Isle Levels (*.hex);;"
                            "Localization Files (*.loc *.sub);;"
                            "Log Files (*.log *.elf);;"
                            "Manifest Files (*.sum);;"
                            "Plasma Font Files (*.p2f);;"
                            "Plasma Font Packages (*.pfp);;"
                            "Python Packages (*.pak);;"
                            "Relevance Map Files (*.csv);;"
                            "SDL Files (*.sdl);;"
                            "Shader Files (*.fx);;"
                            "All Files (*)");
    QStringList filesIt = files;
    for (QStringList::Iterator it = filesIt.begin(); it != filesIt.end(); it++) {
        loadFile(*it);
        QDir dir = QDir(*it);
        dir.cdUp();
        fDialogDir = dir.absolutePath();
    }
    // Select the last opened file's tab
    if (files.count() > 0)
        fEditorPane->setCurrentIndex(fEditorPane->count() - 1);
}

void PlasmaShopMain::onSaveFile()
{
    if (fEditorPane->currentIndex() < 0)
        return;
    QPlasmaDocument* doc = (QPlasmaDocument*)fEditorPane->currentWidget();
    doc->saveDefault();
}

void PlasmaShopMain::onSaveAs()
{
    if (fEditorPane->currentIndex() < 0)
        return;
    QPlasmaDocument* doc = (QPlasmaDocument*)fEditorPane->currentWidget();

    QString ext, fnameNoPath;
    QRegExp re;
    re.setPattern(".*\\.([^.]*)");
    if (re.indexIn(doc->filename()) >= 0)
        ext = re.cap(1).toLower();
    re.setPattern(".*[\\/]([^\\/]*)");
    if (re.indexIn(doc->filename()) >= 0)
        fnameNoPath = re.cap(1).toLower();

    QString typeList;
    QString curFilter;
    if (doc->docType() == kDocText) {
        typeList = "Age Files (*.age);;"
                   "Config Files (*.cfg);;"
                   "Console Files (*.fni);;"
                   "Hex Isle Levels (*.hex);;"
                   "Ini Files (*.ini);;"
                   "Localization Files (*.loc);;"
                   "Log Files (*.log);;"
                   "Relevance Map Files (*.csv);;"
                   "SDL Files (*.sdl);;"
                   "Shader Files (*.fx);;"
                   "Subtitle Files (*.sub);;"
                   "Text Files (*.txt);;"
                   "All Files (*)";
        if (ext == "age")
            curFilter = "Age Files (*.age)";
        else if (ext == "cfg")
            curFilter = "Config Files (*.cfg)";
        else if (ext == "fni")
            curFilter = "Console Files (*.fni)";
        else if (ext == "hex")
            curFilter = "Hex Isle Levels (*.hex)";
        else if (ext == "ini")
            curFilter = "Ini Files (*.ini)";
        else if (ext == "loc")
            curFilter = "Localization Files (*.loc)";
        else if (ext == "log" || ext == "elf")
            curFilter = "Log Files (*.log)";
        else if (ext == "csv")
            curFilter = "Relevance Map Files (*.csv)";
        else if (ext == "sdl")
            curFilter = "SDL Files (*.sdl)";
        else if (ext == "fx")
            curFilter = "Shader Files (*.fx)";
        else if (ext == "sub")
            curFilter = "Subtitle Files (*.sub)";
        else
            curFilter = "Text Files (*.txt)";
    } else if (doc->docType() == kDocFont) {
        typeList = "Plasma Font Files (*.p2f);;"
                   "All Files (*)";
        curFilter = "Plasma Font Files (*.p2f)";
    } else if (doc->docType() == kDocManifest) {
        typeList = "Manifest Files (*.sum);;"
                   "All Files (*)";
        curFilter = "Manifest Files (*.sum)";
    } else if (doc->docType() == kDocPackage) {
        typeList = "Cursor Packages (Cursors.dat);;"
                   "Plasma Font Packages (*.pfp);;"
                   "Python Packages (*.pak);;"
                   "All Files (*)";
        if (fnameNoPath == "cursors.dat")
            curFilter = "Cursor Packages (Cursors.dat)";
        else if (ext == "pfp")
            curFilter = "Plasma Font Packages (*.pfp)";
        else
            curFilter = "Python Packages (*.pak)";
    }
    QString filename = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                    doc->filename(),
                                                    typeList, &curFilter);
    if (!filename.isEmpty()) {
        doc->saveTo(filename);
        QDir dir = QDir(filename);
        dir.cdUp();
        fDialogDir = dir.absolutePath();
    }
}

void PlasmaShopMain::onCloseTab(int idx)
{
    QPlasmaDocument* doc = (QPlasmaDocument*)fEditorPane->widget(idx);
    if (doc->isDirty()) {
        QMessageBox::StandardButton result =
            QMessageBox::question(this, tr("Save Changes?"),
                                  tr("You have unsaved changes in %1.\n"
                                     "Would you like to save before closing?")
                                  .arg(doc->filename()),
                                  QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if (result == QMessageBox::Yes)
            doc->saveDefault();
        else if (result == QMessageBox::Cancel)
            // Don't continue and don't close anything
            return;
    }
    fEditorPane->removeTab(idx);
    delete doc;
}


int main(int argc, char* argv[])
{
    // Redirect libPlasma's debug stuff to PlasmaShop.log
    plDebug::InitFile(plDebug::kDLAll, "PlasmaShop.log");

    QApplication app(argc, argv);
    PlasmaShopMain mainWnd;
    mainWnd.show();
    for (int i=1; i<argc; i++)
        mainWnd.loadFile(argv[i]);
    return app.exec();
}
