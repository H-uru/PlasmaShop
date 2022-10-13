/* This file is part of PlasmaShop.
 *
 * PlasmaShop is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PlasmaShop is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PlasmaShop.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "QCEMap_Preview.h"

#include <PRP/Object/plSceneObject.h>
#include <PRP/Surface/plCubicEnvironmap.h>

#include <QPainter>
#include <QToolBar>
#include <QGridLayout>


QCEMap_Preview::QCEMap_Preview(plCreatable* pCre, QWidget* parent)
                 : QCreatable(pCre, kPreviewCubicEnvironmap, parent)
{
    plCubicEnvironmap* obj = plCubicEnvironmap::Convert(fCreatable);

    // Render the faces onto a single cubemap atlas texture
    auto cubemap = buildCubeMapTexture(obj);
    
    fRender = new QPlasmaCubicRender(this, cubemap);
    fRender->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    fRender->setMinimumSize(512, 512);

    QToolBar* viewToolbar = new QToolBar(tr("View"), this);
    viewToolbar->setFloatable(false);
    auto viewOptions = fRender->createViewOptions();
    viewToolbar->addWidget(viewOptions);

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setVerticalSpacing(0);
    layout->addWidget(viewToolbar, 0, 0);
    layout->addWidget(fRender, 1, 0);
}

const QImage QCEMap_Preview::buildCubeMapTexture(plCubicEnvironmap* obj) const
{
    plMipmap* frontFace = obj->getFace(plCubicEnvironmap::kFrontFace);
    QImage cubemap(frontFace->getWidth() * 3, frontFace->getHeight() * 2, QImage::Format_ARGB32);

    QPainter painter(&cubemap);
    for (int i = plCubicEnvironmap::kLeftFace; i < plCubicEnvironmap::kNumFaces; i++) {
        auto face_num = QPlasmaCubicRender::FaceOrdering[i];
        plMipmap* face = obj->getFace(face_num);
        if (face) {
            // Decompress the face image and convert it to QImage
            auto buffer_size = face->GetUncompressedSize(0);
            auto buffer = std::make_unique<unsigned char[]>(buffer_size);
            face->DecompressImage(0, buffer.get(), buffer_size);
            auto face_image = QImage(buffer.get(), face->getLevelWidth(0), face->getLevelHeight(0), QImage::Format_ARGB32).rgbSwapped();

            // Rotate top and bottom faces
            float face_rotation = 0.0f;
            if (face_num == plCubicEnvironmap::kTopFace) {
                face_rotation = 90.0f;
            } else if (face_num == plCubicEnvironmap::kBottomFace) {
                face_rotation = -90.0f;
            }
            face_image = face_image.transformed(QTransform().rotate(face_rotation));

            // Copy the face image into the correct position in the cubemap
            int x = (i % 3) * face->getHeight();
            int y = (i / 3) * face->getWidth();
            painter.drawImage(QRect(x, y, face_image.width(), face_image.height()), face_image, QRect(0, 0, face_image.width(), face_image.height()));
        }
    }
    painter.end();

    return cubemap;
}
