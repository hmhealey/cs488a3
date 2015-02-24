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

static const double PUPPET_TRANSLATION_X_FACTOR = -0.02;
static const double PUPPET_TRANSLATION_Y_FACTOR = 0.02;
static const double PUPPET_TRANSLATION_Z_FACTOR = 0.05;

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
    sceneTranslation = Matrix4();
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

    // just construct the circle with a radius of 1 for now and transform it later since we don't know 
    // the final window size yet
    for(size_t i = 0; i < 40; ++i) {
        circleData[i * 3] = 0.5 * cos(i * 2 * M_PI / 40);
        circleData[i * 3 + 1] = 0.5 * sin(i * 2 * M_PI / 40);
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
        scene->walk_gl(shader, sceneTranslation, false);
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
    shader.setProjectionMatrix(Matrix4::makePerspective(30, (double) height / (double) width, 0.001, 1000));
    interfaceShader.setProjectionMatrix(Matrix4::makeOrtho(0, width, 0, height, -0.1, 0.1));

    // center interface in the window
    double radius = min(width / 2.0, height / 2.0);
    interfaceShader.setModelMatrix(Matrix4::makeTranslation((double) width / 2, (double) height / 2, 0) * Matrix4::makeScaling(radius, radius, 1));

    glViewport(0, 0, width, height);
}

void Viewer::mousePressEvent(QMouseEvent* event) {
    lastMouseX = event->x();
    lastMouseY = event->y();

    if (mode == Viewer::Joints) {
        // TODO joint manipulation
    }
}

void Viewer::mouseMoveEvent(QMouseEvent* event) {
    int dx = lastMouseX - event->x();
    int dy = lastMouseY - event->y();
        
    if (mode == Viewer::Puppet) {
        // don't allow multiple operations to work at once since that really doesn't make sense here
        if ((event->buttons() & Qt::LeftButton) != 0) { 
            sceneTranslation = sceneTranslation * Matrix4::makeTranslation(dx * PUPPET_TRANSLATION_X_FACTOR, dy * PUPPET_TRANSLATION_Y_FACTOR, 0);
        } else if ((event->buttons() & Qt::RightButton) != 0) {
            sceneTranslation = sceneTranslation * Matrix4::makeTranslation(0, 0, dy * PUPPET_TRANSLATION_Z_FACTOR);
        } else if ((event->buttons() & Qt::MiddleButton) != 0) {
            // TODO allow puppet rotation with the virtual trackball
        }
    } else if (mode == Viewer::Joints) {
        // but allow multiple operations at once since it actually works here
        // TODO joint manipulation
    }

    lastMouseX = event->x();
    lastMouseY = event->y();
}

void Viewer::mouseReleaseEvent(QMouseEvent* event) {
    if (mode == Viewer::Joints) {
        // TODO joint manipulation
    }
}

void Viewer::draw_trackball_circle() {
    interfaceShader.use();

    mVertexArrayObject.bind();
    mCircleBufferObject.bind();

    // actually draw trackball
    glDrawArrays(GL_LINE_LOOP, 0, 40);
}
