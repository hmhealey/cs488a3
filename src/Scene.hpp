#ifndef SCENE_HPP
#define SCENE_HPP

#include <list>

#include "Algebra.hpp"
#include "Material.hpp"

class JointNode;
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
    Matrix4 transform;

    // Hierarchy
    std::list<SceneNode*> m_children;

public:
    SceneNode(const std::string& name, int id);
    virtual ~SceneNode();

    std::string getName() const;
    int getId() const;

    bool isSelected() const;
    void setSelected(bool selected);

    virtual Matrix4 getTransform() const;
    void setTransform(const Matrix4& transform);

    virtual void walk_gl(Shader& shader, const Matrix4& parentTransform, bool picking = false) const;
    void walk_children(Shader& shader, const Matrix4& parentTransform, bool picking = false) const;

    void add_child(SceneNode* child) {
        m_children.push_back(child);
    }

    void remove_child(SceneNode* child) {
        m_children.remove(child);
    }

    SceneNode* getById(int id);

    bool pick(int id);

    const std::list<JointNode*> getSelectedJoints();
    virtual void resetJoints();

    // Callbacks to be implemented.
    // These will be called from Lua.
    void rotate(char axis, double angle);
    void scale(const Vector3& amount);
    void translate(const Vector3& amount);

    virtual NodeType getType() const;
};

class JointNode : public SceneNode {
protected:
    struct JointRange {
        double min, init, max;
    };

    JointRange xRange;
    JointRange yRange;

    double xRotation = 0;
    double yRotation = 0;

public:
    JointNode(const std::string& name, int id);
    virtual ~JointNode();

    virtual Matrix4 getTransform() const;

    virtual void walk_gl(Shader& shader, const Matrix4& parentTransform, bool picking = false) const;

    virtual void resetJoints();

    virtual SceneNode::NodeType getType() const;

    double getXRotation() const;
    double setXRotation(double xRotation);

    double getYRotation() const;
    double setYRotation(double yRotation);

    void setXRange(double min, double init, double max);
    void setYRange(double min, double init, double max);
};

class GeometryNode : public SceneNode {
public:
    GeometryNode(const std::string& name, int id, Primitive* primitive);
    virtual ~GeometryNode();

    virtual void walk_gl(Shader& shader, const Matrix4& parentTransform, bool picking = false) const;

    virtual SceneNode::NodeType getType() const;

    const Material& getMaterial() const;
    void setMaterial(const Material& material);

protected:
    Primitive* m_primitive;
};

#endif
