#include "StdAfx.h"
#include "TableFrameSink.h"

//////////////////////////////////////////////////////////////////////////

//��̬����
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;			//��Ϸ����
const enStartMode	CTableFrameSink::m_GameStartMode=enStartMode_FullReady;	//��ʼģʽ

//////////////////////////////////////////////////////////////////////////

//���캯��
CTableFrameSink::CTableFrameSink()
{
	//��Ϸ����
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));
	ZeroMemory(m_bUserHuXiCount,sizeof(m_bUserHuXiCount));

	//��־����
	m_bOutCard=false;
	m_bDispatch=false;
	m_bEstimate=false;

	//���˱���
	m_bTiCard=false;
	m_bSendNotify=false;

	//��������
	m_cbOutCardData=0;
	m_wResumeUser=INVALID_CHAIR;
	m_wOutCardUser=INVALID_CHAIR;

	//������Ϣ
	m_bLeftCardCount=0;
	ZeroMemory(m_cbRepertoryCard,sizeof(m_cbRepertoryCard));

	//�û�״̬
	ZeroMemory(m_bResponse,sizeof(m_bResponse));
	ZeroMemory(m_bUserAction,sizeof(m_bUserAction));
	ZeroMemory(m_bChiCardKind,sizeof(m_bChiCardKind));
	ZeroMemory(m_bPerformAction,sizeof(m_bPerformAction));

	//����״̬
	ZeroMemory(m_bMingPao,sizeof(m_bMingPao));
	ZeroMemory(m_bAbandonCard,sizeof(m_bAbandonCard));

	//����˿�
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));
	ZeroMemory(m_bWeaveItemCount,sizeof(m_bWeaveItemCount));

	//�������
	m_pITableFrame=NULL;
	m_pGameServiceOption=NULL;

	return;
}

//��������
CTableFrameSink::~CTableFrameSink(void)
{
}

//�ӿڲ�ѯ
void * __cdecl CTableFrameSink::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableFrameSink,Guid,dwQueryVer);
	//QUERYINTERFACE(ITableFrameEvent,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITableFrameSink,Guid,dwQueryVer);
	return NULL;
}

//��ʼ��
bool __cdecl CTableFrameSink::InitTableFrameSink(IUnknownEx * pIUnknownEx)
{
	//��ѯ�ӿ�
	ASSERT(pIUnknownEx!=NULL);
	m_pITableFrame=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrame);
	if (m_pITableFrame==NULL) return false;

	//��ȡ����
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	ASSERT(m_pGameServiceOption!=NULL);

	//���ò���
	//m_pITableFrame->SetLessTimes(50L);

	return true;
}

//��λ����
void __cdecl CTableFrameSink::RepositTableFrameSink()
{
	//��Ϸ����
	m_wCurrentUser=INVALID_CHAIR;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));
	ZeroMemory(m_bUserHuXiCount,sizeof(m_bUserHuXiCount));

	//��־����
	m_bOutCard=false;
	m_bDispatch=false;
	m_bEstimate=false;

	//���˱���
	m_bTiCard=false;
	m_bSendNotify=false;

	//��������
	m_cbOutCardData=0;
	m_wResumeUser=INVALID_CHAIR;
	m_wOutCardUser=INVALID_CHAIR;

	//������Ϣ
	m_bLeftCardCount=0;
	ZeroMemory(m_cbRepertoryCard,sizeof(m_cbRepertoryCard));

	//�û�״̬
	ZeroMemory(m_bResponse,sizeof(m_bResponse));
	ZeroMemory(m_bUserAction,sizeof(m_bUserAction));
	ZeroMemory(m_bChiCardKind,sizeof(m_bChiCardKind));
	ZeroMemory(m_bPerformAction,sizeof(m_bPerformAction));

	//����״̬
	ZeroMemory(m_bMingPao,sizeof(m_bMingPao));
	ZeroMemory(m_bAbandonCard,sizeof(m_bAbandonCard));

	//����˿�
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));
	ZeroMemory(m_bWeaveItemCount,sizeof(m_bWeaveItemCount));

	return;
}

//��ʼģʽ
enStartMode __cdecl CTableFrameSink::GetGameStartMode()
{
	return m_GameStartMode;
}

//��Ϸ״̬
bool __cdecl CTableFrameSink::IsUserPlaying(WORD wChairID)
{
	return true;
}

