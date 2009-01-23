#include "QVaultSDLNode.h"

#include <QLabel>
#include <QMessageBox>
#include <QDateTime>
#include <Vault/plVaultNodeTypes.h>
#include <Stream/hsRAMStream.h>
#include <Debug/plDebug.h>
#include <ResManager/pdUnifiedTypeMap.h>

/* QSDLTreeItem */
class QSDLTreeItem : public QTreeWidgetItem {
public:
    enum { kSDLVarType = UserType };

private:
    plStateVariable* fVariable;

public:
    QSDLTreeItem(plStateVariable* var, QTreeWidget* parent)
        : QTreeWidgetItem(parent, kSDLVarType)
    {
        set(var);
    }

    QSDLTreeItem(plStateVariable* var, QTreeWidgetItem* parent)
        : QTreeWidgetItem(parent, kSDLVarType)
    {
        set(var);
    }

    plStateVariable* variable() const
    {
        return fVariable;
    }

    void set(plStateVariable* var)
    {
        fVariable = var;
        QTreeWidgetItem* child;
        while ((child = takeChild(0)) != NULL)
            delete child;

        setText(0, ~fVariable->getDescriptor()->getName());
        setText(1, QSDLEditor::GetVarDisplay(var));
        if (fVariable->getDescriptor()->getType() == plVarDescriptor::kStateDescriptor) {
            if (fVariable->getCount() > 1) {
                for (size_t i=0; i<fVariable->getCount(); i++) {
                    QTreeWidgetItem* idx = new QTreeWidgetItem(this,
                        QStringList() << QString("Item [%1]").arg(i)
                                      << ""
                    );
                    plStateDataRecord* rec = ((plSDStateVariable*)var)->Record(i);
                    for (size_t j=0; j<rec->getNumVars(); j++)
                        new QSDLTreeItem(rec->get(j), idx);
                }
            } else if (var->getCount() == 1) {
                plStateDataRecord* rec = ((plSDStateVariable*)var)->Record(0);
                for (size_t j=0; j<rec->getNumVars(); j++)
                    new QSDLTreeItem(rec->get(j), this);
            }
        }
    }
};

/* QSDLEditor */
QString QSDLEditor::GetVarDisplay(plStateVariable* var)
{
    QString result;
    for (size_t i=0; i<var->getCount(); i++) {
        if (i > 0)
            result += ", ";
        switch (var->getDescriptor()->getType()) {
        case plVarDescriptor::kNone:
            result += "(None)";
            break;
        case plVarDescriptor::kInt:
            result += QString("%1").arg(((plSimpleStateVariable*)var)->Int(i));
            break;
        case plVarDescriptor::kFloat:
            result += QString("%1").arg(((plSimpleStateVariable*)var)->Float(i));
            break;
        case plVarDescriptor::kBool:
            result += ((plSimpleStateVariable*)var)->Bool(i) ? "True" : "False";
            break;
        case plVarDescriptor::kString:
            result += ~((plSimpleStateVariable*)var)->String(i);
            break;
        case plVarDescriptor::kKey:
            result += ~((plSimpleStateVariable*)var)->Uoid(i).toString();
            break;
        case plVarDescriptor::kDouble:
            result += QString("%1").arg(((plSimpleStateVariable*)var)->Double(i));
            break;
        case plVarDescriptor::kByte:
            result += QString("%1").arg(((plSimpleStateVariable*)var)->Byte(i));
            break;
        case plVarDescriptor::kShort:
            result += QString("%1").arg(((plSimpleStateVariable*)var)->Short(i));
            break;
        case plVarDescriptor::kRGB:
            {
                hsColorRGBA color = ((plSimpleStateVariable*)var)->ColorRGBA(i);
                result += QString("hsColorRGBA(%1,%2,%3)")
                          .arg(color.r).arg(color.g).arg(color.b);
            }
            break;
        case plVarDescriptor::kRGBA:
            {
                hsColorRGBA color = ((plSimpleStateVariable*)var)->ColorRGBA(i);
                result += QString("hsColorRGBA(%1,%2,%3,%4)")
                          .arg(color.r).arg(color.g).arg(color.b).arg(color.a);
            }
            break;
        case plVarDescriptor::kRGB8:
            {
                hsColor32 color = ((plSimpleStateVariable*)var)->Color32(i);
                result += QString("hsColor32(%1,%2,%3)")
                          .arg(color.r).arg(color.g).arg(color.b);
            }
            break;
        case plVarDescriptor::kRGBA8:
            {
                hsColor32 color = ((plSimpleStateVariable*)var)->Color32(i);
                result += QString("hsColor32(%1,%2,%3,%4)")
                          .arg(color.r).arg(color.g).arg(color.b).arg(color.a);
            }
            break;
        case plVarDescriptor::kAgeTimeOfDay:
            {
                QDateTime dt;
                if (!((plSimpleStateVariable*)var)->getTimeStamp().atEpoch()) {
                    dt.setTime_t(((plSimpleStateVariable*)var)->getTimeStamp().getSecs());
                    result += dt.toString("yyyy/MM/dd hh:mm:ss");
                } else {
                    result += "N/A";
                }
            }
            break;
        case plVarDescriptor::kStateDescriptor:
            result = QString("(SDL Record: %1)").arg(~var->getDescriptor()->getStateDescType());
            break;
        default:
            result += "(incomplete)";
            break;
        }
    }
    return result;
}

