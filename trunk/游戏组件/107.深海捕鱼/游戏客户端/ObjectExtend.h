#ifndef __FISH_OBJECT_EXTEND_H__
#define __FISH_OBJECT_EXTEND_H__

//////////////////////////////////////////////////////////////////
#include "xlbe.h"

#include "..\消息定义\CMD_Fish.h"

#include "GameCore.h"

/////////////////////////////////////////////////////////////////
class CFishObjectExtend : public Node_Extend
{
public:
    CFishObjectExtend();
    virtual ~CFishObjectExtend ();

    int GetFishGoldByStyle();
    Size GetFishObjectSize();

    static int GetFishGoldByStyle(CGameCore::enFishType FishType);
    static bool ComputeCollision( float w, float h, float r, float rx, float ry);

public:
    WORD wID;
    WORD wRoundID;
    CGameCore::enFishType FishType; 
    DWORD dwTime;

};

class CBulletObjectExtend : public Node_Extend
{
public:
    CBulletObjectExtend();
    virtual ~CBulletObjectExtend ();

public:
    WORD wChairID;
    CGameCore::enCannonType CannonType;
};

class CNetObjectExtend : public Node_Extend
{
public:
    CNetObjectExtend();
    virtual ~CNetObjectExtend ();

    int GetNetRadius();

public:
    WORD wChairID;
    CGameCore::enCannonType CannonType;
};


//////////////////////////////////////////////////////////////////////////
#endif