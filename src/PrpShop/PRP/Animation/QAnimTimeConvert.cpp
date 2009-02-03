#include "QAnimTimeConvert.h"

#include <QLabel>
#include <QDoubleValidator>
#include <QGroupBox>
#include <QTabWidget>
#include <QGridLayout>
#include <QMenu>
#include "../../Main.h"
#include <ResManager/plFactory.h>

/* QATCCurveLink */
QATCCurveLink::QATCCurveLink(const QString& text, QWidget* parent, bool canEdit)
             : QCreatableLink(parent, canEdit)
{
    setText(text);
}

void QATCCurveLink::menuRequested()
{
    QMenu menu(this);

    QMenu* m_createTypes = new QMenu(tr("Create..."), &menu);
    QAction* newATC = m_createTypes->addAction(tr("ATC Ease Curve"));
    QAction* newSpline = m_createTypes->addAction(tr("Spline Ease Curve"));
    QAction* newConst = m_createTypes->addAction(tr("Const Accel Ease Curve"));

    menu.addMenu(m_createTypes);
    QAction* delObjItem = menu.addAction(tr("Delete"));

    if (fCreatable == NULL)
        delObjItem->setEnabled(false);

    QPoint pos = fEditLabel->mapToGlobal(QPoint(0, fEditLabel->height()));
    QAction* sel = menu.exec(pos);
    if (sel == delObjItem)
        emit delObject();
    else if (sel == newATC)
        emit createObject(kATCEaseCurve);
    else if (sel == newSpline)
        emit createObject(kSplineEaseCurve);
    else if (sel == newConst)
        emit createObject(kConstAccelEaseCurve);
}


/* QATCEaseCurve */
QATCEaseCurve::QATCEaseCurve(plCreatable* pCre, QWidget* parent)
             : QCreatable(pCre, pCre->ClassIndex(), parent)
{
    plATCEaseCurve* obj = (plATCEaseCurve*)fCreatable;

    fStartSpeed = new QLineEdit(this);
    fSpeed = new QLineEdit(this);
    fMinLength = new QLineEdit(this);
    fMaxLength = new QLineEdit(this);
    fLength = new QLineEdit(this);
    fBeginWorldTime = new QLineEdit(this);
    fStartSpeed->setValidator(new QDoubleValidator(fStartSpeed));
    fSpeed->setValidator(new QDoubleValidator(fSpeed));
    fMinLength->setValidator(new QDoubleValidator(fMinLength));
    fMaxLength->setValidator(new QDoubleValidator(fMaxLength));
    fLength->setValidator(new QDoubleValidator(fLength));
    fBeginWorldTime->setValidator(new QDoubleValidator(fBeginWorldTime));
    fStartSpeed->setText(QString("%1").arg(obj->getStartSpeed()));
    fSpeed->setText(QString("%1").arg(obj->getSpeed()));
    fMinLength->setText(QString("%1").arg(obj->getMinLength()));
    fMaxLength->setText(QString("%1").arg(obj->getMaxLength()));
    fLength->setText(QString("%1").arg(obj->getLength()));
    fBeginWorldTime->setText(QString("%1").arg(obj->getBeginWorldTime()));

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(new QLabel(tr("Start Speed:"), this), 0, 0);
    layout->addWidget(fStartSpeed, 0, 1);
    layout->addItem(new QSpacerItem(16, 0), 0, 2);
    layout->addWidget(new QLabel(tr("Speed:"), this), 0, 3);
    layout->addWidget(fSpeed, 0, 4);
    layout->addWidget(new QLabel(tr("Min Length:"), this), 1, 0);
    layout->addWidget(fMinLength, 1, 1);
    layout->addWidget(new QLabel(tr("Max Length:"), this), 1, 3);
    layout->addWidget(fMaxLength, 1, 4);
    layout->addWidget(new QLabel(tr("Begin World Time:"), this), 2, 0);
    layout->addWidget(fBeginWorldTime, 2, 1);
    layout->addWidget(new QLabel(tr("Length:"), this), 2, 3);
    layout->addWidget(fLength, 2, 4);

    if (plSplineEaseCurve* spline = plSplineEaseCurve::Convert(fCreatable)) {
        QGroupBox* splineGroup = new QGroupBox(tr("Spline Coefficients"), this);
        for (size_t i=0; i<4; i++) {
            fSplineCoef[i] = new QLineEdit(splineGroup);
            fSplineCoef[i]->setValidator(new QDoubleValidator(fSplineCoef[i]));
            fSplineCoef[i]->setText(QString("%1").arg(spline->getSplineCoef(i)));
        }
        QGridLayout* splineLayout = new QGridLayout(splineGroup);
        splineLayout->addWidget(fSplineCoef[0], 0, 0);
        splineLayout->addWidget(fSplineCoef[1], 0, 1);
        splineLayout->addWidget(fSplineCoef[2], 1, 0);
        splineLayout->addWidget(fSplineCoef[3], 1, 1);
        layout->addWidget(splineGroup, 3, 0, 1, 5);
    }
}

