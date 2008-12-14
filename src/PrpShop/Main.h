#ifndef _PLASMASHOP_MAIN_H
#define _PLASMASHOP_MAIN_H

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

#define PRPSHOP_VERSION "1.0 (build 35)"

class PrpShopMain : public QMainWindow {
private:
    Q_OBJECT
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
        kFileNewPage, kFileOpen, kFileSave, kFileSaveAs, kFileClose, kFileExit,
        kViewProperties, kWindowPrev, kWindowNext, kWindowTile, kWindowCascade,
        kWindowClose, kWindowCloseAll, kNumActions
    };
    QAction* fActions[kNumActions];

    // libPlasma stuff
    plResManager fResMgr;
    QHash<plLocation, QPlasmaTreeItem*> fLoadedLocations;

    // Magic for Creatable loading
    static PrpShopMain* sInstance;

public:
    static PrpShopMain* Instance();

    PrpShopMain();
    void setPropertyPage(PropWhich which);
    void loadFile(const QString& filename);
    void editCreatable(plCreatable* pCre);

protected:
    virtual void closeEvent(QCloseEvent* evt);
    QPlasmaTreeItem* loadPage(plPageInfo* page, const QString& filename);

public slots:
    void openFiles();
    void treeItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous);
    void treeItemActivated(QTreeWidgetItem* item, int column);
};

#endif
