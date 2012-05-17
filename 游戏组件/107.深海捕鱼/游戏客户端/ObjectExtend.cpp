#include "ObjectExtend.h"

#include "GameCore.h"
#include "ClientKernel.h"
#include <cmath>
#include <algorithm>
////////////////////////////////////////////////////////////
CFishObjectExtend::CFishObjectExtend()
{	
    wID = INVALID_WORD;
    wRoundID = 0;
}

CFishObjectExtend::~CFishObjectExtend()
{
}


Size CFishObjectExtend::GetFishObjectSize()
{
    switch(FishType)
    {
    case CGameCore::FishType_0: return Size(14,36);
    case CGameCore::FishType_1: return Size(12,36);
    case CGameCore::FishType_2: return Size(20,70);
    case CGameCore::FishType_3: return Size(24,72);
    case CGameCore::FishType_4: return Size(30,80);
    case CGameCore::FishType_5: return Size(40,60);
    case CGameCore::FishType_6: return Size(20,160);
    case CGameCore::FishType_7: return Size(98,116);
    case CGameCore::FishType_8: return Size(70,180);
    case CGameCore::FishType_9: return Size(90,180);
   case CGameCore::FishType_10: return Size(77,287);
   case CGameCore::FishType_11: return Size(112,400);
   case CGameCore::FishType_12: return Size(112,400);
    default:  return Size(0,0);
    };
}

int CFishObjectExtend::GetFishGoldByStyle()
{
    return GetFishGoldByStyle(FishType);
}

int CFishObjectExtend::GetFishGoldByStyle(CGameCore::enFishType FishType)
{
    switch(FishType)
    {
    case CGameCore::FishType_0: return 1;
    case CGameCore::FishType_1: return 2;
    case CGameCore::FishType_2: return 3;
    case CGameCore::FishType_3: return 4;
    case CGameCore::FishType_4: return 5;
    case CGameCore::FishType_5: return 10;
    case CGameCore::FishType_6: return 20;
    case CGameCore::FishType_7: return 30;
    case CGameCore::FishType_8: return 40;
    case CGameCore::FishType_9: return 50;
    case CGameCore::FishType_10: return 70;
    case CGameCore::FishType_11: return 100;
    case CGameCore::FishType_12: return 0;
    default:  return 0;
    }
}

bool CFishObjectExtend::ComputeCollision( float w, float h, float r, float rx, float ry)
{

    float dx = std::min( rx, w*0.5f);
    dx = std::max( dx, -w*0.5f);

    float dy = std::min( ry, h*0.5f);
    dy = std::max( dy, -h*0.5f);

    return (rx - dx)*(rx - dx) + (ry - dy)*(ry - dy) <= r*r;
}


////////////////////////////////////////////////////////////
CBulletObjectExtend::CBulletObjectExtend()
{	
    wChairID = INVALID_WORD;
    CannonType =CGameCore::CannonTypeCount;
}

CBulletObjectExtend::~CBulletObjectExtend()
{
}

////////////////////////////////////////////////////////////
CNetObjectExtend::CNetObjectExtend()
{	
    wChairID = INVALID_WORD;
    CannonType =CGameCore::CannonTypeCount;
}

CNetObjectExtend::~CNetObjectExtend()
{
}

int CNetObjectExtend::GetNetRadius()
{
    switch(CannonType)
    {
    case CGameCore::CannonType_0: return 70*0.5;
    case CGameCore::CannonType_1: return 70*0.6;
    case CGameCore::CannonType_2: return 70*0.7;
    case CGameCore::CannonType_3: return 70*0.8;
    case CGameCore::CannonType_4: return 70*0.9;
    case CGameCore::CannonType_5: return 70*1.0;
    case CGameCore::CannonType_6: return 70*1.1;
    case CGameCore::CannonTypeCount: return 0;
    default:  return 0;
    }
}