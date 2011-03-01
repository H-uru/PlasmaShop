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
    QString gameTitle() const { return fGameTitle->text(); }
    QString gameLocation() const { return fGameLocation->text(); }
    int gameType() const { return fGameType->currentIndex(); }

private slots:
    void onBrowseLocation();

private:
    QLineEdit* fGameTitle;
    QLineEdit* fGameLocation;
    QComboBox* fGameType;
};

#endif