QString QSDLEditor::GetSDLName(plVaultBlob blob)
{
    if (blob.getSize() == 0)
        return "(No SDL Descriptor)";

    hsRAMStream S;
    S.copyFrom(blob.getData(), blob.getSize());
    plString name;
    int version;
    plStateDataRecord::ReadStreamHeader(&S, name, version, NULL);
    return ~name;
}

QSDLEditor::QSDLEditor(QWidget* parent)
          : QWidget(parent), fSDLVersion(-1)
{
    fSDLList = new QTreeWidget(this);
    fSDLList->setColumnCount(2);
    fSDLList->setUniformRowHeights(true);
    fSDLList->setRootIsDecorated(false);
    fSDLList->headerItem()->setText(0, tr("Name"));
    fSDLList->headerItem()->setText(1, tr("Value"));

    fEditorPanel = new QWidget(this);
    fEditorLayout = new QGridLayout(fEditorPanel);
    fEditorLayout->setContentsMargins(0, 0, 0, 0);
    fEditorLayout->setHorizontalSpacing(8);
    fEditorLayout->setVerticalSpacing(8);

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setHorizontalSpacing(8);
    layout->setVerticalSpacing(8);
    layout->addWidget(fSDLList, 0, 0);
    layout->addWidget(fEditorPanel, 1, 0);

    connect(fSDLList, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)),
            this, SLOT(itemSelected(QTreeWidgetItem*, QTreeWidgetItem*)));
}

void QSDLEditor::setMgrs(plResManager* mgr, plSDLMgr* sdl)
{
    fResMgr = mgr;
    fSDLMgr = sdl;
}

void QSDLEditor::loadBlob(plVaultBlob blob)
{
    fSDLList->clear();
    fSDLVersion = -1;
    if (blob.getSize() == 0)
        return;

    hsRAMStream S;
    S.setVer(pvPots);
    S.copyFrom(blob.getData(), blob.getSize());

    plStateDataRecord::ReadStreamHeader(&S, fSDLName, fSDLVersion, NULL);
    plStateDescriptor* desc = fSDLMgr->GetDescriptor(fSDLName, fSDLVersion);
    if (desc == NULL) {
        QMessageBox msgBox(QMessageBox::Critical, tr("Error"),
                           tr("No SDL Descriptor for %1 (version %2)")
                           .arg(~fSDLName).arg(fSDLVersion),
                           QMessageBox::Ok, this);
        msgBox.exec();
        return;
    }
    fRecord.setDescriptor(desc);
    fRecord.read(&S, fResMgr);
    if (S.size() != S.pos()) {
        plDebug::Debug("[%s] SDL size-read difference: %d",
                       fSDLName.cstr(), S.size() - S.pos());
    }

    for (size_t i=0; i<fRecord.getNumVars(); i++)
        addVar(fRecord.get(i), fSDLList->invisibleRootItem());
    fSDLList->expandAll();
    fSDLList->resizeColumnToContents(0);
    fSDLList->resizeColumnToContents(1);
}

plVaultBlob QSDLEditor::saveBlob()
{
    if (fSDLVersion == -1)
        return plVaultBlob();

    if (fSDLList->currentItem() != NULL && fSDLList->currentItem()->type() == QSDLTreeItem::kSDLVarType)
        saveVarCustomEdit(fSDLList->currentItem(), fCurrentWhich);

    plVaultBlob blob;
    hsRAMStream S;
    S.setVer(pvPots);
    plStateDataRecord::WriteStreamHeader(&S, fSDLName, fSDLVersion, NULL);
    fRecord.write(&S, fResMgr);
    unsigned char* data = new unsigned char[S.size()];
    S.copyTo(data, S.size());
    blob.setData(S.size(), data);
    delete[] data;
    return blob;
}

void QSDLEditor::addVar(plStateVariable* var, QTreeWidgetItem* parent)
{
    new QSDLTreeItem(var, parent);
}

