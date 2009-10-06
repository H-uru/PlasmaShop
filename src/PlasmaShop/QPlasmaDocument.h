#ifndef _QPLASMADOCUMENT_H
#define _QPLASMADOCUMENT_H

#include <QWidget>
#include <QIcon>

enum DocumentType {
    kDocText, kDocFont, kDocPackage, kDocManifest,
    kDocUnknown
};

class QPlasmaDocument : public QWidget {
    Q_OBJECT

public:
    static QIcon GetDocIcon(QString filename);
    static QPlasmaDocument* GetEditor(DocumentType docType, QWidget* parent);
    static bool GetEncryptionKeyFromUser(QWidget* parent, unsigned int* key);

    QPlasmaDocument(DocumentType docType, QWidget* parent);

    DocumentType docType() const;
    bool isDirty() const;
    QString filename() const;

    virtual bool canCut() const;
    virtual bool canCopy() const;
    virtual bool canPaste() const;
    virtual bool canDelete() const;
    virtual bool canSelectAll() const;
    virtual bool canUndo() const;
    virtual bool canRedo() const;

    virtual bool loadFile(QString filename);
    virtual bool saveTo(QString filename);
    bool saveDefault();
    bool revert();

signals:
    void becameDirty();
    void becameClean();
    void statusChanged();

public slots:
    virtual void updateSettings();
    virtual void performCut();
    virtual void performCopy();
    virtual void performPaste();
    virtual void performDelete();
    virtual void performSelectAll();
    virtual void performUndo();
    virtual void performRedo();

protected:
    DocumentType fDocType;
    QString fFilename;
    bool fDirty;

protected slots:
    void makeDirty();
    void makeClean();
};

#endif
