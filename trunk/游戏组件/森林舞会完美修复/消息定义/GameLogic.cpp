#include "StdAfx.h"
#include "GameLogic.h"
#include "../��Ϣ����/CMD_Baccarat.h"

//////////////////////////////////////////////////////////////////////////

//�˿�����
//const BYTE CGameLogic::m_cbCardListData[52*8]=
//{
	//0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,	//���� A - K
	//0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	//÷�� A - K
	//0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	//���� A - K
	//0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,	//���� A - K

	//0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,	//���� A - K
	//0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	//÷�� A - K
	//0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	//���� A - K
	//0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,	//���� A - K

	//0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,	//���� A - K
	//0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	//÷�� A - K
	//0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	//���� A - K
	//0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,	//���� A - K

	//0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,	//���� A - K
	//0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	//÷�� A - K
	//0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	//���� A - K
	//0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,	//���� A - K

	//0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,	//���� A - K
	//0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	//÷�� A - K
	//0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	//���� A - K
	//0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,	//���� A - K

	//0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,	//���� A - K
	//0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	//÷�� A - K
	//0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	//���� A - K
	//0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,	//���� A - K

	//0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,	//���� A - K
	//0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	//÷�� A - K
	//0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	//���� A - K
	//0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,	//���� A - K

	//0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,	//���� A - K
	//0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	//÷�� A - K
	//0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	//���� A - K
	//0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,	//���� A - K
//};

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameLogic::CGameLogic()
{
}

//��������
CGameLogic::~CGameLogic()
{
}

//�����˿�
void CGameLogic::RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount)
{
	//����׼��
	//BYTE cbCardData[CountArray(m_cbCardListData)];
	//CopyMemory(cbCardData,m_cbCardListData,sizeof(m_cbCardListData));

	////�����˿�
	//BYTE cbRandCount=0,cbPosition=0;
	//do
	//{
	//	cbPosition=rand()%(CountArray(cbCardData)-cbRandCount);
	//	cbCardBuffer[cbRandCount++]=cbCardData[cbPosition];
	//	cbCardData[cbPosition]=cbCardData[CountArray(cbCardData)-cbRandCount];
	//} while (cbRandCount<cbBufferCount);

	return;
}

//��ȡ�Ƶ�
BYTE CGameLogic::GetCardPip(BYTE cbCardData)
{
	//�����Ƶ�
	BYTE cbCardValue=GetCardValue(cbCardData);
	BYTE cbPipCount=(cbCardValue>=10)?0:cbCardValue;

	return cbPipCount;
}

//��ȡ�Ƶ�
BYTE CGameLogic::GetCardListPip(const BYTE cbCardData[], BYTE cbCardCount)
{
	////��������
	//BYTE cbPipCount=0;

	////��ȡ�Ƶ�
	//for (BYTE i=0;i<cbCardCount;i++)
	//{
	//	cbPipCount +=GetCardPip(cbCardData[i]);
	//}

//	return cbPipCount%10;
	return 0;
}


/************************************************************************/
//�ɹ���   ��Ӯ: ���м��з���Ӧ��������������������
//			ׯӮ: ��ׯ��ׯ����Ӧ��������������������
//			ƽ�֣���ͣ�ͬʱ��������������ע����
//�¹��� ���ۿ��������Σ�1-9��ֻҪѺ�о��⣬��0�㣬��Ӧ��1-9�㲻�ԣ����ͣ�ׯ�в���
//	
/************************************************************************/
void CGameLogic::GetOdds(BYTE cbPlayerPip, BYTE cbBankerPip, int  nOdds[],BYTE playCardArray[] ,BYTE bankCardArray[],int playcount ,int bankcount)
{
	//FillMemory(nOdds, sizeof(INT) * INDEX_MAX, -1);
	for (int i=0;i<8;i++)
	{
		nOdds[i]=-1;
	}

	if (cbPlayerPip > cbBankerPip)    //�м�ʤ
	{
		nOdds[0] = LOSS_RATES[0];
	}
	if (cbPlayerPip < cbBankerPip)   //ׯ��ʤ
	{
		nOdds[2] = LOSS_RATES[2];
	}
	if (cbPlayerPip == cbBankerPip)
	{
		nOdds[1] = LOSS_RATES[1];
		nOdds[0] = 0;
		nOdds[2] = 0;
	}
	//�����Ƶ�
	BYTE cbCardValue0=GetCardValue(playCardArray[0]);
	BYTE cbCardValue1=GetCardValue(playCardArray[1]);

	if (cbCardValue0==cbCardValue1)
	{
		nOdds[3] = LOSS_RATES[3];
	}

	//�����Ƶ�
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
    //�����С
	if ((playcount+bankcount)!=4)
	{
		nOdds[5] = LOSS_RATES[5];
	}




}

//�������ʣ���ע�������Ӯ���
void CGameLogic::GetWinAcount(INT nOdds[], INT64 pUserStakes[], LONG lRakeRate, long& lWinAmount, long& lRakeOff)
{
	//��������
	LONG lAmount = 0;
	lWinAmount = 0;
	lRakeOff = 0;

	for (BYTE i=0; i<INDEX_MAX; i++)
	{
		if (pUserStakes[i] == 0)
			continue;

		lAmount = pUserStakes[i] * (nOdds[i]/100);

		//ֻ��Ӯ�˲�����Ӷ��
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

	////ׯ�ҵ��⸶ȡ����⸶���෴��
	//for (WORD i=0; i<CountArray(nOdds); i++)
	//	nOdds[i] = -nOdds[i];

	//GetWinAcount(nOdds, pUserStakes, lRakeRate, lWinAmount, lRakeOff);

	//ׯ����Ӯ
	return lWinAmount - lRakeOff;



}
////������㣬�ж��Ƿ񳬹�ׯ������⸶
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
//������㣬�ж��Ƿ񳬹�ׯ������⸶
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

//�������ׯ������⸶
LONG CGameLogic::GetBankerMaxLoss(BYTE cbBetArea, LONG lBetSum[], LONG lRakeRate)
{
	LONG lWinAmount = MAX_SCORE;//, lRakeOff = 0;
	BYTE cbPlayerValue = 0, cbBankerValue = 0;

	
	//ׯ������⸶
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
