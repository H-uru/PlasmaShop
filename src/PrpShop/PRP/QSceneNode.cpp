#include "QSceneNode.h"

#include <QLabel>
#include <QGridLayout>
#include <QTabWidget>

QSceneNode::QSceneNode(plCreatable* pCre, QWidget* parent)
          : QCreatable(pCre, kSceneNode, parent)
{
    plSceneNode* node = (plSceneNode*)fCreatable;

    QTabWidget* tab = new QTabWidget(this);
    fSceneKeys = new QKeyList(tab);
    fPoolKeys = new QKeyList(tab);
    for (size_t i=0; i<node->getNumSceneObjects(); i++)
        fSceneKeys->addKey(node->getSceneObject(i));
    for (size_t i=0; i<node->getNumPoolObjects(); i++)
        fPoolKeys->addKey(node->getPoolObject(i));
    tab->addTab(fSceneKeys, tr("Scene Objects"));
    tab->addTab(fPoolKeys, tr("Pool Objects"));

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(tab, 0, 0);
    fSceneKeys->adjustColumns();
    fPoolKeys->adjustColumns();
}

void QSceneNode::saveDamage()
{
    plSceneNode* node = (plSceneNode*)fCreatable;

    node->getSceneObjects().clear();
    node->getPoolObjects().clear();
    QList<plKey> sokeys = fSceneKeys->keys();
    while (!sokeys.isEmpty())
        node->getSceneObjects().append(sokeys.takeFirst());
    QList<plKey> pokeys = fPoolKeys->keys();
    while (!pokeys.isEmpty())
        node->getPoolObjects().append(pokeys.takeFirst());
}
