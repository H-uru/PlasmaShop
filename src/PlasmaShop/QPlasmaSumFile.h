#ifndef _QPLASMASUMFILE_H
#define _QPLASMASUMFILE_H

#include "QPlasmaDocument.h"
#include <Stream/hsStream.h>
#include <Util/plMD5.h>
#include <QTreeWidget>
#include <QAction>
#include <vector>

struct SumData {
    struct Entry {
        plString fPath;
        plMD5Hash fHash;
        time_t fTimestamp;
        unsigned int fUnknown;

        Entry();
    };

    unsigned int fUnknown;
    std::vector<Entry> fEntries;

    SumData();
    void read(hsStream* S);
    void write(hsStream* S);
};

class QPlasmaSumFile : public QPlasmaDocument {
    Q_OBJECT

public:
    QPlasmaSumFile(QWidget* parent);

    virtual bool loadFile(QString filename);
    virtual bool saveTo(QString filename);

private:
    QTreeWidget* fFileList;
    SumData fSumData;

    enum {
        kAUpdate, kAAdd, kADel, kANumActions
    };
    QAction* fActions[kANumActions];

    bool loadSumData(hsStream* S);
    bool saveSumData(hsStream* S);

private slots:
    void onContextMenu(QPoint pos);
    void onItemChanged(QTreeWidgetItem* item, int column);
    void onUpdate();
    void onAdd();
    void onDel();
};

#endif