//��Ϸ��ʼ
bool __cdecl CTableFrameSink::OnEventGameStart()
{
	//����״̬
	m_pITableFrame->SetGameStatus(GS_PLAYING);

	//�����˿�
	m_bLeftCardCount=CountArray(m_cbRepertoryCard);
	m_GameLogic.RandCardData(m_cbRepertoryCard,CountArray(m_cbRepertoryCard));

	//�ַ��˿�
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		m_bLeftCardCount-=(MAX_COUNT-1);
		m_GameLogic.SwitchToCardIndex(&m_cbRepertoryCard[m_bLeftCardCount],MAX_COUNT-1,m_cbCardIndex[i]);
	}

	//�����˿�
	BYTE cbBankerCard=m_cbRepertoryCard[--m_bLeftCardCount];
	m_cbCardIndex[m_wBankerUser][m_GameLogic.SwitchToCardIndex(cbBankerCard)]++;

	//����ͳ��
	bool bTiCardAction=false;
	BYTE cbTiCardCount[GAME_PLAYER],cbWeiCardCount[GAME_PLAYER];
	BYTE cbTiCardIndex[GAME_PLAYER][5],cbWeiCardIndex[GAME_PLAYER][7];
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//����ͳ��
		cbTiCardCount[i]=m_GameLogic.GetAcitonTiCard(m_cbCardIndex[i],cbTiCardIndex[i]);
		cbWeiCardCount[i]=m_GameLogic.GetActionWeiCard(m_cbCardIndex[i],cbWeiCardIndex[i]);

		//���ñ�־
		if (cbTiCardCount[i]>0) bTiCardAction=true;
	}

	//���ñ���
	m_bOutCard=true;
	m_cbOutCardData=0;
	m_wCurrentUser=m_wBankerUser;
	m_wOutCardUser=INVALID_CHAIR;

	//��������
	CMD_S_GameStart GameStart;
	GameStart.wBankerUser=m_wBankerUser;
	GameStart.wCurrentUser=(bTiCardAction==true)?INVALID_CHAIR:m_wCurrentUser;

	//��������
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//��������
		m_GameLogic.SwitchToCardData(m_cbCardIndex[i],GameStart.cbCardData,CountArray(GameStart.cbCardData));

		//��������
		m_pITableFrame->SendTableData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
		m_pITableFrame->SendLookonData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
	}

	//���ƴ���
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//��������
		WORD wChairID=(m_wBankerUser+i)%GAME_PLAYER;

		//���ƴ���
		if (cbTiCardCount[wChairID]>0)
		{
			//�������
			for (BYTE j=0;j<cbTiCardCount[wChairID];j++)
			{
				//��������
				BYTE cbRemoveCount=m_cbCardIndex[wChairID][cbTiCardIndex[wChairID][j]];
				BYTE cbTiCardData=m_GameLogic.SwitchToCardData(cbTiCardIndex[wChairID][j]);

				//�����˿�
				m_cbCardIndex[wChairID][cbTiCardIndex[wChairID][j]]=0;

				//�������
				BYTE cbIndex=m_bWeaveItemCount[wChairID]++;
				m_WeaveItemArray[wChairID][cbIndex].cbCardCount=4;
				m_WeaveItemArray[wChairID][cbIndex].cbWeaveKind=ACK_TI;
				m_WeaveItemArray[wChairID][cbIndex].cbCenterCard=cbTiCardData;
				m_WeaveItemArray[wChairID][cbIndex].cbCardList[0]=cbTiCardData;
				m_WeaveItemArray[wChairID][cbIndex].cbCardList[1]=cbTiCardData;
				m_WeaveItemArray[wChairID][cbIndex].cbCardList[2]=cbTiCardData;
				m_WeaveItemArray[wChairID][cbIndex].cbCardList[3]=cbTiCardData;

				//��������
				CMD_S_UserTiCard UserTiCard;
				UserTiCard.wActionUser=wChairID;
				UserTiCard.cbActionCard=cbTiCardData;
				UserTiCard.cbRemoveCount=cbRemoveCount;

				//��������
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_USER_TI_CARD,&UserTiCard,sizeof(UserTiCard));
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_USER_TI_CARD,&UserTiCard,sizeof(UserTiCard));
			}

			//���º�Ϣ
			UpdateUserHuXiCount(wChairID);
		}
	}

	//������ʾ
	if (bTiCardAction==true) SendOutCardNotify(m_wCurrentUser,true);

	return true;
}

