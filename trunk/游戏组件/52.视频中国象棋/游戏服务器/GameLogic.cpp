#include "StdAfx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameLogic::CGameLogic()
{
	//���̱���
	ZeroMemory(m_ChessItem,sizeof(m_ChessItem));
	ZeroMemory(m_ChessBorad,sizeof(m_ChessBorad));

	//״̬����
	m_wNullStep=0;
	ZeroMemory(m_CatchChessInfo,sizeof(m_CatchChessInfo));
	ZeroMemory(m_GeneralChessInfo,sizeof(m_GeneralChessInfo));

	return;
}

//��������
CGameLogic::~CGameLogic()
{
}

//��ȡ����
WORD CGameLogic::GetStepCount(BYTE cbColor)
{
	//Ч�����
	ASSERT((cbColor==CHESS_BLACK)||(cbColor==CHESS_WHITE));
	if ((cbColor!=CHESS_BLACK)&&(cbColor!=CHESS_WHITE)) return 0;

	//��ȡ����
	WORD wStepCount=(WORD)m_ChessManualArray.GetCount();
	return (cbColor==CHESS_BLACK)?(wStepCount+1)/2:wStepCount/2;
}

//��ȡ����
tagChessManual * CGameLogic::GetLastChessManual()
{
	INT_PTR nManualCount=m_ChessManualArray.GetCount();
	return (nManualCount>0)?&m_ChessManualArray[nManualCount-1]:NULL;
}

//��ȡ����
tagChessItem * CGameLogic::GetChessItem(BYTE cbXPos, BYTE cbYPos)
{
	//Ч�����
	ASSERT((cbXPos<COUNT_X)&&(cbYPos<COUNT_Y));
	if ((cbXPos>=COUNT_X)||(cbYPos>=COUNT_Y)) return NULL;

	//��ȡ����
	return m_ChessBorad[cbXPos][cbYPos];
}

//��������
VOID CGameLogic::ResetChessBorad()
{
	//��������
	m_ChessManualArray.RemoveAll();

	//���̱���
	ZeroMemory(m_ChessItem,sizeof(m_ChessItem));
	ZeroMemory(m_ChessBorad,sizeof(m_ChessBorad));

	//״̬����
	m_wNullStep=0;
	ZeroMemory(m_CatchChessInfo,sizeof(m_CatchChessInfo));
	ZeroMemory(m_GeneralChessInfo,sizeof(m_GeneralChessInfo));

	//��������
	InitChessBorad(m_ChessItem,m_ChessBorad);

	return;
}

//�������
bool CGameLogic::RegretChess(WORD wTargetStep)
{
	//Ч�����
	ASSERT(wTargetStep<=(WORD)m_ChessManualArray.GetCount());
	if (wTargetStep>m_ChessManualArray.GetCount()) return false;

	//���̱���
	ZeroMemory(m_ChessItem,sizeof(m_ChessItem));
	ZeroMemory(m_ChessBorad,sizeof(m_ChessBorad));

	//״̬����
	m_wNullStep=0;
	ZeroMemory(m_CatchChessInfo,sizeof(m_CatchChessInfo));
	ZeroMemory(m_GeneralChessInfo,sizeof(m_GeneralChessInfo));

	//��������
	InitChessBorad(m_ChessItem,m_ChessBorad);

	//��������
	for (INT_PTR i=0;i<wTargetStep;i++)
	{
		//��������
		tagChessManual * pChessManual=&m_ChessManualArray[i];
		BYTE cbXSourcePos=pChessManual->cbXSourceChessPos,cbYSourcePos=pChessManual->cbYSourceChessPos;
		BYTE cbXTargetPos=pChessManual->cbXTargetChessPos,cbYTargetPos=pChessManual->cbYTargetChessPos;

		//�������
		tagMoveChessResult MoveChessResult;
		ZeroMemory(&MoveChessResult,sizeof(MoveChessResult));

		//�ƶ�����
		if (PerformMoveChess(cbXSourcePos,cbYSourcePos,cbXTargetPos,cbYTargetPos,MoveChessResult)==false)
		{
			ASSERT(FALSE);
			return false;
		}
	}

	//ɾ������
	m_ChessManualArray.SetSize(wTargetStep);

	return true;
}

//�ƶ�����
bool CGameLogic::MoveChess(BYTE cbXSourcePos, BYTE cbYSourcePos, BYTE cbXTargetPos, BYTE cbYTargetPos, tagMoveChessResult & MoveChessResult, LPTSTR pszManualName)
{
	//Ч������
	ASSERT((cbXSourcePos<COUNT_X)&&(cbYSourcePos<COUNT_Y));
	ASSERT((cbXTargetPos<COUNT_X)&&(cbYTargetPos<COUNT_Y));

	//Ч������
	if ((cbXSourcePos>=COUNT_X)&&(cbYSourcePos>=COUNT_Y)) return false;//?
	if ((cbXTargetPos>=COUNT_X)&&(cbYTargetPos>=COUNT_Y)) return false;

	//��ȡ����
	if (pszManualName!=NULL)
	{
		GetManualItemName(cbXSourcePos,cbYSourcePos,cbXTargetPos,cbYTargetPos,pszManualName);
	}

	//�ƶ�����
	if (PerformMoveChess(cbXSourcePos,cbYSourcePos,cbXTargetPos,cbYTargetPos,MoveChessResult)==false)
	{
		ASSERT(FALSE);
		return false;
	}

	//��������
	tagChessManual ChessManual;
	ZeroMemory(&ChessManual,sizeof(ChessManual));

	//�������
	ChessManual.cbXSourceChessPos=cbXSourcePos;
	ChessManual.cbYSourceChessPos=cbYSourcePos;
	ChessManual.cbXTargetChessPos=cbXTargetPos;
	ChessManual.cbYTargetChessPos=cbYTargetPos;

	//��������
	m_ChessManualArray.Add(ChessManual);

	return true;
}

