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

    // set up shaders
    shader.initialize("phong");
    interfaceShader.initialize("flat");

    // sets the camera position
    shader.setViewMatrix(Matrix4::makeTranslation(0, 0, 20));

    // construct circle for trackball
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

    // set up trackball vertices to shader
    interfaceShader.getProgram().bind();

    interfaceShader.getProgram().enableAttributeArray("vert");
    interfaceShader.getProgram().setAttributeBuffer("vert", GL_FLOAT, 0, 3);

    // set interface colour
    Material(Colour(0.0, 0.0, 0.0)).applyTo(interfaceShader);

    // release interface stuff so that we can be sure we're not affecting it later when setting up other meshes
    interfaceShader.getProgram().release();
    mCircleBufferObject.release();
    mVertexArrayObject.release();
}

void Viewer::paintGL() {
    // Clear framebuffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // construct the shapes for drawing
    Material material(Colour(0.5, 0.0, 1.0), Colour(0.8, 0.8, 0.8), 64);

    Sphere sphere;
    sphere.setMaterial(material);

    Cube cube;
    cube.setMaterial(material);

    // hackish rotation so we can test stuff
    static double angle = 0;
    angle += M_PI / 240;

    // draw sphere (i should really make it so that you don't need to call use to update the shader's uniforms)
    shader.setModelMatrix(Matrix4::makeTranslation(0, 0, 0) * Matrix4::makeRotation(0, angle, 0) * Matrix4::makeScaling(1, 1, 2));
    shader.use();

    sphere.draw(shader);

    // draw cube
    shader.setModelMatrix(Matrix4::makeTranslation(0, -1, 0) * Matrix4::makeRotation(0, angle, 0) * Matrix4::makeScaling(2, 1, 1));
    shader.use();

    cube.draw(shader);

    /*if (scene != NULL) {
        scene->walk_gl(shader, Matrix4(), false);
    }*/

    draw_trackball_circle();

    update();
}

void Viewer::resizeGL(int width, int height) {
    height = max(height, 1);

    // update perspective matrixes for both the scene and interface
    shader.setProjectionMatrix(Matrix4::makePerspective(30, (float) height / (float) width, 0.001, 1000));
    interfaceShader.setProjectionMatrix(Matrix4::makeOrtho(0, width, 0, height, -0.1, 0.1));

    // center interface in the window
    interfaceShader.setModelMatrix(Matrix4::makeTranslation((float) width / 2, (float) height / 2, 0));

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

    interfaceShader.use();

    mVertexArrayObject.bind();
    mCircleBufferObject.bind();

    // actually draw trackball
    glDrawArrays(GL_LINE_LOOP, 0, 40);

    glEnable(GL_DEPTH_TEST);
}
