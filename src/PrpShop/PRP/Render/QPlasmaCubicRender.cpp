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

#include "QPlasmaCubicRender.h"

#include <PRP/Surface/plMipmap.h>
#include <PRP/Surface/plCubicEnvironmap.h>

#include <QApplication>
#include <QtGui>
#include <QHBoxLayout>
#include <QPushButton>
#include <QScreen>


static const char* vertex_shader_source = R"(
uniform mat4 mvp_matrix;

attribute vec4 a_position;
attribute vec2 a_texcoord;

varying vec2 v_texcoord;

void main()
{
    // Calculate vertex position in screen space
    gl_Position = mvp_matrix * a_position;

    // Pass texture coordinate to fragment shader
    v_texcoord = a_texcoord;
}
)";

static const char* fragmemt_shader_source = R"(
uniform sampler2D envmap;
uniform sampler2D labels;

// Set this uniform to 0.0f to disable face labels, or 1.0f to enable display of labels
uniform float faceLabelBlendFactor;

varying vec2 v_texcoord;

void main()
{
    // Set fragment color from envmap, blended with face labels
    vec4 environmentColor = texture2D(envmap, v_texcoord);
    vec4 labelColor = texture2D(labels, v_texcoord);
    vec4 finalColor = mix(environmentColor, labelColor, labelColor.a * faceLabelBlendFactor);

    gl_FragColor = finalColor;
}
)";

struct VertexData {
    QVector3D position;
    QVector2D texCoord;
};

// Cube vertices and texture coordinates adapted
// from Qt example code.
constexpr VertexData vertices[] = {
    // Vertex data for face 0
    {{-1.0f, -1.0f,  1.0f}, {0.0f,      0.0f}}, //  v0
    {{ 1.0f, -1.0f,  1.0f}, {1.f / 3.f, 0.0f}}, //  v1
    {{-1.0f,  1.0f,  1.0f}, {0.0f,      0.5f}}, //  v2
    {{ 1.0f,  1.0f,  1.0f}, {1.f / 3.f, 0.5f}}, //  v3

    // Vertex data for face 1
    {{ 1.0f, -1.0f,  1.0f}, {0.0f,      0.5f}}, //  v4
    {{ 1.0f, -1.0f, -1.0f}, {1.f / 3.f, 0.5f}}, //  v5
    {{ 1.0f,  1.0f,  1.0f}, {0.0f,      1.0f}}, //  v6
    {{ 1.0f,  1.0f, -1.0f}, {1.f / 3.f, 1.0f}}, //  v7

    // Vertex data for face 2
    {{ 1.0f, -1.0f, -1.0f}, {2.f / 3.f, 0.5f}}, //  v8
    {{-1.0f, -1.0f, -1.0f}, {1.0f,      0.5f}}, //  v9
    {{ 1.0f,  1.0f, -1.0f}, {2.f / 3.f, 1.0f}}, // v10
    {{-1.0f,  1.0f, -1.0f}, {1.0f,      1.0f}}, // v11

    // Vertex data for face 3
    {{-1.0f, -1.0f, -1.0f}, {2.f / 3.f, 0.0f}}, // v12
    {{-1.0f, -1.0f,  1.0f}, {1.0f,      0.0f}}, // v13
    {{-1.0f,  1.0f, -1.0f}, {2.f / 3.f, 0.5f}}, // v14
    {{-1.0f,  1.0f,  1.0f}, {1.0f,      0.5f}}, // v15

    // Vertex data for face 4
    {{-1.0f, -1.0f, -1.0f}, {1.f / 3.f, 0.0f}}, // v16
    {{ 1.0f, -1.0f, -1.0f}, {2.f / 3.f, 0.0f}}, // v17
    {{-1.0f, -1.0f,  1.0f}, {1.f / 3.f, 0.5f}}, // v18
    {{ 1.0f, -1.0f,  1.0f}, {2.f / 3.f, 0.5f}}, // v19

    // Vertex data for face 5
    {{-1.0f,  1.0f,  1.0f}, {1.f / 3.f, 0.5f}}, // v20
    {{ 1.0f,  1.0f,  1.0f}, {2.f / 3.f, 0.5f}}, // v21
    {{-1.0f,  1.0f, -1.0f}, {1.f / 3.f, 1.0f}}, // v22
    {{ 1.0f,  1.0f, -1.0f}, {2.f / 3.f, 1.0f}}  // v23
};