//��ȡ��ͼ
VOID CGameLogic::GetHistoryStatus(tagChessItem ChessItem[2][COUNT_CHESS], tagChessItem * ChessBorad[COUNT_X][COUNT_Y], WORD wStepCount)
{
	//��������
	InitChessBorad(ChessItem,ChessBorad);

	//���ý��
	for (WORD i=0;i<wStepCount;i++)
	{
		//Ч�����
		ASSERT(i<m_ChessManualArray.GetCount());
		if (i>=m_ChessManualArray.GetCount()) break;

		//��ȡ����
		tagChessManual * pChessManual=&m_ChessManualArray[i];

		//λ�ö���
		BYTE cbXSourcePos=pChessManual->cbXSourceChessPos;
		BYTE cbYSourcePos=pChessManual->cbYSourceChessPos;
		BYTE cbXTargetPos=pChessManual->cbXTargetChessPos;
		BYTE cbYTargetPos=pChessManual->cbYTargetChessPos;

		//��ȡ����
		tagChessItem * pSourceChessItem=ChessBorad[cbXSourcePos][cbYSourcePos];
		tagChessItem * pTargetChessItem=ChessBorad[cbXTargetPos][cbYTargetPos];

		//�����ж�
		if (pTargetChessItem!=NULL)
		{
			pTargetChessItem->cbXPos=0xFF;
			pTargetChessItem->cbYPos=0xFF;
		}

		//�ƶ�����
		pSourceChessItem->cbXPos=cbXTargetPos;
		pSourceChessItem->cbYPos=cbYTargetPos;
		ChessBorad[cbXSourcePos][cbYSourcePos]=NULL;
		ChessBorad[cbXTargetPos][cbYTargetPos]=pSourceChessItem;
	}

	return;
}

//�����ж�
bool CGameLogic::IsGameFinish(BYTE cbMoveColor, tagConcludeInfo & ConcludeInfo)
{
	//��������
	BYTE cbEnemyIndex=(cbMoveColor==CHESS_BLACK)?1:0;
	BYTE cbMySelfIndex=(cbMoveColor==CHESS_BLACK)?0:1;

	//׽����Ϣ
	tagCatchChessInfo * pEnemyCatchInfo=&m_CatchChessInfo[cbEnemyIndex];
	tagCatchChessInfo * pMySelfCatchInfo=&m_CatchChessInfo[cbMySelfIndex];

	//������Ϣ
	tagGeneralChessInfo * pGeneralChessInfo=&m_GeneralChessInfo[cbMySelfIndex];

	////�����ж�
	//if (pGeneralChessInfo->cbSourceCount>=6)
	//{
	//	//��������
	//	BYTE cbGeneralCount=0;
	//	tagChessItem * pSourceChessItem[3];

	//	//��������
	//	for (BYTE i=0;i<pGeneralChessInfo->cbSourceCount;i++)
	//	{
	//		//�����ж�
	//		for (BYTE j=0;j<cbGeneralCount;j++)
	//		{
	//			if (pGeneralChessInfo->pSourceChessItem[i]==pSourceChessItem[j]) break;
	//		}

	//		//���ý��
	//		if (j==cbGeneralCount)
	//		{
	//			//���ý��
	//			pSourceChessItem[cbGeneralCount++]=pGeneralChessInfo->pSourceChessItem[i];

	//			//����ж�
	//			if (cbGeneralCount>=CountArray(pSourceChessItem)) break;
	//		}
	//	}

	//	//�����ж�
	//	if (pGeneralChessInfo->cbSourceCount>=cbGeneralCount*6) 
	//	{
	//		//���ñ���
	//		ConcludeInfo.cbReason = REASON_CATCH_GENERA;
	//		ConcludeInfo.bGameConclude=true;
	//		ConcludeInfo.cbWinnerColor=(cbMoveColor==CHESS_BLACK)?CHESS_WHITE:CHESS_BLACK;

	//		return true;
	//	}
	//}

	////��׽�ж�
	//if (pMySelfCatchInfo->cbSourceCount>=6)
	//{
	//	//��������
	//	BYTE cbCatchCount=0;
	//	tagChessItem * pSourceChessItem[3];

	//	//��������
	//	for (BYTE i=0;i<pMySelfCatchInfo->cbSourceCount;i++)
	//	{
	//		//�����ж�
	//		for (BYTE j=0;j<cbCatchCount;j++)
	//		{
	//			if (pMySelfCatchInfo->pSourceChessItem[i]==pSourceChessItem[j]) break;
	//		}

	//		//���ý��
	//		if (j==cbCatchCount)
	//		{
	//			//���ý��
	//			pSourceChessItem[cbCatchCount++]=pMySelfCatchInfo->pSourceChessItem[i];

	//			//����ж�
	//			if (cbCatchCount>=CountArray(pSourceChessItem)) break;
	//		}
	//	}

	//	//��׽�ж�
	//	if (pMySelfCatchInfo->cbSourceCount>=cbCatchCount*6) 
	//	{
	//		//���ñ���
	//		ConcludeInfo.cbReason = REASON_CATCH_CHESS;
	//		ConcludeInfo.bGameConclude=true;
	//		ConcludeInfo.cbWinnerColor=(cbMoveColor==CHESS_BLACK)?CHESS_WHITE:CHESS_BLACK;

	//		return true;
	//	}
	//}

	//��ȡ����
	tagChessItem * pKindChessItem=&m_ChessItem[cbMySelfIndex][4];
	ASSERT((pKindChessItem->cbXPos!=0xFF)&&(pKindChessItem->cbYPos!=0xFF));

	//�����ж�
	if (GetMinatoryChess(pKindChessItem->cbXPos,pKindChessItem->cbYPos)!=NULL)
	{
		//���ñ���
		ConcludeInfo.bGameConclude=true;
		ConcludeInfo.cbWinnerColor=(cbMoveColor==CHESS_BLACK)?CHESS_WHITE:CHESS_BLACK;

		return true;
	}

	////�ղ��к�
	//if ((m_wNullStep>=120)||(m_ChessManualArray.GetCount()>=400))
	//{
	//	//���ñ���
	//	if(m_wNullStep>=120)ConcludeInfo.cbReason = REASON_LEISURE_STEP;
	//	else ConcludeInfo.cbReason = REASON_OVER_STEP;
	//	ConcludeInfo.bGameConclude = true;
	//	ConcludeInfo.cbWinnerColor = CHESS_NULL;

	//	return true;
	//}

	//�����ж�
	if(IsLoseChess((cbMoveColor==CHESS_BLACK)?CHESS_WHITE:CHESS_BLACK))
	{
		//��ȡ����
		tagChessItem * pKindChessItem=&m_ChessItem[cbEnemyIndex][4];
		ASSERT((pKindChessItem->cbXPos!=0xFF)&&(pKindChessItem->cbYPos!=0xFF));

		//�����ж�
		if (GetMinatoryChess(pKindChessItem->cbXPos,pKindChessItem->cbYPos)==NULL)
		{
			ConcludeInfo.cbReason = REASON_CHESS_FEAZE;
		}
		else ConcludeInfo.cbReason = REASON_KILL_GENERA;

		//���ñ���
		ConcludeInfo.bGameConclude=true;
		ConcludeInfo.cbWinnerColor=cbMoveColor;

		return true;
	}

	//���ý��
	ZeroMemory(&ConcludeInfo,sizeof(ConcludeInfo));

	return false;
}

