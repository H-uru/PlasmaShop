#ifndef _QOBJLINK_H
#define _QOBJLINK_H

#include <QLabel>
#include <PRP/KeyedObject/hsKeyedObject.h>

class QLinkLabel : public QLabel {
    Q_OBJECT

protected:
    bool fInClick;

public:
    QLinkLabel(QWidget* parent = NULL);
    QLinkLabel(const QString& text, QWidget* parent = NULL);

protected:
    void init();
    virtual void mousePressEvent(QMouseEvent* evt);
    virtual void mouseReleaseEvent(QMouseEvent* evt);

signals:
    void activated();
};

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
