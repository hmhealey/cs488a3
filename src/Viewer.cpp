/*****************************************
 * Harrison Healey - hmhealey - 20376857 *
 *****************************************/

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

static const double PUPPET_TRANSLATION_X_FACTOR = -0.1;
static const double PUPPET_TRANSLATION_Y_FACTOR = 0.1;
static const double PUPPET_TRANSLATION_Z_FACTOR = 0.5;

static const double JOINT_ROTATION_X_FACTOR = -1;
static const double JOINT_ROTATION_Y_FACTOR = 1;

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
    scene->resetJoints();
    undoStack.clear();

    emit undoStackUpdated();
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
    return undoStack.getUndoStackSize();
}

bool Viewer::undo() {
    bool ret = undoStack.undo();

    emit undoStackUpdated();

    return ret;
}

int Viewer::getRedoStackSize() const {
    return undoStack.getRedoStackSize();
}

bool Viewer::redo() {
    bool ret = undoStack.redo();

    emit undoStackUpdated();

    return ret;
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
        cerr << "Viewer::initializeGL - Could not enable sample buffers." << endl;
        return;
    }

    glClearColor(0.4, 0.4, 0.4, 0.0);
    glShadeModel(GL_SMOOTH);

    // set up shaders
    shader.initialize("phong");
    interfaceShader.initialize("flat");
    pickingShader.initialize("flat");

    // sets the camera position
    shader.setViewMatrix(Matrix4::makeTranslation(0, 0, 40));
    pickingShader.setViewMatrix(Matrix4::makeTranslation(0, 0, 40));

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
        cerr << "Viewer::initializeGL - Could not bind vertex buffer to the context." << endl;
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
            if (!pickingBuffer->bind()) cerr << "Viewer::paintGL - Binding picking buffer failed" << endl;

            glEnable(GL_DEPTH_TEST);
            glCullFace(GL_BACK);
            glEnable(GL_CULL_FACE);

            // clear picking buffer
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            scene->walk_gl(pickingShader, sceneTranslation * sceneRotation, true);

            glDisable(GL_CULL_FACE);
            glDisable(GL_DEPTH_TEST);

            if (!pickingBuffer->release()) cerr << "Viewer::paintGL - Releasing picking buffer failed" << endl;

            if (drawPickingBufferEnabled) {
                if (QGLFramebufferObject::hasOpenGLFramebufferBlit()) {
                    // on Ubuntu we can blit using QT, but not with the built-in commands for some reason
                    /*glBindFramebuffer(GL_READ_FRAMEBUFFER, pickingBuffer->handle());
                    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

                    glBlitFramebuffer(0, 0, width(), height(), 0, 0, width(), height(), GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);

                    glBindFramebuffer(GL_FRAMEBUFFER, 0);*/

                    QRect rect(0, 0, width(), height());
                    QGLFramebufferObject::blitFramebuffer(0, rect, pickingBuffer, rect, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                } else {
                    cerr << "Viewer::paintGL - Unable to draw picking buffer because this platform doesn't support blitting buffers" << endl;
                }
            }
        }
    }

    int error = glGetError();
    if (error != GL_NO_ERROR) {
        cerr << "Viewer::paintGL - Error after drawing " << error << endl;
    }

    if (trackballVisible) {
        draw_trackball_circle();
    }

    update();
}

void Viewer::resizeGL(int width, int height) {
    height = max(height, 1);

    // update perspective matrixes for both the scene and interface
    shader.setProjectionMatrix(Matrix4::makePerspective(30, (double) height / (double) width, 0.01, 100));
    pickingShader.setProjectionMatrix(Matrix4::makePerspective(30, (double) height / (double) width, 0.01, 100));
    interfaceShader.setProjectionMatrix(Matrix4::makeOrtho(0, width, 0, height, -0.1, 0.1));

    // center interface in the window
    double radius = min(width / 4.0, height / 4.0);
    interfaceShader.setModelMatrix(Matrix4::makeTranslation((double) width / 2, (double) height / 2, 0) * Matrix4::makeScaling(radius * 2, radius * 2, 1));

    // resize the virtual trackball
    trackball.radius = radius;
    trackball.center = Point2D(width / 2.0, height / 2.0);

    // update the viewport in the main buffer
    glViewport(0, 0, width, height);

    // resize the picking buffer
    if (pickingBuffer == NULL || pickingBuffer->size().width() != width || pickingBuffer->size().height() != height) {
        if (pickingBuffer != NULL) {
            delete pickingBuffer;
        }

        pickingBuffer = new QGLFramebufferObject(width, height, QGLFramebufferObject::Depth);
    }
}

void Viewer::mousePressEvent(QMouseEvent* event) {
    lastMouseX = event->x();
    lastMouseY = event->y();

    if (mode == Viewer::Joints) {
        if (event->button() == Qt::LeftButton) {
            if (pickingBuffer != NULL) {
                // bind the picking buffer for reading
                pickingBuffer->bind();

                int x = event->x();
                int y = height() - event->y();

                float depth;
                glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);

                if (depth < 1) {
                    // our cursor is actually over something so now we can pull the id from the greyscale colour
                    // value stored in the picking buffer
                    char id;
                    glReadPixels(x, y, 1, 1, GL_RED, GL_UNSIGNED_BYTE, &id);

                    scene->pick(id);
                }

                pickingBuffer->release();
            } else {
                // this should never happen
                cerr << "Viewer::mousePressEvent - Picking buffer is null when going to do picking" << endl;
            }
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
            const list<JointNode*> joints = scene->getSelectedJoints();

            for (auto i = joints.cbegin(); i != joints.cend(); i++) {
                JointNode* joint = *i;

                // rotate joint around its y axis based on x movement while right clicking
                if ((event->buttons() & Qt::RightButton) != 0) {
                    double from = joint->getYRotation();
                    double to = joint->setYRotation(from + dx * JOINT_ROTATION_Y_FACTOR);

                    if (from != to) {
                        updateStepRotation(pendingStep, joint, 'y', from, to);
                    }
                }

                // rotate joint around its x axis based on y movement while middle clicking
                if ((event->buttons() & Qt::MiddleButton) != 0) {
                    double from = joint->getXRotation();
                    double to = joint->setXRotation(from + dy * JOINT_ROTATION_X_FACTOR);

                    if (from != to) {
                        updateStepRotation(pendingStep, joint, 'x', from, to);
                    }
                }
            }
        }
    }

    lastMouseX = event->x();
    lastMouseY = event->y();
}

void Viewer::mouseReleaseEvent(QMouseEvent* event) {
    (void) event;

    if (mode == Viewer::Joints) {
        // push the pending step onto the undo stack if any joints have actually changed
        if (!pendingStep.empty()) {
            undoStack.push(pendingStep);

            emit undoStackUpdated();

            // and clear the pending step so any further actions are separate from these ones
            pendingStep.clear();
        }
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
