#include "StdAfx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

void WriteLog(CString strFileName, CString strText)
{
	try
	{
        CTime tm = CTime::GetCurrentTime();
        CString strTime = tm.Format(_T("%Y-%m-%d %H:%M:%S"));
		//BOOL bFull = FALSE;
		CStdioFile file;
		if( file.Open(strFileName, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite) != 0)
		{
			file.SeekToEnd();
			file.WriteString(strTime);
			file.WriteString(strText);
			file.WriteString(_T("\n\n"));
			//if(file.GetLength() > 2000000)
			//	bFull = TRUE;
			file.Close();
		}
		/*
		if(!bFull) return;
		if( file.Open(strFileName, CFile::modeCreate|CFile::modeReadWrite) != 0)
		{
		file.SeekToEnd();
		file.WriteString(strTime);
		file.WriteString(strText);
		file.WriteString(_T("\n"));
		file.Close();
		}
		*/
	}
	catch(...)
	{
	}
}

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

	//����Ŀ¼
/*	TCHAR m_szWorkDirectory[MAX_PATH];
	GetModuleFileName(AfxGetInstanceHandle(),m_szWorkDirectory,sizeof(m_szWorkDirectory));
	int nModuleLen=lstrlen(m_szWorkDirectory);
	int nProcessLen=lstrlen(AfxGetApp()->m_pszExeName)+lstrlen(TEXT(".EXE"));
	m_szWorkDirectory[nModuleLen-nProcessLen]=0;
	SetCurrentDirectory(m_szWorkDirectory);

	//��ȡ�齫
	TCHAR szFile[MAX_PATH]=TEXT(""),szItem[20];
	_sntprintf(szFile,CountArray(szFile),TEXT("%s\\CardInfo.INI"),m_szWorkDirectory);

	for (BYTE i=0;i<CountArray(m_cbCardDataArray);i++)
	{
		TCHAR szRead[64]=TEXT("");
		_sntprintf(szItem,CountArray(szItem),TEXT("Item%d"),i+1);
		GetPrivateProfileString(TEXT("CardData"),szItem,TEXT(""),szRead,sizeof(szRead),szFile);
		cbCardData[i]=atoi(szRead);
		if (cbCardData[i]==0) break;
	}
	if (i==CountArray(m_cbCardDataArray)) return;*/

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
	BYTE cbValue=(cbCardData&MASK_VALUE);
	BYTE cbColor=(cbCardData&MASK_COLOR)>>4;
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
	WriteLog(strFile, strTemp);
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

		strTemp.Format("GetChiHuActionRank wChiHuKind=%d, wChiHuRight=%d" , ChiHuResult.wChiHuKind, ChiHuResult.wChiHuRight) ;
			WriteLog(strFile, strTemp);
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

		strTemp.Format("GetChiHuActionRank cbChiHuOrder=%d", cbChiHuOrder);
			WriteLog(strFile, strTemp);

	return cbChiHuOrder;
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

//�����ж�
BYTE CGameLogic::EstimatePengCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard)
{
	CString strFile,strTemp;
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	strFile.Format("log\\%sEstimatePengCard.log",strTime);

			strTemp.Format("EstimatePengCard");
			WriteLog(strFile, strTemp);
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
			WriteLog(strFile, strTemp);
	//����Ч��
	ASSERT(IsValidCard(cbCurrentCard));

	//�����ж�
	return (cbCardIndex[SwitchToCardIndex(cbCurrentCard)]==3)?(WIK_GANG|WIK_FILL):WIK_NULL;
}

//���Ʒ���
BYTE CGameLogic::AnalyseGangCard(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbWeaveCount, tagGangCardResult & GangCardResult)
{
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
BYTE CGameLogic::AnalyseChiHuCard(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard, WORD wChiHuRight, tagChiHuResult & ChiHuResult)
{
	CString strFile,strTemp;
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	strFile.Format("log\\%sAnalyseChiHuCard.log",strTime);

	strTemp.Format("AnalyseChiHuCard  cbWeaveCount=%d,  cbCurrentCard=%d",   cbWeaveCount,  cbCurrentCard);
	WriteLog(strFile, strTemp);

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
	if ((cbCurrentCard!=0)&&(cbWeaveCount==4)) wChiHuRight|=CHK_QUAN_QIU_REN;
	if (IsQingYiSe(cbCardIndexTemp,WeaveItem,cbWeaveCount)==true) wChiHuRight|=CHK_QING_YI_SE;

	strTemp.Format("wChiHuRight=%d",   wChiHuRight);
	WriteLog(strFile, strTemp);
	//�����齫
	AnalyseCard(cbCardIndexTemp,WeaveItem,cbWeaveCount,AnalyseItemArray);

	strTemp.Format("AnalyseItemArray.GetCount()=%d",   AnalyseItemArray.GetCount());
	WriteLog(strFile, strTemp);
	//���Ʒ���
	if (AnalyseItemArray.GetCount()>0)
	{
		//��������
		bool bNeedSymbol=((wChiHuRight&0xFF00)==0);

		bNeedSymbol = false;//�̶�

		//���ͷ���
		for (INT_PTR i=0;i<AnalyseItemArray.GetCount();i++)
		{
			//��������
			bool bLianCard=false,bPengCard=false;
			tagAnalyseItem * pAnalyseItem=&AnalyseItemArray[i];

			//��������
			BYTE cbEyeValue=pAnalyseItem->cbCardEye&MASK_VALUE;
			//2,5,8��
			bool bSymbolEye=((cbEyeValue==2)||(cbEyeValue==5)||(cbEyeValue==8));
			//û�н�����
			bSymbolEye = true;


			//���ͷ���
			for (BYTE j=0;j<CountArray(pAnalyseItem->cbWeaveKind);j++)
			{
				BYTE cbWeaveKind=pAnalyseItem->cbWeaveKind[j];
				bPengCard=((cbWeaveKind&(WIK_GANG|WIK_FILL|WIK_PENG))!=0)?true:bPengCard;
				bLianCard=((cbWeaveKind&(WIK_LEFT|WIK_CENTER|WIK_RIGHT))!=0)?true:bLianCard;
			}//End for

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
	}

	return WIK_NULL;
}

//��һɫ��
bool CGameLogic::IsQingYiSe(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbItemCount)
{
	CString strFile,strTemp;
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	strFile.Format("log\\%sIsQingYiSe.log",strTime);

			strTemp.Format("IsQingYiSe");
			WriteLog(strFile, strTemp);
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
	}//End for (BYTE i=0;i<MAX_INDEX;i++)

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
	}//End for (BYTE i=0;i<cbCardCount;i++)

	return cbCardCount;
}

