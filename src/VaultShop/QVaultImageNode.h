#ifndef _QVAULTIMAGENODE_H
#define _QVAULTIMAGENODE_H

#include <QComboBox>
#include <QLineEdit>
#include <QPixmap>
#include "QVaultNodeEdit.h"

class QStaticImage : public QWidget {
    Q_OBJECT

public:
    QStaticImage(QWidget* parent);
    virtual ~QStaticImage();

    void set(QPixmap* img);
    QPixmap* get() const;
    void refresh();

protected:
    virtual void paintEvent(QPaintEvent*);
    virtual void resizeEvent(QResizeEvent*);

private:
    QPixmap* fImg;
    QSize fScale;
};


class QVaultImageNode : public QVaultNodeEdit {
    Q_OBJECT

protected:
    QLineEdit* fImgTitle;
    QComboBox* fImgType;
    QStaticImage* fImage; 

public:
    QVaultImageNode(QWidget* parent);
    virtual QString getEditorTitle() const;
    virtual plVaultNode saveNode();

protected slots:
    void IImportImage();
    void IExportImage();

protected:
    virtual void IRefreshNode();
};

#endif