void QSDLEditor::setupVarEditorCommon(plStateVariable* var)
{
    fEditorWhich = new QSpinBox(fEditorPanel);
    fEditorWhich->setRange(0, var->getCount() - 1);
    fEditorLayout->addWidget(new QLabel(
        QString("%1[%2]:")
        .arg(~var->getDescriptor()->getName())
        .arg(var->getCount()), fEditorPanel), 0, 0);
    fEditorLayout->addWidget(fEditorWhich, 0, 1, 1, 2);

    connect(fEditorWhich, SIGNAL(valueChanged(int)), this, SLOT(indexChanged(int)));
}

void QSDLEditor::setVarCustomEdit(QTreeWidgetItem* item, int which)
{
    if (item == NULL || item->type() != QSDLTreeItem::kSDLVarType)
        return;

    plStateVariable* var = ((QSDLTreeItem*)item)->variable();
    if (var == NULL || var->getCount() < (size_t)which)
        return;

    fCurrentWhich = which;
    switch (var->getDescriptor()->getType()) {
    case plVarDescriptor::kBool:
        fComboEdit->setCurrentIndex(((plSimpleStateVariable*)var)->Bool(which) ? 1 : 0);
        break;
    case plVarDescriptor::kInt:
        fIntEdit->setValue(((plSimpleStateVariable*)var)->Int(which));
        break;
    case plVarDescriptor::kShort:
        fIntEdit->setValue(((plSimpleStateVariable*)var)->Short(which));
        break;
    case plVarDescriptor::kByte:
        fIntEdit->setValue(((plSimpleStateVariable*)var)->Byte(which));
        break;
    case plVarDescriptor::kString:
        fStringEdit->setText(~((plSimpleStateVariable*)var)->String(which));
        break;
    case plVarDescriptor::kKey:
        {
            plUoid key = ((plSimpleStateVariable*)var)->Uoid(which);
            fLocationEdit[0]->setValue(key.getLocation().getSeqPrefix());
            fLocationEdit[1]->setValue(key.getLocation().getPageNum());
            fComboEdit->setCurrentIndex(key.getType());
            fStringEdit->setText(~key.getName());
        }
        break;
    case plVarDescriptor::kRGB8:
        {
            hsColorRGBA color;
            color.r = ((plSimpleStateVariable*)var)->Color32(which).r / 255.0;
            color.g = ((plSimpleStateVariable*)var)->Color32(which).g / 255.0;
            color.b = ((plSimpleStateVariable*)var)->Color32(which).b / 255.0;
            color.a = 1.0;
            fColorEdit->setColor(color);
        }
        break;
    case plVarDescriptor::kRGBA8:
        {
            hsColorRGBA color;
            color.r = ((plSimpleStateVariable*)var)->Color32(which).r / 255.0;
            color.g = ((plSimpleStateVariable*)var)->Color32(which).g / 255.0;
            color.b = ((plSimpleStateVariable*)var)->Color32(which).b / 255.0;
            color.a = ((plSimpleStateVariable*)var)->Color32(which).a / 255.0;
            fColorEdit->setColor(color);
        }
        break;
    default:
        break;
    }
}

void QSDLEditor::saveVarCustomEdit(QTreeWidgetItem* item, int which)
{
    if (item == NULL || item->type() != QSDLTreeItem::kSDLVarType)
        return;

    plStateVariable* var = ((QSDLTreeItem*)item)->variable();
    if (var == NULL || var->getCount() < (size_t)which)
        return;

    switch (var->getDescriptor()->getType()) {
    case plVarDescriptor::kBool:
        ((plSimpleStateVariable*)var)->Bool(which) = fComboEdit->currentIndex() == 1;
        break;
    case plVarDescriptor::kInt:
        ((plSimpleStateVariable*)var)->Int(which) = fIntEdit->value();
        break;
    case plVarDescriptor::kShort:
        ((plSimpleStateVariable*)var)->Short(which) = fIntEdit->value();
        break;
    case plVarDescriptor::kByte:
        ((plSimpleStateVariable*)var)->Byte(which) = fIntEdit->value();
        break;
    case plVarDescriptor::kString:
        ((plSimpleStateVariable*)var)->String(which) = ~fStringEdit->text();
        break;
    case plVarDescriptor::kKey:
        {
            plUoid key = ((plSimpleStateVariable*)var)->Uoid(which);
            plLocation loc = key.getLocation();
            loc.setSeqPrefix(fLocationEdit[0]->value());
            loc.setPageNum(fLocationEdit[1]->value());
            key.setLocation(loc);
            key.setType(fComboEdit->currentIndex());
            key.setName(~fStringEdit->text());
            ((plSimpleStateVariable*)var)->Uoid(which) = key;
        }
        break;
    case plVarDescriptor::kRGB8:
        {
            hsColorRGBA color = fColorEdit->color();
            ((plSimpleStateVariable*)var)->Color32(which).r = (unsigned char)(color.r * 255);
            ((plSimpleStateVariable*)var)->Color32(which).g = (unsigned char)(color.g * 255);
            ((plSimpleStateVariable*)var)->Color32(which).b = (unsigned char)(color.b * 255);
        }
        break;
    case plVarDescriptor::kRGBA8:
        {
            hsColorRGBA color = fColorEdit->color();
            ((plSimpleStateVariable*)var)->Color32(which).r = (unsigned char)(color.r * 255);
            ((plSimpleStateVariable*)var)->Color32(which).g = (unsigned char)(color.g * 255);
            ((plSimpleStateVariable*)var)->Color32(which).b = (unsigned char)(color.b * 255);
            ((plSimpleStateVariable*)var)->Color32(which).a = (unsigned char)(color.a * 255);
        }
        break;
    default:
        break;
    }
    if (var->getDescriptor()->getType() != plVarDescriptor::kStateDescriptor)
        ((QSDLTreeItem*)item)->set(var);
}

