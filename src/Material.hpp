#ifndef CS488_MATERIAL_HPP
#define CS488_MATERIAL_HPP

#include "Algebra.hpp"

class Shader;

struct Material {
    virtual ~Material();

    virtual void applyTo(Shader& shader) const = 0;
};

struct FlatMaterial : public Material {
    Colour colour;

    FlatMaterial();
    FlatMaterial(const Colour& colour);
    FlatMaterial(const FlatMaterial& other);

    FlatMaterial& operator=(const FlatMaterial& other);

    virtual void applyTo(Shader& shader) const;
};

struct PhongMaterial : public Material {
    Colour kd;
    Colour ks;

    double shininess;

    PhongMaterial();
    PhongMaterial(const Colour& kd, const Colour& ks, double shininess);
    PhongMaterial(const PhongMaterial& other);

    PhongMaterial& operator=(const PhongMaterial& other);

    virtual void applyTo(Shader& shader) const;
};

#endif
