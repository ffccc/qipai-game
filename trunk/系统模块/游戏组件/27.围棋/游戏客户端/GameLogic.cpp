#include "StdAfx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameLogic::CGameLogic()
{
	//״̬����
	memset(m_wStepCount,0,sizeof(m_wStepCount));
	memset(m_wTakeChessCount,0,sizeof(m_wTakeChessCount));
	memset(m_cbLastChessBorad,0,sizeof(m_cbLastChessBorad));

	//���̱���
	memset(m_wIdentifier,0,sizeof(m_wIdentifier));
	memset(m_cbChessBorad,0,sizeof(m_cbChessBorad));
	
	return;
}

//��������
CGameLogic::~CGameLogic()
{
}

//��ȡ����
WORD CGameLogic::GetStepCount(BYTE cbColor)
{
	ASSERT((cbColor==BLACK_CHESS)||(cbColor==WHITE_CHESS));
	return m_wStepCount[(cbColor==BLACK_CHESS)?0:1];
}

//������Ŀ
WORD CGameLogic::GetTakeChessCount(BYTE cbColor)
{
	ASSERT((cbColor==BLACK_CHESS)||(cbColor==WHITE_CHESS));
	return m_wTakeChessCount[(cbColor==BLACK_CHESS)?0:1];
}

//��ȡ����
const tagChessManual * CGameLogic::GetLastChessManual()
{
	INT_PTR nManualCount=m_ChessManualInfo.GetCount();
	return (nManualCount>0)?&m_ChessManualInfo[nManualCount-1]:NULL;
}

//��ʼ�߼�
void CGameLogic::InitGameLogic()
{
	//״̬����
	memset(m_wStepCount,0,sizeof(m_wStepCount));
	memset(m_wTakeChessCount,0,sizeof(m_wTakeChessCount));
	memset(m_cbLastChessBorad,0,sizeof(m_cbLastChessBorad));

	//���̱���
	memset(m_wIdentifier,0,sizeof(m_wIdentifier));
	memset(m_cbChessBorad,0,sizeof(m_cbChessBorad));

	//���ױ���
	m_DeadChessInfo.RemoveAll();
	m_ChessManualInfo.RemoveAll();

	return;
}

//�������
bool CGameLogic::RegretChess(WORD wStepCount)
{
	//Ч�����
	ASSERT(wStepCount<=(WORD)m_ChessManualInfo.GetCount());

	//��������
	tagChessManual * pChessManual=NULL;
	INT_PTR nManualCount=m_ChessManualInfo.GetCount();

	//�������
	for (WORD i=0;i<wStepCount;i++)
	{
		//��������
		pChessManual=&m_ChessManualInfo[nManualCount-i-1];

		//״̬����
		m_wStepCount[(pChessManual->cbColor==BLACK_CHESS)?0:1]--;
		m_wTakeChessCount[(pChessManual->cbColor==BLACK_CHESS)?0:1]-=pChessManual->wDeadChessCount;

		//���̱���
		m_wIdentifier[pChessManual->cbXPos][pChessManual->cbYPos]=0;
		m_cbChessBorad[pChessManual->cbXPos][pChessManual->cbYPos]=NO_CHESS;

		//�ָ�����
		if (pChessManual->wDeadChessCount>0)
		{
			INT_PTR wDeadChessInfoCount=m_DeadChessInfo.GetCount();
			BYTE cbDeadColor=(pChessManual->cbColor==BLACK_CHESS)?WHITE_CHESS:BLACK_CHESS;
			for (WORD wDeadChessCount=0;wDeadChessCount<pChessManual->wDeadChessCount;wDeadChessCount++)
			{
				tagChessItem * pChessItem=&m_DeadChessInfo[wDeadChessInfoCount-wDeadChessCount-1];
				m_cbChessBorad[pChessItem->cbXPos][pChessItem->cbYPos]=cbDeadColor;
				m_wIdentifier[pChessItem->cbXPos][pChessItem->cbYPos]=pChessItem->wIdentifier;
			}
			m_DeadChessInfo.RemoveAt(wDeadChessInfoCount-pChessManual->wDeadChessCount,pChessManual->wDeadChessCount);
		}
	}

	//ɾ������
	m_ChessManualInfo.RemoveAt(nManualCount-wStepCount,wStepCount);

	return true;
}