void QATCEaseCurve::saveDamage()
{
    plATCEaseCurve* obj = (plATCEaseCurve*)fCreatable;

    obj->setStartSpeed(fStartSpeed->text().toDouble());
    obj->setSpeed(fSpeed->text().toDouble());
    obj->setLengthBounds(fMinLength->text().toDouble(), fMaxLength->text().toDouble());
    obj->setLength(fLength->text().toDouble());
    obj->setBeginWorldTime(fBeginWorldTime->text().toDouble());

    if (plSplineEaseCurve* spline = plSplineEaseCurve::Convert(fCreatable)) {
        for (size_t i=0; i<4; i++)
            spline->setSplineCoef(i, fSplineCoef[i]->text().toDouble());
    }
}


/* QAnimTimeConvert */
QAnimTimeConvert::QAnimTimeConvert(plCreatable* pCre, QWidget* parent)
                : QCreatable(pCre, kAnimTimeConvert, parent)
{
    plAnimTimeConvert* obj = (plAnimTimeConvert*)fCreatable;

    QGroupBox* flagGroup = new QGroupBox(tr("Flags"), this);
    QGridLayout* flagLayout = new QGridLayout(flagGroup);
    flagLayout->setVerticalSpacing(0);
    flagLayout->setHorizontalSpacing(8);
    fFlags[kStopped] = new QCheckBox(tr("Stopped"), flagGroup);
    fFlags[kLoop] = new QCheckBox(tr("Loop"), flagGroup);
    fFlags[kBackwards] = new QCheckBox(tr("Backwards"), flagGroup);
    fFlags[kWrap] = new QCheckBox(tr("Wrap"), flagGroup);
    fFlags[kNeedsReset] = new QCheckBox(tr("Needs Reset"), flagGroup);
    fFlags[kEasingIn] = new QCheckBox(tr("Easing In"), flagGroup);
    fFlags[kForcedMove] = new QCheckBox(tr("Forced Move"), flagGroup);
    fFlags[kNoCallbacks] = new QCheckBox(tr("No Callbacks"), flagGroup);
    fFlags[kStopped]->setChecked((obj->getFlags() & plAnimTimeConvert::kStopped) != 0);
    fFlags[kLoop]->setChecked((obj->getFlags() & plAnimTimeConvert::kLoop) != 0);
    fFlags[kBackwards]->setChecked((obj->getFlags() & plAnimTimeConvert::kBackwards) != 0);
    fFlags[kWrap]->setChecked((obj->getFlags() & plAnimTimeConvert::kWrap) != 0);
    fFlags[kNeedsReset]->setChecked((obj->getFlags() & plAnimTimeConvert::kNeedsReset) != 0);
    fFlags[kEasingIn]->setChecked((obj->getFlags() & plAnimTimeConvert::kEasingIn) != 0);
    fFlags[kForcedMove]->setChecked((obj->getFlags() & plAnimTimeConvert::kForcedMove) != 0);
    fFlags[kNoCallbacks]->setChecked((obj->getFlags() & plAnimTimeConvert::kNoCallbacks) != 0);
    flagLayout->addWidget(fFlags[kStopped], 0, 0);
    flagLayout->addWidget(fFlags[kLoop], 0, 1);
    flagLayout->addWidget(fFlags[kBackwards], 0, 2);
    flagLayout->addWidget(fFlags[kWrap], 0, 3);
    flagLayout->addWidget(fFlags[kNeedsReset], 1, 0);
    flagLayout->addWidget(fFlags[kEasingIn], 1, 1);
    flagLayout->addWidget(fFlags[kForcedMove], 1, 2);
    flagLayout->addWidget(fFlags[kNoCallbacks], 1, 3);

    QGroupBox* timeGroup = new QGroupBox(tr("Timing Parameters"), this);
    QGridLayout* timeLayout = new QGridLayout(timeGroup);
    fBegin = new QLineEdit(timeGroup);
    fEnd = new QLineEdit(timeGroup);
    fLoopBegin = new QLineEdit(timeGroup);
    fLoopEnd = new QLineEdit(timeGroup);
    fSpeed = new QLineEdit(timeGroup);
    fCurrentAnimTime = new QLineEdit(timeGroup);
    fLastEvalWorldTime = new QLineEdit(timeGroup);
    fBegin->setValidator(new QDoubleValidator(fBegin));
    fEnd->setValidator(new QDoubleValidator(fEnd));
    fLoopBegin->setValidator(new QDoubleValidator(fLoopBegin));
    fLoopEnd->setValidator(new QDoubleValidator(fLoopEnd));
    fSpeed->setValidator(new QDoubleValidator(fSpeed));
    fCurrentAnimTime->setValidator(new QDoubleValidator(fCurrentAnimTime));
    fLastEvalWorldTime->setValidator(new QDoubleValidator(fLastEvalWorldTime));
    fBegin->setText(QString("%1").arg(obj->getBegin()));
    fEnd->setText(QString("%1").arg(obj->getEnd()));
    fLoopBegin->setText(QString("%1").arg(obj->getLoopBegin()));
    fLoopEnd->setText(QString("%1").arg(obj->getLoopEnd()));
    fSpeed->setText(QString("%1").arg(obj->getSpeed()));
    fCurrentAnimTime->setText(QString("%1").arg(obj->getCurrentAnimTime()));
    fLastEvalWorldTime->setText(QString("%1").arg(obj->getLastEvalWorldTime()));
    timeLayout->addWidget(new QLabel(tr("Range:"), timeGroup), 0, 0);
    timeLayout->addWidget(fBegin, 0, 1);
    timeLayout->addWidget(fEnd, 0, 2);
    timeLayout->addWidget(new QLabel(tr("Loop Range:"), timeGroup), 1, 0);
    timeLayout->addWidget(fLoopBegin, 1, 1);
    timeLayout->addWidget(fLoopEnd, 1, 2);
    timeLayout->addWidget(new QLabel(tr("Speed:"), timeGroup), 2, 0);
    timeLayout->addWidget(fSpeed, 2, 1, 1, 2);
    timeLayout->addWidget(new QLabel(tr("Anim Time:"), timeGroup), 3, 0);
    timeLayout->addWidget(fCurrentAnimTime, 3, 1, 1, 2);
    timeLayout->addWidget(new QLabel(tr("World Time:"), timeGroup), 4, 0);
    timeLayout->addWidget(fLastEvalWorldTime, 4, 1, 1, 2);

    QTabWidget* listTabs = new QTabWidget(this);

    fCallbackList = new QTreeWidget(listTabs);
    fCallbackList->setColumnCount(2);
    fCallbackList->setUniformRowHeights(true);
    fCallbackList->setRootIsDecorated(false);
    fCallbackList->headerItem()->setText(0, tr("Idx"));
    fCallbackList->headerItem()->setText(1, tr("Type"));
    fCallbackList->setContextMenuPolicy(Qt::CustomContextMenu);
    for (size_t i=0; i<obj->getNumCallbacks(); i++) {
        plEventCallbackMsg* msg = obj->getCallback(i);
        QTreeWidgetItem* item = new QTreeWidgetItem(fCallbackList,
            QStringList() << QString("%1").arg(i) << pqGetFriendlyClassName(msg->ClassIndex()));
        item->setIcon(0, pqGetTypeIcon(msg->ClassIndex()));
    }

    fStopPoints = new QDoubleListWidget(listTabs);
    for (size_t i=0; i<obj->getStopPoints().getSize(); i++)
        fStopPoints->addValue(obj->getStopPoints()[i]);

    listTabs->addTab(fCallbackList, tr("Callbacks"));
    listTabs->addTab(fStopPoints, tr("Stop Points"));

    fEaseInCurve = new QATCCurveLink(tr("Ease In Curve"), this);
    fEaseInCurve->setCreatable(obj->getEaseInCurve());
    fEaseOutCurve = new QATCCurveLink(tr("Ease Out Curve"), this);
    fEaseOutCurve->setCreatable(obj->getEaseOutCurve());
    fSpeedEaseCurve = new QATCCurveLink(tr("Speed Ease Curve"), this);
    fSpeedEaseCurve->setCreatable(obj->getSpeedEaseCurve());

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(flagGroup, 0, 0);
    layout->addWidget(timeGroup, 1, 0);
    layout->addWidget(listTabs, 2, 0);
    layout->addWidget(fEaseInCurve, 3, 0);
    layout->addWidget(fEaseOutCurve, 4, 0);
    layout->addWidget(fSpeedEaseCurve, 5, 0);
    fCallbackList->resizeColumnToContents(1);
    fCallbackList->resizeColumnToContents(0);

    connect(fCallbackList, SIGNAL(customContextMenuRequested(const QPoint&)),
            this, SLOT(callbackContextMenu(const QPoint&)));
    connect(fCallbackList, SIGNAL(itemActivated(QTreeWidgetItem*, int)),
            this, SLOT(activateCallbackItem(QTreeWidgetItem*, int)));
    connect(fEaseInCurve, SIGNAL(createObject(short)), this, SLOT(newEaseIn(short)));
    connect(fEaseInCurve, SIGNAL(delObject()), this, SLOT(delEaseIn()));
    connect(fEaseOutCurve, SIGNAL(createObject(short)), this, SLOT(newEaseOut(short)));
    connect(fEaseOutCurve, SIGNAL(delObject()), this, SLOT(delEaseOut()));
    connect(fSpeedEaseCurve, SIGNAL(createObject(short)), this, SLOT(newSpeedEase(short)));
    connect(fSpeedEaseCurve, SIGNAL(delObject()), this, SLOT(delSpeedEase()));
}