//��Ϸ����
bool __cdecl CTableFrameSink::OnEventGameEnd(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_DISMISS:		//��Ϸ��ɢ
		{
			//Ч�����
			ASSERT(pIServerUserItem!=NULL);
			ASSERT(wChairID<m_wPlayerCount);

			//��������
			CMD_S_GameEnd GameEnd;
			memset(&GameEnd,0,sizeof(GameEnd));

			//�����û�
			GameEnd.cbHuCard=0;
			GameEnd.cbReason=cbReason;
			GameEnd.wWinUser=INVALID_CHAIR;
			GameEnd.wProvideUser=INVALID_CHAIR;

			//ʣ���˿�
			BYTE cbPostion=0;
			BYTE cbCardData[MAX_COUNT];
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				//ת���˿�
				BYTE cbCardCount=m_GameLogic.SwitchToCardData(m_cbCardIndex[i],cbCardData,CountArray(cbCardData));

				//�����˿�
				GameEnd.bCardCount[i]=cbCardCount;
				CopyMemory(&GameEnd.bCardData[cbPostion],cbCardData,cbCardCount*sizeof(BYTE));

				//��������
				cbPostion+=cbCardCount;
			}

			//������Ϣ
			CMD_S_GameEnd GameEndTemp;
			GameEndTemp=GameEnd;
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
//				GameEndTemp.lGameScore[i]=m_pITableFrame->AccountGameGold(GameEndTemp.lGameScore[i]);
			}

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEndTemp,sizeof(GameEndTemp));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEndTemp,sizeof(GameEndTemp));

			//������Ϸ
			m_pITableFrame->ConcludeGame();

			return true;
		}
	case GER_NORMAL:		//�������
		{
			//�������
			CMD_S_GameEnd GameEnd;
			memset(&GameEnd,0,sizeof(GameEnd));

			//ʣ���˿�
			BYTE cbPostion=0;
			BYTE cbCardData[MAX_COUNT];
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				//ת���˿�
				BYTE cbCardCount=m_GameLogic.SwitchToCardData(m_cbCardIndex[i],cbCardData,CountArray(cbCardData));

				//�����˿�
				GameEnd.bCardCount[i]=cbCardCount;
				CopyMemory(&GameEnd.bCardData[cbPostion],cbCardData,cbCardCount*sizeof(BYTE));

				//��������
				cbPostion+=cbCardCount;
			}

			//�����û�
			GameEnd.cbReason=cbReason;
			GameEnd.wWinUser=wChairID;
			GameEnd.wProvideUser=INVALID_CHAIR;
			GameEnd.cbHuCard=(m_wOutCardUser!=INVALID_CHAIR)?m_cbOutCardData:0;;

			//ʤ������
			if (wChairID!=INVALID_CHAIR)
			{
				//ͳ�ƺ�Ϣ
				tagHuCardInfo HuCardInfo;
				m_GameLogic.GetHuCardInfo(m_cbCardIndex[wChairID],GameEnd.cbHuCard,m_bUserHuXiCount[wChairID],HuCardInfo);

				//�������
				LONG lScore=(((HuCardInfo.cbHuXiCount+m_bUserHuXiCount[wChairID])-15)/3+1)*m_pGameServiceOption->lCellScore;

				//ͳ�ƻ���
				GameEnd.lGameScore[wChairID]=2*lScore;
				GameEnd.lGameScore[(wChairID+1)%m_wPlayerCount]=-lScore;
				GameEnd.lGameScore[(wChairID+2)%m_wPlayerCount]=-lScore;

				//����ׯ��
				m_wBankerUser=wChairID;
			}
			else 
			{
				//����ׯ��
				m_wBankerUser=(m_wBankerUser+m_wPlayerCount-1)%m_wPlayerCount;
			}

			//������Ϣ
			CMD_S_GameEnd GameEndTemp;
			GameEndTemp=GameEnd;
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
			//	GameEndTemp.lGameScore[i]=m_pITableFrame->AccountGameGold(GameEndTemp.lGameScore[i]);
			}

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEndTemp,sizeof(GameEndTemp));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEndTemp,sizeof(GameEndTemp));

			//�޸Ļ���
			tagScoreInfo ScoreInfo[m_wPlayerCount];
			memset(&ScoreInfo,0,sizeof(ScoreInfo));
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				ScoreInfo[i].lScore=GameEnd.lGameScore[i];
				ScoreInfo[i].ScoreKind=(GameEnd.lGameScore[i]>0L)?enScoreKind_Win:enScoreKind_Lost;
			}
			//m_pITableFrame->WriteTableScore(ScoreInfo,m_wPlayerCount,GameEnd.lGameTax);

			//������Ϸ
			m_pITableFrame->ConcludeGame();

			return true;
		}
	case GER_USER_LEFT:		//�û�ǿ��
		{
			//Ч�����
			ASSERT(pIServerUserItem!=NULL);
			ASSERT(wChairID<m_wPlayerCount);

			//��������
			CMD_S_GameEnd GameEnd;
			memset(&GameEnd,0,sizeof(GameEnd));

			//�����û�
			GameEnd.cbHuCard=0;
			GameEnd.cbReason=cbReason;
			GameEnd.wWinUser=INVALID_CHAIR;
			GameEnd.wProvideUser=INVALID_CHAIR;

			//ͳ�ƻ���
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if (i==wChairID) GameEnd.lGameScore[i]=-20*m_pGameServiceOption->lCellScore;
				else GameEnd.lGameScore[i]=10*m_pGameServiceOption->lCellScore;
			}

			//ʣ���˿�
			BYTE cbPostion=0;
			BYTE cbCardData[MAX_COUNT];
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				//ת���˿�
				BYTE cbCardCount=m_GameLogic.SwitchToCardData(m_cbCardIndex[i],cbCardData,CountArray(cbCardData));

				//�����˿�
				GameEnd.bCardCount[i]=cbCardCount;
				CopyMemory(&GameEnd.bCardData[cbPostion],cbCardData,cbCardCount*sizeof(BYTE));

				//��������
				cbPostion+=cbCardCount;
			}

			//������Ϣ
			CMD_S_GameEnd GameEndTemp;
			GameEndTemp=GameEnd;
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//GameEndTemp.lGameScore[i]=m_pITableFrame->AccountGameGold(GameEndTemp.lGameScore[i]);
			}

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEndTemp,sizeof(GameEndTemp));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEndTemp,sizeof(GameEndTemp));

			//�޸Ļ���
			tagScoreInfo ScoreInfo[m_wPlayerCount];
			memset(&ScoreInfo,0,sizeof(ScoreInfo));
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				ScoreInfo[i].lScore=GameEnd.lGameScore[i];
				ScoreInfo[i].ScoreKind=(GameEnd.lGameScore[i]>0L)?enScoreKind_Win:enScoreKind_Lost;
			}
			//m_pITableFrame->WriteTableScore(ScoreInfo,m_wPlayerCount,GameEnd.lGameTax);

			//�л��û�
			if (wChairID==m_wBankerUser) m_wBankerUser=(m_wBankerUser+m_wPlayerCount-1)%m_wPlayerCount;

			//֪ͨ��Ϣ
			TCHAR szMessage[128]=TEXT("");
			_snprintf(szMessage,CountArray(szMessage),TEXT("���� [ %s ] �뿪��Ϸ����Ϸ����"),pIServerUserItem->GetAccounts());
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				IServerUserItem * pISendUserItem=m_pITableFrame->GetServerUserItem(i);
				if (pISendUserItem!=NULL) m_pITableFrame->SendGameMessage(pISendUserItem,szMessage,SMT_INFO);
			}
			WORD wIndex=0;
			do
			{
				IServerUserItem * pISendUserItem=m_pITableFrame->EnumLookonUserItem(wIndex++);
				if (pISendUserItem==NULL) break;
				m_pITableFrame->SendGameMessage(pISendUserItem,szMessage,SMT_INFO);
			} while (true);

			//������Ϸ
			m_pITableFrame->ConcludeGame();

			return true;
		}
	}

	return false;
}

//���ͳ���
bool __cdecl CTableFrameSink::SendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE bGameStatus, bool bSendSecret)
{
	switch (bGameStatus)
	{
	case GS_FREE:		//����״̬
		{
			//��������
			CMD_S_StatusFree StatusFree;
			memset(&StatusFree,0,sizeof(StatusFree));

			//��������
			StatusFree.wBankerUser=m_wBankerUser;
			StatusFree.lCellScore=m_pGameServiceOption->lCellScore;

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	case GS_PLAYING:	//��Ϸ״̬
		{
			//��������
			CMD_S_StatusPlay StatusPlay;
			memset(&StatusPlay,0,sizeof(StatusPlay));

			//��Ϸ��Ϣ
			StatusPlay.wBankerUser=m_wBankerUser;
			StatusPlay.wCurrentUser=m_wCurrentUser;
			StatusPlay.lCellScore=m_pGameServiceOption->lCellScore;
			
			//������Ϣ
			StatusPlay.wOutCardUser=m_wOutCardUser;
			StatusPlay.cbOutCardData=m_cbOutCardData;
			StatusPlay.bOutCard=(m_bOutCard==true)?TRUE:FALSE;

			//�˿���Ϣ
			StatusPlay.bLeftCardCount=m_bLeftCardCount;
			CopyMemory(StatusPlay.cbCardIndex,m_cbCardIndex[wChiarID],sizeof(m_cbCardIndex[wChiarID]));
			for (WORD i=0;i<m_wPlayerCount;i++) StatusPlay.bUserCardCount[i]=m_GameLogic.GetCardCount(m_cbCardIndex[i]);

			//�����Ϣ
			CopyMemory(StatusPlay.WeaveItemArray,m_WeaveItemArray,sizeof(m_WeaveItemArray));
			CopyMemory(StatusPlay.bWeaveItemCount,m_bWeaveItemCount,sizeof(m_bWeaveItemCount));

			//������Ϣ
			StatusPlay.bUserAction=m_bUserAction[wChiarID];
			StatusPlay.bResponse=(m_bResponse[wChiarID]==true)?TRUE:FALSE;

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));
		}
	}

	//Ч����
	ASSERT(FALSE);

	return false;
}

