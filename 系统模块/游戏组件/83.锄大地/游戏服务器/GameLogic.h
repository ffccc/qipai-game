#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////

//扑克类型
#define CT_ERROR					0									//错误类型
#define CT_SINGLE					1									//单牌类型
#define CT_DOUBLE					2									//对子类型
#define CT_THREE_TIAO				3									//三条类型
#define	CT_FOUR_TIAO				4									//四条类型
#define	CT_SHUN_ZI					5									//顺子类型
#define CT_TONG_HUA					6									//同花类型
#define CT_HU_LU					7									//葫芦类型
#define CT_TIE_ZHI					8									//铁支类型
#define CT_TONG_HUA_SHUN			9									//同花顺型

//宏定义
#define MAX_COUNT					13									//最大数目
#define FULL_COUNT					52									//所有牌
#define BIG_COUNT					24									//

//数值掩码
#define	LOGIC_MASK_COLOR			0xF0								//花色掩码
#define	LOGIC_MASK_VALUE			0x0F								//数值掩码


//排序类型
#define ST_ORDER					0									//大小排序
#define ST_COLOR					1									//花色排序

//////////////////////////////////////////////////////////////////////////

//分析结构
struct tagAnalyseResult
{
	BYTE 							cbCardCount[4];						//扑克数目
	BYTE							cbCardData[4][MAX_COUNT];			//扑克数据
};

//出牌结果
struct tagOutCardResult
{
	BYTE							cbCardCount;						//扑克数目
	BYTE							cbResultCard[5];					//结果扑克
};

//分布信息
struct tagDistributing
{
	BYTE							cbCardCount;						//扑克数目
	BYTE							cbDistributing[15][6];				//分布信息
};

//分析牌
struct tagAnalysedCard
{
	BYTE							bCardCount[9];						//各种类型牌数目
	BYTE							bCardData[9][MAX_COUNT];			//各种类型牌数据
};

struct tagOutCardType
{
	bool							bPower;								//是否可出完该类型并收回出牌权
	bool							bOutAll;							//是否出得完
	BYTE							bRemainCount;						//剩余数目
	BYTE							bCardCount;							//
	BYTE							bCardData[5];						//
};

//////////////////////////////////////////////////////////////////////////

//游戏逻辑类
class CGameLogic
{
	//变量定义
public:
	static const BYTE				m_cbCardData[FULL_COUNT];			//扑克数据
	static const BYTE				m_cbBigCardData[BIG_COUNT];			//

	//函数定义
public:
	//构造函数
	CGameLogic();
	//析构函数
	virtual ~CGameLogic();


	//属性函数
public:
	//获取数值
	BYTE GetCardValue(BYTE cbCardData) { return cbCardData&LOGIC_MASK_VALUE; }
	//获取花色
	BYTE GetCardColor(BYTE cbCardData) { return cbCardData&LOGIC_MASK_COLOR; }

	//类型函数
public:
	//获取类型
	BYTE GetCardType(const BYTE cbCardData[], BYTE cbCardCount);
	//获取花色
	BYTE GetCardColor(const BYTE cbCardData[], BYTE cbCardCount);

	//控制函数
public:
	//混乱扑克
	void RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount);
	//排列扑克
	void SortCardList(BYTE cbCardData[], BYTE cbCardCount, BYTE cbSortType=ST_ORDER);
	//删除扑克
	bool RemoveCard(const BYTE cbRemoveCard[], BYTE cbRemoveCount, BYTE cbCardData[], BYTE cbCardCount);
	//混乱扑克
	void RandCard(const BYTE bCardData[],BYTE bCardCount,BYTE bCardBuffer[],BYTE bBufferCount);

	//逻辑函数
public:
	//有效判断
	bool IsValidCard(BYTE cbCardData);
	//逻辑数值
	BYTE GetCardLogicValue(BYTE cbCardData);
	//对比扑克
	bool CompareCard(BYTE cbFirstCard, BYTE cbNextCard);
	//对比扑克
	bool CompareCard(const BYTE cbFirstCard[], const BYTE cbNextCard[], BYTE cbFirstCount, BYTE cbNextCount);
	//出牌搜索
	bool SearchOutCard(const BYTE cbHandCardData[], BYTE cbHandCardCount, const BYTE cbTurnCardData[], BYTE cbTurnCardCount, tagOutCardResult & OutCardResult);

	//搜索函数
