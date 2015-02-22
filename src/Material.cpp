#include "Material.hpp"

#include "Shader.hpp"

Material::~Material() { }

FlatMaterial::FlatMaterial() { }

FlatMaterial::FlatMaterial(const Colour& colour) : colour(colour) { }

FlatMaterial::FlatMaterial(const FlatMaterial& other) {
    colour = other.colour;
}

FlatMaterial& FlatMaterial::operator=(const FlatMaterial& other) {
    colour = other.colour;
    return *this;
}

void FlatMaterial::applyTo(Shader& shader) const {
    QGLShaderProgram& program = shader.getProgram();

    program.setUniformValue("materialAmbient", 0.0, 0.0, 0.0, 1.0);
    program.setUniformValue("materialDiffuse", colour[0], colour[1], colour[2], 1.0);
    program.setUniformValue("materialSpecular", 0.0, 0.0, 0.0, 1.0);
    program.setUniformValue("materialShininess", 128.0f);
}

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

void PhongMaterial::applyTo(Shader& shader) const {
    QGLShaderProgram& program = shader.getProgram();

    program.setUniformValue("materialAmbient", 1.0, 1.0, 1.0, 1.0);
    program.setUniformValue("materialDiffuse", kd[0], kd[1], kd[2], 1.0);
    program.setUniformValue("materialSpecular", ks[0], ks[1], ks[2], 1.0);
    program.setUniformValue("materialShininess", (GLfloat) shininess);
}
