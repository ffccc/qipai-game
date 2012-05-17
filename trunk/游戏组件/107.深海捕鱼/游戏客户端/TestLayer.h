#ifndef __FISH_TEST_LAYER_H__
#define __FISH_TEST_LAYER_H__

//////////////////////////////////////////////////////////////////
#include "xlbe.h"

#include "widget/progress_widget.h"



/////////////////////////////////////////////////////////////////
class CTestLayer : public Layer
{
public:
    CTestLayer();
    virtual ~CTestLayer ();

    virtual void update(float dt);
    virtual void draw();
    virtual void enter();
    virtual void exit();

    bool FishEnd(Node *node, int tag);
private:
    Sprite *m_sprFish;

    int m_nIndex;
    bool m_bChange;

};

//////////////////////////////////////////////////////////////////////////
#endif