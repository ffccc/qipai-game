#include "StdAfx.h"
#include "GameLogic.h"

#include "GameClient.h"
/*
�����齫��
 1���ƾ֣� ������ 136 �š�4����Ϸ��

2���򷨣�
��1�����ɳԡ��������ɸܡ�

3�����ƣ� 
�������ͣ�
ƽ����ͣ�ƺ������Ʊ����˺��Ƴ�ƽ����
�������Լ����ƺ���������,
��������û��ͣ��ǰ�ź�����Ҫ����������һ�־���ͣ��ʱ�����һ���Ʊ��˺��ƺ�Ҳ���������������һ�˳е�������ҵ�������ķ�����
�߶� ��7�Կ��Ժ��ơ�
�ܺ��������ϻ�һ����ƽ���������ϻ���������

4�����ƣ� 
��1�����ƺ���ܺ��ƣ����ƺ��ܻ��ơ�
��2��ͣ�ƺ�ź�������ơ�(!!)

5.���ƣ���10�յ��ƣ�������û�к��ƾͻ��ƣ����ƺ�ׯ�Ҳ��䡣������ׯׯ�����м�һƽ���ķ֡�ׯ�Ҳ��䡣

*/

//Ӧ�ó������
extern CGameClientApp theApp;
//////////////////////////////////////////////////////////////////////////

//�齫����
BYTE const CGameLogic::m_cbCardDataArray[MAX_REPERTORY]=
{
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//����
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//����
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//����
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//����
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//����
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//����
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//����
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//����
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//ͬ��
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//ͬ��
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//ͬ��
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//ͬ��
//�� �� �� ������ ���� �װ�
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,						//��
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,						//��
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,						//��
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,						//��
};

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameLogic::CGameLogic()
{

}

//��������
CGameLogic::~CGameLogic()
{
}

//�����齫
void CGameLogic::RandCardData(BYTE cbCardData[], BYTE cbMaxCount)
{
	//����׼��
	BYTE cbCardDataTemp[CountArray(m_cbCardDataArray)];
	CopyMemory(cbCardDataTemp,m_cbCardDataArray,sizeof(m_cbCardDataArray));

	//�����齫
	BYTE cbRandCount=0,cbPosition=0;
	do
	{
		cbPosition=rand()%(cbMaxCount-cbRandCount);
		cbCardData[cbRandCount++]=cbCardDataTemp[cbPosition];
		cbCardDataTemp[cbPosition]=cbCardDataTemp[cbMaxCount-cbRandCount];
	} while (cbRandCount<cbMaxCount);

	return;
}

//ɾ���齫
bool CGameLogic::RemoveCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbRemoveCard)
{
	//Ч���齫
	ASSERT(IsValidCard(cbRemoveCard));
	ASSERT(cbCardIndex[SwitchToCardIndex(cbRemoveCard)]>0);

	//ɾ���齫
	BYTE cbRemoveIndex=SwitchToCardIndex(cbRemoveCard);
	if (cbCardIndex[cbRemoveIndex]>0)
	{
		cbCardIndex[cbRemoveIndex]--;
		return true;
	}

	//ʧ��Ч��
	ASSERT(FALSE);

	return false;
}

//ɾ���齫
bool CGameLogic::RemoveCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbRemoveCard[], BYTE cbRemoveCount)
{
	//ɾ���齫
	for (BYTE i=0;i<cbRemoveCount;i++)
	{
		//Ч���齫
		ASSERT(IsValidCard(cbRemoveCard[i]));
		ASSERT(cbCardIndex[SwitchToCardIndex(cbRemoveCard[i])]>0);

		//ɾ���齫
		BYTE cbRemoveIndex=SwitchToCardIndex(cbRemoveCard[i]);
		if (cbCardIndex[cbRemoveIndex]==0)
		{
			//�������
			ASSERT(FALSE);

			//��ԭɾ��
			for (BYTE j=0;j<i;j++) 
			{
				ASSERT(IsValidCard(cbRemoveCard[j]));
				cbCardIndex[SwitchToCardIndex(cbRemoveCard[j])]++;
			}

			return false;
		}
		else 
		{
			//ɾ���齫
			--cbCardIndex[cbRemoveIndex];
		}
	}

	return true;
}

