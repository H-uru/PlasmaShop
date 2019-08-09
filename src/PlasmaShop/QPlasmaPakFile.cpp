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

#include "QPlasmaPakFile.h"
#include <Debug/plDebug.h>
#include <Stream/plEncryptedStream.h>
#include <QGridLayout>
#include <QMessageBox>
#include <QMenu>
#include <QToolBar>
#include <QFileDialog>
#include <QSettings>
#include "QPlasma.h"

#include <ASTree.h>
#include <data.h>
#include <cstdio>
#include "Main.h"

#define PYC_MAGIC_22  (0x0A0DED2D)
#define PYC_MAGIC_23  (0x0A0DF23B)

/* PlasmaPackage */
void PlasmaPackage::read(hsStream* S)
{
    uint32_t magic = S->readInt();
    if (magic == kMyst5Arc) {
        // Cursors.dat or Fonts.pfp file
        fUnknown = S->readInt();
        fEntries.resize(S->readInt());

        // Try to read the first name as if it were a Cursors.dat file...  If
        // this fails, we're probably reading Fonts.pfp instead
        fType = kCursorsDat;
        uint32_t startPos = S->pos();
        uint32_t firstNameSize = S->readInt();
        if (firstNameSize < 0x8000) {
            // Make sure there are no control characters -- a quick sign that
            // we're not really reading a string!
            while (firstNameSize != 0 && S->pos() < S->size()) {
                if (S->readByte() < 0x20) {
                    fType = kFontsPfp;
                    break;
                }
                --firstNameSize;
            }
            if (firstNameSize != 0)
                fType = kFontsPfp;
        } else {
            // The string length was unrealistically long...  Probably ASCII
            fType = kFontsPfp;
        }

        // Now actually read the data
        S->seek(startPos);
        for (size_t i=0; i<fEntries.size(); i++) {
            if (fType == kCursorsDat) {
                uint32_t length = S->readInt();
                fEntries[i].fName = S->readStr(length);
                fEntries[i].fOffset = S->pos();
                uint32_t size = S->readInt();
                uint8_t* data = new uint8_t[size];
                S->read(size, data);
                fEntries[i].fData = FileBlob(data, size);
            } else {
                fEntries[i].fOffset = S->pos();
                fEntries[i].fFontData.readP2F(S);
            }
        }
    } else {
        // Python.pak
        fEntries.resize(magic);
        fType = kPythonPak;
        for (size_t i=0; i<fEntries.size(); i++) {
            fEntries[i].fName = S->readSafeStr();
            fEntries[i].fOffset = S->readInt();
        }

        for (size_t i=0; i<fEntries.size(); i++) {
            if (fEntries[i].fOffset != S->pos())
                S->seek(fEntries[i].fOffset);
            uint32_t size = S->readInt();
            if (S->pos() + size > S->size()) {
                plDebug::Warning("Warning: Pak file: Truncating last entry");
                size = S->size() - S->pos();
            }
            uint8_t* data = new uint8_t[size];
            S->read(size, data);
            fEntries[i].fData = FileBlob(data, size);
        }
    }
}

void PlasmaPackage::write(hsStream* S)
{
    if (fType == kPythonPak) {
        S->writeInt(fEntries.size());

        // Calculate all the offsets first, so we don't have to seek on an
        // encrypted stream (very slow)
        uint32_t off_accum = S->pos();
        for (size_t i=0; i<fEntries.size(); i++) {
            // SafeString header + offset
            off_accum += 6 + fEntries[i].fName.size();
        }
        for (size_t i=0; i<fEntries.size(); i++) {
            fEntries[i].fOffset = off_accum;
            off_accum += fEntries[i].fData.getSize() + sizeof(uint32_t);
        }

        // Now actually write the data
        for (size_t i=0; i<fEntries.size(); i++) {
            S->writeSafeStr(fEntries[i].fName);
            S->writeInt(fEntries[i].fOffset);
        }
        for (size_t i=0; i<fEntries.size(); i++) {
            S->writeInt(fEntries[i].fData.getSize());
            S->write(fEntries[i].fData.getSize(), fEntries[i].fData.getData());
        }
    } else {
        S->writeInt(kMyst5Arc);
        S->writeInt(fUnknown);
        S->writeInt(fEntries.size());

        for (size_t i=0; i<fEntries.size(); i++) {
            if (fType == kCursorsDat) {
                S->writeInt(fEntries[i].fName.size());
                S->writeStr(fEntries[i].fName);
                S->writeInt(fEntries[i].fData.getSize());
                S->write(fEntries[i].fData.getSize(), fEntries[i].fData.getData());
            } else {
                fEntries[i].fFontData.writeP2F(S);
            }
        }
    }
}

