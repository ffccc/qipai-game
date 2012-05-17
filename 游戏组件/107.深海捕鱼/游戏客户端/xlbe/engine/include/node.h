#ifndef __XLBE_NODE_H__
#define __XLBE_NODE_H__

#include "prereqs.h"

#include "size.h"
#include "point.h"
#include "rect.h"

#include "color_rect.h"

#include "action.h"
#include "render_primitive.h"

///////////////////////////////////////////////////////////
//namespace xlbe{

class Node;

///////////////////////////////////////////////////////////
class Xlbe_Export Node_Extend
{
public:
    Node_Extend():group_(-1), mask_(0), node_(0) {}
    virtual ~Node_Extend() {}

public:
    virtual int group() const { return group_; }
    virtual void set_group(int group) { group_ = group; }

    virtual int mask() const { return mask_; }
    virtual void set_mask(int mask) { mask_ = mask; }

    virtual Node *node() { return node_; }
    virtual void set_node(Node *node) { node_ = node; }

private:
    int group_;
    int mask_;
    Node *node_;
};

///////////////////////////////////////////////////////////
class Xlbe_Export Node
{	
public: 
    typedef std::vector<Node *> Nodes;
    typedef std::vector<Action *> Actions;

public:
    Node();
    virtual ~Node();

public:
    int tag() const { return tag_; }
    void set_tag(int tag) { tag_ = tag; }

    bool visible() const { return visible_; }
    void set_visible(bool visible) { visible_ = visible; }

    bool disable() const { return disable_; }
    void set_disable(bool disable) { disable_ = disable; }

    bool running() const { return running_; }
    void set_running(bool running) { running_ = running; }

    int zorder() const { return zorder_; }
    void set_zorder(int z)  { zorder_ = z; }

    float rotation() const { return rotation_; }
    void set_rotation(float rotation) { rotation_ = rotation; }

    Size scale() const { return scale_; }
    void set_scale(const Size &scale)  { scale_ = scale; }

    bool flip_x() const { return flip_x_; }
    void set_flip_x(bool flip) { flip_x_ = flip; }

    bool flip_y() const { return flip_y_; }
    void set_flip_y(bool flip) { flip_y_ = flip; }

    Point hot() const { return hot_; }
    void set_hot(const Point &hot)  { hot_ = hot; } 

    Color_Rect color() const { return color_; }
    void set_color(const Color_Rect &color) { color_ = color;}

    Render_Blend blend() const { return blend_; }
    void set_blend(Render_Blend blend) { blend_ = blend;}

    Point position() const { return position_; }
    void set_position(const Point &position) { position_ = position; }

    Size content_size() const { return content_size_; }
    void set_content_size(const Size &size)  { content_size_ = size;  }

    Point src_position() const { return src_position_; }
    void set_src_position(const Point &position) { src_position_ = position; }

    Size src_size() const { return src_size_; }
    void set_src_size(const Size &size)  { src_size_ = size;  }

    Rect boundbox() ;
    Point position_absolute();

public:
    Node* parent() { return parent_; }
    void set_parent(Node *parent) { parent_ = parent; }

    bool has_child() const { return childs_.size()>0; }
    int number_of_child() const { return childs_.size(); }

    void add_child(Node *child);
    void add_child(Node *child, int z);
    void add_child(Node *child, int z, int tag);

    void remove_child(Node *child);
    Node *remove_child_by_tag(int tag);
    void remove_all_child();
    void remove_from_parent();

    Nodes::iterator child_begin()  { return childs_.begin(); }
    Nodes::iterator child_end()    { return childs_.end(); }

    Node *child_by_tag(int tag);
    void reorder_child(Node *child, int z);

public:
    int number_of_running_action();

    Action *action_by_tag(int tag);
    void run_action(Action *action, bool immediate=true);
    void stop_action(Action* action);
    void stop_action_by_tag(int tag);
    void stop_all_action();

    Node_Extend *node_extend() { return extend_; }
    void set_node_extend(Node_Extend *extend) { extend_ = extend; extend_->set_node(this); }

public:
    virtual void enter();
    virtual void exit();
    virtual void cleanup();

    virtual void update(float dt);
    virtual void draw();

protected:
    void insert_child(Node *node, int index);
    void delete_child(Node *node);

protected:
    int tag_;

    bool visible_; 
    bool disable_;
    bool running_;
    int zorder_;

    float rotation_;	
    Size scale_;
    Point hot_; 
    Point position_;
    bool flip_x_, flip_y_;
    Size content_size_;
    Color_Rect color_;
    Render_Blend blend_;

    Point src_position_;
    Size src_size_;

    int update_count_;

    Node *parent_;
    Nodes childs_;
    Actions actions_;

    Node_Extend *extend_;


};

/////////////////////////////////////////////////////////
//}
#endif