#ifndef __MESH_HPP__
#define __MESH_HPP__

#include <QGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QtOpenGL>

class Mesh {
    QOpenGLVertexArrayObject vertexArrayObject;

    QOpenGLBuffer vertexBuffer;
    QOpenGLBuffer normalBuffer;
    QOpenGLBuffer colourBuffer;
    QOpenGLBuffer indexBuffer;

    int numVertices;
    int numIndices;
    
public:
    Mesh(const float vertices[], int numVertices);
    Mesh(const float vertices[], int numVertices, const int indices[], int numIndices);
    Mesh(const Mesh &mesh);
    ~Mesh();

    void draw(QGLShaderProgram& program);

    int getNumVertices() const {
        return numVertices;
    }
    void getVertices(float dest[]);
    void setVertices(const float vertices[], int numVertices);

    void getColours(float dest[]);
    void setColours(const float colours[]);

    //int[] getTriangles(int[] dest) const;
    void setIndices(const int indices[], int numIndices);

    void setNormals(const float normals[]);

    static Mesh* makeSquare(float sideLength);
    static Mesh* makeRectangle(float width, float height);
    static Mesh* makeCube(float sideLength);
    static Mesh* makeBox(float width, float height, float depth);
    static Mesh* makeIcosphere(int refinement, float radius);

};

struct RawMesh;

class NMesh {
    QOpenGLVertexArrayObject vertexArrayObject;

    int numVertices;
    QOpenGLBuffer* vertexBuffer = NULL;
    QOpenGLBuffer* normalBuffer = NULL;
    QOpenGLBuffer* colourBuffer = NULL;

    int numIndices;
    QOpenGLBuffer* indexBuffer = NULL;

    unsigned int type;

public:
    NMesh(unsigned int type = GL_TRIANGLES);
    ~NMesh();

    void bind();
    void release();

    void draw(QGLShaderProgram& program);

    static NMesh* makeSquare(float sideLength);
    static NMesh* makeRectangle(float width, float height);
    static NMesh* makeCube(float sideLength);
    static NMesh* makeBox(float width, float height, float depth);
    static NMesh* makeIcosphere(float radius, int refinement);

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

    NMesh *construct() const;

};

#endif
