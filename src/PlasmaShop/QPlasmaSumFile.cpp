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
#include <QToolBar>
#include <QFileDialog>
#include <QSettings>
#include <QCryptographicHash>
#include "QPlasma.h"

void QPlasmaSumFile::addToSumFile(const QString& filename)
{
    QFileInfo finfo(filename);

    ST::string sumPath;
    uint32_t timestamp = finfo.lastModified().toTime_t();

    // Construct a default path based on the file extension
    if (finfo.suffix() == "prp" || finfo.suffix() == "fni" || finfo.suffix() == "age" ||
        finfo.suffix() == "pfp" || finfo.suffix() == "p2f" || finfo.suffix() == "dat" ||
        finfo.suffix() == "sub" || finfo.suffix() == "loc" || finfo.suffix() == "csv" ||
        finfo.suffix() == "node")
        sumPath = ST::format("dat\\{}", finfo.fileName());
    else if (finfo.suffix() == "ogg")
        sumPath = ST::format("sfx\\{}", finfo.fileName());
    else if (finfo.suffix() == "pak")
        sumPath = ST::format("Python\\{}", finfo.fileName());
    else if (finfo.suffix() == "sdl")
        sumPath = ST::format("SDL\\{}", finfo.fileName());
    else
        sumPath = qstr2st(finfo.fileName());

    // Calculate updated MD5 hash
    hsFileStream FS;
    FS.open(qstr2st(filename), fmRead);
    fSumData.updateFile(sumPath, &FS, timestamp);
}


struct TreeSignalBlocker
{
    TreeSignalBlocker(QTreeWidget *tree) : fTree(tree)
    {
        fTree->blockSignals(true);
    }

    ~TreeSignalBlocker()
    {
        fTree->blockSignals(false);
    }

    QTreeWidget *fTree;
};

/* QPlasmaSumFile */
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
    fFileList->setHeaderLabels(QStringList{tr("Filename"), tr("Timestamp"), tr("MD5")});

    QToolBar* toolbar = new QToolBar(this);
    toolbar->setOrientation(Qt::Vertical);
    toolbar->setToolButtonStyle(Qt::ToolButtonIconOnly);
    toolbar->setIconSize(QSize(22, 22));

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(4, 4, 4, 4);
    layout->addWidget(fFileList, 0, 0);
    layout->addWidget(toolbar, 0, 1);
    setLayout(layout);

    fActions[kAUpdate] = new QAction(qStdIcon("view-refresh"), tr("&Update all..."), this);
    fActions[kAAdd] = new QAction(qStdIcon("list-add"), tr("&Add / Update..."), this);
    fActions[kADel] = new QAction(qStdIcon("list-remove"), tr("&Remove"), this);

    toolbar->addAction(fActions[kAUpdate]);
    toolbar->addSeparator();
    toolbar->addAction(fActions[kAAdd]);
    toolbar->addAction(fActions[kADel]);

    connect(fFileList, &QTreeWidget::customContextMenuRequested,
            this, &QPlasmaSumFile::onContextMenu);
    connect(fFileList, &QTreeWidget::itemChanged,
            this, &QPlasmaSumFile::onItemChanged);
    connect(fActions[kAUpdate], &QAction::triggered, this, &QPlasmaSumFile::onUpdate);
    connect(fActions[kAAdd], &QAction::triggered, this, &QPlasmaSumFile::onAdd);
    connect(fActions[kADel], &QAction::triggered, this, &QPlasmaSumFile::onDel);
}

bool QPlasmaSumFile::loadFile(QString filename)
{
    ST::string st_filename = qstr2st(filename);
    if (plEncryptedStream::IsFileEncrypted(st_filename)) {
        plEncryptedStream S(PlasmaVer::pvPrime);
        S.open(st_filename, fmRead, plEncryptedStream::kEncAuto);
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
        if (!loadSumData(&S))
            return false;
    } else {
        hsFileStream S(PlasmaVer::pvMoul);
        S.open(st_filename, fmRead);
        fEncryption = kEncNone;
        if (!loadSumData(&S))
            return false;
    }
    fFileList->resizeColumnToContents(2);
    fFileList->resizeColumnToContents(1);
    fFileList->resizeColumnToContents(0);
    return QPlasmaDocument::loadFile(filename);
}

