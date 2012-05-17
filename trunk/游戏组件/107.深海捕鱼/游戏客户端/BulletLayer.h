#ifndef __FISH_BULLET_LAYER_H__
#define __FISH_BULLET_LAYER_H__

//////////////////////////////////////////////////////////////////
#include "xlbe.h"

#include "..\消息定义\CMD_Fish.h"

#include "GameCore.h"
#include "ObjectExtend.h"
/////////////////////////////////////////////////////////////////
class CBulletLayer : public Layer
{
public:
    CBulletLayer();
    virtual ~CBulletLayer ();

    virtual void update(float dt);
    virtual void draw();

public:
    bool BulletFire(const Point &ptStart, float rotation, WORD wChairID, CGameCore::enCannonType CannonType);

private:
    std::vector<Node *> m_NodeDelete;
};

//////////////////////////////////////////////////////////////////////////
#endif