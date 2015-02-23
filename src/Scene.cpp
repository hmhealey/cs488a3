#include "Scene.hpp"

#include <iostream>

#include "Primitive.hpp"
#include "Shader.hpp"

using namespace std;

SceneNode::SceneNode(const std::string& name) : m_name(name) { }

SceneNode::~SceneNode() { }

void SceneNode::walk_gl(Shader& shader, const Matrix4& parentTransform, bool picking) const {
    walk_children(shader, parentTransform, picking);
}

void SceneNode::walk_children(Shader& shader, const Matrix4& parentTransform, bool picking) const {
    const Matrix4 transform(parentTransform * m_trans);
    for (auto i = m_children.cbegin(); i != m_children.cend(); i++) {
        (*i)->walk_gl(shader, transform, picking);
    }
}

void SceneNode::rotate(char axis, double angle) {
    cerr << "SceneNode::rotate - Rotating " << m_name << " around " << axis << " by " << angle << endl;
    switch(axis) {
    case 'x':
        m_trans = m_trans * Matrix4::makeRotation(angle, 0, 0);
        break;
    case 'y':
        m_trans = m_trans * Matrix4::makeRotation(0, angle, 0);
        break;
    case 'z':
        m_trans = m_trans * Matrix4::makeRotation(0, 0, angle);
        break;
    default:
        cerr << "SceneNode::rotate - " << axis << " isn't a valid axis" << endl;
        break;
    }
}

void SceneNode::scale(const Vector3& amount) {
    cerr << "SceneNode::scale - Scaling " << m_name << " by " << amount << endl;
    m_trans = m_trans * Matrix4::makeScaling(amount[0], amount[1], amount[2]);
}

void SceneNode::translate(const Vector3& amount) {
    cerr << "SceneNode::scale - Translating " << m_name << " by " << amount << endl;
    m_trans = m_trans * Matrix4::makeTranslation(amount[0], amount[1], amount[2]);
}

bool SceneNode::is_joint() const {
    return false;
}

JointNode::JointNode(const std::string& name) : SceneNode(name) { }

JointNode::~JointNode() { }

void JointNode::walk_gl(Shader& shader, const Matrix4& parentTransform, bool picking) const {
    walk_children(shader, parentTransform, picking);
}

bool JointNode::is_joint() const {
    return true;
}

void JointNode::set_joint_x(double min, double init, double max) {
    m_joint_x.min = min;
    m_joint_x.init = init;
    m_joint_x.max = max;
}

void JointNode::set_joint_y(double min, double init, double max) {
    m_joint_y.min = min;
    m_joint_y.init = init;
    m_joint_y.max = max;
}

GeometryNode::GeometryNode(const std::string& name, Primitive* primitive) : SceneNode(name), m_primitive(primitive) { }

GeometryNode::~GeometryNode() { }

void GeometryNode::walk_gl(Shader& shader, const Matrix4& parentTransform, bool picking) const {
    shader.setModelMatrix(parentTransform * m_trans);
    m_primitive->draw(shader, picking);

    walk_children(shader, parentTransform, picking);
}

const Material& GeometryNode::getMaterial() const {
    return m_primitive->getMaterial();
}

Material& GeometryNode::getMaterial() {
    return m_primitive->getMaterial();
}

void GeometryNode::setMaterial(const Material& material) {
    m_primitive->setMaterial(material);
}
 
