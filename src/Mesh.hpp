#ifndef __MESH_HPP__
#define __MESH_HPP__

#include <QGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QtOpenGL>

struct RawMesh;

class Mesh {
    QOpenGLVertexArrayObject vertexArrayObject;

    int numVertices;
    QOpenGLBuffer* vertexBuffer = NULL;
    QOpenGLBuffer* normalBuffer = NULL;
    QOpenGLBuffer* colourBuffer = NULL;

    int numIndices;
    QOpenGLBuffer* indexBuffer = NULL;

    unsigned int type;

public:
    Mesh(unsigned int type = GL_TRIANGLES);
    ~Mesh();

    void bind();
    void release();

    void draw(QGLShaderProgram& program);

    static Mesh* makeSquare(float sideLength);
    static Mesh* makeRectangle(float width, float height);
    static Mesh* makeCube(float sideLength);
    static Mesh* makeBox(float width, float height, float depth);
    static Mesh* makeIcosphere(float radius, int refinement);

    friend struct RawMesh;
};

struct RawMesh {
    int numVertices = 0;
    float* vertices = NULL;

    float* normals = NULL;

    float* colours = NULL;

    int numIndices = 0;
    int* indices = NULL;

    unsigned int type = GL_TRIANGLES;

    Mesh *construct() const;

};

#endif