//ɾ���齫
bool CGameLogic::RemoveCard(BYTE cbCardData[], BYTE cbCardCount, BYTE cbRemoveCard[], BYTE cbRemoveCount)
{
	//��������
	ASSERT(cbCardCount<=14);
	ASSERT(cbRemoveCount<=cbCardCount);

	//�������
	BYTE cbDeleteCount=0,cbTempCardData[14];
	if (cbCardCount>CountArray(cbTempCardData)) return false;
	CopyMemory(cbTempCardData,cbCardData,cbCardCount*sizeof(cbCardData[0]));

	//�����齫
	for (BYTE i=0;i<cbRemoveCount;i++)
	{
		for (BYTE j=0;j<cbCardCount;j++)
		{
			if (cbRemoveCard[i]==cbTempCardData[j])
			{
				cbDeleteCount++;
				cbTempCardData[j]=0;
				break;
			}
		}
	}

	//�ɹ��ж�
	if (cbDeleteCount!=cbRemoveCount) 
	{
		ASSERT(FALSE);
		return false;
	}

	//�����齫
	BYTE cbCardPos=0;
	for (BYTE i=0;i<cbCardCount;i++)
	{
		if (cbTempCardData[i]!=0) cbCardData[cbCardPos++]=cbTempCardData[i];
	}

	return true;
}

//��Ч�ж�
bool CGameLogic::IsValidCard(BYTE cbCardData)
{
	BYTE cbValue=(cbCardData&MASK_VALUE);//����
	BYTE cbColor=(cbCardData&MASK_COLOR)>>4;//��ɫ
	return (((cbValue>=1)&&(cbValue<=9)&&(cbColor<=2))||((cbValue>=1)&&(cbValue<=7)&&(cbColor==3)));
}

//�齫��Ŀ
BYTE CGameLogic::GetCardCount(BYTE cbCardIndex[MAX_INDEX])
{
	//��Ŀͳ��
	BYTE cbCardCount=0;
	for (BYTE i=0;i<MAX_INDEX;i++) cbCardCount+=cbCardIndex[i];

	return cbCardCount;
}

//��ȡ���
BYTE CGameLogic::GetWeaveCard(BYTE cbWeaveKind, BYTE cbCenterCard, BYTE cbCardBuffer[4])
{
	CString strFile,strTemp;
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	strFile.Format("log\\%sGetWeaveCard.log",strTime);

			strTemp.Format("GetWeaveCard  BYTE cbWeaveKind=%d, BYTE cbCenterCard=%d, BYTE cbCardBuffer[4]",  cbWeaveKind,  cbCenterCard);
			theApp.WriteLog(strFile, strTemp);
	//����齫
	switch (cbWeaveKind)
	{
	case WIK_LEFT:		//���Ʋ���
		{
			//���ñ���
			cbCardBuffer[0]=cbCenterCard+1;
			cbCardBuffer[1]=cbCenterCard+2;
			cbCardBuffer[2]=cbCenterCard;

			return 3;
		}
	case WIK_RIGHT:		//���Ʋ���
		{
			//���ñ���
			cbCardBuffer[0]=cbCenterCard-2;
			cbCardBuffer[1]=cbCenterCard-1;
			cbCardBuffer[2]=cbCenterCard;

			return 3;
		}
	case WIK_CENTER:	//���Ʋ���
		{
			//���ñ���
			cbCardBuffer[0]=cbCenterCard-1;
			cbCardBuffer[1]=cbCenterCard;
			cbCardBuffer[2]=cbCenterCard+1;

			return 3;
		}
	case WIK_PENG:		//���Ʋ���
		{
			//���ñ���
			cbCardBuffer[0]=cbCenterCard;
			cbCardBuffer[1]=cbCenterCard;
			cbCardBuffer[2]=cbCenterCard;

			return 3;
		}
	case WIK_FILL:		//���Ʋ���
		{
			//���ñ���
			cbCardBuffer[0]=cbCenterCard;
			cbCardBuffer[1]=cbCenterCard;
			cbCardBuffer[2]=cbCenterCard;
			cbCardBuffer[3]=cbCenterCard;

			return 4;
		}
	case WIK_GANG:		//���Ʋ���
		{
			//���ñ���
			cbCardBuffer[0]=cbCenterCard;
			cbCardBuffer[1]=cbCenterCard;
			cbCardBuffer[2]=cbCenterCard;
			cbCardBuffer[3]=cbCenterCard;

			return 4;
		}
	default:
		{
			ASSERT(FALSE);
		}
	}

	return 0;
}

