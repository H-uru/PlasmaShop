#include "GameBrowser.h"

#include <QApplication>
#include <QPushButton>
#include <QToolButton>
#include <QLabel>
#include <QGridLayout>
#include <QSettings>
#include <QMessageBox>
#include <QCompleter>
#include <QDirModel>
#include <QDialogButtonBox>
#include <QFileDialog>

/* GameInfo */
QIcon GameInfo::GetGameIcon(int type)
{
    switch (type) {
    case kGameUru:
    case kGameUruLive:
        return QIcon(":/img/gm_URU.png");
    case kGameMyst5:
        return QIcon(":/img/gm_EOA.png");
    case kGameCrowthistle:
        return QIcon(":/img/gm_CT.png");
    case kGameHexIsle:
        return QIcon(":/img/gm_HI.png");
    case kGameMQO:
        return QIcon(":/img/gm_MQO.png");
    default:
        return QIcon();
    }
}

QString GameInfo::GetGameTypeName(int type)
{
    switch (type) {
    case kGameUru:
        return qApp->tr("UU, Alcugs, Uru CC");
    case kGameUruLive:
        return qApp->tr("Uru Live");
    case kGameMyst5:
        return qApp->tr("Myst 5");
    case kGameCrowthistle:
        return qApp->tr("Crowthistle");
    case kGameHexIsle:
        return qApp->tr("Hex Isle");
    case kGameMQO:
        return qApp->tr("Magiquest Online");
    default:
        return qApp->tr("Unknown");
    }
}


/* GameListDialog */
GameListDialog::GameListDialog(QWidget* parent)
              : QDialog(parent)
{
    fGameTree = new QTreeWidget(this);
    fGameTree->setHeaderLabels(QStringList() << tr("Game Name") << tr("Game Type")
                                             << tr("Location"));
    fGameTree->setRootIsDecorated(false);

    QSettings settings("PlasmaShop", "PlasmaShop");
    settings.beginGroup("Games");
    QStringList gamesList = settings.childKeys();
    foreach (QString gm, gamesList) {
        QStringList gmParams = settings.value(gm).toStringList();
        GameInfo inf;
        inf.fGameTitle = gm;
        inf.fGamePath = gmParams[0];
        inf.fGameType = gmParams[1].toInt();
        fGames.append(inf);
        QTreeWidgetItem* item = new QTreeWidgetItem(fGameTree);
        item->setText(0, inf.fGameTitle);
        item->setText(1, GameInfo::GetGameTypeName(inf.fGameType));
        item->setText(2, inf.fGamePath);
        item->setIcon(1, GameInfo::GetGameIcon(inf.fGameType));
    }
    settings.endGroup();

    QPushButton* btnAdd = new QPushButton(tr("Add..."), this);
    QPushButton* btnDel = new QPushButton(tr("Remove"), this);
    QPushButton* btnClose = new QPushButton(tr("Close"), this);
    btnClose->setDefault(true);

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->setVerticalSpacing(8);
    layout->setHorizontalSpacing(4);
    layout->addWidget(fGameTree, 0, 0, 1, 3);
    layout->addWidget(btnAdd, 1, 0);
    layout->addWidget(btnDel, 1, 1);
    layout->addWidget(btnClose, 1, 2);

    connect(btnAdd, SIGNAL(clicked()), this, SLOT(onAddGame()));
    connect(btnDel, SIGNAL(clicked()), this, SLOT(onDelGame()));
    connect(btnClose, SIGNAL(clicked()), this, SLOT(onSave()));
    connect(fGameTree, SIGNAL(itemActivated(QTreeWidgetItem*, int)),
            this, SLOT(onEditGame(QTreeWidgetItem*, int)));

    // Adjust the list headers to fit the games list
    fGameTree->resizeColumnToContents(2);
    fGameTree->resizeColumnToContents(1);
    fGameTree->resizeColumnToContents(0);

    setWindowTitle(tr("Edit Games"));
}

void GameListDialog::onSave()
{
    QSettings settings("PlasmaShop", "PlasmaShop");
    settings.beginGroup("Games");

    // Remove existing games list
    QStringList gamesList = settings.childKeys();
    foreach (QString gm, gamesList)
        settings.remove(gm);

    foreach (GameInfo info, fGames) {
        QStringList gmParams;
        gmParams << info.fGamePath;
        gmParams << QString("%1").arg(info.fGameType);
        settings.setValue(info.fGameTitle, gmParams);
    }
    settings.endGroup();
    accept();
}

