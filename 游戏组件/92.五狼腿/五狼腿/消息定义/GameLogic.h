#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

//////////////////////////////////////////////////////////////////////////

//排序类型
#define ST_ORDER					0
#define ST_VIEW						1
#define ST_BOMB						2

//扑克类型
#define CT_ERROR					0									//错误类型
#define CT_SINGLE					1									//单牌类型
#define CT_DOUBLE					2									//对子类型
#define CT_SINGLE_LINE				3									//单连类型
#define CT_DOUBLE_LINE				4									//对连类型
#define CT_BOMB						5									//炸弹类型
#define CT_DOUBLE_BOMB				6									//双炸类型
#define CT_CRURA_FAKE				7									//假腿类型
#define CT_KING_LITTET				8									//小王类型
#define CT_KING_BIG					9									//大王类型
#define CT_CRURA_PURE				10									//纯腿类型

//宏定义
#define MAX_COUNT					27									//最大数目

//数值掩码
#define	LOGIC_MASK_COLOR			0xF0								//花色掩码
#define	LOGIC_MASK_VALUE			0x0F								//数值掩码

//分析结构
struct tagAnalyseResult
{
	BYTE 							bFourCount;							//四张数目
	BYTE 							bThreeCount;						//三张数目
	BYTE 							bDoubleCount;						//两张数目
	BYTE							bSignedCount;						//单张数目
	BYTE 							bFourLogicVolue[6];					//四张列表
	BYTE 							bThreeLogicVolue[9];				//三张列表
	BYTE 							bDoubleLogicVolue[13];				//两张列表
};

//出牌结果
struct tagOutCardResult
{
	BYTE							cbCardCount;						//扑克数目
	BYTE							cbResultCard[MAX_COUNT];			//结果扑克
};

//分布信息
struct tagDistributing
{
	BYTE							cbCardCount;						//扑克数目
	BYTE							cbDistributing[15][6];				//分布信息
};


//////////////////////////////////////////////////////////////////////////

//游戏逻辑类
class CGameLogic
{
	//变量定义
protected:
	static const BYTE				m_bCardListData[108];				//扑克数据


	//类型函数
public:
	//获取类型
	static BYTE GetCardType(BYTE bCardData[], BYTE bCardCount, bool bDoubleBomo);
	//获取数值
	static BYTE GetCardValue(BYTE bCardData) { return bCardData&LOGIC_MASK_VALUE; }
	//获取花色
	static BYTE GetCardColor(BYTE bCardData) { return bCardData&LOGIC_MASK_COLOR; }

	//控制函数
public:
	//排列扑克
	static void SortCardList(BYTE bCardData[], BYTE bCardCount, BYTE cbSortType);
	//混乱扑克
	static void RandCardList(BYTE bCardBuffer[], BYTE bBufferCount);
	//删除扑克
	static bool RemoveCard(const BYTE bRemoveCard[], BYTE bRemoveCount, BYTE bCardData[], BYTE bCardCount);

	//逻辑函数
public:
	//有效判断
	static bool IsValidCard(BYTE cbCardData);
	//逻辑数值
	static BYTE GetCardLogicValue(BYTE cbCardData);
	//构造扑克
	static BYTE MakeCardData(BYTE cbValueIndex, BYTE cbColorIndex);
	//获取分布
	static void GetDistributingInfo(BYTE cbCardData[], BYTE cbCardCount, tagDistributing & Distributing);

	//功能函数
public:
	//对比扑克
	static bool CompareCard(BYTE bFirstList[], BYTE bNextList[], BYTE bFirstCount, BYTE bNextCount);
	//分析扑克
	static void AnalysebCardData(const BYTE bCardData[], BYTE bCardCount, tagAnalyseResult & AnalyseResult);
	//出牌判断
	static bool SearchOutCard(BYTE bCardData[], BYTE bCardCount, BYTE bTurnCardData[], BYTE bTurnCardCount, BYTE bTurnOutType, tagOutCardResult & OutCardResult);
	//得到炸弹和腿
private:
	static void _GetPrivateCard(BYTE bCards[], BYTE& bCardCount, BYTE bPrivateCards[], BYTE& bPrivateCount);
	static void	_DeleteCards(BYTE bCards[], BYTE& bCardCount, BYTE bDelete[], BYTE bDeleteCount);
	static void	_AddCards(BYTE bCards[], BYTE& bCardCount, BYTE bAdd[], BYTE bAddCount);
};

//////////////////////////////////////////////////////////////////////////

#endif