//�����ȼ�
BYTE CGameLogic::GetUserActionRank(BYTE cbUserAction)
{
	CString strFile,strTemp;
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	strFile.Format("log\\%sGetUserActionRank.log",strTime);

			strTemp.Format("GetUserActionRank");
			theApp.WriteLog(strFile, strTemp);
	//���Ƶȼ�
	if (cbUserAction&WIK_CHI_HU) { return 4; }

	//���Ƶȼ�
	if (cbUserAction&(WIK_FILL|WIK_GANG)) { return 3; }

	//���Ƶȼ�
	if (cbUserAction&WIK_PENG) { return 2; }

	//���Ƶȼ�
	if (cbUserAction&(WIK_RIGHT|WIK_CENTER|WIK_LEFT)) { return 1; }

	return 0;
}

//���Ƶȼ�
BYTE CGameLogic::GetChiHuActionRank(tagChiHuResult & ChiHuResult)
{
	CString strFile,strTemp;
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	strFile.Format("log\\%sGetChiHuActionRank.log",strTime);

			strTemp.Format("GetChiHuActionRank");
			theApp.WriteLog(strFile, strTemp);
	//��������
	BYTE cbChiHuOrder=0;
	WORD wChiHuRight=ChiHuResult.wChiHuRight;
	WORD wChiHuKind=(ChiHuResult.wChiHuKind&0xFF00)>>4;

	//�������
	for (BYTE i=0;i<8;i++)
	{
		wChiHuKind>>=1;
		if ((wChiHuKind&0x0001)!=0) cbChiHuOrder++;
	}

	//Ȩλ����
	for (BYTE i=0;i<16;i++)
	{
		wChiHuRight>>=1;
		if ((wChiHuRight&0x0001)!=0) cbChiHuOrder++;
	}

	return cbChiHuOrder;
}
/*
//�Ժ��ж�
BYTE CGameLogic::EstimateChiHu(BYTE cbCardIndex[MAX_INDEX])
{
	//��������
	BYTE cbActionMask=WIK_NULL;

	//�������
	if (IsSiXi(cbCardIndex)==true) cbActionMask|=WIK_CHI_HU;
	else if (IsQueYiSe(cbCardIndex)==true) cbActionMask|=WIK_CHI_HU;
	else if (IsBanBanHu(cbCardIndex)==true) cbActionMask|=WIK_CHI_HU;
//	else if (IsLiuLiuShun(cbCardIndex)==true) cbActionMask|=WIK_CHI_HU;

	return cbActionMask;
}

//�����ж�
BYTE CGameLogic::EstimateEatCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard)
{
	//����Ч��
	ASSERT(IsValidCard(cbCurrentCard));

	//�����ж�
	if (cbCurrentCard>=0x31) return WIK_NULL;

	//��������
	BYTE cbExcursion[3]={0,1,2};
	BYTE cbItemKind[3]={WIK_LEFT,WIK_CENTER,WIK_RIGHT};

	//�����ж�
	BYTE cbEatKind=0,cbFirstIndex=0;
	BYTE cbCurrentIndex=SwitchToCardIndex(cbCurrentCard);
	for (BYTE i=0;i<CountArray(cbItemKind);i++)
	{
		BYTE cbValueIndex=cbCurrentIndex%9;
		if ((cbValueIndex>=cbExcursion[i])&&((cbValueIndex-cbExcursion[i])<=6))
		{
			//�����ж�
			cbFirstIndex=cbCurrentIndex-cbExcursion[i];
			if ((cbCurrentIndex!=cbFirstIndex)&&(cbCardIndex[cbFirstIndex]==0)) continue;
			if ((cbCurrentIndex!=(cbFirstIndex+1))&&(cbCardIndex[cbFirstIndex+1]==0)) continue;
			if ((cbCurrentIndex!=(cbFirstIndex+2))&&(cbCardIndex[cbFirstIndex+2]==0)) continue;

			//��������
			cbEatKind|=cbItemKind[i];
		}
	}

	return cbEatKind;
}
*/
//�����ж�
BYTE CGameLogic::EstimatePengCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard)
{
	CString strFile,strTemp;
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	strFile.Format("log\\%sEstimatePengCard.log",strTime);

			strTemp.Format("EstimatePengCard");
			theApp.WriteLog(strFile, strTemp);
	//����Ч��
	ASSERT(IsValidCard(cbCurrentCard));

	//�����ж�
	return (cbCardIndex[SwitchToCardIndex(cbCurrentCard)]>=2)?WIK_PENG:WIK_NULL;
}