//��ʱ���¼�
bool __cdecl CTableFrameSink::OnTimerMessage(WORD wTimerID, WPARAM wBindParam)
{
	return false;
}

//��Ϸ��Ϣ����
bool __cdecl CTableFrameSink::OnGameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	switch (wSubCmdID)
	{
	case SUB_C_OUT_CARD:		//������Ϣ
		{
			//Ч����Ϣ
			ASSERT(wDataSize==sizeof(CMD_C_OutCard));
			if (wDataSize!=sizeof(CMD_C_OutCard)) return false;

			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;

			//��Ϣ����
			CMD_C_OutCard * pOutCard=(CMD_C_OutCard *)pDataBuffer;
			return OnUserOutCard(pUserData->wChairID,pOutCard->cbCardData);
		}
	case SUB_C_OPERATE_CARD:	//������Ϣ
		{
			//Ч����Ϣ
			ASSERT(wDataSize==sizeof(CMD_C_OperateCard));
			if (wDataSize!=sizeof(CMD_C_OperateCard)) return false;

			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;

			//��Ϣ����
			CMD_C_OperateCard * pOperateCard=(CMD_C_OperateCard *)pDataBuffer;
			return OnUserOperateCard(pUserData->wChairID,pOperateCard->cbOperateCode,pOperateCard->cbChiKind);
		}
	case SUB_C_CONTINUE_CARD:	//��������
		{
			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;

			//��Ϣ����
			return OnUserContinueCard(pUserData->wChairID);
		}
	}

	return false;
}

//�����Ϣ����
bool __cdecl CTableFrameSink::OnFrameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

//�û�����
bool __cdecl CTableFrameSink::OnEventUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//ׯ������
	if ((bLookonUser==false)&&(m_wBankerUser==INVALID_CHAIR))
	{
		m_wBankerUser=pIServerUserItem->GetChairID();
	}

	return true;
}

//�û�����
bool __cdecl CTableFrameSink::OnEventUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//ׯ������
	if ((bLookonUser==false)&&(wChairID==m_wBankerUser))
	{
		m_wBankerUser=INVALID_CHAIR;

		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			if ((i!=wChairID)&&(m_pITableFrame->GetServerUserItem(i)!=NULL))
			{
				m_wBankerUser=i;
				break;
			}
		}
	}

	return true;
}

//�û�����
bool CTableFrameSink::OnUserOutCard(WORD wChairID, BYTE cbCardData)
{
	//Ч�����
	ASSERT(m_GameLogic.IsValidCard(cbCardData)==true);
	ASSERT((wChairID==m_wCurrentUser)&&(m_bOutCard==true));

	//Ч��״̬
	if (m_GameLogic.IsValidCard(cbCardData)==false) return false;
	if ((wChairID!=m_wCurrentUser)||(m_bOutCard==false)) return false;

	//ɾ���˿�
	if (m_GameLogic.RemoveCard(m_cbCardIndex[wChairID],cbCardData)==false)
	{
		ASSERT(FALSE);
		return false;
	}

	//������Ϣ
	CMD_S_OutCard OutCard;
	OutCard.wOutCardUser=wChairID;
	OutCard.cbOutCardData=cbCardData;
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,sizeof(OutCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,sizeof(OutCard));

	//���ñ���
	m_bOutCard=false;
	m_bDispatch=false;
	m_bEstimate=false;
	m_wOutCardUser=wChairID;
	m_cbOutCardData=cbCardData;
	m_wCurrentUser=INVALID_CHAIR;
	m_wResumeUser=(wChairID+m_wPlayerCount-1)%m_wPlayerCount;

	//���Ƽ�¼
	m_bAbandonCard[wChairID][m_GameLogic.SwitchToCardIndex(m_cbOutCardData)]=true;

	//�����ж�
	BYTE cbCurrentCard=(m_wOutCardUser!=INVALID_CHAIR)?m_cbOutCardData:0;
	bool bUserAction=EstimateUserRespond(m_wOutCardUser,cbCurrentCard,m_bDispatch);

	//�ɷ��˿�
	if (bUserAction==false) DispatchCardData((wChairID+m_wPlayerCount-1)%m_wPlayerCount);

	return true;
}

