#include "StdAfx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameLogic::CGameLogic()
{
	CString strFile,strTemp;
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	strFile.Format("log\\%sCGameClientDlg.log",strTime);

	strTemp.Format("into CGameFrameDlg()");
	WriteLog(strFile, strTemp);

	//��������
	memset(m_pItemLately,0,sizeof(m_pItemLately));

	//���̱���
	memset(m_ChessItem,0,sizeof(m_ChessItem));
	memset(m_wStepCount,0,sizeof(m_wStepCount));
	memset(m_ChessBorad,0,sizeof(m_ChessBorad));

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
	return m_wStepCount[GetColorIndex(cbColor)];
}

//��ȡ����
const tagChessManual * CGameLogic::GetLastChessManual()
{
	INT_PTR nManualCount=m_ChessManualInfo.GetCount();
	return (nManualCount>0)?&m_ChessManualInfo[nManualCount-1]:NULL;
}

//��ȡ����
const tagChessItem * CGameLogic::GetChessItem(BYTE cbXPos, BYTE cbYPos)
{
	//Ч�����
	ASSERT((cbXPos<8)&&(cbYPos<8));
	return m_ChessBorad[cbXPos][cbYPos];
}

//��������
void CGameLogic::ResetChessBorad()
{
	//��������
	BYTE cbChessInfo[8]={CHESS_ROOK,CHESS_HORSE,CHESS_ELEPHANT,CHESS_KING,CHESS_QUEEN,
		CHESS_ELEPHANT,CHESS_HORSE,CHESS_ROOK};

	//���ñ���
	m_ChessManualInfo.RemoveAll();
	memset(m_ChessItem,0,sizeof(m_ChessItem));
	memset(m_wStepCount,0,sizeof(m_wStepCount));
	memset(m_ChessBorad,0,sizeof(m_ChessBorad));

	//��������
	BYTE cbYPos=255;
	for (BYTE cbUser=0;cbUser<2;cbUser++)
	{
		for (BYTE nIndex=0;nIndex<8;nIndex++)
		{
			//��������
			cbYPos=(cbUser==0)?7:0;
			m_ChessItem[cbUser][nIndex].cbYPos=cbYPos;
			m_ChessItem[cbUser][nIndex].cbXPos=nIndex;
			m_ChessItem[cbUser][nIndex].cbChessID=nIndex;
			m_ChessItem[cbUser][nIndex].cbChess=cbChessInfo[nIndex];
			m_ChessItem[cbUser][nIndex].cbColor=BLACK_CHESS+cbUser;
			m_ChessBorad[nIndex][cbYPos]=&m_ChessItem[cbUser][nIndex];

			//��������
			cbYPos=(cbUser==0)?6:1;
			m_ChessItem[cbUser][nIndex+8].cbYPos=cbYPos;
			m_ChessItem[cbUser][nIndex+8].cbXPos=nIndex;
			m_ChessItem[cbUser][nIndex+8].cbChessID=nIndex+8;
			m_ChessItem[cbUser][nIndex+8].cbChess=CHESS_SOLDIER;
			m_ChessItem[cbUser][nIndex+8].cbColor=BLACK_CHESS+cbUser;
			m_ChessBorad[nIndex][cbYPos]=&m_ChessItem[cbUser][nIndex+8];
		}
	}

	return;
}

//��������
void CGameLogic::UpdateChessBorad()
{
	//��������
	memset(m_ChessBorad,0,sizeof(m_ChessBorad));
	memset(m_pItemLately,0,sizeof(m_pItemLately));

	//��������
	for (BYTE i=0;i<2;i++)
	{
		for (BYTE j=0;j<16;j++)
		{
			if ((m_ChessItem[i][j].cbXPos!=255)&&(m_ChessItem[i][j].cbYPos!=255))
			{
				ASSERT(m_ChessItem[i][j].cbXPos<8);
				ASSERT(m_ChessItem[i][j].cbYPos<8);
				m_ChessBorad[m_ChessItem[i][j].cbXPos][m_ChessItem[i][j].cbYPos]=&m_ChessItem[i][j];
			}
		}
	}

	//�������
	tagChessManual * pChessManual=NULL;
	INT_PTR nManualCount=m_ChessManualInfo.GetCount(),nIndex=0;
	if (nManualCount>=1)
	{
		nIndex=(nManualCount-1)%2;
		pChessManual=&m_ChessManualInfo[nManualCount-1];
		m_pItemLately[nIndex]=&m_ChessItem[nIndex][pChessManual->cbSourceID];
	}
	if (nManualCount>=2)
	{
		nIndex=nManualCount%2;
		pChessManual=&m_ChessManualInfo[nManualCount-2];
		m_pItemLately[nIndex]=&m_ChessItem[nIndex][pChessManual->cbSourceID];
	}

	return;
}