//Ч�鲽��
bool CGameLogic::IsWalkLegality(const tagChessItem * pSourceChessItem, BYTE cbXTargetPos, BYTE cbYTargetPos)
{
	//Ч������
	ASSERT(pSourceChessItem!=NULL);
	ASSERT((cbXTargetPos<COUNT_X)&&(cbYTargetPos<COUNT_Y));
	ASSERT((pSourceChessItem->cbXPos<COUNT_X)&&(pSourceChessItem->cbYPos<COUNT_Y));

	//��ȡ����
	BYTE cbXSourcePos=pSourceChessItem->cbXPos;
	BYTE cbYSourcePos=pSourceChessItem->cbYPos;
	tagChessItem * pTargetChessItem=m_ChessBorad[cbXTargetPos][cbYTargetPos];

	//ͬ���ж�
	if ((pSourceChessItem->cbXPos==cbXTargetPos)&&(pSourceChessItem->cbYPos==cbYTargetPos)) return false;

    //�����ж�
	switch (pSourceChessItem->cbChess)
	{
	case CHESS_GENERAL:	//��
		{
			//ƫ�Ʊ���
			BYTE cbXExcursion=GetExcursion(cbXSourcePos,cbXTargetPos);
			BYTE cbYExcursion=GetExcursion(cbYSourcePos,cbYTargetPos);

			//�ɽ��ж�
			if ((pTargetChessItem!=NULL)&&(pTargetChessItem->cbChess==CHESS_GENERAL))
			{
				if (cbXSourcePos!=cbXTargetPos) return false;
				if (ObstructAnalyse(cbXSourcePos,cbYSourcePos,cbXTargetPos,cbYTargetPos)==true) return false;
				return true;
			}

			//�����ж�
			if ((cbXTargetPos<3)||(cbXTargetPos>5)) return false;
			if ((cbXExcursion>1)||(cbYExcursion>1)) return false;
			if ((cbXExcursion==1)&&(cbYExcursion==1)) return false;
			if ((pSourceChessItem->cbColor==CHESS_BLACK)&&(cbYTargetPos<7)) return false;
			if ((pSourceChessItem->cbColor==CHESS_WHITE)&&(cbYTargetPos>2)) return false;

			return true;
		}
	case CHESS_KAVASS:	//ʿ
		{
			//ƫ�Ʊ���
			BYTE cbXExcursion=GetExcursion(cbXSourcePos,cbXTargetPos);
			BYTE cbYExcursion=GetExcursion(cbYSourcePos,cbYTargetPos);

			//�����ж�
			if ((cbXExcursion!=1)||(cbYExcursion!=1)) return false;
			if ((cbXTargetPos<3)||(cbXTargetPos>5)) return false;
			if ((pSourceChessItem->cbColor==CHESS_BLACK)&&(cbYTargetPos<7)) return false;
			if ((pSourceChessItem->cbColor==CHESS_WHITE)&&(cbYTargetPos>2)) return false;

			return true;
		}
	case CHESS_ELEPHANT://��
		{
			//ƫ�Ʊ���
			BYTE cbXExcursion=GetExcursion(cbXSourcePos,cbXTargetPos);
			BYTE cbYExcursion=GetExcursion(cbYSourcePos,cbYTargetPos);

			//�����ж�
			if ((cbXExcursion!=2)||(cbYExcursion!=2)) return false;
			if ((pSourceChessItem->cbColor==CHESS_BLACK)&&(cbYTargetPos<5)) return false;
			if ((pSourceChessItem->cbColor==CHESS_WHITE)&&(cbYTargetPos>4)) return false;
			if (m_ChessBorad[(cbXSourcePos+cbXTargetPos)/2][(cbYSourcePos+cbYTargetPos)/2]!=NULL) return false;

			return true;
		}
	case CHESS_HORSE:	//��
		{
			//ƫ�Ʊ���
			BYTE cbXExcursion=GetExcursion(cbXSourcePos,cbXTargetPos);
			BYTE cbYExcursion=GetExcursion(cbYSourcePos,cbYTargetPos);

			//�����ж�
			if (((cbXExcursion!=1)||(cbYExcursion!=2))&&((cbXExcursion!=2)||(cbYExcursion!=1))) return false;

			//Ʋ���ж�
			BYTE cbXVerdict=(cbXExcursion==1)?cbXSourcePos:(cbXSourcePos+cbXTargetPos)/2;
			BYTE cbYVerdict=(cbYExcursion==1)?cbYSourcePos:(cbYSourcePos+cbYTargetPos)/2;
			if (m_ChessBorad[cbXVerdict][cbYVerdict]!=NULL) return false;

			return true;
		}
	case CHESS_ROOK:	//��
		{
			//�����ж�
			if ((cbXSourcePos!=cbXTargetPos)&&(cbYSourcePos!=cbYTargetPos)) return false;

			//��������
			if (ObstructAnalyse(cbXSourcePos,cbYSourcePos,cbXTargetPos,cbYTargetPos)==true) return false;

			return true;
		}
	case CHESS_CANNON:	//��
		{
			//�����ж�
			if ((cbXSourcePos!=cbXTargetPos)&&(cbYSourcePos!=cbYTargetPos)) return false;

			//�����ж�
			if (pTargetChessItem!=NULL)
			{
				//��������
				INT nXStep=0,nYStep=0;
				if (cbXTargetPos!=cbXSourcePos) nXStep=(cbXTargetPos>cbXSourcePos)?1:-1;
				if (cbYTargetPos!=cbYSourcePos) nYStep=(cbYTargetPos>cbYSourcePos)?1:-1;

				//��̨����
				bool bEmplacement=false;
				do
				{
					cbXSourcePos+=nXStep;
					cbYSourcePos+=nYStep;
					if ((cbXSourcePos==cbXTargetPos)&&(cbYSourcePos==cbYTargetPos))	return bEmplacement;
					if (m_ChessBorad[cbXSourcePos][cbYSourcePos]!=NULL)
					{
						if (bEmplacement==false) bEmplacement=true;
						else return false;
					}
				} while (true);

				return false;
			}

			//��������
			if (ObstructAnalyse(cbXSourcePos,cbYSourcePos,cbXTargetPos,cbYTargetPos)==true) return false;

			return true;
		}
	case CHESS_SOLDIER:	//��
		{
			//ƫ�Ʊ���
			BYTE cbXExcursion=GetExcursion(cbXSourcePos,cbXTargetPos);
			BYTE cbYExcursion=GetExcursion(cbYSourcePos,cbYTargetPos);

			//�����ж�
			if (pSourceChessItem->cbColor==CHESS_BLACK)
			{
				if (cbYSourcePos>=5)
				{
					if (cbXSourcePos!=cbXTargetPos) return false;
					if ((cbYSourcePos-1)!=cbYTargetPos) return false;
				}
				else
				{
					if (cbYTargetPos>cbYSourcePos) return false;
					if ((cbXExcursion>1)||(cbYExcursion>1)) return false;
					if ((cbXExcursion!=0)&&(cbYExcursion!=0)) return false;
				}
			}
			else
			{
				if (cbYSourcePos<=4)
				{
					if (cbXSourcePos!=cbXTargetPos) return false;
					if ((cbYSourcePos+1)!=cbYTargetPos) return false;
				}
				else
				{
					if (cbYTargetPos<cbYSourcePos) return false;
					if ((cbXExcursion>1)||(cbYExcursion>1)) return false;
					if ((cbXExcursion!=0)&&(cbYExcursion!=0)) return false;
				}
			}

			return true;
		}
	}

	ASSERT(FALSE);

	return false;
}