void QSDLEditor::itemSelected(QTreeWidgetItem* current, QTreeWidgetItem* previous)
{
    saveVarCustomEdit(previous, fCurrentWhich);
    QLayoutItem* child;
    while ((child = fEditorLayout->takeAt(0)) != NULL) {
        delete child->widget();
        delete child;
    }

    if (current != NULL && current->type() == QSDLTreeItem::kSDLVarType) {
        // Setup a custom editor for the variable
        plStateVariable* var = ((QSDLTreeItem*)current)->variable();
        switch (var->getDescriptor()->getType()) {
        case plVarDescriptor::kBool:
            setupVarEditorCommon(var);
            fComboEdit = new QComboBox(fEditorPanel);
            fComboEdit->addItem("False");
            fComboEdit->addItem("True");
            fEditorLayout->addWidget(new QLabel("Boolean:", fEditorPanel), 1, 0);
            fEditorLayout->addWidget(fComboEdit, 1, 1, 1, 2);
            break;
        case plVarDescriptor::kInt:
            setupVarEditorCommon(var);
            fIntEdit = new QSpinBox(fEditorPanel);
            fIntEdit->setRange(-0x80000000, 0x7FFFFFFF);
            fEditorLayout->addWidget(new QLabel("Int:", fEditorPanel), 1, 0);
            fEditorLayout->addWidget(fIntEdit, 1, 1, 1, 2);
            break;
        case plVarDescriptor::kShort:
            setupVarEditorCommon(var);
            fIntEdit = new QSpinBox(fEditorPanel);
            fIntEdit->setRange(-0x8000, 0x7FFF);
            fEditorLayout->addWidget(new QLabel("Short:", fEditorPanel), 1, 0);
            fEditorLayout->addWidget(fIntEdit, 1, 1, 1, 2);
            break;
        case plVarDescriptor::kByte:
            setupVarEditorCommon(var);
            fIntEdit = new QSpinBox(fEditorPanel);
            fIntEdit->setRange(0, 0xFF);
            fEditorLayout->addWidget(new QLabel("Byte:", fEditorPanel), 1, 0);
            fEditorLayout->addWidget(fIntEdit, 1, 1, 1, 2);
            break;
        case plVarDescriptor::kString:
            setupVarEditorCommon(var);
            fStringEdit = new QLineEdit(fEditorPanel);
            fEditorLayout->addWidget(new QLabel("String:", fEditorPanel), 1, 0);
            fEditorLayout->addWidget(fStringEdit, 1, 1, 1, 2);
            break;
        case plVarDescriptor::kKey:
            setupVarEditorCommon(var);
            fLocationEdit[0] = new QSpinBox(fEditorPanel);
            fLocationEdit[1] = new QSpinBox(fEditorPanel);
            fLocationEdit[0]->setRange(-0x80000000, 0x7FFFFFFF);
            fLocationEdit[1]->setRange(-0x80000000, 0x7FFFFFFF);
            fComboEdit = new QComboBox(fEditorPanel);
            for (size_t i=0; i<kSwimStraightCurrentRegion; i++)
                fComboEdit->addItem(pdUnifiedTypeMap::ClassName(i));
            fStringEdit = new QLineEdit(fEditorPanel);
            fEditorLayout->addWidget(new QLabel("Key Location:", fEditorPanel), 1, 0);
            fEditorLayout->addWidget(fLocationEdit[0], 1, 1);
            fEditorLayout->addWidget(fLocationEdit[1], 1, 2);
            fEditorLayout->addWidget(new QLabel("Key Type:", fEditorPanel), 2, 0);
            fEditorLayout->addWidget(fComboEdit, 2, 1, 1, 2);
            fEditorLayout->addWidget(new QLabel("Key Name:", fEditorPanel), 3, 0);
            fEditorLayout->addWidget(fStringEdit, 3, 1, 1, 2);
            break;
        case plVarDescriptor::kRGB8:
            setupVarEditorCommon(var);
            fColorEdit = new QColorEdit(false, fEditorPanel);
            fEditorLayout->addWidget(new QLabel("Color:", fEditorPanel), 1, 0);
            fEditorLayout->addWidget(fColorEdit, 1, 1, 1, 2);
            break;
        case plVarDescriptor::kRGBA8:
            setupVarEditorCommon(var);
            fColorEdit = new QColorEdit(true, fEditorPanel);
            fEditorLayout->addWidget(new QLabel("Color:", fEditorPanel), 1, 0);
            fEditorLayout->addWidget(fColorEdit, 1, 1, 1, 2);
            break;
        default:
            break;
        }
        setVarCustomEdit(current, 0);
    }
    fEditorLayout->update();
}

