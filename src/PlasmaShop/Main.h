#ifndef _PLASMASHOP_MAIN_H
#define _PLASMASHOP_MAIN_H

#include <QMainWindow>
#include <QTreeView>
#include <QTabWidget>
#include <QDockWidget>
#include <QAction>
#include "QPlasmaDocument.h"

#define PLASMASHOP_VERSION "3.0 (build 80)"

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
        kEditUndo, kEditRedo, kEditCut, kEditCopy, kEditPaste, kEditRevert,
        kHelpAbout,

        // Text Editor Menu
        kTextFind, kTextFindNext, kTextFindPrev, kTextReplace,
        kTextStxNone, kTextStxPython, kTextStxSDL1, kTextStxSDL2, kTextStxIni,
        kTextStxConsole, kTextStxXML, kTextStxHex, kTextStxFX,
        kTextEncNone, kTextEncXtea, kTextEncAes, kTextEncDroid,
        kTextTypeAnsi, kTextTypeUTF8, kTextTypeUTF16, kTextTypeUTF32,
        kTextExpandAll, kTextCollapseAll,

        // Tree Context Menu
        kTreeOpen, kTreeRename, kTreeDelete,

        kNumActions
    };
    QAction* fActions[kNumActions];

public:
    PlasmaShopMain();
    void loadFile(QString filename);

protected:
    virtual void closeEvent(QCloseEvent* evt);

private slots:
    void onOpenFile();
    void onSaveFile();
    void onSaveAs();
    void onCloseTab(int idx);
};

#endif
