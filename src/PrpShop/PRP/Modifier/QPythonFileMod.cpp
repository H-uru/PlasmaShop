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

#include "QPythonFileMod.h"

#include <QMenu>
#include <QContextMenuEvent>
#include <QDialogButtonBox>
#include <QGridLayout>
#include "../../Main.h"
#include "../../QKeyDialog.h"

static const QString s_PythonParamTypes[] = {
    "(Invalid)", "Integer", "Float", "Boolean", "String", "Scene Object",
    "Scene Object List", "Activator", "Responder", "Dynamic Text", "GUI Dialog",
    "Exclude Region", "Animation", "Animation Name", "Behavior", "Material",
    "GUI Pop-Up Menu", "GUI Skin", "Water Component", "Swim Current Interface",
    "Cluster Component", "Material Animation", "Grass Shader Component",
    "Global SDL Variable", "Subtitle", "Blower Component", "None"
};

static QStringList makeParamItem(const plPythonParameter& param)
{
    QStringList row;
    row << QString("%1").arg(param.fID) << s_PythonParamTypes[param.fValueType];
    if (param.fValueType == plPythonParameter::kInt)
        row << QString("%1").arg(param.fIntValue);
    else if (param.fValueType == plPythonParameter::kBoolean)
        row << (param.fBoolValue ? "true" : "false");
    else if (param.fValueType == plPythonParameter::kFloat)
        row << QString("%1").arg(param.fFloatValue);
    else if (param.fValueType == plPythonParameter::kString ||
             param.fValueType == plPythonParameter::kAnimationName ||
             param.fValueType == plPythonParameter::kGlobalSDLVar ||
             param.fValueType == plPythonParameter::kSubtitle)
        row << ~param.fStrValue;
    else if (param.fValueType == plPythonParameter::kNone)
        row << "(Null)";
    else
        row << ~param.fObjKey->getName();
    return row;
}

/* QPythonParamList */
QPythonParamList::QPythonParamList(QWidget* parent)
                : QTreeWidget(parent)
{
    setColumnCount(3);
    setUniformRowHeights(true);
    setRootIsDecorated(false);
    headerItem()->setText(0, tr("Id"));
    headerItem()->setText(1, tr("Type"));
    headerItem()->setText(2, tr("Value"));

    connect(this, SIGNAL(itemActivated(QTreeWidgetItem*, int)),
            this, SLOT(handleActivate(QTreeWidgetItem*, int)));
}

QSize QPythonParamList::sizeHint() const
{
    QSize treeSize = QTreeWidget::sizeHint();
    return QSize((treeSize.width() * 4) / 3, treeSize.height());
}

void QPythonParamList::addParam(const plPythonParameter& param)
{
    new QTreeWidgetItem(this, makeParamItem(param));
    fParams << param;
}

void QPythonParamList::delParam(int idx)
{
    QTreeWidgetItem* item = takeTopLevelItem(idx);
    delete item;
    fParams.erase(fParams.begin() + idx);
}

QList<plPythonParameter> QPythonParamList::params() const
{ return fParams; }

void QPythonParamList::adjustColumns()
{
    resizeColumnToContents(0);
    resizeColumnToContents(2);
    resizeColumnToContents(1);
}

void QPythonParamList::handleActivate(QTreeWidgetItem* item, int)
{
    plPythonParameter param(fParams[indexOfTopLevelItem(item)]);
    if (param.fObjKey.Exists() && param.fObjKey.isLoaded())
        PrpShopMain::Instance()->editCreatable(param.fObjKey->getObj());
}

void QPythonParamList::contextMenuEvent(QContextMenuEvent* evt)
{
    QMenu menu(this);
    QAction* addObjItem = menu.addAction(tr("Add Parameter..."));
    QAction* editObjItem = menu.addAction(tr("Edit Parameter..."));
    QAction* delObjItem = menu.addAction(tr("Remove Parameter"));

    if (currentItem() == NULL)
        delObjItem->setEnabled(false);

    QAction* sel = menu.exec(evt->globalPos());
    if (sel == addObjItem) {
        QPythonParamDialog dlg(this);
        if (dlg.exec() == QDialog::Accepted)
            addParam(dlg.parameter());
    } else if (sel == editObjItem) {
        QPythonParamDialog dlg(this);
        dlg.init(fParams[indexOfTopLevelItem(currentItem())]);
        if (dlg.exec() == QDialog::Accepted) {
            plPythonParameter param = dlg.parameter();
            fParams[indexOfTopLevelItem(currentItem())] = param;
            QStringList list = makeParamItem(param);
            for (int i=0; i<list.size(); i++)
                currentItem()->setText(i, list[i]);
        }
    } else if (sel == delObjItem) {
        delParam(indexOfTopLevelItem(currentItem()));
    }
}


