#ifndef _QDRAWINTERFACE_H
#define _QDRAWINTERFACE_H

#include "../QCreatable.h"

#include <PRP/Object/plDrawInterface.h>
#include <QCheckBox>
#include <QSpinBox>
#include <QComboBox>
#include <QDialog>
#include "../QKeyList.h"
#include "../QObjLink.h"

class QDrawableList : public QKeyList {
    Q_OBJECT

protected:
    QList<int> fDrawKeys;
    plLocation fLocation;

public:
    QDrawableList(const plLocation& loc, QWidget* parent = NULL);

    void addKey(plKey key, int dkey);
    QList<int> drawKeys() const;

protected:
    virtual void contextMenuEvent(QContextMenuEvent* evt);
};

class QFindDrawKeyDialog : public QDialog {
    Q_OBJECT

protected:
    QComboBox* fKeyBox;
    QSpinBox* fDrawKey;
    plResManager* fResMgr;
    plLocation fLocation;
    std::vector<plKey> fKeys;

public:
    QFindDrawKeyDialog(QWidget* parent = NULL);
    void init(plResManager* mgr, const plLocation& loc);

    plKey selection() const;
    int drawKey() const;
};

class QDrawInterface : public QCreatable {
    Q_OBJECT

protected:
    QCreatableLink* fOwnerLink;
    QCreatableLink* fSynchObjLink;
    QCheckBox* fDisabled;
    QDrawableList* fDrawKeys;
    QKeyList* fRegions;

public:
    QDrawInterface(plCreatable* pCre, QWidget* parent = NULL);
    virtual void saveDamage();

protected slots:
    void setOwner();
    void unsetOwner();
};

#endif