//��ȡ����
BYTE CGameLogic::GetColorIndex(BYTE cbChessColor)
{
	return (cbChessColor==CHESS_BLACK)?0:1;
}

//��ȡƫ��
BYTE CGameLogic::GetExcursion(BYTE cbSourcePos, BYTE cbTargetPos)
{
	return (cbSourcePos>cbTargetPos)?(cbSourcePos-cbTargetPos):(cbTargetPos-cbSourcePos);
}

//ִ������
bool CGameLogic::PerformMoveChess(BYTE cbXSourcePos, BYTE cbYSourcePos, BYTE cbXTargetPos, BYTE cbYTargetPos, tagMoveChessResult & MoveChessResult)
{
	//Ч������
	ASSERT((cbXSourcePos<COUNT_X)&&(cbYSourcePos<COUNT_Y));
	ASSERT((cbXTargetPos<COUNT_X)&&(cbYTargetPos<COUNT_Y));

	//��ȡ����
	tagChessItem * pSourceChessItem=m_ChessBorad[cbXSourcePos][cbYSourcePos];
	tagChessItem * pTargetChessItem=m_ChessBorad[cbXTargetPos][cbYTargetPos];

	//Ч������
	ASSERT(pSourceChessItem!=NULL);
	if (pSourceChessItem==NULL) return false;

	//��������
	bool bRescueChess=false;
	tagChessItem * pCatchTargetChess=NULL;
	tagChessItem * pCatchSourceChess=NULL;

	//��������
	BYTE cbEnemyIndex=(pSourceChessItem->cbColor==CHESS_BLACK)?1:0;
	BYTE cbMySelfIndex=(pSourceChessItem->cbColor==CHESS_BLACK)?0:1;

	//׽����Ϣ
	tagCatchChessInfo * pEnemyCatchInfo=&m_CatchChessInfo[cbEnemyIndex];
	tagCatchChessInfo * pMySelfCatchInfo=&m_CatchChessInfo[cbMySelfIndex];

	//��׽�ж�
	if ((pTargetChessItem==NULL)&&(pEnemyCatchInfo->pTargetChessItem!=pSourceChessItem))
	{
		//��в�ж�
		tagChessItem * pMinatoryChess=GetMinatoryChess(pSourceChessItem->cbXPos,pSourceChessItem->cbYPos);

		//��׽�ж�
		if (pMinatoryChess!=NULL)
		{
			//�и��ж�
			tagChessItem * pProtectedChess=GetProtectedChess(pSourceChessItem->cbXPos,pSourceChessItem->cbYPos);

			//����ж�
			if (pProtectedChess!=NULL)
			{
				BYTE cbSourceChess=pMinatoryChess->cbChess;
				BYTE cbTargetChess=pSourceChessItem->cbChess;
				if ((cbTargetChess==CHESS_ROOK)&&((cbSourceChess==CHESS_HORSE)||(cbSourceChess==CHESS_CANNON))) bRescueChess=true;
			}
			else bRescueChess=true;
		}
	}

	//���Ӵ���
	if (pTargetChessItem!=NULL)
	{
		//����״̬
		m_wNullStep=0;

		//��������
		pTargetChessItem->cbXPos=0xFF;
		pTargetChessItem->cbYPos=0xFF;
	}
	else
	{
		//����״̬
		m_wNullStep++;
	}

	//��������
	pSourceChessItem->cbXPos=cbXTargetPos;
	pSourceChessItem->cbYPos=cbYTargetPos;
	m_ChessBorad[cbXSourcePos][cbYSourcePos]=NULL;
	m_ChessBorad[cbXTargetPos][cbYTargetPos]=pSourceChessItem;

	//���ý��
	ZeroMemory(&MoveChessResult,sizeof(MoveChessResult));

	//�Է�����
	BYTE cbChessColor=pSourceChessItem->cbColor;
	tagChessItem * pKindChessItem=&m_ChessItem[(cbChessColor==CHESS_BLACK)?1:0][4];

	//�����ж�
	if ((pKindChessItem->cbXPos!=0xFF)&&(pKindChessItem->cbYPos!=0xFF))
	{
		//�����ж�
		if (GetMinatoryChess(pKindChessItem->cbXPos,pKindChessItem->cbYPos)!=NULL)
		{
			//���ñ�־
			MoveChessResult.bMinatoryKing=true;

			//������¼
			BYTE cbIndex=m_GeneralChessInfo[cbMySelfIndex].cbSourceCount++;
			m_GeneralChessInfo[cbMySelfIndex].pSourceChessItem[cbIndex]=pSourceChessItem;

			return true;
		}
		else
		{
			//���ü�¼
			m_GeneralChessInfo[cbMySelfIndex].cbSourceCount=0;
		}
	}

	//׽���ж�
	if ((pTargetChessItem==NULL)&&(pSourceChessItem->cbChess!=CHESS_GENERAL)&&(pSourceChessItem->cbChess!=CHESS_SOLDIER))
	{
		//����ж�
		if ((bRescueChess==false)&&(pEnemyCatchInfo->cbSourceCount>0)&&(pEnemyCatchInfo->pTargetChessItem!=NULL))
		{
			//��ȡ����
			tagChessItem * pMySelfChessItem=pEnemyCatchInfo->pTargetChessItem;
			tagChessItem * pEnemyChessItem=pEnemyCatchInfo->pSourceChessItem[pEnemyCatchInfo->cbSourceCount-1];

			//����ж�
			if (IsWalkLegality(pEnemyChessItem,pMySelfChessItem->cbXPos,pMySelfChessItem->cbYPos)==false) bRescueChess=true;
		}

		//��ԭ״̬
		if (bRescueChess==false)
		{
			pEnemyCatchInfo->cbSourceCount=0;
			pEnemyCatchInfo->pTargetChessItem=NULL;
		}

		//׽���ж�
		if (bRescueChess==false)
		{
			//׽������
			for (BYTE i=0;i<CountArray(m_ChessItem[cbMySelfIndex]);i++)
			{
				//��ȡ����
				tagChessItem * pMySelfChessItem=&m_ChessItem[cbMySelfIndex][i];
				if ((pMySelfChessItem->cbXPos==0xFF)||(pMySelfChessItem->cbYPos==0xFF)) continue;

				//׽���ж�
				for (BYTE j=0;j<CountArray(m_ChessItem[cbEnemyIndex]);j++)
				{
					//��ȡ����
					tagChessItem * pEnemyChessItem=&m_ChessItem[cbEnemyIndex][j];
					if ((pEnemyChessItem->cbXPos==0xFF)||(pEnemyChessItem->cbYPos==0xFF)) continue;

					//��в�ж�
					if (IsWalkLegality(pMySelfChessItem,pEnemyChessItem->cbXPos,pEnemyChessItem->cbYPos)==false) continue;

					//�и��ж�
					tagChessItem * pProtectedChess=GetProtectedChess(pEnemyChessItem->cbXPos,pEnemyChessItem->cbYPos);

					//����ж�
					bool bCatchChess=true;
					if (pProtectedChess!=NULL)
					{
						BYTE cbTargetChess=pEnemyChessItem->cbChess;
						BYTE cbSourceChess=pMySelfChessItem->cbChess;
						bCatchChess=((cbTargetChess==CHESS_ROOK)&&((cbSourceChess==CHESS_HORSE)||(cbSourceChess==CHESS_CANNON)));
					}

					//���ý��
					if (bCatchChess==true)
					{
						MoveChessResult.bCatchChess=true;
						pCatchTargetChess=pEnemyChessItem;
						pCatchSourceChess=pMySelfChessItem;

						break;
					}
				}

				//����ж�
				if ((pCatchSourceChess!=NULL)&&(pCatchTargetChess!=NULL)) break;
			}
		}

		//��ԭ״̬
		if ((pCatchTargetChess==NULL)||(pMySelfCatchInfo->pTargetChessItem!=pCatchTargetChess))
		{
			pMySelfCatchInfo->cbSourceCount=0;
			pMySelfCatchInfo->pTargetChessItem=pCatchTargetChess;
		}

		//�����б�
		if (pCatchTargetChess!=NULL)
		{
			ASSERT(pMySelfCatchInfo->cbSourceCount<CountArray(pMySelfCatchInfo->pSourceChessItem));
			pMySelfCatchInfo->pSourceChessItem[pMySelfCatchInfo->cbSourceCount++]=pCatchSourceChess;
		}
	}
	else
	{
		//��������
		BYTE cbMySelfIndex=(pSourceChessItem->cbColor==CHESS_BLACK)?0:1;
		tagCatchChessInfo * pMySelfCatchInfo=&m_CatchChessInfo[cbMySelfIndex];

		//��ԭ״̬
		pMySelfCatchInfo->cbSourceCount=0;
		pMySelfCatchInfo->pTargetChessItem=NULL;
	}

	return true;
}

