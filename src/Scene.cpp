#include "Scene.hpp"

#include <iostream>

#include "Algebra.hpp"
#include "Mesh.hpp"
#include "Primitive.hpp"
#include "Shader.hpp"

using namespace std;

SceneNode::SceneNode(const std::string& name, int id) : m_id(id), m_name(name) { }

SceneNode::~SceneNode() { }

string SceneNode::getName() const {
    return m_name;
}

int SceneNode::getId() const {
    return m_id;
}

bool SceneNode::isSelected() const {
    return selected;
}

void SceneNode::setSelected(bool selected) {
    this->selected = selected;
}

Matrix4 SceneNode::getTransform() const {
    return transform;
}

void SceneNode::setTransform(const Matrix4& transform) {
    this->transform = transform;
}

void SceneNode::walk_gl(Shader& shader, const Matrix4& parentTransform, bool picking) const {
    walk_children(shader, parentTransform, picking);
}

void SceneNode::walk_children(Shader& shader, const Matrix4& parentTransform, bool picking) const {
    const Matrix4 transform(parentTransform * getTransform());
    for (auto i = m_children.cbegin(); i != m_children.cend(); i++) {
        (*i)->walk_gl(shader, transform, picking);
    }
}

SceneNode* SceneNode::getById(int id) {
    if (id == m_id) {
        return this;
    } else {
        for (auto i = m_children.cbegin(); i != m_children.cend(); i++) {
            SceneNode* node = (*i)->getById(id);

            if (node != NULL) {
                return node;
            }
        }

        return NULL;
    }
}

bool SceneNode::pick(int id) {
    for (auto i = m_children.cbegin(); i != m_children.cend(); i++) {
        SceneNode* child = *i;

        if (child->m_id == id) {
            // we've found the node we're looking for, but only nodes directly under Joints are selectable
            // so we only actually select it if we're a joint
            if (getType() == SceneNode::Joint) {
                child->selected = !child->selected;
            }

            // even if we couldn't select the node, return true to indicate that we found it
            return true;
        } else {
            // search depth-first for the node with the given id
            bool found = child->pick(id);

            if (found) {
                return true;
            }
        }
    }

    return false;
}

void SceneNode::rotate(char axis, double angle) {
    switch(axis) {
    case 'x':
        transform = transform * Matrix4::makeXRotation(angle);
        break;
    case 'y':
        transform = transform * Matrix4::makeYRotation(angle);
        break;
    case 'z':
        transform = transform * Matrix4::makeZRotation(angle);
        break;
    default:
        cerr << "SceneNode::rotate - " << axis << " isn't a valid axis" << endl;
        break;
    }
}

void SceneNode::scale(const Vector3& amount) {
    transform = transform * Matrix4::makeScaling(amount[0], amount[1], amount[2]);
}

void SceneNode::translate(const Vector3& amount) {
    transform = transform * Matrix4::makeTranslation(amount[0], amount[1], amount[2]);
}

SceneNode::NodeType SceneNode::getType() const {
    return SceneNode::Node;
}

JointNode::JointNode(const std::string& name, int id) : SceneNode(name, id) { }

JointNode::~JointNode() { }

Matrix4 JointNode::getTransform() const {
    return Matrix4::makeYRotation(yRotation) * Matrix4::makeXRotation(xRotation) * transform;
}

void JointNode::walk_gl(Shader& shader, const Matrix4& parentTransform, bool picking) const {
    walk_children(shader, parentTransform, picking);
}

SceneNode::NodeType JointNode::getType() const {
    return SceneNode::Joint;
}

void JointNode::setXRange(double min, double init, double max) {
    xRange.min = min;
    xRange.init = init;
    xRange.max = max;
}

void JointNode::setYRange(double min, double init, double max) {
    yRange.min = min;
    yRange.init = init;
    yRange.max = max;
}

GeometryNode::GeometryNode(const std::string& name, int id, Primitive* primitive) : SceneNode(name, id), m_primitive(primitive) { }

GeometryNode::~GeometryNode() { }

void GeometryNode::walk_gl(Shader& shader, const Matrix4& parentTransform, bool picking) const {
    shader.setModelMatrix(parentTransform * getTransform());
    shader.use();

    if (!picking) {
        const Material& material = m_primitive->getMaterial();
        if (!selected) {
            // use the primitive's built-in material
            material.applyTo(shader);
        } else {
            // make a selected shape's material just the inverse of the normal one
            Material(material.diffuse.inverse(), material.specular, material.shininess).applyTo(shader);
        }
    } else {
        // assign a different material that we can later query while picking
        Material(Colour(m_id / 255.0, m_id / 255.0, m_id / 255.0)).applyTo(shader);
    }

    m_primitive->getMesh().draw(shader);

    walk_children(shader, parentTransform, picking);
}

SceneNode::NodeType GeometryNode::getType() const {
    return SceneNode::Geometry;
}

const Material& GeometryNode::getMaterial() const {
    return m_primitive->getMaterial();
}

void GeometryNode::setMaterial(const Material& material) {
    m_primitive->setMaterial(material);
}
 
