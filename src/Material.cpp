#include "Material.hpp"

PhongMaterial::PhongMaterial() : kd(0.2, 0.2, 0.2), ks(0.8, 0.8, 0.8), shininess(64) { }

PhongMaterial::PhongMaterial(const Colour& kd, const Colour& ks, double shininess) : kd(kd), ks(ks), shininess(shininess) { }

PhongMaterial::PhongMaterial(const PhongMaterial& other) {
    kd = other.kd;
    ks = other.ks;
    shininess = other.shininess;
}

PhongMaterial& PhongMaterial::operator=(const PhongMaterial& other) {
    kd = other.kd;
    ks = other.ks;
    shininess = other.shininess;
    return *this;
}