/* QPythonFileMod */
QPythonFileMod::QPythonFileMod(plCreatable* pCre, QWidget* parent)
              : QCreatable(pCre, kPythonFileMod, parent)
{
    plPythonFileMod* mod = (plPythonFileMod*)fCreatable;

    fSynchObjLink = new QCreatableLink(this, false);
    fSynchObjLink->setText(tr("Synch Flags"));
    fSynchObjLink->setCreatable(mod);
    fSynchObjLink->setForceType(kSynchedObject);

    fFileName = new QLineEdit(~mod->getFilename(), this);

    fReceivers = new QKeyList(this);
    for (size_t i=0; i<mod->getReceivers().getSize(); i++)
        fReceivers->addKey(mod->getReceivers()[i]);

    fParams = new QPythonParamList(this);
    for (size_t i=0; i<mod->getParameters().getSize(); i++)
        fParams->addParam(mod->getParameters()[i]);

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(fSynchObjLink, 0, 0, 1, 2);
    layout->addWidget(new QLabel(tr("Python File:"), this), 1, 0, 1, 1);
    layout->addWidget(fFileName, 1, 1, 1, 1);
    layout->addWidget(new QLabel(tr("Receivers:"), this), 2, 0, 1, 2);
    layout->addWidget(fReceivers, 3, 0, 1, 2);
    layout->addWidget(new QLabel(tr("Parameters:"), this), 4, 0, 1, 2);
    layout->addWidget(fParams, 5, 0, 1, 2);
    fReceivers->adjustColumns();
    fParams->adjustColumns();
}

void QPythonFileMod::saveDamage()
{
    plPythonFileMod* mod = (plPythonFileMod*)fCreatable;
    mod->setFilename(~fFileName->text());

    mod->clearReceivers();
    QList<plKey> recs = fReceivers->keys();
    while (!recs.isEmpty())
        mod->addReceiver(recs.takeFirst());

    mod->clearParameters();
    QList<plPythonParameter> params = fParams->params();
    while (!params.isEmpty())
        mod->addParameter(params.takeFirst());
}


/* QPythonParamDialog */
QPythonParamDialog::QPythonParamDialog(QWidget* parent)
                  : QDialog(parent)
{
    setWindowTitle(tr("Python Parameter"));

    fTypeBox = new QComboBox(this);
    fID = new QSpinBox(this);
    fID->setRange(0, 0x7FFFFFFF);
    fKeyValue = new QLinkLabel("(Null)", this);
    fStringValue = new QLineEdit(this);
    fIntValue = new QIntEdit(this);
    fFloatValue = new QFloatEdit(this);
    fBoolValue = new QComboBox(this);
    fBoolValue->addItem("false");
    fBoolValue->addItem("true");
    fLabelNull = new QLabel("(Null)", this);
    QDialogButtonBox* buttonBox = new QDialogButtonBox(Qt::Horizontal, this);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel |
                                  QDialogButtonBox::Ok);

    fKeyValue->setVisible(false);
    fStringValue->setVisible(false);
    fIntValue->setVisible(false);
    fFloatValue->setVisible(false);
    fBoolValue->setVisible(false);

    for (int i=1; i<=plPythonParameter::kNone; i++)
        fTypeBox->addItem(s_PythonParamTypes[i]);

    QGridLayout* layout = new QGridLayout(this);
    layout->setHorizontalSpacing(8);
    layout->setVerticalSpacing(8);
    layout->addWidget(new QLabel(tr("ID:"), this), 0, 0);
    layout->addWidget(fID, 0, 1, 1, 2);
    layout->addWidget(new QLabel(tr("Type:"), this), 1, 0);
    layout->addWidget(fTypeBox, 1, 1, 1, 2);
    layout->addWidget(new QLabel(tr("Value:"), this), 2, 0);
    layout->addWidget(fKeyValue, 2, 1);
    layout->addItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding), 2, 2);
    layout->addWidget(fStringValue, 2, 1, 1, 2);
    layout->addWidget(fIntValue, 2, 1, 1, 2);
    layout->addWidget(fFloatValue, 2, 1, 1, 2);
    layout->addWidget(fBoolValue, 2, 1, 1, 2);
    layout->addWidget(fLabelNull, 2, 1, 1, 2);
    layout->addWidget(buttonBox, 3, 0, 1, 3);

    connect(fTypeBox, SIGNAL(currentIndexChanged(int)), this, SLOT(typeChanged(int)));
    connect(fKeyValue, SIGNAL(activated()), this, SLOT(selectKey()));
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    fTypeBox->setCurrentIndex(plPythonParameter::kNone-1);
}

