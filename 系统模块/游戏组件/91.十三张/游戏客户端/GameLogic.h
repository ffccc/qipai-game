#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once
//////////////////////////////////////////////////////////////////////////

//扑克类型

#define CT_INVALID						0								//错误类型
#define CT_SINGLE						1								//单牌类型
#define CT_ONE_DOUBLE					2								//只有一对
#define CT_FIVE_TWO_DOUBLE				3								//两对牌型
#define CT_THREE						4								//三张牌型
#define CT_FIVE_MIXED_FLUSH_FIRST_A		5								//A在前顺子
#define CT_FIVE_MIXED_FLUSH_NO_A		6								//没A杂顺
#define CT_FIVE_MIXED_FLUSH_BACK_A		7								//A在后顺子
#define CT_FIVE_FLUSH					8								//同花五牌
#define CT_FIVE_THREE_DEOUBLE			9								//三条一对
#define CT_FIVE_FOUR_ONE				10								//四带一张
#define CT_FIVE_STRAIGHT_FLUSH_FIRST_A	11								//A在前同花顺
#define CT_FIVE_STRAIGHT_FLUSH			12								//同花顺牌

//数值掩码
#define	LOGIC_MASK_COLOR				0xF0							//花色掩码
#define	LOGIC_MASK_VALUE				0x0F							//数值掩码

//分析结构
struct tagAnalyseData
{
	BYTE							bOneCount ;								//单张数目
	BYTE							bTwoCount ;								//两张数目 
	BYTE							bThreeCount ;							//三张数目
	BYTE							bFourCount ;							//四张数目
	BYTE							bFiveCount ;							//五张数目
	BYTE							bOneFirst[13];							//单牌位置
	BYTE							bTwoFirst[13];							//对牌位置
	BYTE							bThreeFirst[13];						//三条位置
	BYTE							bFourFirst[13];							//四张位置
	bool							bStraight;								//是否顺子
};

//分析结构
struct tagAnalyseResult
{
	BYTE 							cbFourCount;						//四张数目
	BYTE 							cbThreeCount;						//三张数目
	BYTE 							cbDoubleCount;						//两张数目
	BYTE							cbSignedCount;						//单张数目
	BYTE							cbFourCardData[13];					//四张扑克
	BYTE							cbThreeCardData[13];				//三张扑克
	BYTE							cbDoubleCardData[13];				//两张扑克
	BYTE							cbSignedCardData[13];				//单张扑克
};
//////////////////////////////////////////////////////////////////////////
//排列类型
enum enSortCardType
{
	enDescend ,																//降序类型 
	enAscend ,																//升序类型
	enColor																	//花色类型
};


//游戏逻辑类
class CGameLogic
{
	//变量定义
protected:
	static const BYTE				m_bCardListData[52];				//扑克数据

	//函数定义
public:
	//构造函数
	CGameLogic();
	//析构函数
	virtual ~CGameLogic();

	//类型函数
public:
	//获取类型
	BYTE GetCardType(BYTE bCardData[], BYTE bCardCount);
	//获取数值
	BYTE GetCardValue(BYTE bCardData) { return bCardData&LOGIC_MASK_VALUE; } //十六进制前面四位表示牌的数值
	//获取花色
	BYTE GetCardColor(BYTE bCardData) { return (bCardData&LOGIC_MASK_COLOR)>>4; } //十六进制后面四位表示牌的花色 

	//控制函数
public:
	//排列扑克
	void SortCardList(BYTE bCardData[], BYTE bCardCount,enSortCardType SortCardType=enDescend);
	//混乱扑克
	void RandCardList(BYTE bCardBuffer[], BYTE bBufferCount);
	//删除扑克
	bool RemoveCard(const BYTE bRemoveCard[], BYTE bRemoveCount, BYTE bCardData[], BYTE bCardCount);

	//逻辑函数
public:
	//逻辑数值
	BYTE GetCardLogicValue(BYTE bCardData);
	//对比扑克
	bool CompareCard(BYTE bInFirstList[], BYTE bInNextList[], BYTE bFirstCount, BYTE bNextCount , bool bComperWithOther);

	//内部函数

	//分析扑克
	void AnalyseCard(const BYTE bCardDataList[] , const BYTE bCardCount , tagAnalyseData& AnalyseData) ;
	//分析扑克
	void AnalysebCardData(const BYTE cbCardData[], BYTE cbCardCount, tagAnalyseResult & AnalyseResult) ;
};

//////////////////////////////////////////////////////////////////////////

#endif
