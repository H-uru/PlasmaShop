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

#include "QPlasmaSumFile.h"
#include <Debug/plDebug.h>
#include <Stream/plEncryptedStream.h>
#include <QDateTime>
#include <QGridLayout>
#include <QMessageBox>
#include <QMenu>
#include <QFileDialog>
#include <QSettings>
#include "../QPlasma.h"

/* SumData */
void SumData::read(hsStream* S)
{
    fEntries.resize(S->readInt());
    fUnknown = S->readInt();
    for (size_t i=0; i<fEntries.size(); i++) {
        fEntries[i].fPath = S->readSafeStr();
        fEntries[i].fHash.read(S);
        fEntries[i].fTimestamp = S->readInt();
        fEntries[i].fUnknown = S->readInt();
    }
}

void SumData::write(hsStream* S)
{
    S->writeInt(fEntries.size());
    S->writeInt(fUnknown);
    for (size_t i=0; i<fEntries.size(); i++) {
        S->writeSafeStr(fEntries[i].fPath);
        fEntries[i].fHash.write(S);
        S->writeInt(fEntries[i].fTimestamp);
        S->writeInt(fEntries[i].fUnknown);
    }
}


/* QPlasmaSumFile */
static int s_editLock = 0;

QPlasmaSumFile::QPlasmaSumFile(QWidget* parent)
              : QPlasmaDocument(kDocManifest, parent)
{
    fFileList = new QTreeWidget(this);
    fFileList->setUniformRowHeights(true);
    fFileList->setRootIsDecorated(false);
    fFileList->setEditTriggers(QAbstractItemView::SelectedClicked |
                               QAbstractItemView::EditKeyPressed);
    fFileList->setContextMenuPolicy(Qt::CustomContextMenu);
    fFileList->setSelectionMode(QAbstractItemView::ExtendedSelection);
    fFileList->setHeaderLabels(QStringList() << "Filename" << "Timestamp" << "MD5");

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(4, 4, 4, 4);
    layout->addWidget(fFileList, 0, 0);
    setLayout(layout);

    fActions[kAUpdate] = new QAction(tr("&Update &all..."), this);
    fActions[kAAdd] = new QAction(tr("&Add / Update..."), this);
    fActions[kADel] = new QAction(tr("&Remove"), this);

    connect(fFileList, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(onContextMenu(QPoint)));
    connect(fFileList, SIGNAL(itemChanged(QTreeWidgetItem*, int)),
            this, SLOT(onItemChanged(QTreeWidgetItem*, int)));
    connect(fActions[kAUpdate], SIGNAL(triggered()), this, SLOT(onUpdate()));
    connect(fActions[kAAdd], SIGNAL(triggered()), this, SLOT(onAdd()));
    connect(fActions[kADel], SIGNAL(triggered()), this, SLOT(onDel()));
}

bool QPlasmaSumFile::loadFile(QString filename)
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
        loadSumData(&S);
    } else {
        hsFileStream S(PlasmaVer::pvMoul);
        S.open(filename.toUtf8().data(), fmRead);
        fEncryption = kEncNone;
        loadSumData(&S);
    }
    fFileList->resizeColumnToContents(2);
    fFileList->resizeColumnToContents(1);
    fFileList->resizeColumnToContents(0);
    return QPlasmaDocument::loadFile(filename);
}

bool QPlasmaSumFile::saveTo(QString filename)
{
    if (fEncryption == kEncNone) {
        hsFileStream S(PlasmaVer::pvMoul);
        S.open(filename.toUtf8().data(), fmCreate);
        saveSumData(&S);
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
        saveSumData(&S);
    }
    return QPlasmaDocument::saveTo(filename);
}

bool QPlasmaSumFile::loadSumData(hsStream* S)
{
    fFileList->clear();
    try {
        fSumData.read(S);
    } catch (...) {
        plDebug::Error("Error reading SUM file %s", fFilename.toUtf8().data());
        return false;
    }

    ++s_editLock;
    for (size_t i=0; i<fSumData.fEntries.size(); i++) {
        QTreeWidgetItem* ent = new QTreeWidgetItem(fFileList);
        ent->setText(0, ~fSumData.fEntries[i].fPath);
        ent->setText(1, QDateTime::fromTime_t(fSumData.fEntries[i].fTimestamp)
                                  .toString(Qt::SystemLocaleShortDate));
        ent->setText(2, ~fSumData.fEntries[i].fHash.toHex());
        ent->setIcon(0, QPlasmaDocument::GetDocIcon(~fSumData.fEntries[i].fPath));
        ent->setFlags(ent->flags() | Qt::ItemIsEditable);
    }
    --s_editLock;
    return true;
}

