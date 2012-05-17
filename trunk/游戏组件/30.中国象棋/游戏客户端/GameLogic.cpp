#include "StdAfx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameLogic::CGameLogic()
{
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
	ASSERT((cbXPos<9)&&(cbYPos<10));
	return m_ChessBorad[cbXPos][cbYPos];
}

//��������
void CGameLogic::ResetChessBorad()
{
	//��������
	BYTE cbChessInfo[9]={CHESS_ROOK,CHESS_HORSE,CHESS_ELEPHANT,CHESS_KAVASS,CHESS_GENERAL,
		CHESS_KAVASS,CHESS_ELEPHANT,CHESS_HORSE,CHESS_ROOK};

	//���ñ���
	m_ChessManualInfo.RemoveAll();
	memset(m_ChessItem,0,sizeof(m_ChessItem));
	memset(m_wStepCount,0,sizeof(m_wStepCount));
	memset(m_ChessBorad,0,sizeof(m_ChessBorad));

	//��������
	for (BYTE cbUserIndex=0;cbUserIndex<2;cbUserIndex++)
	{
		//��������
		BYTE cbExcursion=0;
		BYTE cbYPos=(cbUserIndex==0)?9:0;
		for (BYTE nIndex=0;nIndex<9;nIndex++,cbExcursion++)
		{
			m_ChessItem[cbUserIndex][cbExcursion].cbYPos=cbYPos;
			m_ChessItem[cbUserIndex][cbExcursion].cbXPos=nIndex;
			m_ChessItem[cbUserIndex][cbExcursion].cbChessID=nIndex;
			m_ChessItem[cbUserIndex][cbExcursion].cbChess=cbChessInfo[nIndex];
			m_ChessItem[cbUserIndex][cbExcursion].cbColor=BLACK_CHESS+cbUserIndex;
			m_ChessBorad[nIndex][cbYPos]=&m_ChessItem[cbUserIndex][cbExcursion];
		}

		//��������
		cbYPos=(cbUserIndex==0)?6:3;
		for (BYTE nIndex=0;nIndex<5;nIndex++,cbExcursion++)
		{
			m_ChessItem[cbUserIndex][cbExcursion].cbYPos=cbYPos;
			m_ChessItem[cbUserIndex][cbExcursion].cbXPos=nIndex*2;
			m_ChessItem[cbUserIndex][cbExcursion].cbChess=CHESS_SOLDIER;
			m_ChessItem[cbUserIndex][cbExcursion].cbChessID=cbExcursion;
			m_ChessItem[cbUserIndex][cbExcursion].cbColor=BLACK_CHESS+cbUserIndex;
			m_ChessBorad[nIndex*2][cbYPos]=&m_ChessItem[cbUserIndex][cbExcursion];
		}

		//�ڵ�����
		cbYPos=(cbUserIndex==0)?7:2;
		for (BYTE nIndex=0;nIndex<2;nIndex++,cbExcursion++)
		{
			m_ChessItem[cbUserIndex][cbExcursion].cbYPos=cbYPos;
			m_ChessItem[cbUserIndex][cbExcursion].cbXPos=1+nIndex*6;
			m_ChessItem[cbUserIndex][cbExcursion].cbChess=CHESS_CANNON;
			m_ChessItem[cbUserIndex][cbExcursion].cbChessID=cbExcursion;
			m_ChessItem[cbUserIndex][cbExcursion].cbColor=BLACK_CHESS+cbUserIndex;
			m_ChessBorad[1+nIndex*6][cbYPos]=&m_ChessItem[cbUserIndex][cbExcursion];
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
				ASSERT(m_ChessItem[i][j].cbXPos<9);
				ASSERT(m_ChessItem[i][j].cbYPos<10);
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
		BYTE cbMeIndex=GetColorIndex(pChessManual->cbSourceColor);
		tagChessItem * pSourceChessItem=&m_ChessItem[cbMeIndex][pChessManual->cbSourceID];
		tagChessItem * pTargetChessItem=(pChessManual->cbUniteChess!=NO_CHESS)?pTargetChessItem=&m_ChessItem[(cbMeIndex+1)%2][pChessManual->cbUniteID]:NULL;

		//Ч������
		ASSERT(pSourceChessItem->cbXPos!=255);
		ASSERT(pSourceChessItem->cbYPos!=255);

		//���ò���
		m_wStepCount[GetColorIndex(pChessManual->cbSourceColor)]--;

		//����Դ��
		pSourceChessItem->cbChess=pChessManual->cbSourceChess;
		pSourceChessItem->cbXPos=pChessManual->cbXSourceChessPos;
		pSourceChessItem->cbYPos=pChessManual->cbYSourceChessPos;

		//��������
		if (pTargetChessItem!=NULL)
		{
			pTargetChessItem->cbXPos=pChessManual->cbXTargetChessPos;
			pTargetChessItem->cbYPos=pChessManual->cbYTargetChessPos;
		}
	}

	//ɾ������
	m_ChessManualInfo.RemoveAt(nManualCount-wStepCount,wStepCount);

	//��������
	UpdateChessBorad();

	return true;
}

//�ƶ�����
bool CGameLogic::MoveChess(BYTE cbXSourcePos, BYTE cbYSourcePos, BYTE cbXTargetPos, BYTE cbYTargetPos)
{
	//Ч������
	ASSERT((cbXSourcePos<9)&&(cbYSourcePos<10));
	ASSERT((cbXTargetPos<9)&&(cbYTargetPos<10));

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

	//�����ж�
	if (pTargetChessItem!=NULL)
	{
		//��������
		pTargetChessItem->cbXPos=255;
		pTargetChessItem->cbYPos=255;

		//��������
		ChessManual.cbUniteID=pTargetChessItem->cbChessID;
		ChessManual.cbUniteChess=pTargetChessItem->cbChess;
	}

	//��¼����
	m_ChessManualInfo.Add(ChessManual);
	m_wStepCount[pSourceChessItem->cbColor-1]++;
	m_pItemLately[pSourceChessItem->cbColor-1]=pSourceChessItem;

	//�ƶ�����
	pSourceChessItem->cbXPos=cbXTargetPos;
	pSourceChessItem->cbYPos=cbYTargetPos;
	m_ChessBorad[cbXSourcePos][cbYSourcePos]=NULL;
	m_ChessBorad[cbXTargetPos][cbYTargetPos]=pSourceChessItem;

	return true;
}

//�����ж�
bool CGameLogic::IsGameFinish(BYTE cbColor)
{
	//��ȡ����
	tagChessItem * pKindChessItem=&m_ChessItem[(cbColor==BLACK_CHESS)?0:1][4];

	//�����ж�
	return ((pKindChessItem->cbXPos==255)&&(pKindChessItem->cbYPos==255));
}

//ɱ���ж�
bool CGameLogic::IsKingDanger(BYTE cbColor)
{
	//�Է�����
	tagChessItem * pKindChessItem=&m_ChessItem[(cbColor==BLACK_CHESS)?1:0][4];
	if ((pKindChessItem->cbXPos==255)||(pKindChessItem->cbYPos==255)) return false;

	//�Լ�����
	pKindChessItem=&m_ChessItem[(cbColor==BLACK_CHESS)?0:1][4];
	if ((pKindChessItem->cbXPos==255)||(pKindChessItem->cbYPos==255)) return false;

	//Σ���ж�
	return IsChessDanger(pKindChessItem->cbXPos,pKindChessItem->cbYPos);
}

//Σ���ж�
bool CGameLogic::IsChessDanger(BYTE cbXTargetPos, BYTE cbYTargetPos)
{
	//Ч�����
	ASSERT((cbXTargetPos<9)&&(cbYTargetPos<10));

	//��ȡ����
	tagChessItem * pTargetChessItem=m_ChessBorad[cbXTargetPos][cbYTargetPos];
	if (pTargetChessItem==NULL) return false;

	//��������
	BYTE cbXSourcePos=255,cbYSourcePos=255;
	BYTE cbEnemyIndex=(pTargetChessItem->cbColor==BLACK_CHESS)?1:0;

	//��������
	for (BYTE i=0;i<CountArray(m_ChessItem[cbEnemyIndex]);i++)
	{
		//��ȡ����
		const tagChessItem * pSourceChessItem=&m_ChessItem[cbEnemyIndex][i];
		if ((pSourceChessItem->cbXPos==255)||(pSourceChessItem->cbYPos==255)) continue;

		//��в�ж�
		if (IsWalkLegality(pSourceChessItem,cbXTargetPos,cbYTargetPos)==true) return true;
	}

	return false;
}

//Ч�鲽��
bool CGameLogic::IsWalkLegality(const tagChessItem * pSourceChessItem, BYTE cbXTargetPos, BYTE cbYTargetPos)
{
	//Ч������
	ASSERT(pSourceChessItem!=NULL);
	ASSERT((cbXTargetPos<9)&&(cbYTargetPos<10));
	ASSERT((pSourceChessItem->cbXPos<9)&&(pSourceChessItem->cbYPos<10));

	//��ȡ����
	BYTE cbXSourcePos=pSourceChessItem->cbXPos;
	BYTE cbYSourcePos=pSourceChessItem->cbYPos;
	tagChessItem * pTargetChessItem=m_ChessBorad[cbXTargetPos][cbYTargetPos];

	//Ŀ������
	if ((pTargetChessItem!=NULL)&&(pTargetChessItem->cbColor==pSourceChessItem->cbColor)) return false;

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
			if ((pSourceChessItem->cbColor==BLACK_CHESS)&&(cbYTargetPos<7)) return false;
			if ((pSourceChessItem->cbColor==WHITE_CHESS)&&(cbYTargetPos>2)) return false;

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
			if ((pSourceChessItem->cbColor==BLACK_CHESS)&&(cbYTargetPos<7)) return false;
			if ((pSourceChessItem->cbColor==WHITE_CHESS)&&(cbYTargetPos>2)) return false;

			return true;
		}
	case CHESS_ELEPHANT://��
		{
			//ƫ�Ʊ���
			BYTE cbXExcursion=GetExcursion(cbXSourcePos,cbXTargetPos);
			BYTE cbYExcursion=GetExcursion(cbYSourcePos,cbYTargetPos);

			//�����ж�
			if ((cbXExcursion!=2)||(cbYExcursion!=2)) return false;
			if ((pSourceChessItem->cbColor==BLACK_CHESS)&&(cbYTargetPos<5)) return false;
			if ((pSourceChessItem->cbColor==WHITE_CHESS)&&(cbYTargetPos>4)) return false;
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
				int nXStep=0,nYStep=0;
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
			if (pSourceChessItem->cbColor==BLACK_CHESS)
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
