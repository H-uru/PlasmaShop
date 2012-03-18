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

#ifndef _GAMESCANNER_H
#define _GAMESCANNER_H

#include <QTreeWidget>
#include <QDir>

class GameScanner : public QObject {
    Q_OBJECT

public:
    GameScanner(QTreeWidget* tree);

    void scanGame(int gameType, QString gameDir, QString appDataDir, QString docsDir);
    void refresh();

protected:
    QTreeWidgetItem* ensurePath(QTreeWidgetItem* root, QStringList path,
                                QString folderType);
    void recursiveScan(QStringList path, QDir root);
    void recursiveAppDataScan(QStringList path, QDir root);
    void recursiveDocumentScan(QStringList path, QDir root);
    void scan();

private:
    QTreeWidget* fTree;
    QString fGameDir, fAppDataDir, fDocumentsDir;
    int fGameType;

    QTreeWidgetItem* fAgesItem;
    QTreeWidgetItem* fFontsItem;
    QTreeWidgetItem* fImagesItem;
    QTreeWidgetItem* fLevelsItem;
    QTreeWidgetItem* fLocalizationItem;
    QTreeWidgetItem* fLogItem;
    QTreeWidgetItem* fPythonItem;
    QTreeWidgetItem* fSDLItem;
    QTreeWidgetItem* fSettingsItem;
    QTreeWidgetItem* fShadersItem;
    QTreeWidgetItem* fRootItem;
    QTreeWidgetItem* fAppDataItem;
    QTreeWidgetItem* fDocumentsItem;
};

#endif