void QPythonParamDialog::init(const plPythonParameter& param)
{
    fID->setValue(param.fID);
    fTypeBox->setCurrentIndex(param.fValueType-1);

    if (param.fValueType == plPythonParameter::kInt) {
        fIntValue->setValue(param.fIntValue);
    } else if (param.fValueType == plPythonParameter::kBoolean) {
        fBoolValue->setCurrentIndex(param.fBoolValue ? 1 : 0);
    } else if (param.fValueType == plPythonParameter::kFloat) {
        fFloatValue->setValue(param.fFloatValue);
    } else if (param.fValueType == plPythonParameter::kString ||
             param.fValueType == plPythonParameter::kAnimationName ||
             param.fValueType == plPythonParameter::kGlobalSDLVar ||
             param.fValueType == plPythonParameter::kSubtitle) {
        fStringValue->setText(~param.fStrValue);
    } else if (param.fValueType == plPythonParameter::kNone) {
        // Do nothing
    } else {
        fKey = param.fObjKey;
        if (fKey.Exists())
            fKeyValue->setText(~fKey->getName());
        else
            fKeyValue->setText("(Null)");
    }
}

plPythonParameter QPythonParamDialog::parameter() const
{
    plPythonParameter param;
    param.fID = fID->text().toUInt();
    param.fValueType = fTypeBox->currentIndex() + 1;

    if (param.fValueType == plPythonParameter::kInt) {
        param.fIntValue = fIntValue->value();
    } else if (param.fValueType == plPythonParameter::kBoolean) {
        param.fBoolValue = fBoolValue->currentIndex() != 0;
    } else if (param.fValueType == plPythonParameter::kFloat) {
        param.fFloatValue = fFloatValue->value();
    } else if (param.fValueType == plPythonParameter::kString ||
             param.fValueType == plPythonParameter::kAnimationName ||
             param.fValueType == plPythonParameter::kGlobalSDLVar ||
             param.fValueType == plPythonParameter::kSubtitle) {
        param.fStrValue = ~fStringValue->text();
    } else if (param.fValueType == plPythonParameter::kNone) {
        // Do nothing
    } else {
        param.fObjKey = fKey;
    }
    return param;
}

void QPythonParamDialog::selectKey()
{
    QFindKeyDialog dlg(this);
    dlg.init(PrpShopMain::ResManager(), fKey);
    if (dlg.exec() == QDialog::Accepted) {
        fKey = dlg.selection();
        fKeyValue->setText(~fKey->getName());
    }
}

void QPythonParamDialog::typeChanged(int type)
{
    fKeyValue->setVisible(false);
    fStringValue->setVisible(false);
    fIntValue->setVisible(false);
    fFloatValue->setVisible(false);
    fBoolValue->setVisible(false);
    fLabelNull->setVisible(false);

    type++;
    if (type == plPythonParameter::kInt) {
        fIntValue->setVisible(true);
    } else if (type == plPythonParameter::kBoolean) {
        fBoolValue->setVisible(true);
    } else if (type == plPythonParameter::kFloat) {
        fFloatValue->setVisible(true);
    } else if (type == plPythonParameter::kString ||
               type == plPythonParameter::kAnimationName ||
               type == plPythonParameter::kGlobalSDLVar ||
               type == plPythonParameter::kSubtitle) {
        fStringValue->setVisible(true);
    } else if (type == plPythonParameter::kNone) {
        fLabelNull->setVisible(true);
    } else {
        fKeyValue->setVisible(true);
    }
}
