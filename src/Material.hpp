#ifndef CS488_MATERIAL_HPP
#define CS488_MATERIAL_HPP

#include "Algebra.hpp"

class Shader;

struct Material {
    Colour diffuse;
    Colour specular;
    double shininess;

    Material();
    Material(const Colour& diffuse);
    Material(const Colour& diffuse, const Colour& specular, double shininess);
    Material(const Material& other);

    Material& operator=(const Material& other);

    void applyTo(Shader& shader) const;
};

#endif
