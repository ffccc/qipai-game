#include "StdAfx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

//构造函数
CGameLogic::CGameLogic()
{
}

//析构函数
CGameLogic::~CGameLogic()
{
}

float CGameLogic::GetCaptureProbability(int nFishNetStyle, int nFishStyle, int nBulletStyle)
{
    float fFishNet = 60*((rand()%100)/100.0)*(0.95+nFishNetStyle/100.0);
    float fFish = 100*((rand()%100)/100.0)*(1.0-nFishStyle/100.0);
    float fBullet = 40*((rand()%100)/100.0)*(0.95+nBulletStyle/100.0);

    return (fFishNet+fFish+fBullet);
}

//////////////////////////////////////////////////////////////////////////
