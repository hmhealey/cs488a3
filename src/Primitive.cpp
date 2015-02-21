#include "Primitive.hpp"

#include <QGLShaderProgram>

#include "Algebra.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Viewer.hpp"

Primitive::~Primitive() { }

void Primitive::setMaterial(const PhongMaterial& material) {
    this->material = material;
}

Sphere::Sphere(double radius) : mesh(Mesh::makeUvSphere(radius, 64, 64)) { } 

Sphere::~Sphere() {
    delete mesh;
}

void Sphere::draw(Shader& shader, bool picking) const {
    shader.setMaterial(material);
    mesh->draw(shader);
}

Cube::Cube(double size) : mesh(Mesh::makeCube(size)) { }

Cube::~Cube() {
    delete mesh;
}

void Cube::draw(Shader& shader, bool picking) const {
    shader.setMaterial(material);
    mesh->draw(shader);
}