//�û�����
bool CTableFrameSink::OnUserOperateCard(WORD wChairID, BYTE cbOperateCode, BYTE cbChiKind)
{
	//Ч���û�
	ASSERT(m_wCurrentUser==INVALID_CHAIR);
	if (m_wCurrentUser!=INVALID_CHAIR) return true;

	//Ч��״̬
	ASSERT((m_bResponse[wChairID]==false)&&((cbOperateCode==ACK_NULL)||(m_bUserAction[wChairID]&cbOperateCode)!=0));
	if ((m_bResponse[wChairID]==true)||((cbOperateCode!=ACK_NULL)&&(m_bUserAction[wChairID]&cbOperateCode)==0)) return true;

	//��������
	BYTE cbTargetAction=cbOperateCode;
	WORD wTargetUser=(cbOperateCode==ACK_NULL)?INVALID_CHAIR:wChairID;

	//���ñ���
	m_bResponse[wChairID]=true;
	m_bChiCardKind[wChairID]=cbChiKind;
	m_bPerformAction[wChairID]=cbOperateCode;

	//���ȴ���
	if ((cbOperateCode!=ACK_NULL)&&(m_bUserAction[wChairID]&ACK_CHI_EX)) m_bPerformAction[wChairID]|=ACK_CHI_EX;

	//ִ���ж�
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//��ȡ����
		BYTE cbUserAction=(m_bResponse[i]==false)?m_bUserAction[i]:m_bPerformAction[i];

		//�����ж�
		if (cbUserAction>cbTargetAction)
		{
			wTargetUser=i;
			cbTargetAction=cbUserAction;
		}
	}
	if ((wTargetUser!=INVALID_CHAIR)&&(m_bResponse[wTargetUser]==false)) return true;

	//�Ժ�����
	if (cbTargetAction==ACK_CHIHU)
	{
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			if ((m_bResponse[i]==false)&&(m_bUserAction[i]&ACK_CHIHU)) return true;
		}
	}

	//���Ƽ�¼
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if ((m_bUserAction[i]>cbTargetAction)&&(i!=wTargetUser))
		{
			m_bAbandonCard[i][m_GameLogic.SwitchToCardIndex(m_cbOutCardData)]=true;
		}
	}

	//����״̬
	ZeroMemory(m_bResponse,sizeof(m_bResponse));
	ZeroMemory(m_bUserAction,sizeof(m_bUserAction));
	ZeroMemory(m_bPerformAction,sizeof(m_bPerformAction));

	//���Ʋ���
	if (cbTargetAction&ACK_CHI)
	{
		//��ȡ����
		tagChiCardInfo ChiCardInfo[6];
		BYTE cbChiCardCount=m_GameLogic.GetActionChiCard(m_cbCardIndex[wTargetUser],m_cbOutCardData,ChiCardInfo);

		//��������
		for (BYTE j=0;j<cbChiCardCount;j++)
		{
			if (ChiCardInfo[j].cbChiKind==m_bChiCardKind[wTargetUser]) break;
		}
		if (j==cbChiCardCount) return false;

		//���ƴ���
		BYTE cbDebarCard=m_cbOutCardData;
		for (BYTE k=0;k<ChiCardInfo[j].cbResultCount;k++)
		{
			//�������
			BYTE cbIndex=m_bWeaveItemCount[wTargetUser]++;
			m_WeaveItemArray[wTargetUser][cbIndex].cbCardCount=3;
			m_WeaveItemArray[wTargetUser][cbIndex].cbWeaveKind=ACK_CHI;
			m_WeaveItemArray[wTargetUser][cbIndex].cbCenterCard=m_cbOutCardData;
			m_WeaveItemArray[wTargetUser][cbIndex].cbCardList[0]=ChiCardInfo[j].cbCardData[k][0];
			m_WeaveItemArray[wTargetUser][cbIndex].cbCardList[1]=ChiCardInfo[j].cbCardData[k][1];
			m_WeaveItemArray[wTargetUser][cbIndex].cbCardList[2]=ChiCardInfo[j].cbCardData[k][2];

			//ɾ���˿�
			for (BYTE l=0;l<3;l++)
			{
				BYTE cbCurrentCard=ChiCardInfo[j].cbCardData[k][l];
				if (cbCurrentCard==cbDebarCard) cbDebarCard=0;
				else m_cbCardIndex[wTargetUser][m_GameLogic.SwitchToCardIndex(cbCurrentCard)]--;
			}
		}

		//���º�Ϣ
		UpdateUserHuXiCount(wTargetUser);

		//���ñ���
		m_wOutCardUser=INVALID_CHAIR;

		//��������
		CMD_S_UserChiCard UserChiCard;
		UserChiCard.wActionUser=wTargetUser;
		UserChiCard.cbActionCard=m_cbOutCardData;
		UserChiCard.cbResultCount=ChiCardInfo[j].cbResultCount;
		CopyMemory(UserChiCard.cbCardData,ChiCardInfo[j].cbCardData,sizeof(UserChiCard.cbCardData));

		//��������
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_USER_CHI_CARD,&UserChiCard,sizeof(UserChiCard));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_USER_CHI_CARD,&UserChiCard,sizeof(UserChiCard));

		//������ʾ
		SendOutCardNotify(wTargetUser,true);

		return true;
	}

	//���Ʋ���
	if (cbTargetAction&ACK_PENG)
	{
		//�����˿�
		m_cbCardIndex[wTargetUser][m_GameLogic.SwitchToCardIndex(m_cbOutCardData)]=0;

		//�������
		BYTE cbIndex=m_bWeaveItemCount[wTargetUser]++;
		m_WeaveItemArray[wTargetUser][cbIndex].cbCardCount=3;
		m_WeaveItemArray[wTargetUser][cbIndex].cbWeaveKind=ACK_PENG;
		m_WeaveItemArray[wTargetUser][cbIndex].cbCenterCard=m_cbOutCardData;
		m_WeaveItemArray[wTargetUser][cbIndex].cbCardList[0]=m_cbOutCardData;
		m_WeaveItemArray[wTargetUser][cbIndex].cbCardList[1]=m_cbOutCardData;
		m_WeaveItemArray[wTargetUser][cbIndex].cbCardList[2]=m_cbOutCardData;

		//���º�Ϣ
		UpdateUserHuXiCount(wTargetUser);

		//���ñ���
		m_wOutCardUser=INVALID_CHAIR;

		//��������
		CMD_S_UserPengCard UserPengCard;
		UserPengCard.wActionUser=wTargetUser;
		UserPengCard.cbActionCard=m_cbOutCardData;

		//��������
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_USER_PENG_CARD,&UserPengCard,sizeof(UserPengCard));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_USER_PENG_CARD,&UserPengCard,sizeof(UserPengCard));

		//������ʾ
		SendOutCardNotify(wTargetUser,true);

		return true;
	}

	//�Ժ�����
	if (cbTargetAction&ACK_CHIHU)
	{
		//������Ϸ
		OnEventGameEnd(wTargetUser,NULL,GER_NORMAL);

		return true;
	}

	//�����ж�
	bool bUserAction=false;
	if (m_bEstimate==false)
	{
		BYTE cbCurrentCard=(m_wOutCardUser!=INVALID_CHAIR)?m_cbOutCardData:0;
		bUserAction=EstimateUserRespond(m_wOutCardUser,cbCurrentCard,m_bDispatch);
	}

	//�ɷ��˿�
	if (bUserAction==false) DispatchCardData(m_wResumeUser);

	return true;
}

