#include "Shader.hpp"

#include <QFile>
#include <QtOpenGL>
#include <sstream>

#include "AlgebraToQt.hpp"

using namespace std;

// shaders can either be located in the current directory or a src subdirectory
static const char* SHADER_PATHS[] = {
    ".",
    "src",
    NULL
};

Shader::Shader() { }

void Shader::initialize(const char* name) {
    for (int i = 0; SHADER_PATHS[i] != NULL; i++) {
        string vertexShaderPath = generateShaderPath(SHADER_PATHS[i], name, "vert");
        string fragmentShaderPath = generateShaderPath(SHADER_PATHS[i], name, "frag");

        if (QFile(vertexShaderPath.c_str()).exists() && QFile(fragmentShaderPath.c_str()).exists()) {
            if (!program.addShaderFromSourceFile(QGLShader::Vertex, vertexShaderPath.c_str())) {
                cerr << "Shader::Shader - Unable to load vertex shader from " << vertexShaderPath  << " : " << program.log().toStdString() << endl;
                return;
            }

            if (!program.addShaderFromSourceFile(QGLShader::Fragment, fragmentShaderPath.c_str())) {
                cerr << "Shader::Shader - Unable to load fragment shader from " << fragmentShaderPath << " : " << program.log().toStdString() << endl;
                return;
            }

            if (!program.link()) {
                cerr << "Shader::Shader - Unable to link shaders" << endl;
            }

            return;
        }
    }

    cerr << "Shader::Shader - Unable to find shader source files" << endl;
}

void Shader::use() {
    program.bind();

    program.setUniformValue("modelView", toQt(view.inverse() * model));
    program.setUniformValue("modelViewProjection", toQt(perspective * view.inverse() * model));
}

QGLShaderProgram& Shader::getProgram() {
    return program;
}

const Matrix4& Shader::getModelMatrix() const {
    return model;
}

void Shader::setModelMatrix(const Matrix4& model) {
    this->model = model;
}

const Matrix4& Shader::getViewMatrix() const {
    return view;
}

void Shader::setViewMatrix(const Matrix4& view) {
    this->view = view;
}

const Matrix4& Shader::getPerspectiveMatrix() const {
    return perspective;
}

void Shader::setPerspectiveMatrix(const Matrix4& perspective) {
    this->perspective = perspective;
}

string Shader::generateShaderPath(const char* basePath, const char* name, const char* type) {
    stringstream path;
    path << basePath << "/" << name << "." << type;
    return path.str();
}
