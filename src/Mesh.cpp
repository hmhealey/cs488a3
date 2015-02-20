#include "Mesh.hpp"

#include <iostream>

#include "Algebra.hpp"

using namespace std;

Mesh::Mesh(unsigned int type) : type(type) { }

Mesh::~Mesh() {
    if (vertexBuffer != NULL) delete vertexBuffer;
    if (normalBuffer != NULL) delete normalBuffer;
    if (colourBuffer != NULL) delete colourBuffer;
    if (indexBuffer != NULL) delete indexBuffer;
}

void Mesh::draw(QGLShaderProgram& program) {
    int error = glGetError();
    if (error != GL_NO_ERROR) {
        cerr << "Mesh::draw - Error at start of drawing " << error << endl;
    }
    
    vertexArrayObject.bind();

    // bind vertex positions
    if (vertexBuffer != NULL) {
        if (!vertexBuffer->bind()) {
            cerr << "Mesh::draw - Unable to bind vertex buffer" << endl;
            return;
        }
        program.enableAttributeArray("vert");
        program.setAttributeBuffer("vert", GL_FLOAT, 0, 3);
    }

    // bind vertex normals
    if (normalBuffer != NULL) {
        if (!normalBuffer->bind()) {
            cerr << "Mesh::draw - Unable to bind normal buffer" << endl;
            return;
        }
        program.enableAttributeArray("normal");
        program.setAttributeBuffer("normal", GL_FLOAT, 0, 3);
    }

    // bind vertex colours
    if (colourBuffer != NULL) {
        if (!colourBuffer->bind()) {
            cerr << "Mesh::draw - Unable to bind colour buffer" << endl;
            return;
        }
        program.enableAttributeArray("colour");
        program.setAttributeBuffer("colour", GL_FLOAT, 0, 4);
    }

    if (indexBuffer != NULL) {
        error = glGetError();
        if (error != GL_NO_ERROR) {
            cerr << "Mesh::draw - Error before binding " << error << endl;
        }

        if (!indexBuffer->bind()) {
            cerr << "Mesh::draw - Unable to bind index buffer" << endl;
            return;
        }

        error = glGetError();
        if (error != GL_NO_ERROR) {
            cerr << "Mesh::draw - Error before drawing " << error << endl;
        }

        glDrawElements(type, numIndices, GL_UNSIGNED_INT, NULL);

        error = glGetError();
        if (error != GL_NO_ERROR) {
            cerr << "Mesh::draw - Error after drawing" << error << endl;
        }

        indexBuffer->release();
    } else {
        glDrawArrays(type, 0, numVertices);
    }

    vertexArrayObject.release();
}

/** Returns a square mesh with the given side length that is centered at (0, 0, 0). **/
Mesh* Mesh::makeSquare(float sideLength) {
    return Mesh::makeRectangle(sideLength, sideLength);
}

/** Returns a rectangular mesh with the given dimensions that is centered at (0, 0, 0). **/
Mesh* Mesh::makeRectangle(float width, float height) {
    float vertices[] = {
        -width / 2, height / 2, 0,
        width / 2, height / 2, 0,
        -width / 2, -height / 2, 0,
        width / 2, -height / 2, 0
    };
    int indices[] = {
        0, 2, 1,
        1, 2, 3
    };

    RawMesh raw;

    raw.numVertices = 4;
    raw.vertices = vertices;
    raw.numIndices = 2;
    raw.indices = indices;

    return raw.construct();
}

/** Returns a cube mesh with the given side length that is centered at (0, 0, 0). **/
Mesh* Mesh::makeCube(float sideLength) {
    return Mesh::makeBox(sideLength, sideLength, sideLength);
}