void PlasmaPackage::addFrom(QString filename)
{
    QFileInfo finfo(filename);
    FileEntry add;

    hsFileStream S;
    S.open(qstr2st(QDir::toNativeSeparators(filename)), fmRead);

    if (fType == kFontsPfp) {
        add.fFontData.readP2F(&S);
    } else {
        if (fType == kPythonPak) // TODO: compile py to pyc
            add.fName = qstr2st(finfo.fileName()).replace(".pyc", ".py");
        else
            add.fName = qstr2st(finfo.fileName());

        uint8_t* data = new uint8_t[S.size()];
        S.read(S.size(), data);
        add.fData = PlasmaPackage::FileBlob(data, S.size());
    }
    S.close();

    // Check if the file is already in the package...  If so, delete it first
    // to avoid duplicates
    std::vector<FileEntry>::iterator iter = fEntries.begin();
    while (iter != fEntries.end()) {
        if (displayName(*iter) == displayName(add)) {
            iter = fEntries.erase(iter);
            break;
        } else {
            ++iter;
        }
    }
    fEntries.push_back(add);
}

void PlasmaPackage::writeToFile(const FileEntry& ent, QString filename)
{
    hsFileStream S;
    S.open(QDir::toNativeSeparators(filename).toUtf8().data(), fmCreate);

    if (fType == kFontsPfp) {
        ent.fFontData.writeP2F(&S);
    } else if (fType == kCursorsDat) {
        S.write(ent.fData.getSize(), ent.fData.getData());
    } else {
        // Dirty hack which is more likely to be correct than PlasmaShop 2.x:
        // Examine the header of each bytecode blob -- if the PyCode object
        // is from Python 2.2, there will be a PyString after 4 uint16s...
        // if it is from Python 2.3, the PyString will be after 4 uint32s
        if (ent.fData.getSize() > 9 && ent.fData.getData()[9] == 's') {
            S.writeInt(PYC_MAGIC_22);
        } else if (ent.fData.getSize() > 17 && ent.fData.getData()[17] == 's') {
            S.writeInt(PYC_MAGIC_23);
        } else {
            QMessageBox::critical(NULL, QObject::tr("Error parsing Python blob"),
                    QObject::tr("Could not determine Python code blob version.  Assuming 2.3"));
            S.writeInt(PYC_MAGIC_23);
        }

        // Timestamp...  Not saved, so just set it to zero
        S.writeInt(0);

        S.write(ent.fData.getSize(), ent.fData.getData());
    }
    S.close();
}

QString PlasmaPackage::displayName(const FileEntry& ent) const
{
    if (fType == kFontsPfp) {
        return st2qstr(ST::format("{}-{}.p2f", ent.fFontData.getName(),
                                  ent.fFontData.getSize()));
    } else {
        return st2qstr(ent.fName);
    }
}

QString PlasmaPackage::displaySize(const PlasmaPackage::FileEntry& ent) const
{
    if (fType == kFontsPfp) {
        return QString("%1 (w), %2 (h), %3 chars")
                .arg(ent.fFontData.getWidth())
                .arg(ent.fFontData.getHeight() / ent.fFontData.getNumCharacters())
                .arg(ent.fFontData.getNumCharacters());
    } else {
        return QString("%L1 bytes").arg(ent.fData.getSize());
    }
}

QString PlasmaPackage::getFilter() const
{
    if (fType == kCursorsDat) {
        return "Targa Images (*.tga)";
    } else if (fType == kFontsPfp) {
        return "Plasma Fonts (*.p2f)";
    } else {
        return "Python Bytecode (*.pyc *.pyo)";
    }
}


