#ifndef __FISH_CURSOR_LAYER_H__
#define __FISH_CURSOR_LAYER_H__

//////////////////////////////////////////////////////////////////
#include "xlbe.h"

#include "widget/progress_widget.h"


/////////////////////////////////////////////////////////////////
class CCursorLayer : public Layer
{
public:
    CCursorLayer();
    virtual ~CCursorLayer ();

    virtual void update(float dt);
    virtual void draw();
    virtual void enter();
    virtual void exit();

    void SetCursor(Image *cursor);

private:
    Sprite *m_sprCursor;

};

//////////////////////////////////////////////////////////////////////////
#endif