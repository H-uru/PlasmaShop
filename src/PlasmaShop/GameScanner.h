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
