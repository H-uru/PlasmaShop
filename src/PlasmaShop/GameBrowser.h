#ifndef _GAME_BROWSER_H
#define _GAME_BROWSER_H

#include <QDialog>
#include <QTreeWidget>
#include <QLineEdit>
#include <QComboBox>

struct GameInfo {
    enum GameType {
        kGameNone, kGameUru, kGameUruLive, kGameMyst5, kGameCrowthistle,
        kGameHexIsle, kGameMQO, kNumGameTypes
    };

    int fGameType;
    QString fGameTitle, fGamePath;

    static QIcon GetGameIcon(int type);
    static QString GetGameTypeName(int type);
};

class GameListDialog : public QDialog {
    Q_OBJECT

public:
    GameListDialog(QWidget* parent);

private slots:
    void onSave();
    void onAddGame();
    void onDelGame();
    void onEditGame(QTreeWidgetItem* item, int);

private:
    QList<GameInfo> fGames;
    QTreeWidget* fGameTree;
};

class GameConfigDialog : public QDialog {
    Q_OBJECT

public:
    GameConfigDialog(const GameInfo& info, QWidget* parent);
    QString gameTitle() const;
    QString gameLocation() const;
    int gameType() const;

private slots:
    void onBrowseLocation();

private:
    QLineEdit* fGameTitle;
    QLineEdit* fGameLocation;
    QComboBox* fGameType;
};

#endif