//�����齫
bool CGameLogic::AnalyseCard(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbWeaveCount, CAnalyseItemArray & AnalyseItemArray)
{
	CString strFile,strTemp;
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	strFile.Format("log\\%sAnalyseCard.log",strTime);

	// 0, 1, 2
	strTemp.Format("AnalyseCard cbWeaveCount=%d", cbWeaveCount);
	WriteLog(strFile, strTemp);

	//������Ŀ
	BYTE cbCardCount=0;
	BYTE i = 0;
	for (i=0;i<MAX_INDEX;i++) 
	{
	strTemp.Format("cbCardIndex[%d]=%d", i,cbCardIndex[i]);
//	WriteLog(strFile, strTemp);
//0,1
		cbCardCount+=cbCardIndex[i];
	}//End if

	strTemp.Format("AnalyseCard cbCardCount=%d", cbCardCount);
	WriteLog(strFile, strTemp);


	//Ч����Ŀ
	ASSERT((cbCardCount>=2)&&(cbCardCount<=MAX_COUNT)&&((cbCardCount-2)%3==0));
	//11,14
	if ((cbCardCount<2)||(cbCardCount>MAX_COUNT)||((cbCardCount-2)%3!=0)) 
	{
		strTemp.Format("AnalyseCard ���أ�");
		WriteLog(strFile, strTemp);

		return false;
	}

	//��������
	BYTE cbKindItemCount=0;
	tagKindItem KindItem[MAX_COUNT-2];
	ZeroMemory(KindItem,sizeof(KindItem));

	//�����ж�
	BYTE cbLessKindItem=(cbCardCount-2)/3;
	ASSERT((cbLessKindItem+cbWeaveCount)==4);

//3,4
	strTemp.Format("cbLessKindItem=%d", cbLessKindItem);
	WriteLog(strFile, strTemp);

	//�����ж�
	if (cbLessKindItem==0)
	{
		strTemp.Format("�����ж�");
		WriteLog(strFile, strTemp);

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
				}//End for
				AnalyseItem.cbCardEye=SwitchToCardData(i);

				//������
				AnalyseItemArray.Add(AnalyseItem);
				strTemp.Format("AnalyseCard ����22��");
				WriteLog(strFile, strTemp);
				return true;
			}//End if (cbCardIndex[i]==2)
		}//End for (BYTE i=0;i<MAX_INDEX;i++)
		strTemp.Format("AnalyseCard ����2��");
		WriteLog(strFile, strTemp);
		return false;
	}//End if

	//��ַ���
	if (cbCardCount>=3)
	{
		strTemp.Format("��ַ���");
		WriteLog(strFile, strTemp);

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
				strTemp.Format("ͬ���ж�");
				WriteLog(strFile, strTemp);
			}//End if (cbCardIndex[i]>=3)
			
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
					}//End if
				}//End for (BYTE j=1;j<=cbCardIndex[i];j++)
				strTemp.Format("�����ж�");
				WriteLog(strFile, strTemp);
			}//End if
		}//End for (BYTE i=0;i<MAX_INDEX;i++)
	}//End if (cbCardCount>=3)

	strTemp.Format("cbKindItemCount=%d, cbLessKindItem=%d", cbKindItemCount,cbLessKindItem);
	WriteLog(strFile, strTemp);
	//��Ϸ���
	if (cbKindItemCount>=cbLessKindItem)
	{
		strTemp.Format("��Ϸ���");
		WriteLog(strFile, strTemp);

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
			}//End for (i=0;i<cbLessKindItem*3;i++)

			//�����ж�
			if (bEnoughCard==true)
			{
				//�����ж�
				BYTE cbCardEye=0;
				for (BYTE i=0;i<MAX_INDEX;i++)
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
					for (BYTE i=0;i<cbWeaveCount;i++)
					{
						AnalyseItem.cbWeaveKind[i]=WeaveItem[i].cbWeaveKind;
						AnalyseItem.cbCenterCard[i]=WeaveItem[i].cbCenterCard;
					}

					//��������
					for (BYTE i=0;i<cbLessKindItem;i++) 
					{
						AnalyseItem.cbWeaveKind[i+cbWeaveCount]=pKindItem[i]->cbWeaveKind;
						AnalyseItem.cbCenterCard[i+cbWeaveCount]=pKindItem[i]->cbCenterCard;
					}

					//��������
					AnalyseItem.cbCardEye=cbCardEye;

					//������
					AnalyseItemArray.Add(AnalyseItem);
				}
			}//End if (bEnoughCard==true)

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