bool QPlasmaSumFile::saveTo(QString filename)
{
    ST::string st_filename = qstr2st(filename);
    if (fEncryption == kEncNone) {
        hsFileStream S(PlasmaVer::pvMoul);
        S.open(st_filename, fmCreate);
        if (!saveSumData(&S))
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
        S.open(st_filename, fmCreate, type);
        if (!saveSumData(&S))
            return false;
    }
    return QPlasmaDocument::saveTo(filename);
}

bool QPlasmaSumFile::loadSumData(hsStream* S)
{
    fFileList->clear();
    if (S != nullptr) {
        try {
            fSumData.read(S);
        } catch (const std::exception &e) {
            plDebug::Error("Error reading SUM file {}: {}", fFilename, e.what());
            return false;
        }
    }

    TreeSignalBlocker block(fFileList);
    for (const hsSumFile::FileInfo& file : fSumData.getFiles()) {
        QTreeWidgetItem* ent = new QTreeWidgetItem(fFileList);
        QString qpath = st2qstr(file.fPath);
        ent->setText(0, qpath);
        ent->setText(1, QDateTime::fromTime_t(file.fTimestamp)
                                  .toString(Qt::SystemLocaleShortDate));
        ent->setText(2, st2qstr(file.fHash.toHex()));
        ent->setIcon(0, QPlasmaDocument::GetDocIcon(qpath));
        ent->setFlags(ent->flags() | Qt::ItemIsEditable);
    }
    return true;
}

bool QPlasmaSumFile::saveSumData(hsStream* S)
{
    try {
        fSumData.write(S);
    } catch (const std::exception &e) {
        plDebug::Error("Error writing SUM file {}: {}", fFilename, e.what());
        return false;
    }
    return true;
}

void QPlasmaSumFile::onContextMenu(QPoint pos)
{
    if (fFileList->selectedItems().size() <= 1) {
        fFileList->setCurrentItem(fFileList->itemAt(pos));
        QTreeWidgetItem* item = fFileList->currentItem();
        if (item == nullptr)
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
    TreeSignalBlocker block(fFileList);

    int idx = fFileList->indexOfTopLevelItem(item);
    hsSumFile::FileInfo& file = fSumData.getFiles().at(idx);
    switch (column) {
    case 0:
        file.fPath = qstr2st(item->text(0));
        item->setIcon(0, QPlasmaDocument::GetDocIcon(item->text(0)));
        makeDirty();
        break;
    case 1:
        {
            QDateTime ts = QDateTime::fromString(item->text(1), Qt::SystemLocaleShortDate);
            if (!ts.isValid()) {
                QMessageBox::critical(this, tr("Error"),
                                      tr("Invalid date/time: %1").arg(item->text(1)),
                                      QMessageBox::Ok);
                item->setText(1, QDateTime::fromTime_t(file.fTimestamp)
                                           .toString(Qt::SystemLocaleShortDate));
                break;
            }
            file.fTimestamp = ts.toTime_t();
            makeDirty();
        }
        break;
    case 2:
        {
            QString h = item->text(2);
            try {
                file.fHash.fromHex(h.toUtf8().constData());
            } catch (const hsBadParamException&) {
                QMessageBox::critical(this, tr("Error"), tr("Invalid MD5 hash"),
                                      QMessageBox::Ok);
                item->setText(2, st2qstr(file.fHash.toHex()));
                break;
            }
            makeDirty();
        }
    }
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
        // Need a copy since addToSumFile() can invalidate iterators
        const std::vector<hsSumFile::FileInfo> curFiles = fSumData.getFiles();
        for (const hsSumFile::FileInfo& file : curFiles) {
            QString path = st2qstr(file.fPath);
            path.replace('\\', QDir::separator()).replace('/', QDir::separator());
            path = dir + QDir::separator() + QFileInfo(path).fileName();
            if (QFileInfo(path).exists())
                addToSumFile(path);
        }

        loadSumData(nullptr);
        makeDirty();
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
    QStringList files = QFileDialog::getOpenFileNames(this, tr("Add / update files"),
                                                      gameRoot, QString("All Files (*)"));
    for (const QString& f : files)
        addToSumFile(f);

    if (files.size() != 0) {
        loadSumData(nullptr);
        makeDirty();
    }
}

void QPlasmaSumFile::onDel()
{
    for (QTreeWidgetItem* item : fFileList->selectedItems()) {
        int idx = fFileList->indexOfTopLevelItem(item);
        fSumData.getFiles().erase(fSumData.getFiles().begin() + idx);
        delete item;
    }

    loadSumData(nullptr);
    makeDirty();
}
