#include "QLayerMovie.h"

#include <QLabel>
#include <QGridLayout>
#include <ResManager/plFactory.h>

QLayerMovie::QLayerMovie(plCreatable* pCre, QWidget* parent)
           : QCreatable(pCre, pCre->ClassIndex(), parent)
{
    plLayerMovie* lay = (plLayerMovie*)fCreatable;

    fLayerAnimLink = new QCreatableLink(this, false);
    fLayerAnimLink->setText(tr("Layer Animation Properties"));
    fLayerAnimLink->setCreatable(lay);
    fLayerAnimLink->setForceType(kLayerAnimation);

    fMovieName = new QLineEdit(this);
    fMovieName->setText(~lay->getMovieName());

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(fLayerAnimLink, 0, 0, 1, 2);
    layout->addWidget(new QLabel(tr("Movie Name:"), this), 3, 0);
    layout->addWidget(fMovieName, 3, 1);
}

void QLayerMovie::saveDamage()
{
    plLayerMovie* lay = (plLayerMovie*)fCreatable;
    lay->setMovieName(~fMovieName->text());
}
