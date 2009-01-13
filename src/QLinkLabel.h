#ifndef _QLINKLABEL_H
#define _QLINKLABEL_H

#include <QLabel>
#include <QMouseEvent>

class QLinkLabel : public QLabel {
protected:
    Q_OBJECT
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

#endif
