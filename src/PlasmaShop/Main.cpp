#include <QApplication>
#include <QSettings>
#include <QMenuBar>
#include <QToolBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include <QProcess>
#include <QLabel>
#include <qticonloader.h>
#include <Debug/plDebug.h>

#include "Main.h"
#include "../QPlasma.h"
#include "OptionsDialog.h"
#include "QPlasmaTextDoc.h"

static QString s_textMenuTitle;

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
    fActions[kFileRevert] = new QAction(qStdIcon("document-revert"), tr("Re&vert"), this);
    fActions[kFileOptions] = new QAction(tr("&Options..."), this);
    fActions[kFileExit] = new QAction(qStdIcon("application-exit"), tr("E&xit"), this);
    fActions[kEditUndo] = new QAction(qStdIcon("edit-undo"), tr("&Undo"), this);
    fActions[kEditRedo] = new QAction(qStdIcon("edit-redo"), tr("&Redo"), this);
    fActions[kEditCut] = new QAction(qStdIcon("edit-cut"), tr("Cu&t"), this);
    fActions[kEditCopy] = new QAction(qStdIcon("edit-copy"), tr("&Copy"), this);
    fActions[kEditPaste] = new QAction(qStdIcon("edit-paste"), tr("&Paste"), this);
    fActions[kEditDelete] = new QAction(qStdIcon("edit-delete"), tr("&Delete"), this);
    fActions[kEditSelectAll] = new QAction(qStdIcon("edit-select-all"), tr("Select &All"), this);
    fActions[kHelpAbout] = new QAction(qStdIcon("help-about"), tr("&About PlasmaShop"), this);

    fActions[kTextFind] = new QAction(qStdIcon("edit-find"), tr("&Find..."), this);
    fActions[kTextFindNext] = new QAction(tr("Find &Next"), this);
    fActions[kTextFindPrev] = new QAction(tr("Find &Previous"), this);
    fActions[kTextReplace] = new QAction(qStdIcon("edit-find-replace"), tr("&Replace..."), this);
    fActions[kTextStxNone] = new QAction(tr("&None"), this);
    fActions[kTextStxPython] = new QAction(tr("&Python"), this);
    fActions[kTextStxSDL] = new QAction(tr("&SDL"), this);
    fActions[kTextStxIni] = new QAction(tr("&Age / Ini"), this);
    fActions[kTextStxConsole] = new QAction(tr("&Console"), this);
    fActions[kTextStxXML] = new QAction(tr("&XML"), this);
    fActions[kTextStxHex] = new QAction(tr("&Hex Isle Level"), this);
    fActions[kTextStxFX] = new QAction(tr("Shader &FX"), this);
    fActions[kTextEncNone] = new QAction(tr("&None"), this);
    fActions[kTextEncXtea] = new QAction(tr("&Uru Prime / PotS / UU"), this);
    fActions[kTextEncAes] = new QAction(tr("Myst &5 / Crowthistle / Hex Isle"), this);
    fActions[kTextEncDroid] = new QAction(tr("&MOUL / MQO (Requires Key)"), this);
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
    fActions[kEditDelete]->setShortcut(Qt::Key_Delete);
    fActions[kEditSelectAll]->setShortcut(Qt::CTRL + Qt::Key_A);
    fActions[kTextFind]->setShortcut(Qt::CTRL + Qt::Key_F);
    fActions[kTextFindNext]->setShortcut(Qt::Key_F3);
    fActions[kTextFindPrev]->setShortcut(Qt::SHIFT + Qt::Key_F3);
    fActions[kTextReplace]->setShortcut(Qt::CTRL + Qt::Key_R);

    fActions[kTextStxNone]->setCheckable(true);
    fActions[kTextStxPython]->setCheckable(true);
    fActions[kTextStxSDL]->setCheckable(true);
    fActions[kTextStxIni]->setCheckable(true);
    fActions[kTextStxConsole]->setCheckable(true);
    fActions[kTextStxXML]->setCheckable(true);
    fActions[kTextStxHex]->setCheckable(true);
    fActions[kTextStxFX]->setCheckable(true);
    fActions[kTextEncNone]->setCheckable(true);
    fActions[kTextEncXtea]->setCheckable(true);
    fActions[kTextEncAes]->setCheckable(true);
    fActions[kTextEncDroid]->setCheckable(true);
    fActions[kTextTypeAnsi]->setCheckable(true);
    fActions[kTextTypeUTF8]->setCheckable(true);
    fActions[kTextTypeUTF16]->setCheckable(true);
    fActions[kTextTypeUTF32]->setCheckable(true);

    // Main Menus
    QMenu* fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(fActions[kFileNew]);
    fileMenu->addSeparator();
    fileMenu->addAction(fActions[kFileOpen]);
    fileMenu->addAction(fActions[kFileSave]);
    fileMenu->addAction(fActions[kFileSaveAs]);
    fileMenu->addAction(fActions[kFileRevert]);
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
    editMenu->addAction(fActions[kEditDelete]);
    editMenu->addSeparator();
    editMenu->addAction(fActions[kEditSelectAll]);

    // Editor-specific menus
    s_textMenuTitle = tr("&Text");
    fTextMenu = menuBar()->addMenu(s_textMenuTitle);
    fTextMenu->addAction(fActions[kTextFind]);
    fTextMenu->addAction(fActions[kTextFindNext]);
    fTextMenu->addAction(fActions[kTextFindPrev]);
    fTextMenu->addAction(fActions[kTextReplace]);
    fTextMenu->addSeparator();
    QMenu* text_stxMenu = fTextMenu->addMenu(tr("&Syntax"));
    text_stxMenu->addAction(fActions[kTextStxNone]);
    text_stxMenu->addAction(fActions[kTextStxPython]);
    text_stxMenu->addAction(fActions[kTextStxSDL]);
    text_stxMenu->addAction(fActions[kTextStxIni]);
    text_stxMenu->addAction(fActions[kTextStxConsole]);
    text_stxMenu->addAction(fActions[kTextStxXML]);
    text_stxMenu->addAction(fActions[kTextStxHex]);
    text_stxMenu->addAction(fActions[kTextStxFX]);
    QMenu* text_encMenu = fTextMenu->addMenu(tr("&Encryption"));
    text_encMenu->addAction(fActions[kTextEncNone]);
    text_encMenu->addAction(fActions[kTextEncXtea]);
    text_encMenu->addAction(fActions[kTextEncAes]);
    text_encMenu->addAction(fActions[kTextEncDroid]);
    QMenu* text_typeMenu = fTextMenu->addMenu(tr("&File Encoding"));
    text_typeMenu->addAction(fActions[kTextTypeAnsi]);
    text_typeMenu->addAction(fActions[kTextTypeUTF8]);
    text_typeMenu->addAction(fActions[kTextTypeUTF16]);
    text_typeMenu->addAction(fActions[kTextTypeUTF32]);
    fTextMenu->addSeparator();
    fTextMenu->addAction(fActions[kTextExpandAll]);
    fTextMenu->addAction(fActions[kTextCollapseAll]);

    // Help menu is always at the end
    QMenu* helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(fActions[kHelpAbout]);

    // Toolbars
    QToolBar* mainTbar = addToolBar(tr("Main Toolbar"));
    mainTbar->setObjectName("MainToolBar");
    mainTbar->addAction(fActions[kFileNew]);
    mainTbar->addAction(fActions[kFileOpen]);
    mainTbar->addAction(fActions[kFileSave]);
    mainTbar->addSeparator();
    mainTbar->addAction(fActions[kEditCut]);
    mainTbar->addAction(fActions[kEditCopy]);
    mainTbar->addAction(fActions[kEditPaste]);
    mainTbar->addSeparator();
    mainTbar->addAction(fActions[kEditUndo]);
    mainTbar->addAction(fActions[kEditRedo]);
    fGameSelector = new QComboBox(mainTbar);
    mainTbar->addSeparator();
    mainTbar->addWidget(new QLabel(tr("Game: "), this));
    mainTbar->addWidget(fGameSelector);
    statusBar();

    // Tabbed Editor Pane for open documents
    fEditorPane = new QTabWidget(this);
    fEditorPane->setMovable(true);
    fEditorPane->setTabsClosable(true);
    setCentralWidget(fEditorPane);

    // Object Browser
    fBrowserDock = new QDockWidget(tr("File Browser"), this);
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
    connect(fActions[kFileRevert], SIGNAL(triggered()), this, SLOT(onRevert()));
    connect(fActions[kFileOptions], SIGNAL(triggered()), this, SLOT(onOptions()));
    connect(fActions[kFileExit], SIGNAL(triggered()), this, SLOT(close()));

    connect(fActions[kEditCut], SIGNAL(triggered()), this, SLOT(onCut()));
    connect(fActions[kEditCopy], SIGNAL(triggered()), this, SLOT(onCopy()));
    connect(fActions[kEditPaste], SIGNAL(triggered()), this, SLOT(onPaste()));
    connect(fActions[kEditDelete], SIGNAL(triggered()), this, SLOT(onDelete()));
    connect(fActions[kEditSelectAll], SIGNAL(triggered()), this, SLOT(onSelectAll()));
    connect(fActions[kEditUndo], SIGNAL(triggered()), this, SLOT(onUndo()));
    connect(fActions[kEditRedo], SIGNAL(triggered()), this, SLOT(onRedo()));

    connect(fActions[kTextCollapseAll], SIGNAL(triggered()), this, SLOT(onTextCollapseAll()));
    connect(fActions[kTextExpandAll], SIGNAL(triggered()), this, SLOT(onTextExpandAll()));
    connect(fActions[kTextStxNone], SIGNAL(triggered()), this, SLOT(onTextStxNone()));
    connect(fActions[kTextStxPython], SIGNAL(triggered()), this, SLOT(onTextStxPython()));
    connect(fActions[kTextStxSDL], SIGNAL(triggered()), this, SLOT(onTextStxSDL()));
    connect(fActions[kTextStxIni], SIGNAL(triggered()), this, SLOT(onTextStxIni()));
    connect(fActions[kTextStxConsole], SIGNAL(triggered()), this, SLOT(onTextStxConsole()));
    connect(fActions[kTextStxXML], SIGNAL(triggered()), this, SLOT(onTextStxXML()));
    connect(fActions[kTextStxHex], SIGNAL(triggered()), this, SLOT(onTextStxHex()));
    connect(fActions[kTextStxFX], SIGNAL(triggered()), this, SLOT(onTextStxFX()));
    connect(fActions[kTextEncNone], SIGNAL(triggered()), this, SLOT(onTextEncNone()));
    connect(fActions[kTextEncXtea], SIGNAL(triggered()), this, SLOT(onTextEncXtea()));
    connect(fActions[kTextEncAes], SIGNAL(triggered()), this, SLOT(onTextEncAes()));
    connect(fActions[kTextEncDroid], SIGNAL(triggered()), this, SLOT(onTextEncDroid()));
    connect(fActions[kTextTypeAnsi], SIGNAL(triggered()), this, SLOT(onTextTypeAnsi()));
    connect(fActions[kTextTypeUTF8], SIGNAL(triggered()), this, SLOT(onTextTypeUTF8()));
    connect(fActions[kTextTypeUTF16], SIGNAL(triggered()), this, SLOT(onTextTypeUTF16()));
    connect(fActions[kTextTypeUTF32], SIGNAL(triggered()), this, SLOT(onTextTypeUTF32()));

    connect(fGameSelector, SIGNAL(activated(int)), this, SLOT(onSelectGame(int)));
    connect(fBrowserTree, SIGNAL(itemActivated(QTreeWidgetItem*, int)),
            this, SLOT(onBrowserItemActivated(QTreeWidgetItem*, int)));
    connect(fEditorPane, SIGNAL(tabCloseRequested(int)), this, SLOT(onCloseTab(int)));
    connect(fEditorPane, SIGNAL(currentChanged(int)), this, SLOT(onChangeTab(int)));

    // Set up menus, etc
    onChangeTab(-1);
    populateGameList();
    onSelectGame(fGameSelector->currentIndex());
}

