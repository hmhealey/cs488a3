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

bool Sphere::raycast(const Point3D& point, const Vector3& direction) const {
    Vector3 pointToCenter = Point3D(0, 0, 0) - point;

    // t * direction is the projection of pointToCenter onto directio
    double t = pointToCenter.dot(direction) / direction.length();

    // now we have a right-angle triangle made up of pointToCenter, t * direction, and the shortest line from the center
    // of the sphere to the ray so we just take the distance of that last side
    double distance = (pointToCenter - t * direction).length();

    // our radius is 1
    return distance <= 1;
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

bool Cube::raycast(const Point3D& point, const Vector3& direction) const {
    (void) point;
    (void) direction;
    cerr << "Cube::raycast - Not yet implemented" << endl;
    return false;
}
