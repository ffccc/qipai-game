#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////

//数值掩码
#define	LOGIC_MASK_COLOR			0xF0								//花色掩码
#define	LOGIC_MASK_VALUE			0x0F								//数值掩码

//定义牌的类型
#define CARDTYPE_ERROR						0									//错误牌型
#define	CARDTYPE_SAN_ZHANG					1									//三张
#define	CARDTYPE_SAN_GONG					2									//三公
#define	CARDTYPE_SAN_PAI					3									//散牌
#define	CARDTYPE_BI_SHI						4									//毙十

//////////////////////////////////////////////////////////////////////////

//游戏逻辑
class CGameLogic
{
	//变量定义
private:
	static const BYTE				m_cbCardListData[54];				//扑克定义

	//函数定义
public:
	//构造函数
	CGameLogic();
	//析构函数
	virtual ~CGameLogic();

	//类型函数
public:
	//获取数值
	BYTE GetCardValue(BYTE cbCardData) { return cbCardData&LOGIC_MASK_VALUE; }
	//获取花色
	BYTE GetCardColor(BYTE cbCardData) { return (cbCardData&LOGIC_MASK_COLOR)>>4 ; }

	//控制函数
public:
	//混乱扑克
	void RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount);
	//有效判断
	bool IsValidCard(BYTE cbCardData);
	//排序
	void SortCardList(BYTE cbCardBuffer[], BYTE cbBufferCount);
	//获取单张牌的点数
	BYTE GetCardPoint(BYTE cbCardData);

	//逻辑函数
public:
	//获取牌点
	BYTE GetCardPip(BYTE cbCardData);
	//获取牌点
	BYTE GetCardListPip(const BYTE cbCardData[], BYTE cbCardCount);

	//是否是三公
	bool IsSanGong(const BYTE cbCardBuffer[], const BYTE cbBufferCount);
	//是否三张
	bool IsSanZhang(const BYTE cbCardBuffer[], const BYTE cbBufferCount);
	//是否是毙十
	bool IsBiShi(const BYTE cbCardBuffer[], const BYTE cbBufferCount);
	//是否散牌
	bool IsSanPai(const BYTE cbCardBuffer[], const BYTE cbBufferCount);
	//获取牌的类型
	BYTE GetCardType(const BYTE cbCardBuffer[], const BYTE cbBufferCount);
	//获取点数
	BYTE GetPoint(const BYTE cbCardBuffer[], const BYTE cbBufferCount);
	//比较牌大小, 如果First 不大于 Next  
	bool IsBigger(const BYTE cbFirstCard[], const BYTE cbNextCard[], const BYTE cbFirstCount, const BYTE cbNextCount);
	//获取番的倍数
	BYTE GetMultiple(const BYTE cbCardBuffer[], const BYTE cbBufferCount);
};

//////////////////////////////////////////////////////////////////////////

#endif
