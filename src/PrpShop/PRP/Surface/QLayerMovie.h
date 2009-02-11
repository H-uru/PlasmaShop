#ifndef _QLAYERMOVIE_H
#define _QLAYERMOVIE_H

#include "../QCreatable.h"

#include <PRP/Surface/plLayerMovie.h>
#include <QLineEdit>
#include "../QObjLink.h"

class QLayerMovie : public QCreatable {
    Q_OBJECT

protected:
    QCreatableLink* fLayerAnimLink;
    QLineEdit* fMovieName;

public:
    QLayerMovie(plCreatable* pCre, QWidget* parent = NULL);
    virtual void saveDamage();
};

#endif