//�����ж�
BYTE CGameLogic::EstimateGangCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard)
{
	CString strFile,strTemp;
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	strFile.Format("log\\%sEstimateGangCard.log",strTime);

			strTemp.Format("EstimateGangCard");
			theApp.WriteLog(strFile, strTemp);
	//����Ч��
	ASSERT(IsValidCard(cbCurrentCard));

	//�����ж�
	return (cbCardIndex[SwitchToCardIndex(cbCurrentCard)]==3)?(WIK_GANG|WIK_FILL):WIK_NULL;
}

//���Ʒ���
BYTE CGameLogic::AnalyseGangCard(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbWeaveCount, tagGangCardResult & GangCardResult)
{
	CString strFile,strTemp;
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	strFile.Format("log\\%sAnalyseGangCard.log",strTime);

			strTemp.Format("AnalyseGangCard");
			theApp.WriteLog(strFile, strTemp);
	//���ñ���
	BYTE cbActionMask=WIK_NULL;
	ZeroMemory(&GangCardResult,sizeof(GangCardResult));

	//���ϸ���
	for (BYTE i=0;i<MAX_INDEX;i++)
	{
		if (cbCardIndex[i]==4)
		{
			cbActionMask|=(WIK_GANG|WIK_FILL);
			GangCardResult.cbCardData[GangCardResult.cbCardCount]=(WIK_GANG|WIK_FILL);
			GangCardResult.cbCardData[GangCardResult.cbCardCount++]=SwitchToCardData(i);
		}
	}

	//��ϸ���
	for (BYTE i=0;i<cbWeaveCount;i++)
	{
		if (WeaveItem[i].cbWeaveKind==WIK_PENG)
		{
			if (cbCardIndex[SwitchToCardIndex(WeaveItem[i].cbCenterCard)]==1)
			{
				cbActionMask|=(WIK_GANG|WIK_FILL);
				GangCardResult.cbCardData[GangCardResult.cbCardCount]=(WIK_GANG|WIK_FILL);
				GangCardResult.cbCardData[GangCardResult.cbCardCount++]=WeaveItem[i].cbCenterCard;
			}
		}
	}

	return cbActionMask;
}

