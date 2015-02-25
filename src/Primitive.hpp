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

    const Material& getMaterial() const;
    void setMaterial(const Material& material);

    virtual Mesh& getMesh() = 0;

    virtual bool raycast(const Point3D& point, const Vector3& direction) const = 0;;
};

class Sphere : public Primitive {
    // maintain only a single Mesh here because the assignment states that we should only use one for all instances
    static Mesh* mesh;

public:
    Sphere();
    virtual ~Sphere();

    virtual Mesh& getMesh();

    virtual bool raycast(const Point3D& point, const Vector3& direction) const;
};

class Cube : public Primitive {
    Mesh* mesh;

public:
    Cube(double size = 1.0);
    virtual ~Cube();

    virtual Mesh& getMesh();

    virtual bool raycast(const Point3D& point, const Vector3& direction) const;
};

#endif