//�жϽ���
bool CGameLogic::IsKillGeneral(BYTE cbXSourcePos, BYTE cbYSourcePos, BYTE cbXTargetPos, BYTE cbYTargetPos)
{
	//Ч������
	ASSERT((cbXSourcePos<COUNT_X)&&(cbYSourcePos<COUNT_Y));
	ASSERT((cbXTargetPos<COUNT_X)&&(cbYTargetPos<COUNT_Y));

	//��ȡ����
	tagChessItem * pSourceChessItem = m_ChessBorad[cbXSourcePos][cbYSourcePos];
	tagChessItem * pTargetChessItem = m_ChessBorad[cbXTargetPos][cbYTargetPos];

	//��������
	BYTE cbColor = pSourceChessItem->cbColor;
	BYTE cbMySelfIndex=(cbColor==CHESS_BLACK)?0:1;
	//BYTE cbEnemyIndex=(cbColor==CHESS_BLACK)?1:0;

	//���Ӵ���
	if (pTargetChessItem!=NULL)
	{
		//��������
		pTargetChessItem->cbXPos=0xFF;
		pTargetChessItem->cbYPos=0xFF;
	}

	//��������
	//tagChessItem * pSouTempChessItem = m_ChessBorad[cbXSourcePos][cbYSourcePos];
	pSourceChessItem->cbXPos=cbXTargetPos;
	pSourceChessItem->cbYPos=cbYTargetPos;
	m_ChessBorad[cbXSourcePos][cbYSourcePos]=NULL;
	m_ChessBorad[cbXTargetPos][cbYTargetPos]=pSourceChessItem;

	//�ҷ�����
	tagChessItem * pKindChessItem=&m_ChessItem[cbMySelfIndex][4];
	ASSERT((pKindChessItem->cbXPos!=0xFF)&&(pKindChessItem->cbYPos!=0xFF));
	ASSERT(pKindChessItem->cbChess==CHESS_GENERAL);

	//�жϽ���
	bool bKillGeneral = false;
	if (GetMinatoryChess(pKindChessItem->cbXPos,pKindChessItem->cbYPos)!=NULL)
	{
		bKillGeneral =  true;
	}

	//��ԭ����
	if (pTargetChessItem!=NULL)
	{
		pTargetChessItem->cbXPos = cbXTargetPos;
		pTargetChessItem->cbYPos = cbYTargetPos;
		m_ChessBorad[cbXTargetPos][cbYTargetPos] = pTargetChessItem;
	}
	else m_ChessBorad[cbXTargetPos][cbYTargetPos] = NULL;
	pSourceChessItem->cbXPos=cbXSourcePos;
	pSourceChessItem->cbYPos=cbYSourcePos;
	m_ChessBorad[cbXSourcePos][cbYSourcePos]=pSourceChessItem;

	return bKillGeneral;
}

