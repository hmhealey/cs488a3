/*****************************************
 * Harrison Healey - hmhealey - 20376857 *
 *****************************************/

#ifndef CS488_VIEWER_HPP
#define CS488_VIEWER_HPP

#include <QGLFramebufferObject>
#include <QGLWidget>
#include <QMatrix4x4>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QtGlobal>

#include "Option.hpp"
#include "Shader.hpp"
#include "Trackball.hpp"
#include "UndoStack.hpp"

struct Matrix4;
class QGLShaderProgram;
class SceneNode;

class Viewer : public QGLWidget {
    Q_OBJECT

public:
    enum InputMode {
        Puppet,
        Joints
    };

private:
    Shader shader;
    Shader interfaceShader;

    SceneNode* scene = NULL;

    Trackball trackball;

    Matrix4 sceneTranslation;
    Matrix4 sceneRotation;

    QOpenGLBuffer mCircleBufferObject;
    QOpenGLVertexArrayObject mVertexArrayObject;

    InputMode mode = Viewer::Puppet;

    int lastMouseX = -1;
    int lastMouseY = -1;

    Shader pickingShader;
    QGLFramebufferObject* pickingBuffer = NULL;

    UndoStack undoStack;
    UndoStep pendingStep;

public:
    BooleanOption trackballVisible = false;
    BooleanOption depthBufferEnabled = false;
    BooleanOption backfaceCullingEnabled = false;
    BooleanOption frontfaceCullingEnabled = false;
    BooleanOption drawPickingBufferEnabled = false;

    Viewer(const QGLFormat& format, QWidget *parent = 0);
    virtual ~Viewer();
    
    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    const SceneNode* getScene() const;
    void setScene(SceneNode* scene);

    void resetPosition();
    void resetOrientation();
    void resetJoints();
    void resetAll();

    InputMode getInputMode() const;
    void setInputMode(InputMode mode);

    int getUndoStackSize() const;
    bool undo();

    int getRedoStackSize() const;
    bool redo();

    bool isTrackballVisible() const;
    void setTrackballVisible(bool trackballVisible);

    bool isDepthBufferEnabled() const;
    void setDepthBufferEnabled(bool depthBufferEnabled);

    bool isBackfaceCullingEnabled() const;
    void setBackfaceCullingEnabled(bool backfaceCullingEnabled);

    bool isFrontfaceCullingEnabled() const;
    void setFrontfaceCullingEnabled(bool frontfaceCullingEnabled);

    bool isDrawPickingBufferEnabled() const;
    void setDrawPickingBufferEnabled(bool drawPickingBufferEnabled);

signals:
    void undoStackUpdated();

protected:

    // Events we implement

    // Called when GL is first initialized
    virtual void initializeGL();
    // Called when our window needs to be redrawn
    virtual void paintGL();
    // Called when the window is resized (formerly on_configure_event)
    virtual void resizeGL(int width, int height);
    // Called when a mouse button is pressed
    virtual void mousePressEvent(QMouseEvent* event);
    // Called when a mouse button is released
    virtual void mouseReleaseEvent(QMouseEvent* event);
    // Called when the mouse moves
    virtual void mouseMoveEvent(QMouseEvent* event);
    
    // Draw a circle for the trackball, with OpenGL commands.
    // Assumes the context for the viewer is active.
    void draw_trackball_circle();
};

#endif
