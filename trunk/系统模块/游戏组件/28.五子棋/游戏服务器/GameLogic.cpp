#include "StdAfx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameLogic::CGameLogic()
{
	//���ñ���
	memset(m_cbStepCount,0,sizeof(m_cbStepCount));
	memset(m_cbIdentifier,0,sizeof(m_cbIdentifier));
	memset(m_cbChessBorad,0,sizeof(m_cbChessBorad));

	return;
}

//��������
CGameLogic::~CGameLogic()
{
}

//��ȡ����
BYTE CGameLogic::GetStepCount(BYTE cbColor)
{
	ASSERT((cbColor==BLACK_CHESS)||(cbColor==WHITE_CHESS));
	return m_cbStepCount[(cbColor==BLACK_CHESS)?0:1];
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
	//��������
	m_ChessManualInfo.RemoveAll();

	//���ӱ���
	memset(m_cbStepCount,0,sizeof(m_cbStepCount));
	memset(m_cbIdentifier,0,sizeof(m_cbIdentifier));
	memset(m_cbChessBorad,0,sizeof(m_cbChessBorad));

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

		//���̱���
		m_cbStepCount[(pChessManual->cbColor==BLACK_CHESS)?0:1]--;
		m_cbChessBorad[pChessManual->cbXPos][pChessManual->cbYPos]=NO_CHESS;
	}

	//ɾ������
	m_ChessManualInfo.RemoveAt(nManualCount-wStepCount,wStepCount);

	return true;
}

//��������
BYTE CGameLogic::PlaceChess(BYTE cbXPos, BYTE cbYPos, BYTE cbColor)
{
	//Ч�����
	ASSERT((cbXPos<15)&&(cbYPos<15));
	ASSERT(m_cbChessBorad[cbXPos][cbYPos]==NO_CHESS);
	if ((cbXPos>=15)||(cbYPos>=15)) return false;
	if (m_cbChessBorad[cbXPos][cbYPos]!=NO_CHESS) return false;

	//�����ж�
	if (cbColor==BLACK_CHESS)
	{
	}

	//��������
	m_cbChessBorad[cbXPos][cbYPos]=cbColor;
	m_cbStepCount[(cbColor==BLACK_CHESS)?0:1]++;
	m_cbIdentifier[cbXPos][cbYPos]=(BYTE)(m_ChessManualInfo.GetCount()+1);

	//��¼����
	tagChessManual ChessManual;
	ChessManual.cbXPos=cbXPos;
	ChessManual.cbYPos=cbYPos;
	ChessManual.cbColor=cbColor;
	m_ChessManualInfo.Add(ChessManual);

	return m_cbIdentifier[cbXPos][cbYPos];
}

//ʤ���ж�
bool CGameLogic::WinEstimate(BYTE cbXPos, BYTE cbYPos)
{
	//��������
	int nMoveWay[4][2]={{1,0},{0,1},{1,-1},{1,1}};
	BYTE cbChess=m_cbChessBorad[cbXPos][cbYPos];
	BYTE cbXPosTemp=0,cbYPosTemp=0,cbChessCount=0;

	//ʤ���ж�
	for (int i=0;i<sizeof(nMoveWay)/sizeof(nMoveWay[0]);i++)
	{
		//���ñ���
		cbChessCount=0;

		//�������
		cbXPosTemp=cbXPos;
		cbYPosTemp=cbYPos;
		do
		{
			//�ƶ����
			cbXPosTemp+=nMoveWay[i][0];
			cbYPosTemp+=nMoveWay[i][1];
			if ((cbXPosTemp>=15)||(cbYPosTemp>=15)) break;
			if (m_cbChessBorad[cbXPosTemp][cbYPosTemp]!=cbChess) break;

			//�ۼ�����
			cbChessCount++;
			if (cbChessCount==4) return true;

		} while (true);

		//�������
		cbXPosTemp=cbXPos;
		cbYPosTemp=cbYPos;
		do
		{
			//�ƶ����
			cbXPosTemp-=nMoveWay[i][0];
			cbYPosTemp-=nMoveWay[i][1];
			if ((cbXPosTemp>=15)||(cbYPosTemp>=15)) break;
			if (m_cbChessBorad[cbXPosTemp][cbYPosTemp]!=cbChess) break;

			//�ۼ�����
			cbChessCount++;
			if (cbChessCount==4) return true;

		} while (true);
	};

	return false;
}

//////////////////////////////////////////////////////////////////////////