//�ж�����
bool CGameLogic::IsLoseChess(BYTE cbColor)
{
	//��������
	BYTE cbEnemyIndex=(cbColor==CHESS_BLACK)?1:0;
	BYTE cbMySelfIndex=(cbColor==CHESS_BLACK)?0:1;

	//�ҷ�����
	tagChessItem * pKindChessItem=&m_ChessItem[cbMySelfIndex][4];
	ASSERT((pKindChessItem->cbXPos!=0xFF)&&(pKindChessItem->cbYPos!=0xFF));
	ASSERT(pKindChessItem->cbChess==CHESS_GENERAL);

	//�������
	for (BYTE i=0;i<CountArray(m_ChessItem[cbMySelfIndex]);i++)
	{
		//��ȡ����
		tagChessItem * pMySelfChessItem=&m_ChessItem[cbMySelfIndex][i];
		if ((pMySelfChessItem->cbXPos==0xFF)||(pMySelfChessItem->cbYPos==0xFF)) continue;

		//��������
		for(BYTE x=0;x<COUNT_X;x++)
		{
			for(BYTE y=0;y<COUNT_Y;y++)
			{
				//����ͬ��
				tagChessItem * pChessItem=m_ChessBorad[x][y];
				if ((pChessItem!=NULL)&&(pChessItem->cbColor==cbColor))continue;

				//�ж�����
				if (IsWalkLegality(pMySelfChessItem,x,y))
				{
					//��ȡ����
					tagChessItem * pSourceChessItem=pMySelfChessItem;
					tagChessItem * pTargetChessItem=m_ChessBorad[x][y];

					//���Ӵ���
					if (pTargetChessItem!=NULL)
					{
						//��������
						pTargetChessItem->cbXPos=0xFF;
						pTargetChessItem->cbYPos=0xFF;
					}

					//��������
					BYTE cbXSourcePos = pSourceChessItem->cbXPos;
					BYTE cbYSourcePos = pSourceChessItem->cbYPos;
					//tagChessItem * pTempChessItem = m_ChessBorad[cbXSourcePos][cbYSourcePos];
					pSourceChessItem->cbXPos=x;
					pSourceChessItem->cbYPos=y;
					m_ChessBorad[cbXSourcePos][cbYSourcePos]=NULL;
					m_ChessBorad[x][y]=pSourceChessItem;

					//����ж�
					ASSERT(pKindChessItem->cbChess==CHESS_GENERAL);
					if (GetMinatoryChess(pKindChessItem->cbXPos,pKindChessItem->cbYPos)==NULL)
					{
						//��ԭ����
						if (pTargetChessItem!=NULL)
						{
							pTargetChessItem->cbXPos = x;
							pTargetChessItem->cbYPos = y;
							m_ChessBorad[x][y] = pTargetChessItem;
						}
						else m_ChessBorad[x][y] = NULL;
						pSourceChessItem->cbXPos=cbXSourcePos;
						pSourceChessItem->cbYPos=cbYSourcePos;
						m_ChessBorad[cbXSourcePos][cbYSourcePos]=pSourceChessItem;
						
						return false;
					}

					//��ԭ����
					if (pTargetChessItem!=NULL)
					{
						pTargetChessItem->cbXPos = x;
						pTargetChessItem->cbYPos = y;
						m_ChessBorad[x][y] = pTargetChessItem;
					}
					else m_ChessBorad[x][y] = NULL;
					pSourceChessItem->cbXPos=cbXSourcePos;
					pSourceChessItem->cbYPos=cbYSourcePos;
					m_ChessBorad[cbXSourcePos][cbYSourcePos]=pSourceChessItem;
				}
			}
		}
	}

	return true;
}

