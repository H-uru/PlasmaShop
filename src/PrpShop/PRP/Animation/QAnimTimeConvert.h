#ifndef _QANIMTIMECONVERT_H
#define _QANIMTIMECONVERT_H

#include "../QCreatable.h"

#include <PRP/Animation/plAnimTimeConvert.h>
#include <QLineEdit>
#include <QCheckBox>
#include "../QObjLink.h"
#include "../QKeyList.h"

class QATCCurveLink : public QCreatableLink {
    Q_OBJECT

public:
    QATCCurveLink(const QString& text, QWidget* parent = NULL,
                  bool canEdit = true);

protected slots:
    virtual void menuRequested();
};

class QATCEaseCurve : public QCreatable {
    Q_OBJECT

protected:
    QLineEdit* fStartSpeed;
    QLineEdit* fSpeed;
    QLineEdit* fMinLength;
    QLineEdit* fMaxLength;
    QLineEdit* fLength;
    QLineEdit* fBeginWorldTime;
    QLineEdit* fSplineCoef[4];

public:
    QATCEaseCurve(plCreatable* pCre, QWidget* parent = NULL);
    virtual void saveDamage();
};

class QAnimTimeConvert : public QCreatable {
    Q_OBJECT

protected:
    enum {
        kStopped, kLoop, kBackwards, kWrap, kNeedsReset, kEasingIn, kForcedMove,
        kNoCallbacks, kNumATCFlags
    };
    QCheckBox* fFlags[kNumATCFlags];

    QLineEdit* fBegin;
    QLineEdit* fEnd;
    QLineEdit* fLoopBegin;
    QLineEdit* fLoopEnd;
    QLineEdit* fSpeed;
    QLineEdit* fCurrentAnimTime;
    QLineEdit* fLastEvalWorldTime;
    QTreeWidget* fCallbackList;
    QDoubleListWidget* fStopPoints;
    QATCCurveLink* fEaseInCurve;
    QATCCurveLink* fEaseOutCurve;
    QATCCurveLink* fSpeedEaseCurve;

public:
    QAnimTimeConvert(plCreatable* pCre, QWidget* parent = NULL);
    virtual void saveDamage();

protected slots:
    void callbackContextMenu(const QPoint& pos);
    void activateCallbackItem(QTreeWidgetItem* item, int column);

    void newEaseIn(short type);
    void newEaseOut(short type);
    void newSpeedEase(short type);
    void delEaseIn();
    void delEaseOut();
    void delSpeedEase();
};

#endif
