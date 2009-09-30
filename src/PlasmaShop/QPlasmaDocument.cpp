#include "QPlasmaDocument.h"

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
        else if (filename == "<FONTFOLD>")
            return QIcon(":/img/ofldr.png");
        else if (filename == "<DATAFOLD>")
            return QIcon(":/img/wfldr.png");
    }

    QString ext, fnameNoPath;
    QRegExp re;
    re.setPattern(".*\\.([^.]*)");
    if (re.indexIn(filename) >= 0)
        ext = re.cap(1).toLower();
    re.setPattern(".*[\\/]([^\\/]*)");
    if (re.indexIn(filename) >= 0)
        fnameNoPath = re.cap(1).toLower();

    if (ext == "age")
        return QIcon(":/img/age.png");
    else if (ext == "prp")
        return QIcon(":/img/page.png");
    else if (ext == "p2f")
        return QIcon(":/img/font.png");
    else if (ext == "tga")
        return QIcon(":/img/img.png");
    // cfg, csv, fni, ini, mfold, mlist, mfs, xml, fx
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
    else if (ext == "pak")
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
        // TODO
    default:
        //return NULL;
        return new QPlasmaDocument(docType, parent);    // Placeholder
    }
}

QPlasmaDocument::QPlasmaDocument(DocumentType docType, QWidget* parent)
               : QWidget(parent), fDocType(docType)
{ }

DocumentType QPlasmaDocument::docType() const
{ return fDocType; }

bool QPlasmaDocument::isDirty() const
{ return fDirty; }

QString QPlasmaDocument::filename() const
{ return fFilename; }

void QPlasmaDocument::loadFile(QString filename)
{
    fFilename = filename;
    makeClean();
}

void QPlasmaDocument::saveTo(QString filename)
{
    fFilename = filename;
    makeClean();
}

void QPlasmaDocument::saveDefault()
{
    saveTo(fFilename);
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
