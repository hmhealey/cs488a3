#ifndef CS488_PRIMITIVE_HPP
#define CS488_PRIMITIVE_HPP

#include "Algebra.hpp"

class Mesh;

class Primitive {
public:
    virtual ~Primitive();
    virtual void walk_gl(bool picking) const = 0;
};

class Sphere : public Primitive {
    Mesh* mesh;
public:
    Sphere(double radius = 1.0);
    virtual ~Sphere();
    virtual void walk_gl(bool picking) const;
};

#endif
