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

#include "GameScanner.h"

#include "GameBrowser.h"
#include "QPlasmaDocument.h"

GameScanner::GameScanner(QTreeWidget* tree)
    : fTree(tree), fGameType(GameInfo::kGameNone) { }

void GameScanner::scanGame(int gameType, QString gameDir, QString appDataDir,
                           QString docsDir)
{
    fGameType = gameType;
    fGameDir = gameDir;
    fAppDataDir = appDataDir;
    fDocumentsDir = docsDir;
    fTree->clear();
    if (fGameType == GameInfo::kGameNone)
        return;

    fRootItem = new QTreeWidgetItem(fTree);
    fRootItem->setIcon(0, QPlasmaDocument::GetDocIcon("<FOLDER>"));
    fRootItem->setText(0, tr("(Game Root)"));
    fRootItem->setData(0, Qt::UserRole, "<FOLDER>");

    fAppDataItem = new QTreeWidgetItem(fTree);
    fAppDataItem->setIcon(0, QPlasmaDocument::GetDocIcon("<FOLDER>"));
    fAppDataItem->setText(0, tr("(Game Data - AppData)"));
    fAppDataItem->setData(0, Qt::UserRole, "<FOLDER>");

    fDocumentsItem = new QTreeWidgetItem(fTree);
    fDocumentsItem->setIcon(0, QPlasmaDocument::GetDocIcon("<FOLDER>"));
    fDocumentsItem->setText(0, tr("(Game Data - Documents)"));
    fDocumentsItem->setData(0, Qt::UserRole, "<FOLDER>");

    fAgesItem = new QTreeWidgetItem(fTree);
    fAgesItem->setIcon(0, QPlasmaDocument::GetDocIcon("<AGEFOLD>"));
    fAgesItem->setText(0, tr("Ages"));
    fAgesItem->setData(0, Qt::UserRole, "<AGEFOLD>");

    fFontsItem = new QTreeWidgetItem(fTree);
    fFontsItem->setIcon(0, QPlasmaDocument::GetDocIcon("<FONTFOLD>"));
    fFontsItem->setText(0, tr("Fonts"));
    fFontsItem->setData(0, Qt::UserRole, "<FONTFOLD>");

    fImagesItem = new QTreeWidgetItem(fTree);
    fImagesItem->setIcon(0, QPlasmaDocument::GetDocIcon("<IMGFOLD>"));
    fImagesItem->setText(0, tr("Images"));
    fImagesItem->setData(0, Qt::UserRole, "<IMGFOLD>");

    fLevelsItem = new QTreeWidgetItem(fTree);
    fLevelsItem->setIcon(0, QPlasmaDocument::GetDocIcon("<AGEFOLD>"));
    fLevelsItem->setText(0, tr("Levels"));
    fLevelsItem->setData(0, Qt::UserRole, "<AGEFOLD>");

    fLocalizationItem = new QTreeWidgetItem(fTree);
    fLocalizationItem->setIcon(0, QPlasmaDocument::GetDocIcon("<DATAFOLD>"));
    fLocalizationItem->setText(0, tr("Localization"));
    fLocalizationItem->setData(0, Qt::UserRole, "<DATAFOLD>");

    fLogItem = new QTreeWidgetItem(fTree);
    fLogItem->setIcon(0, QPlasmaDocument::GetDocIcon("<DATAFOLD>"));
    fLogItem->setText(0, tr("Logs"));
    fLogItem->setData(0, Qt::UserRole, "<DATAFOLD>");

    fPythonItem = new QTreeWidgetItem(fTree);
    fPythonItem->setIcon(0, QPlasmaDocument::GetDocIcon("<PYFOLD>"));
    fPythonItem->setText(0, tr("Python"));
    fPythonItem->setData(0, Qt::UserRole, "<PYFOLD>");

    fSDLItem = new QTreeWidgetItem(fTree);
    fSDLItem->setIcon(0, QPlasmaDocument::GetDocIcon("<SDLFOLD>"));
    fSDLItem->setText(0, tr("SDL"));
    fSDLItem->setData(0, Qt::UserRole, "<SDLFOLD>");

    fSettingsItem = new QTreeWidgetItem(fTree);
    fSettingsItem->setIcon(0, QPlasmaDocument::GetDocIcon("<DATAFOLD>"));
    fSettingsItem->setText(0, tr("Settings"));
    fSettingsItem->setData(0, Qt::UserRole, "<DATAFOLD>");

    fShadersItem = new QTreeWidgetItem(fTree);
    fShadersItem->setIcon(0, QPlasmaDocument::GetDocIcon("<DATAFOLD>"));
    fShadersItem->setText(0, tr("Shaders"));
    fShadersItem->setData(0, Qt::UserRole, "<DATAFOLD>");

    // Perform the scan -- hide everything until the initial update is done
    fAgesItem->setHidden(true);
    fFontsItem->setHidden(true);
    fImagesItem->setHidden(true);
    fLevelsItem->setHidden(true);
    fLocalizationItem->setHidden(true);
    fLogItem->setHidden(true);
    fPythonItem->setHidden(true);
    fSDLItem->setHidden(true);
    fSettingsItem->setHidden(true);
    fShadersItem->setHidden(true);
    scan();
}

