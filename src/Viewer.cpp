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

Viewer::~Viewer() {
    if (pickingBuffer != NULL) {
        delete pickingBuffer;
    }
}

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
    sceneRotation = Matrix4();
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

bool Viewer::isDrawPickingBufferEnabled() const {
    return drawPickingBufferEnabled;
}

void Viewer::setDrawPickingBufferEnabled(bool drawPickingBufferEnabled) {
    this->drawPickingBufferEnabled = drawPickingBufferEnabled;
}

void Viewer::initializeGL() {
    QGLFormat glFormat = QGLWidget::format();
    if (!glFormat.sampleBuffers()) {
        cerr << "Could not enable sample buffers." << endl;
        return;
    }

    glClearColor(0.4, 0.4, 0.4, 0.0);
    glShadeModel(GL_SMOOTH);

    // set up shaders
    shader.initialize("phong");
    interfaceShader.initialize("flat");
    pickingShader.initialize("flat");

    // sets the camera position
    shader.setViewMatrix(Matrix4::makeTranslation(0, 0, 20));
    pickingShader.setViewMatrix(Matrix4::makeTranslation(0, 0, 20));

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

    // set the pixel alignment to 1 so that we can read from any size of frame buffer without worrying about data being offset
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
}

void Viewer::paintGL() {
    if (scene != NULL) {
        // clear main framebuffer
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

        // actually draw scene
        scene->walk_gl(shader, sceneTranslation * sceneRotation, false);

        // disable options after drawing
        glDisable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);

        // draw to picking buffer so that picking works
        if (pickingBuffer != NULL) {
            if (!pickingBuffer->bind()) cerr << "binding picking buffer failed" << endl;

            glEnable(GL_DEPTH_TEST);
            glCullFace(GL_BACK);
            glEnable(GL_CULL_FACE);

            // clear picking buffer
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            scene->walk_gl(pickingShader, sceneTranslation * sceneRotation, true);

            glDisable(GL_CULL_FACE);
            glDisable(GL_DEPTH_TEST);

            if (!pickingBuffer->release()) cerr << "releasing picking buffer failed" << endl;

            if (drawPickingBufferEnabled) {
                glBindFramebuffer(GL_READ_FRAMEBUFFER, pickingBuffer->handle());
                glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

                glBlitFramebuffer(0, 0, width(), height(), 0, 0, width(), height(), GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);

                glBindFramebuffer(GL_FRAMEBUFFER, 0);
            }
        }
    }

    int error = glGetError();
    if (error != GL_NO_ERROR) {
        cerr << "error after drawing " << error << endl;
    }

    if (trackballVisible) {
        draw_trackball_circle();
    }

    update();
}

void Viewer::resizeGL(int width, int height) {
    height = max(height, 1);

    // update perspective matrixes for both the scene and interface
    shader.setProjectionMatrix(Matrix4::makePerspective(30, (double) height / (double) width, 0.001, 1000));
    pickingShader.setProjectionMatrix(Matrix4::makePerspective(30, (double) height / (double) width, 0.001, 1000));
    interfaceShader.setProjectionMatrix(Matrix4::makeOrtho(0, width, 0, height, -0.1, 0.1));

    // center interface in the window
    double radius = min(width / 4.0, height / 4.0);
    interfaceShader.setModelMatrix(Matrix4::makeTranslation((double) width / 2, (double) height / 2, 0) * Matrix4::makeScaling(radius * 2, radius * 2, 1));

    // resize the virtual trackball
    trackball.radius = radius;
    trackball.center = Point2D(width / 2.0, height / 2.0);

    // update the viewport in the main buffer
    glViewport(0, 0, width, height);

    // resize the picking buffer to the next largest power of two so we aren't constantly resizing it
    int size = pow(2, ceil(log2(max(width, height))));
    if (pickingBuffer == NULL || pickingBuffer->size().width() != size) {
        if (pickingBuffer != NULL) {
            delete pickingBuffer;
        }

        pickingBuffer = new QGLFramebufferObject(size, size, QGLFramebufferObject::Depth);
    }
}

void Viewer::mousePressEvent(QMouseEvent* event) {
    lastMouseX = event->x();
    lastMouseY = event->y();

    if (mode == Viewer::Joints) {
        if (event->button() == Qt::LeftButton) {
            if (pickingBuffer != NULL) {
                // bind the picking buffer for reading
                glBindFramebuffer(GL_READ_FRAMEBUFFER, pickingBuffer->handle());

                int x = event->x();
                int y = height() - event->y();

                float depth;
                glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);

                char colour[4];
                glReadPixels(x, y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, colour);

                cerr << "point: " << x << ", " << y << endl;
                cerr << "colour: " << (int) colour[0] << ", " << (int) colour[1] << ", " << (int) colour[2] << ", " << (int) colour[3] << endl;
                cerr << "depth: " << depth << endl;

                glBindFramebuffer(GL_READ_FRAMEBUFFER, 0;
            } else {
                // this should never happen
                cerr << "picking buffer is null when going to do picking" << endl;
            }
        } else {
            // TODO joint manipulation
        }
    }
}

void Viewer::mouseMoveEvent(QMouseEvent* event) {
    int dx = lastMouseX - event->x();
    int dy = lastMouseY - event->y();
    
    if (dx != 0 || dy != 0) {
        if (mode == Viewer::Puppet) {
            // don't allow multiple operations to work at once since that really doesn't make sense here
            if ((event->buttons() & Qt::LeftButton) != 0) { 
                sceneTranslation = sceneTranslation * Matrix4::makeTranslation(dx * PUPPET_TRANSLATION_X_FACTOR, dy * PUPPET_TRANSLATION_Y_FACTOR, 0);
            } else if ((event->buttons() & Qt::RightButton) != 0) {
                // find where the mouse was/is in the trackball's coordinate system
                Vector3 current = trackball.windowToTrackball(Point2D(event->x(), event->y()));
                Vector3 old = trackball.windowToTrackball(Point2D(lastMouseX, lastMouseY));

                // flip y from window coordinates to OpenGL ones
                current.y() = -current.y();
                old.y() = -old.y();

                // despite the mouse having actually moved, these rarely end up being equal which results in their cross product being
                // the zero vector which causes everything to break
                if (current != old) {
                    // we can cross those vectors to get a vector representing the axis of rotation which has length matching the angle to rotate
                    Vector3 vec = old.cross(current);

                    // fVec.length() is in radians
                    double angle = vec.length() * 180 / M_PI;
                    Vector3 axis = (sceneRotation.inverse() * vec).normalized();

                    sceneRotation = sceneRotation * Matrix4::makeRotation(angle, axis);
                }
            } else if ((event->buttons() & Qt::MiddleButton) != 0) {
                sceneTranslation = sceneTranslation * Matrix4::makeTranslation(0, 0, dy * PUPPET_TRANSLATION_Z_FACTOR);
            }
        } else if (mode == Viewer::Joints) {
            // but allow multiple operations at once since it actually works here
            // TODO joint manipulation
        }
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

    Material material(Colour(0.0, 0.0, 0.0));
    material.applyTo(interfaceShader);

    mVertexArrayObject.bind();
    mCircleBufferObject.bind();

    // actually draw trackball
    glDrawArrays(GL_LINE_LOOP, 0, 40);

    mCircleBufferObject.release();
    mVertexArrayObject.release();
}
