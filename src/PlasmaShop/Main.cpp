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
#include <QMenuBar>
#include <QToolBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include <QProcess>
#include <QLabel>
#include <QDropEvent>
#include <QUrl>
#include <QMimeData>
#include <qticonloader.h>
#include <Debug/plDebug.h>

#include "Main.h"
#include "QPlasma.h"
#include "OptionsDialog.h"
#include "QPlasmaTextDoc.h"
#include "QPlasmaPakFile.h"
#include "GameScanner.h"
#include "NewFile.h"

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
    fActions[kFileSaveAs] = new QAction(qStdIcon("document-save-as"), tr("Save &As..."), this);
    fActions[kFileRevert] = new QAction(qStdIcon("document-revert"), tr("Re&load"), this);
    fActions[kFileOptions] = new QAction(tr("&Preferences..."), this);
    fActions[kFileShowBrowser] = new QAction(tr("Show File &Browser"), this);
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
    fActions[kTextReplace] = new QAction(qStdIcon("edit-find-replace"), tr("&Replace..."), this);
    fActions[kTextStxNone] = new QAction(tr("&None"), this);
    fActions[kTextStxPython] = new QAction(tr("&Python"), this);
    fActions[kTextStxSDL] = new QAction(tr("&SDL"), this);
    fActions[kTextStxIni] = new QAction(tr("&Age / Ini"), this);
    fActions[kTextStxConsole] = new QAction(tr("&Console"), this);
    fActions[kTextStxXML] = new QAction(tr("&XML"), this);
    fActions[kTextStxHex] = new QAction(tr("&Hex Isle Level"), this);
    fActions[kTextStxFX] = new QAction(tr("Shader &FX"), this);
    fActions[kTextTypeAnsi] = new QAction(tr("&ANSI"), this);
    fActions[kTextTypeUTF8] = new QAction(tr("UTF-&8"), this);
    fActions[kTextTypeUTF16] = new QAction(tr("UTF-1&6"), this);
    fActions[kTextTypeUTF32] = new QAction(tr("UTF-&32"), this);
    fActions[kTextExpandAll] = new QAction(tr("E&xpand All"), this);
    fActions[kTextCollapseAll] = new QAction(tr("&Collapse All"), this);

    fActions[kGenEncNone] = new QAction(tr("&None"), this);
    fActions[kGenEncXtea] = new QAction(tr("&Uru Prime / PotS / UU"), this);
    fActions[kGenEncAes] = new QAction(tr("Myst &5 / Crowthistle / Hex Isle"), this);
    fActions[kGenEncDroid] = new QAction(tr("&MOUL / MQO (Requires Key)"), this);

    fActions[kTreeOpen] = new QAction(tr("&Open"), this);
    fActions[kTreeRename] = new QAction(tr("&Rename"), this);
    fActions[kTreeDelete] = new QAction(tr("&Delete"), this);

    fActions[kFileNew]->setShortcut(Qt::CTRL + Qt::Key_N);
    fActions[kFileOpen]->setShortcut(Qt::CTRL + Qt::Key_O);
    fActions[kFileSave]->setShortcut(Qt::CTRL + Qt::Key_S);
    fActions[kFileSaveAs]->setShortcut(Qt::SHIFT + Qt::CTRL + Qt::Key_S);
    fActions[kFileRevert]->setShortcut(Qt::Key_F5);
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
    fActions[kTextReplace]->setShortcut(Qt::CTRL + Qt::Key_R);

    fActions[kFileShowBrowser]->setCheckable(true);
    fActions[kTextStxNone]->setCheckable(true);
    fActions[kTextStxPython]->setCheckable(true);
    fActions[kTextStxSDL]->setCheckable(true);
    fActions[kTextStxIni]->setCheckable(true);
    fActions[kTextStxConsole]->setCheckable(true);
    fActions[kTextStxXML]->setCheckable(true);
    fActions[kTextStxHex]->setCheckable(true);
    fActions[kTextStxFX]->setCheckable(true);
    fActions[kTextTypeAnsi]->setCheckable(true);
    fActions[kTextTypeUTF8]->setCheckable(true);
    fActions[kTextTypeUTF16]->setCheckable(true);
    fActions[kTextTypeUTF32]->setCheckable(true);
    fActions[kGenEncNone]->setCheckable(true);
    fActions[kGenEncXtea]->setCheckable(true);
    fActions[kGenEncAes]->setCheckable(true);
    fActions[kGenEncDroid]->setCheckable(true);

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
    fileMenu->addAction(fActions[kFileShowBrowser]);
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
    fTextMenu = menuBar()->addMenu(tr("&Text"));
    fTextMenu->addAction(fActions[kTextFind]);
    fTextMenu->addAction(fActions[kTextFindNext]);
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
    QMenu* text_typeMenu = fTextMenu->addMenu(tr("File &Encoding"));
    text_typeMenu->addAction(fActions[kTextTypeAnsi]);
    text_typeMenu->addAction(fActions[kTextTypeUTF8]);
    text_typeMenu->addAction(fActions[kTextTypeUTF16]);
    text_typeMenu->addAction(fActions[kTextTypeUTF32]);
    fTextMenu->addSeparator();
    fTextMenu->addAction(fActions[kTextExpandAll]);
    fTextMenu->addAction(fActions[kTextCollapseAll]);

    fEncryptMenu = menuBar()->addMenu(tr("Encr&yption"));
    fEncryptMenu->addAction(fActions[kGenEncNone]);
    fEncryptMenu->addAction(fActions[kGenEncXtea]);
    fEncryptMenu->addAction(fActions[kGenEncAes]);
    fEncryptMenu->addAction(fActions[kGenEncDroid]);

    // Help menu is always at the end
    fHelpMenu = menuBar()->addMenu(tr("&Help"));
    fHelpMenu->addAction(fActions[kHelpAbout]);

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
    QLabel* lblGame = new QLabel(tr("&Game: "), this);
    lblGame->setBuddy(fGameSelector);
    mainTbar->addWidget(lblGame);
    mainTbar->addWidget(fGameSelector);
    fGameSelector->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    statusBar();

    // Tabbed Editor Pane for open documents
    fEditorPane = new QTabWidget(this);
    fEditorPane->setMovable(true);
    fEditorPane->setTabsClosable(true);
    setCentralWidget(fEditorPane);

    // File Browser
    fBrowserDock = new QDockWidget(tr("File Browser"), this);
    fBrowserDock->setObjectName("BrowserDock");
    fBrowserTree = new QTreeWidget(fBrowserDock);
    fBrowserDock->setWidget(fBrowserTree);
    fBrowserDock->setAllowedAreas(Qt::LeftDockWidgetArea |
                                  Qt::RightDockWidgetArea);
    fBrowserDock->setFeatures(QDockWidget::DockWidgetMovable |
                              QDockWidget::DockWidgetFloatable |
                              QDockWidget::DockWidgetClosable);
    fBrowserTree->setUniformRowHeights(true);
    fBrowserTree->setHeaderHidden(true);
    fBrowserTree->setContextMenuPolicy(Qt::CustomContextMenu);
    addDockWidget(Qt::LeftDockWidgetArea, fBrowserDock);
    fScanner = new GameScanner(fBrowserTree);

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

    fActions[kFileShowBrowser]->setChecked(fBrowserDock->isVisible());

    // Signals
    connect(fActions[kFileNew], SIGNAL(triggered()), this, SLOT(onNewFile()));
    connect(fActions[kFileOpen], SIGNAL(triggered()), this, SLOT(onOpenFile()));
    connect(fActions[kFileSave], SIGNAL(triggered()), this, SLOT(onSaveFile()));
    connect(fActions[kFileSaveAs], SIGNAL(triggered()), this, SLOT(onSaveAs()));
    connect(fActions[kFileRevert], SIGNAL(triggered()), this, SLOT(onRevert()));
    connect(fActions[kFileOptions], SIGNAL(triggered()), this, SLOT(onOptions()));
    connect(fActions[kFileShowBrowser], SIGNAL(triggered(bool)), fBrowserDock, SLOT(setVisible(bool)));
    connect(fActions[kFileExit], SIGNAL(triggered()), this, SLOT(close()));

    connect(fActions[kEditCut], SIGNAL(triggered()), this, SLOT(onCut()));
    connect(fActions[kEditCopy], SIGNAL(triggered()), this, SLOT(onCopy()));
    connect(fActions[kEditPaste], SIGNAL(triggered()), this, SLOT(onPaste()));
    connect(fActions[kEditDelete], SIGNAL(triggered()), this, SLOT(onDelete()));
    connect(fActions[kEditSelectAll], SIGNAL(triggered()), this, SLOT(onSelectAll()));
    connect(fActions[kEditUndo], SIGNAL(triggered()), this, SLOT(onUndo()));
    connect(fActions[kEditRedo], SIGNAL(triggered()), this, SLOT(onRedo()));

    connect(fActions[kTextFind], SIGNAL(triggered()), this, SLOT(onTextFind()));
    connect(fActions[kTextFindNext], SIGNAL(triggered()), this, SLOT(onTextFindNext()));
    connect(fActions[kTextReplace], SIGNAL(triggered()), this, SLOT(onTextReplace()));
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
    connect(fActions[kTextTypeAnsi], SIGNAL(triggered()), this, SLOT(onTextTypeAnsi()));
    connect(fActions[kTextTypeUTF8], SIGNAL(triggered()), this, SLOT(onTextTypeUTF8()));
    connect(fActions[kTextTypeUTF16], SIGNAL(triggered()), this, SLOT(onTextTypeUTF16()));
    connect(fActions[kTextTypeUTF32], SIGNAL(triggered()), this, SLOT(onTextTypeUTF32()));
    connect(fActions[kGenEncNone], SIGNAL(triggered()), this, SLOT(onGenEncNone()));
    connect(fActions[kGenEncXtea], SIGNAL(triggered()), this, SLOT(onGenEncXtea()));
    connect(fActions[kGenEncAes], SIGNAL(triggered()), this, SLOT(onGenEncAes()));
    connect(fActions[kGenEncDroid], SIGNAL(triggered()), this, SLOT(onGenEncDroid()));

    connect(fGameSelector, SIGNAL(activated(int)), this, SLOT(onSelectGame(int)));
    connect(fBrowserTree, SIGNAL(itemActivated(QTreeWidgetItem*, int)),
            this, SLOT(onBrowserItemActivated(QTreeWidgetItem*, int)));
    connect(fEditorPane, SIGNAL(tabCloseRequested(int)), this, SLOT(onCloseTab(int)));
    connect(fEditorPane, SIGNAL(currentChanged(int)), this, SLOT(onChangeTab(int)));
    connect(fBrowserDock, SIGNAL(visibilityChanged(bool)), fActions[kFileShowBrowser], SLOT(setChecked(bool)));

    // Set up menus, etc
    onChangeTab(-1);
    populateGameList();
    onSelectGame(fGameSelector->currentIndex());
}