void PlasmaShopMain::loadFile(QString filename)
{
    // Guess the filetype based on its extension
    QString ext, fnameNoPath, fnameDisplay = filename;
    QRegExp re;
    re.setPattern(".*\\.([^\\.]*)");
    if (re.indexIn(filename) >= 0)
        ext = re.cap(1).toLower();
    re.setPattern("(.*[\\\\\\/])?([^\\\\\\/]*)");
    if (re.indexIn(filename) >= 0) {
        fnameNoPath = re.cap(2).toLower();
        fnameDisplay = re.cap(2);
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
        QSettings settings("PlasmaShop", "PlasmaShop");
        QString editor = settings.value("PrpEditorPath", DEFAULT_PRP_EDITOR).toString();
        QProcess proc;
        proc.startDetached(GetPSBinPath(editor), QStringList(filename));
        return;
    } else if (ext == "pyc") {
        // TODO: Launch Decompyler...
    } else if (ext == "tga" || ext == "jpg") {
        QSettings settings("PlasmaShop", "PlasmaShop");
        QString editor = settings.value("ImageEditorPath", "").toString();
        if (!editor.isEmpty()) {
            QProcess proc;
            proc.startDetached(GetPSBinPath(editor), QStringList(filename));
        } else {
            QMessageBox::information(this, tr("No Image Editor set"),
                                     tr("Error: You have not set any image editor yet.  "
                                        "Please set one in the PlasmaShop Options to edit this file."),
                                     QMessageBox::Ok);
        }
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
        if (!plDoc->loadFile(filename)) {
            // Error loading the file.  Remove it now to avoid problems later
            fEditorPane->removeTab(fEditorPane->count() - 1);
            delete plDoc;
        } else if (dtype == kDocText) {
            if (ext == "fni" || ext == "cfg")
                ((QPlasmaTextDoc*)plDoc)->setSyntax(QPlasmaTextDoc::kStxConsole);
            else if (ext == "fx")
                ((QPlasmaTextDoc*)plDoc)->setSyntax(QPlasmaTextDoc::kStxFX);
            else if (ext == "hex")
                ((QPlasmaTextDoc*)plDoc)->setSyntax(QPlasmaTextDoc::kStxHex);
            else if (ext == "age")
                ((QPlasmaTextDoc*)plDoc)->setSyntax(QPlasmaTextDoc::kStxIni);
            else if (ext == "py")
                ((QPlasmaTextDoc*)plDoc)->setSyntax(QPlasmaTextDoc::kStxPython);
            else if (ext == "sdl")
                ((QPlasmaTextDoc*)plDoc)->setSyntax(QPlasmaTextDoc::kStxSDL);
            else if (ext == "loc" || ext == "sub" || ext == "xml")
                ((QPlasmaTextDoc*)plDoc)->setSyntax(QPlasmaTextDoc::kStxXML);
            else if (fnameNoPath == "serverconfig.ini")
                ((QPlasmaTextDoc*)plDoc)->setSyntax(QPlasmaTextDoc::kStxNone);
            else if (ext == "ini")
                ((QPlasmaTextDoc*)plDoc)->setSyntax(((QPlasmaTextDoc*)plDoc)->GuessIniType());
            else
                ((QPlasmaTextDoc*)plDoc)->setSyntax(QPlasmaTextDoc::kStxNone);
        }
        connect(plDoc, SIGNAL(statusChanged()), this, SLOT(updateMenuStatus()));
        connect(plDoc, SIGNAL(becameDirty()), this, SLOT(onDocDirty()));
        connect(plDoc, SIGNAL(becameClean()), this, SLOT(onDocClean()));

        // Update menus
        onChangeTab(fEditorPane->currentIndex());
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
            if (result == QMessageBox::Yes) {
                doc->saveDefault();
            } else if (result == QMessageBox::Cancel) {
                // Don't continue and don't close anything
                evt->ignore();
                return;
            }
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
                            "All supported Plasma Files (*.age *.cfg *.ini *.fni *.hex *.loc *.sub *.log *.elf *.p2f *.pfp *.pak *.py *.csv *.sdl *.sum *.fx Cursors.dat);;"
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
                            "Python Source Files (*.py);;"
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
    if (doc->filename() == "<NEW>")
        onSaveAs();
    else if (doc->isDirty())
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
                   "Python Source Files (*.py);;"
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
        else if (ext == "py")
            curFilter = "Python Source Files (*.py)";
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

        // Update the displayed filename for the file
        QString fnameDisplay = filename;
        QRegExp re;
        re.setPattern("(.*[\\\\\\/])?([^\\\\\\/]*)");
        if (re.indexIn(filename) >= 0)
            fnameDisplay = re.cap(2);
        fEditorPane->setTabText(fEditorPane->currentIndex(), fnameDisplay);
    }
}

