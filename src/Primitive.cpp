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

Sphere::Sphere(double radius) : mesh(Mesh::makeCube(1)) { } // TODO this shouldn't be a cube

Sphere::~Sphere() {
    delete mesh;
}

void Sphere::draw(Shader& shader, bool picking) const {
    shader.setMaterial(material);
    mesh->draw(shader);
}