protected:
	//搜索葫芦
	BYTE SearchHuLu(const tagDistributing & Distributing, BYTE cbReferCard, BYTE cbResultCard[MAX_COUNT]);
	//搜索铁支
	BYTE SearchTieZhi(const tagDistributing & Distributing, BYTE cbReferCard, BYTE cbResultCard[MAX_COUNT]);
	//搜索顺子
	BYTE SearchShunZi(const tagDistributing & Distributing, BYTE cbReferCard, BYTE cbResultCard[MAX_COUNT]);
	//搜索同花
	BYTE SearchTongHua(const tagDistributing & Distributing, BYTE cbReferCard, BYTE cbResultCard[MAX_COUNT]);
	//搜索同花顺
	BYTE SearchTongHuaShun(const tagDistributing & Distributing, BYTE cbReferCard, BYTE cbResultCard[MAX_COUNT]);

	//内部函数
private:
	//构造扑克
	BYTE MakeCardData(BYTE cbValueIndex, BYTE cbColorIndex);
	//是否连牌
	bool IsStructureLink(const BYTE cbCardData[], BYTE cbCardCount);
	//分析扑克
	void AnalysebCardData(const BYTE cbCardData[], BYTE cbCardCount, tagAnalyseResult & AnalyseResult);
	//分析分布
	void AnalysebDistributing(const BYTE cbCardData[], BYTE cbCardCount, tagDistributing & Distributing);

public:
	//搜索出牌
	bool StrusteeOutCard( const BYTE cbHandCardData[],BYTE cbHandCardCount,const BYTE cbTurnCardData[],BYTE cbTurnCardCount,
		bool bOutCard3,bool bOutMost,tagOutCardResult &OutCardResult );

public:
	//分析扑克
	void AnalyseCardData( const BYTE cbCardData[],BYTE cbCardCount,tagAnalysedCard & AnalysedCard );
	//出牌搜索
	bool SearchFromAnalysedCard( const tagAnalysedCard& AnalysedCard,WORD wChairId,const BYTE bTurnCardData[], BYTE bTurnCardCount,const BYTE bOutCardRecord[],BYTE bOutCardCount,
		const BYTE (&cbCardData)[GAME_PLAYER][13],const BYTE cbCardCount[],tagOutCardResult & OutCardResult,bool &bSplit );
	//首先出牌搜索
	bool SearchFirstOutCard( const tagAnalysedCard& AnalysedCard,tagOutCardResult &OutCardResult,bool &bSplit );
	//强制出最大牌搜索
	bool SearchForcedOutCard( const tagAnalysedCard& AnalysedCard,WORD wChairId,const BYTE bTurnCardData[],BYTE bTurnCardCount,const BYTE bOutCardRecord[],BYTE bOutCardCount,
		const BYTE (&cbCardData)[GAME_PLAYER][13],const BYTE cbCardCount[],tagOutCardResult& OutCardResult,bool &bReAnalyse );
	//抓人
	bool HuntUser( const tagAnalysedCard &AnalysedCard,BYTE bHandCardData[],BYTE bHandCardCount,BYTE bNextUserCardCount,tagOutCardResult &OutCardResult,bool &bSplit );
	//自动玩牌
	bool AutomatismPlay( tagAnalysedCard &AnalysedCard,WORD wChairId,WORD wOutCardUser,const BYTE bTurnCardData[],BYTE bTurnCardCount,const BYTE (&bCardData)[GAME_PLAYER][13],const BYTE bCardCount[],const BYTE bOutCardRecord[],BYTE bOutCardCount,tagOutCardResult &OutCardResult,bool bOutCard3=false );
	//获取最大牌
	BYTE GetMostCardValue( const tagAnalysedCard &AnalysedCard,const BYTE bOutCardRecord[],BYTE bOutCardCount );
	//判断是否能出完
	bool IsOutCardFinish( WORD wChairId,const tagAnalysedCard &AnalysedCard,const BYTE bOutCardRecord[],BYTE bOutCardCount,bool bTurn,const BYTE (&cbCardData)[GAME_PLAYER][13],const BYTE cbCardCount[],BYTE &bMostCount,BYTE &bLessCount );
	//分析牌型
	bool AnalyseOutCardType( const tagAnalysedCard &cAnalysedCard,WORD wMeChairId,BYTE bCardType,const BYTE cbTurnCardData[],BYTE cbTurnCardCount,
		const BYTE (&cbCardData)[GAME_PLAYER][13],const BYTE cbCardCount[],tagOutCardType &OutCardType );
	//搜索出牌
	bool SearchTurnCard(const BYTE cbHandCardData[], BYTE cbHandCardCount, const BYTE cbTurnCardData[], BYTE cbTurnCardCount,tagOutCardResult &OutCardResult);
	//判断是否最大
	bool IsTurnCardMost(WORD wChairId,const BYTE cbTurnCardData[],BYTE cbTurnCardCount,const BYTE (&cbCardData)[GAME_PLAYER][13],const BYTE cbCardCount[]);
};

//////////////////////////////////////////////////////////////////////////

#endif