void GameListDialog::onAddGame()
{
    GameInfo info;
    info.fGameType = GameInfo::kGameNone;
    GameConfigDialog dlg(info, this);
    if (dlg.exec() == QDialog::Accepted) {
        info.fGameTitle = dlg.gameTitle();
        info.fGamePath = dlg.gameLocation();
        info.fGameType = dlg.gameType();
        fGames.append(info);
        QTreeWidgetItem* item = new QTreeWidgetItem(fGameTree);
        item->setText(0, info.fGameTitle);
        item->setText(1, GameInfo::GetGameTypeName(info.fGameType));
        item->setText(2, info.fGamePath);
        item->setIcon(1, GameInfo::GetGameIcon(info.fGameType));
    }
}

void GameListDialog::onDelGame()
{
    if (fGameTree->selectedItems().size() == 0)
        return;

    QTreeWidgetItem* item = fGameTree->selectedItems()[0];
    if (QMessageBox::question(this, tr("Remove Game?"),
                              tr("Are you sure you want to remove game %1 "
                                 "from the list?").arg(item->text(0)),
                              QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        fGames.removeAt(fGameTree->indexOfTopLevelItem(item));
        delete item;
    }
}

void GameListDialog::onEditGame(QTreeWidgetItem* item, int)
{
    if (fGameTree->selectedItems().size() == 0)
        return;

    int which = fGameTree->indexOfTopLevelItem(item);
    GameConfigDialog dlg(fGames[which], this);
    if (dlg.exec() == QDialog::Accepted) {
        fGames[which].fGameTitle = dlg.gameTitle();
        fGames[which].fGamePath = dlg.gameLocation();
        fGames[which].fGameType = dlg.gameType();
        item->setText(0, fGames[which].fGameTitle);
        item->setText(1, GameInfo::GetGameTypeName(fGames[which].fGameType));
        item->setText(2, fGames[which].fGamePath);
        item->setIcon(1, GameInfo::GetGameIcon(fGames[which].fGameType));
    }
}


/* GameConfigDialog */
GameConfigDialog::GameConfigDialog(const GameInfo& info, QWidget* parent)
                : QDialog(parent)
{
    QDirModel* dirModel = new QDirModel(QStringList("*"),
                                        QDir::Dirs | QDir::NoDotAndDotDot,
                                        QDir::Name | QDir::IgnoreCase,
                                        this);

    fGameTitle = new QLineEdit(info.fGameTitle, this);
    fGameType = new QComboBox(this);
    fGameLocation = new QLineEdit(info.fGamePath, this);

    for (int i=0; i<GameInfo::kNumGameTypes; i++)
        fGameType->addItem(GameInfo::GetGameIcon(i), GameInfo::GetGameTypeName(i));
    fGameType->setCurrentIndex(info.fGameType);

    QToolButton* btnBrowse = new QToolButton(this);
    btnBrowse->setText("...");
    fGameLocation->setCompleter(new QCompleter(dirModel, fGameLocation));

    QDialogButtonBox* buttons = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Cancel,
                                                     Qt::Horizontal, this);
    buttons->setCenterButtons(true);

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(new QLabel(tr("Game Title: "), this), 0, 0);
    layout->addWidget(fGameTitle, 0, 1, 1, 2);
    layout->addWidget(new QLabel(tr("Game Type: "), this), 1, 0);
    layout->addWidget(fGameType, 1, 1, 1, 2);
    layout->addWidget(new QLabel(tr("Location: "), this), 2, 0);
    layout->addWidget(fGameLocation, 2, 1);
    layout->addWidget(btnBrowse, 2, 2);
    layout->addWidget(buttons, 3, 0, 1, 3);

    connect(buttons, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttons, SIGNAL(rejected()), this, SLOT(reject()));
    connect(btnBrowse, SIGNAL(clicked()), this, SLOT(onBrowseLocation()));

    setWindowTitle(tr("Game Properties"));
}

QString GameConfigDialog::gameTitle() const
{ return fGameTitle->text(); }

QString GameConfigDialog::gameLocation() const
{ return fGameLocation->text(); }

int GameConfigDialog::gameType() const
{ return fGameType->currentIndex(); }

void GameConfigDialog::onBrowseLocation()
{
    QString path = QFileDialog::getExistingDirectory(this, tr("Find Game Path"),
                                                     fGameLocation->text());
    if (!path.isEmpty())
        fGameLocation->setText(path);
}
