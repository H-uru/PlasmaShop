#ifndef _QGAMESERVERSTATE_H
#define _QGAMESERVERSTATE_H

#include "QVaultSDLNode.h"
#include <QComboBox>
#include "SDL/plNetGameServerState.h"

class QGameServerState : public QWidget {
    Q_OBJECT

public:
    QGameServerState(QWidget* parent);
    void loadSav(plNetGameServerState* state);

private slots:
    void objectChanged(int idx);

private:
    QComboBox* fObjectSel;
    QSDLEditor* fSDLEditor;

    plNetGameServerState* fState;
};

#endif
