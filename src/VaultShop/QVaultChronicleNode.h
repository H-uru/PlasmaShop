#ifndef _QVAULTCHRONICLENODE_H
#define _QVAULTCHRONICLENODE_H

#include <QSpinBox>
#include <QLineEdit>
#include "QVaultNodeEdit.h"

class QVaultChronicleNode : public QVaultNodeEdit {
    Q_OBJECT

protected:
    QSpinBox* fChronicleType;
    QLineEdit* fChronicleName;
    QLineEdit* fChronicleValue;

public:
    QVaultChronicleNode(QWidget* parent);
    virtual QString getEditorTitle() const;
    virtual plVaultNode saveNode();

protected:
    virtual void IRefreshNode();
};

#endif