//��������
void CGameLogic::InsertChessManual(tagChessManual ChessManual[], WORD wManualCount)
{
	INT_PTR nScrCount=m_ChessManualInfo.GetCount();
	m_ChessManualInfo.SetSize(nScrCount+wManualCount);
	for (WORD i=0;i<wManualCount;i++) m_ChessManualInfo[nScrCount+i]=ChessManual[i];

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

		//��ȡ����
		tagChessItem * pSourceChessItem=&m_ChessItem[GetColorIndex(pChessManual->cbSourceColor)][pChessManual->cbSourceID];
		tagChessItem * pTargetChessItem=(pChessManual->cbUniteChess!=NO_CHESS)?pTargetChessItem=&m_ChessItem[GetColorIndex(pChessManual->cbUniteColor)][pChessManual->cbUniteID]:NULL;

		//Ч������
		ASSERT(pSourceChessItem->cbXPos!=255);
		ASSERT(pSourceChessItem->cbYPos!=255);

		//���ò���
		m_wStepCount[GetColorIndex(pChessManual->cbSourceColor)]--;

		//����Դ��
		pSourceChessItem->wWalkCount--;
		pSourceChessItem->cbChess=pChessManual->cbSourceChess;
		pSourceChessItem->cbXPos=pChessManual->cbXSourceChessPos;
		pSourceChessItem->cbYPos=pChessManual->cbYSourceChessPos;

		//��������
		if (pTargetChessItem!=NULL)
		{
			pTargetChessItem->cbXPos=pChessManual->cbXUniteChessPos;
			pTargetChessItem->cbYPos=pChessManual->cbYUniteChessPos;
			if (pSourceChessItem->cbColor==pTargetChessItem->cbColor) pTargetChessItem->wWalkCount--;
		}
	}

	//ɾ������
	m_ChessManualInfo.RemoveAt(nManualCount-wStepCount,wStepCount);

	//��������
	UpdateChessBorad();

	return true;
}

