#ifndef _PLASMASHOP_MAIN_H
#define _PLASMASHOP_MAIN_H

#include <QMainWindow>
#include <QTreeView>
#include <QTabWidget>
#include <QDockWidget>
#include <QAction>

#define PLASMASHOP_VERSION "3.0 (build 79)"

class PlasmaShopMain : public QMainWindow {
    Q_OBJECT

private:
    QString fDialogDir;
    QTabWidget* fEditorPane;
    QDockWidget* fBrowserDock;
    QTreeView* fBrowserTree;

    // Menu actions
    enum {
        // Main Menu
        kFileNew, kFileOpen, kFileSave, kFileSaveAs, kFileExit, kFileOptions,
        kWindowPrev, kWindowNext, kWindowTile, kWindowCascade, kWindowClose,
        kWindowCloseAll,

        // Text Editor Menu
        kTextFind, kTextReplace, kTextStxNone, kTextStxPython, kTextStxSDL1,
        kTextStxSDL2, kTextStxIni, kTextStxConsole, kTextStxXML, kTextStxHex,
        kTextStxFX, kTextEncNone, kTextEncXtea, kTextEncAes, kTextEncDroid,
        kTextTypeAnsi, kTextTypeUTF8, kTextTypeUTF16, kTextTypeUTF32,
        kTextExpandAll, kTextCollapseAll,

        // Tree Context Menu
        kTreeOpen, kTreeRename, kTreeDelete,

        kNumActions
    };
    QAction* fActions[kNumActions];

    // Magic for Creatable loading
    static PlasmaShopMain* sInstance;

public:
    static PlasmaShopMain* Instance();

    PlasmaShopMain();

protected:
    virtual void closeEvent(QCloseEvent* evt);
};

#endif
