#ifndef _QGUIPOPUPMENU_H
#define _QGUIPOPUPMENU_H

#include "../QCreatable.h"

#include <PRP/GUI/pfGUIPopUpMenu.h>
#include <QCheckBox>
#include <QTreeWidget>
#include <QComboBox>
#include <QDialog>
#include "../QObjLink.h"

class QPopupMenuItemList : public QTreeWidget {
    Q_OBJECT

public:
    QPopupMenuItemList(QWidget* parent = NULL);
    virtual QSize sizeHint() const;

    void addItem(const pfGUIPopUpMenu::pfMenuItem& item);
    void delItem(int idx);

    void adjustColumns();

signals:
    void requestNewItem();
    void requestDelItem(int idx);
    void requestEditItem(int idx);

public slots:
    void handleUpdate(int idx, QString name, bool hasSubMenu);

protected:
    virtual void contextMenuEvent(QContextMenuEvent* evt);

private slots:
    void handleActivate(QTreeWidgetItem* item, int column);
};


class QGUIPopUpMenu : public QCreatable {
    Q_OBJECT

protected:
    enum {
        kModFlagStart = pfGUIPopUpMenu::kStayOpenAfterClick,
        kModFlagEnd = pfGUIPopUpMenu::kScaleWithResolution,
        kModFlagCount = (kModFlagEnd - kModFlagStart) + 1
    };

    QCreatableLink* fDialogModLink;
    QCheckBox* fModFlags[kModFlagCount];
    QPopupMenuItemList* fMenuItems;
    QCreatableLink* fSkin;
    QCreatableLink* fOriginContext;
    QCreatableLink* fOriginAnchor;
    QIntEdit* fMargin;
    QComboBox* fAlignment;

public:
    QGUIPopUpMenu(plCreatable* pCre, QWidget* parent = NULL);
    virtual void saveDamage();

private slots:
    void setSkin();
    void setOriginContext();
    void setOriginAnchor();
    void unsetSkin();
    void unsetOriginContext();
    void unsetOriginAnchor();

    void addMenuItem();
    void editMenuItem(int idx);
    void delMenuItem(int idx);
};


class QPopUpMenuItemDialog : public QDialog {
    Q_OBJECT

protected:
    QLineEdit* fName;
    QComboBox* fProcType;
    QLineEdit* fProcCommand;
    QLabel* fProcCommandLabel;
    QLinkLabel* fSubMenuKey;
    QFloatEdit* fYOffsetToNext;
    pfGUIPopUpMenu::pfMenuItem* fItem;
    int fItemIdx;

public:
    QPopUpMenuItemDialog(QWidget* parent);
    void init(pfGUIPopUpMenu::pfMenuItem* item, int idx);

signals:
    void updateItem(int idx, QString name, bool hasSubMenu);

private slots:
    void setProcType(int typeIdx);
    void selectKey();
    void nameChanged(QString name);
    void cmdChanged(QString cmd);
    void offsetChanged(QString);
};

#endif
