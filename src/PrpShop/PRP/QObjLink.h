#ifndef _QOBJLINK_H
#define _QOBJLINK_H

#include <PRP/KeyedObject/hsKeyedObject.h>
#include "../../QLinkLabel.h"

class QCreatableLink : public QWidget {
    Q_OBJECT

protected:
    plCreatable* fCreatable;
    QLinkLabel* fObjLabel;
    QLinkLabel* fEditLabel;
    short fForceType;

public:
    QCreatableLink(QWidget* parent = NULL, bool canEdit = true);
    void setText(const QString& str);
    void setCreatable(plCreatable* pCre);
    void setKey(plKey key);
    void setForceType(short forceType);

protected slots:
    void objectActivated();
    void menuRequested();

signals:
    void addObject();
    void delObject();
};

#endif
