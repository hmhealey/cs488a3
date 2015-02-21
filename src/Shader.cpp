#include "Shader.hpp"

#include <QFile>
#include <QOpenGLBuffer>
#include <sstream>

#include "AlgebraToQt.hpp"

using namespace std;

// shaders can either be located in the current directory or a src subdirectory
static const char* SHADER_PATHS[] = {
    ".",
    "src",
    NULL
};

Shader::Shader() : colour(Colour(1.0, 1.0, 1.0)) { }

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
    program.setUniformValue("modelViewProjection", toQt(projection * view.inverse() * model));

    program.setUniformValue("colour", colour[0], colour[1], colour[2], 1.0);
}

void Shader::bindBuffer(const char* name, QOpenGLBuffer& buffer, unsigned int type, int components) {
    if (buffer.bind()) {
        program.enableAttributeArray(name);
        program.setAttributeArray(name, type, 0, components);
    } else {
        cerr << "Shader::bindBuffer - Unable to bind buffer to " << name << endl;
        return;
    }
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

const Matrix4& Shader::getProjectionMatrix() const {
    return projection;
}

void Shader::setProjectionMatrix(const Matrix4& projection) {
    this->projection = projection;
}

const Colour& Shader::getColour() const {
    return colour;
}

void Shader::setColour(const Colour& colour) {
    this->colour = colour;
}

string Shader::generateShaderPath(const char* basePath, const char* name, const char* type) {
    stringstream path;
    path << basePath << "/" << name << "." << type;
    return path.str();
}
