#include "Scene.hpp"

#include <iostream>

#include "Primitive.hpp"
#include "Viewer.hpp"

SceneNode::SceneNode(const std::string& name) : m_name(name) { }

SceneNode::~SceneNode() { }

void SceneNode::walk_gl(Viewer& viewer, const Matrix4& parentTransform, bool picking) const {
    walk_children(viewer, parentTransform, picking);
}

void SceneNode::walk_children(Viewer& viewer, const Matrix4& parentTransform, bool picking) const {
    const Matrix4 transform(parentTransform * m_trans);
    for (auto i = m_children.cbegin(); i != m_children.cend(); i++) {
        (*i)->walk_gl(viewer, transform, picking);
    }
}

void SceneNode::rotate(char axis, double angle) {
    std::cerr << "Stub: Rotate " << m_name << " around " << axis << " by " << angle << std::endl;
    // Fill me in
}

void SceneNode::scale(const Vector3& amount) {
    std::cerr << "Stub: Scale " << m_name << " by " << amount << std::endl;
    // Fill me in
}

void SceneNode::translate(const Vector3& amount) {
    std::cerr << "Stub: Translate " << m_name << " by " << amount << std::endl;
    // Fill me in
}

bool SceneNode::is_joint() const {
    return false;
}

JointNode::JointNode(const std::string& name) : SceneNode(name) { }

JointNode::~JointNode() { }

void JointNode::walk_gl(Viewer& viewer, const Matrix4& parentTransform, bool picking) const {
    walk_children(viewer, parentTransform, picking);
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

void GeometryNode::walk_gl(Viewer& viewer, const Matrix4& parentTransform, bool picking) const {
    m_primitive->draw(viewer.getShader(), picking);

    walk_children(viewer, parentTransform, picking);
}
 
