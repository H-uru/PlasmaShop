#ifndef _QVAULTTEXTNODE_H
#define _QVAULTTEXTNODE_H

#include <QComboBox>
#include <QLineEdit>
#include <QPlainTextEdit>
#include "QVaultNodeEdit.h"

class QVaultTextNode : public QVaultNodeEdit {
    Q_OBJECT

protected:
    QLineEdit* fNoteTitle;
    QComboBox* fNoteType;
    QPlainTextEdit* fNoteContents;

public:
    QVaultTextNode(QWidget* parent);
    virtual QString getEditorTitle() const;
    virtual plVaultNode saveNode();

protected:
    virtual void IRefreshNode();
};

#endif