//�Ժ�����
/*
BYTE CGameLogic::AnalyseChiHuCard(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard, WORD wChiHuRight, tagChiHuResult & ChiHuResult)
{
	CString strFile,strTemp;
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	strFile.Format("log\\%sAnalyseChiHuCard.log",strTime);

			strTemp.Format("AnalyseChiHuCard  cbWeaveCount=%d,  cbCurrentCard=%d",   cbWeaveCount,  cbCurrentCard);
			theApp.WriteLog(strFile, strTemp);

	//��������
	WORD wChiHuKind=CHK_NULL;
	static CAnalyseItemArray AnalyseItemArray;

	//���ñ���
	AnalyseItemArray.RemoveAll();
	ZeroMemory(&ChiHuResult,sizeof(ChiHuResult));

	//�����齫
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));

	//�����齫
	if (cbCurrentCard!=0) cbCardIndexTemp[SwitchToCardIndex(cbCurrentCard)]++;

	//Ȩλ����
	if ((cbCurrentCard!=0)&&(cbWeaveCount==4)) wChiHuRight|=CHR_QUAN_QIU_REN;
	if (IsQingYiSe(cbCardIndexTemp,WeaveItem,cbWeaveCount)==true) wChiHuRight|=CHR_QING_YI_SE;

			strTemp.Format("wChiHuRight=%d",   wChiHuRight);
			theApp.WriteLog(strFile, strTemp);

	//�����ж�
	if (IsQiXiaoDui(cbCardIndexTemp,WeaveItem,cbWeaveCount)==true) wChiHuKind|=CHK_QI_XIAO_DUI;
	if (IsHaoHuaDui(cbCardIndexTemp,WeaveItem,cbWeaveCount)==true) wChiHuKind|=CHK_HAO_HUA_DUI;
//	if (IsJiangJiangHu(cbCardIndexTemp,WeaveItem,cbWeaveCount)==true) wChiHuKind|=CHK_JIANG_JIANG;

			strTemp.Format("wChiHuKind=%d",   wChiHuKind);
			theApp.WriteLog(strFile, strTemp);

	//�����齫
	AnalyseCard(cbCardIndexTemp,WeaveItem,cbWeaveCount,AnalyseItemArray);


			strTemp.Format("AnalyseItemArray.GetCount()=%d",   AnalyseItemArray.GetCount());
			theApp.WriteLog(strFile, strTemp);

	//���Ʒ���
	if (AnalyseItemArray.GetCount()>0)
	{
		//��������
		bool bNeedSymbol=((wChiHuRight&0xFF00)==0);

		//���ͷ���
		for (INT_PTR i=0;i<AnalyseItemArray.GetCount();i++)
		{
			//��������
			bool bLianCard=false,bPengCard=false;
			tagAnalyseItem * pAnalyseItem=&AnalyseItemArray[i];

			//��������
			BYTE cbEyeValue=pAnalyseItem->cbCardEye&MASK_VALUE;
			bool bSymbolEye=((cbEyeValue==2)||(cbEyeValue==5)||(cbEyeValue==8));

			//���ͷ���
			for (BYTE j=0;j<CountArray(pAnalyseItem->cbWeaveKind);j++)
			{
				BYTE cbWeaveKind=pAnalyseItem->cbWeaveKind[j];
				bPengCard=((cbWeaveKind&(WIK_GANG|WIK_FILL|WIK_PENG))!=0)?true:bPengCard;
				bLianCard=((cbWeaveKind&(WIK_LEFT|WIK_CENTER|WIK_RIGHT))!=0)?true:bLianCard;
			}//End for (BYTE j=0;j<CountArray(pAnalyseItem->cbWeaveKind);j++)

			//�����ж�
			ASSERT((bLianCard==true)||(bPengCard==true));

			//��������
			if ((bLianCard==false)&&(bPengCard==true)) wChiHuKind|=CHK_PENG_PENG;
			if ((bLianCard==true)&&(bPengCard==true)&&((bSymbolEye==true)||(bNeedSymbol==false))) wChiHuKind|=CHK_JI_HU;
			if ((bLianCard==true)&&(bPengCard==false)&&((bSymbolEye==true)||(bNeedSymbol==false))) wChiHuKind|=CHK_PING_HU;
		}//End for (INT_PTR i=0;i<AnalyseItemArray.GetCount();i++)
	}//End if (AnalyseItemArray.GetCount()>0)

	//����ж�
	if (wChiHuKind!=CHK_NULL)
	{
		//���ý��
		ChiHuResult.wChiHuKind=wChiHuKind;
		ChiHuResult.wChiHuRight=wChiHuRight;

		return WIK_CHI_HU;
	}//End if (wChiHuKind!=CHK_NULL)

	return WIK_NULL;
}
*/
/*
//��ϲ����
bool CGameLogic::IsSiXi(BYTE cbCardIndex[MAX_INDEX])
{
	//�����ж�
	for (BYTE i=0;i<MAX_INDEX;i++)
	{
		if (cbCardIndex[i]==4) return true;
	}

	return false;
}
*/
//ȱһɫ��
bool CGameLogic::IsQueYiSe(BYTE cbCardIndex[MAX_INDEX])
{
	//�����ж�
	BYTE cbIndex[]={0,9,18};
	BYTE i = 0;
	BYTE j = 0;
	for (i=0;i<CountArray(cbIndex);i++)
	{
		for (j=cbIndex[i];j<(cbIndex[i]+9);j++)
		{
			if (cbCardIndex[j]!=0) break;
		}
		if (j==(cbIndex[i]+9)) return true;
	}

	return false;
}
/*
//������
bool CGameLogic::IsBanBanHu(BYTE cbCardIndex[MAX_INDEX])
{
	//�����ж�
	for (BYTE i=1;i<MAX_INDEX;i+=3) if (cbCardIndex[i]!=0) return false;

	return true;
}

//����˳��
bool CGameLogic::IsLiuLiuShun(BYTE cbCardIndex[MAX_INDEX])
{
	//�����ж�
	BYTE cbPengCount=0;
	for (BYTE i=0;i<MAX_INDEX;i++)
	{
		if ((cbCardIndex[i]>=3)&&(++cbPengCount>=2)) return true;
	}

	return false;
}
*/
//��һɫ��
bool CGameLogic::IsQingYiSe(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbItemCount)
{
	CString strFile,strTemp;
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	strFile.Format("log\\%sIsQingYiSe.log",strTime);

			strTemp.Format("IsQingYiSe");
			theApp.WriteLog(strFile, strTemp);
	//�����ж�
	BYTE cbCardColor=0xFF;
	for (BYTE i=0;i<MAX_INDEX;i++)
	{
		if (cbCardIndex[i]!=0)
		{
			//��ɫ�ж�
			if (cbCardColor!=0xFF) return false;

			//���û�ɫ
			cbCardColor=(SwitchToCardData(i)&MASK_COLOR);

			//��������
			i=(i/9+1)*9;
		}
	}

	//����ж�
	for (BYTE i=0;i<cbItemCount;i++)
	{
		BYTE cbCenterCard=WeaveItem[i].cbCenterCard;
		if ((cbCenterCard&MASK_COLOR)!=cbCardColor) return false;
	}

	return true;
}