void QSDLEditor::indexChanged(int idx)
{
    saveVarCustomEdit(fSDLList->currentItem(), fCurrentWhich);
    setVarCustomEdit(fSDLList->currentItem(), idx);
}


/* QVaultSDLNode */
QString QVaultSDLNode::sSDLTypeNames[] = {
    "(Generic)", "Age Instance SDL", "Age Global SDL"
};

int QVaultSDLNode::MapSDLType(int type)
{
    switch (type) {
    case plVault::kAgeInstanceSDLNode:
        return 1;
    case plVault::kAgeGlobalSDLNode:
        return 2;
    default:
        return 0;
    }
}

int QVaultSDLNode::UnmapSDLType(int idx)
{
    static int sMapping[] = {
        plVault::kUserDefinedNode, plVault::kAgeInstanceSDLNode,
        plVault::kAgeGlobalSDLNode
    };

    if (idx < 0 || idx > 2)
        // Set Invalid nodes to being Generic ones
        return plVault::kUserDefinedNode;
    return sMapping[idx];
}

QVaultSDLNode::QVaultSDLNode(QWidget* parent)
             : QVaultNodeEdit(parent)
{
    fSDLType = new QComboBox(this);
    for (int i=0; i<=2; i++)
        fSDLType->addItem(sSDLTypeNames[i]);

    fSDLEditor = new QSDLEditor(this);

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->setHorizontalSpacing(8);
    layout->setVerticalSpacing(8);
    layout->addWidget(new QLabel(tr("Type:"), this), 0, 0);
    layout->addWidget(fSDLType, 0, 1);
    layout->addWidget(fSDLEditor, 1, 1);
}

QString QVaultSDLNode::getEditorTitle() const
{ return "SDL"; }

void QVaultSDLNode::setMgrs(plResManager* mgr, plSDLMgr* sdl)
{ fSDLEditor->setMgrs(mgr, sdl); }

plVaultNode QVaultSDLNode::saveNode()
{
    plVaultSDLNode* sdl = fNode.upcastToSDLNode();
    if (sdl == NULL)
        return plVaultNode();

    sdl->setSDLIdent(UnmapSDLType(fSDLType->currentIndex()));

    try {
        sdl->setSDLData(fSDLEditor->saveBlob());
    } catch (hsException& ex) {
        plDebug::Error("%s:%d: %s", ex.File(), ex.Line(), ex.what());
        QMessageBox msgBox(QMessageBox::Critical, tr("Error"),
                           tr("Error saving SDL data"),
                           QMessageBox::Ok, this);
        msgBox.exec();
    }

    return fNode;
}

void QVaultSDLNode::IRefreshNode()
{
    plVaultSDLNode* sdl = fNode.upcastToSDLNode();
    if (sdl == NULL)
        return;

    fSDLType->setCurrentIndex(MapSDLType(sdl->getSDLIdent()));

    try {
        fSDLEditor->loadBlob(sdl->getSDLData());
    } catch (hsException& ex) {
        plDebug::Error("%s:%d: %s", ex.File(), ex.Line(), ex.what());
        QMessageBox msgBox(QMessageBox::Critical, tr("Error"),
                           tr("Error loading SDL data"),
                           QMessageBox::Ok, this);
        msgBox.exec();
    }
}