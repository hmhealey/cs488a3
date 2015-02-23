#ifndef SCENE_HPP
#define SCENE_HPP

#include <list>

#include "Algebra.hpp"
#include "Material.hpp"

class Primitive;
class Shader;

class SceneNode {
protected:
    // Useful for picking
    int m_id;
    std::string m_name;

    // Transformations
    Matrix4 translationRotation;
    Matrix4 scaling;

    // Hierarchy
    std::list<SceneNode*> m_children;

public:
    SceneNode(const std::string& name);
    virtual ~SceneNode();

    Matrix4 getTransform() const;
    void setTransform(const Matrix4& translationRotation, const Matrix4& scaling);

    virtual void walk_gl(Shader& shader, const Matrix4& parentTransform, bool picking = false) const;
    void walk_children(Shader& shader, const Matrix4& parentTransform, bool picking = false) const;

    void add_child(SceneNode* child) {
        m_children.push_back(child);
    }

    void remove_child(SceneNode* child) {
        m_children.remove(child);
    }

    // Callbacks to be implemented.
    // These will be called from Lua.
    void rotate(char axis, double angle);
    void scale(const Vector3& amount);
    void translate(const Vector3& amount);

    // Returns true if and only if this node is a JointNode
    virtual bool is_joint() const;
};

class JointNode : public SceneNode {
public:
    JointNode(const std::string& name);
    virtual ~JointNode();

    virtual void walk_gl(Shader& shader, const Matrix4& parentTransform, bool picking = false) const;

    virtual bool is_joint() const;

    void set_joint_x(double min, double init, double max);
    void set_joint_y(double min, double init, double max);

    struct JointRange {
        double min, init, max;
    };
  
protected:
    JointRange m_joint_x, m_joint_y;
};

class GeometryNode : public SceneNode {
public:
    GeometryNode(const std::string& name, Primitive* primitive);
    virtual ~GeometryNode();

    virtual void walk_gl(Shader& shader, const Matrix4& parentTransform, bool picking = false) const;

    const Material& getMaterial() const;
    Material& getMaterial();
    void setMaterial(const Material& material);

protected:
    Primitive* m_primitive;
};

#endif
