#include "QGameServerState.h"
#include <QLabel>

QGameServerState::QGameServerState(QWidget* parent)
                : QWidget(parent)
{
    fObjectSel = new QComboBox(this);
    fSDLEditor = new QSDLEditor(this);

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->setHorizontalSpacing(8);
    layout->setVerticalSpacing(8);
    layout->addWidget(new QLabel(tr("Object:"), this), 0, 0);
    layout->addWidget(fObjectSel, 0, 1);
    layout->addWidget(fSDLEditor, 1, 1);

    connect(fObjectSel, SIGNAL(currentIndexChanged(int)),
            this, SLOT(objectChanged(int)));
}

void QGameServerState::loadSav(plNetGameServerState* state)
{
    fObjectSel->clear();
    fState = state;

    int selId = 0;
    for (size_t i=0; i<fState->numRecords(); i++) {
        fObjectSel->addItem(QIcon(":img/sceneobj.png"),
                            ~fState->getObject(i).getName());

        // If this is the AgeSDLHook, select it by default
        if (fState->getObject(i).getName() == "AgeSDLHook")
            selId = (int)i;
    }

    if (fObjectSel->count() > 0) {
        fObjectSel->setEnabled(true);
        fSDLEditor->setEnabled(true);
        fSDLEditor->setMgrs(NULL, &fState->getSDLMgr());
        fObjectSel->setCurrentIndex(selId);
    } else {
        // No SDL records, so disable everything
        fObjectSel->setEnabled(false);
        fSDLEditor->setEnabled(false);
    }
}

void QGameServerState::objectChanged(int idx)
{
    fSDLEditor->setRecord(fState->getRecord(idx));
}