constexpr GLushort indices[] = {
     0,  1,  2,  3,  3,     // Face 0 - triangle strip ( v0,  v1,  v2,  v3)
     4,  4,  5,  6,  7,  7, // Face 1 - triangle strip ( v4,  v5,  v6,  v7)
     8,  8,  9, 10, 11, 11, // Face 2 - triangle strip ( v8,  v9, v10, v11)
    12, 12, 13, 14, 15, 15, // Face 3 - triangle strip (v12, v13, v14, v15)
    16, 16, 17, 18, 19, 19, // Face 4 - triangle strip (v16, v17, v18, v19)
    20, 20, 21, 22, 23      // Face 5 - triangle strip (v20, v21, v22, v23)
};

// Calculate actual size once
constexpr size_t vertexCount = std::size(vertices);
constexpr size_t indexCount = std::size(indices);


QPlasmaCubicRender::QPlasmaCubicRender(QWidget* parent, const QImage& cubemap)
    : QOpenGLWidget(parent), fGLProgram(), fGLArrayBuf(QOpenGLBuffer::VertexBuffer),
      fGLIndexBuf(QOpenGLBuffer::IndexBuffer), fGLTexture(), fGLLabelTexture(),
      fGLRotation(0.0f, 0.0f, 0.0f, 1.0f), fCubeMap(cubemap), fMouseFrom(0, 0),
      fShowFaceLabelsOption()
{
    buildFaceLabelTexture();
}

QPlasmaCubicRender::~QPlasmaCubicRender()
{
    cleanupGL();
}

void QPlasmaCubicRender::initCubeGeometry()
{
    // Transfer vertex data to VBO 0
    fGLArrayBuf.bind();
    fGLArrayBuf.allocate(vertices, vertexCount * sizeof(VertexData));
    fGLArrayBuf.setUsagePattern(QOpenGLBuffer::StaticDraw);

    // Transfer index data to VBO 1
    fGLIndexBuf.bind();
    fGLIndexBuf.allocate(indices, indexCount * sizeof(GLushort));
    fGLIndexBuf.setUsagePattern(QOpenGLBuffer::StaticDraw);
}

QWidget *QPlasmaCubicRender::createViewOptions()
{
    // Create controls to send back to the parent window that affect how we render
    auto optionsWidget = new QWidget(this);
    optionsWidget->setLayout(new QHBoxLayout(optionsWidget));

    // Create a button to toggle face label visibility
    fShowFaceLabelsOption = new QPushButton(tr(""), this);
    fShowFaceLabelsOption->setIcon(QIcon(":/img/view-facelabels.png"));
    fShowFaceLabelsOption->setToolTip(tr("Toggle visibility of face labels on the cube"));
    fShowFaceLabelsOption->setCheckable(true);
    connect(fShowFaceLabelsOption, &QPushButton::toggled, this, [this]() {
        // Request a redraw when the option is toggled
        this->update();
    });
    optionsWidget->layout()->addWidget(fShowFaceLabelsOption);

    return optionsWidget;
}

