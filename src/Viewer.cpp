#include "Viewer.hpp"

#include <cmath>
#include <iostream>
#include <QGLShaderProgram>
#include <QtOpenGL>
#include <QtWidgets>

#include "AlgebraToQt.hpp"
#include "Material.hpp"
#include "Mesh.hpp"
#include "Primitive.hpp"
#include "Scene.hpp"

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

const SceneNode* Viewer::getScene() const {
    return scene;
}

void Viewer::setScene(SceneNode* scene) {
    this->scene = scene;
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

    shader.initialize("phong");
    interfaceShader.initialize("flat");

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

    interfaceShader.getProgram().bind();

    interfaceShader.getProgram().enableAttributeArray("vert");
    interfaceShader.getProgram().setAttributeBuffer("vert", GL_FLOAT, 0, 3);

    mMvpMatrixLocation = interfaceShader.getProgram().uniformLocation("modelViewProjection");
    mColorLocation = interfaceShader.getProgram().uniformLocation("colour");

    interfaceShader.getProgram().release();
    mCircleBufferObject.release();
    mVertexArrayObject.release();
}

void Viewer::paintGL() {
    // Clear framebuffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    static double angle = 0;
    angle += M_PI / 240;

    shader.setModelMatrix(Matrix4::makeTranslation(0, -0.5, 0) * Matrix4::makeRotation(0, angle, 0));
    shader.setViewMatrix(Matrix4::makeTranslation(0, 0, 2));

    shader.use();

    PhongMaterial material(Colour(0.5, 0.0, 1.0), Colour(0.8, 0.8, 0.8), 64);
    //FlatMaterial material(Colour(1.0, 0.0, 0.0));
    Sphere sphere;
    sphere.setMaterial(material);
    sphere.draw(shader);

    shader.setModelMatrix(Matrix4::makeTranslation(0, -1, 0) * Matrix4::makeRotation(0, angle, 0));
    shader.use();

    Cube cube;
    cube.setMaterial(material);
    cube.draw(shader);

    draw_trackball_circle();

    update();
}

void Viewer::resizeGL(int width, int height) {
    height = max(height, 1);

    // update perspective matrixes for both the scene and interface
    shader.setProjectionMatrix(Matrix4::makePerspective(30, (float) height / (float) width, 0.001, 1000));
    interfaceShader.setProjectionMatrix(Matrix4::makeOrtho(0, width, 0, height, -0.1, 0.1));

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

void Viewer::draw_trackball_circle() {
    glDisable(GL_DEPTH_TEST);

    mVertexArrayObject.bind();

    int current_width = width();
    int current_height = height();

    // Set up for orthogonal drawing to draw a circle on screen.
    // You'll want to make the rest of the function conditional on
    // whether or not we want to draw the circle this time around.

    FlatMaterial(Colour(0.0, 0.0, 0.0)).applyTo(interfaceShader);

    // Translate the view to the middle
    interfaceShader.setModelMatrix(Matrix4::makeTranslation(current_width / 2, current_height / 2, 0));

    interfaceShader.use();

    // Bind buffer object
    mCircleBufferObject.bind();

    // Draw buffer
    glDrawArrays(GL_LINE_LOOP, 0, 40);

    glEnable(GL_DEPTH_TEST);
}
