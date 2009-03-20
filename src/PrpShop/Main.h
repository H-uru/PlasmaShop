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

#define PRPSHOP_VERSION "1.0 (build 72)"

class PrpShopMain : public QMainWindow {
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

    enum {
        kLocLocalOnly, kLocVolatile, kLocItinerant, kLocReserved, kLocBuiltIn,
        kLocNumFlags
    };
    QCheckBox* fLocationFlags[kLocNumFlags];

    // Menu actions
    enum {
        // Main Menu
        kFileNewPage, kFileOpen, kFileSave, kFileSaveAs, kFileExit,
        kToolsProperties, kToolsNewObject, kWindowPrev, kWindowNext,
        kWindowTile, kWindowCascade, kWindowClose, kWindowCloseAll,

        // Tree Context Menu
        kTreeClose, kTreeEdit, kTreePreview, kTreeDelete, kTreeImport,
        kTreeExport, kTreeExportDDS, kTreeExportJPEG, kTreeExportOBJ,

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
    void editCreatable(plCreatable* pCre, short forceType = -1);

protected:
    virtual void closeEvent(QCloseEvent* evt);
    QPlasmaTreeItem* loadPage(plPageInfo* page, QString filename);
    QPlasmaTreeItem* findCurrentPageItem(bool isSave = false);
    QPlasmaTreeItem* ensurePath(const plLocation& loc, short objType);

public slots:
    void openFiles();
    void performSave();
    void performSaveAs();
    void treeItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous);
    void treeItemActivated(QTreeWidgetItem* item, int column);
    void treeContextMenu(const QPoint& pos);
    void createNewObject();

    void treeClose();
    void treeEdit();
    void treePreview();
    void treeDelete();
    void treeImport();
    void treeExport();
    void treeExportDDS();
    void treeExportJPEG();
    void treeExportOBJ();
};

#endif
