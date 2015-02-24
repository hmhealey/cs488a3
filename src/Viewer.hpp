#ifndef CS488_VIEWER_HPP
#define CS488_VIEWER_HPP

#include <QGLWidget>
#include <QMatrix4x4>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QtGlobal>

#include "Shader.hpp"

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

    QOpenGLBuffer mCircleBufferObject;
    QOpenGLVertexArrayObject mVertexArrayObject;

    bool trackballVisible = false;
    bool depthBufferEnabled = false;
    bool backfaceCullingEnabled = false;
    bool frontfaceCullingEnabled = false;

public:
    Viewer(const QGLFormat& format, QWidget *parent = 0);
    virtual ~Viewer();
    
    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    // TODO do we need to call this manually since the window is receiving these events
    // Called when a mouse button is released
    virtual void mouseReleaseEvent(QMouseEvent* event);

    const SceneNode* getScene() const;
    void setScene(SceneNode* scene);

    void resetPosition();
    void resetOrientation();
    void resetJoints();
    void resetAll();

    InputMode getInputMode() const;
    void setInputMode(InputMode mode);

    int getUndoStackSize() const;
    int undo();

    int getRedoStackSize() const;
    int redo();

    bool isTrackballVisible() const;
    void setTrackballVisible(bool trackballVisible);

    bool isDepthBufferEnabled() const;
    void setDepthBufferEnabled(bool depthBufferEnabled);

    bool isBackfaceCullingEnabled() const;
    void setBackfaceCullingEnabled(bool backfaceCullingEnabled);

    bool isFrontfaceCullingEnabled() const;
    void setFrontfaceCullingEnabled(bool frontfaceCullingEnabled);

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
    // Called when the mouse moves
    virtual void mouseMoveEvent(QMouseEvent* event);
    
    // Draw a circle for the trackball, with OpenGL commands.
    // Assumes the context for the viewer is active.
    void draw_trackball_circle();
};

#endif
