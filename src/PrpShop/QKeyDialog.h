#ifndef _QKEYDIALOG_H
#define _QKEYDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QLineEdit>
#include <ResManager/plResManager.h>
#include <vector>

class QNewKeyDialog : public QDialog {
    Q_OBJECT

protected:
    QComboBox* fLocationBox;
    QComboBox* fTypeBox;
    QLineEdit* fName;
    std::vector<plLocation> fLocations;
    std::vector<short> fTypes;

public:
    QNewKeyDialog(QWidget* parent = NULL);
    void init(plResManager* mgr);

    plLocation location() const;
    short type() const;
    QString name() const;
};

class QFindKeyDialog : public QDialog {
    Q_OBJECT

protected:
    QComboBox* fLocationBox;
    QComboBox* fTypeBox;
    QComboBox* fKeyBox;
    plResManager* fResMgr;
    std::vector<plLocation> fLocations;
    std::vector<short> fTypes;
    std::vector<plKey> fKeys;

public:
    QFindKeyDialog(QWidget* parent = NULL);
    void init(plResManager* mgr, plLocation loc = plLocation(), short type = -1);
    void init(plResManager* mgr, plKey current);

    plKey selection() const;

protected slots:
    void locationSelected(int idx);
    void typeSelected(int idx);
};

#endif
