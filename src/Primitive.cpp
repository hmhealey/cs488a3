#include "Primitive.hpp"

#include "Mesh.hpp"

Primitive::~Primitive() { }

Sphere::Sphere(double radius) { }

Sphere::~Sphere() {
    delete mesh;
}

void Sphere::walk_gl(bool picking) const {
    // Fill me in
}