//�ƶ�����
enMoveType CGameLogic::MoveChess(BYTE cbXSourcePos, BYTE cbYSourcePos, BYTE cbXTargetPos, BYTE cbYTargetPos, BYTE cbSwitchChess)
{
	//Ч������
	ASSERT((cbXSourcePos<8)&&(cbYSourcePos<8));
	ASSERT((cbXTargetPos<8)&&(cbYTargetPos<8));

	//��ȡ����
	tagChessItem * pSourceChessItem=m_ChessBorad[cbXSourcePos][cbYSourcePos];
	tagChessItem * pTargetChessItem=m_ChessBorad[cbXTargetPos][cbYTargetPos];

	//���ױ���
	tagChessManual ChessManual;
	memset(&ChessManual,0,sizeof(ChessManual));
	ChessManual.cbXSourceChessPos=cbXSourcePos;
	ChessManual.cbYSourceChessPos=cbYSourcePos;
	ChessManual.cbXTargetChessPos=cbXTargetPos;
	ChessManual.cbYTargetChessPos=cbYTargetPos;
	ChessManual.cbSourceID=pSourceChessItem->cbChessID;
	ChessManual.cbSourceColor=pSourceChessItem->cbColor;
	ChessManual.cbSourceChess=pSourceChessItem->cbChess;

	//��������
	enMoveType MoveChessType=enMoveType_Normal;
	if ((pSourceChessItem->cbChess==CHESS_SOLDIER)&&((cbYTargetPos==0)||(cbYTargetPos==7)))
	{
		if ((cbSwitchChess<CHESS_QUEEN)||(cbSwitchChess>CHESS_HORSE)) return enMoveType_Error;
		pSourceChessItem->cbChess=cbSwitchChess;
		ChessManual.cbSwitchChess=cbSwitchChess;
		MoveChessType=enMoveType_SoldierUp;
	}
	else 
	{
		cbSwitchChess=NO_CHESS;
		ChessManual.cbSwitchChess=cbSwitchChess;
	}

	//������λ
	if ((pTargetChessItem!=NULL)&&(pTargetChessItem->cbColor==pSourceChessItem->cbColor))
	{
		//��ȡ����
		tagChessItem * pKingChessItem=(pTargetChessItem->cbChess==CHESS_KING)?pTargetChessItem:pSourceChessItem;
		tagChessItem * pRookChessItem=(pTargetChessItem->cbChess==CHESS_ROOK)?pTargetChessItem:pSourceChessItem;

		//λ�ö���
		BYTE cbXKingPos=pKingChessItem->cbXPos;
		BYTE cbXRookPos=pRookChessItem->cbXPos;
		BYTE cbXKingExcursion=(cbXKingPos>cbXRookPos)?-2:2;
		BYTE cbXRookExcursion=(cbXKingPos>cbXRookPos)?1:-1;

		//��¼����
		ChessManual.cbUniteID=pTargetChessItem->cbChessID;
		ChessManual.cbUniteColor=pTargetChessItem->cbColor;
		ChessManual.cbUniteChess=pTargetChessItem->cbChess;
		ChessManual.cbXUniteChessPos=pTargetChessItem->cbXPos;
		ChessManual.cbYUniteChessPos=pTargetChessItem->cbYPos;
		m_ChessManualInfo.Add(ChessManual);

		//��������
		pKingChessItem->wWalkCount++;
		pRookChessItem->wWalkCount++;
		pKingChessItem->cbXPos+=cbXKingExcursion;
		pRookChessItem->cbXPos=pKingChessItem->cbXPos+cbXRookExcursion;

		//��������
		m_ChessBorad[cbXKingPos][pKingChessItem->cbYPos]=NULL;
		m_ChessBorad[cbXRookPos][pRookChessItem->cbYPos]=NULL;
		m_ChessBorad[pKingChessItem->cbXPos][pKingChessItem->cbYPos]=pKingChessItem;
		m_ChessBorad[pRookChessItem->cbXPos][pRookChessItem->cbYPos]=pRookChessItem;

		return enMoveType_KingRook;
	}

	//�����ж�
	if (pTargetChessItem!=NULL)
	{
		//��¼����
		ChessManual.cbUniteID=pTargetChessItem->cbChessID;
		ChessManual.cbUniteColor=pTargetChessItem->cbColor;
		ChessManual.cbUniteChess=pTargetChessItem->cbChess;
		ChessManual.cbXUniteChessPos=pTargetChessItem->cbXPos;
		ChessManual.cbYUniteChessPos=pTargetChessItem->cbYPos;

		//��������
		pTargetChessItem->cbXPos=255;
		pTargetChessItem->cbYPos=255;
		m_ChessBorad[cbXTargetPos][cbYTargetPos]=NULL;
	}

	//��·���ж�
	if ((pTargetChessItem==NULL)&&(pSourceChessItem->cbChess==CHESS_SOLDIER)&&(cbXSourcePos!=cbXTargetPos))
	{
		//��ȡ����
		tagChessItem * pEnemyChessItem=m_ChessBorad[cbXTargetPos][cbYSourcePos];
		ASSERT(pEnemyChessItem!=NULL);

		//��¼����
		ChessManual.cbUniteID=pEnemyChessItem->cbChessID;
		ChessManual.cbUniteColor=pEnemyChessItem->cbColor;
		ChessManual.cbUniteChess=pEnemyChessItem->cbChess;
		ChessManual.cbXUniteChessPos=pEnemyChessItem->cbXPos;
		ChessManual.cbYUniteChessPos=pEnemyChessItem->cbYPos;

		//��������
		pEnemyChessItem->cbXPos=255;
		pEnemyChessItem->cbYPos=255;
		m_ChessBorad[cbXTargetPos][cbYSourcePos]=NULL;
	}

	//��¼����
	m_ChessManualInfo.Add(ChessManual);
	m_wStepCount[pSourceChessItem->cbColor-1]++;
	m_pItemLately[pSourceChessItem->cbColor-1]=pSourceChessItem;

	//�ƶ�����
	pSourceChessItem->wWalkCount++;
	pSourceChessItem->cbXPos=cbXTargetPos;
	pSourceChessItem->cbYPos=cbYTargetPos;
	m_ChessBorad[cbXSourcePos][cbYSourcePos]=NULL;
	m_ChessBorad[cbXTargetPos][cbYTargetPos]=pSourceChessItem;

	return MoveChessType;
}