void QAnimTimeConvert::saveDamage()
{
    plAnimTimeConvert* obj = (plAnimTimeConvert*)fCreatable;

    obj->setFlags((fFlags[kStopped]->isChecked() ? plAnimTimeConvert::kStopped : 0)
                | (fFlags[kLoop]->isChecked() ? plAnimTimeConvert::kLoop : 0)
                | (fFlags[kBackwards]->isChecked() ? plAnimTimeConvert::kBackwards : 0)
                | (fFlags[kWrap]->isChecked() ? plAnimTimeConvert::kWrap : 0)
                | (fFlags[kNeedsReset]->isChecked() ? plAnimTimeConvert::kNeedsReset : 0)
                | (fFlags[kEasingIn]->isChecked() ? plAnimTimeConvert::kEasingIn : 0)
                | (fFlags[kForcedMove]->isChecked() ? plAnimTimeConvert::kForcedMove : 0)
                | (fFlags[kNoCallbacks]->isChecked() ? plAnimTimeConvert::kNoCallbacks : 0));
    obj->setRange(fBegin->text().toDouble(), fEnd->text().toDouble());
    obj->setLoop(fLoopBegin->text().toDouble(), fLoopEnd->text().toDouble());
    obj->setSpeed(fSpeed->text().toDouble());
    obj->setCurrentAnimTime(fCurrentAnimTime->text().toDouble());
    obj->setLastEvalWorldTime(fLastEvalWorldTime->text().toDouble());
    QList<double> stops = fStopPoints->values();
    hsTArray<float> plstops;
    plstops.setSize(stops.size());
    for (size_t i=0; i<plstops.getSize(); i++)
        plstops[i] = stops[i];
    obj->setStopPoints(plstops);
}

