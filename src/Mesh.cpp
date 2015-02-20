#include "Mesh.hpp"

#include <iostream>
#include <QtOpenGL>

#include "Algebra.hpp"

using namespace std;

Mesh::Mesh(const float vertices[], int numVertices) : Mesh(vertices, numVertices, NULL, -1) { }

Mesh::Mesh(const float vertices[], int numVertices, const int indices[], int numIndices) : indexBuffer(QOpenGLBuffer::IndexBuffer), numIndices(numIndices) {
    int error = glGetError();
    if (error != GL_NO_ERROR) {
        cerr << "Mesh::Mesh - Error at start of constructor " << error << endl;
    }

    // create vertex array object
    if (!vertexArrayObject.create()) {
        cerr << "Mesh::Mesh - Unable to create vertex array object" << endl;
        return;
    }
    vertexArrayObject.bind();

    // initialize buffers
    vertexBuffer.create();
    vertexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    setVertices(vertices, numVertices);

    colourBuffer.create();
    colourBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);

    normalBuffer.create();
    normalBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);

    indexBuffer.create();
    indexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    setIndices(indices, numIndices);

    error = glGetError();
    if (error != GL_NO_ERROR) {
        cerr << "Mesh::Mesh - Error at end of constructor " << error << endl;
    }
}

Mesh::~Mesh() { }

/** Draws the mesh using the given shader program. Assumes that the shader program has already been bound
 *  and required uniforms have already been set up. **/
void Mesh::draw(QGLShaderProgram& program) {
    int error = glGetError();
    if (error != GL_NO_ERROR) {
        cerr << "Mesh::draw - Error at start of drawing " << error << endl;
    }
    
    vertexArrayObject.bind();

    // bind vertex positions
    if (!vertexBuffer.bind()) {
        cerr << "Mesh::draw - Unable to bind vertex buffer" << endl;
        return;
    }
    program.enableAttributeArray("vert");
    program.setAttributeBuffer("vert", GL_FLOAT, 0, 3);

    // bind vertex normals
    if (!normalBuffer.bind()) {
        cerr << "Mesh::draw - Unable to bind normal buffer" << endl;
        return;
    }
    program.enableAttributeArray("normal");
    program.setAttributeBuffer("normal", GL_FLOAT, 0, 3);

    // bind vertex colours
    if (!colourBuffer.bind()) {
        cerr << "Mesh::draw - Unable to bind colour buffer" << endl;
        return;
    }
    program.enableAttributeArray("colour");
    program.setAttributeBuffer("colour", GL_FLOAT, 0, 4);

    if (numIndices >= 0) {
        error = glGetError();
        if (error != GL_NO_ERROR) {
            cerr << "Mesh::draw - Error before binding " << error << endl;
        }

        if (!indexBuffer.bind()) {
            cerr << "Mesh::draw - Unable to bind index buffer" << endl;
            return;
        }

        error = glGetError();
        if (error != GL_NO_ERROR) {
            cerr << "Mesh::draw - Error before drawing " << error << endl;
        }

        glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, NULL);

        error = glGetError();
        if (error != GL_NO_ERROR) {
            cerr << "Mesh::draw - Error after drawing" << error << endl;
        }

        indexBuffer.release();
    } else {
        glDrawArrays(GL_TRIANGLES, 0, numVertices);
    }

    vertexArrayObject.release();
}

/** Fills an array with the vertices of this Mesh. Assumes that the destination array has enough space
 *  to store 3 * getNumVertices() floats in it. **/
void Mesh::getVertices(float dest[]) {
    if (!vertexBuffer.bind()) {
        cerr << "Mesh::getVertices - Unable to bind vertex buffer" << endl;
        return;
    }
    // TODO figure out why this doesn't work in the graphics lab
    //glGetBufferSubData(GL_ARRAY_BUFFER, 0, numVertices * 3 * sizeof(float), dest);
    (void)dest;
}

/** Copies an array of vertices into this Mesh. Assumes that numVertices matches the number of complete
 *  vertices being passed in (ie. sizeof(vertices) / 3). **/
void Mesh::setVertices(const float vertices[], int numVertices) {
    if (!vertexBuffer.bind()) {
        cerr << "Mesh::setVertices - Unable to bind vertex buffer" << endl;
        return;
    }
    vertexBuffer.allocate(vertices, numVertices * 3 * sizeof(float));
    this->numVertices = numVertices;
}

void Mesh::setNormals(const float normals[]) {
    if (!normalBuffer.bind()) {
        cerr << "Mesh::setNormals - Unable to bind normal buffer" << endl;
        return;
    }
    normalBuffer.allocate(normals, numVertices * 3 * sizeof(float));
}

/** Fills an array with the vertex colours of this Mesh. Assumes that the destination array has
 *  enough space to store 4 * getNumVertices() floats in it. **/
void Mesh::getColours(float dest[]) {
    if (!colourBuffer.bind()) {
        cerr << "Mesh::getColours - Unable to bind colour buffer" << endl;
        return;
    }
    // TODO figure out why this doesn't work in the graphics lab
    //glGetBufferSubData(GL_ARRAY_BUFFER, 0, numVertices * 4 * sizeof(float), dest);
    (void)dest;
}

/** Copies an array of vertex colours into this Mesh. Assumes that there is one colour per vertex
 *  in this Mesh (ie.  number of entries in colours is 4 * getNumVertices()). **/
void Mesh::setColours(const float colours[]) {
    if (!colourBuffer.bind()) {
        cerr << "Mesh::setColours - Unable to bind colour buffer" << endl;
        return;
    }
    colourBuffer.allocate(colours, numVertices * 4 * sizeof(float));
}

void Mesh::setIndices(const int indices[], int numIndices) {
    if (!indexBuffer.bind()) {
        cerr << "Mesh::setIndices - Unable to bind index buffer" << endl;
        return;
    }

    if (numIndices != -1) {
        indexBuffer.allocate(indices, numIndices * 3 * sizeof(int));
    } else if (this->numIndices != -1) {
        // TODO support removing indices? or just don't support it
    }

    this->numIndices = numIndices;
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

    return new Mesh(vertices, sizeof(vertices), indices, sizeof(indices));
}

/** Returns a cube mesh with the given side length that is centered at (0, 0, 0). **/
Mesh* Mesh::makeCube(float sideLength) {
    return Mesh::makeBox(sideLength, sideLength, sideLength);
}

/** Returns a box mesh with the given dimensions that is centered at (0, 0, 0). **/
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
    
    Mesh* mesh = new Mesh(vertices, 36);
    mesh->setNormals(normals);

    return mesh;
}

Mesh* Mesh::makeIcosphere(int refinement, float radius) {
    // TODO support parameters
    // adapted from http://blog.andreaskahler.com/2009/06/creating-icosphere-mesh-in-code.html
    float t = radius * (1.0 + sqrt(5.0)) / 2.0;

    int numVertices = 36;
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

    int numIndices = 60;
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

    float normals[numVertices];
    for (size_t i = 0; i < numVertices; i += 3) {
        cerr << "a " << vertices[i] << " " << vertices[i + 1] << " " << vertices[i + 2] << endl;
        Vector3 normal(&vertices[i]);
        cerr << "aa " << normal << endl;
        normal.normalize();

        normals[i] = normal[0];
        normals[i + 1] = normal[1];
        normals[i + 2] = normal[2];
    }

    Mesh* mesh = new Mesh(vertices, 12, indices, 20);
    mesh->setNormals(normals);

    return mesh;
}