void QPlasmaCubicRender::initializeGL()
{
    initializeOpenGLFunctions();

    glShadeModel(GL_SMOOTH);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    // Compile vertex shader
    if (!fGLProgram.addShaderFromSourceCode(QOpenGLShader::Vertex, vertex_shader_source))
        close();

    // Compile fragment shader
    if (!fGLProgram.addShaderFromSourceCode(QOpenGLShader::Fragment, fragmemt_shader_source))
        close();
    
    // Link shader pipeline
    if (!fGLProgram.link())
        close();

    // Bind shader pipeline for use
    if (!fGLProgram.bind())
        close();

    // Create a texture from the cubemap image
    fGLTexture = new QOpenGLTexture(fCubeMap, QOpenGLTexture::MipMapGeneration::DontGenerateMipMaps);
    fGLTexture->setMinificationFilter(QOpenGLTexture::Nearest);
    fGLTexture->setMagnificationFilter(QOpenGLTexture::Linear);
    fGLTexture->setWrapMode(QOpenGLTexture::Repeat);

    // Create a texture from face labels image
    fGLLabelTexture = new QOpenGLTexture(fFaceLabelTexture, QOpenGLTexture::MipMapGeneration::DontGenerateMipMaps);
    fGLLabelTexture->setMinificationFilter(QOpenGLTexture::Nearest);
    fGLLabelTexture->setMagnificationFilter(QOpenGLTexture::Linear);
    fGLLabelTexture->setWrapMode(QOpenGLTexture::Repeat);

    // Generate 2 VBOs
    fGLArrayBuf.create();
    fGLIndexBuf.create();

    // Load cube geometry into VBOs
    initCubeGeometry();

    // Ensure we clean up OpenGL resources when the context is destroyed.
    // Qt documentation recommends this for safety regardless of the context lifetime. 
    connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &QPlasmaCubicRender::cleanupGL);
}

void QPlasmaCubicRender::resizeGL(int width, int height)
{
    const float aspect = float(width) / float(height ? height : 1);
    const float fov = 90.0f, zNear = 0.5f, zFar = 2.0f;

    // Reset fGLProjection
    fGLProjection.setToIdentity();

    // Set perspective projection
    fGLProjection.perspective(fov, aspect, zNear, zFar);
}

void QPlasmaCubicRender::paintGL()
{
    glMatrixMode(GL_MODELVIEW);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    fGLProgram.bind();

    // Calculate model view transformation
    QMatrix4x4 matrix;
    matrix.translate(0.0f, 0.0f, 0.0f);
    matrix.rotate(fGLRotation);

    // Set modelview-projection matrix
    fGLProgram.setUniformValue("mvp_matrix", fGLProjection * matrix);

    // Set texture unit 0 to contain envmap
    glActiveTexture(GL_TEXTURE0);
    fGLTexture->bind();
    fGLProgram.setUniformValue("envmap", 0);

    // Set texture unit 1 to contain face labels
    glActiveTexture(GL_TEXTURE1);
    fGLLabelTexture->bind();
    fGLProgram.setUniformValue("labels", 1);

    // Set face label blend factor based on the toggle-button state
    float faceLabelBlendFactor = fShowFaceLabelsOption->isChecked() ? 1.0f : 0.0f;
    fGLProgram.setUniformValue("faceLabelBlendFactor", faceLabelBlendFactor);

    // Tell OpenGL which VBOs to use
    fGLArrayBuf.bind();
    fGLIndexBuf.bind();

    // Offset for position
    int offset = 0;
    
    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = fGLProgram.attributeLocation("a_position");
    fGLProgram.enableAttributeArray(vertexLocation);
    fGLProgram.setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));
    
    // Offset for texture coordinate
    offset += sizeof(QVector3D);

    // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
    int texcoordLocation = fGLProgram.attributeLocation("a_texcoord");
    fGLProgram.enableAttributeArray(texcoordLocation);
    fGLProgram.setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(VertexData));
    
    // Draw cube geometry using indices from VBO 1
    glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_SHORT, nullptr);

    // Release the textures
    glActiveTexture(GL_TEXTURE1);
    fGLLabelTexture->release();
    glActiveTexture(GL_TEXTURE0);
    fGLTexture->release();
}

void QPlasmaCubicRender::cleanupGL() 
{
    makeCurrent();

    fGLArrayBuf.destroy();
    fGLIndexBuf.destroy();

    delete fGLTexture;
    delete fGLLabelTexture;

    doneCurrent();

    disconnect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &QPlasmaCubicRender::cleanupGL);
}

