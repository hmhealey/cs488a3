#include "Primitive.hpp"

#include <QGLShaderProgram>

#include "Algebra.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Viewer.hpp"

Primitive::Primitive() { }

Primitive::~Primitive() { }

const Material& Primitive::getMaterial() const {
    return material;
}

Material& Primitive::getMaterial() {
    return material;
}

void Primitive::setMaterial(const Material& material) {
    this->material = material;
}

Mesh* Sphere::mesh = NULL;

Sphere::Sphere() { } 

Sphere::~Sphere() { }

void Sphere::draw(Shader& shader, bool picking) const {
    if (Sphere::mesh == NULL) {
        Sphere::mesh = Mesh::makeUvSphere(1.0, 64, 64);
    }

    shader.use();
    material.applyTo(shader);
    Sphere::mesh->draw(shader);
}

Cube::Cube(double size) : mesh(Mesh::makeCube(size)) { }

Cube::~Cube() {
    delete mesh;
}

void Cube::draw(Shader& shader, bool picking) const {
    shader.use();
    material.applyTo(shader);
    mesh->draw(shader);
}