//��Ŀͳ��
bool CGameLogic::GetChessCountInfo(tagCountResult & CountResult)
{
	//��Ŀ����
	ASSERT((m_wStepCount[0]+m_wStepCount[1])>0);
	if ((m_wStepCount[0]+m_wStepCount[1])==0) return false;

	//���ñ���
	tagCountAssistant CountAssistant;
	memset(&CountResult,0,sizeof(CountResult));
	memset(&CountAssistant,0,sizeof(CountAssistant));

	//��Ŀͳ��
	int i=0;
	for (BYTE cbXPos=0;cbXPos<19;cbXPos++)
	{
		for (BYTE cbYPos=0;cbYPos<19;cbYPos++)
		{
			//��λ����
			if (m_cbChessBorad[cbXPos][cbYPos]==NO_CHESS) continue;

			//���ñ���
			CountAssistant.cbCurIndex=(m_cbChessBorad[cbXPos][cbYPos]==BLACK_CHESS)?0:1;
			CountAssistant.cbChessMask=(m_cbChessBorad[cbXPos][cbYPos]==BLACK_CHESS)?CT_BLACE_CHESS:CT_WHITE_CHESS;

			//��������
			CountResult.wChessCount[CountAssistant.cbCurIndex]++;
			CountResult.cbCountInfo[cbXPos][cbYPos]|=CountAssistant.cbChessMask;

			//����׼��
			CountAssistant.wChessCount[CountAssistant.cbCurIndex]=1;
			CountAssistant.wChessList[CountAssistant.cbCurIndex][0]=MAKEWORD(cbXPos,cbYPos);

			//��������
			for (WORD wExpandIndex=0;wExpandIndex<CountAssistant.wChessCount[CountAssistant.cbCurIndex];wExpandIndex++)
			{
				BYTE cbXChessPos=LOBYTE(CountAssistant.wChessList[CountAssistant.cbCurIndex][wExpandIndex]);
				BYTE cbYChessPos=HIBYTE(CountAssistant.wChessList[CountAssistant.cbCurIndex][wExpandIndex]);
				GetCrossExpand(cbXChessPos,cbYChessPos,CountResult,CountAssistant);
			}
		}
	}

	//������Ŀ
	ASSERT((CountResult.wChessCount[0]+CountResult.wChessCount[1])>=361);
	CountResult.wCommomCount=CountResult.wChessCount[0]+CountResult.wChessCount[1]-361;

	return true;
}

//��������
WORD CGameLogic::PlaceChess(BYTE cbXPos, BYTE cbYPos, BYTE cbColor, WORD wDeadChess[361], WORD & wDeadChessCount)
{
	//Ч�����
	ASSERT((cbXPos<19)&&(cbYPos<19));
	ASSERT(m_cbChessBorad[cbXPos][cbYPos]==NO_CHESS);

	//��������
	tagAreaResult AreaResult;
	memset(&AreaResult,0,sizeof(AreaResult));

	//��������
	m_cbChessBorad[cbXPos][cbYPos]=cbColor;
	m_wIdentifier[cbXPos][cbYPos]=(WORD)(m_ChessManualInfo.GetCount()+1);

	//�������
	AreaResult.wChessCount=1;
	AreaResult.wChessList[0]=MAKEWORD(cbXPos,cbYPos);
	for (WORD wIndex=0;((wIndex<AreaResult.wChessCount)&&(AreaResult.wBurrowCount==0));wIndex++)
	{
		BYTE cbXChessPos=LOBYTE(AreaResult.wChessList[wIndex]);
		BYTE cbYChessPos=HIBYTE(AreaResult.wChessList[wIndex]);
		GetCrossArea(cbXChessPos,cbYChessPos,AreaResult);
	}

	//��ȡ����
	wDeadChessCount=TakeOutChess(cbXPos,cbYPos,wDeadChess);

	//��Ѩ�ж�
	if ((AreaResult.wBurrowCount==0)&&(wDeadChessCount==0))
	{
		m_cbChessBorad[cbXPos][cbYPos]=NO_CHESS;
		return 0;
	}

	//����ж�
	if (memcmp(m_cbLastChessBorad[cbColor-1],m_cbChessBorad,sizeof(m_cbChessBorad))==0) 
	{
		m_cbChessBorad[cbXPos][cbYPos]=NO_CHESS;
		return 0xFFFF;
	}

	//��¼״̬
	CopyMemory(m_cbLastChessBorad[cbColor-1],m_cbChessBorad,sizeof(m_cbChessBorad));

	//״̬����
	m_wStepCount[(cbColor==BLACK_CHESS)?0:1]++;
	m_wTakeChessCount[(cbColor==BLACK_CHESS)?0:1]+=wDeadChessCount;

	//��������
	tagChessManual ChessManual;
	ChessManual.cbXPos=cbXPos;
	ChessManual.cbYPos=cbYPos;
	ChessManual.cbColor=cbColor;
	ChessManual.wDeadChessCount=wDeadChessCount;

	//��¼����
	m_ChessManualInfo.Add(ChessManual);
	if (wDeadChessCount>0)
	{
		//���ô�С
		INT_PTR nChessItemCount=m_DeadChessInfo.GetCount();
		m_DeadChessInfo.SetSize(nChessItemCount+wDeadChessCount);

		//��������
		tagChessItem ChessItem;
		memset(&ChessItem,0,sizeof(ChessItem));

		//��������
		for (WORD i=0;i<wDeadChessCount;i++)
		{
			ChessItem.cbXPos=LOBYTE(wDeadChess[i]);
			ChessItem.cbYPos=HIBYTE(wDeadChess[i]);
			ChessItem.wIdentifier=m_wIdentifier[ChessItem.cbXPos][ChessItem.cbYPos];
			CopyMemory(&m_DeadChessInfo[nChessItemCount+i],&ChessItem,sizeof(ChessItem));
		}
	}

	return m_wIdentifier[cbXPos][cbYPos];
}

