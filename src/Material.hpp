#ifndef CS488_MATERIAL_HPP
#define CS488_MATERIAL_HPP

#include "Algebra.hpp"

struct Material { };

struct PhongMaterial : public Material {
    Colour kd;
    Colour ks;

    double shininess;

    PhongMaterial();
    PhongMaterial(const Colour& kd, const Colour& ks, double shininess);
    PhongMaterial(const PhongMaterial& other);

    PhongMaterial& operator=(const PhongMaterial& other);
};

#endif