void QAnimTimeConvert::callbackContextMenu(const QPoint& pos)
{
    plAnimTimeConvert* obj = (plAnimTimeConvert*)fCreatable;

    QMenu menu(this);
    QAction* addObjItem = menu.addAction(tr("Add Callback..."));
    QAction* delObjItem = menu.addAction(tr("Remove Callback"));

    if (fCallbackList->currentItem() == NULL)
        delObjItem->setEnabled(false);

    QAction* sel = menu.exec(fCallbackList->viewport()->mapToGlobal(pos));
    if (sel == addObjItem) {
        plEventCallbackMsg* msg = new plEventCallbackMsg();
        int id = obj->getNumCallbacks();
        QTreeWidgetItem* item = new QTreeWidgetItem(fCallbackList,
            QStringList() << QString("%1").arg(id) << pqGetFriendlyClassName(msg->ClassIndex()));
        item->setIcon(0, pqGetTypeIcon(msg->ClassIndex()));
        obj->addCallback(msg);
    } else if (sel == delObjItem) {
        int idx = fCallbackList->indexOfTopLevelItem(fCallbackList->currentItem());
        QTreeWidgetItem* item = fCallbackList->takeTopLevelItem(idx);
        delete item;
        obj->delCallback(idx);
    }
}

void QAnimTimeConvert::activateCallbackItem(QTreeWidgetItem* item, int)
{
    plAnimTimeConvert* obj = (plAnimTimeConvert*)fCreatable;
    PrpShopMain::Instance()->editCreatable(obj->getCallback(fCallbackList->indexOfTopLevelItem(item)));
}

