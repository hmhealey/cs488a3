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
    QOpenGLBuffer* colourBuffer = NULL;

    int numIndices;
    QOpenGLBuffer* indexBuffer = NULL;

    unsigned int type;

public:
    Mesh(unsigned int type = GL_TRIANGLES);
    ~Mesh();

    void bind();
    void release();

    void draw(Shader& shader);

    static Mesh* makeSquare(float sideLength, const Colour& colour);
    static Mesh* makeRectangle(float width, float height, const Colour& colour);
    static Mesh* makeCube(float sideLength, const Colour& colour);
    static Mesh* makeBox(float width, float height, float depth, const Colour& colour);
    static Mesh* makeIcosphere(float radius, int refinement, const Colour& colour);

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