//��������
bool CTableFrameSink::OnUserContinueCard(WORD wChairID)
{
	//Ч���û�
	ASSERT((wChairID==m_wCurrentUser)&&(m_bOutCard==false));
	if ((wChairID!=m_wCurrentUser)||(m_bOutCard==true)) return false;

	//�ɷ��˿�
	DispatchCardData((wChairID+m_wPlayerCount-1)%m_wPlayerCount);

	return true;
}

//�ɷ��˿�
bool CTableFrameSink::DispatchCardData(WORD wCurrentUser)
{
	//״̬Ч��
	ASSERT(wCurrentUser!=INVALID_CHAIR);
	if (wCurrentUser==INVALID_CHAIR) return false;

	//�����ж�
	if (m_bLeftCardCount==0)
	{
		OnEventGameEnd(INVALID_CHAIR,NULL,GER_NORMAL);
		return true;
	}

	//�����˿�
	BYTE bSendCardData=m_cbRepertoryCard[--m_bLeftCardCount];

	//��������
	CMD_S_SendCard SendCard;
	SendCard.cbCardData=bSendCardData;
	SendCard.wAttachUser=wCurrentUser;
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));

	//���ñ���
	m_bDispatch=true;
	m_bEstimate=false;
	m_wCurrentUser=wCurrentUser;
	m_wOutCardUser=wCurrentUser;
	m_cbOutCardData=bSendCardData;
	m_wResumeUser=(wCurrentUser+m_wPlayerCount-1)%m_wPlayerCount;

	//�����ж�
	bool bUserAction=EstimateUserRespond(wCurrentUser,bSendCardData,m_bDispatch);

	//������ʾ
	if (bUserAction==false) SendOutCardNotify(m_wCurrentUser,false);

	return true;
}

//������ʾ
bool CTableFrameSink::SendOutCardNotify(WORD wCurrentUser, bool bOutCard)
{
	//Ч�����
	ASSERT(wCurrentUser!=INVALID_CHAIR);
	if (wCurrentUser==INVALID_CHAIR) return false;

	//�����ж�
	if (bOutCard==true)
	{
		for (BYTE i=0;i<MAX_INDEX;i++)
		{
			if ((m_cbCardIndex[wCurrentUser][i]>0)&&(m_cbCardIndex[wCurrentUser][i]<3)) break;
		}
		if (i==MAX_INDEX) bOutCard=false;
	}

	//���ñ���
	m_bOutCard=bOutCard;
	m_wCurrentUser=wCurrentUser;

	//��������
	CMD_S_OutCardNotify OutCardNotify;
	OutCardNotify.wCurrentUser=m_wCurrentUser;
	OutCardNotify.bOutCard=(m_bOutCard==true)?TRUE:FALSE;

	//��������
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OUT_CARD_NOTIFY,&OutCardNotify,sizeof(OutCardNotify));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OUT_CARD_NOTIFY,&OutCardNotify,sizeof(OutCardNotify));

	return true;
}

