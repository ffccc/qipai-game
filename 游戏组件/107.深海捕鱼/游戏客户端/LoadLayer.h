#ifndef __FISH_LOAD_LAYER_H__
#define __FISH_LOAD_LAYER_H__

//////////////////////////////////////////////////////////////////
#include "xlbe.h"

#include "widget/progress_widget.h"

/////////////////////////////////////////////////////////////////
class CLoadLayer : public Layer
{
public:
    CLoadLayer();
    virtual ~CLoadLayer ();

    virtual void update(float dt);
    virtual void draw();

private:
    Sprite *m_sprBg;
    Font *m_Font;

    std::string m_strDateLoading;
    std::string m_strHelp;
    std::vector<std::string> m_strHelps;
};

//////////////////////////////////////////////////////////////////////////
#endif