//�����ж�
bool CGameLogic::IsGameFinish(BYTE cbColor)
{
	//��ȡ����
	tagChessItem * pKindChessItem=&m_ChessItem[(cbColor==BLACK_CHESS)?0:1][3];

	//�����ж�
	return ((pKindChessItem->cbXPos==255)&&(pKindChessItem->cbYPos==255));
}

//ɱ���ж�
bool CGameLogic::IsKingDanger(BYTE cbColor)
{
	//��ȡ����
	tagChessItem * pKindChessItem=&m_ChessItem[(cbColor==BLACK_CHESS)?0:1][3];

	//��ȡ����
	ASSERT(pKindChessItem->cbXPos!=255);
	ASSERT(pKindChessItem->cbYPos!=255);

	//Σ���ж�
	return IsChessDanger(pKindChessItem->cbXPos,pKindChessItem->cbYPos);
}

//Σ���ж�
bool CGameLogic::IsChessDanger(BYTE cbXTargetPos, BYTE cbYTargetPos)
{
	//Ч�����
	ASSERT((cbXTargetPos<8)&&(cbYTargetPos<8));

	//��ȡ����
	tagChessItem * pTargetChessItem=m_ChessBorad[cbXTargetPos][cbYTargetPos];
	if (pTargetChessItem==NULL) return false;

	//��������
	BYTE cbEnemyIndex=(pTargetChessItem->cbColor==BLACK_CHESS)?1:0;

	//��������
	BYTE cbXSourcePos=255,cbYSourcePos=255;
	for (BYTE i=0;i<CountArray(m_ChessItem[cbEnemyIndex]);i++)
	{
		//��ȡ����
		tagChessItem * pSourceChessItem=&m_ChessItem[cbEnemyIndex][i];
		if ((pSourceChessItem->cbXPos==255)||(pSourceChessItem->cbYPos==255)) continue;

		//��ȡ����
		cbXSourcePos=pSourceChessItem->cbXPos;
		cbYSourcePos=pSourceChessItem->cbYPos;

		//·������
		switch (pSourceChessItem->cbChess)
		{
		case CHESS_KING:	//��
			{
				//ƫ�Ʊ���
				BYTE cbXExcursion=GetExcursion(cbXSourcePos,cbXTargetPos);
				BYTE cbYExcursion=GetExcursion(cbYSourcePos,cbYTargetPos);

				//�����ж�
				if ((cbXExcursion<=1)&&(cbYExcursion<=1)) return true;

				break;
			}
		case CHESS_QUEEN:	//��
			{
				//ƫ�Ʊ���
				BYTE cbXExcursion=GetExcursion(cbXSourcePos,cbXTargetPos);
				BYTE cbYExcursion=GetExcursion(cbYSourcePos,cbYTargetPos);

				//�����ж�
				if ((cbXSourcePos!=cbXTargetPos)&&(cbYSourcePos!=cbYTargetPos)&&(cbXExcursion!=cbYExcursion)) break;

				//��������
				if (ObstructAnalyse(cbXSourcePos,cbYSourcePos,cbXTargetPos,cbYTargetPos)==false) return true;

				break;
			}
		case CHESS_ROOK:	//��
			{
				//�����ж�
				if ((cbXSourcePos!=cbXTargetPos)&&(cbYSourcePos!=cbYTargetPos)) break;

				//��������
				if (ObstructAnalyse(cbXSourcePos,cbYSourcePos,cbXTargetPos,cbYTargetPos)==false) return true;

				break;
			}
		case CHESS_ELEPHANT://��
			{
				//ƫ�Ʊ���
				BYTE cbXExcursion=GetExcursion(cbXSourcePos,cbXTargetPos);
				BYTE cbYExcursion=GetExcursion(cbYSourcePos,cbYTargetPos);

				//�����ж�
				if ((cbXExcursion==0)||(cbYExcursion==0)||(cbXExcursion!=cbYExcursion)) break;

				//��������
				if (ObstructAnalyse(cbXSourcePos,cbYSourcePos,cbXTargetPos,cbYTargetPos)==false) return true;

				break;
			}
		case CHESS_HORSE:	//��
			{
				//ƫ�Ʊ���
				BYTE cbXExcursion=GetExcursion(cbXSourcePos,cbXTargetPos);
				BYTE cbYExcursion=GetExcursion(cbYSourcePos,cbYTargetPos);

				//�����ж�
				if ((cbXExcursion==1)&&(cbYExcursion==2)) return true;
				if ((cbXExcursion==2)&&(cbYExcursion==1)) return true;

				break;
			}
		case CHESS_SOLDIER:	//��
			{
				//��������
				int nYStep=(pSourceChessItem->cbColor==BLACK_CHESS)?-1:1;
				BYTE cbXExcursion=GetExcursion(cbXSourcePos,cbXTargetPos);

				//�����ж�
				if (cbXExcursion==1)
				{
					if ((cbYSourcePos+nYStep)==cbYTargetPos) return true;
					if ((cbYSourcePos==cbYTargetPos)&&(pTargetChessItem->cbChess==CHESS_SOLDIER)&&(pTargetChessItem->wWalkCount==1)) return true;
				}

				break;
			}
		}
	}

	return false;
}