void QPlasmaCubicRender::mouseMoveEvent(QMouseEvent* evt)
{
    if (evt->buttons() & Qt::LeftButton) {
        QPoint currentPos = evt->pos();

        // Get screen geometry and current global cursor position
        QRect screenGeometry = QApplication::primaryScreen()->geometry();
        QPoint center = mapFromGlobal(screenGeometry.center());
        QPoint globalPos = mapToGlobal(currentPos);

        // Check if cursor hit screen bounds
        if (globalPos.x() <= screenGeometry.left() ||
            globalPos.x() >= screenGeometry.right() ||
            globalPos.y() <= screenGeometry.top() ||
            globalPos.y() >= screenGeometry.bottom()) {

            // Calculate the center point and warp cursor there
            QCursor::setPos(mapToGlobal(center));
            currentPos = center;

            // Since we warped, reset fMouseFrom as well or else we'll get a jump
            fMouseFrom = currentPos;
        }

        // Convert pixel movement to angles (and scale down the movement)
        QPoint diff = currentPos - fMouseFrom;
        float xAngle = diff.y() * -0.5f;  // Pitch
        float yAngle = diff.x() *  0.5f;  // Yaw

        // Create rotation quaternions for each axis
        QQuaternion xRotation = QQuaternion::fromAxisAndAngle(1.0f, 0.0f, 0.0f, xAngle);
        QQuaternion yRotation = QQuaternion::fromAxisAndAngle(0.0f, 1.0f, 0.0f, yAngle);
        
        // Apply the new rotation, mindful of ordering to prevent roll
        fGLRotation = xRotation * fGLRotation * yRotation;

        // Update the stored position
        fMouseFrom = currentPos;

        // Request a redraw
        update();
    }
}

void QPlasmaCubicRender::mousePressEvent(QMouseEvent* evt)
{
    if (evt->button() == Qt::LeftButton) {
        fMouseFrom = evt->pos();
        setCursor(Qt::BlankCursor);  // Hide the cursor when starting a drag
    }
}

void QPlasmaCubicRender::mouseReleaseEvent(QMouseEvent* evt)
{
    if (evt->button() == Qt::LeftButton) {
        setCursor(Qt::ArrowCursor);  // Restore the cursor when done dragging
    }
}

void QPlasmaCubicRender::buildFaceLabelTexture()
{
    // Create a texture to hold 3x2 cube faces
    int face_size = 512;
    fFaceLabelTexture = QImage(QSize(face_size * 3, face_size * 2), QImage::Format_ARGB32);
    fFaceLabelTexture.fill(Qt::transparent);

    QPainter painter(&fFaceLabelTexture);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(Qt::black));

    // Set font size to a reasonable percentage of the texture resolution
    QFont font = painter.font();
    font.setPixelSize(face_size / 8);
    painter.setFont(font);

    QFontMetricsF fm(font);
    float textHeight = fm.ascent() - fm.descent(); // Get actual text height

    for (int i = plCubicEnvironmap::kLeftFace; i < plCubicEnvironmap::kNumFaces; i++) {
        int x = (i % 3) * face_size;
        int y = (i / 3) * face_size;

        QRect textRect(x, y, face_size, face_size);
        auto face_num = QPlasmaCubicRender::FaceOrdering[i];

        // Calculate center point, accounting for actual text height
        QPointF centerPoint(
            textRect.center().x() - fm.horizontalAdvance(plCubicEnvironmap::kFaceNames[face_num]) / 2.0f,
            textRect.center().y() + textHeight / 4.0f  // Adjust vertical position for true center
        );
        QPainterPath textPath;
        textPath.addText(centerPoint, font, plCubicEnvironmap::kFaceNames[face_num]);

        // Draw lighter outline for visibility
        QPen outlinePen(Qt::gray);
        outlinePen.setWidth(1);
        painter.setPen(outlinePen);
        painter.setBrush(Qt::black);
        painter.drawPath(textPath);
    }
    painter.end();
}
