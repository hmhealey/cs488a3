#ifndef CS488_PRIMITIVE_HPP
#define CS488_PRIMITIVE_HPP

#include "Material.hpp"

struct Matrix4;
class Mesh;
class Point3D;
class Shader;
class Vector3;

class Primitive {
protected:
    Material material;
    
public:
    Primitive();
    virtual ~Primitive();

    virtual void draw(Shader& shader, bool picking = false) const = 0;

    virtual bool raycast(const Point3D& point, const Vector3& direction) const = 0;;

    const Material& getMaterial() const;
    Material& getMaterial();
    void setMaterial(const Material& material);
};

class Sphere : public Primitive {
    // maintain only a single Mesh here because the assignment states that we should only use one for all instances
    static Mesh* mesh;

public:
    Sphere();
    virtual ~Sphere();

    virtual void draw(Shader& shader, bool picking = false) const;

    virtual bool raycast(const Point3D& point, const Vector3& direction) const;
};

class Cube : public Primitive {
    Mesh* mesh;

public:
    Cube(double size = 1.0);
    virtual ~Cube();

    virtual void draw(Shader& shader, bool picking = false) const;

    virtual bool raycast(const Point3D& point, const Vector3& direction) const;
};

#endif