//Ч�鲽��
bool CGameLogic::IsWalkLegality(BYTE cbXSourcePos, BYTE cbYSourcePos, BYTE cbXTargetPos, BYTE cbYTargetPos)
{
	//Ч������
	ASSERT((cbXSourcePos<8)&&(cbYSourcePos<8));
	ASSERT((cbXTargetPos<8)&&(cbYTargetPos<8));

	//��ȡ����
	tagChessItem * pSourceChessItem=m_ChessBorad[cbXSourcePos][cbYSourcePos];
	tagChessItem * pTargetChessItem=m_ChessBorad[cbXTargetPos][cbYTargetPos];

	//Ч�����
	ASSERT(pSourceChessItem!=NULL);
	if (pSourceChessItem==NULL) return false;

	//������λ
	if ((pTargetChessItem!=NULL)&&(pTargetChessItem->cbColor==pSourceChessItem->cbColor))
	{
		//�����ж�
		if ((pTargetChessItem->wWalkCount!=0)&&(pSourceChessItem->wWalkCount!=0)) return false;

		//�����ж�
		tagChessItem * pKingChessItem=NULL;
		if (pTargetChessItem->cbChess==CHESS_KING) pKingChessItem=pTargetChessItem;
		else if (pSourceChessItem->cbChess==CHESS_KING) pKingChessItem=pTargetChessItem;
		else return false;

		//�����ж�
		tagChessItem * pRookChessItem=NULL;
		if (pTargetChessItem->cbChess==CHESS_ROOK) pKingChessItem=pTargetChessItem;
		else if (pSourceChessItem->cbChess==CHESS_ROOK) pKingChessItem=pTargetChessItem;
		else return false;

		//�赲�ж�
		return (ObstructAnalyse(cbXSourcePos,cbYSourcePos,cbXTargetPos,cbYTargetPos)==false);
	}

    //�����ж�
	switch (pSourceChessItem->cbChess)
	{
	case CHESS_KING:	//��
		{
			//ƫ�Ʊ���
			BYTE cbXExcursion=GetExcursion(cbXSourcePos,cbXTargetPos);
			BYTE cbYExcursion=GetExcursion(cbYSourcePos,cbYTargetPos);

			//�����ж�
			if ((cbXExcursion>1)||(cbYExcursion>1)) return false;

			return true;
		}
	case CHESS_QUEEN:	//��
		{
			//ƫ�Ʊ���
			BYTE cbXExcursion=GetExcursion(cbXSourcePos,cbXTargetPos);
			BYTE cbYExcursion=GetExcursion(cbYSourcePos,cbYTargetPos);

			//�����ж�
			if ((cbXSourcePos!=cbXTargetPos)&&(cbYSourcePos!=cbYTargetPos)&&(cbXExcursion!=cbYExcursion)) return false;

			//��������
			return (ObstructAnalyse(cbXSourcePos,cbYSourcePos,cbXTargetPos,cbYTargetPos)==false);
		}
	case CHESS_ROOK:	//��
		{
			//�����ж�
			if ((cbXSourcePos!=cbXTargetPos)&&(cbYSourcePos!=cbYTargetPos)) return false;

			//��������
			return (ObstructAnalyse(cbXSourcePos,cbYSourcePos,cbXTargetPos,cbYTargetPos)==false);
		}
	case CHESS_ELEPHANT://��
		{
			//ƫ�Ʊ���
			BYTE cbXExcursion=GetExcursion(cbXSourcePos,cbXTargetPos);
			BYTE cbYExcursion=GetExcursion(cbYSourcePos,cbYTargetPos);

			//�����ж�
			if ((cbXExcursion==0)||(cbYExcursion==0)||(cbXExcursion!=cbYExcursion)) return false;

			//��������
			return (ObstructAnalyse(cbXSourcePos,cbYSourcePos,cbXTargetPos,cbYTargetPos)==false);
		}
	case CHESS_HORSE:	//��
		{
			//ƫ�Ʊ���
			BYTE cbXExcursion=GetExcursion(cbXSourcePos,cbXTargetPos);
			BYTE cbYExcursion=GetExcursion(cbYSourcePos,cbYTargetPos);

			//�����ж�
			if ((cbXExcursion==1)&&(cbYExcursion==2)) return true;
			if ((cbXExcursion==2)&&(cbYExcursion==1)) return true;

			return false;
		}
	case CHESS_SOLDIER:	//��
		{
			//��������
			int nYStep=(pSourceChessItem->cbColor==BLACK_CHESS)?-1:1;

			//ֱ���ж�
			if (cbXSourcePos==cbXTargetPos)
			{
				if (pTargetChessItem!=NULL) return false;
				if ((cbYSourcePos+nYStep)==cbYTargetPos) return true;
				if ((pSourceChessItem->wWalkCount==0)&&((cbYSourcePos+nYStep*2)==cbYTargetPos)
					&&(m_ChessBorad[cbXSourcePos][cbYSourcePos+nYStep]==NULL)) return true;
				return false;
			}

			//б���ж�
			if ((cbYSourcePos+nYStep)!=cbYTargetPos) return false;
			if ((GetExcursion(cbXSourcePos,cbXTargetPos))!=1) return false;

			//�����ж�
			if ((pTargetChessItem!=NULL)&&(pTargetChessItem->cbColor!=pSourceChessItem->cbColor)) return true;

			//��·���ж�
			BYTE cbYTempPos=(pSourceChessItem->cbColor==BLACK_CHESS)?3:4;
			if ((pTargetChessItem==NULL)&&(cbYSourcePos==cbYTempPos))
			{
				tagChessItem * pTempChessItem=m_pItemLately[(pSourceChessItem->cbColor==BLACK_CHESS)?1:0];
				if (pTempChessItem==NULL) return false;
				if (pTempChessItem->wWalkCount!=1) return false;
				if (pTempChessItem->cbXPos!=cbXTargetPos) return false;
				if (pTempChessItem->cbChess!=CHESS_SOLDIER) return false;
				return true;
			}

			return false;
		}
	}

	ASSERT(FALSE);

	return false;
}