void GameScanner::refresh()
{
    if (fGameType == GameInfo::kGameNone)
        return;

    // Perform the scan
    scan();
}

QTreeWidgetItem* GameScanner::ensurePath(QTreeWidgetItem* root, QStringList path,
                                         QString folderType)
{
    if (path.size() == 0)
        return root;

    QString p = path[0];
    path = path.mid(1);
    for (int i=0; i<root->childCount(); i++) {
        if (root->child(i)->text(0) == p)
            return ensurePath(root->child(i), path, folderType);
    }

    // Path component not found
    QTreeWidgetItem* folder = new QTreeWidgetItem(root);
    folder->setIcon(0, QPlasmaDocument::GetDocIcon(folderType));
    folder->setText(0, p);
    folder->setData(0, Qt::UserRole, folderType);
    return ensurePath(folder, path, folderType);
}

void GameScanner::recursiveScan(QStringList path, QDir root)
{
    bool isDat = (path.size() > 0) && (path[0].toLower() == "dat");
    bool isReallyDat = (path.size() == 1) && (path[0].toLower() == "dat");
    bool isLog = (path.size() > 0) && (path[0].toLower() == "log");
    bool isPy = (path.size() > 0) && (path[0].toLower() == "python");
    bool isSdl = (path.size() > 0) && (path[0].toLower() == "sdl");
    bool isFx = (path.size() > 0) && (path[0].toLower() == "fx");
    QStringList datbPath = isDat ? path.mid(1) : path;

    QStringList subdirs = root.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    foreach (QString dir, subdirs) {
        if (!root.cd(dir))
            continue;
        path.push_back(dir);
        recursiveScan(path, root);
        path.pop_back();
        root.cdUp();
    }

    QStringList files = root.entryList(QDir::Files);
    foreach (QString f, files) {
        QString ftest = f.toLower();
        if (isReallyDat && ftest.endsWith(".prp")) {
            QStringList parts = f.split('_');
            QTreeWidgetItem* fold = ensurePath(fAgesItem,
                QStringList() << parts.first(), "<AGEFOLD>");
            QString fnameDisp = f.mid(parts.first().size() + 1);
            if (fnameDisp.toLower().startsWith("District_"))
                fnameDisp = fnameDisp.mid(9);
            QTreeWidgetItem* item = new QTreeWidgetItem(fold);
            item->setIcon(0, QPlasmaDocument::GetDocIcon(f));
            item->setText(0, fnameDisp);
            item->setData(0, Qt::UserRole, root.absoluteFilePath(f));
        } else if (isReallyDat && (ftest.endsWith(".age") || ftest.endsWith(".csv") ||
                                   ftest.endsWith(".fni") || ftest.endsWith(".sum"))) {
            QString name = f.left(f.length() - 4);  // Trim the extension
            QTreeWidgetItem* fold = ensurePath(fAgesItem,
                QStringList() << name, "<AGEFOLD>");
            QTreeWidgetItem* item = new QTreeWidgetItem(fold);
            item->setIcon(0, QPlasmaDocument::GetDocIcon(f));
            item->setText(0, f);
            item->setData(0, Qt::UserRole, root.absoluteFilePath(f));
        } else if (isReallyDat && ftest.endsWith(".hex")) {
            QTreeWidgetItem* fold = ensurePath(fLevelsItem, datbPath, "<AGEFOLD>");
            QTreeWidgetItem* item = new QTreeWidgetItem(fold);
            item->setIcon(0, QPlasmaDocument::GetDocIcon(f));
            item->setText(0, f);
            item->setData(0, Qt::UserRole, root.absoluteFilePath(f));
        } else if (ftest.endsWith(".pfp") || ftest.endsWith(".p2f")) {
            QTreeWidgetItem* fold = ensurePath(fFontsItem, datbPath, "<FONTFOLD>");
            QTreeWidgetItem* item = new QTreeWidgetItem(fold);
            item->setIcon(0, QPlasmaDocument::GetDocIcon(f));
            item->setText(0, f);
            item->setData(0, Qt::UserRole, root.absoluteFilePath(f));
        } else if (ftest == "cursors.dat" || ftest.endsWith(".tga") ||
                   ftest == "resource.dat") {
            QTreeWidgetItem* fold = ensurePath(fImagesItem, datbPath, "<IMGFOLD>");
            QTreeWidgetItem* item = new QTreeWidgetItem(fold);
            item->setIcon(0, QPlasmaDocument::GetDocIcon(f));
            item->setText(0, f);
            item->setData(0, Qt::UserRole, root.absoluteFilePath(f));
        } else if (ftest.endsWith(".jpg")) {
            QTreeWidgetItem* fold = ensurePath(fImagesItem, path, "<IMGFOLD>");
            QTreeWidgetItem* item = new QTreeWidgetItem(fold);
            item->setIcon(0, QPlasmaDocument::GetDocIcon(f));
            item->setText(0, f);
            item->setData(0, Qt::UserRole, root.absoluteFilePath(f));
        } else if (ftest.endsWith(".loc") || ftest.endsWith(".sub")) {
            QTreeWidgetItem* fold = ensurePath(fLocalizationItem, datbPath, "<DATAFOLD>");
            QTreeWidgetItem* item = new QTreeWidgetItem(fold);
            item->setIcon(0, QPlasmaDocument::GetDocIcon(f));
            item->setText(0, f);
            item->setData(0, Qt::UserRole, root.absoluteFilePath(f));
        } else if (isLog && (ftest.endsWith(".txt") || ftest.endsWith(".log") ||
                             ftest.endsWith(".elf"))) {
            QTreeWidgetItem* fold = ensurePath(fLogItem, path.mid(1), "<DATAFOLD>");
            QTreeWidgetItem* item = new QTreeWidgetItem(fold);
            item->setIcon(0, QPlasmaDocument::GetDocIcon(f));
            item->setText(0, f);
            item->setData(0, Qt::UserRole, root.absoluteFilePath(f));
        } else if (isPy && (ftest.endsWith(".py") || ftest.endsWith(".pak"))) {
            QTreeWidgetItem* fold = ensurePath(fPythonItem, path.mid(1), "<PYFOLD>");
            QTreeWidgetItem* item = new QTreeWidgetItem(fold);
            item->setIcon(0, QPlasmaDocument::GetDocIcon(f));
            item->setText(0, f);
            item->setData(0, Qt::UserRole, root.absoluteFilePath(f));
        } else if (isSdl && ftest.endsWith(".sdl")) {
            QTreeWidgetItem* fold = ensurePath(fSDLItem, path.mid(1), "<SDLFOLD>");
            QTreeWidgetItem* item = new QTreeWidgetItem(fold);
            item->setIcon(0, QPlasmaDocument::GetDocIcon(f));
            item->setText(0, f);
            item->setData(0, Qt::UserRole, root.absoluteFilePath(f));
        } else if (isFx && ftest.endsWith(".fx")) {
            QTreeWidgetItem* fold = ensurePath(fShadersItem, path.mid(1), "<DATAFOLD>");
            QTreeWidgetItem* item = new QTreeWidgetItem(fold);
            item->setIcon(0, QPlasmaDocument::GetDocIcon(f));
            item->setText(0, f);
            item->setData(0, Qt::UserRole, root.absoluteFilePath(f));
        } else if (ftest.endsWith(".fni") || ftest.endsWith(".ini") ||
                   ftest.endsWith(".cfg") || ftest == "dev_mode.dat") {
            QTreeWidgetItem* fold = ensurePath(fSettingsItem, path, "<DATAFOLD>");
            QTreeWidgetItem* item = new QTreeWidgetItem(fold);
            item->setIcon(0, QPlasmaDocument::GetDocIcon(f));
            item->setText(0, f);
            item->setData(0, Qt::UserRole, root.absoluteFilePath(f));
        } else {
            QTreeWidgetItem* fold = ensurePath(fRootItem, path, "<FOLDER>");
            QTreeWidgetItem* item = new QTreeWidgetItem(fold);
            item->setIcon(0, QPlasmaDocument::GetDocIcon(f));
            item->setText(0, f);
            item->setData(0, Qt::UserRole, root.absoluteFilePath(f));
        }
    }
}

