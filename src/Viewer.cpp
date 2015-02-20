#include "Viewer.hpp"

#include <cmath>
#include <iostream>
#include <QtOpenGL>
#include <QtWidgets>

#include "Mesh.hpp"
#include "AlgebraToQt.hpp"
#include "Primitive.hpp"

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE 0x809D
#endif

using namespace std;

Viewer::Viewer(const QGLFormat& format, QWidget *parent) : QGLWidget(format, parent), mCircleBufferObject(QOpenGLBuffer::VertexBuffer), mVertexArrayObject(this) { }

Viewer::~Viewer() { }

QSize Viewer::minimumSizeHint() const {
    return QSize(50, 50);
}

QSize Viewer::sizeHint() const {
    return QSize(300, 300);
}

void Viewer::initializeGL() {
    QGLFormat glFormat = QGLWidget::format();
    if (!glFormat.sampleBuffers()) {
        cerr << "Could not enable sample buffers." << endl;
        return;
    }

    glShadeModel(GL_SMOOTH);
    glClearColor(0.4, 0.4, 0.4, 0.0);
    glEnable(GL_DEPTH_TEST);
    
    if (!program.addShaderFromSourceFile(QGLShader::Vertex, "shader.vert")) {
        cerr << "Cannot load vertex shader." << endl;
        return;
    }

    if (!program.addShaderFromSourceFile(QGLShader::Fragment, "shader.frag")) {
        cerr << "Cannot load fragment shader." << endl;
        return;
    }

    if ( !program.link() ) {
        cerr << "Cannot link shaders." << endl;
        return;
    }

    float circleData[120];

    double radius = width() < height() ? (float)width() * 0.25 : (float)height() * 0.25;
        
    for(size_t i = 0; i < 40; ++i) {
        circleData[i * 3] = radius * cos(i * 2 * M_PI / 40);
        circleData[i * 3 + 1] = radius * sin(i * 2 * M_PI / 40);
        circleData[i * 3 + 2] = 0.0;
    }

    mVertexArrayObject.create();
    mVertexArrayObject.bind();

    mCircleBufferObject.create();
    mCircleBufferObject.setUsagePattern(QOpenGLBuffer::StaticDraw);

    if (!mCircleBufferObject.bind()) {
        cerr << "could not bind vertex buffer to the context." << endl;
        return;
    }

    mCircleBufferObject.allocate(circleData, 40 * 3 * sizeof(float));

    program.bind();

    program.enableAttributeArray("vert");
    program.setAttributeBuffer("vert", GL_FLOAT, 0, 3);

    mMvpMatrixLocation = program.uniformLocation("modelViewProjection");
    mColorLocation = program.uniformLocation("frag_color");
}

void Viewer::paintGL() {
    int error = glGetError();
    if (error != GL_NO_ERROR) {
        cerr << "Viewer::paintGL - error before painting " << error << endl;
    }
    // Clear framebuffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set up lighting

    // Draw stuff

    error = glGetError();
    if (error != GL_NO_ERROR) {
        cerr << "Viewer::paintGL - error after drawing trackball " << error << endl;
    }

    //draw_trackball_circle();

    error = glGetError();
    if (error != GL_NO_ERROR) {
        cerr << "Viewer::paintGL - error after drawing trackball " << error << endl;
    }
    cerr << "aaa" << endl;

    static double angle = 0;
    angle += M_PI / 240;

    program.setUniformValue("modelView", toQt(Matrix4::makeTranslation(0, 0, -2) * Matrix4::makeRotation(0, angle, 0)));
    program.setUniformValue("modelViewProjection", toQt(Matrix4::makePerspective(30, 1, 0.1, 10) * Matrix4::makeTranslation(0, 0, -2) * Matrix4::makeRotation(0, angle, 0)));

    //Mesh* mesh = Mesh::makeBox(1, 1, 1);
    Mesh* mesh = Mesh::makeIcosphere(0.5f, 1);
    mesh->draw(program);
    delete mesh;

    //Sphere sphere;

    error = glGetError();
    if (error != GL_NO_ERROR) {
        cerr << "Viewer::paintGL - error after painting " << error << endl;
    }

    update();
}

void Viewer::resizeGL(int width, int height) {
    if (height == 0) {
        height = 1;
    }

    mPerspMatrix.setToIdentity();
    mPerspMatrix.perspective(60.0, (float) width / (float) height, 0.001, 1000);

    glViewport(0, 0, width, height);
}

void Viewer::mousePressEvent(QMouseEvent* event) {
    cerr << "Stub: button " << event->button() << " pressed" << endl;
}

void Viewer::mouseReleaseEvent(QMouseEvent* event) {
    cerr << "Stub: button " << event->button() << " released" << endl;
}

void Viewer::mouseMoveEvent(QMouseEvent* event) {
    cerr << "Stub: Motion at " << event->x() << ", " << event->y() << endl;
}

QMatrix4x4 Viewer::getCameraMatrix() {
    // Todo: Ask if we want to keep this.
    QMatrix4x4 vMatrix;

    QMatrix4x4 cameraTransformation;
    QVector3D cameraPosition = cameraTransformation * QVector3D(0, 0, 20.0);
    QVector3D cameraUpDirection = cameraTransformation * QVector3D(0, 1, 0);

    vMatrix.lookAt(cameraPosition, QVector3D(0, 0, 0), cameraUpDirection);

    return mPerspMatrix * vMatrix * mTransformMatrix;
}

void Viewer::translateWorld(float x, float y, float z) {
    // Todo: Ask if we want to keep this.
    mTransformMatrix.translate(x, y, z);
}

void Viewer::rotateWorld(float x, float y, float z) {
    // Todo: Ask if we want to keep this.
    mTransformMatrix.rotate(x, y, z);
}

void Viewer::scaleWorld(float x, float y, float z) {
    // Todo: Ask if we want to keep this.
    mTransformMatrix.scale(x, y, z);
}

void Viewer::set_colour(const QColor& col) {
    program.setUniformValue(mColorLocation, col.red(), col.green(), col.blue());
}

void Viewer::draw_trackball_circle() {
    mVertexArrayObject.bind();

    int current_width = width();
    int current_height = height();

    // Set up for orthogonal drawing to draw a circle on screen.
    // You'll want to make the rest of the function conditional on
    // whether or not we want to draw the circle this time around.

    set_colour(QColor(0.0, 0.0, 0.0));

    // Set orthographic Matrix
    Matrix4 orthoMatrix = Matrix4::makeOrtho(0, current_width, 0, current_height, -0.1, 0.1);

    // Translate the view to the middle
    Matrix4 transformMatrix = Matrix4::makeTranslation(current_width / 2, current_height / 2, 0);

    // Bind buffer object
    mCircleBufferObject.bind();
    program.setUniformValue(mMvpMatrixLocation, toQt(orthoMatrix * transformMatrix));

    int error = glGetError();
    if (error != GL_NO_ERROR) {
        cerr << "Viewer::draw_trackball_circle - error before drawing trackball " << error << endl;
    }

    // Draw buffer
    glDrawArrays(GL_LINE_LOOP, 0, 40);

    error = glGetError();
    if (error != GL_NO_ERROR) {
        cerr << "Viewer::draw_trackball_circle - error after drawing trackball " << error << endl;
    }
}
