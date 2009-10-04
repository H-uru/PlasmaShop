#ifndef _PLASMASHOP_MAIN_H
#define _PLASMASHOP_MAIN_H

#include <QMainWindow>
#include <QTreeView>
#include <QTabWidget>
#include <QDockWidget>
#include <QAction>
#include <QMenu>
#include "QPlasmaDocument.h"

#define PLASMASHOP_VERSION "3.0 (build 83)"

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

public:
    PlasmaShopMain();
    void loadFile(QString filename);

protected:
    virtual void closeEvent(QCloseEvent* evt);

    void setTextSyntax(int sel);
    void setTextEncryption(int sel);
    void setTextEncoding(int sel);

private slots:
    void onOpenFile();
    void onSaveFile();
    void onSaveAs();
    void onRevert();

    void onCut();
    void onCopy();
    void onPaste();
    void onDelete();
    void onSelectAll();
    void onUndo();
    void onRedo();

    void onCloseTab(int idx);
    void onChangeTab(int idx);
    void updateMenuStatus();
};

#endif