//��С����
bool CGameLogic::IsQiXiaoDui(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbWeaveCount)
{
	CString strFile,strTemp;
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	strFile.Format("log\\%sIsQiXiaoDui.log",strTime);

			strTemp.Format("IsQiXiaoDui");
			theApp.WriteLog(strFile, strTemp);
	//����ж�
	if (cbWeaveCount!=0) return false;

	//�齫�ж�
	for (BYTE i=0;i<MAX_INDEX;i++)
	{
		BYTE cbCardCount=cbCardIndex[i];
		if ((cbCardCount!=0)&&(cbCardCount!=2)&&(cbCardCount!=4)) return false;
	}

	return true;
}

//��������
bool CGameLogic::IsHaoHuaDui(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbItemCount)
{
	//��������
	bool bFourCard=false;

	//����ж�
	for (BYTE i=0;i<cbItemCount;i++)
	{
		//�˲��ж�
		if (WeaveItem[i].cbWeaveKind!=WIK_FILL) return false;
		if (WeaveItem[i].cbWeaveKind!=WIK_GANG) return false;

		//���ñ���
		bFourCard=true;
	}

	//�齫�ж�
	for (BYTE i=0;i<MAX_INDEX;i++)
	{
		//�����ж�
		if (cbCardIndex[i]==4)
		{
			bFourCard=true;
			continue;
		}

		//�����ж�
		if ((cbCardIndex[i]!=0)&&(cbCardIndex[i]!=2)) return false;
	}

	//����ж�
	if (bFourCard==false) return false;

	return true;
}
/*
//��������
bool CGameLogic::IsJiangJiangHu(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbWeaveCount)
{
	//����ж�
	for (BYTE i=0;i<cbWeaveCount;i++)
	{
		//�����ж�
		BYTE cbWeaveKind=WeaveItem[i].cbWeaveKind;
		if ((cbWeaveKind!=WIK_PENG)&&(cbWeaveKind!=WIK_GANG)&&(cbWeaveKind!=WIK_FILL)) return false;

		//��ֵ�ж�
		BYTE cbCenterValue=(WeaveItem[i].cbCenterCard&MASK_VALUE);
		if ((cbCenterValue!=2)&&(cbCenterValue!=5)&&(cbCenterValue!=8)) return false;
	}

	//�齫�ж�
	for (BYTE i=0;i<MAX_INDEX;i++)
	{
		if ((i%3!=1)&&(cbCardIndex[i]!=0)) return false;
	}

	return true;
}
*/
//�齫ת��
BYTE CGameLogic::SwitchToCardData(BYTE cbCardIndex)
{
	ASSERT(cbCardIndex<34);
	return ((cbCardIndex/9)<<4)|(cbCardIndex%9+1);
}