//��Ӧ�ж�
bool CTableFrameSink::EstimateUserRespond(WORD wCenterUser, BYTE cbCenterCard, bool bDispatch)
{
	//ִ�б���
	bool bPerformAction=false;

	//�����ж�
	if ((m_bEstimate==false)&&(bDispatch==true)&&(m_GameLogic.IsTiPaoCard(m_cbCardIndex[wCenterUser],cbCenterCard)==true))
	{
		//��������
		BYTE cbRemoveIndex=m_GameLogic.SwitchToCardIndex(cbCenterCard);
		BYTE cbRemoveCount=m_cbCardIndex[wCenterUser][cbRemoveIndex];

		//ɾ���˿�
		m_cbCardIndex[wCenterUser][cbRemoveIndex]=0;

		//�������
		BYTE cbIndex=m_bWeaveItemCount[wCenterUser]++;
		m_WeaveItemArray[wCenterUser][cbIndex].cbCardCount=4;
		m_WeaveItemArray[wCenterUser][cbIndex].cbWeaveKind=ACK_TI;
		m_WeaveItemArray[wCenterUser][cbIndex].cbCenterCard=m_cbOutCardData;
		m_WeaveItemArray[wCenterUser][cbIndex].cbCardList[0]=m_cbOutCardData;
		m_WeaveItemArray[wCenterUser][cbIndex].cbCardList[1]=m_cbOutCardData;
		m_WeaveItemArray[wCenterUser][cbIndex].cbCardList[2]=m_cbOutCardData;
		m_WeaveItemArray[wCenterUser][cbIndex].cbCardList[3]=m_cbOutCardData;

		//���º�Ϣ
		UpdateUserHuXiCount(wCenterUser);

		//��������
		CMD_S_UserTiCard UserTiCard;
		UserTiCard.wActionUser=wCenterUser;
		UserTiCard.cbActionCard=cbCenterCard;
		UserTiCard.cbRemoveCount=cbRemoveCount;

		//��������
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_USER_TI_CARD,&UserTiCard,sizeof(UserTiCard));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_USER_TI_CARD,&UserTiCard,sizeof(UserTiCard));

		//���ñ���
		cbCenterCard=0;
		bPerformAction=true;
		m_wResumeUser=wCenterUser;
		m_wOutCardUser=INVALID_CHAIR;

		//������ʾ
		m_bTiCard=true;
		m_bSendNotify=true;
	}

	//���Ʊ���
	if ((bPerformAction==false)&&(m_bEstimate==false)&&(bDispatch==true))
	{
		for (BYTE cbIndex=0;cbIndex<m_bWeaveItemCount[wCenterUser];cbIndex++)
		{
			//��������
			BYTE cbWeaveKind=m_WeaveItemArray[wCenterUser][cbIndex].cbWeaveKind;
			BYTE cbWeaveCard=m_WeaveItemArray[wCenterUser][cbIndex].cbCardList[0];

			//ת���ж�
			if ((cbCenterCard!=cbWeaveCard)||(cbWeaveKind!=ACK_WEI)) continue;

			//�������
			m_WeaveItemArray[wCenterUser][cbIndex].cbCardCount=4;
			m_WeaveItemArray[wCenterUser][cbIndex].cbWeaveKind=ACK_TI;
			m_WeaveItemArray[wCenterUser][cbIndex].cbCardList[3]=cbCenterCard;

			//���º�Ϣ
			UpdateUserHuXiCount(wCenterUser);

			//��������
			CMD_S_UserTiCard UserTiCard;
			UserTiCard.cbRemoveCount=0;
			UserTiCard.wActionUser=wCenterUser;
			UserTiCard.cbActionCard=cbCenterCard;

			//��������
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_USER_TI_CARD,&UserTiCard,sizeof(UserTiCard));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_USER_TI_CARD,&UserTiCard,sizeof(UserTiCard));

			//���ñ���
			cbCenterCard=0;
			bPerformAction=true;
			m_wResumeUser=wCenterUser;
			m_wOutCardUser=INVALID_CHAIR;

			//������ʾ
			m_bTiCard=true;
			m_bSendNotify=true;

			break;
		}
	}

	//�����ж�
	if ((bPerformAction==false)&&(bDispatch==true)&&(m_GameLogic.IsWeiPengCard(m_cbCardIndex[wCenterUser],cbCenterCard)==true))
	{
		//�����˿�
		m_cbCardIndex[wCenterUser][m_GameLogic.SwitchToCardIndex(cbCenterCard)]=0;

		//�������
		BYTE cbIndex=m_bWeaveItemCount[wCenterUser]++;
		m_WeaveItemArray[wCenterUser][cbIndex].cbCardCount=3;
		m_WeaveItemArray[wCenterUser][cbIndex].cbWeaveKind=ACK_WEI;
		m_WeaveItemArray[wCenterUser][cbIndex].cbCenterCard=cbCenterCard;
		m_WeaveItemArray[wCenterUser][cbIndex].cbCardList[0]=cbCenterCard;
		m_WeaveItemArray[wCenterUser][cbIndex].cbCardList[1]=cbCenterCard;
		m_WeaveItemArray[wCenterUser][cbIndex].cbCardList[2]=cbCenterCard;

		//���º�Ϣ
		UpdateUserHuXiCount(wCenterUser);

		//��������
		CMD_S_UserWeiCard UserWeiCard;
		UserWeiCard.wActionUser=wCenterUser;
		UserWeiCard.cbActionCard=cbCenterCard;

		//��������
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_USER_WEI_CARD,&UserWeiCard,sizeof(UserWeiCard));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_USER_WEI_CARD,&UserWeiCard,sizeof(UserWeiCard));

		//���ñ���
		cbCenterCard=0;
		bPerformAction=true;
		m_wResumeUser=wCenterUser;
		m_wOutCardUser=INVALID_CHAIR;

		//������ʾ
		m_bTiCard=false;
		m_bSendNotify=true;
	}

	//�����ж�
	if ((m_bEstimate==false)&&(m_bDispatch==true))	
	{
		//��������
		ZeroMemory(m_bResponse,sizeof(m_bResponse));
		ZeroMemory(m_bUserAction,sizeof(m_bUserAction));
		ZeroMemory(m_bChiCardKind,sizeof(m_bChiCardKind));
		ZeroMemory(m_bPerformAction,sizeof(m_bPerformAction));

		//�����ж�
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			//�û�����
			if ((bDispatch==false)&&(i==wCenterUser)) continue;			

			//�����ж�
			tagHuCardInfo HuCardInfo;
			if (m_GameLogic.GetHuCardInfo(m_cbCardIndex[i],cbCenterCard,m_bUserHuXiCount[i],HuCardInfo)==true) m_bUserAction[i]|=ACK_CHIHU;
		}

		//��Ӧ����
		if ((m_bUserAction[0]!=ACK_NULL)||(m_bUserAction[1]!=ACK_NULL)||(m_bUserAction[2]!=ACK_NULL))
		{
			//�����û�
			m_bEstimate=true;
			m_wCurrentUser=INVALID_CHAIR;

			//������Ϣ
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				CMD_S_OperateNotify OperateNotify;
				OperateNotify.wResumeUser=m_wResumeUser;
				OperateNotify.cbActionCard=cbCenterCard;
				OperateNotify.cbOperateCode=m_bUserAction[i];
				m_pITableFrame->SendTableData(i,SUB_S_OPERATE_NOTIFY,&OperateNotify,sizeof(OperateNotify));
			}

			return true;
		}
	}

	//���ñ�־
	m_bEstimate=true;

	//������ʾ
	if (m_bSendNotify==true)
	{
		//������Ϣ
		if (m_bTiCard==false) SendOutCardNotify(m_wResumeUser,true);
		else SendOutCardNotify(m_wResumeUser,IsAllowOutCard(m_wResumeUser));

		//���ñ���
		m_bTiCard=false;
		m_bSendNotify=false;

		return true;
	}

	//�����ж�
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//�û�����
		if (i==wCenterUser) continue;

		//�����ж�
		if (m_GameLogic.IsTiPaoCard(m_cbCardIndex[i],cbCenterCard)==true)
		{
			//��������
			WORD wActionUser=i;
			BYTE cbRemoveIndex=m_GameLogic.SwitchToCardIndex(cbCenterCard);
			BYTE cbRemoveCount=m_cbCardIndex[wActionUser][cbRemoveIndex];

			//ɾ���˿�
			m_cbCardIndex[wActionUser][cbRemoveIndex]=0;

			//�������
			BYTE cbIndex=m_bWeaveItemCount[wActionUser]++;
			m_WeaveItemArray[wActionUser][cbIndex].cbCardCount=4;
			m_WeaveItemArray[wActionUser][cbIndex].cbWeaveKind=ACK_PAO;
			m_WeaveItemArray[wActionUser][cbIndex].cbCenterCard=cbCenterCard;
			m_WeaveItemArray[wActionUser][cbIndex].cbCardList[0]=cbCenterCard;
			m_WeaveItemArray[wActionUser][cbIndex].cbCardList[1]=cbCenterCard;
			m_WeaveItemArray[wActionUser][cbIndex].cbCardList[2]=cbCenterCard;
			m_WeaveItemArray[wActionUser][cbIndex].cbCardList[3]=cbCenterCard;

			//���º�Ϣ
			UpdateUserHuXiCount(wActionUser);

			//��������
			CMD_S_UserPaoCard UserPaoCard;
			UserPaoCard.wActionUser=wActionUser;
			UserPaoCard.cbActionCard=cbCenterCard;
			UserPaoCard.cbRemoveCount=cbRemoveCount;

			//��������
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_USER_PAO_CARD,&UserPaoCard,sizeof(UserPaoCard));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_USER_PAO_CARD,&UserPaoCard,sizeof(UserPaoCard));

			//���ñ���
			m_wOutCardUser=INVALID_CHAIR;

			//������ʾ
			SendOutCardNotify(wActionUser,IsAllowOutCard(wActionUser));

			return true;
		}
	}

	//����ת��
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		for (BYTE cbIndex=0;cbIndex<m_bWeaveItemCount[i];cbIndex++)
		{
			//��������
			BYTE cbWeaveKind=m_WeaveItemArray[i][cbIndex].cbWeaveKind;
			BYTE cbWeaveCard=m_WeaveItemArray[i][cbIndex].cbCardList[0];

			//ת���ж�
			bool bChangeWeave=false;
			if (cbCenterCard==cbWeaveCard)
			{
				if (cbWeaveKind==ACK_WEI) bChangeWeave=true;
				else if ((cbWeaveKind==ACK_PENG)&&(wCenterUser==i)) bChangeWeave=true;
			}
			if (bChangeWeave==false) continue;

			//��������
			WORD wActionUser=i;
			BYTE cbResultWeave=((wCenterUser==i)&&(cbWeaveKind==ACK_WEI))?ACK_TI:ACK_PAO;

			//�������
			m_WeaveItemArray[wActionUser][cbIndex].cbCardCount=4;
			m_WeaveItemArray[wActionUser][cbIndex].cbWeaveKind=cbResultWeave;
			m_WeaveItemArray[wActionUser][cbIndex].cbCardList[3]=cbCenterCard;

			//�����ж�
			if ((m_bDispatch==false)&&(cbResultWeave==ACK_PAO)) m_bMingPao[wCenterUser]=true;

			//���º�Ϣ
			UpdateUserHuXiCount(wActionUser);

			//��������
			if (cbResultWeave==ACK_TI)
			{
				//��������
				CMD_S_UserTiCard UserTiCard;
				UserTiCard.cbRemoveCount=0;
				UserTiCard.wActionUser=wActionUser;
				UserTiCard.cbActionCard=cbCenterCard;

				//��������
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_USER_TI_CARD,&UserTiCard,sizeof(UserTiCard));
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_USER_TI_CARD,&UserTiCard,sizeof(UserTiCard));
			}
			else
			{
				//��������
				CMD_S_UserPaoCard UserPaoCard;
				UserPaoCard.cbRemoveCount=0;
				UserPaoCard.wActionUser=wActionUser;
				UserPaoCard.cbActionCard=cbCenterCard;

				//��������
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_USER_PAO_CARD,&UserPaoCard,sizeof(UserPaoCard));
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_USER_PAO_CARD,&UserPaoCard,sizeof(UserPaoCard));
			}

			//���ñ���
			m_wOutCardUser=INVALID_CHAIR;

			//������ʾ
			SendOutCardNotify(wActionUser,IsAllowOutCard(wActionUser));

			return true;
		}
	}

	//��������
	ZeroMemory(m_bResponse,sizeof(m_bResponse));
	ZeroMemory(m_bUserAction,sizeof(m_bUserAction));
	ZeroMemory(m_bChiCardKind,sizeof(m_bChiCardKind));
	ZeroMemory(m_bPerformAction,sizeof(m_bPerformAction));

	//�����ж�
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//�û�����
		if ((bDispatch==false)&&(i==wCenterUser)) continue;

		//�����ж�
		BYTE cbOperateIndex=m_GameLogic.SwitchToCardIndex(cbCenterCard);
		if ((m_bMingPao[i]==false)&&(m_bAbandonCard[i][cbOperateIndex]==false))
		{
			//�����ж�
			if (m_GameLogic.IsWeiPengCard(m_cbCardIndex[i],cbCenterCard)==true)	m_bUserAction[i]|=ACK_PENG;

			//�����ж�
			WORD wEatUser=(wCenterUser+m_wPlayerCount-1)%m_wPlayerCount;
			if (((wEatUser==i)||(wCenterUser==i))&&(m_GameLogic.IsChiCard(m_cbCardIndex[i],cbCenterCard)==true))
			{
				m_bUserAction[i]|=ACK_CHI;
				if (i==wCenterUser) m_bUserAction[i]|=ACK_CHI_EX;
			}
		}
	}

	//��Ӧ����
	if ((m_bUserAction[0]!=ACK_NULL)||(m_bUserAction[1]!=ACK_NULL)||(m_bUserAction[2]!=ACK_NULL))
	{
		//�����û�
		m_wCurrentUser=INVALID_CHAIR;

		//������Ϣ
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			CMD_S_OperateNotify OperateNotify;
			OperateNotify.wResumeUser=m_wResumeUser;
			OperateNotify.cbActionCard=cbCenterCard;
			OperateNotify.cbOperateCode=m_bUserAction[i];
			m_pITableFrame->SendTableData(i,SUB_S_OPERATE_NOTIFY,&OperateNotify,sizeof(OperateNotify));
		}

		return true;
	}

	return false;
}

//�����ж�
bool CTableFrameSink::IsAllowOutCard(WORD wChairID)
{
	//�������
	BYTE cbTiPaoCount=0;
	for (WORD i=0;i<m_bWeaveItemCount[wChairID];i++)
	{
		if (m_WeaveItemArray[wChairID][i].cbCardCount==4) cbTiPaoCount++;
	}

	return (cbTiPaoCount<=1)?true:false;
}

//���º�Ϣ
bool CTableFrameSink::UpdateUserHuXiCount(WORD wChairID)
{
	//��Ϣ����
	BYTE bUserHuXiCount=0;
	for (BYTE i=0;i<m_bWeaveItemCount[wChairID];i++)
	{
		bUserHuXiCount+=m_GameLogic.GetWeaveHuXi(m_WeaveItemArray[wChairID][i]);
	}
	
	//���ú�Ϣ
	m_bUserHuXiCount[wChairID]=bUserHuXiCount;

	return true;
}

//////////////////////////////////////////////////////////////////////////