bool QPlasmaSumFile::saveSumData(hsStream* S)
{
    try {
        fSumData.write(S);
    } catch (...) {
        plDebug::Error("Error writing SUM file %s", fFilename.toUtf8().data());
        return false;
    }
    return true;
}

void QPlasmaSumFile::onContextMenu(QPoint pos)
{
    if (fFileList->selectedItems().size() <= 1) {
        fFileList->setCurrentItem(fFileList->itemAt(pos));
        QTreeWidgetItem* item = (QTreeWidgetItem*)fFileList->currentItem();
        if (item == NULL)
            fActions[kADel]->setEnabled(false);
        else
            fActions[kADel]->setEnabled(true);
    } else {
        fActions[kADel]->setEnabled(true);
    }

    QMenu menu(this);
    menu.addAction(fActions[kAUpdate]);
    menu.addSeparator();
    menu.addAction(fActions[kAAdd]);
    menu.addAction(fActions[kADel]);
    menu.exec(fFileList->viewport()->mapToGlobal(pos));
}

void QPlasmaSumFile::onItemChanged(QTreeWidgetItem* item, int column)
{
    if (s_editLock != 0)
        return;

    ++s_editLock;
    int idx = fFileList->indexOfTopLevelItem(item);
    switch (column) {
    case 0:
        if (fSumData.fEntries[idx].fPath != ~item->text(0)) {
            fSumData.fEntries[idx].fPath = ~item->text(0);
            item->setIcon(0, QPlasmaDocument::GetDocIcon(~fSumData.fEntries[idx].fPath));
            makeDirty();
        }
        break;
    case 1:
        {
            QDateTime ts = QDateTime::fromString(item->text(1), Qt::SystemLocaleShortDate);
            if (!ts.isValid()) {
                QMessageBox::critical(this, tr("Error"),
                                      tr("Invalid date/time: %1").arg(item->text(1)),
                                      QMessageBox::Ok);
                item->setText(1, QDateTime::fromTime_t(fSumData.fEntries[idx].fTimestamp)
                                           .toString(Qt::SystemLocaleShortDate));
                break;
            }
            fSumData.fEntries[idx].fTimestamp = ts.toTime_t();
            makeDirty();
        }
        break;
    case 2:
        {
            QString h = item->text(2);
            if (h.length() != 32) {
                QMessageBox::critical(this, tr("Error"), tr("Invalid MD5 hash"),
                                      QMessageBox::Ok);
                item->setText(2, ~fSumData.fEntries[idx].fHash.toHex());
                break;
            }
            quint32 newhash[4];
            bool ok = true;
            if (ok) newhash[0] = h.mid(0, 8).toUInt(&ok, 16);
            if (ok) newhash[1] = h.mid(8, 8).toUInt(&ok, 16);
            if (ok) newhash[2] = h.mid(16, 8).toUInt(&ok, 16);
            if (ok) newhash[3] = h.mid(24, 8).toUInt(&ok, 16);
            if (!ok) {
                QMessageBox::critical(this, tr("Error"), tr("Invalid MD5 hash"),
                                      QMessageBox::Ok);
                item->setText(2, ~fSumData.fEntries[idx].fHash.toHex());
                break;
            }
            memcpy(fSumData.fEntries[idx].fHash.fHash, newhash, 32);
            makeDirty();
        }
    }
    --s_editLock;
}

void QPlasmaSumFile::onUpdate()
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
    QString dir = QFileDialog::getExistingDirectory(this, tr("Select Game directory"),
                                                    gameRoot);
    if (!dir.isEmpty()) {
        // TODO
    }
}

void QPlasmaSumFile::onAdd()
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
    QStringList files = QFileDialog::getOpenFileNames(this, tr("Add files to checksum"),
                                                      gameRoot, QString("All Files (*)"));
    foreach (QString f, files) {
        // TODO
    }
}

void QPlasmaSumFile::onDel()
{
    foreach (QTreeWidgetItem* item, fFileList->selectedItems()) {
        int idx = fFileList->indexOfTopLevelItem(item);
        fSumData.fEntries.erase(fSumData.fEntries.begin() + idx);
        delete item;
    }
    makeDirty();
}