void PlasmaShopMain::onRevert()
{
    if (fEditorPane->currentIndex() < 0)
        return;
    QPlasmaDocument* doc = (QPlasmaDocument*)fEditorPane->currentWidget();
    if (doc->isDirty()) {
        QMessageBox::StandardButton result =
            QMessageBox::question(this, tr("Are you sure?"),
                                  tr("You have unsaved changes in %1.\n"
                                     "Are you sure you want to revert to the last saved version?")
                                  .arg(doc->filename()),
                                  QMessageBox::Yes | QMessageBox::No);
        if (result == QMessageBox::Yes)
            doc->revert();
    }
}

void PlasmaShopMain::onOptions()
{
    OptionsDialog dlg(this);
    dlg.setModal(true);
    if (dlg.exec() == QDialog::Accepted) {
        for (int i=0; i<fEditorPane->count(); i++) {
            QPlasmaDocument* doc = (QPlasmaDocument*)fEditorPane->widget(i);
            doc->updateSettings();
        }
    }
}

void PlasmaShopMain::onCut()
{
    if (fEditorPane->currentIndex() < 0)
        return;
    QPlasmaDocument* doc = (QPlasmaDocument*)fEditorPane->currentWidget();
    doc->performCut();
}

void PlasmaShopMain::onCopy()
{
    if (fEditorPane->currentIndex() < 0)
        return;
    QPlasmaDocument* doc = (QPlasmaDocument*)fEditorPane->currentWidget();
    doc->performCopy();
}

void PlasmaShopMain::onPaste()
{
    if (fEditorPane->currentIndex() < 0)
        return;
    QPlasmaDocument* doc = (QPlasmaDocument*)fEditorPane->currentWidget();
    doc->performPaste();
}

void PlasmaShopMain::onDelete()
{
    if (fEditorPane->currentIndex() < 0)
        return;
    QPlasmaDocument* doc = (QPlasmaDocument*)fEditorPane->currentWidget();
    doc->performDelete();
}

void PlasmaShopMain::onSelectAll()
{
    if (fEditorPane->currentIndex() < 0)
        return;
    QPlasmaDocument* doc = (QPlasmaDocument*)fEditorPane->currentWidget();
    doc->performSelectAll();
}

void PlasmaShopMain::onUndo()
{
    if (fEditorPane->currentIndex() < 0)
        return;
    QPlasmaDocument* doc = (QPlasmaDocument*)fEditorPane->currentWidget();
    doc->performUndo();
}

void PlasmaShopMain::onRedo()
{
    if (fEditorPane->currentIndex() < 0)
        return;
    QPlasmaDocument* doc = (QPlasmaDocument*)fEditorPane->currentWidget();
    doc->performRedo();
}

void PlasmaShopMain::onTextExpandAll()
{
    if (fEditorPane->currentIndex() < 0)
        return;
    QPlasmaDocument* doc = (QPlasmaDocument*)fEditorPane->currentWidget();
    if (doc->docType() != kDocText)
        return;
    ((QPlasmaTextDoc*)doc)->expandAll();
}

void PlasmaShopMain::onTextCollapseAll()
{
    if (fEditorPane->currentIndex() < 0)
        return;
    QPlasmaDocument* doc = (QPlasmaDocument*)fEditorPane->currentWidget();
    if (doc->docType() != kDocText)
        return;
    ((QPlasmaTextDoc*)doc)->collapseAll();
}

void PlasmaShopMain::onTextStxNone()
{
    if (fEditorPane->currentIndex() < 0)
        return;
    QPlasmaDocument* doc = (QPlasmaDocument*)fEditorPane->currentWidget();
    if (doc->docType() != kDocText)
        return;
    ((QPlasmaTextDoc*)doc)->setSyntax(QPlasmaTextDoc::kStxNone);
    setTextSyntax(kTextStxNone);
}

void PlasmaShopMain::onTextStxPython()
{
    if (fEditorPane->currentIndex() < 0)
        return;
    QPlasmaDocument* doc = (QPlasmaDocument*)fEditorPane->currentWidget();
    if (doc->docType() != kDocText)
        return;
    ((QPlasmaTextDoc*)doc)->setSyntax(QPlasmaTextDoc::kStxPython);
    setTextSyntax(kTextStxPython);
}

void PlasmaShopMain::onTextStxSDL()
{
    if (fEditorPane->currentIndex() < 0)
        return;
    QPlasmaDocument* doc = (QPlasmaDocument*)fEditorPane->currentWidget();
    if (doc->docType() != kDocText)
        return;
    ((QPlasmaTextDoc*)doc)->setSyntax(QPlasmaTextDoc::kStxSDL);
    setTextSyntax(kTextStxSDL);
}

