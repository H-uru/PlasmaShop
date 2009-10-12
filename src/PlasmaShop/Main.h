#ifndef _PLASMASHOP_MAIN_H
#define _PLASMASHOP_MAIN_H

#include <QMainWindow>
#include <QTreeWidget>
#include <QTabWidget>
#include <QComboBox>
#include <QDockWidget>
#include <QAction>
#include <QMenu>
#include "QPlasmaDocument.h"
#include "GameBrowser.h"

#define PLASMASHOP_VERSION "3.0 (build 86)"

class PlasmaShopMain : public QMainWindow {
    Q_OBJECT

private:
    QString fDialogDir;
    QTabWidget* fEditorPane;
    QDockWidget* fBrowserDock;

    // Menu actions
    enum {
        // Main Menu
        kFileNew, kFileOpen, kFileSave, kFileSaveAs, kFileExit, kFileOptions,
        kFileRevert,
        kEditUndo, kEditRedo, kEditCut, kEditCopy, kEditPaste, kEditDelete,
        kEditSelectAll,
        kHelpAbout,

        // Text Editor Menu
        kTextFind, kTextFindNext, kTextFindPrev, kTextReplace,
        kTextStxNone, kTextStxPython, kTextStxSDL, kTextStxIni,
        kTextStxConsole, kTextStxXML, kTextStxHex, kTextStxFX,
        kTextEncNone, kTextEncXtea, kTextEncAes, kTextEncDroid,
        kTextTypeAnsi, kTextTypeUTF8, kTextTypeUTF16, kTextTypeUTF32,
        kTextExpandAll, kTextCollapseAll,

        // Tree Context Menu
        kTreeOpen, kTreeRename, kTreeDelete,

        kNumActions
    };
    QAction* fActions[kNumActions];

    // Editor-specific menus (for dynamically hiding)
    QMenu* fTextMenu;

    // Game Browser
    QTreeWidget* fBrowserTree;
    QComboBox* fGameSelector;
    QList<GameInfo> fGames;
    int fCurrentGame;

public:
    PlasmaShopMain();
    void loadFile(QString filename);

protected:
    virtual void closeEvent(QCloseEvent* evt);

    void setTextSyntax(int sel);
    void setTextEncryption(int sel);
    void setTextEncoding(int sel);
    void populateGameList();

private slots:
    void onOpenFile();
    void onSaveFile();
    void onSaveAs();
    void onRevert();
    void onOptions();

    void onCut();
    void onCopy();
    void onPaste();
    void onDelete();
    void onSelectAll();
    void onUndo();
    void onRedo();

    void onTextCollapseAll();
    void onTextExpandAll();
    void onTextStxNone();
    void onTextStxPython();
    void onTextStxSDL();
    void onTextStxIni();
    void onTextStxConsole();
    void onTextStxXML();
    void onTextStxHex();
    void onTextStxFX();
    void onTextEncNone();
    void onTextEncXtea();
    void onTextEncAes();
    void onTextEncDroid();
    void onTextTypeAnsi();
    void onTextTypeUTF8();
    void onTextTypeUTF16();
    void onTextTypeUTF32();

    void onCloseTab(int idx);
    void onChangeTab(int idx);
    void onSelectGame(int gameId);
    void onBrowserItemActivated(QTreeWidgetItem* item, int);
    void updateMenuStatus();
    void onDocDirty();
    void onDocClean();
};

#endif