//��λ����
bool CGameLogic::InitChessBorad(tagChessItem ChessItem[2][COUNT_CHESS], tagChessItem * ChessBorad[COUNT_X][COUNT_Y])
{
	//��������
	BYTE cbChessInfo[9]={CHESS_ROOK,CHESS_HORSE,CHESS_ELEPHANT,CHESS_KAVASS,CHESS_GENERAL,
		CHESS_KAVASS,CHESS_ELEPHANT,CHESS_HORSE,CHESS_ROOK};

	//��������
	for (BYTE cbUserIndex=0;cbUserIndex<2;cbUserIndex++)
	{
		//��������
		BYTE cbExcursion=0;
		BYTE cbYPos=(cbUserIndex==0)?9:0;
		for (BYTE nIndex=0;nIndex<9;nIndex++,cbExcursion++)
		{
			ChessItem[cbUserIndex][cbExcursion].cbYPos=cbYPos;
			ChessItem[cbUserIndex][cbExcursion].cbXPos=nIndex;
			ChessItem[cbUserIndex][cbExcursion].cbChessID=nIndex;
			ChessItem[cbUserIndex][cbExcursion].cbChess=cbChessInfo[nIndex];
			ChessItem[cbUserIndex][cbExcursion].cbColor=CHESS_BLACK+cbUserIndex;
			ChessBorad[nIndex][cbYPos]=&ChessItem[cbUserIndex][cbExcursion];
		}

		//��������
		cbYPos=(cbUserIndex==0)?6:3;
		for (BYTE nIndex=0;nIndex<5;nIndex++,cbExcursion++)
		{
			ChessItem[cbUserIndex][cbExcursion].cbYPos=cbYPos;
			ChessItem[cbUserIndex][cbExcursion].cbXPos=nIndex*2;
			ChessItem[cbUserIndex][cbExcursion].cbChess=CHESS_SOLDIER;
			ChessItem[cbUserIndex][cbExcursion].cbChessID=cbExcursion;
			ChessItem[cbUserIndex][cbExcursion].cbColor=CHESS_BLACK+cbUserIndex;
			ChessBorad[nIndex*2][cbYPos]=&ChessItem[cbUserIndex][cbExcursion];
		}

		//�ڵ�����
		cbYPos=(cbUserIndex==0)?7:2;
		for (BYTE nIndex=0;nIndex<2;nIndex++,cbExcursion++)
		{
			ChessItem[cbUserIndex][cbExcursion].cbYPos=cbYPos;
			ChessItem[cbUserIndex][cbExcursion].cbXPos=1+nIndex*6;
			ChessItem[cbUserIndex][cbExcursion].cbChess=CHESS_CANNON;
			ChessItem[cbUserIndex][cbExcursion].cbChessID=cbExcursion;
			ChessItem[cbUserIndex][cbExcursion].cbColor=CHESS_BLACK+cbUserIndex;
			ChessBorad[1+nIndex*6][cbYPos]=&ChessItem[cbUserIndex][cbExcursion];
		}
	}

	return true;
}