/* QPlasmaPakFile */
QPlasmaPakFile::QPlasmaPakFile(QWidget* parent)
    : QPlasmaDocument(kDocPackage, parent)
{
    fFileList = new QTreeWidget(this);
    fFileList->setUniformRowHeights(true);
    fFileList->setRootIsDecorated(false);
    fFileList->setEditTriggers(QAbstractItemView::SelectedClicked |
                               QAbstractItemView::EditKeyPressed);
    fFileList->setContextMenuPolicy(Qt::CustomContextMenu);
    fFileList->setSelectionMode(QAbstractItemView::ExtendedSelection);
    fFileList->setHeaderLabels(QStringList() << "Filename" << "Size");

    QToolBar* toolbar = new QToolBar(this);
    toolbar->setOrientation(Qt::Vertical);
    toolbar->setToolButtonStyle(Qt::ToolButtonIconOnly);
    toolbar->setIconSize(QSize(22, 22));

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(4, 4, 4, 4);
    layout->addWidget(fFileList, 0, 0);
    layout->addWidget(toolbar, 0, 1);
    setLayout(layout);

    fActions[kAdd] = new QAction(qStdIcon("list-add"), tr("&Add / Update..."), this);
    fActions[kDel] = new QAction(qStdIcon("list-remove"), tr("&Delete"), this);
    fActions[kExtract] = new QAction(qStdIcon("document-save"), tr("&Extract..."), this);
    fActions[kExtractAll] = new QAction(QIcon(":/img/pak.png"), tr("Ex&tract all..."), this);

    toolbar->addAction(fActions[kAdd]);
    toolbar->addAction(fActions[kDel]);
    toolbar->addSeparator();
    toolbar->addAction(fActions[kExtract]);
    toolbar->addAction(fActions[kExtractAll]);

    connect(fFileList, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(onContextMenu(QPoint)));
    connect(fActions[kAdd], SIGNAL(triggered()), this, SLOT(onAdd()));
    connect(fActions[kDel], SIGNAL(triggered()), this, SLOT(onDel()));
    connect(fActions[kExtract], SIGNAL(triggered()), this, SLOT(onExtract()));
    connect(fActions[kExtractAll], SIGNAL(triggered()), this, SLOT(onExtractAll()));
}

bool QPlasmaPakFile::loadFile(QString filename)
{
    if (plEncryptedStream::IsFileEncrypted(filename.toUtf8().data())) {
        plEncryptedStream S(PlasmaVer::pvPrime);
        S.open(filename.toUtf8().data(), fmRead, plEncryptedStream::kEncAuto);
        if (S.getEncType() == plEncryptedStream::kEncDroid) {
            if (!GetEncryptionKeyFromUser(this, fDroidKey))
                return false;
            S.setKey(fDroidKey);
            fEncryption = kEncDroid;
        } else if (S.getEncType() == plEncryptedStream::kEncXtea) {
            fEncryption = kEncXtea;
        } else if (S.getEncType() == plEncryptedStream::kEncAES) {
            fEncryption = kEncAes;
            S.setVer(PlasmaVer::pvEoa);
        }
        if (!loadPakData(&S))
            return false;
    } else {
        hsFileStream S(PlasmaVer::pvMoul);
        S.open(filename.toUtf8().data(), fmRead);
        fEncryption = kEncNone;
        if (!loadPakData(&S))
            return false;
    }
    fFileList->resizeColumnToContents(1);
    fFileList->resizeColumnToContents(0);
    return QPlasmaDocument::loadFile(filename);
}

bool QPlasmaPakFile::saveTo(QString filename)
{
    if (fEncryption == kEncNone) {
        hsFileStream S(PlasmaVer::pvMoul);
        S.open(filename.toUtf8().data(), fmCreate);
        if (!savePakData(&S))
            return false;
    } else {
        plEncryptedStream S(PlasmaVer::pvPrime);
        plEncryptedStream::EncryptionType type = plEncryptedStream::kEncNone;
        if (fEncryption == kEncDroid) {
            if ((fFilename != filename) || isZeroKey(fDroidKey)) {
                if (!GetEncryptionKeyFromUser(this, fDroidKey))
                    return false;
            }
            S.setKey(fDroidKey);
            type = plEncryptedStream::kEncDroid;
        } else if (fEncryption == kEncAes) {
            type = plEncryptedStream::kEncAES;
            S.setVer(PlasmaVer::pvEoa);
        } else if (fEncryption == kEncXtea) {
            type = plEncryptedStream::kEncXtea;
        }
        S.open(filename.toUtf8().data(), fmCreate, type);
        if (!savePakData(&S))
            return false;
    }
    return QPlasmaDocument::saveTo(filename);
}

