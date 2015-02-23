#include "Primitive.hpp"

#include <QGLShaderProgram>

#include "Algebra.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Viewer.hpp"

Primitive::Primitive() : material(new FlatMaterial()) { }

Primitive::~Primitive() { }

// I feel like I don't understand enough about templates to make this more elegant
void Primitive::setMaterial(const FlatMaterial& material) {
    delete this->material;
    this->material = new FlatMaterial(material);
}
void Primitive::setMaterial(const PhongMaterial& material) {
    delete this->material;
    this->material = new PhongMaterial(material);
}

Sphere::Sphere(double radius) : mesh(Mesh::makeUvSphere(radius, 64, 64)) { } 

Sphere::~Sphere() {
    delete mesh;
}

void Sphere::draw(Shader& shader, bool picking) const {
    shader.use();
    material->applyTo(shader);
    mesh->draw(shader);
}

Cube::Cube(double size) : mesh(Mesh::makeCube(size)) { }

Cube::~Cube() {
    delete mesh;
}

void Cube::draw(Shader& shader, bool picking) const {
    shader.use();
    material->applyTo(shader);
    mesh->draw(shader);
}