//�齫ת��
BYTE CGameLogic::SwitchToCardIndex(BYTE cbCardData)
{
	ASSERT(IsValidCard(cbCardData));
	return ((cbCardData&MASK_COLOR)>>4)*9+(cbCardData&MASK_VALUE)-1;
}

//�齫ת��
BYTE CGameLogic::SwitchToCardData(BYTE cbCardIndex[MAX_INDEX], BYTE cbCardData[MAX_COUNT])
{
	//ת���齫
	BYTE cbPosition=0;
	for (BYTE i=0;i<MAX_INDEX;i++)
	{
		if (cbCardIndex[i]!=0)
		{
			for (BYTE j=0;j<cbCardIndex[i];j++)
			{
				ASSERT(cbPosition<MAX_COUNT);
				cbCardData[cbPosition++]=SwitchToCardData(i);
			}
		}
	}

	return cbPosition;
}

//�齫ת��
BYTE CGameLogic::SwitchToCardIndex(BYTE cbCardData[], BYTE cbCardCount, BYTE cbCardIndex[MAX_INDEX])
{
	//���ñ���
	ZeroMemory(cbCardIndex,sizeof(BYTE)*MAX_INDEX);

	//ת���齫
	for (BYTE i=0;i<cbCardCount;i++)
	{
		ASSERT(IsValidCard(cbCardData[i]));
		cbCardIndex[SwitchToCardIndex(cbCardData[i])]++;
	}

	return cbCardCount;
}

