#ifndef CS488_VIEWER_HPP
#define CS488_VIEWER_HPP

#include <QGLShaderProgram>
#include <QGLWidget>
#include <QMatrix4x4>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QtGlobal>

class Viewer : public QGLWidget {
    Q_OBJECT

public:
    Viewer(const QGLFormat& format, QWidget *parent = 0);
    virtual ~Viewer();
    
    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    // If you want to render a new frame, call do not call paintGL(),
    // instead, call update() to ensure that the view gets a paint 
    // event.
  
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

private:

    QMatrix4x4 getCameraMatrix();
    void translateWorld(float x, float y, float z);
    void rotateWorld(float x, float y, float z);
    void scaleWorld(float x, float y, float z);
    void set_colour(const QColor& col);

    QOpenGLBuffer mCircleBufferObject;
    QOpenGLVertexArrayObject mVertexArrayObject;
    
    int mMvpMatrixLocation;
    int mColorLocation;

    QMatrix4x4 mPerspMatrix;
    QMatrix4x4 mTransformMatrix;
    QGLShaderProgram program;
};

#endif