//��ȡ����
BYTE CGameLogic::GetColorIndex(BYTE cbChessColor)
{
	return (cbChessColor==BLACK_CHESS)?0:1;
}

//��ȡƫ��
BYTE CGameLogic::GetExcursion(BYTE cbSourcePos, BYTE cbTargetPos)
{
	return (cbSourcePos>cbTargetPos)?(cbSourcePos-cbTargetPos):(cbTargetPos-cbSourcePos);
}

//��������
bool CGameLogic::ObstructAnalyse(BYTE cbXSourcePos, BYTE cbYSourcePos, BYTE cbXTargetPos, BYTE cbYTargetPos)
{
	//Ч������
	ASSERT((cbXSourcePos!=cbXTargetPos)||(cbYSourcePos!=cbYTargetPos));

	//��������
	int nXStep=0,nYStep=0;
	if (cbXTargetPos!=cbXSourcePos) nXStep=(cbXTargetPos>cbXSourcePos)?1:-1;
	if (cbYTargetPos!=cbYSourcePos) nYStep=(cbYTargetPos>cbYSourcePos)?1:-1;

	//��������
	do
	{
		cbXSourcePos+=nXStep;
		cbYSourcePos+=nYStep;
		if ((cbXSourcePos==cbXTargetPos)&&(cbYSourcePos==cbYTargetPos))	return false;
	} while (m_ChessBorad[cbXSourcePos][cbYSourcePos]==NULL);

	return true;
}

//////////////////////////////////////////////////////////////////////////
