#include "StdAfx.h"
#include "GameLogic.h"
#include "../消息定义/CMD_Baccarat.h"

//////////////////////////////////////////////////////////////////////////

//扑克数据
//const BYTE CGameLogic::m_cbCardListData[52*8]=
//{
	//0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,	//方块 A - K
	//0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	//梅花 A - K
	//0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	//红桃 A - K
	//0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,	//黑桃 A - K

	//0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,	//方块 A - K
	//0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	//梅花 A - K
	//0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	//红桃 A - K
	//0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,	//黑桃 A - K

	//0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,	//方块 A - K
	//0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	//梅花 A - K
	//0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	//红桃 A - K
	//0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,	//黑桃 A - K

	//0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,	//方块 A - K
	//0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	//梅花 A - K
	//0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	//红桃 A - K
	//0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,	//黑桃 A - K

	//0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,	//方块 A - K
	//0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	//梅花 A - K
	//0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	//红桃 A - K
	//0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,	//黑桃 A - K

	//0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,	//方块 A - K
	//0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	//梅花 A - K
	//0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	//红桃 A - K
	//0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,	//黑桃 A - K

	//0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,	//方块 A - K
	//0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	//梅花 A - K
	//0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	//红桃 A - K
	//0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,	//黑桃 A - K

	//0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,	//方块 A - K
	//0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	//梅花 A - K
	//0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	//红桃 A - K
	//0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,	//黑桃 A - K
//};

//////////////////////////////////////////////////////////////////////////

//构造函数
CGameLogic::CGameLogic()
{
}

//析构函数
CGameLogic::~CGameLogic()
{
}

//混乱扑克
void CGameLogic::RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount)
{
	//混乱准备
	//BYTE cbCardData[CountArray(m_cbCardListData)];
	//CopyMemory(cbCardData,m_cbCardListData,sizeof(m_cbCardListData));

	////混乱扑克
	//BYTE cbRandCount=0,cbPosition=0;
	//do
	//{
	//	cbPosition=rand()%(CountArray(cbCardData)-cbRandCount);
	//	cbCardBuffer[cbRandCount++]=cbCardData[cbPosition];
	//	cbCardData[cbPosition]=cbCardData[CountArray(cbCardData)-cbRandCount];
	//} while (cbRandCount<cbBufferCount);

	return;
}

//获取牌点
BYTE CGameLogic::GetCardPip(BYTE cbCardData)
{
	//计算牌点
	BYTE cbCardValue=GetCardValue(cbCardData);
	BYTE cbPipCount=(cbCardValue>=10)?0:cbCardValue;

	return cbPipCount;
}

//获取牌点
BYTE CGameLogic::GetCardListPip(const BYTE cbCardData[], BYTE cbCardCount)
{
	////变量定义
	//BYTE cbPipCount=0;

	////获取牌点
	//for (BYTE i=0;i<cbCardCount;i++)
	//{
	//	cbPipCount +=GetCardPip(cbCardData[i]);
	//}

//	return cbPipCount%10;
	return 0;
}


/************************************************************************/
//旧规则：   闲赢: 陪闲及闲方对应点数，吃其它所有区域
//			庄赢: 陪庄及庄方对应点数，吃其它所有区域
//			平局：陪和，同时不吃所有其它下注区域
//新规则： 无论开奖结果如何，1-9点只要押中就赔，开0点，对应方1-9点不吃，开和，庄闲不吃
//	
/************************************************************************/
void CGameLogic::GetOdds(BYTE cbPlayerPip, BYTE cbBankerPip, int  nOdds[],BYTE playCardArray[] ,BYTE bankCardArray[],int playcount ,int bankcount)
{
	//FillMemory(nOdds, sizeof(INT) * INDEX_MAX, -1);
	for (int i=0;i<8;i++)
	{
		nOdds[i]=-1;
	}

	if (cbPlayerPip > cbBankerPip)    //闲家胜
	{
		nOdds[0] = LOSS_RATES[0];
	}
	if (cbPlayerPip < cbBankerPip)   //庄家胜
	{
		nOdds[2] = LOSS_RATES[2];
	}
	if (cbPlayerPip == cbBankerPip)
	{
		nOdds[1] = LOSS_RATES[1];
		nOdds[0] = 0;
		nOdds[2] = 0;
	}
	//计算牌点
	BYTE cbCardValue0=GetCardValue(playCardArray[0]);
	BYTE cbCardValue1=GetCardValue(playCardArray[1]);

	if (cbCardValue0==cbCardValue1)
	{
		nOdds[3] = LOSS_RATES[3];
	}

	//计算牌点
	BYTE	cbCardValue3=GetCardValue(bankCardArray[0]);
	BYTE    cbCardValue4=GetCardValue(bankCardArray[1]);

	if (cbCardValue3==cbCardValue4)
	{
		nOdds[4] = LOSS_RATES[4];
	}
	if((playcount+bankcount)==4)
	{
		nOdds[6] = LOSS_RATES[6];
	}
    //计算大小
	if ((playcount+bankcount)!=4)
	{
		nOdds[5] = LOSS_RATES[5];
	}




}

