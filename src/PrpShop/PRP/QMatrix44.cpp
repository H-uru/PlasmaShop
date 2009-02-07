#include "QMatrix44.h"

#include <QLabel>
#include <QGridLayout>

QMatrix44::QMatrix44(QWidget* parent) : QWidget(parent)
{
    QGridLayout* layout = new QGridLayout(this);
    layout->addWidget(new QLabel(tr("cX"), this), 0, 1);
    layout->addWidget(new QLabel(tr("cY"), this), 0, 2);
    layout->addWidget(new QLabel(tr("cZ"), this), 0, 3);
    layout->addWidget(new QLabel(tr("Pos"), this), 0, 4);
    layout->addWidget(new QLabel(tr("X"), this), 1, 0);
    layout->addWidget(new QLabel(tr("Y"), this), 2, 0);
    layout->addWidget(new QLabel(tr("Z"), this), 3, 0);

    for (size_t i=0; i<4; i++) {
        for (size_t j=0; j<4; j++) {
            fMatEdit[i][j] = new QFloatEdit(this);
            fMatEdit[i][j]->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed));
            layout->addWidget(fMatEdit[i][j], i+1, j+1);
        }
    }
}

hsMatrix44 QMatrix44::value() const
{
    hsMatrix44 mat;
    for (size_t i=0; i<4; i++)
        for (size_t j=0; j<4; j++)
            mat(i, j) = fMatEdit[i][j]->value();
    return mat;
}

void QMatrix44::setValue(const hsMatrix44& val)
{
    for (size_t i=0; i<4; i++)
        for (size_t j=0; j<4; j++)
            fMatEdit[i][j]->setValue(val(i, j));
}
