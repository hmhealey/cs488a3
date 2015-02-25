#ifndef SCENE_HPP
#define SCENE_HPP

#include <list>

#include "Algebra.hpp"
#include "Material.hpp"

class Primitive;
class Shader;

class SceneNode {
public:
    enum NodeType {
        Node,
        Joint,
        Geometry
    };
protected:
    // Useful for picking
    int m_id;
    std::string m_name;

    bool selected = false;

    // Transformations
    Matrix4 translationRotation;
    Matrix4 scaling;

    // Hierarchy
    std::list<SceneNode*> m_children;

public:
    SceneNode(const std::string& name, int id);
    virtual ~SceneNode();

    std::string getName() const;
    int getId() const;

    bool isSelected() const;
    void setSelected(bool selected);

    Matrix4 getTransform() const;
    void setTransform(const Matrix4& translationRotation, const Matrix4& scaling);

    virtual void walk_gl(Shader& shader, const Matrix4& parentTransform, bool picking = false) const;
    void walk_children(Shader& shader, const Matrix4& parentTransform, bool picking = false) const;

    /** Returns true if a given ray intersects this node. It is assumed that the ray has already been transformed
      * so that it is in the parent node's coordinate system. **/
    virtual bool raycast(const Point3D& point, const Vector3& direction) const;
    void raycastAll(const Point3D& point, const Vector3& direction) const;

    void add_child(SceneNode* child) {
        m_children.push_back(child);
    }

    void remove_child(SceneNode* child) {
        m_children.remove(child);
    }

    SceneNode* getById(int id);

    // Callbacks to be implemented.
    // These will be called from Lua.
    void rotate(char axis, double angle);
    void scale(const Vector3& amount);
    void translate(const Vector3& amount);

    virtual NodeType getType() const;
};

class JointNode : public SceneNode {
public:
    JointNode(const std::string& name, int id);
    virtual ~JointNode();

    virtual void walk_gl(Shader& shader, const Matrix4& parentTransform, bool picking = false) const;

    virtual SceneNode::NodeType getType() const;

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
    GeometryNode(const std::string& name, int id, Primitive* primitive);
    virtual ~GeometryNode();

    virtual void walk_gl(Shader& shader, const Matrix4& parentTransform, bool picking = false) const;

    virtual bool raycast(const Point3D& point, const Vector3& direction) const;

    virtual SceneNode::NodeType getType() const;

    const Material& getMaterial() const;
    Material& getMaterial();
    void setMaterial(const Material& material);

protected:
    Primitive* m_primitive;
};

#endif
