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
    float GetFishNetColor(int nCannonStyle);
    int GetFishBonus(int nFishStyle);
    unsigned long GetFireInterval(int nBulletStyle);

};

//////////////////////////////////////////////////////////////////////////

#endif