//���Ӵ���
WORD CGameLogic::TakeOutChess(BYTE cbXPos, BYTE cbYPos, WORD wDeadChess[361])
{
	//Ч�����
	ASSERT((cbXPos<19)&&(cbYPos<19));
	ASSERT(m_cbChessBorad[cbXPos][cbYPos]!=NO_CHESS);

	//��������
	WORD wDeadChessCount=0;
	tagAreaResult AreaResult;
	memset(&AreaResult,0,sizeof(AreaResult));

	//���ӷ���
	bool bAnalyse[4]={true,true,true,true};
	const int nExcursion[4][2]={{1,0},{0,1},{-1,0},{0,-1}};
	for (int i=0;i<CountArray(nExcursion);i++)
	{
		//�����ų�
		if (bAnalyse[i]==false)	continue;

		//��������
		BYTE cbXStationPos=cbXPos+nExcursion[i][0];
		BYTE cbYStationPos=cbYPos+nExcursion[i][1];

		//���ӷ���
		if ((cbXStationPos>=19)|(cbYStationPos>=19)) continue;
		if ((m_cbChessBorad[cbXStationPos][cbYStationPos]==NO_CHESS)||
			(m_cbChessBorad[cbXStationPos][cbYStationPos]==m_cbChessBorad[cbXPos][cbYPos])) continue;

		//��Ѩ����
		memset(&AreaResult,0,sizeof(AreaResult));
		GetChessArea(cbXStationPos,cbYStationPos,AreaResult);

		//ͬ���ж�
		for (int j=(i+1);j<CountArray(nExcursion);j++)
		{
			if (bAnalyse[j]==true)
			{
				BYTE cbXStationPos=cbXPos+nExcursion[j][0];
				BYTE cbYStationPos=cbYPos+nExcursion[j][1];
				if ((cbXStationPos>=19)|(cbYStationPos>=19)) bAnalyse[j]=false;
				else if (AreaResult.cbAreaResult[cbXStationPos][cbYStationPos]==AREA_CHESS) bAnalyse[j]=false;
			}
		}

		//�ϲ�����
		if (AreaResult.wBurrowCount==0)
		{
			ASSERT((wDeadChessCount+AreaResult.wChessCount)<=361);
			CopyMemory(wDeadChess+wDeadChessCount,AreaResult.wChessList,AreaResult.wChessCount*sizeof(AreaResult.wChessList[0]));
			wDeadChessCount+=AreaResult.wChessCount;
		}
	}

	//ɾ������
	for (WORD wIndex=0;wIndex<wDeadChessCount;wIndex++)
	{
		BYTE cbXChessPos=LOBYTE(wDeadChess[wIndex]);
		BYTE cbYChessPos=HIBYTE(wDeadChess[wIndex]);
		m_cbChessBorad[cbXChessPos][cbYChessPos]=NO_CHESS;
	}

	return wDeadChessCount;
}

