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

#ifndef _QPYTHONFILEMOD_H
#define _QPYTHONFILEMOD_H

#include "../QCreatable.h"

#include <PRP/Modifier/plPythonFileMod.h>
#include <QDialog>
#include <QComboBox>
#include <QSpinBox>
#include <QLabel>
#include "../QObjLink.h"
#include "../QKeyList.h"

class QPythonParamList : public QTreeWidget {
    Q_OBJECT

protected:
    QList<plPythonParameter> fParams;

public:
    QPythonParamList(QWidget* parent = NULL);
    virtual QSize sizeHint() const;

    void addParam(const plPythonParameter& param);
    void delParam(int idx);
    QList<plPythonParameter> params() const { return fParams; }

    void adjustColumns();

protected slots:
    void handleActivate(QTreeWidgetItem* item, int column);

protected:
    virtual void contextMenuEvent(QContextMenuEvent* evt);
};

class QPythonFileMod : public QCreatable {
    Q_OBJECT

protected:
    QCreatableLink* fSynchObjLink;
    QLineEdit* fFileName;
    QKeyList* fReceivers;
    QPythonParamList* fParams;

public:
    QPythonFileMod(plCreatable* pCre, QWidget* parent = NULL);
    virtual void saveDamage();
};

class QPythonParamDialog : public QDialog {
    Q_OBJECT

protected:
    QComboBox* fTypeBox;
    QSpinBox* fID;
    QLinkLabel* fKeyValue;
    QLineEdit* fStringValue;
    QIntEdit* fIntValue;
    QFloatEdit* fFloatValue;
    QComboBox* fBoolValue;
    QLabel* fLabelNull;
    plKey fKey;

public:
    QPythonParamDialog(QWidget* parent = NULL);
    void init(const plPythonParameter& param);

    plPythonParameter parameter() const;

protected slots:
    void selectKey();
    void typeChanged(int);
};

#endif
