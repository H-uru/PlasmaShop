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

#ifndef _QPLASMACUBICRENDER_H
#define _QPLASMACUBICRENDER_H

#include <QOpenGLBuffer>
#include <QOpenGLWidget>
#include <QOpenGLTexture>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QQuaternion>

#include <PRP/Surface/plCubicEnvironmap.h>

class plMipmap;
class QPushButton;
class QPlasmaCubicRender : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

private:
    QOpenGLShaderProgram fGLProgram;        // Shader program for rendering the cube.
    QOpenGLBuffer        fGLArrayBuf;       // Buffer for the cube vertex positions and texture coordinates.
    QOpenGLBuffer        fGLIndexBuf;       // Buffer for the cube geometry.
    QOpenGLTexture*      fGLTexture;        // GL texture of fCubeMap.
    QOpenGLTexture*      fGLLabelTexture;   // GL texture of fFaceLabelTexture.
    QQuaternion          fGLRotation;       // Rotation of the cube/viewport in OpenGL space.
    QMatrix4x4           fGLProjection;     // Projection matrix for the OpenGL viewport.

    QImage        fCubeMap;              // A texture atlas containing the six faces of the environment map.
    QImage        fFaceLabelTexture;     // A texture atlas containing the face labels to be used as an overlay.
    QPoint        fMouseFrom;            // Mouse position when the button was pressed.
    QPushButton*  fShowFaceLabelsOption; // Button to toggle the visibility of face labels.    

public:
     QPlasmaCubicRender(QWidget* parent, const QImage& cubemap);
    ~QPlasmaCubicRender();

    // Creates toolbar options for the render widget.
    QWidget* createViewOptions();

    // Defines a common layout for a packed texture atlas containing all faces of the Cubic Environmap. 
    static constexpr plCubicEnvironmap::Faces FaceOrdering[] = {
        plCubicEnvironmap::kRightFace, plCubicEnvironmap::kTopFace, plCubicEnvironmap::kFrontFace,
        plCubicEnvironmap::kBackFace, plCubicEnvironmap::kBottomFace, plCubicEnvironmap::kLeftFace
    };

protected:
    void initCubeGeometry();
    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void paintGL() override;
    void cleanupGL();

    void mouseMoveEvent(QMouseEvent *evt) override;
    void mousePressEvent(QMouseEvent *evt) override;
    void mouseReleaseEvent(QMouseEvent *evt) override;

    // Pre-renders the face label texture.
    void buildFaceLabelTexture();
};


#endif