//�����齫
bool CGameLogic::AnalyseCard(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbWeaveCount, CAnalyseItemArray & AnalyseItemArray)
{
	CString strFile,strTemp;
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	strFile.Format("log\\%sAnalyseCard.log",strTime);

			strTemp.Format("AnalyseCard cbWeaveCount=%d", cbWeaveCount);
			theApp.WriteLog(strFile, strTemp);
	//������Ŀ
	BYTE cbCardCount=0;
	for (BYTE i=0;i<MAX_INDEX;i++) cbCardCount+=cbCardIndex[i];

	//Ч����Ŀ
	ASSERT((cbCardCount>=2)&&(cbCardCount<=MAX_COUNT)&&((cbCardCount-2)%3==0));
	if ((cbCardCount<2)||(cbCardCount>MAX_COUNT)||((cbCardCount-2)%3!=0)) return false;

	//��������
	BYTE cbKindItemCount=0;
	tagKindItem KindItem[MAX_COUNT-2];
	ZeroMemory(KindItem,sizeof(KindItem));

	//�����ж�
	BYTE cbLessKindItem=(cbCardCount-2)/3;
	ASSERT((cbLessKindItem+cbWeaveCount)==4);

	//�����ж�
	if (cbLessKindItem==0)
	{
		//Ч�����
		ASSERT((cbCardCount==2)&&(cbWeaveCount==4));

		//�����ж�
		for (BYTE i=0;i<MAX_INDEX;i++)
		{
			if (cbCardIndex[i]==2)
			{
				//��������
				tagAnalyseItem AnalyseItem;
				ZeroMemory(&AnalyseItem,sizeof(AnalyseItem));

				//���ý��
				for (BYTE j=0;j<cbWeaveCount;j++)
				{
					AnalyseItem.cbWeaveKind[j]=WeaveItem[j].cbWeaveKind;
					AnalyseItem.cbCenterCard[j]=WeaveItem[j].cbCenterCard;
				}
				AnalyseItem.cbCardEye=SwitchToCardData(i);

				//������
				AnalyseItemArray.Add(AnalyseItem);

				return true;
			}
		}

		return false;
	}

	//��ַ���
	if (cbCardCount>=3)
	{
		for (BYTE i=0;i<MAX_INDEX;i++)
		{
			//ͬ���ж�
			if (cbCardIndex[i]>=3)
			{
				KindItem[cbKindItemCount].cbCenterCard=i;
				KindItem[cbKindItemCount].cbCardIndex[0]=i;
				KindItem[cbKindItemCount].cbCardIndex[1]=i;
				KindItem[cbKindItemCount].cbCardIndex[2]=i;
				KindItem[cbKindItemCount++].cbWeaveKind=WIK_PENG;
			}
			
			//�����ж�
			if ((i<(MAX_INDEX-2))&&(cbCardIndex[i]>0)&&((i%9)<7))
			{
				for (BYTE j=1;j<=cbCardIndex[i];j++)
				{
					if ((cbCardIndex[i+1]>=j)&&(cbCardIndex[i+2]>=j))
					{
						KindItem[cbKindItemCount].cbCenterCard=i;
						KindItem[cbKindItemCount].cbCardIndex[0]=i;
						KindItem[cbKindItemCount].cbCardIndex[1]=i+1;
						KindItem[cbKindItemCount].cbCardIndex[2]=i+2;
						KindItem[cbKindItemCount++].cbWeaveKind=WIK_LEFT;
					}
				}
			}
		}
	}

	//��Ϸ���
	if (cbKindItemCount>=cbLessKindItem)
	{
		BYTE i = 0;
		//��������
		BYTE cbCardIndexTemp[MAX_INDEX];
		ZeroMemory(cbCardIndexTemp,sizeof(cbCardIndexTemp));

		//��������
		BYTE cbIndex[4]={0,1,2,3};
		tagKindItem * pKindItem[4];
		ZeroMemory(&pKindItem,sizeof(pKindItem));

		//��ʼ���
		do
		{
			//���ñ���
			CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));
			for (i=0;i<cbLessKindItem;i++) pKindItem[i]=&KindItem[cbIndex[i]];

			//�����ж�
			bool bEnoughCard=true;
			for (i=0;i<cbLessKindItem*3;i++)
			{
				//�����ж�
				BYTE cbCardIndex=pKindItem[i/3]->cbCardIndex[i%3];
				if (cbCardIndexTemp[cbCardIndex]==0)
				{
					bEnoughCard=false;
					break;
				}
				else cbCardIndexTemp[cbCardIndex]--;
			}

			//�����ж�
			if (bEnoughCard==true)
			{
				//�����ж�
				BYTE cbCardEye=0;
				for (i=0;i<MAX_INDEX;i++)
				{
					if (cbCardIndexTemp[i]==2)
					{
						cbCardEye=SwitchToCardData(i);
						break;
					}
				}

				//�������
				if (cbCardEye!=0)
				{
					//��������
					tagAnalyseItem AnalyseItem;
					ZeroMemory(&AnalyseItem,sizeof(AnalyseItem));

					//�������
					for (i=0;i<cbWeaveCount;i++)
					{
						AnalyseItem.cbWeaveKind[i]=WeaveItem[i].cbWeaveKind;
						AnalyseItem.cbCenterCard[i]=WeaveItem[i].cbCenterCard;
					}

					//��������
					for (i=0;i<cbLessKindItem;i++) 
					{
						AnalyseItem.cbWeaveKind[i+cbWeaveCount]=pKindItem[i]->cbWeaveKind;
						AnalyseItem.cbCenterCard[i+cbWeaveCount]=pKindItem[i]->cbCenterCard;
					}

					//��������
					AnalyseItem.cbCardEye=cbCardEye;

					//������
					AnalyseItemArray.Add(AnalyseItem);
				}
			}

			//��������
			if (cbIndex[cbLessKindItem-1]==(cbKindItemCount-1))
			{
				for (i=cbLessKindItem-1;i>0;i--)
				{
					if ((cbIndex[i-1]+1)!=cbIndex[i])
					{
						BYTE cbNewIndex=cbIndex[i-1];
						for (BYTE j=(i-1);j<cbLessKindItem;j++) cbIndex[j]=cbNewIndex+j-i+2;
						break;
					}
				}
				if (i==0) break;
			}
			else cbIndex[cbLessKindItem-1]++;
			
		} while (true);

	}

	return (AnalyseItemArray.GetCount()>0);
}

//////////////////////////////////////////////////////////////////////////
