#include "Material.hpp"

PhongMaterial::PhongMaterial(const Colour& kd, const Colour& ks, double shininess) : kd(kd), ks(ks), shininess(shininess) { }

PhongMaterial::PhongMaterial(const PhongMaterial& other) {
    kd = other.kd;
    ks = other.ks;
    shininess = other.shininess;
}
