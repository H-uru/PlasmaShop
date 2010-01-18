#include "QPlasmaDocument.h"
#include "QPlasmaTextDoc.h"
#include "QPlasmaSumFile.h"
#include <QInputDialog>
#include <QSettings>
#include <QMessageBox>

QIcon QPlasmaDocument::GetDocIcon(QString filename)
{
    // Check for special types first
    if (filename.startsWith('<')) {
        if (filename == "<FOLDER>")
            return QIcon(":/img/folder.png");
        else if (filename == "<AGEFOLD>")
            return QIcon(":/img/rfldr.png");
        else if (filename == "<PYFOLD>")
            return QIcon(":/img/gfldr.png");
        else if (filename == "<SDLFOLD>")
            return QIcon(":/img/bfldr.png");
        else if (filename == "<FONTFOLD>" || filename == "<IMGFOLD>")
            return QIcon(":/img/ofldr.png");
        else if (filename == "<DATAFOLD>")
            return QIcon(":/img/wfldr.png");
        else if (filename == "<AGE>")
            return QIcon(":/img/age.png");
        else if (filename == "<VAULT>")
            return QIcon(":/img/db.png");
    }

    QString ext, fnameNoPath;
    QRegExp re;
    re.setPattern(".*\\.([^\\.]*)");
    if (re.indexIn(filename) >= 0)
        ext = re.cap(1).toLower();
    re.setPattern("(.*[\\\\\\/])?([^\\\\\\/]*)");
    if (re.indexIn(filename) >= 0)
        fnameNoPath = re.cap(2).toLower();

    if (ext == "age")
        return QIcon(":/img/age.png");
    else if (ext == "prp")
        return QIcon(":/img/page.png");
    else if (ext == "p2f")
        return QIcon(":/img/font.png");
    else if (ext == "tga" || ext == "jpg")
        return QIcon(":/img/img.png");
    else if (ext == "cfg" || ext == "csv" || ext == "fni" || ext == "fx"  ||
             ext == "ini" || ext == "mfs" || ext == "xml" ||
             ext == "mfold" || ext == "mlist" || fnameNoPath == "dev_mode.dat")
        return QIcon(":/img/ini.png");
    else if (ext == "loc" || ext == "log" || ext == "sub" || ext == "txt")
        return QIcon(":/img/text.png");
    else if (ext == "py" || ext == "pyc")
        return QIcon(":/img/py.png");
    else if (ext == "pak")
        return QIcon(":/img/pak.png");
    else if (ext == "pfp")
        return QIcon(":/img/pfp.png");
    else if (fnameNoPath == "cursors.dat")
        return QIcon(":/img/cursor_dat.png");
    else if (ext == "elf")
        return QIcon(":/img/elf.png");
    else if (ext == "sdl")
        return QIcon(":/img/sdl.png");
    else if (ext == "sum")
        return QIcon(":/img/sum.png");
    else if (ext == "hex")
        return QIcon(":/img/hex.png");
    else
        return QIcon(":/img/blank.png");
}

QPlasmaDocument* QPlasmaDocument::GetEditor(DocumentType docType, QWidget* parent)
{
    switch (docType) {
    case kDocText:
        return new QPlasmaTextDoc(parent);
    case kDocManifest:
        return new QPlasmaSumFile(parent);
    default:
        return NULL;
    }
}

bool QPlasmaDocument::GetEncryptionKeyFromUser(QWidget* parent, unsigned int* key)
{
    QSettings settings("PlasmaShop", "PlasmaShop");
    QString keyStr = settings.value("NtdKey", "00000000000000000000000000000000").toString();
    bool dlgAccept, validKey = false;
    while (!validKey) {
        keyStr = QInputDialog::getText(parent, tr("Enter NTD Key"),
                                       tr("Enter your MOUL/MQO Encryption key"),
                                       QLineEdit::Normal, keyStr, &dlgAccept);
        if (!dlgAccept)
            return false;
        if (keyStr.length() == 32) {
            validKey = true;
            for (int i=0; i<keyStr.length() && validKey; ++i) {
                QChar ch = keyStr.at(i);
                if (ch.isDigit())
                    continue;
                if (ch >= 'A' && ch <= 'F')
                    continue;
                if (ch >= 'a' && ch <= 'f')
                    continue;
                validKey = false;
            }
        }
        if (!validKey) {
            QMessageBox::warning(parent, tr("Invalid Key"),
                                 tr("You have entered an invalid encryption key.\n"
                                    "Encryption keys should be 32 hex digits, with no\n"
                                    "spaces or dashes"),
                                 QMessageBox::Ok);
        } else {
            key[0] = keyStr.mid(0, 8).toUInt(0, 16);
            key[1] = keyStr.mid(8, 8).toUInt(0, 16);
            key[2] = keyStr.mid(16, 8).toUInt(0, 16);
            key[3] = keyStr.mid(24, 8).toUInt(0, 16);
            settings.setValue("NtdKey", keyStr);
        }
    }
    return true;
}

QPlasmaDocument::QPlasmaDocument(DocumentType docType, QWidget* parent)
               : QWidget(parent), fDocType(docType), fEncryption(kEncNone),
                 fDirty(false), fPersistDirty(false)
{ }

DocumentType QPlasmaDocument::docType() const
{ return fDocType; }

bool QPlasmaDocument::isDirty() const
{ return fDirty; }

QString QPlasmaDocument::filename() const
{ return fFilename; }

bool QPlasmaDocument::canCut() const
{ return false; }

bool QPlasmaDocument::canCopy() const
{ return false; }

bool QPlasmaDocument::canPaste() const
{ return false; }

bool QPlasmaDocument::canDelete() const
{ return false; }

bool QPlasmaDocument::canSelectAll() const
{ return false; }

bool QPlasmaDocument::canUndo() const
{ return false; }

bool QPlasmaDocument::canRedo() const
{ return false; }

void QPlasmaDocument::updateSettings()
{ }

void QPlasmaDocument::performCut()
{ }

void QPlasmaDocument::performCopy()
{ }

void QPlasmaDocument::performPaste()
{ }

void QPlasmaDocument::performDelete()
{ }

void QPlasmaDocument::performSelectAll()
{ }

void QPlasmaDocument::performUndo()
{ }

void QPlasmaDocument::performRedo()
{ }

bool QPlasmaDocument::loadFile(QString filename)
{
    fFilename = filename;
    fPersistDirty = false;
    makeClean();
    return true;
}

bool QPlasmaDocument::saveTo(QString filename)
{
    fFilename = filename;
    fPersistDirty = false;
    makeClean();
    return true;
}

bool QPlasmaDocument::saveDefault()
{
    return saveTo(fFilename);
}

bool QPlasmaDocument::revert()
{
    return loadFile(fFilename);
}

void QPlasmaDocument::setEncryption(EncryptionMode enc)
{
    fEncryption = enc;
    makeDirty();
    fPersistDirty = true;
}

QPlasmaDocument::EncryptionMode QPlasmaDocument::encryption() const
{ return fEncryption; }

bool QPlasmaDocument::isZeroKey(const unsigned int* key)
{
    return (key[0] == 0) && (key[1] == 0) && (key[2] == 0) && (key[3] == 0);
}

void QPlasmaDocument::makeDirty()
{
    if (!fDirty) {
        fDirty = true;
        emit becameDirty();
    }
}

void QPlasmaDocument::makeClean()
{
    if (fDirty) {
        fDirty = false;
        emit becameClean();
    }
}

void QPlasmaDocument::maybeClean()
{
    if (!fPersistDirty)
        makeClean();
}