//��������
bool CGameLogic::GetManualItemName(BYTE cbXSourcePos, BYTE cbYSourcePos, BYTE cbXTargetPos, BYTE cbYTargetPos, LPTSTR pszManualName)
{
	//��������
	LPCTSTR pszChessName[2][7]=
	{
		TEXT("˧"),TEXT("��"),TEXT("��"),TEXT("��"),TEXT("܇"),TEXT("��"),TEXT("��"),
		TEXT("��"),TEXT("ʿ"),TEXT("��"),TEXT("��"),TEXT("܇"),TEXT("��"),TEXT("��"),
	};

	//��������
	LPCTSTR pszXLineName[2][COUNT_X]=
	{
		TEXT("��"),TEXT("��"),TEXT("��"),TEXT("��"),TEXT("��"),TEXT("��"),TEXT("��"),TEXT("��"),TEXT("һ"),
		TEXT("��"),TEXT("��"),TEXT("��"),TEXT("��"),TEXT("��"),TEXT("��"),TEXT("��"),TEXT("��"),TEXT("��"),
	};

	//��������
	LPCTSTR pszYLineName[2][COUNT_Y]=
	{
		TEXT("һ"),TEXT("��"),TEXT("��"),TEXT("��"),TEXT("��"),TEXT("��"),TEXT("��"),TEXT("��"),TEXT("��"),TEXT("ʮ"),
		TEXT("��"),TEXT("��"),TEXT("��"),TEXT("��"),TEXT("��"),TEXT("��"),TEXT("��"),TEXT("��"),TEXT("��"),TEXT("����"),
	};

	//��������
	bool bLocateDown=true;
	bool bExistSameChess=false;

	//��������
	ASSERT(m_ChessBorad[cbXSourcePos][cbYSourcePos]!=NULL);
	BYTE cbSourceColor=m_ChessBorad[cbXSourcePos][cbYSourcePos]->cbColor;
	BYTE cbSourceChess=m_ChessBorad[cbXSourcePos][cbYSourcePos]->cbChess;

	//�����ж�
	for (BYTE nYPos=0;nYPos<COUNT_Y;nYPos++)
	{
		if ((cbYSourcePos!=nYPos)&&(m_ChessBorad[cbXSourcePos][nYPos]!=NULL))
		{
			//��ȡ����
			BYTE cbTargetColor=m_ChessBorad[cbXSourcePos][nYPos]->cbColor;
			BYTE cbTargetChess=m_ChessBorad[cbXSourcePos][nYPos]->cbChess;
			
			//��ͬ�ж�
			if ((cbTargetColor==cbSourceColor)&&(cbTargetChess==cbSourceChess))
			{
				bExistSameChess=true;
				bLocateDown=(nYPos<cbYSourcePos)?true:false;
				break;
			}
		}
	}

	//�����û�
	lstrcpy(pszManualName,(cbSourceColor==CHESS_BLACK)?TEXT("�췽��"):TEXT("�ڷ���"));

	//����ǰ��
	if (bExistSameChess==true)
	{
		//ǰ���ַ�
		if (cbSourceColor==CHESS_BLACK)
		{
			lstrcat(pszManualName,(bLocateDown==true)?TEXT("��"):TEXT("ǰ"));
		}
		else
		{
			lstrcat(pszManualName,(bLocateDown==true)?TEXT("ǰ"):TEXT("��"));
		}

		//��������
		lstrcat(pszManualName,pszChessName[cbSourceColor-1][cbSourceChess-1]);
	}
	else
	{
		//ǰ���ַ�
		lstrcat(pszManualName,pszChessName[cbSourceColor-1][cbSourceChess-1]);
		lstrcat(pszManualName,pszXLineName[cbSourceColor-1][cbXSourcePos]);
	}

	//������
	if (cbYSourcePos==cbYTargetPos)
	{
		//ƽ���ַ�
		lstrcat(pszManualName,TEXT("ƽ"));
		lstrcat(pszManualName,pszXLineName[cbSourceColor-1][cbXTargetPos]);
	}
	else if (cbYSourcePos<cbYTargetPos)
	{
		//ǰ���ַ�
		lstrcat(pszManualName,(cbSourceColor==CHESS_BLACK)?TEXT("��"):TEXT("��"));

		//�ƶ���Ŀ
		if (cbXSourcePos==cbXTargetPos)
		{
			lstrcat(pszManualName,pszYLineName[cbSourceColor-1][cbYTargetPos-cbYSourcePos-1]);
		}
		else
		{
			lstrcat(pszManualName,pszXLineName[cbSourceColor-1][cbXTargetPos]);
		}
	}
	else
	{
		//�����ַ�
		lstrcat(pszManualName,(cbSourceColor==CHESS_BLACK)?TEXT("��"):TEXT("��"));

		//�ƶ���Ŀ
		if (cbXSourcePos==cbXTargetPos)
		{
			lstrcat(pszManualName,pszYLineName[cbSourceColor-1][cbYSourcePos-cbYTargetPos-1]);
		}
		else
		{
			lstrcat(pszManualName,pszXLineName[cbSourceColor-1][cbXTargetPos]);
		}
	}

	return true;
}

//Σ���ж�
tagChessItem * CGameLogic::GetMinatoryChess(BYTE cbXTargetPos, BYTE cbYTargetPos)
{
	//Ч�����
	ASSERT((cbXTargetPos<COUNT_X)&&(cbYTargetPos<COUNT_Y));

	//��ȡ����
	ASSERT(m_ChessBorad[cbXTargetPos][cbYTargetPos]!=NULL);
	tagChessItem * pTargetChessItem=m_ChessBorad[cbXTargetPos][cbYTargetPos];

	//��������
	BYTE cbEnemyIndex=(pTargetChessItem->cbColor==CHESS_BLACK)?1:0;

	//��������
	for (BYTE i=0;i<CountArray(m_ChessItem[cbEnemyIndex]);i++)
	{
		//��ȡ����
		tagChessItem * pSourceChessItem=&m_ChessItem[cbEnemyIndex][i];
		if ((pSourceChessItem->cbXPos==0xFF)||(pSourceChessItem->cbYPos==0xFF)) continue;

		//��в�ж�
		if (IsWalkLegality(pSourceChessItem,cbXTargetPos,cbYTargetPos)==true) return pSourceChessItem;
	}

	return NULL;
}

//�и��ж�
tagChessItem * CGameLogic::GetProtectedChess(BYTE cbXTargetPos, BYTE cbYTargetPos)
{
	//Ч�����
	ASSERT((cbXTargetPos<COUNT_X)&&(cbYTargetPos<COUNT_Y));

	//��ȡ����
	ASSERT(m_ChessBorad[cbXTargetPos][cbYTargetPos]!=NULL);
	tagChessItem * pTargetChessItem=m_ChessBorad[cbXTargetPos][cbYTargetPos];

	//��������
	BYTE cbMySelfIndex=(pTargetChessItem->cbColor==CHESS_BLACK)?0:1;

	//��������
	for (BYTE i=0;i<CountArray(m_ChessItem[cbMySelfIndex]);i++)
	{
		//��ȡ����
		tagChessItem * pSourceChessItem=&m_ChessItem[cbMySelfIndex][i];
		if ((pSourceChessItem->cbXPos==0xFF)||(pSourceChessItem->cbYPos==0xFF)) continue;
		if ((pSourceChessItem->cbXPos==cbXTargetPos)&&(pSourceChessItem->cbYPos==cbYTargetPos)) continue;

		//��в�ж�
		if (IsWalkLegality(pSourceChessItem,cbXTargetPos,cbYTargetPos)==true) return pSourceChessItem;
	}

	return NULL;
}

//��������
bool CGameLogic::ObstructAnalyse(BYTE cbXSourcePos, BYTE cbYSourcePos, BYTE cbXTargetPos, BYTE cbYTargetPos)
{
	//Ч������
	ASSERT((cbXSourcePos!=cbXTargetPos)||(cbYSourcePos!=cbYTargetPos));

	//��������
	INT nXStep=0,nYStep=0;
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
