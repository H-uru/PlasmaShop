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

    QPlasmaDocument(DocumentType docType, QWidget* parent);

    DocumentType docType() const;
    bool isDirty() const;
    QString filename() const;

    virtual void loadFile(QString filename);
    virtual void saveTo(QString filename);
    void saveDefault();

signals:
    void becameDirty();
    void becameClean();

protected:
    DocumentType fDocType;
    QString fFilename;
    bool fDirty;

private slots:
    void makeDirty();
    void makeClean();
};

#endif