bool QPlasmaPakFile::loadPakData(hsStream* S)
{
    fFileList->clear();
    if (S != NULL) {
        try {
            fPackage.read(S);
        } catch (std::exception &e) {
            plDebug::Error("Error reading Package file {}: {}", fFilename.toUtf8().data(), e.what());
            return false;
        }
    }

    for (size_t i=0; i<fPackage.fEntries.size(); i++) {
        QTreeWidgetItem* ent = new QTreeWidgetItem(fFileList);
        ent->setText(0, fPackage.displayName(fPackage.fEntries[i]));
        ent->setText(1, fPackage.displaySize(fPackage.fEntries[i]));
        ent->setIcon(0, QPlasmaDocument::GetDocIcon(fPackage.displayName(fPackage.fEntries[i])));
        ent->setFlags(ent->flags() | Qt::ItemIsEditable);
    }
    return true;
}

bool QPlasmaPakFile::savePakData(hsStream* S)
{
    try {
        fPackage.write(S);
    } catch (std::exception &e) {
        plDebug::Error("Error writing package file {}: {}", fFilename.toUtf8().data(), e.what());
        return false;
    }
    return true;
}

void QPlasmaPakFile::onContextMenu(QPoint pos)
{
    bool itemSelected = true;
    if (fFileList->selectedItems().size() <= 1) {
        fFileList->setCurrentItem(fFileList->itemAt(pos));
        QTreeWidgetItem* item = (QTreeWidgetItem*)fFileList->currentItem();
        itemSelected = (item != NULL);
    }
    fActions[kDel]->setEnabled(itemSelected);
    fActions[kExtract]->setEnabled(itemSelected);

    QMenu menu(this);
    menu.addAction(fActions[kAdd]);
    menu.addAction(fActions[kDel]);
    menu.addSeparator();
    menu.addAction(fActions[kExtract]);
    menu.addAction(fActions[kExtractAll]);
    menu.exec(fFileList->viewport()->mapToGlobal(pos));
}

void QPlasmaPakFile::onAdd()
{
    QSettings settings("PlasmaShop", "PlasmaShop");
    QString gameRoot = settings.value("DialogDir", "").toString();
    QString curGame = settings.value("CurrentGame", QString()).toString();
    settings.beginGroup("Games");
    if (!curGame.isEmpty()) {
        QStringList gmParams = settings.value(curGame).toStringList();
        if (!gmParams.isEmpty())
            gameRoot = gmParams[0];
    }
    QStringList files = QFileDialog::getOpenFileNames(this, tr("Add / update files"),
                                                      gameRoot, fPackage.getFilter());
    foreach (QString f, files)
        fPackage.addFrom(f);

    if (files.size() != 0) {
        loadPakData(NULL);
        makeDirty();
    }
}

void QPlasmaPakFile::onDel()
{
    foreach (QTreeWidgetItem* item, fFileList->selectedItems()) {
        int idx = fFileList->indexOfTopLevelItem(item);
        fPackage.fEntries.erase(fPackage.fEntries.begin() + idx);
        delete item;
    }

    loadPakData(NULL);
    makeDirty();
}

void QPlasmaPakFile::onExtract()
{
    QSettings settings("PlasmaShop", "PlasmaShop");
    QString gameRoot = settings.value("DialogDir", "").toString();
    QString curGame = settings.value("CurrentGame", QString()).toString();
    settings.beginGroup("Games");
    if (!curGame.isEmpty()) {
        QStringList gmParams = settings.value(curGame).toStringList();
        if (!gmParams.isEmpty())
            gameRoot = gmParams[0];
    }
    QString dir = QFileDialog::getExistingDirectory(this, tr("Select Extract location"),
                                                    gameRoot);
    if (!dir.isEmpty()) {
        OverwritingConfirmation confirmation = Ask;
        foreach (QTreeWidgetItem* item, fFileList->selectedItems()) {
            int idx = fFileList->indexOfTopLevelItem(item);
            extract(fPackage.fEntries[idx], dir, &confirmation);
        }
    }
}

