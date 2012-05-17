#ifndef __FISH_TASK_LAYER_H__
#define __FISH_TASK_LAYER_H__

//////////////////////////////////////////////////////////////////
#include "xlbe.h"

/////////////////////////////////////////////////////////////////
class CTaskLayer : public Layer
{
public:
    CTaskLayer();
    virtual ~CTaskLayer ();

    virtual void update(float dt);
    virtual void draw();
    virtual void enter();
    virtual void exit();

public:
    bool CommandTaskStart(WORD wChairID);

public:
    bool BarEnoughEnd(Node *node, int tag);

private:
    Sprite *m_sprBarEnough;
};

//////////////////////////////////////////////////////////////////////////
#endif