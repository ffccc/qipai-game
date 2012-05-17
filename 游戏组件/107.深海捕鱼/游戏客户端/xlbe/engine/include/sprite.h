#ifndef __XLBE_SPRITE_H__
#define __XLBE_SPRITE_H__

#include <map>

#include "prereqs.h"

#include "render_system.h"

#include "animation.h"
#include "imageset.h"
#include "node.h"

/////////////////////////////////////////////////////////
//namespace xlbe {

/////////////////////////////////////////////////////////
class Xlbe_Export Sprite : public Node
{
public:
    Sprite();
    Sprite(Image *image);
    virtual ~Sprite();

public:
    virtual void draw(); 
    virtual void update(float dt);

public:
    void set_display_image(Image *image);
    void set_display_image(Animation *animation, int index); 

    bool is_image_displayed(Image *image);
    Image *displayed_image();

private:
    Image *image_;
};

/////////////////////////////////////////////////////////
//}
#endif