void QPlasmaPakFile::onExtractAll()
{
    QSettings settings("PlasmaShop", "PlasmaShop");
    QString gameRoot = settings.value("DialogDir", "").toString();
    QString curGame = settings.value("CurrentGame", QString()).toString();
    settings.beginGroup("Games");
    if (!curGame.isEmpty()) {
        QStringList gmParams = settings.value(curGame).toStringList();
        if (!gmParams.isEmpty())
            gameRoot = gmParams[0];
    }
    QString dir = QFileDialog::getExistingDirectory(this, tr("Select Extract location"),
                                                    gameRoot);
    if (!dir.isEmpty()) {
        std::vector<PlasmaPackage::FileEntry>::iterator iter;
        OverwritingConfirmation confirmation = Ask;
        for (iter = fPackage.fEntries.begin(); iter != fPackage.fEntries.end(); ++iter) {
             extract(*iter, dir, &confirmation);
        }
    }
}

void QPlasmaPakFile::extract(const PlasmaPackage::FileEntry& entry, QString dir, OverwritingConfirmation *confirmation)
{
    QString dispName = fPackage.displayName(entry);
    if (fPackage.fType == PlasmaPackage::kPythonPak)
        dispName.replace(".py", ".pyc");

    QString path = dir + QDir::separator() + dispName;
    if (QFileInfo(path).exists()) {
        int result;
        if (*confirmation == YesToAll) {
            result = QMessageBox::Yes;
        } else if (*confirmation == NoToAll) {
            result = QMessageBox::No;
        } else {
            result = QMessageBox::question(this, tr("Replace file"),
                                tr("File %1 already exists.  Would you like to replace it?").arg(dispName),
                                QMessageBox::Yes | QMessageBox::No | QMessageBox::YesToAll | QMessageBox::NoToAll);
        }
        if (result == QMessageBox::YesToAll)
            *confirmation = YesToAll;
        else if (result == QMessageBox::NoToAll)
            *confirmation = NoToAll;
        if (result == QMessageBox::No || result == QMessageBox::NoToAll)
            return;
    }
    fPackage.writeToFile(entry, path);
    if (fPackage.fType == PlasmaPackage::kPythonPak)
        decompylePyc(this, path, confirmation);
}

bool QPlasmaPakFile::decompylePyc(QWidget *parent, QString filename, OverwritingConfirmation *confirmation)
{
    QString output = filename;
    output.replace(".pyc", ".py");

    if (QFile::exists(output)) {
        QMessageBox::StandardButtons buttons = QMessageBox::Yes | QMessageBox::No;
        int replace = 0;
        if (confirmation) {
            if (*confirmation == YesToAll)
                replace = QMessageBox::Yes;
            else if (*confirmation == NoToAll)
                replace = QMessageBox::No;
            else
                buttons |= QMessageBox::YesToAll | QMessageBox::NoToAll;
        }
        if (!replace) {
            replace = QMessageBox::question(parent, QObject::tr("File Exists"),
                              QObject::tr("File %1 already exists.  Would you like to replace it?").arg(output),
                              buttons);
        }
        if (replace == QMessageBox::YesToAll)
            *confirmation = YesToAll;
        else if (replace == QMessageBox::NoToAll)
            *confirmation = NoToAll;
        if (replace == QMessageBox::No || replace == QMessageBox::NoToAll)
            return true;
    }

    PycModule mod;
    mod.loadFromFile(filename.toUtf8().data());
    if (!mod.isValid()) {
        QMessageBox::critical(parent, QObject::tr("Decompyle Error"),
                              QObject::tr("Could not load file %1").arg(filename));
        return false;
    }

    pyc_output = fopen(output.toUtf8().data(), "w");
    fprintf(pyc_output, "# Source generated with PlasmaShop " PLASMASHOP_VERSION "\n");
    fprintf(pyc_output, "# Powered by Decompyle++\n");
    fprintf(pyc_output, "# File: %s (Python %d.%d%s)\n\n", QFileInfo(output).fileName().toUtf8().data(),
            mod.majorVer(), mod.minorVer(), (mod.majorVer() < 3 && mod.isUnicode()) ? " Unicode" : "");
    decompyle(mod.code(), &mod);
    fclose(pyc_output);
    return true;
}
