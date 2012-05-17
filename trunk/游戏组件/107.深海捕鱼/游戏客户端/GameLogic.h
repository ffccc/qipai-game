#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

//////////////////////////////////////////////////////////////////////////

//数值掩码
#define	LOGIC_MASK_COLOR			0xF0								//花色掩码
#define	LOGIC_MASK_VALUE			0x0F								//数值掩码

//////////////////////////////////////////////////////////////////////////

//游戏逻辑
class CGameLogic
{
    //函数定义
public:
    //构造函数
    CGameLogic();
    //析构函数
    virtual ~CGameLogic();

public:
    float GetBulletRadius(int nCannonStyle);
    unsigned long GetFishNetColor(int nCannonStyle);
    float GetFishNetScale(int nCannonStyle);
    int GetFishBonus(int nFishStyle);
    int GetFishRadius(int nFishStyle);
    unsigned long GetFireInterval(int nBulletStyle);

	/// 鲨鱼碰撞检测
    bool CollisionShark(const Point &pos1, const Point &pos2, int nRadius1, int nRadius2);
	/// 子弹撞鱼检测
    bool CollisionFish(const Point &pos1, const Point &pos2, int nRadius1, int nRadius2);
	///  捕鱼检测
    bool Collision(const Point &pos1, const Point &pos2, int nRadius1, int nRadius2);
};

//////////////////////////////////////////////////////////////////////////

#endif