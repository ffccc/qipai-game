#include "CursorLayer.h"

#include <sstream>

////////////////////////////////////////////////////////////
CCursorLayer::CCursorLayer()
{	
    m_sprCursor = new Sprite;
    add_child(m_sprCursor);
}

CCursorLayer::~CCursorLayer()
{
}

void CCursorLayer::SetCursor(Image *cursor)
{
    m_sprCursor->set_display_image(cursor);
}

void CCursorLayer::update(float dt)
{
    __super::update(dt);

}

void CCursorLayer::draw()
{
    __super::draw();

}

void CCursorLayer::enter()
{
      __super::enter();

}

void CCursorLayer::exit()
{
      __super::exit();
}

////////////////////////////////////////////////////////////