#include "Primitive.hpp"

#include <iostream>
#include <QGLShaderProgram>

#include "Algebra.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Viewer.hpp"

using namespace std;

Primitive::Primitive() { }

Primitive::~Primitive() { }

const Material& Primitive::getMaterial() const {
    return material;
}

void Primitive::setMaterial(const Material& material) {
    this->material = material;
}

Mesh* Sphere::mesh = NULL;

Sphere::Sphere() { } 

Sphere::~Sphere() { }

Mesh& Sphere::getMesh() {
    if (Sphere::mesh == NULL) {
        Sphere::mesh = Mesh::makeUvSphere(1.0, 64, 64);
    }

    return *Sphere::mesh;
}

Cube::Cube(double size) : mesh(Mesh::makeCube(size)) { }

Cube::~Cube() {
    delete mesh;
}

Mesh& Cube::getMesh() {
    return *mesh;
}