//根据赔率，下注额计算输赢金额
void CGameLogic::GetWinAcount(INT nOdds[], INT64 pUserStakes[], LONG lRakeRate, long& lWinAmount, long& lRakeOff)
{
	//变量定义
	LONG lAmount = 0;
	lWinAmount = 0;
	lRakeOff = 0;

	for (BYTE i=0; i<INDEX_MAX; i++)
	{
		if (pUserStakes[i] == 0)
			continue;

		lAmount = pUserStakes[i] * (nOdds[i]/100);

		//只有赢了才能算佣金
		if (lAmount > 0 && lRakeRate != 0)
		{
			lRakeOff += lAmount * lRakeRate/100L;
		}

		lWinAmount += lAmount;
	}

	return;
}

LONG CGameLogic::GetWinAcountBanker(BYTE cbPlayerValue, BYTE cbBankerValue, LONG pUserStakes[], LONG lRakeRate)
{
	LONG lWinAmount = 0, lRakeOff = 0;
	//INT nOdds[INDEX_MAX];
	//ZeroMemory(nOdds, sizeof(nOdds));

	//GetOdds(cbPlayerValue, cbBankerValue, nOdds);

	////庄家的赔付取玩家赔付的相反数
	//for (WORD i=0; i<CountArray(nOdds); i++)
	//	nOdds[i] = -nOdds[i];

	//GetWinAcount(nOdds, pUserStakes, lRakeRate, lWinAmount, lRakeOff);

	//庄家输赢
	return lWinAmount - lRakeOff;



}
////虚拟计算，判断是否超过庄家最大赔付
//bool CGameLogic::BankerIsEnd(BYTE cbBetArea, LONG lBetAmount, LONG lBetSumOld[], LONG lBankerMaxLoss, LONG lRakeRate)
//{
//	LONG lWinAmount = 0;
//	INT nOdds[INDEX_MAX];
//	LONG lBetSumNew[INDEX_MAX];
//
//	ZeroMemory(nOdds, sizeof(nOdds));
//	ZeroMemory(lBetSumNew, sizeof(lBetSumNew));
//
//	CopyMemory(lBetSumNew, lBetSumOld, sizeof(lBetSumNew));
//	lBetSumNew[cbBetArea] += lBetAmount;
//
//	lWinAmount = GetBankerMaxLoss(cbBetArea, lBetSumNew, lRakeRate);
//
//	return lWinAmount + lBankerMaxLoss < 0;
//}
//虚拟计算，判断是否超过庄家最大赔付
bool CGameLogic::BankerIsOver(BYTE cbBetArea, LONG lBetAmount, LONG lBetSumOld[], LONG lBankerMaxLoss, LONG lRakeRate)
{
	LONG lWinAmount = 0;
	INT nOdds[INDEX_MAX];
	LONG lBetSumNew[INDEX_MAX];

	ZeroMemory(nOdds, sizeof(nOdds));
	ZeroMemory(lBetSumNew, sizeof(lBetSumNew));

	CopyMemory(lBetSumNew, lBetSumOld, sizeof(lBetSumNew));
	lBetSumNew[cbBetArea] += lBetAmount;

	lWinAmount = GetBankerMaxLoss(cbBetArea, lBetSumNew, lRakeRate);

	return lWinAmount + lBankerMaxLoss < 0;
}

void CGameLogic::GetMaxStakes(LONG lMaxStakes[], LONG lBetSum[], LONG lBankerMaxLoss, LONG lRakeRate)
{
	LONG lWinAmount;

	for (BYTE cbBetArea = 0; cbBetArea < INDEX_MAX; cbBetArea++)
	{
		lWinAmount = GetBankerMaxLoss(cbBetArea, lBetSum, lRakeRate);
		lMaxStakes[cbBetArea] = max((lBankerMaxLoss + lWinAmount)*100 / LOSS_RATES[cbBetArea], 0);
	}
}

//虚拟计算庄家最大赔付
LONG CGameLogic::GetBankerMaxLoss(BYTE cbBetArea, LONG lBetSum[], LONG lRakeRate)
{
	LONG lWinAmount = MAX_SCORE;//, lRakeOff = 0;
	BYTE cbPlayerValue = 0, cbBankerValue = 0;

	
	//庄家最大赔付
	return lWinAmount;
}

INT64 CGameLogic::Min(INT64 iFirst, ... )
{
	INT64 iMin = iFirst;
	INT64 iNext = iFirst;
	va_list marker;
	va_start(marker, iFirst);     /*Initialize variable arguments.*/
	iNext = va_arg(marker, INT64);

	while(iNext !=-1)
	{

		//iMin = min(iMin, iNext);
		if (iMin>iNext)
		{
           iMin=iNext;
		}
		iNext = va_arg(marker, INT64);
	}

	va_end(marker);              /*Reset variable arguments.*/
	return iMin;
}
bool CGameLogic::BankerIsEnd(BYTE cbBetArea, LONG lBetAmount, INT64 lBetSumOld[], INT64 lBankerMaxLoss, INT64 lBankersore)
{

    INT64 TOtal=min(lBankerMaxLoss,lBankersore);
    INT64 fAllYaZhuTotale=0;
	for (int i=0;i<INDEX_MAX;i++)
	{
		if (i==cbBetArea)
		{
             lBetSumOld[i]+=lBetAmount;
		}
         fAllYaZhuTotale+=lBetSumOld[i];

	}

	return  (lBetSumOld[cbBetArea]*LOSS_RATES[cbBetArea]>(TOtal+fAllYaZhuTotale-lBetSumOld[cbBetArea]))?true:false;


}
//////////////////////////////////////////////////////////////////////////