//��ȡ����
void CGameLogic::GetChessArea(BYTE cbXPos, BYTE cbYPos, tagAreaResult & AreaResult)
{
	//Ч�����
	ASSERT((cbXPos<19)&&(cbYPos<19));
	ASSERT(m_cbChessBorad[cbXPos][cbYPos]!=NO_CHESS);

	//���ñ���
	AreaResult.wChessCount=1;
	AreaResult.wBurrowCount=0;
	AreaResult.wChessList[0]=MAKEWORD(cbXPos,cbYPos);
	AreaResult.cbAreaResult[cbXPos][cbYPos]=AREA_CHESS;
	
	//����ͳ��
	for (WORD wIndex=0;wIndex<AreaResult.wChessCount;wIndex++)
	{
		BYTE cbXChessPos=LOBYTE(AreaResult.wChessList[wIndex]);
		BYTE cbYChessPos=HIBYTE(AreaResult.wChessList[wIndex]);
		GetCrossArea(cbXChessPos,cbYChessPos,AreaResult);
	}

	return;
}

//ʮ������
void CGameLogic::GetCrossArea(BYTE cbXPos, BYTE cbYPos, tagAreaResult & AreaResult)
{
	//Ч�����
	ASSERT((cbXPos<19)&&(cbYPos<19));
	ASSERT(m_cbChessBorad[cbXPos][cbYPos]!=NO_CHESS);

	//��������
	BYTE cbChessColor=m_cbChessBorad[cbXPos][cbYPos];
	BYTE cbOtherChessColor=(cbChessColor==BLACK_CHESS)?WHITE_CHESS:BLACK_CHESS;

	//�������
	const int nExcursion[4][2]={{1,0},{0,1},{-1,0},{0,-1}};
	for (int i=0;i<CountArray(nExcursion);i++)
	{
		//��������
		BYTE cbXStationPos=cbXPos+nExcursion[i][0];
		BYTE cbYStationPos=cbYPos+nExcursion[i][1];

		//�������
		for (;((cbXStationPos<19)&&(cbYStationPos<19));cbXStationPos+=nExcursion[i][0],cbYStationPos+=nExcursion[i][1])
		{
			//�������
			if (m_cbChessBorad[cbXStationPos][cbYStationPos]==cbOtherChessColor) break;
			if (AreaResult.cbAreaResult[cbXStationPos][cbYStationPos]!=AREA_UNKNOW) break;

			//�Լ�����
			if (m_cbChessBorad[cbXStationPos][cbYStationPos]==cbChessColor)	
			{
				AreaResult.cbAreaResult[cbXStationPos][cbYStationPos]=AREA_CHESS;
				AreaResult.wChessList[AreaResult.wChessCount++]=MAKEWORD(cbXStationPos,cbYStationPos);
			}
			else //��Ѩ����
			{
				AreaResult.wBurrowCount++;
				AreaResult.cbAreaResult[cbXStationPos][cbYStationPos]=AREA_BURROW;
				break;
			}
		}
	}

	return;
}

//ʮ������
void CGameLogic::GetCrossExpand(BYTE cbXPos, BYTE cbYPos, tagCountResult & CountResult, tagCountAssistant & CountAssistant)
{
	//Ч�����
	ASSERT((cbXPos<19)&&(cbYPos<19));

	//���ŷ���
	const int nExcursion[4][2]={{1,0},{0,1},{-1,0},{0,-1}};
	for (int i=0;i<CountArray(nExcursion);i++)
	{
		//��������
		BYTE cbXStationPos=cbXPos+nExcursion[i][0];
		BYTE cbYStationPos=cbYPos+nExcursion[i][1];

		//�������
		for (;((cbXStationPos<19)&&(cbYStationPos<19));cbXStationPos+=nExcursion[i][0],cbYStationPos+=nExcursion[i][1])
		{
			//���Ź���
			if (m_cbChessBorad[cbXStationPos][cbYStationPos]!=NO_CHESS) break;
			if ((CountResult.cbCountInfo[cbXStationPos][cbYStationPos]&CountAssistant.cbChessMask)!=0) break;

			//���Ŵ���
			BYTE cbCurIndex=CountAssistant.cbCurIndex;
			CountResult.wChessCount[CountAssistant.cbCurIndex]++;
			CountResult.cbCountInfo[cbXStationPos][cbYStationPos]|=CountAssistant.cbChessMask;
			CountAssistant.wChessList[cbCurIndex][CountAssistant.wChessCount[cbCurIndex]++]=MAKEWORD(cbXStationPos,cbYStationPos);
		}
	}

	return;
}

//////////////////////////////////////////////////////////////////////////

