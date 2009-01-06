#include "QVaultSDLNode.h"

#include <QLabel>
#include <QGridLayout>
#include <QMessageBox>
#include <QDateTime>
#include <Vault/plVaultNodeTypes.h>
#include <Stream/hsRAMStream.h>
#include <Debug/plDebug.h>

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
            result += QString::fromUtf8(((plSimpleStateVariable*)var)->String(i).cstr());
            break;
        case plVarDescriptor::kKey:
            result += QString::fromUtf8(((plSimpleStateVariable*)var)->Uoid(i).toString().cstr());
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
                if (((plSimpleStateVariable*)var)->getTimeStamp().getSecs() != 0) {
                    dt.setTime_t(((plSimpleStateVariable*)var)->getTimeStamp().getSecs());
                    result += dt.toString("yyyy/MM/dd hh:mm:ss");
                } else {
                    result += "N/A";
                }
            }
            break;
        case plVarDescriptor::kStateDescriptor:
            result = QString("(SDL Record: %1)").arg(QString::fromUtf8(var->getDescriptor()->getStateDescType().cstr()));
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
    return QString::fromUtf8(name.cstr());
}

QSDLEditor::QSDLEditor(QWidget* parent)
          : QWidget(parent)
{
    fSDLList = new QTreeWidget(this);
    fSDLList->setColumnCount(2);
    fSDLList->setUniformRowHeights(true);
    fSDLList->setRootIsDecorated(false);
    fSDLList->headerItem()->setText(0, tr("Name"));
    fSDLList->headerItem()->setText(1, tr("Value"));

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setHorizontalSpacing(8);
    layout->setVerticalSpacing(8);
    layout->addWidget(fSDLList, 0, 0);
}

void QSDLEditor::setMgrs(plResManager* mgr, plSDLMgr* sdl)
{
    fResMgr = mgr;
    fSDLMgr = sdl;
}

void QSDLEditor::loadBlob(plVaultBlob blob)
{
    fSDLList->clear();
    if (blob.getSize() == 0)
        return;

    hsRAMStream S;
    S.copyFrom(blob.getData(), blob.getSize());

    plString name;
    int version;
    plStateDataRecord::ReadStreamHeader(&S, name, version, NULL);
    plStateDescriptor* desc = fSDLMgr->GetDescriptor(name, version);
    if (desc == NULL) {
        QMessageBox msgBox(QMessageBox::Critical, tr("Error"),
                           tr("No SDL Descriptor for %1 (version %2)")
                           .arg(QString::fromUtf8(name.cstr())).arg(version),
                           QMessageBox::Ok, this);
        msgBox.exec();
        return;
    }
    fRecord.setDescriptor(desc);
    fRecord.read(&S, fResMgr);
    if (S.size() != S.pos())
        plDebug::Debug("SDL size-read difference: %d", S.size() - S.pos());

    for (size_t i=0; i<fRecord.getNumVars(); i++)
        addVar(fRecord.get(i), fSDLList->invisibleRootItem());
    fSDLList->expandAll();
    fSDLList->resizeColumnToContents(0);
    fSDLList->resizeColumnToContents(1);
}

plVaultBlob QSDLEditor::saveBlob()
{
    plVaultBlob blob;
    hsRAMStream S;
    fRecord.write(&S, fResMgr);
    unsigned char* data = new unsigned char[S.size()];
    S.copyTo(data, S.size());
    blob.setData(S.size(), data);
    delete[] data;
    return blob;
}

void QSDLEditor::addVar(plStateVariable* var, QTreeWidgetItem* parent)
{
    QTreeWidgetItem* item = new QTreeWidgetItem(parent,
        QStringList() << var->getDescriptor()->getName().cstr()
                      << GetVarDisplay(var)
    );
    if (var->getDescriptor()->getType() == plVarDescriptor::kStateDescriptor) {
        if (var->getCount() > 1) {
            for (size_t i=0; i<var->getCount(); i++) {
                QTreeWidgetItem* idx = new QTreeWidgetItem(item,
                    QStringList() << QString("Item [%1]").arg(i)
                                  << ""
                );
                plStateDataRecord* rec = ((plSDStateVariable*)var)->Record(i);
                for (size_t j=0; j<rec->getNumVars(); j++)
                    addVar(rec->get(j), idx);
            }
        } else if (var->getCount() == 1) {
            plStateDataRecord* rec = ((plSDStateVariable*)var)->Record(0);
            for (size_t j=0; j<rec->getNumVars(); j++)
                addVar(rec->get(j), item);
        }
    }
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
        return;
    }
}
