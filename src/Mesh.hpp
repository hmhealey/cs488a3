/*****************************************
 * Harrison Healey - hmhealey - 20376857 *
 *****************************************/

#ifndef __MESH_HPP__
#define __MESH_HPP__

#include <QGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QtOpenGL>

#include "Algebra.hpp"

class Shader;

class Mesh {
    QOpenGLVertexArrayObject vertexArrayObject;

    int numVertices;
    QOpenGLBuffer* vertexBuffer = NULL;
    QOpenGLBuffer* normalBuffer = NULL;

    int numIndices;
    QOpenGLBuffer* indexBuffer = NULL;

    unsigned int type;

public:
    Mesh(unsigned int type = GL_TRIANGLES);
    ~Mesh();

    void bind();
    void release();

    void draw(Shader& shader);

    static Mesh* makeSquare(float sideLength);
    static Mesh* makeRectangle(float width, float height);
    static Mesh* makeCube(float sideLength);
    static Mesh* makeBox(float width, float height, float depth);
    static Mesh* makeUvSphere(float radium, int horizontalResolution, int verticalResolution);

    friend struct RawMesh;
};

struct RawMesh {
    int numVertices = 0;
    float* vertices = NULL;

    float* normals = NULL;

    int numIndices = 0;
    int* indices = NULL;

    unsigned int type = GL_TRIANGLES;

    Mesh *construct() const;
};

#endif
