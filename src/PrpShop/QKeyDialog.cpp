#include "QKeyDialog.h"

#include <QLabel>
#include <QDialogButtonBox>
#include <QGridLayout>
#include "QPlasmaUtils.h"

/* QNewKeyDialog */
QNewKeyDialog::QNewKeyDialog(QWidget* parent)
             : QDialog(parent)
{
    setWindowTitle(tr("New Object..."));

    fLocationBox = new QComboBox(this);
    fTypeBox = new QComboBox(this);
    fName = new QLineEdit(this);
    QDialogButtonBox* buttonBox = new QDialogButtonBox(Qt::Horizontal, this);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel |
                                  QDialogButtonBox::Ok);

    QGridLayout* layout = new QGridLayout(this);
    layout->setHorizontalSpacing(8);
    layout->setVerticalSpacing(8);
    layout->addWidget(new QLabel(tr("Location:"), this), 0, 0);
    layout->addWidget(fLocationBox, 0, 1);
    layout->addWidget(new QLabel(tr("Type:"), this), 1, 0);
    layout->addWidget(fTypeBox, 1, 1);
    layout->addWidget(new QLabel(tr("Name:"), this), 2, 0);
    layout->addWidget(fName, 2, 1);
    layout->addWidget(buttonBox, 3, 0, 1, 2);

    QObject::connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

void QNewKeyDialog::init(plResManager* mgr)
{
    fLocationBox->clear();
    fTypeBox->clear();
    fName->clear();

    fLocations = mgr->getLocations();
    for (size_t i=0; i<fLocations.size(); i++) {
        plPageInfo* page = mgr->FindPage(fLocations[i]);
        fLocationBox->addItem(QIcon(":/img/page.png"),
            QString("[%1] %2").arg(page->getAge().cstr())
                              .arg(page->getPage().cstr()));
    }

    fTypes = pqGetValidKOTypes();
    for (size_t i=0; i<fTypes.size(); i++)
        fTypeBox->addItem(pqGetTypeIcon(fTypes[i]), pqGetFriendlyClassName(fTypes[i]));
}

plLocation QNewKeyDialog::location() const
{ return fLocations[fLocationBox->currentIndex()]; }

short QNewKeyDialog::type() const
{ return fTypes[fTypeBox->currentIndex()]; }

QString QNewKeyDialog::name() const
{ return fName->text(); }


/* QFindKeyDialog */
QFindKeyDialog::QFindKeyDialog(QWidget* parent)
              : QDialog(parent)
{
    setWindowTitle(tr("Find Object..."));

    fLocationBox = new QComboBox(this);
    fTypeBox = new QComboBox(this);
    fKeyBox = new QComboBox(this);
    QDialogButtonBox* buttonBox = new QDialogButtonBox(Qt::Horizontal, this);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel |
                                  QDialogButtonBox::Ok);

    QGridLayout* layout = new QGridLayout(this);
    layout->setHorizontalSpacing(8);
    layout->setVerticalSpacing(8);
    layout->addWidget(new QLabel(tr("Location:"), this), 0, 0);
    layout->addWidget(fLocationBox, 0, 1);
    layout->addWidget(new QLabel(tr("Type:"), this), 1, 0);
    layout->addWidget(fTypeBox, 1, 1);
    layout->addWidget(new QLabel(tr("Object:"), this), 2, 0);
    layout->addWidget(fKeyBox, 2, 1);
    layout->addWidget(buttonBox, 3, 0, 1, 2);

    QObject::connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    QObject::connect(fLocationBox, SIGNAL(currentIndexChanged(int)),
                     this, SLOT(locationSelected(int)));
    QObject::connect(fTypeBox, SIGNAL(currentIndexChanged(int)),
                     this, SLOT(typeSelected(int)));
}

void QFindKeyDialog::init(plResManager* mgr, plLocation loc, short type)
{
    fResMgr = mgr;
    fLocationBox->clear();

    fLocations = fResMgr->getLocations();
    for (size_t i=0; i<fLocations.size(); i++) {
        plPageInfo* page = fResMgr->FindPage(fLocations[i]);
        fLocationBox->addItem(QIcon(":/img/page.png"),
            QString("[%1] %2").arg(page->getAge().cstr())
                              .arg(page->getPage().cstr()));
    }

    if (loc.isValid()) {
        for (size_t i=0; i<fLocations.size(); i++) {
            if (loc == fLocations[i])
                fLocationBox->setCurrentIndex(i);
        }
    }
    if (type >= 0) {
        for (size_t i=0; i<fTypes.size(); i++) {
            if (type == fTypes[i])
                fTypeBox->setCurrentIndex(i);
        }
    }
}

void QFindKeyDialog::init(plResManager* mgr, plKey current)
{
    fResMgr = mgr;
    fLocationBox->clear();

    fLocations = fResMgr->getLocations();
    for (size_t i=0; i<fLocations.size(); i++) {
        plPageInfo* page = fResMgr->FindPage(fLocations[i]);
        fLocationBox->addItem(QIcon(":/img/page.png"),
            QString("[%1] %2").arg(page->getAge().cstr())
                              .arg(page->getPage().cstr()));
    }

    if (current.Exists()) {
        for (size_t i=0; i<fLocations.size(); i++) {
            if (fLocations[i] == current->getLocation())
                fLocationBox->setCurrentIndex(i);
        }
        for (size_t i=0; i<fTypes.size(); i++) {
            if (fTypes[i] == current->getType())
                fTypeBox->setCurrentIndex(i);
        }
        for (size_t i=0; i<fKeys.size(); i++) {
            if (fKeys[i] == current)
                fKeyBox->setCurrentIndex(i);
        }
    }
}

plKey QFindKeyDialog::selection() const
{
    if (fKeyBox->currentIndex() < 0) return plKey();
    return fKeys[fKeyBox->currentIndex()];
}

void QFindKeyDialog::locationSelected(int idx)
{
    fTypeBox->clear();
    if (idx < 0) return;

    fTypes = fResMgr->getTypes(fLocations[idx]);
    for (size_t i=0; i<fTypes.size(); i++)
        fTypeBox->addItem(pqGetTypeIcon(fTypes[i]), pqGetFriendlyClassName(fTypes[i]));
}

void QFindKeyDialog::typeSelected(int idx)
{
    fKeyBox->clear();
    if (idx < 0) return;

    fKeys = fResMgr->getKeys(fLocations[fLocationBox->currentIndex()], fTypes[idx]);
    for (size_t i=0; i<fKeys.size(); i++)
        fKeyBox->addItem(fKeys[i]->getName().cstr());
}
