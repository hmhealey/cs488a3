/*****************************************
 * Harrison Healey - hmhealey - 20376857 *
 *****************************************/

#include "Material.hpp"

#include "Shader.hpp"

Material::Material() : diffuse(0.2, 0.2, 0.2), specular(0.8, 0.8, 0.8), shininess(64) { }

Material::Material(const Colour& diffuse) : diffuse(diffuse), specular(0.8, 0.8, 0.8), shininess(64) { }

Material::Material(const Colour& diffuse, const Colour& specular, double shininess) : diffuse(diffuse), specular(specular), shininess(shininess) { }

Material::Material(const Material& other) {
    diffuse = other.diffuse;
    specular = other.specular;
    shininess = other.shininess;
}

Material& Material::operator=(const Material& other) {
    diffuse = other.diffuse;
    specular = other.specular;
    shininess = other.shininess;
    return *this;
}

void Material::applyTo(Shader& shader) const {
    QGLShaderProgram& program = shader.getProgram();

    program.setUniformValue("materialAmbient", 1.0, 1.0, 1.0, 1.0);
    program.setUniformValue("materialDiffuse", diffuse[0], diffuse[1], diffuse[2], 1.0);
    program.setUniformValue("materialSpecular", specular[0], specular[1], specular[2], 1.0);
    program.setUniformValue("materialShininess", (GLfloat) shininess);
}
