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
    return translationRotation * scaling;
}

void SceneNode::setTransform(const Matrix4& translationRotation, const Matrix4& scaling) {
    this->translationRotation = translationRotation;
    this->scaling = scaling;
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

bool SceneNode::raycast(const Point3D& point, const Vector3& direction) const {
    // only GeometryNodes have a volume with which to intersect
    (void) point;
    (void) direction;
    return false;
}

void SceneNode::raycastAll(const Point3D& point, const Vector3& direction) const {
    if (raycast(point, direction)) {
        cout << "ray intersects node " << m_name << endl;
    }

    Matrix4 inverse = (translationRotation * scaling).inverse();
    Point3D childPoint = inverse * point;
    Vector3 childDirection = inverse * direction;

    for (auto i = m_children.cbegin(); i != m_children.cend(); i++) {
        (*i)->raycastAll(childPoint, childDirection);
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

void SceneNode::rotate(char axis, double angle) {
    switch(axis) {
    case 'x':
        translationRotation = translationRotation * Matrix4::makeXRotation(angle);
        break;
    case 'y':
        translationRotation = translationRotation * Matrix4::makeYRotation(angle);
        break;
    case 'z':
        translationRotation = translationRotation * Matrix4::makeZRotation(angle);
        break;
    default:
        cerr << "SceneNode::rotate - " << axis << " isn't a valid axis" << endl;
        break;
    }
}

void SceneNode::scale(const Vector3& amount) {
    scaling = scaling * Matrix4::makeScaling(amount[0], amount[1], amount[2]);
}

void SceneNode::translate(const Vector3& amount) {
    translationRotation = translationRotation * Matrix4::makeTranslation(amount[0], amount[1], amount[2]);
}

SceneNode::NodeType SceneNode::getType() const {
    return SceneNode::Node;
}

JointNode::JointNode(const std::string& name, int id) : SceneNode(name, id) { }

JointNode::~JointNode() { }

void JointNode::walk_gl(Shader& shader, const Matrix4& parentTransform, bool picking) const {
    walk_children(shader, parentTransform, picking);
}

SceneNode::NodeType JointNode::getType() const {
    return SceneNode::Joint;
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

GeometryNode::GeometryNode(const std::string& name, int id, Primitive* primitive) : SceneNode(name, id), m_primitive(primitive) { }

GeometryNode::~GeometryNode() { }

void GeometryNode::walk_gl(Shader& shader, const Matrix4& parentTransform, bool picking) const {
    shader.setModelMatrix(parentTransform * getTransform());
    shader.use();

    if (!picking) {
        // use the primitive's built-in material
        m_primitive->getMaterial().applyTo(shader);
    } else {
        // assign a different material that we can later query while picking
        Material(Colour(m_id / 255.0, m_id / 255.0, m_id / 255.0)).applyTo(shader);
    }

    m_primitive->getMesh().draw(shader);

    walk_children(shader, parentTransform, picking);
}

bool GeometryNode::raycast(const Point3D& point, const Vector3& direction) const {
    (void) point;
    (void) direction;
    return false;
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
 
