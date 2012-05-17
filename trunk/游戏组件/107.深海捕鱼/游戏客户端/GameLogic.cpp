
#include "utility/rect.h"
#include "GameLogic.h"
#include <math.h>

#include <fstream>

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
        time = 500;
    }
    else if (nBulletStyle == 1)
    {
        time = 350;
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

unsigned long CGameLogic::GetFishNetColor(int nCannonStyle)
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

float CGameLogic::GetFishNetScale(int nCannonStyle)
{
    float fScale = 0;

    if (nCannonStyle == 0)
    {
        fScale = 0.6;
    }
    else if (nCannonStyle == 1)
    {
        fScale = 0.73;
    }
    else if (nCannonStyle == 2)
    {
        fScale = 0.86;
    }
    else if (nCannonStyle == 3)
    {
        fScale = 1.0;
    }
    else
    {
        fScale = 1.15;
    }

    return fScale;
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

int CGameLogic::GetFishRadius(int nFishStyle)
{
    int nRadius;

    if (nFishStyle == 0)
    {
        nRadius = 14;//17;
    }
    else if (nFishStyle == 1)
    {
        nRadius = 22;//26;
    }
    else if (nFishStyle == 2)
    {
        nRadius = 32;//38;
    }
    else if (nFishStyle == 3)
    {
        nRadius = 25;//30;
    }
    else if (nFishStyle == 4)
    {
        nRadius = 25;//30;
    }
    else if (nFishStyle == 5)
    {
        nRadius = 30;//35;
    }
    else if (nFishStyle == 6)
    {
        nRadius = 25;//30;
    }
    else if (nFishStyle == 7)
    {
        nRadius = 50;//55;
    }
    else if (nFishStyle == 8)
    {
        nRadius = 45;//50;
    }
    else if (nFishStyle == 9)
    {
        nRadius = 95;//100;
    }
    else if (nFishStyle == 10)
    {
        nRadius = 100;
    }
   
    return nRadius;

}

/// 子弹撞鱼检测
bool CGameLogic::CollisionFish(const Point &pos1, const Point &pos2, int nRadius1, int nRadius2)
{
    Point ptDistance(pos2);

    ptDistance -= pos1;

    //if ((ptDistance.x_*ptDistance.x_ + ptDistance.y_*ptDistance.y_)<((nRadius1+nRadius2)*(nRadius1+nRadius2)))
    //{
    //    return true;
    //}
	
    /// 子弹进入鱼三分之一位置
    if ((ptDistance.x_*ptDistance.x_ + ptDistance.y_*ptDistance.y_)<pow(nRadius1/**2/3*/+nRadius2, 2))
    {
        return true;
    }

    return false;
}

///  捕鱼检测
bool CGameLogic::Collision(const Point &pos1, const Point &pos2, int nRadius1, int nRadius2)
{
	if (nRadius2 >= nRadius1)
	{
		if (pow(pos1.x_ - pos2.x_, 2) + pow(pos1.y_ - pos2.y_, 2) <= pow(nRadius2 + nRadius1, 2))
			return true;
	}

    return false;
}

/// 鲨鱼碰撞检测
bool CGameLogic::CollisionShark(const Point &pos1, const Point &pos2, int nRadius1, int nRadius2)
{
    Rect rc(pos1.x_+42-155, pos1.y_-55, 310,110);

    if (nRadius2 == 0)
    {

        if (rc.pt_in_rect(pos2))
        {
            return true;
        }
    }
    else
    {
        Rect rc2(pos2.x_-nRadius2, pos2.y_-nRadius2, nRadius2,nRadius2);
        if (rc.collision(rc2))
        {
            return true;
        }


    }

    return false;
}

//////////////////////////////////////////////////////////////////////////
