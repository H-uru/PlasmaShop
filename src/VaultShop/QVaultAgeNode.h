#ifndef _QVAULTAGENODE_H
#define _QVAULTAGENODE_H

#include <QLineEdit>
#include <QSpinBox>
#include "QVaultNodeEdit.h"

class QVaultAgeNode : public QVaultNodeEdit {
    Q_OBJECT

protected:
    QLineEdit* fAgeInstanceGuid;

public:
    QVaultAgeNode(QWidget* parent);
    virtual QString getEditorTitle() const;
    virtual plVaultNode saveNode();

protected:
    virtual void IRefreshNode();
};

#endif