void GameScanner::recursiveAppDataScan(QStringList path, QDir root)
{
    QStringList subdirs = root.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    foreach (QString dir, subdirs) {
        if (!root.cd(dir))
            continue;
        path.push_back(dir);
        recursiveAppDataScan(path, root);
        path.pop_back();
        root.cdUp();
    }

    QStringList files = root.entryList(QDir::Files);
    foreach (QString f, files) {
        QString ftest = f.toLower();
        if (ftest.endsWith(".fni") || ftest.endsWith(".ini") ||
            ftest.endsWith(".cfg") || ftest.endsWith(".hex") ||
            ftest == "dev_mode.dat") {
            QTreeWidgetItem* sfold = ensurePath(fSettingsItem,
                QStringList() << "(App Data)", "<FOLDER>");
            QTreeWidgetItem* fold = ensurePath(sfold, path, "<DATAFOLD>");
            QTreeWidgetItem* item = new QTreeWidgetItem(fold);
            item->setIcon(0, QPlasmaDocument::GetDocIcon(f));
            item->setText(0, f);
            item->setData(0, Qt::UserRole, root.absoluteFilePath(f));
        } else if (ftest.endsWith(".jpg")) {
            QTreeWidgetItem* sfold = ensurePath(fImagesItem,
                QStringList() << "(App Data)", "<FOLDER>");
            QTreeWidgetItem* fold = ensurePath(sfold, path, "<IMGFOLD>");
            QTreeWidgetItem* item = new QTreeWidgetItem(fold);
            item->setIcon(0, QPlasmaDocument::GetDocIcon(f));
            item->setText(0, f);
            item->setData(0, Qt::UserRole, root.absoluteFilePath(f));
        } else if (ftest.endsWith(".txt") || ftest.endsWith(".log") || ftest.endsWith(".elf")) {
            QTreeWidgetItem* sfold = ensurePath(fLogItem,
                QStringList() << "(App Data)", "<FOLDER>");
            QTreeWidgetItem* fold = ensurePath(sfold, path, "<DATAFOLD>");
            QTreeWidgetItem* item = new QTreeWidgetItem(fold);
            item->setIcon(0, QPlasmaDocument::GetDocIcon(f));
            item->setText(0, f);
            item->setData(0, Qt::UserRole, root.absoluteFilePath(f));
        } else {
            QTreeWidgetItem* fold = ensurePath(fAppDataItem, path, "<FOLDER>");
            QTreeWidgetItem* item = new QTreeWidgetItem(fold);
            item->setIcon(0, QPlasmaDocument::GetDocIcon(f));
            item->setText(0, f);
            item->setData(0, Qt::UserRole, root.absoluteFilePath(f));
        }
    }
}

