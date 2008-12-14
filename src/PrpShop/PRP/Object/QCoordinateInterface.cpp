#include "QCoordinateInterface.h"

#include <QGroupBox>
#include <QLabel>
#include <QGridLayout>

QCoordinateInterface::QCoordinateInterface(plCreatable* pCre, QWidget* parent)
                    : QCreatable(pCre, parent)
{
    plCoordinateInterface* intf = (plCoordinateInterface*)fCreatable;
    QWidget* center = new QWidget(this);

    QGroupBox* grpProps = new QGroupBox(tr("Properties"), center);
    QGridLayout* layProps = new QGridLayout(grpProps);
    layProps->setVerticalSpacing(2);
    fCBProperties[plCoordinateInterface::kDisable] = new QCheckBox(tr("Disable"), grpProps);
    fCBProperties[plCoordinateInterface::kCanEverDelayTransform] = new QCheckBox(tr("Can Ever Delay Transform"), grpProps);
    fCBProperties[plCoordinateInterface::kDelayedTransformEval] = new QCheckBox(tr("Delayed Transform Evaluation"), grpProps);
    layProps->addWidget(fCBProperties[plCoordinateInterface::kDisable], 0, 0);
    layProps->addWidget(fCBProperties[plCoordinateInterface::kCanEverDelayTransform], 1, 0);
    layProps->addWidget(fCBProperties[plCoordinateInterface::kDelayedTransformEval], 2, 0);
    for (size_t i=0; i<plCoordinateInterface::kNumProps; i++)
        fCBProperties[i]->setChecked(intf->getProperty(i));

    fChildren = new QKeyList(center);
    for (size_t i=0; i<intf->getNumChildren(); i++)
        fChildren->addKey(intf->getChild(i));

    QGridLayout* layout = new QGridLayout(center);
    layout->addWidget(grpProps, 0, 0);
    layout->addWidget(new QLabel(tr("Children:"), center), 1, 0);
    layout->addWidget(fChildren, 2, 0);
    layout->setContentsMargins(4, 4, 4, 4);
    setWidget(center);

    fChildren->adjustColumns();
}

void QCoordinateInterface::saveDamage()
{
    plCoordinateInterface* intf = (plCoordinateInterface*)fCreatable;
    for (size_t i=0; i<plCoordinateInterface::kNumProps; i++)
        intf->setProperty(i, fCBProperties[i]->isChecked());
}
