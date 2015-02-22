#ifndef __SHADER_HPP__
#define __SHADER_HPP__

#include <QGLShaderProgram>
#include <QtOpenGL>

#include "Algebra.hpp"
#include "Material.hpp"

class QOpenGLBuffer;

class Shader {
    QGLShaderProgram program;

    Matrix4 model;
    Matrix4 view;
    Matrix4 projection;

public:
    Shader();
    Shader(const Shader& other);

    void initialize(const char* name);

    void use();

    void bindBuffer(const char* name, QOpenGLBuffer& buffer, unsigned int type = GL_FLOAT, int components = 3);

    QGLShaderProgram& getProgram();

    const Matrix4& getModelMatrix() const;
    void setModelMatrix(const Matrix4& model);

    const Matrix4& getViewMatrix() const;
    void setViewMatrix(const Matrix4& view);

    const Matrix4& getProjectionMatrix() const;
    void setProjectionMatrix(const Matrix4& projection);

    void applyMaterial(const Material& material);

private:
    static std::string generateShaderPath(const char* path, const char* name, const char* type);
};

#endif
