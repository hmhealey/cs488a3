#ifndef CS488_PRIMITIVE_HPP
#define CS488_PRIMITIVE_HPP

#include "Material.hpp"

struct Matrix4;
class Mesh;
class Shader;

class Primitive {
protected:
    PhongMaterial material;
    
public:
    virtual ~Primitive();

    void setMaterial(const PhongMaterial& material);

    virtual void draw(Shader& shader, bool picking = false) const = 0;
};

class Sphere : public Primitive {
    Mesh* mesh;

public:
    Sphere(double radius = 1.0);
    virtual ~Sphere();

    virtual void draw(Shader& shader, bool picking = false) const;
};

#endif