void QAnimTimeConvert::newEaseIn(short type)
{
    plAnimTimeConvert* obj = (plAnimTimeConvert*)fCreatable;
    plATCEaseCurve* pCre = plATCEaseCurve::Convert(plFactory::Create(type));
    obj->setEaseInCurve(pCre);
    fEaseInCurve->setCreatable(pCre);
}

void QAnimTimeConvert::newEaseOut(short type)
{
    plAnimTimeConvert* obj = (plAnimTimeConvert*)fCreatable;
    plATCEaseCurve* pCre = plATCEaseCurve::Convert(plFactory::Create(type));
    obj->setEaseOutCurve(pCre);
    fEaseOutCurve->setCreatable(pCre);
}

void QAnimTimeConvert::newSpeedEase(short type)
{
    plAnimTimeConvert* obj = (plAnimTimeConvert*)fCreatable;
    plATCEaseCurve* pCre = plATCEaseCurve::Convert(plFactory::Create(type));
    obj->setSpeedEaseCurve(pCre);
    fSpeedEaseCurve->setCreatable(pCre);
}

void QAnimTimeConvert::delEaseIn()
{
    plAnimTimeConvert* obj = (plAnimTimeConvert*)fCreatable;
    obj->setEaseInCurve(NULL);
    fEaseInCurve->setCreatable(NULL);
}

void QAnimTimeConvert::delEaseOut()
{
    plAnimTimeConvert* obj = (plAnimTimeConvert*)fCreatable;
    obj->setEaseOutCurve(NULL);
    fEaseOutCurve->setCreatable(NULL);
}

void QAnimTimeConvert::delSpeedEase()
{
    plAnimTimeConvert* obj = (plAnimTimeConvert*)fCreatable;
    obj->setSpeedEaseCurve(NULL);
    fSpeedEaseCurve->setCreatable(NULL);
}
