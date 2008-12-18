#ifndef _QPYTHONFILEMOD_H
#define _QPYTHONFILEMOD_H

#include "../QCreatable.h"

#include <PRP/Modifier/plPythonFileMod.h>
#include <QLineEdit>
#include <QDialog>
#include <QComboBox>
#include <QSpinBox>
#include <QLabel>
#include "../QObjLink.h"
#include "../QKeyList.h"

class QPythonParamList : public QTreeWidget {
protected:
    Q_OBJECT
    QList<plPythonParameter> fParams;

public:
    QPythonParamList(QWidget* parent = NULL);
    virtual QSize sizeHint() const;

    void addParam(const plPythonParameter& param);
    void delParam(int idx);
    QList<plPythonParameter> params() const;

    void adjustColumns();

protected slots:
    void handleActivate(QTreeWidgetItem* item, int column);

protected:
    virtual void contextMenuEvent(QContextMenuEvent* evt);
};

class QPythonFileMod : public QCreatable {
protected:
    Q_OBJECT
    QCreatableLink* fSynchObjLink;
    QLineEdit* fFileName;
    QKeyList* fReceivers;
    QPythonParamList* fParams;

public:
    QPythonFileMod(plCreatable* pCre, QWidget* parent = NULL);
    virtual void saveDamage();
};

class QPythonParamDialog : public QDialog {
protected:
    Q_OBJECT
    QComboBox* fTypeBox;
    QSpinBox* fID;
    QLinkLabel* fKeyValue;
    QLineEdit* fStringValue;
    QLineEdit* fIntValue;
    QLineEdit* fFloatValue;
    QComboBox* fBoolValue;
    QLabel* fLabelNull;
    plWeakKey fKey;

public:
    QPythonParamDialog(QWidget* parent = NULL);
    void init(const plPythonParameter& param);

    plPythonParameter parameter() const;

protected slots:
    void selectKey();
    void typeChanged(int);
};

#endif