void PlasmaShopMain::onTextStxIni()
{
    if (fEditorPane->currentIndex() < 0)
        return;
    QPlasmaDocument* doc = (QPlasmaDocument*)fEditorPane->currentWidget();
    if (doc->docType() != kDocText)
        return;
    ((QPlasmaTextDoc*)doc)->setSyntax(QPlasmaTextDoc::kStxIni);
    setTextSyntax(kTextStxIni);
}

void PlasmaShopMain::onTextStxConsole()
{
    if (fEditorPane->currentIndex() < 0)
        return;
    QPlasmaDocument* doc = (QPlasmaDocument*)fEditorPane->currentWidget();
    if (doc->docType() != kDocText)
        return;
    ((QPlasmaTextDoc*)doc)->setSyntax(QPlasmaTextDoc::kStxConsole);
    setTextSyntax(kTextStxConsole);
}

void PlasmaShopMain::onTextStxXML()
{
    if (fEditorPane->currentIndex() < 0)
        return;
    QPlasmaDocument* doc = (QPlasmaDocument*)fEditorPane->currentWidget();
    if (doc->docType() != kDocText)
        return;
    ((QPlasmaTextDoc*)doc)->setSyntax(QPlasmaTextDoc::kStxXML);
    setTextSyntax(kTextStxXML);
}

void PlasmaShopMain::onTextStxHex()
{
    if (fEditorPane->currentIndex() < 0)
        return;
    QPlasmaDocument* doc = (QPlasmaDocument*)fEditorPane->currentWidget();
    if (doc->docType() != kDocText)
        return;
    ((QPlasmaTextDoc*)doc)->setSyntax(QPlasmaTextDoc::kStxHex);
    setTextSyntax(kTextStxHex);
}

void PlasmaShopMain::onTextStxFX()
{
    if (fEditorPane->currentIndex() < 0)
        return;
    QPlasmaDocument* doc = (QPlasmaDocument*)fEditorPane->currentWidget();
    if (doc->docType() != kDocText)
        return;
    ((QPlasmaTextDoc*)doc)->setSyntax(QPlasmaTextDoc::kStxFX);
    setTextSyntax(kTextStxFX);
}

void PlasmaShopMain::onTextEncNone()
{
    if (fEditorPane->currentIndex() < 0)
        return;
    QPlasmaDocument* doc = (QPlasmaDocument*)fEditorPane->currentWidget();
    if (doc->docType() != kDocText)
        return;
    ((QPlasmaTextDoc*)doc)->setEncryption(QPlasmaTextDoc::kEncNone);
    setTextEncryption(kTextEncNone);
}

void PlasmaShopMain::onTextEncXtea()
{
    if (fEditorPane->currentIndex() < 0)
        return;
    QPlasmaDocument* doc = (QPlasmaDocument*)fEditorPane->currentWidget();
    if (doc->docType() != kDocText)
        return;
    ((QPlasmaTextDoc*)doc)->setEncryption(QPlasmaTextDoc::kEncXtea);
    setTextEncryption(kTextEncXtea);
}

void PlasmaShopMain::onTextEncAes()
{
    if (fEditorPane->currentIndex() < 0)
        return;
    QPlasmaDocument* doc = (QPlasmaDocument*)fEditorPane->currentWidget();
    if (doc->docType() != kDocText)
        return;
    ((QPlasmaTextDoc*)doc)->setEncryption(QPlasmaTextDoc::kEncAes);
    setTextEncryption(kTextEncAes);
}

void PlasmaShopMain::onTextEncDroid()
{
    if (fEditorPane->currentIndex() < 0)
        return;
    QPlasmaDocument* doc = (QPlasmaDocument*)fEditorPane->currentWidget();
    if (doc->docType() != kDocText)
        return;
    ((QPlasmaTextDoc*)doc)->setEncryption(QPlasmaTextDoc::kEncDroid);
    setTextEncryption(kTextEncDroid);
}

void PlasmaShopMain::onTextTypeAnsi()
{
    if (fEditorPane->currentIndex() < 0)
        return;
    QPlasmaDocument* doc = (QPlasmaDocument*)fEditorPane->currentWidget();
    if (doc->docType() != kDocText)
        return;
    ((QPlasmaTextDoc*)doc)->setEncoding(QPlasmaTextDoc::kTypeAnsi);
    setTextEncoding(kTextTypeAnsi);
}

void PlasmaShopMain::onTextTypeUTF8()
{
    if (fEditorPane->currentIndex() < 0)
        return;
    QPlasmaDocument* doc = (QPlasmaDocument*)fEditorPane->currentWidget();
    if (doc->docType() != kDocText)
        return;
    ((QPlasmaTextDoc*)doc)->setEncoding(QPlasmaTextDoc::kTypeUTF8);
    setTextEncoding(kTextTypeUTF8);
}

void PlasmaShopMain::onTextTypeUTF16()
{
    if (fEditorPane->currentIndex() < 0)
        return;
    QPlasmaDocument* doc = (QPlasmaDocument*)fEditorPane->currentWidget();
    if (doc->docType() != kDocText)
        return;
    ((QPlasmaTextDoc*)doc)->setEncoding(QPlasmaTextDoc::kTypeUTF16);
    setTextEncoding(kTextTypeUTF16);
}

