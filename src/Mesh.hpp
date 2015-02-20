#ifndef __MESH_HPP__
#define __MESH_HPP__

#include <QGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

class Mesh {
private:
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

#endif
