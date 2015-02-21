#ifndef __SHADER_HPP__
#define __SHADER_HPP__

#include <QGLShaderProgram>

#include "Algebra.hpp"

class Shader {
    QGLShaderProgram program;

    Matrix4 model;
    Matrix4 view;
    Matrix4 perspective;

public:
    Shader(const char* name);
    Shader(const Shader& other);

    void use();

    QGLShaderProgram& getProgram();

    const Matrix4& getModelMatrix() const;
    void setModelMatrix(const Matrix4& model);

    const Matrix4& getViewMatrix() const;
    void setViewMatrix(const Matrix4& view);

    const Matrix4& getPerspectiveMatrix() const;
    void setPerspectiveMatrix(const Matrix4& perspective);

private:
    static std::string generateShaderPath(const char* path, const char* name, const char* type);
};

#endif