void PlasmaShopMain::onTextTypeUTF32()
{
    if (fEditorPane->currentIndex() < 0)
        return;
    QPlasmaDocument* doc = (QPlasmaDocument*)fEditorPane->currentWidget();
    if (doc->docType() != kDocText)
        return;
    ((QPlasmaTextDoc*)doc)->setEncoding(QPlasmaTextDoc::kTypeUTF32);
    setTextEncoding(kTextTypeUTF32);
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

void PlasmaShopMain::setTextSyntax(int sel)
{
    fActions[kTextStxNone]->setChecked(sel == kTextStxNone);
    fActions[kTextStxPython]->setChecked(sel == kTextStxPython);
    fActions[kTextStxSDL]->setChecked(sel == kTextStxSDL);
    fActions[kTextStxIni]->setChecked(sel == kTextStxIni);
    fActions[kTextStxConsole]->setChecked(sel == kTextStxConsole);
    fActions[kTextStxXML]->setChecked(sel == kTextStxXML);
    fActions[kTextStxHex]->setChecked(sel == kTextStxHex);
    fActions[kTextStxFX]->setChecked(sel == kTextStxFX);
}

void PlasmaShopMain::setTextEncryption(int sel)
{
    fActions[kTextEncNone]->setChecked(sel == kTextEncNone);
    fActions[kTextEncXtea]->setChecked(sel == kTextEncXtea);
    fActions[kTextEncAes]->setChecked(sel == kTextEncAes);
    fActions[kTextEncDroid]->setChecked(sel == kTextEncDroid);
}

void PlasmaShopMain::setTextEncoding(int sel)
{
    fActions[kTextTypeAnsi]->setChecked(sel == kTextTypeAnsi);
    fActions[kTextTypeUTF8]->setChecked(sel == kTextTypeUTF8);
    fActions[kTextTypeUTF16]->setChecked(sel == kTextTypeUTF16);
    fActions[kTextTypeUTF32]->setChecked(sel == kTextTypeUTF32);
}

void PlasmaShopMain::populateGameList()
{
    QSettings settings("PlasmaShop", "PlasmaShop");
    settings.beginGroup("Games");
    fGames.clear();
    fGameSelector->clear();
    fGameSelector->addItem(tr("(None)"), 0);
    QStringList gamesList = settings.childKeys();
    foreach (QString gm, gamesList) {
        QStringList gmParams = settings.value(gm).toStringList();
        GameInfo inf;
        inf.fGameTitle = gm;
        inf.fGamePath = gmParams[0];
        inf.fGameType = gmParams[1].toInt();
        fGames.append(inf);
        fGameSelector->addItem(GameInfo::GetGameIcon(inf.fGameType),
                               inf.fGameTitle, fGames.count());
    }
    fGameSelector->insertSeparator(fGameSelector->count());
    fGameSelector->addItem(tr("Edit Games List..."), -1);
    settings.endGroup();

    QString curGame = settings.value("CurrentGame", QString()).toString();
    fCurrentGame = 0;
    if (!curGame.isEmpty()) {
        for (int i=0; i<fGames.size(); i++) {
            if (fGames[i].fGameTitle == curGame)
                fCurrentGame = (i + 1);
        }
    }
    fGameSelector->setCurrentIndex(fCurrentGame);
}

void PlasmaShopMain::onChangeTab(int idx)
{
    // Reset menu states
    fTextMenu->setTitle(QString());

    fActions[kTextFind]->setEnabled(false);
    fActions[kTextFindNext]->setEnabled(false);
    fActions[kTextFindPrev]->setEnabled(false);
    fActions[kTextReplace]->setEnabled(false);
    fActions[kTextExpandAll]->setEnabled(false);
    fActions[kTextCollapseAll]->setEnabled(false);
    fActions[kTextReplace]->setEnabled(false);
    fActions[kTextStxNone]->setEnabled(false);
    fActions[kTextStxPython]->setEnabled(false);
    fActions[kTextStxSDL]->setEnabled(false);
    fActions[kTextStxIni]->setEnabled(false);
    fActions[kTextStxConsole]->setEnabled(false);
    fActions[kTextStxXML]->setEnabled(false);
    fActions[kTextStxHex]->setEnabled(false);
    fActions[kTextStxFX]->setEnabled(false);
    fActions[kTextEncNone]->setEnabled(false);
    fActions[kTextEncXtea]->setEnabled(false);
    fActions[kTextEncAes]->setEnabled(false);
    fActions[kTextEncDroid]->setEnabled(false);
    fActions[kTextTypeAnsi]->setEnabled(false);
    fActions[kTextTypeUTF8]->setEnabled(false);
    fActions[kTextTypeUTF16]->setEnabled(false);
    fActions[kTextTypeUTF32]->setEnabled(false);
    setTextSyntax(kTextStxNone);
    setTextEncryption(kTextEncNone);
    setTextEncoding(kTextTypeAnsi);

    if (idx < 0) {
        fActions[kFileSave]->setEnabled(false);
        fActions[kFileSaveAs]->setEnabled(false);
        fActions[kFileRevert]->setEnabled(false);
        fActions[kEditCut]->setEnabled(false);
        fActions[kEditCopy]->setEnabled(false);
        fActions[kEditPaste]->setEnabled(false);
        fActions[kEditDelete]->setEnabled(false);
        fActions[kEditSelectAll]->setEnabled(false);
        fActions[kEditUndo]->setEnabled(false);
        fActions[kEditRedo]->setEnabled(false);
        return;
    }
    fActions[kFileSave]->setEnabled(true);
    fActions[kFileSaveAs]->setEnabled(true);
    fActions[kFileRevert]->setEnabled(true);

    QPlasmaDocument* doc = (QPlasmaDocument*)fEditorPane->widget(idx);
    if (doc->docType() == kDocText) {
        fTextMenu->setTitle(s_textMenuTitle);
        fActions[kTextFind]->setEnabled(true);
        fActions[kTextFindNext]->setEnabled(true);
        fActions[kTextFindPrev]->setEnabled(true);
        fActions[kTextReplace]->setEnabled(true);
        fActions[kTextExpandAll]->setEnabled(true);
        fActions[kTextCollapseAll]->setEnabled(true);
        fActions[kTextReplace]->setEnabled(true);
        fActions[kTextStxNone]->setEnabled(true);
        fActions[kTextStxPython]->setEnabled(true);
        fActions[kTextStxSDL]->setEnabled(true);
        fActions[kTextStxIni]->setEnabled(true);
        fActions[kTextStxConsole]->setEnabled(true);
        fActions[kTextStxXML]->setEnabled(true);
        fActions[kTextStxHex]->setEnabled(true);
        fActions[kTextStxFX]->setEnabled(true);
        fActions[kTextEncNone]->setEnabled(true);
        fActions[kTextEncXtea]->setEnabled(true);
        fActions[kTextEncAes]->setEnabled(true);
        fActions[kTextEncDroid]->setEnabled(true);
        fActions[kTextTypeAnsi]->setEnabled(true);
        fActions[kTextTypeUTF8]->setEnabled(true);
        fActions[kTextTypeUTF16]->setEnabled(true);
        fActions[kTextTypeUTF32]->setEnabled(true);

        QPlasmaTextDoc::SyntaxMode syn = ((QPlasmaTextDoc*)doc)->syntax();
        QPlasmaTextDoc::EncryptionMode enc = ((QPlasmaTextDoc*)doc)->encryption();
        QPlasmaTextDoc::EncodingMode type = ((QPlasmaTextDoc*)doc)->encoding();

        switch (syn) {
        case QPlasmaTextDoc::kStxConsole:
            setTextSyntax(kTextStxConsole);
            break;
        case QPlasmaTextDoc::kStxFX:
            setTextSyntax(kTextStxFX);
            break;
        case QPlasmaTextDoc::kStxHex:
            setTextSyntax(kTextStxHex);
            break;
        case QPlasmaTextDoc::kStxIni:
            setTextSyntax(kTextStxIni);
            break;
        case QPlasmaTextDoc::kStxNone:
            setTextSyntax(kTextStxNone);
            break;
        case QPlasmaTextDoc::kStxPython:
            setTextSyntax(kTextStxPython);
            break;
        case QPlasmaTextDoc::kStxSDL:
            setTextSyntax(kTextStxSDL);
            break;
        case QPlasmaTextDoc::kStxXML:
            setTextSyntax(kTextStxXML);
            break;
        }

        switch (enc) {
        case QPlasmaTextDoc::kEncAes:
            setTextEncryption(kTextEncAes);
            break;
        case QPlasmaTextDoc::kEncDroid:
            setTextEncryption(kTextEncDroid);
            break;
        case QPlasmaTextDoc::kEncNone:
            setTextEncryption(kTextEncNone);
            break;
        case QPlasmaTextDoc::kEncXtea:
            setTextEncryption(kTextEncXtea);
            break;
        }

        switch (type) {
        case QPlasmaTextDoc::kTypeAnsi:
            setTextEncoding(kTextTypeAnsi);
            break;
        case QPlasmaTextDoc::kTypeUTF8:
            setTextEncoding(kTextTypeUTF8);
            break;
        case QPlasmaTextDoc::kTypeUTF16:
            setTextEncoding(kTextTypeUTF16);
            break;
        case QPlasmaTextDoc::kTypeUTF32:
            setTextEncoding(kTextTypeUTF32);
            break;
        }
    }
}

static void RecursiveAddFiles(QTreeWidgetItem* parent, QString folderType,
                              QStringList fileFilter, QDir gameRoot)
{
    QStringList subdirs = gameRoot.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    foreach (QString dir, subdirs) {
        gameRoot.cd(dir);
        QTreeWidgetItem* folder = new QTreeWidgetItem(parent);
        folder->setIcon(0, QPlasmaDocument::GetDocIcon(folderType));
        folder->setText(0, dir);
        folder->setData(0, Qt::UserRole, folderType);
        RecursiveAddFiles(folder, folderType, fileFilter, gameRoot);
        if (folder->childCount() == 0)
            delete folder;
        gameRoot.cdUp();
    }

    QStringList files = gameRoot.entryList(fileFilter);
    foreach (QString f, files) {
        QTreeWidgetItem* item = new QTreeWidgetItem(parent);
        item->setIcon(0, QPlasmaDocument::GetDocIcon(f));
        item->setText(0, f);
        item->setData(0, Qt::UserRole, gameRoot.filePath(f));
    }
}

void PlasmaShopMain::onSelectGame(int gameId)
{
    int which = fGameSelector->itemData(gameId).toInt();
    if (which == -1) {
        GameListDialog dlg(this);
        dlg.exec();
        populateGameList();
    } else {
        // Save the selected value
        QSettings settings("PlasmaShop", "PlasmaShop");
        settings.setValue("CurrentGame", (fGameSelector->currentIndex() > 0)
                                         ? fGameSelector->currentText() : QString());
        fCurrentGame = which;
    }

    // Update the browser for the selected game
    fBrowserTree->clear();
    if (fCurrentGame <= 0)
        return;

    QRegExp re;
    int gameType = fGames[fCurrentGame-1].fGameType;
    QDir gameRoot(fGames[fCurrentGame-1].fGamePath);
    QDir appDataRoot(GetAppDataPath());
    QDir docRoot(GetDocumentsPath());
    gameRoot.setSorting(QDir::Name | QDir::IgnoreCase);
    gameRoot.setFilter(QDir::Files);

    if (gameRoot.cd("dat")) {
        // Ages; all game types
        {
            QTreeWidgetItem* fldr = new QTreeWidgetItem(fBrowserTree);
            fldr->setIcon(0, QPlasmaDocument::GetDocIcon("<AGEFOLD>"));
            fldr->setText(0, tr("Ages"));
            fldr->setData(0, Qt::UserRole, "<AGEFOLD>");
            QStringList files = gameRoot.entryList(QStringList("*.age"));
            foreach (QString f, files) {
                re.setPattern("(.*)\\.[^\\.]*");
                QString ageDisp = f;
                if (re.indexIn(f) >= 0)
                    ageDisp = re.cap(1);
                QTreeWidgetItem* age = new QTreeWidgetItem(fldr);
                age->setIcon(0, QPlasmaDocument::GetDocIcon("<AGE>"));
                age->setText(0, ageDisp);
                age->setData(0, Qt::UserRole, "<AGE>");

                QTreeWidgetItem* distFold = new QTreeWidgetItem(age);
                distFold->setIcon(0, QPlasmaDocument::GetDocIcon("<AGEFOLD>"));
                distFold->setText(0, tr("Districts"));
                distFold->setData(0, Qt::UserRole, "<AGEFOLD>");
                QStringList districts = gameRoot.entryList(QStringList(QString("%1_*.prp").arg(ageDisp)));
                foreach (QString d, districts) {
                    re.setPattern("_(District_)?(.*)\\.[^\\.]*");
                    QString pageDisp = d;
                    if (re.indexIn(d) >= 0)
                        pageDisp = re.cap(2);
                    QTreeWidgetItem* page = new QTreeWidgetItem(distFold);
                    page->setIcon(0, QPlasmaDocument::GetDocIcon(d));
                    page->setText(0, pageDisp);
                    page->setData(0, Qt::UserRole, gameRoot.filePath(d));
                }

                QTreeWidgetItem* ageFile = new QTreeWidgetItem(age);
                ageFile->setIcon(0, QPlasmaDocument::GetDocIcon(f));
                ageFile->setText(0, f);
                ageFile->setData(0, Qt::UserRole, gameRoot.filePath(f));

                QString csv = ageDisp + ".csv";
                if (gameRoot.exists(csv)) {
                    QTreeWidgetItem* csvFile = new QTreeWidgetItem(age);
                    csvFile->setIcon(0, QPlasmaDocument::GetDocIcon(csv));
                    csvFile->setText(0, csv);
                    csvFile->setData(0, Qt::UserRole, gameRoot.filePath(csv));
                }

                QString fni = ageDisp + ".fni";
                if (gameRoot.exists(fni)) {
                    QTreeWidgetItem* fniFile = new QTreeWidgetItem(age);
                    fniFile->setIcon(0, QPlasmaDocument::GetDocIcon(fni));
                    fniFile->setText(0, fni);
                    fniFile->setData(0, Qt::UserRole, gameRoot.filePath(fni));
                }

                QString sum = ageDisp + ".sum";
                if (gameRoot.exists(sum)) {
                    QTreeWidgetItem* sumFile = new QTreeWidgetItem(age);
                    sumFile->setIcon(0, QPlasmaDocument::GetDocIcon(sum));
                    sumFile->setText(0, sum);
                    sumFile->setData(0, Qt::UserRole, gameRoot.filePath(sum));
                }
            }
        }

        // Fonts; all games except Crowthistle
        if (gameType != GameInfo::kGameCrowthistle) {
            QTreeWidgetItem* fldr = new QTreeWidgetItem(fBrowserTree);
            fldr->setIcon(0, QPlasmaDocument::GetDocIcon("<FONTFOLD>"));
            fldr->setText(0, tr("Fonts"));
            fldr->setData(0, Qt::UserRole, "<FONTFOLD>");

            // Font packages; Myst 5 and Hex Isle
            if (gameType == GameInfo::kGameMyst5 || gameType == GameInfo::kGameHexIsle) {
                if (gameRoot.exists("Fonts.pfp")) {
                    QTreeWidgetItem* pfpFile = new QTreeWidgetItem(fldr);
                    pfpFile->setIcon(0, QPlasmaDocument::GetDocIcon("Fonts.pfp"));
                    pfpFile->setText(0, "Fonts.pfp");
                    pfpFile->setData(0, Qt::UserRole, gameRoot.filePath("Fonts.pfp"));
#ifndef Q_OS_WIN
                } else if (gameRoot.exists("fonts.pfp")) {
                    // For case-sensitive OSes
                    QTreeWidgetItem* pfpFile = new QTreeWidgetItem(fldr);
                    pfpFile->setIcon(0, QPlasmaDocument::GetDocIcon("fonts.pfp"));
                    pfpFile->setText(0, "fonts.pfp");
                    pfpFile->setData(0, Qt::UserRole, gameRoot.filePath("fonts.pfp"));
#endif
                }
            }
            RecursiveAddFiles(fldr, "<FONTFOLD>", QStringList("*.p2f"), gameRoot);
        }

        // Cursors and Images; Myst 5, CT and Hex Isle
        if (gameType == GameInfo::kGameMyst5 || gameType == GameInfo::kGameCrowthistle
            || gameType == GameInfo::kGameHexIsle) {
            QTreeWidgetItem* fldr = new QTreeWidgetItem(fBrowserTree);
            fldr->setIcon(0, QPlasmaDocument::GetDocIcon("<IMGFOLD>"));
            fldr->setText(0, tr("Images"));
            fldr->setData(0, Qt::UserRole, "<IMGFOLD>");
            if (gameRoot.exists("Cursors.dat")) {
                QTreeWidgetItem* csrFile = new QTreeWidgetItem(fldr);
                csrFile->setIcon(0, QPlasmaDocument::GetDocIcon("Cursors.dat"));
                csrFile->setText(0, "Cursors.dat");
                csrFile->setData(0, Qt::UserRole, gameRoot.filePath("Cursors.dat"));
#ifndef Q_OS_WIN
            } else if (gameRoot.exists("cursors.dat")) {
                // For case-sensitive OSes
                QTreeWidgetItem* csrFile = new QTreeWidgetItem(fldr);
                csrFile->setIcon(0, QPlasmaDocument::GetDocIcon("cursors.dat"));
                csrFile->setText(0, "cursors.dat");
                csrFile->setData(0, Qt::UserRole, gameRoot.filePath("cursors.dat"));
#endif
            }
            RecursiveAddFiles(fldr, "<IMGFOLD>", QStringList("*.tga"), gameRoot);
        }

        // Avatar and KI Images; Uru Live
        if (gameType == GameInfo::kGameUruLive) {
            QTreeWidgetItem* fldr = new QTreeWidgetItem(fBrowserTree);
            fldr->setIcon(0, QPlasmaDocument::GetDocIcon("<IMGFOLD>"));
            fldr->setText(0, tr("Images"));
            fldr->setData(0, Qt::UserRole, "<IMGFOLD>");
            if (appDataRoot.cd("Uru Live")) {
                RecursiveAddFiles(fldr, "<IMGFOLD>", QStringList("*.jpg"), appDataRoot);
                appDataRoot.cdUp();
            }
            if (docRoot.cd("Uru Live")) {
                RecursiveAddFiles(fldr, "<IMGFOLD>", QStringList("*.jpg"), docRoot);
                docRoot.cdUp();
            }
            if (fldr->childCount() == 0)
                delete fldr;
        }

        // Game Scripts; Hex Isle
        if (gameType == GameInfo::kGameHexIsle) {
            QTreeWidgetItem* fldr = new QTreeWidgetItem(fBrowserTree);
            fldr->setIcon(0, QPlasmaDocument::GetDocIcon("<AGEFOLD>"));
            fldr->setText(0, tr("Levels"));
            fldr->setData(0, Qt::UserRole, "<AGEFOLD>");

            QStringList files = gameRoot.entryList(QStringList("*.hex"));
            foreach (QString f, files) {
                QTreeWidgetItem* item = new QTreeWidgetItem(fldr);
                item->setIcon(0, QPlasmaDocument::GetDocIcon(f));
                item->setText(0, f);
                item->setData(0, Qt::UserRole, gameRoot.filePath(f));
            }
        }

        // Localization and Subtitles; Uru Live, Myst 5, MQO
        if (gameType == GameInfo::kGameUruLive || gameType == GameInfo::kGameMyst5
            || gameType == GameInfo::kGameMQO) {
            QTreeWidgetItem* fldr = new QTreeWidgetItem(fBrowserTree);
            fldr->setIcon(0, QPlasmaDocument::GetDocIcon("<DATAFOLD>"));
            fldr->setText(0, tr("Localization"));
            fldr->setData(0, Qt::UserRole, "<DATAFOLD>");
            RecursiveAddFiles(fldr, "<DATAFOLD>", QStringList()
                              << "*.loc" << "*.sub", gameRoot);
        }

        gameRoot.cdUp();
    }

    // Logs; All games
    {
        QTreeWidgetItem* fldr = new QTreeWidgetItem(fBrowserTree);
        fldr->setIcon(0, QPlasmaDocument::GetDocIcon("<DATAFOLD>"));
        fldr->setText(0, tr("Logs"));
        fldr->setData(0, Qt::UserRole, "<DATAFOLD>");

        // Logs for Uru Live
        if (gameType == GameInfo::kGameUruLive) {
            if (appDataRoot.cd("Uru Live")) {
                QTreeWidgetItem* appData = new QTreeWidgetItem(fldr);
                appData->setIcon(0, QPlasmaDocument::GetDocIcon("<DATAFOLD>"));
                appData->setText(0, tr("AppData"));
                appData->setData(0, Qt::UserRole, "<DATAFOLD>");
                RecursiveAddFiles(appData, "<DATAFOLD>", QStringList()
                                  << "*.txt" << "*.log" << "*.elf", appDataRoot);
                appDataRoot.cdUp();
                if (appData->childCount() == 0)
                    delete appData;
            }
            if (docRoot.cd("Uru Live")) {
                QTreeWidgetItem* docs = new QTreeWidgetItem(fldr);
                docs->setIcon(0, QPlasmaDocument::GetDocIcon("<DATAFOLD>"));
                docs->setText(0, tr("Uru Live"));
                docs->setData(0, Qt::UserRole, "<DATAFOLD>");
                RecursiveAddFiles(docs, "<DATAFOLD>", QStringList()
                                  << "*.txt" << "*.log" << "*.elf", docRoot);
                docRoot.cdUp();
                if (docs->childCount() == 0)
                    delete docs;
            }
        }

        // Logs for other games
        if (gameRoot.cd("Log")) {
            RecursiveAddFiles(fldr, "<DATAFOLD>", QStringList()
                              << "*.txt" << "*.log" << "*.elf", gameRoot);
            gameRoot.cdUp();
        }
#ifndef Q_OS_WIN
        if (gameRoot.cd("log")) {
            // For case-sensitive OSes
            RecursiveAddFiles(fldr, "<DATAFOLD>", QStringList()
                              << "*.txt" << "*.log" << "*.elf", gameRoot);
            gameRoot.cdUp();
        }
#endif

        if (fldr->childCount() == 0)
            delete fldr;
    }

    if (gameRoot.cd("Python")) {
        // Python; All games
        QTreeWidgetItem* fldr = new QTreeWidgetItem(fBrowserTree);
        fldr->setIcon(0, QPlasmaDocument::GetDocIcon("<PYFOLD>"));
        fldr->setText(0, tr("Python"));
        fldr->setData(0, Qt::UserRole, "<PYFOLD>");
        RecursiveAddFiles(fldr, "<PYFOLD>", QStringList()
                          << "*.pak" << "*.py", gameRoot);
        gameRoot.cdUp();
    }

    if (gameRoot.cd("SDL")) {
        // SDL; All games
        QTreeWidgetItem* fldr = new QTreeWidgetItem(fBrowserTree);
        fldr->setIcon(0, QPlasmaDocument::GetDocIcon("<SDLFOLD>"));
        fldr->setText(0, tr("SDL"));
        fldr->setData(0, Qt::UserRole, "<SDLFOLD>");
        RecursiveAddFiles(fldr, "<SDLFOLD>", QStringList("*.sdl"), gameRoot);
        gameRoot.cdUp();
    }

    // Settings; All games
    {
        QTreeWidgetItem* fldr = new QTreeWidgetItem(fBrowserTree);
        fldr->setIcon(0, QPlasmaDocument::GetDocIcon("<DATAFOLD>"));
        fldr->setText(0, tr("Settings"));
        fldr->setData(0, Qt::UserRole, "<DATAFOLD>");

        if (gameType == GameInfo::kGameCrowthistle) {
            // Settings for Crowthistle
            if (appDataRoot.cd("Crowthistle")) {
                RecursiveAddFiles(fldr, "<DATAFOLD>", QStringList("*.ini"), appDataRoot);
                appDataRoot.cdUp();
            }
        } else if (gameType == GameInfo::kGameHexIsle) {
            // Settings for Hex Isle
            if (appDataRoot.cd("Hexisle")) {
                RecursiveAddFiles(fldr, "<DATAFOLD>", QStringList()
                                  << "*.fni" << "*.ini" << "*.hex", appDataRoot);
                appDataRoot.cdUp();
            }
        } else if (gameType == GameInfo::kGameMyst5) {
            // Settings for Myst 5
            if (appDataRoot.cd("Myst V Demo")) {
                QTreeWidgetItem* appData = new QTreeWidgetItem(fldr);
                appData->setIcon(0, QPlasmaDocument::GetDocIcon("<DATAFOLD>"));
                appData->setText(0, tr("Demo"));
                appData->setData(0, Qt::UserRole, "<DATAFOLD>");
                RecursiveAddFiles(appData, "<DATAFOLD>", QStringList("*.ini"), appDataRoot);
                appDataRoot.cdUp();
                if (appData->childCount() == 0)
                    delete appData;
            }
            if (appDataRoot.cd("Myst V End of Ages")) {
                QTreeWidgetItem* appData = new QTreeWidgetItem(fldr);
                appData->setIcon(0, QPlasmaDocument::GetDocIcon("<DATAFOLD>"));
                appData->setText(0, tr("End of Ages"));
                appData->setData(0, Qt::UserRole, "<DATAFOLD>");
                RecursiveAddFiles(appData, "<DATAFOLD>", QStringList("*.ini"), appDataRoot);
                appDataRoot.cdUp();
                if (appData->childCount() == 0)
                    delete appData;
            }
        } else if (gameType == GameInfo::kGameUruLive) {
            // Settings for Uru Live
            if (appDataRoot.cd("Uru Live")) {
                QTreeWidgetItem* appData = new QTreeWidgetItem(fldr);
                appData->setIcon(0, QPlasmaDocument::GetDocIcon("<DATAFOLD>"));
                appData->setText(0, tr("AppData"));
                appData->setData(0, Qt::UserRole, "<DATAFOLD>");
                RecursiveAddFiles(appData, "<DATAFOLD>", QStringList("*.ini"), appDataRoot);
                appDataRoot.cdUp();
                if (appData->childCount() == 0)
                    delete appData;
            }
            if (docRoot.cd("Uru Live")) {
                QTreeWidgetItem* docs = new QTreeWidgetItem(fldr);
                docs->setIcon(0, QPlasmaDocument::GetDocIcon("<DATAFOLD>"));
                docs->setText(0, tr("Uru Live"));
                docs->setData(0, Qt::UserRole, "<DATAFOLD>");
                RecursiveAddFiles(docs, "<DATAFOLD>", QStringList("*.ini"), docRoot);
                docRoot.cdUp();
                if (docs->childCount() == 0)
                    delete docs;
            }
        }

        // All other settings in the game folder
        RecursiveAddFiles(fldr, "<DATAFOLD>", QStringList()
                          << "*.cfg" << "*.ini", gameRoot);

        if (fldr->childCount() == 0)
            delete fldr;
    }

    if (gameRoot.cd("fx") && gameType == GameInfo::kGameHexIsle) {
        // Shaders; Hex Isle
        QTreeWidgetItem* fldr = new QTreeWidgetItem(fBrowserTree);
        fldr->setIcon(0, QPlasmaDocument::GetDocIcon("<DATAFOLD>"));
        fldr->setText(0, tr("Shaders"));
        fldr->setData(0, Qt::UserRole, "<DATAFOLD>");
        RecursiveAddFiles(fldr, "<DATAFOLD>", QStringList("*.fx"), gameRoot);
        gameRoot.cdUp();
    }

    // Vault; Single-Player Uru
    if (gameType == GameInfo::kGameUru && gameRoot.exists("sav")) {
        QTreeWidgetItem* vault = new QTreeWidgetItem(fBrowserTree);
        vault->setIcon(0, QPlasmaDocument::GetDocIcon("<VAULT>"));
        vault->setText(0, tr("Single-Player Vault"));
        vault->setData(0, Qt::UserRole, QString("VAULT:%1").arg(gameRoot.absolutePath()));
    }
}

void PlasmaShopMain::onBrowserItemActivated(QTreeWidgetItem* item, int)
{
    QString filename = item->data(0, Qt::UserRole).toString();
    if (filename[0] == '<')
        return;
    if (filename.startsWith("VAULT:")) {
        QString path = filename.mid(6);
        QSettings settings("PlasmaShop", "PlasmaShop");
        QString editor = settings.value("VaultEditorPath", DEFAULT_VAULT_EDITOR).toString();
        QProcess proc;
        proc.startDetached(GetPSBinPath(editor), QStringList(path));
        return;
    }
    loadFile(filename);
}

void PlasmaShopMain::updateMenuStatus()
{
    if (fEditorPane->currentIndex() < 0)
        return;
    QPlasmaDocument* doc = (QPlasmaDocument*)fEditorPane->currentWidget();
    fActions[kEditCut]->setEnabled(doc->canCut());
    fActions[kEditCopy]->setEnabled(doc->canCopy());
    fActions[kEditPaste]->setEnabled(doc->canPaste());
    fActions[kEditDelete]->setEnabled(doc->canDelete());
    fActions[kEditSelectAll]->setEnabled(doc->canSelectAll());
    fActions[kEditUndo]->setEnabled(doc->canUndo());
    fActions[kEditRedo]->setEnabled(doc->canRedo());
}

void PlasmaShopMain::onDocDirty()
{
    if (fEditorPane->currentIndex() < 0)
        return;
    QString curTitle = fEditorPane->tabText(fEditorPane->currentIndex());
    fEditorPane->setTabText(fEditorPane->currentIndex(), "* " + curTitle);
}

void PlasmaShopMain::onDocClean()
{
    if (fEditorPane->currentIndex() < 0)
        return;
    QString curTitle = fEditorPane->tabText(fEditorPane->currentIndex());
    fEditorPane->setTabText(fEditorPane->currentIndex(), curTitle.mid(2));
}


int main(int argc, char* argv[])
{
    // Redirect libPlasma's debug stuff to PlasmaShop.log
    plDebug::InitFile(plDebug::kDLAll, "PlasmaShop.log");

    QApplication app(argc, argv);

    // Set this at the very beginning, so it can be re-used in case the
    // application's CWD changes
    s_binBasePath = app.applicationDirPath();

    PlasmaShopMain mainWnd;
    mainWnd.show();
    for (int i=1; i<argc; i++)
        mainWnd.loadFile(argv[i]);
    return app.exec();
}