void GameScanner::recursiveDocumentScan(QStringList path, QDir root)
{
    QStringList subdirs = root.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    foreach (QString dir, subdirs) {
        if (!root.cd(dir))
            continue;
        path.push_back(dir);
        recursiveDocumentScan(path, root);
        path.pop_back();
        root.cdUp();
    }

    QStringList files = root.entryList(QDir::Files);
    foreach (QString f, files) {
        QString ftest = f.toLower();
        if (ftest.endsWith(".fni") || ftest.endsWith(".ini") ||
            ftest.endsWith(".cfg") || ftest.endsWith(".hex") ||
            ftest == "dev_mode.dat") {
            QTreeWidgetItem* sfold = ensurePath(fSettingsItem,
                QStringList() << "(Documents)", "<FOLDER>");
            QTreeWidgetItem* fold = ensurePath(sfold, path, "<DATAFOLD>");
            QTreeWidgetItem* item = new QTreeWidgetItem(fold);
            item->setIcon(0, QPlasmaDocument::GetDocIcon(f));
            item->setText(0, f);
            item->setData(0, Qt::UserRole, root.absoluteFilePath(f));
        } else if (ftest.endsWith(".jpg")) {
            QTreeWidgetItem* sfold = ensurePath(fImagesItem,
                QStringList() << "(Documents)", "<FOLDER>");
            QTreeWidgetItem* fold = ensurePath(sfold, path, "<IMGFOLD>");
            QTreeWidgetItem* item = new QTreeWidgetItem(fold);
            item->setIcon(0, QPlasmaDocument::GetDocIcon(f));
            item->setText(0, f);
            item->setData(0, Qt::UserRole, root.absoluteFilePath(f));
        } else if (ftest.endsWith(".txt") || ftest.endsWith(".log") || ftest.endsWith(".elf")) {
            QTreeWidgetItem* sfold = ensurePath(fLogItem,
                QStringList() << "(Documents)", "<FOLDER>");
            QTreeWidgetItem* fold = ensurePath(sfold, path, "<DATAFOLD>");
            QTreeWidgetItem* item = new QTreeWidgetItem(fold);
            item->setIcon(0, QPlasmaDocument::GetDocIcon(f));
            item->setText(0, f);
            item->setData(0, Qt::UserRole, root.absoluteFilePath(f));
        } else {
            QTreeWidgetItem* fold = ensurePath(fDocumentsItem, path, "<FOLDER>");
            QTreeWidgetItem* item = new QTreeWidgetItem(fold);
            item->setIcon(0, QPlasmaDocument::GetDocIcon(f));
            item->setText(0, f);
            item->setData(0, Qt::UserRole, root.absoluteFilePath(f));
        }
    }
}

