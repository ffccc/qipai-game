
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

unsigned long CGameLogic::GetFireInterval(int nBulletStyle)
{
    unsigned long time = 0;

    if (nBulletStyle == 0)
    {
        time = 600;
    }
    else if (nBulletStyle == 1)
    {
        time = 400;
    }
    else
    {
        time = 200;
    }

    return time;
}

float CGameLogic::GetBulletRadius(int nCannonStyle)
{
    float fRadius = 200.0;
    return fRadius+(nCannonStyle+1)*80;
}

float CGameLogic::GetFishNetColor(int nCannonStyle)
{
    unsigned long color = 0;

    if (nCannonStyle == 0)
    {
        color = 0xFFECFF2D;
    }
    else if (nCannonStyle == 1)
    {
        color = 0xff24A754;
    }
    else if (nCannonStyle == 2)
    {
        color = 0xff15269D;
    }
    else if (nCannonStyle == 3)
    {
        color = 0xff6B2FBB;
    }
    else
    {
        color = 0xffB31F12;
    }

    return color;
}


int CGameLogic::GetFishBonus(int nFishStyle)
{
    int nBonus;

    if (nFishStyle == 0)
    {
        nBonus = 1;
    }
    else if (nFishStyle == 1)
    {
        nBonus = 2;
    }
    else if (nFishStyle == 2)
    {
        nBonus = 4;
    }
    else if (nFishStyle == 3)
    {
        nBonus = 7;
    }
    else if (nFishStyle == 4)
    {
        nBonus = 10;
    }
    else if (nFishStyle == 5)
    {
        nBonus = 20;
    }
    else if (nFishStyle == 6)
    {
        nBonus = 30;
    }
    else if (nFishStyle == 7)
    {
        nBonus = 40;
    }
    else if (nFishStyle == 8)
    {
        nBonus = 50;
    }
    else if (nFishStyle == 9)
    {
        nBonus = 60;
    }
    else if (nFishStyle == 10)
    {
        nBonus = 100;
    }
   

    return nBonus;

}
//////////////////////////////////////////////////////////////////////////
