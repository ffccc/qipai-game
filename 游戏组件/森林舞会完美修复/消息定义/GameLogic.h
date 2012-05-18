#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

using namespace std;

#pragma once

//#include "..\..\..\服务器组件\游戏服务\GameServiceExport.h"

//////////////////////////////////////////////////////////////////////////

//下注位置定义；地、天、和、地1->地9、天1->天9，共21个下注位置
#define INDEX_PLAYER				0									//闲家索引
#define INDEX_BANKER				1									//庄家索引
#define INDEX_DRAW					2									//和索引
#define INDEX_PLAYER_MIN		    3									//闲最小索引
#define INDEX_PLAYER_MAX		   10									//闲最大索引
#define INDEX_BANKER_MIN		   10									//庄最小索引
#define INDEX_BANKER_MAX		   10									//庄最大索引
#define INDEX_MAX				   15									//最大索引


#define HISTORY_COUNT				30									//历史记录局数

//数值掩码
#define	LOGIC_MASK_COLOR			0xF0								//花色掩码
#define	LOGIC_MASK_VALUE			0x0F								//数值掩码

//#define MAX_SCORE					100000000							//最大金币
#define MAX_SCORE					((INT64)pow(2.0,31.0)-1)					//最大金币

#define NO_WINNER					0									//没有赢家时的取值

const int LOSS_RATES[] ={19,19,19,19,4,4,4,4};


//游戏逻辑
class CGameLogic
{
	//变量定义
private:
	static const BYTE				m_cbCardListData[52*8];				//扑克定义

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
	BYTE GetCardColor(BYTE cbCardData) { return cbCardData&LOGIC_MASK_COLOR; }

	//控制函数
public:
	//混乱扑克
	void RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount);

	//逻辑函数
public:
	//获取牌点
	BYTE GetCardPip(BYTE cbCardData);
	//获取牌点
	BYTE GetCardListPip(const BYTE cbCardData[], BYTE cbCardCount);
	//根据牌局取每个下注区域的赔率
	void GetOdds(BYTE cbPlayerPip, BYTE cbBankerPip, INT nOdds[],BYTE playCardArray[] ,BYTE bankCardArray[],int playcount ,int bankcount);

	//根据赔率，下注额计算输赢金额
	void GetWinAcount(INT nOdds[], INT64 pUserStakes[], LONG lRakeRate, long& lWinAmount, long& lRakeOff);

	//开奖结果，下注额计算庄家输赢金额
	LONG GetWinAcountBanker(BYTE cbPlayerValue, BYTE cbBankerValue, LONG pUserStakes[], LONG lRakeRate);

	//虚拟计算庄家最大赔付
	LONG GetBankerMaxLoss(BYTE cbBetArea, LONG lBetSum[], LONG lRakeRate);

	//计算每个区域的最大下注额
	void GetMaxStakes(LONG lMaxStakes[], LONG lBetSum[], LONG lBankerMaxLoss, LONG lRakeRate);

	//虚拟计算，判断是否超过庄家最大赔付
	bool BankerIsOver(BYTE cbBetArea, LONG lBetAmount, LONG lBetSumOld[], LONG lBankerMaxLoss, LONG lRakeRate);
    bool BankerIsEnd(BYTE cbBetArea, LONG lBetAmount, INT64 lBetSumOld[], INT64 lBankerMaxLoss, INT64 lBankersore);
	//取多个数中的最小值
	INT64 Min(INT64 iFirst, ...);

};

//////////////////////////////////////////////////////////////////////////

#endif