void GameScanner::scan()
{
    QDir root(fAppDataDir);
    if (fGameType == GameInfo::kGameCrowthistle && root.cd("Crowthistle"))
        recursiveAppDataScan(QStringList(), root);
    else if (fGameType == GameInfo::kGameHexIsle && root.cd("HexIsle"))
        recursiveAppDataScan(QStringList(), root);
    else if (fGameType == GameInfo::kGameUruLive && root.cd("Uru Live"))
        recursiveAppDataScan(QStringList(), root);
    else if (fGameType == GameInfo::kGameMQO && root.cd("MagiQuest Online"))
        recursiveAppDataScan(QStringList(), root);
    else if (fGameType == GameInfo::kGameMyst5 && root.cd("Myst V End of Ages"))
        recursiveAppDataScan(QStringList(), root);
    else if (fGameType == GameInfo::kGameMyst5 && root.cd("Myst V Demo"))
        recursiveAppDataScan(QStringList(), root);

    root.setPath(fDocumentsDir);
    if (fGameType == GameInfo::kGameUruLive && root.cd("Uru Live"))
        recursiveDocumentScan(QStringList(), root);
    else if (fGameType == GameInfo::kGameMQO && root.cd("MagiQuest Online"))
        recursiveDocumentScan(QStringList(), root);

    root.setPath(fGameDir);
    root.setSorting(QDir::Name | QDir::IgnoreCase);
    recursiveScan(QStringList(), root);

    fAgesItem->setHidden(fAgesItem->childCount() == 0);
    fFontsItem->setHidden(fFontsItem->childCount() == 0);
    fImagesItem->setHidden(fImagesItem->childCount() == 0);
    fLevelsItem->setHidden(fLevelsItem->childCount() == 0);
    fLocalizationItem->setHidden(fLocalizationItem->childCount() == 0);
    fLogItem->setHidden(fLogItem->childCount() == 0);
    fPythonItem->setHidden(fPythonItem->childCount() == 0);
    fSDLItem->setHidden(fSDLItem->childCount() == 0);
    fSettingsItem->setHidden(fSettingsItem->childCount() == 0);
    fShadersItem->setHidden(fShadersItem->childCount() == 0);
    fAppDataItem->setHidden(fAppDataItem->childCount() == 0);
    fDocumentsItem->setHidden(fDocumentsItem->childCount() == 0);

    root.setPath(fGameDir);
    if (fGameType == GameInfo::kGameUru && root.exists("sav/vault.dat")) {
        QTreeWidgetItem* vault = new QTreeWidgetItem(fTree);
        vault->setIcon(0, QPlasmaDocument::GetDocIcon("<VAULT>"));
        vault->setText(0, tr("Single-Player Vault"));
        vault->setData(0, Qt::UserRole, QString("VAULT:%1").arg(root.absolutePath()));
    }
}
