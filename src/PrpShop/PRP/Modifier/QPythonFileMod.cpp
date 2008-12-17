#include "QPythonFileMod.h"

#include <QLabel>
#include <QMenu>
#include <QContextMenuEvent>
#include <QGridLayout>
#include "../../Main.h"

static const QString s_PythonParamTypes[] = {
    "(Invalid)", "Integer", "Float", "Boolean", "String", "Scene Object",
    "Scene Object List", "Activator", "Responder", "Dynamic Text", "GUI Dialog",
    "Exclude Region", "Animation", "Animation Name", "Behavior", "Material",
    "GUI Pop-Up Menu", "GUI Skin", "Water Component", "Swim Current Interface",
    "Cluster Component", "Material Animation", "Grass Shader Component",
    "Global SDL Variable", "Subtitle", "Blower Component", "None"
};

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

    QObject::connect(this, SIGNAL(itemActivated(QTreeWidgetItem*, int)),
                     this, SLOT(handleActivate(QTreeWidgetItem*, int)));
}

QSize QPythonParamList::sizeHint() const
{
    QSize treeSize = QTreeWidget::sizeHint();
    return QSize((treeSize.width() * 4) / 3, treeSize.height());
}

void QPythonParamList::addParam(const plPythonParameter& param)
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
        row << param.fStrValue.cstr();
    else if (param.fValueType == plPythonParameter::kNone)
        row << "(Null)";
    else
        row << param.fObjKey->getName().cstr();
    new QTreeWidgetItem(this, row);
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
    QAction* delObjItem = menu.addAction(tr("Remove Parameter"));

    if (currentItem() == NULL)
        delObjItem->setEnabled(false);

    QAction* sel = menu.exec(evt->globalPos());
    if (sel == addObjItem) {
        // ...
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

    fFileName = new QLineEdit(mod->getFilename().cstr(), this);

    fReceivers = new QKeyList(this);
    for (size_t i=0; i<mod->getNumReceivers(); i++)
        fReceivers->addKey(mod->getReceiver(i));

    fParams = new QPythonParamList(this);
    for (size_t i=0; i<mod->getNumParameters(); i++)
        fParams->addParam(mod->getParameter(i));

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
    mod->setFilename(fFileName->text().toUtf8().data());

    mod->clearReceivers();
    QList<plKey> recs = fReceivers->keys();
    while (!recs.isEmpty())
        mod->addReceiver(recs.takeFirst());

    mod->clearParameters();
    QList<plPythonParameter> params = fParams->params();
    while (!params.isEmpty())
        mod->addParameter(params.takeFirst());
}
