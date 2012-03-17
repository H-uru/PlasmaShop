/* This file is part of PlasmaShop.
 *
 * PlasmaShop is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PlasmaShop is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PlasmaShop.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _QPLASMADOCUMENT_H
#define _QPLASMADOCUMENT_H

#include <QWidget>
#include <QIcon>

enum DocumentType {
    kDocText, kDocFont, kDocPackage, kDocManifest, kDocDevMode,
    kDocUnknown
};

class QPlasmaDocument : public QWidget {
    Q_OBJECT

public:
    enum EncryptionMode {
        kEncNone, kEncXtea, kEncAes, kEncDroid,
    };

public:
    static QIcon GetDocIcon(QString filename);
    static QPlasmaDocument* GetEditor(DocumentType docType, QWidget* parent);
    static bool GetEncryptionKeyFromUser(QWidget* parent, unsigned int* key);

    QPlasmaDocument(DocumentType docType, QWidget* parent);

    DocumentType docType() const { return fDocType; }
    bool isDirty() const { return fDirty; }

    QString filename() const { return fFilename; }
    void setFilename(QString filename) { fFilename = filename; }

    virtual bool canCut() const { return false; }
    virtual bool canCopy() const { return false; }
    virtual bool canPaste() const { return false; }
    virtual bool canDelete() const { return false; }
    virtual bool canSelectAll() const { return false; }
    virtual bool canUndo() const { return false; }
    virtual bool canRedo() const { return false; }

    virtual bool loadFile(QString filename);
    virtual bool saveTo(QString filename);
    bool saveDefault() { return saveTo(fFilename); }
    bool revert() { return loadFile(fFilename); }

    void setEncryption(EncryptionMode enc);
    EncryptionMode encryption() const { return fEncryption; }

signals:
    void becameDirty();
    void becameClean();
    void statusChanged();

public slots:
    virtual void updateSettings() { }
    virtual void performCut() { }
    virtual void performCopy() { }
    virtual void performPaste() { }
    virtual void performDelete() { }
    virtual void performSelectAll() { }
    virtual void performUndo() { }
    virtual void performRedo() { }

protected:
    DocumentType fDocType;
    EncryptionMode fEncryption;
    unsigned int fDroidKey[4];
    QString fFilename;
    bool fDirty, fPersistDirty;

    static bool isZeroKey(const unsigned int* key)
    { return (key[0] == 0) && (key[1] == 0) && (key[2] == 0) && (key[3] == 0); }

public slots:
    void makeDirty();
    void makeClean();
    void maybeClean();
};

#endif
