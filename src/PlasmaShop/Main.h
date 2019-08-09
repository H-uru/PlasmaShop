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
#include "GameScanner.h"

class PlasmaShopMain : public QMainWindow
{
    Q_OBJECT

private:
    QString fDialogDir;
    QTabWidget* fEditorPane;
    QDockWidget* fBrowserDock;

    // Menu actions
    enum
    {
        // Main Menu
        kFileNew, kFileOpen, kFileSave, kFileSaveAs, kFileExit, kFileOptions,
        kFileRevert, kFileShowBrowser,
        kEditUndo, kEditRedo, kEditCut, kEditCopy, kEditPaste, kEditDelete,
        kEditSelectAll,
        kHelpAbout,

        // Text Editor Menu
        kTextFind, kTextFindNext, kTextReplace,
        kTextStxNone, kTextStxPython, kTextStxSDL, kTextStxIni,
        kTextStxConsole, kTextStxXML, kTextStxHex, kTextStxFX,
        kTextTypeAnsi, kTextTypeUTF8, kTextTypeUTF16, kTextTypeUTF32,

        // Generic Encryption options
        kGenEncNone, kGenEncXtea, kGenEncAes, kGenEncDroid,

        // Tree Context Menu
        kTreeOpen, kTreeRename, kTreeDelete,

        kNumActions
    };
    QAction* fActions[kNumActions];

    // Editor-specific menus (for dynamically hiding)
    QMenu* fTextMenu;
    QMenu* fEncryptMenu;
    QMenu* fHelpMenu;

    // Game Browser
    QTreeWidget* fBrowserTree;
    QComboBox* fGameSelector;
    QList<GameInfo> fGames;
    int fCurrentGame;
    GameScanner* fScanner;

public:
    PlasmaShopMain();
    virtual ~PlasmaShopMain();
    void loadFile(QString filename);

protected:
    virtual void closeEvent(QCloseEvent* evt);
    virtual void dragEnterEvent(QDragEnterEvent* evt);
    virtual void dropEvent(QDropEvent* evt);

    void setTextSyntax(int sel);
    void setTextEncoding(int sel);
    void setEncryption(int sel);
    void populateGameList();

private slots:
    void onNewFile();
    void onOpenFile();
    void onSaveFile();
    void onSaveAs();
    void onRevert();
    void onOptions();
    void onShowAbout();

    void onCut();
    void onCopy();
    void onPaste();
    void onDelete();
    void onSelectAll();
    void onUndo();
    void onRedo();

    void onTextFind();
    void onTextFindNext();
    void onTextReplace();
    void onTextStxNone();
    void onTextStxPython();
    void onTextStxSDL();
    void onTextStxIni();
    void onTextStxConsole();
    void onTextStxXML();
    void onTextStxHex();
    void onTextStxFX();
    void onTextTypeAnsi();
    void onTextTypeUTF8();
    void onTextTypeUTF16();
    void onTextTypeUTF32();

    void onGenEncNone();
    void onGenEncXtea();
    void onGenEncAes();
    void onGenEncDroid();

    void onCloseTab(int idx);
    void onChangeTab(int idx);
    void onSelectGame(int gameId);
    void onBrowserItemActivated(QTreeWidgetItem* item, int);
    void updateMenuStatus();
    void onDocDirty();
    void onDocClean();
};

#endif
