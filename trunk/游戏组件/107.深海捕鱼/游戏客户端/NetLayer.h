#ifndef __FISH_NET_LAYER_H__
#define __FISH_NET_LAYER_H__

//////////////////////////////////////////////////////////////////
#include "xlbe.h"

#include "..\消息定义\CMD_Fish.h"

#include "GameCore.h"
#include "ObjectExtend.h"
/////////////////////////////////////////////////////////////////
class CNetLayer : public Layer
{
public:
    CNetLayer();
    virtual ~CNetLayer ();

    virtual void update(float dt);
    virtual void draw();

public:
    bool NetFire(const Point &pt, WORD wChairID, CGameCore::enCannonType CannonType);
    bool NetEnd(Node *node, int tag);
};

//////////////////////////////////////////////////////////////////////////
#endif