Mesh* Mesh::makeBox(float width, float height, float depth) {
    /*
          A***B      *+y
         *,  **      *
        C***D *      *  +x
        * E,*,F      *****
        *,  **      *
        G***H      *+z
    */
    float vertices[] = {
        // top face
        -width / 2, height / 2, -depth / 2, // A
        -width / 2, height / 2, depth / 2, // C
        width / 2, height / 2, -depth / 2, // B
        width / 2, height / 2, -depth / 2, // B
        -width / 2, height / 2, depth / 2, // C
        width / 2, height / 2, depth / 2, // D
        // bottom face
        width / 2, -height / 2, depth / 2, // H
        -width / 2, -height / 2, depth / 2, // G
        width / 2, -height / 2, -depth / 2, // F
        width / 2, -height / 2, -depth / 2, // F
        -width / 2, -height / 2, depth / 2, // G
        -width / 2, -height / 2, -depth / 2, // E
        // left face
        -width / 2, height / 2, -depth / 2, // A
        -width / 2, -height / 2, -depth / 2, // E
        -width / 2, height / 2, depth / 2, // C
        -width / 2, height / 2, depth / 2, // C
        -width / 2, -height / 2, -depth / 2, // E
        -width / 2, -height / 2, depth / 2, // G
        // right face
        width / 2, height / 2, depth / 2, // D
        width / 2, -height / 2, depth / 2, // H
        width / 2, height / 2, -depth / 2, // B
        width / 2, height / 2, -depth / 2, // B
        width / 2, -height / 2, depth / 2, // H
        width / 2, -height / 2, -depth / 2, // F
        // front face
        -width / 2, height / 2, depth / 2, // C
        -width / 2, -height / 2, depth / 2, // G
        width / 2, height / 2, depth / 2, // D
        width / 2, height / 2, depth / 2, // D
        -width / 2, -height / 2, depth / 2, // G
        width / 2, -height / 2, depth / 2, // H
        // back face
        width / 2, height / 2, -depth / 2, // B
        width / 2, -height / 2, -depth / 2, // F
        -width / 2, height / 2, -depth / 2, // A
        -width / 2, height / 2, -depth / 2, // A
        width / 2, -height / 2, -depth / 2, // F
        -width / 2, -height / 2, -depth / 2, // E
    };

    float normals[] = {
        0, 1, 0,
        0, 1, 0,
        0, 1, 0,
        0, 1, 0,
        0, 1, 0,
        0, 1, 0,
        0, -1, 0,
        0, -1, 0,
        0, -1, 0,
        0, -1, 0,
        0, -1, 0,
        0, -1, 0,
        -1, 0, 0,
        -1, 0, 0,
        -1, 0, 0,
        -1, 0, 0,
        -1, 0, 0,
        -1, 0, 0,
        1, 0, 0,
        1, 0, 0,
        1, 0, 0,
        1, 0, 0,
        1, 0, 0,
        1, 0, 0,
        0, 0, 1,
        0, 0, 1,
        0, 0, 1,
        0, 0, 1,
        0, 0, 1,
        0, 0, 1,
        0, 0, -1,
        0, 0, -1,
        0, 0, -1,
        0, 0, -1,
        0, 0, -1,
        0, 0, -1
    };

    RawMesh raw;

    raw.numVertices = 6 * 6;
    raw.vertices = vertices;
    raw.normals = normals;

    return raw.construct();
}

Mesh* Mesh::makeIcosphere(float radius, int refinement) {
    // TODO support parameters
    // adapted from http://blog.andreaskahler.com/2009/06/creating-icosphere-mesh-in-code.html
    float t = radius * (1.0 + sqrt(5.0)) / 2.0;

    float vertices[] = {
        -1, t, 0,
        1, t, 0,
        -1, -t, 0,
        1, -t, 0,
        0, -1, t,
        0, 1, t,
        0, -1, -t,
        0, 1, -t,
        t, 0, -1,
        t, 0, 1,
        -t, 0, -1,
        -t, 0, 1
    };

    float normals[36];
    for (int i = 0; i < 36; i += 3) {
        Vector3 normal(&vertices[i]);
        normal.normalize();

        normals[i] = normal[0];
        normals[i + 1] = normal[1];
        normals[i + 2] = normal[2];
    }

    int indices[] = {
        0, 11, 5,
        0, 5, 1,
        0, 1, 7,
        0, 7, 10,
        0, 10, 11,
        1, 5, 9,
        5, 11, 4,
        11, 10, 2,
        10, 7, 6,
        7, 1, 8,
        3, 9, 4,
        3, 4, 2,
        3, 2, 6,
        3, 6, 8,
        3, 8, 9,
        4, 9, 5,
        2, 4, 11,
        8, 6, 7,
        9, 8, 1
    };

    RawMesh raw;

    raw.numVertices = 12;
    raw.vertices = vertices;
    raw.normals = normals;

    raw.numIndices = 20;
    raw.indices = indices;

    return raw.construct();
}

Mesh* RawMesh::construct() const {
    Mesh* mesh = new Mesh(type);

    mesh->vertexArrayObject.bind();

    if (vertices != NULL && numVertices > 0) {
        mesh->vertexBuffer = new QOpenGLBuffer();
        mesh->numVertices = numVertices;

        mesh->vertexBuffer->create();
        mesh->vertexBuffer->setUsagePattern(QOpenGLBuffer::StaticDraw);

        mesh->vertexBuffer->bind();
        mesh->vertexBuffer->allocate(vertices, numVertices * 3 * sizeof(float));
    } else {
        cerr << "RawMesh::construct - Unable to construct Mesh with no vertices" << endl;
        return NULL;
    }

    if (normals != NULL) {
        mesh->normalBuffer = new QOpenGLBuffer();

        mesh->normalBuffer->create();
        mesh->normalBuffer->setUsagePattern(QOpenGLBuffer::StaticDraw);

        mesh->normalBuffer->bind();
        mesh->normalBuffer->allocate(normals, numVertices * 3 * sizeof(float));
    }

    if (colours != NULL) {
        mesh->colourBuffer = new QOpenGLBuffer();

        mesh->colourBuffer->create();
        mesh->colourBuffer->setUsagePattern(QOpenGLBuffer::StaticDraw);

        mesh->colourBuffer->bind();
        mesh->colourBuffer->allocate(colours, numVertices * 4 * sizeof(float));
    }

    if (indices != NULL) {
        if (numIndices > 0) {
            mesh->indexBuffer = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
            mesh->numIndices = numIndices;

            mesh->indexBuffer->create();
            mesh->indexBuffer->setUsagePattern(QOpenGLBuffer::StaticDraw);

            mesh->indexBuffer->bind();
            mesh->indexBuffer->allocate(indices, numIndices * 3 * sizeof(float));
        } else {
            cerr << "RawMesh::construct - Indexed drawing specified without an index count of <= 0" << endl;
        }
    }

    mesh->vertexArrayObject.release();

    return mesh;
}

