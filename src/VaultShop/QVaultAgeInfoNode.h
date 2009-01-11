#ifndef _QVAULTAGEINFONODE_H
#define _QVAULTAGEINFONODE_H

#include <QLineEdit>
#include <QSpinBox>
#include "QVaultNodeEdit.h"
#include "QLinkLabel.h"

class QVaultAgeInfoNode : public QVaultNodeEdit {
    Q_OBJECT

protected:
    QSpinBox* fAgeID;
    QLinkLabel* fAgeLink;
    QSpinBox* fAgeCzarID;
    QLinkLabel* fAgeCzarLink;
    QLineEdit* fAgeFilename;
    QLineEdit* fAgeInstanceName;
    QLineEdit* fAgeUserDefinedName;
    QLineEdit* fAgeDescription;
    QLineEdit* fAgeInstanceGuid;

public:
    QVaultAgeInfoNode(QWidget* parent);
    virtual QString getEditorTitle() const;
    virtual plVaultNode saveNode();

protected:
    virtual void IRefreshNode();

protected slots:
    void subscribeAge();
    void subscribeAgeCzar();
};

#endif
