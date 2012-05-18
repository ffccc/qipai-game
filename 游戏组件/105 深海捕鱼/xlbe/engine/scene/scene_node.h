#ifndef __XLBE_SCENE_NODE_H__
#define __XLBE_SCENE_NODE_H__

#include <string>
#include <vector>

#include "xlbe.h"

#include "utility/size.h"
#include "utility/point.h"
#include "utility/rect.h"

#include "action/action.h"

///////////////////////////////////////////////////////////
//namespace xlbe{

///////////////////////////////////////////////////////////
class Xlbe_Export Scene_Node //: private std::vector<Scene_Node *>
{	
public: 
    typedef std::vector<Scene_Node *> Scene_Nodes;

public:
    Scene_Node();
    virtual ~Scene_Node();

public:
    int tag() const               { return tag_; }
    void set_tag(int tag)         { tag_ = tag; }

    bool visible() const           { return visible_; }
    void set_visible(bool visible) { visible_ = visible; }

    float rotation() const              { return rotation_; }
    void set_rotation(float rotation)   { rotation_ = rotation; transform_=true; }

    float scalex() const              { return scalex_; }
    void set_scalex(float scalex)     { scalex_ = scalex;  transform_=true; }

    float scaley() const              { return scaley_; }
    void set_scaley(float scaley)     { scaley_ = scaley;  transform_=true; }

    float hotx() const                { return hotx_; }
    void set_hotx(float hotx)         { hotx_ = hotx;  transform_=true; } 

    float hoty() const                { return hoty_; }
    void set_hoty(float hoty)         { hoty_ = hoty;  transform_=true; } 

    Point position() const            { return position_; }
    void set_position(const Point &position) { position_ = position;  transform_=true; }

    Size content_size() const                { return content_size_; }
    void set_content_size(const Size &size)  { content_size_ = size; transform_=true; }

    int zorder() const     { return zorder_; }
    void set_zorder(int z)  { zorder_ = z; }

    Scene_Node* parent() 	                  { return parent_; }
    void set_parent(Scene_Node *parent)     { parent_ = parent; }

public:
    bool has_child() const { return childs_.size()>0; }

    Rect boundbox();
    Point position_absolute();

    int number_of_child() const { return childs_.size(); }

    void add_child(Scene_Node *child);
    void add_child(Scene_Node *child, int z);
    void add_child(Scene_Node *child, int z, int tag);

    void remove_child(Scene_Node *child);
    Scene_Node *remove_child_by_tag(int tag);
    void remove_all_child();
    void remove_from_parent();

    Scene_Nodes::iterator child_begin()  { return childs_.begin(); }
    Scene_Nodes::iterator child_end()    { return childs_.end(); }

    Scene_Node *child_by_tag(int tag);
    void reorder_child(Scene_Node *child, int z);

    int number_of_running_action();

    Action *action_by_tag(int tag);
    void run_action(Action *action);
    void stop_action(Action* action);
    void stop_action_by_tag(int tag);
    void stop_all_action();

public:
    virtual void enter();
    virtual void exit();

    virtual void draw();
    virtual void visit(float dt);

    virtual void update(float dt);
    virtual void transform();

    virtual void cleanup();

protected:
    void insert_child(Scene_Node *node, int index);
    void delete_child(Scene_Node *node);

protected:
    int tag_;

    bool visible_; 
    bool running_;

    int zorder_;

    float rotation_;	
    float scalex_, scaley_;
    float hotx_, hoty_; 
    Point position_;
    Size content_size_;

    bool transform_;  
    Scene_Node *parent_;
    Scene_Nodes childs_;
};

/////////////////////////////////////////////////////////
//}
#endif