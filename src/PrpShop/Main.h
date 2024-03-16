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

#ifndef _PRPSHOP_MAIN_H
#define _PRPSHOP_MAIN_H

#include <QMainWindow>
#include <QMdiArea>
#include <QTreeWidget>
#include <QDockWidget>
#include <QGroupBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QCheckBox>
#include <QLabel>
#include <QAction>

#include <ResManager/plResManager.h>
#include "QPlasmaTreeItem.h"
#include "QPlasmaUtils.h"

class QCreatable;

class PrpShopMain : public QMainWindow
{
    Q_OBJECT

private:
    QString fDialogDir;
    QMdiArea* fMdiArea;
    QDockWidget* fBrowserDock;
    QTreeWidget* fBrowserTree;

    // Property Panel stuff
    enum PropWhich { kPropsNone, kPropsAge, kPropsPage, kPropsKO };
    QDockWidget* fPropertyDock;
    QWidget* fPropertyContainer;

    QLineEdit* fAgeName;
    QLineEdit* fPageName;
    QSpinBox* fReleaseVersion;
    QSpinBox* fSeqPrefix;
    QSpinBox* fSeqSuffix;
    QLineEdit* fObjName;
    QLabel* fObjType;
    QSpinBox* fLoadMaskQ[2];
    QGroupBox* fCloneIdBox;
    QSpinBox* fCloneId;
    QSpinBox* fClonePlayerId;

    enum
    {
        kLocLocalOnly, kLocVolatile, kLocItinerant, kLocReserved, kLocBuiltIn,
        kLocNumFlags
    };
    QCheckBox* fLocationFlags[kLocNumFlags];

    // Menu actions
    enum
    {
        // Main Menu
        kFileNewPage, kFileOpen, kFileSave, kFileSaveAs, kFileExit,
        kToolsProperties, kToolsShowAgePageIDs, kToolsShowTypeIDs, kToolsNewObject,
        kWindowPrev, kWindowNext, kWindowTile, kWindowCascade, kWindowClose, kWindowCloseAll,

        // Tree Context Menu
        kTreeClose, kTreeEdit, kTreeEditPRC, kTreeEditHex, kTreePreview,
        kTreeViewTargets, kTreeDelete, kTreeImport, kTreeExport, kTreeNewObject,

        kNumActions
    };
    QAction* fActions[kNumActions];

    // libPlasma stuff
    plResManager fResMgr;
    QHash<plLocation, QPlasmaTreeItem*> fLoadedLocations;

    // Magic for Creatable loading
    static PrpShopMain* sInstance;

public:
    static PrpShopMain* Instance();
    static plResManager* ResManager();

    PrpShopMain();
    void setPropertyPage(PropWhich which);
    void loadFile(QString filename);
    void saveFile(plPageInfo* page, QString filename);
    void saveProps(QPlasmaTreeItem* item);
    QCreatable* editCreatable(plCreatable* pCre, int forceType = -1);

protected:
    void closeEvent(QCloseEvent* evt) override;
    void dragEnterEvent(QDragEnterEvent* evt) override;
    void dropEvent(QDropEvent* evt) override;
    QPlasmaTreeItem* loadPage(plPageInfo* page, QString filename);
    QPlasmaTreeItem* findCurrentPageItem(bool isSave = false);
    void addNewObjectToTree(const hsKeyedObject* ko);

public slots:
    void newPage();
    void openFiles();
    void performSave();
    void performSaveAs();
    void treeItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous);
    void treeItemActivated(QTreeWidgetItem* item, int column);
    void treeContextMenu(const QPoint& pos);
    void createNewObject();
    void showAgePageIDs(bool show);
    void showTypeIDs(bool show);
    void closeWindows(const plLocation& loc);

    void treeClose();
    void treeEdit();
    void treeEditPRC();
    void treeEditHex();
    void treePreview();
    void treeShowTargets();
    void treeDelete();
    void treeImport();
    void treeExport();
};

#endif
