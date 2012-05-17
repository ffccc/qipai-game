#ifndef __FISH_LOAD_SCENE_H__
#define __FISH_LOAD_SCENE_H__

//////////////////////////////////////////////////////////////////
#include "xlbe.h"

#include "LoadLayer.h"
#include "FrameLayer.h"
#include "CursorLayer.h"

/////////////////////////////////////////////////////////////////
class CLoadScene : public Scene
{
public:
    CLoadScene();
    virtual ~CLoadScene();

    virtual void update(float dt);
    virtual bool mouseMoved( const OIS::MouseEvent &arg ) ;
public:
    CLoadLayer *m_layLoad;
    Progress_Widget *m_widgetProgress;

    CFrameLayer *m_layFrame;
    CCursorLayer *m_layCursor;

};

//////////////////////////////////////////////////////////////////////////
#endif