PlasmaShopMain::~PlasmaShopMain()
{
    delete fScanner;
}

void PlasmaShopMain::loadFile(QString filename)
{
    for (int i=0; i<fEditorPane->count(); i++) {
        QPlasmaDocument* doc = (QPlasmaDocument*)fEditorPane->widget(i);
        if (doc->filename() == filename) {
            // File already open -- focus it
            fEditorPane->setCurrentIndex(i);
            return;
        }
    }

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
        ext == "inf" || ext == "mfold" || ext == "mlist") {
        dtype = kDocText;
    } else if (ext == "p2f") {
        dtype = kDocFont;
    } else if (ext == "pfp" || ext == "pak" || fnameNoPath == "cursors.dat" ||
               fnameNoPath == "resource.dat") {
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
        if (QPlasmaPakFile::decompylePyc(this, filename)) {
            filename.replace(".pyc", ".py");
            fnameDisplay.replace(".pyc", ".py");
            fnameNoPath.replace(".pyc", ".py");
            ext = "py";
            dtype = kDocText;
        } else {
            return;
        }
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
        return;
    } else if (fnameNoPath == "dev_mode.dat") {
        dtype = kDocDevMode;
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
            QMessageBox::critical(this, tr("Oops"),
                        tr("Loading %1 failed.").arg(filename),
                        QMessageBox::Ok);
            // Error loading the file.  Remove it now to avoid problems later
            fEditorPane->removeTab(fEditorPane->count() - 1);
            delete plDoc;
            return;
        }
        if (dtype == kDocText) {
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

        // Select the last opened file's tab
        fEditorPane->setCurrentIndex(fEditorPane->count() - 1);
    } else {
        QMessageBox::critical(this, tr("Oops"),
                              tr("No editor is currently available for %1")
                              .arg(fnameNoPath), QMessageBox::Ok);
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
                if (!doc->saveDefault()) {
                    QMessageBox::critical(this, tr("Oops"),
                                tr("Writing to %1 failed.").arg(doc->filename()),
                                QMessageBox::Ok);
                    return;
                }
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

void PlasmaShopMain::dragEnterEvent(QDragEnterEvent* evt)
{
    if (evt->mimeData()->hasUrls())
        evt->acceptProposedAction();
}

void PlasmaShopMain::dropEvent(QDropEvent* evt)
{
    if (evt->mimeData()->hasUrls()) {
        foreach (QUrl url, evt->mimeData()->urls()) {
            QString filename = url.toLocalFile();
            if (!filename.isEmpty())
                loadFile(filename);
        }
    }
}

void PlasmaShopMain::onNewFile()
{
    NewFileDialog dlg(this);
    dlg.setModal(true);
    if (dlg.exec() != QDialog::Accepted)
        return;

    DocumentType dtype = kDocUnknown;
    QString fnameDisplay;
    QPlasmaTextDoc::SyntaxMode syntax = QPlasmaTextDoc::kStxNone;
    QPlasmaTextDoc::EncodingMode encoding = QPlasmaTextDoc::kTypeAnsi;

    QPlasmaDocument::EncryptionMode encrypt = QPlasmaDocument::kEncNone;
    int gameType = fGames.isEmpty() ? GameInfo::kGameNone : fGames[fCurrentGame-1].fGameType;
    if (gameType == GameInfo::kGameUru || gameType == GameInfo::kGameUruLive ||
        gameType == GameInfo::kGameMQO) {
        encrypt = QPlasmaDocument::kEncXtea;
    } else if (gameType == GameInfo::kGameMyst5 || gameType == GameInfo::kGameCrowthistle ||
               gameType == GameInfo::kGameHexIsle) {
        encrypt = QPlasmaDocument::kEncAes;
    }

    switch (dlg.fileType()) {
    case kFileAge:
        dtype = kDocText;
        fnameDisplay = "Unnamed.age";
        syntax = QPlasmaTextDoc::kStxIni;
        break;
    case kFileCsv:
        dtype = kDocText;
        fnameDisplay = "Unnamed.csv";
        break;
    case kFileFni:
        dtype = kDocText;
        fnameDisplay = "Unnamed.fni";
        syntax = QPlasmaTextDoc::kStxConsole;
        break;
    case kFileShader:
        dtype = kDocText;
        fnameDisplay = "Unnamed.fx";
        syntax = QPlasmaTextDoc::kStxFX;
        encrypt = QPlasmaDocument::kEncNone;
        break;
    case kFileHexLevel:
        dtype = kDocText;
        fnameDisplay = "Unnamed.hex";
        syntax = QPlasmaTextDoc::kStxHex;
        encrypt = QPlasmaDocument::kEncNone;
        break;
    case kFileLoc:
        dtype = kDocText;
        fnameDisplay = "Unnamed.loc";
        syntax = QPlasmaTextDoc::kStxXML;
        encrypt = QPlasmaDocument::kEncNone;
        encoding = QPlasmaTextDoc::kTypeUTF16;
        break;
    case kFilePython:
        dtype = kDocText;
        fnameDisplay = "Unnamed.py";
        syntax = QPlasmaTextDoc::kStxPython;
        encrypt = QPlasmaDocument::kEncNone;
        break;
    case kFileSDL:
        dtype = kDocText;
        fnameDisplay = "Unnamed.sdl";
        syntax = QPlasmaTextDoc::kStxSDL;
        if (gameType == GameInfo::kGameUruLive || gameType == GameInfo::kGameMQO)
            encrypt = QPlasmaDocument::kEncDroid;
        break;
    case kFileSub:
        dtype = kDocText;
        fnameDisplay = "Unnamed.sub";
        syntax = QPlasmaTextDoc::kStxXML;
        encoding = QPlasmaTextDoc::kTypeUTF16;
        break;
    case kFileFont:
        dtype = kDocFont;
        fnameDisplay = "Unnamed.p2f";
        break;
    case kFilePak:
        dtype = kDocPackage;
        fnameDisplay = "Unnamed.pak";
        if (gameType == GameInfo::kGameUruLive || gameType == GameInfo::kGameMQO)
            encrypt = QPlasmaDocument::kEncDroid;
        break;
    case kFileCursorPak:
        dtype = kDocPackage;
        fnameDisplay = "Cursors.dat";
        break;
    case kFileFontPak:
        dtype = kDocPackage;
        fnameDisplay = "Fonts.pfp";
        break;
    case kFileSum:
        dtype = kDocManifest;
        fnameDisplay = "Unnamed.sum";
        break;
    default:
        QMessageBox::critical(this, tr("Unsupported File Type"),
                              tr("Error: Cannot create unsupported file type"),
                              QMessageBox::Ok);
        return;
    }

    // Create an appropriate editor and add it to the document panel
    QPlasmaDocument* plDoc = QPlasmaDocument::GetEditor(dtype, this);
    if (plDoc != NULL) {
        fEditorPane->addTab(plDoc, QPlasmaDocument::GetDocIcon(fnameDisplay), fnameDisplay);
        plDoc->setFilename(fnameDisplay);
        plDoc->setEncryption(encrypt);
        if (dtype == kDocText) {
            ((QPlasmaTextDoc*)plDoc)->setSyntax(syntax);
            ((QPlasmaTextDoc*)plDoc)->setEncoding(encoding);
        } else if (dtype == kDocPackage) {
            if (dlg.fileType() == kFilePak)
                ((QPlasmaPakFile*)plDoc)->setPackageType(PlasmaPackage::kPythonPak);
            else if (dlg.fileType() == kFileCursorPak)
                ((QPlasmaPakFile*)plDoc)->setPackageType(PlasmaPackage::kCursorsDat);
            else if (dlg.fileType() == kFileFontPak)
                ((QPlasmaPakFile*)plDoc)->setPackageType(PlasmaPackage::kFontsPfp);
        }
        plDoc->makeClean();
        connect(plDoc, SIGNAL(statusChanged()), this, SLOT(updateMenuStatus()));
        connect(plDoc, SIGNAL(becameDirty()), this, SLOT(onDocDirty()));
        connect(plDoc, SIGNAL(becameClean()), this, SLOT(onDocClean()));

        // Update menus
        onChangeTab(fEditorPane->currentIndex());

        // Select the last opened file's tab
        fEditorPane->setCurrentIndex(fEditorPane->count() - 1);
    } else {
        QMessageBox::critical(this, tr("Oops"),
                              tr("No editor is currently available for this file type"),
                              QMessageBox::Ok);
    }
}

void PlasmaShopMain::onOpenFile()
{
    QStringList files = QFileDialog::getOpenFileNames(this,
                            tr("Open File(s)"), fDialogDir,
                            "All supported Plasma Files (*.age *.cfg *.ini *.fni Cursors.dat dev_mode.dat *.hex *.loc *.sub *.log *.elf *.sum *.p2f *.pfp *.pak *.py *.pyc *.csv *.sdl *.fx *.mfs *.txt *.xml *.inf *.mfold *.mlist);;"
                            "Age Files (*.age);;"
                            "Config Files (*.cfg *.ini);;"
                            "Console Files (*.fni);;"
                            "Cursor Packages (Cursors.dat);;"
                            "Device Mode Files (dev_mode.dat);;"
                            "Hex Isle Levels (*.hex);;"
                            "Localization Files (*.loc *.sub);;"
                            "Log Files (*.log *.elf);;"
                            "Manifest Files (*.sum);;"
                            "Plasma Font Files (*.p2f);;"
                            "Plasma Font Packages (*.pfp);;"
                            "Python Packages (*.pak);;"
                            "Python Source Files (*.py);;"
                            "Python Bytecode (*.pyc);;"
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
}

void PlasmaShopMain::onSaveFile()
{
    if (fEditorPane->currentIndex() < 0)
        return;
    QPlasmaDocument* doc = (QPlasmaDocument*)fEditorPane->currentWidget();
    if (doc->filename() == "<NEW>")
        onSaveAs();
    else if (doc->isDirty()) {
        if (!doc->saveDefault()) {
            QMessageBox::critical(this, tr("Oops"),
                        tr("Writing to %1 failed.").arg(doc->filename()),
                        QMessageBox::Ok);
            return;
        }
    }
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
        if (((QPlasmaPakFile*)doc)->packageType() == PlasmaPackage::kCursorsDat) {
            typeList = "Cursor Packages (Cursors.dat);;"
                       "All Files (*)";
            curFilter = "Cursor Packages (Cursors.dat)";
        } else if (((QPlasmaPakFile*)doc)->packageType() == PlasmaPackage::kFontsPfp) {
            typeList = "Plasma Font Packages (*.pfp);;"
                       "All Files (*)";
            curFilter = "Plasma Font Packages (*.pfp)";
        } else {
            typeList = "Python Packages (*.pak);;"
                       "All Files (*)";
            curFilter = "Python Packages (*.pak)";
        }
    } else if (doc->docType() == kDocDevMode) {
        typeList = "Device Mode Files (dev_mode.dat);;"
                   "All Files (*)";
        curFilter = "Device Mode Files (dev_mode.dat)";
    } else {
        QMessageBox::critical(this, tr("Unsupported File Type"),
                              tr("Error: Cannot save unsupported file type"),
                              QMessageBox::Ok);
        return;
    }
    QString filename = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                    doc->filename(),
                                                    typeList, &curFilter);
    if (!filename.isEmpty()) {
        if (!doc->saveTo(filename)) {
            QMessageBox::critical(this, tr("Oops"),
                        tr("Writing to %1 failed.").arg(filename),
                        QMessageBox::Ok);
            return;
        }
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
    } else {
        // there are no unsaved changes, but the file itself might have changed (useful especially for logfiles)
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

void PlasmaShopMain::onTextFind()
{
    if (fEditorPane->currentIndex() < 0)
        return;
    QPlasmaDocument* doc = (QPlasmaDocument*)fEditorPane->currentWidget();
    if (doc->docType() != kDocText)
        return;
    ((QPlasmaTextDoc*)doc)->textFind();
}

void PlasmaShopMain::onTextFindNext()
{
    if (fEditorPane->currentIndex() < 0)
        return;
    QPlasmaDocument* doc = (QPlasmaDocument*)fEditorPane->currentWidget();
    if (doc->docType() != kDocText)
        return;
    ((QPlasmaTextDoc*)doc)->textFindNext();
}

void PlasmaShopMain::onTextReplace()
{
    if (fEditorPane->currentIndex() < 0)
        return;
    QPlasmaDocument* doc = (QPlasmaDocument*)fEditorPane->currentWidget();
    if (doc->docType() != kDocText)
        return;
    ((QPlasmaTextDoc*)doc)->textReplace();
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

void PlasmaShopMain::onGenEncNone()
{
    if (fEditorPane->currentIndex() < 0)
        return;
    QPlasmaDocument* doc = (QPlasmaDocument*)fEditorPane->currentWidget();
    doc->setEncryption(QPlasmaDocument::kEncNone);
    setEncryption(kGenEncNone);
}

void PlasmaShopMain::onGenEncXtea()
{
    if (fEditorPane->currentIndex() < 0)
        return;
    QPlasmaDocument* doc = (QPlasmaDocument*)fEditorPane->currentWidget();
    doc->setEncryption(QPlasmaDocument::kEncXtea);
    setEncryption(kGenEncXtea);
}

void PlasmaShopMain::onGenEncAes()
{
    if (fEditorPane->currentIndex() < 0)
        return;
    QPlasmaDocument* doc = (QPlasmaDocument*)fEditorPane->currentWidget();
    doc->setEncryption(QPlasmaDocument::kEncAes);
    setEncryption(kGenEncAes);
}

void PlasmaShopMain::onGenEncDroid()
{
    if (fEditorPane->currentIndex() < 0)
        return;
    QPlasmaDocument* doc = (QPlasmaDocument*)fEditorPane->currentWidget();
    doc->setEncryption(QPlasmaDocument::kEncDroid);
    setEncryption(kGenEncDroid);
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
        if (result == QMessageBox::Yes) {
            if (!doc->saveDefault()) {
                QMessageBox::critical(this, tr("Oops"),
                            tr("Writing to %1 failed.").arg(doc->filename()),
                            QMessageBox::Ok);
                return;
            }
        }
        else if (result == QMessageBox::Cancel) {
            // Don't continue and don't close anything
            return;
        }
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

void PlasmaShopMain::setTextEncoding(int sel)
{
    fActions[kTextTypeAnsi]->setChecked(sel == kTextTypeAnsi);
    fActions[kTextTypeUTF8]->setChecked(sel == kTextTypeUTF8);
    fActions[kTextTypeUTF16]->setChecked(sel == kTextTypeUTF16);
    fActions[kTextTypeUTF32]->setChecked(sel == kTextTypeUTF32);
}

void PlasmaShopMain::setEncryption(int sel)
{
    fActions[kGenEncNone]->setChecked(sel == kGenEncNone);
    fActions[kGenEncXtea]->setChecked(sel == kGenEncXtea);
    fActions[kGenEncAes]->setChecked(sel == kGenEncAes);
    fActions[kGenEncDroid]->setChecked(sel == kGenEncDroid);
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
    menuBar()->removeAction(fTextMenu->menuAction());
    fTextMenu->setEnabled(false);

    fActions[kTextFind]->setEnabled(false);
    fActions[kTextFindNext]->setEnabled(false);
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
    fActions[kTextTypeAnsi]->setEnabled(false);
    fActions[kTextTypeUTF8]->setEnabled(false);
    fActions[kTextTypeUTF16]->setEnabled(false);
    fActions[kTextTypeUTF32]->setEnabled(false);
    fActions[kGenEncNone]->setEnabled(false);
    fActions[kGenEncXtea]->setEnabled(false);
    fActions[kGenEncAes]->setEnabled(false);
    fActions[kGenEncDroid]->setEnabled(false);
    setTextSyntax(kTextStxNone);
    setTextEncoding(kTextTypeAnsi);
    setEncryption(kGenEncNone);

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
    fActions[kGenEncNone]->setEnabled(true);
    fActions[kGenEncXtea]->setEnabled(true);
    fActions[kGenEncAes]->setEnabled(true);
    fActions[kGenEncDroid]->setEnabled(true);

    QPlasmaDocument* doc = (QPlasmaDocument*)fEditorPane->widget(idx);
    if (doc->docType() == kDocText) {
        menuBar()->insertMenu(fEncryptMenu->menuAction(), fTextMenu);
        fTextMenu->setEnabled(true);

        fActions[kTextFind]->setEnabled(true);
        fActions[kTextFindNext]->setEnabled(true);
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
        fActions[kTextTypeAnsi]->setEnabled(true);
        fActions[kTextTypeUTF8]->setEnabled(true);
        fActions[kTextTypeUTF16]->setEnabled(true);
        fActions[kTextTypeUTF32]->setEnabled(true);

        QPlasmaTextDoc::SyntaxMode syn = ((QPlasmaTextDoc*)doc)->syntax();
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

    switch (doc->encryption()) {
    case QPlasmaDocument::kEncAes:
        setEncryption(kGenEncAes);
        break;
    case QPlasmaDocument::kEncDroid:
        setEncryption(kGenEncDroid);
        break;
    case QPlasmaDocument::kEncNone:
        setEncryption(kGenEncNone);
        break;
    case QPlasmaDocument::kEncXtea:
        setEncryption(kGenEncXtea);
        break;
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

    if (fCurrentGame == 0) {
        fBrowserTree->clear();
    } else {
        fScanner->scanGame(fGames[fCurrentGame-1].fGameType,
                           fGames[fCurrentGame-1].fGamePath,
                           GetAppDataPath(), GetDocumentsPath());
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
    QString logpath = QStandardPaths_DataLocation();
    QDir dir;
    dir.mkpath(logpath);
    logpath += "/PlasmaShop.log";
    plDebug::InitFile(plDebug::kDLAll, logpath.toUtf8().constData());

    QApplication app(argc, argv);

    // Set this at the very beginning, so it can be re-used in case the
    // application's CWD changes
    s_binBasePath = app.applicationDirPath();

    PlasmaShopMain mainWnd;
    mainWnd.show();
    for (int i=1; i<argc; i++)
        mainWnd.loadFile(argv[i]);
    mainWnd.setAcceptDrops(true);
    return app.exec();
}
