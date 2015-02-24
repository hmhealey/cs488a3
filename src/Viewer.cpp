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

void Viewer::resetPosition() {
    // TODO reset position of the puppet
    cerr << "Viewer::resetPosition - Not yet implemented" << endl;
}

void Viewer::resetOrientation() {
    // TODO reset orientation of the puppet
    cerr << "Viewer::resetOrientation - Not yet implemented" << endl;
}

void Viewer::resetJoints() {
    // TODO reset puppet joint positions
    // TODO clear undo/redo stack
    cerr << "Viewer::resetJoints - Not yet implemented" << endl;
}

void Viewer::resetAll() {
    resetPosition();
    resetOrientation();
    resetJoints();
}

Viewer::InputMode Viewer::getInputMode() const {
    return mode;
}

void Viewer::setInputMode(InputMode mode) {
    this->mode = mode;
}

int Viewer::getUndoStackSize() const {
    // TODO get undo stack size
    cerr << "Viewer::getUndoStackSize - Not yet implemented" << endl;
    return 0;
}

int Viewer::undo() {
    // TODO undo the last joint manipulation
    cerr << "Viewer::undo - Not yet implemented" << endl;
    return getUndoStackSize();
}

int Viewer::getRedoStackSize() const {
    // TODO get redo stack size
    cerr << "Viewer::getRedoStackSize - Not yet implemented" << endl;
    return 0;
}

int Viewer::redo() {
    // TODO redo the last undid reverted joint manipulation
    cerr << "Viewer::redo - Not yet implemented" << endl;
    return getRedoStackSize();
}

bool Viewer::isTrackballVisible() const {
    return trackballVisible;
}

void Viewer::setTrackballVisible(bool trackballVisible) {
    this->trackballVisible = trackballVisible;
}

bool Viewer::isDepthBufferEnabled() const {
    return depthBufferEnabled;
}

void Viewer::setDepthBufferEnabled(bool depthBufferEnabled) {
    this->depthBufferEnabled = depthBufferEnabled;
}

bool Viewer::isBackfaceCullingEnabled() const {
    return backfaceCullingEnabled;
}

void Viewer::setBackfaceCullingEnabled(bool backfaceCullingEnabled) {
    this->backfaceCullingEnabled = backfaceCullingEnabled;
}

bool Viewer::isFrontfaceCullingEnabled() const {
    return frontfaceCullingEnabled;
}

void Viewer::setFrontfaceCullingEnabled(bool frontfaceCullingEnabled) {
    this->frontfaceCullingEnabled = frontfaceCullingEnabled;
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
    shader.setViewMatrix(Matrix4::makeTranslation(0, 0, 2));

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

    // set up OpenGL state based on options
    if (depthBufferEnabled) {
        glEnable(GL_DEPTH_TEST);
    }
    if (backfaceCullingEnabled || frontfaceCullingEnabled) {
        if (backfaceCullingEnabled && frontfaceCullingEnabled) {
            glCullFace(GL_FRONT_AND_BACK);
        } else if (backfaceCullingEnabled) {
            glCullFace(GL_BACK);
        } else /*if (frontfaceCullingEnabled)*/ {
            glCullFace(GL_FRONT);
        }
        glEnable(GL_CULL_FACE);
    }

    // construct the shapes for drawing
    /*Material material(Colour(0.5, 0.0, 1.0), Colour(0.8, 0.8, 0.8), 64);

    Sphere sphere;
    sphere.setMaterial(material);

    Cube cube;
    cube.setMaterial(material);

    // hackish rotation so we can test stuff
    static double angle = 0;
    angle += 1;

    // draw sphere (i should really make it so that you don't need to call use to update the shader's uniforms)
    shader.setModelMatrix(Matrix4::makeTranslation(0, 0, 0) * Matrix4::makeRotation(0, angle, 0) * Matrix4::makeScaling(1, 1, 2));
    shader.use();

    sphere.draw(shader);

    // draw cube
    shader.setModelMatrix(Matrix4::makeTranslation(0, -1, 0) * Matrix4::makeRotation(0, angle, 0) * Matrix4::makeScaling(2, 1, 1));
    shader.use();

    cube.draw(shader);*/

    if (scene != NULL) {
        scene->walk_gl(shader, Matrix4(), false);
    }

    // disable options after drawing
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

    if (trackballVisible) {
        draw_trackball_circle();
    }

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
    interfaceShader.use();

    mVertexArrayObject.bind();
    mCircleBufferObject.bind();

    // actually draw trackball
    glDrawArrays(GL_LINE_LOOP, 0, 40);
}
