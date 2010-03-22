#ifndef _QMIPMAP_H
#define _QMIPMAP_H

#include "../QCreatable.h"
#include "../QObjLink.h"
#include <QImage>
#include <QCheckBox>
#include <QSpinBox>
#include <PRP/Surface/plMipmap.h>

class QTextureBox : public QWidget {
    Q_OBJECT

protected:
    QImage* fImage;
    unsigned char* fImageData;

public:
    QTextureBox(QWidget* parent = NULL);
    virtual ~QTextureBox();
    void setTexture(plMipmap* tex, int level = 0);

protected:
    virtual void paintEvent(QPaintEvent*);
};

class QMipmap_Preview : public QCreatable {
    Q_OBJECT

protected:
    QTextureBox* fTexture;
    int fLevel;

public:
    QMipmap_Preview(plCreatable* pCre, QWidget* parent = NULL);
    virtual void saveDamage();

    int level() const;

public slots:
    void setLevel(int level);
};

class QMipmap : public QCreatable {
    Q_OBJECT

protected:
    enum {
        kAlphaChannelFlag, kAlphaBitFlag, kBumpEnvMap, kForce32Bit,
        kDontThrowAwayImage, kForceOneMipLevel, kNoMaxSize, kIntensityMap,
        kHalfSize, kUserOwnsBitmap, kForceRewrite, kForceNonCompressed,
        kIsTexture, kIsOffscreen, kIsProjected, kIsOrtho, kNumBitmapFlags
    };
    QCheckBox* fFlags[kNumBitmapFlags];
    QCreatableLink* fPreviewLink;

public:
    QMipmap(plCreatable* pCre, QWidget* parent = NULL);
    virtual void saveDamage();

private slots:
    void onExportDDS();
    void onExportJPEG();
    void onImportDDS();
    void onImportJPEG();
};

QString getCompressionText(plBitmap* tex);

#endif
