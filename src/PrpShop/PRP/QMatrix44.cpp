#include "QMatrix44.h"

#include <QLabel>
#include <QGridLayout>
#include <QDoubleValidator>

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
            fMatEdit[i][j] = new QLineEdit(this);
            fMatEdit[i][j]->setValidator(new QDoubleValidator(fMatEdit[i][j]));
            layout->addWidget(fMatEdit[i][j], i+1, j+1);
        }
    }
}

QSize QMatrix44::sizeHint() const
{
    QSize recSize = QWidget::sizeHint();
    return QSize((recSize.width() * 2) / 3, recSize.height());
}

hsMatrix44 QMatrix44::value() const
{
    hsMatrix44 mat;
    for (size_t i=0; i<4; i++)
        for (size_t j=0; j<4; j++)
            mat(i, j) = fMatEdit[i][j]->text().toFloat();
    return mat;
}

void QMatrix44::setValue(const hsMatrix44& val)
{
    for (size_t i=0; i<4; i++)
        for (size_t j=0; j<4; j++)
            fMatEdit[i][j]->setText(QString("%1").arg(val(i, j)));
}
