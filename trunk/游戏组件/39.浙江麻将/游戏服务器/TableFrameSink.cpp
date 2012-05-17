#include "StdAfx.h"
#include "TableFrameSink.h"
#include <math.h>
#include <assert.h>

//////////////////////////////////////////////////////////////////////////

//��̬����
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;			//��Ϸ����
const enStartMode	CTableFrameSink::m_GameStartMode=enStartMode_FullReady;	//��ʼģʽ

//////////////////////////////////////////////////////////////////////////

//���캯��
CTableFrameSink::CTableFrameSink()
{
	//��Ϸ����
	m_wSiceCount=MAKEWORD(1,1);
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));

	//������Ϣ
	m_bOutCardCount=0;
	ZeroMemory(m_wOutCardUser,sizeof(m_wOutCardUser));
	ZeroMemory(m_cbOutCardData,sizeof(m_cbOutCardData));

	//������Ϣ
	m_cbSendCardData=0;
	m_bSendCardCount=0;
	m_bLeftCardCount=0;
	ZeroMemory(m_cbRepertoryCard,sizeof(m_cbRepertoryCard));
	m_cbKingBrothterPos=0;
	
	//�û�״̬
	ZeroMemory(m_bReady, sizeof(m_bReady));
//	ZeroMemory(m_bChiHu,sizeof(m_bChiHu));
	for(int i=0; i<4; i++)
	{
		m_bChiHu[i] = true;
	}
	ZeroMemory(m_bResponse,sizeof(m_bResponse));
	ZeroMemory(m_bUserAction,sizeof(m_bUserAction));
	ZeroMemory(m_bOperateCard,sizeof(m_bOperateCard));
	ZeroMemory(m_bPerformAction,sizeof(m_bPerformAction));

	//����˿�
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));
	ZeroMemory(m_bWeaveItemCount,sizeof(m_bWeaveItemCount));

	//������Ϣ
	m_LastGangInfo.Reset();

	//������Ϣ
	m_lCellScore = 0L;
	m_bCellScoreChange = false;

	//������Ϣ
	m_HuCourt.Reset();
	ZeroMemory(m_cbBankerContinuCount, sizeof(m_cbBankerContinuCount));
	ZeroMemory(m_lScoreLeft, sizeof(m_lScoreLeft));

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
	QUERYINTERFACE(ITableUserAction,Guid,dwQueryVer);
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

#ifdef MJ_DESPERATE
	//��ȡ��λ����
	m_lCellScore = m_pGameServiceOption->lCellScore;
	for(int i=0; i<4; i++)
	{
		m_lScoreLeft[i] = m_lCellScore*TIME_SCORE_STAKE_TIMES;
	}
	
#endif

	return true;
}

//��λ����
void __cdecl CTableFrameSink::RepositTableFrameSink()
{
	//��Ϸ����
	m_wSiceCount=MAKEWORD(1,1);
	m_wCurrentUser=INVALID_CHAIR;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));

	//������Ϣ
	m_bOutCardCount=0;
	ZeroMemory(m_wOutCardUser,sizeof(m_wOutCardUser));
	ZeroMemory(m_cbOutCardData,sizeof(m_cbOutCardData));

	//������Ϣ
	m_cbSendCardData=0;
	m_bSendCardCount=0;
	m_bLeftCardCount=0;
	ZeroMemory(m_cbRepertoryCard,sizeof(m_cbRepertoryCard));
	
	//�û�״̬
//	ZeroMemory(m_bChiHu,sizeof(m_bChiHu));
	for(int i=0; i<4; i++)
	{
		m_bChiHu[i] = true;
	}
	ZeroMemory(m_bResponse,sizeof(m_bResponse));
	ZeroMemory(m_bUserAction,sizeof(m_bUserAction));
	ZeroMemory(m_bOperateCard,sizeof(m_bOperateCard));
	ZeroMemory(m_bPerformAction,sizeof(m_bPerformAction));

	//����˿�
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));
	ZeroMemory(m_bWeaveItemCount,sizeof(m_bWeaveItemCount));

	//������Ϣ
	m_LastGangInfo.Reset();

	//������Ϣ
    m_HuCourt.Reset();
	ZeroMemory(m_cbBankerContinuCount,sizeof(m_cbBankerContinuCount));
	for(i=0; i<4; i++)
	{
		m_lScoreLeft[i] = m_lCellScore*TIME_SCORE_STAKE_TIMES;
	}


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
	//ȷ���ر����еȴ���ʱ��
	for(int i=0; i<m_wPlayerCount; i++)
	{
		m_pITableFrame->KillGameTimer(i);
	}

	//�������
	srand( (unsigned)time( NULL ) );

	//�����˿�
	m_GameLogic.SetKingCardData(0XFF);
	m_wSiceCount=MAKEWORD(rand()%6+1,rand()%6+1);
	m_bLeftCardCount=CountArray(m_cbRepertoryCard);
	m_GameLogic.RandCardData(m_cbRepertoryCard,CountArray(m_cbRepertoryCard));

	//��������
	m_cbKingBrothterPos = (HIBYTE(m_wSiceCount)+LOBYTE(m_wSiceCount))*4-2;
	BYTE cbKingCardData = m_GameLogic.GetKingFromBrother(m_cbRepertoryCard[m_cbKingBrothterPos]);
	m_GameLogic.SetKingCardData(cbKingCardData);
	//�滻����
	BYTE cbSum = 0;
	if(cbKingCardData != 0XFF)
	{
		for(int i=0; i<136; i++)
		{
			if(m_cbRepertoryCard[i] == cbKingCardData)
			{
				m_cbRepertoryCard[i] = CARD_KING_DATA_MASK;
				cbSum ++;
			}
		}
	}

	
	//�ַ��˿�
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		m_bLeftCardCount-=(MAX_COUNT-1);
		m_GameLogic.SwitchToCardIndex(&m_cbRepertoryCard[m_bLeftCardCount],MAX_COUNT-1,m_cbCardIndex[i]);
	}

	//�����˿�
	BYTE cbSendCardData = _DealOneCard();
	m_cbCardIndex[m_wBankerUser][m_GameLogic.SwitchToCardIndex(cbSendCardData)]++;

	//�����û�
	m_wCurrentUser=m_wBankerUser;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	////���
	//for(int i=0; i<MAX_INDEX; i++)
	//{
	//	m_cbCardIndex[m_wBankerUser][i] = 0;
	//}
	//for(i=0; i<3; i++)
	//{
	//	m_cbCardIndex[m_wBankerUser][i] = 3;
	//}
	//m_cbCardIndex[m_wBankerUser][22] = 2; 
	//m_cbCardIndex[m_wBankerUser][30] = 2;
	//m_cbCardIndex[m_wBankerUser][23] = 1;


	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	//�����˿�
	CMD_S_GameStart GameStart;
	GameStart.wSiceCount=m_wSiceCount;
	GameStart.wBankerUser=m_wBankerUser;
	GameStart.wCurrentUser=m_wCurrentUser;
	GameStart.cbKingCardData = m_GameLogic.GetKingCardData();
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//��������
		m_GameLogic.SwitchToCardData(m_cbCardIndex[i],GameStart.cbCardData,CountArray(GameStart.cbCardData));
	
		//��������
		m_pITableFrame->SendTableData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
		m_pITableFrame->SendLookonData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
	}

	return true;
}

//��Ϸ����
bool __cdecl CTableFrameSink::OnEventGameEnd(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
#ifndef MJ_DESPERATE
	for(int i=0; i<m_wPlayerCount; i++)
	{
		IServerUserItem * pUserItem = m_pITableFrame->GetServerUserItem(i);
		if(pUserItem)
		{
			m_pITableFrame->SetGameTimer(i, 120000, 1, pUserItem->GetUserID());
		}
	}
#endif
	switch (cbReason)
	{
	case GER_NORMAL:		//�������
		{
			return _OnGameEndNormal(wChairID, pIServerUserItem);
		}
	case GER_USER_LEFT:		//�û�ǿ��
		{
			return _OnGameEndAbnormal(wChairID, pIServerUserItem);
		}
	case GER_DISMISS:		//��ɢ����
		{
			return _OnGameEndDismiss();
		}
	}

	return false;
}

//���ͳ���
bool __cdecl CTableFrameSink::SendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	switch (cbGameStatus)
	{
	case GS_FREE:		//����״̬
	case GS_FREE_FAKE:	//α����״̬
		{
			//��������
			CMD_S_StatusFree StatusFree;
			memset(&StatusFree,0,sizeof(StatusFree));

			//��������
			StatusFree.wBankerUser = m_wBankerUser;
			StatusFree.lCellScore = m_lCellScore;	
			CopyMemory(StatusFree.lLeftScore, m_lScoreLeft, sizeof(m_lScoreLeft));

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	case GS_PLAYING:
		{
			//��������
			CMD_S_StatusPlay StatusPlay;
			memset(&StatusPlay,0,sizeof(StatusPlay));

			//��Ϸ����
			StatusPlay.wSiceCount=m_wSiceCount;
			StatusPlay.wBankerUser=m_wBankerUser;
			StatusPlay.wCurrentUser=m_wCurrentUser;
			StatusPlay.lCellScore=m_pGameServiceOption->lCellScore;
			CopyMemory(StatusPlay.lLeftScore, m_lScoreLeft, sizeof(m_lScoreLeft));

			//״̬����
			StatusPlay.cbLeftCardCount=m_bLeftCardCount;
			StatusPlay.cbChihu=(m_bChiHu[wChairID]==true)?TRUE:FALSE;
			StatusPlay.cbResponse=(m_bResponse[wChairID]==true)?TRUE:FALSE;

			//������Ϣ
			StatusPlay.cbOutCardCount=m_bOutCardCount;
			CopyMemory(StatusPlay.wOutCardUser,m_wOutCardUser,sizeof(StatusPlay.wOutCardUser));
			CopyMemory(StatusPlay.cbOutCardData,m_cbOutCardData,sizeof(StatusPlay.cbOutCardData));

			//����˿�
			CopyMemory(StatusPlay.bWeaveCount,m_bWeaveItemCount,sizeof(m_bWeaveItemCount));
			CopyMemory(StatusPlay.WeaveItemArray,m_WeaveItemArray,sizeof(m_WeaveItemArray));

			//�˿�����
			StatusPlay.cbKingCardData = m_GameLogic.GetKingCardData();
			StatusPlay.cbCardCount=m_GameLogic.SwitchToCardData(m_cbCardIndex[wChairID],StatusPlay.cbCardData,CountArray(StatusPlay.cbCardData));

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));
		}
	}

	return false;
}

//��ʱ���¼�
bool __cdecl CTableFrameSink::OnTimerMessage(WORD wTimerID, WPARAM wBindParam)
{
	//����ȴ���ʱ��
	if(wTimerID < m_wPlayerCount)
	{
		m_pITableFrame->KillGameTimer(wTimerID);

#ifndef MJ_DESPERATE
		IServerUserItem *pUserItem = m_pITableFrame->GetServerUserItem(wTimerID);
		if(pUserItem && pUserItem->GetUserID() == wBindParam
			&& pUserItem->GetUserStatus() < US_READY)
		{
			m_pITableFrame->SendGameMessage(pUserItem, "������2������δ�����ʼ���������Ϸ����ӭ�´ι��٣�",  SMT_CLOSE_GAME|SMT_EJECT);
		}

		return true;
#endif
	}
	else
	{
		switch(wTimerID) 
		{
		case IDI_DECLARE_OVERTIME:
			{
				assert(0 && "����30��û�л�Ӧ��");
				m_pITableFrame->KillGameTimer(IDI_DECLARE_OVERTIME);

				//���˭û�о���ģ����Ϣ�Ա�֤��Ϸ����
				for(int i=0; i<4; i++)
				{
					if(m_bResponse[i] == false)
					{
						OnUserOperateCard(i, ACK_NULL, 0);
					}
				}

				return true;
			}
		}
	}

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
			return OnUserOperateCard(pUserData->wChairID,pOperateCard->cbOperateCode,pOperateCard->cbOperateCard);
		}
	case SUB_C_READY:			//READY�ź�
		{
			//�û�Ч��
			tagServerUserData * pUserData = pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus != US_PLAY && pUserData->cbUserStatus != US_SIT) 
			{
				return true;
			}

			//��Ϣ����
			return OnUserReady(pUserData->wChairID);
		}
	}

	return false;
}

//�����Ϣ����
bool __cdecl CTableFrameSink::OnFrameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

//�û�ͬ��
bool __cdecl  CTableFrameSink::OnActionUserReady(WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize)
{
	return  true;
}

//�û�����
bool __cdecl CTableFrameSink::OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//ׯ������
	if ((bLookonUser==false)&&(m_wBankerUser==INVALID_CHAIR))
	{
		m_wBankerUser=pIServerUserItem->GetChairID();
	}


#ifndef MJ_DESPERATE
	//��Ͷ�ע���� ����Ϫ�齫Ϊ �����˵�1/50 ��
	if(bLookonUser == false)
	{
		LONG lCellScore = m_lCellScore;
		if(lCellScore == 0)
		{
			lCellScore = pIServerUserItem->GetUserScore()->lScore/50;
		}
		else
		{
			lCellScore = __min(pIServerUserItem->GetUserScore()->lScore/50, lCellScore);
		}
		
		lCellScore = __max(1L, lCellScore);
		if(lCellScore != m_lCellScore)
		{
			m_lCellScore = lCellScore;
			//֪ͨ�������
			CMD_S_Stake Msg;
			ZeroMemory(&Msg, sizeof(Msg));
			Msg.lCellScore = m_lCellScore;
			for(WORD i=0; i<m_wPlayerCount; i++)
			{
				if (i!=wChairID && (m_pITableFrame->GetServerUserItem(i)!=0))
				{
					m_pITableFrame->SendTableData(i,SUB_S_STAKE,&Msg,sizeof(Msg));
					m_pITableFrame->SendLookonData(i,SUB_S_STAKE,&Msg,sizeof(Msg));
				}
			}	
		}
		//���ȴ���ʱ��
		if(pIServerUserItem)
		{
			m_pITableFrame->SetGameTimer(wChairID, 120000,1, pIServerUserItem->GetUserID());
		}

	}
#endif

	return true;
}

//�û�����
bool __cdecl CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{	
	//ׯ������
	if(bLookonUser == false)
	{
		//���ready�ź�
		ZeroMemory(m_bReady, sizeof(m_bReady));
		//�����ׯ��Ŀ
		ZeroMemory(m_cbBankerContinuCount, sizeof(m_cbBankerContinuCount));
		//��������ׯ��
		if(wChairID == m_wBankerUser)
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
	}

#ifndef MJ_DESPERATE
	//��Ͷ�ע���� ����Ϫ�齫Ϊ �����˵�1/50 ��
	if(bLookonUser == false)
	{
		LONG lCellScore = 0;
		bool bHaveUser = false;
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			if(i!=wChairID)
			{
				IServerUserItem* pIServerUserItem = m_pITableFrame->GetServerUserItem(i);
				if(pIServerUserItem != 0)
				{
					bHaveUser = true;
					if(lCellScore == 0)
					{
						lCellScore = pIServerUserItem->GetUserScore()->lScore/50;
					}
					else
					{
						lCellScore = __min(pIServerUserItem->GetUserScore()->lScore/50, lCellScore);
					}					
				}
			}
		}
		if(bHaveUser)
		{
			lCellScore = __max(1L, lCellScore);
			if(lCellScore != m_lCellScore)
			{
				m_lCellScore = lCellScore;
				//֪ͨ�������
				CMD_S_Stake Msg;
				ZeroMemory(&Msg, sizeof(Msg));
				Msg.lCellScore = m_lCellScore;
				for(WORD i=0; i<m_wPlayerCount; i++)
				{
					if (i!=wChairID&&(m_pITableFrame->GetServerUserItem(i)!=0))
					{
						m_pITableFrame->SendTableData(i,SUB_S_STAKE,&Msg,sizeof(Msg));
						m_pITableFrame->SendLookonData(i,SUB_S_STAKE,&Msg,sizeof(Msg));
					}
				}	
			}
		}
		else
		{
			m_lCellScore = 0;
		}	
	}
#endif

	return true;
}

//�û�����
bool CTableFrameSink::OnUserOutCard(WORD wChairID, BYTE cbCardData)
{
	//Ч����Ϣ
	if (wChairID != m_wCurrentUser)
	{
		return false;
	}
	if (m_GameLogic.IsValidCommonCard(cbCardData) == false)
	{
		return false;
	}

	//ɾ���˿�
	if (m_GameLogic.RemoveCard(m_cbCardIndex[wChairID],cbCardData)==false)
	{
		ASSERT(FALSE);
		return false;
	}

	//���ñ���
	m_bOutCardCount++;
	m_bChiHu[wChairID]=true;
	m_wOutCardUser[m_bOutCardCount-1]=wChairID;
	m_cbOutCardData[m_bOutCardCount-1]=cbCardData;

	//��Ӧ�ж�
	bool bAroseAction=false;
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if (i!=wChairID)
		{
			m_bUserAction[i]=ACK_NULL;
#ifdef MJ_DESPERATE
			//����齫���Ƴ���һ������������
			BYTE cbOperateSum = 0;
			for(WORD j=0; j<m_bWeaveItemCount[i]; j++)
			{
				if(m_WeaveItemArray[i][j].wProvideUser == wChairID)
				{
					cbOperateSum ++;
				}
			}
			if(cbOperateSum < 2)
			{
#endif
				//�����ж�
				m_bUserAction[i] |= m_GameLogic.GetPengCardKind(m_cbCardIndex[i],cbCardData);

				//�����ж�
				WORD wEatUser=(wChairID+m_wPlayerCount-1)%m_wPlayerCount;
				if (wEatUser == i) 
				{
					m_bUserAction[i]|=m_GameLogic.GetEatCardKind(m_cbCardIndex[i],cbCardData);
				}
				//���ж�
				if (m_bLeftCardCount > 1)
				{
					m_bUserAction[i]|=m_GameLogic.GetGangCardKind(m_cbCardIndex[i],cbCardData);
				}
#ifdef MJ_DESPERATE
			}
#endif
			//�����ж�
			if (m_bChiHu[i] == true)
			{
				tagHuInput HuInput;
				HuInput.bJudgeHuOnly		= true;
				HuInput.bJudgeKingWait		= false;
				HuInput.bInsertCurrentCard	= true;
				HuInput.cbCurrentCard		= cbCardData;
				HuInput.pWeaveItemSeq		= m_WeaveItemArray[i];
				HuInput.cbWeaveItemCount	= m_bWeaveItemCount[i];
				HuInput.pCardIndexSeq		= m_cbCardIndex[i];
				tagHuCourt HuCourt;
				if(m_GameLogic.GetHuInfo(&HuInput, HuCourt))
				{
					m_bUserAction[i] |= ACK_CHIHU;
					m_bChiHu[i] = false;
				}
			}

			//���ñ���
			if (m_bUserAction[i]!=ACK_NULL)
			{
				bAroseAction=true;
				m_bResponse[i]=false;
				m_bPerformAction[i]=ACK_NULL;

				
			}
			else
			{
				m_bResponse[i]=true;
				m_bUserAction[i]=ACK_NULL;
				m_bPerformAction[i]=ACK_NULL;
			}

		}
		else
		{
			m_bResponse[i]=true;
			m_bUserAction[i]=ACK_NULL;
			m_bPerformAction[i]=ACK_NULL;
		}
	}

	//�л�����
	BYTE cbSendCard=0;
	if ((bAroseAction==false)&&(m_bLeftCardCount>0))
	{
		//����
		m_bSendCardCount++;
		m_wCurrentUser=(wChairID+m_wPlayerCount-1)%m_wPlayerCount;
		cbSendCard=m_cbSendCardData=_DealOneCard();
		m_cbCardIndex[m_wCurrentUser][m_GameLogic.SwitchToCardIndex(cbSendCard)]++;
	}
	else 	
	{
		m_wCurrentUser=INVALID_CHAIR;

		m_pITableFrame->SetGameTimer(IDI_DECLARE_OVERTIME, 30000, DWORD(-1), 0);
	}

	//��������
	CMD_S_OutCard OutCard;
	OutCard.wOutCardUser=wChairID;
	OutCard.cbOutCardData=cbCardData;
	OutCard.cbCurrentCard=cbSendCard;
	OutCard.wCurrentUser=m_wCurrentUser;

	//������Ϣ
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,sizeof(OutCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,sizeof(OutCard));

	//��ׯ����
	if ((bAroseAction==false)&&(m_wCurrentUser==INVALID_CHAIR))
	{
		m_HuCourt.Reset();
		m_bLeftCardCount=0;
		OnEventGameEnd(INVALID_CHAIR, NULL,GER_NORMAL);
	}

	return true;
}

//�û�����
bool CTableFrameSink::OnUserOperateCard(WORD wChairID, BYTE cbOperateCode, BYTE cbOperateCard)
{
	//��ǰ�û�
	if (m_wCurrentUser==wChairID)
	{
		//Ч�����
		ASSERT(cbOperateCode!=ACK_NULL);
		if (cbOperateCode==ACK_NULL) 
		{
			return false;
		}

		//ִ�ж���
		switch (cbOperateCode)
		{
		case ACK_GANG:		//���ܲ���
			{
				//����Ч��
				ASSERT((m_bLeftCardCount>1)&&(m_GameLogic.IsValidCommonCard(cbOperateCard)==true));
				if ((m_bLeftCardCount==1)||(m_GameLogic.IsValidCommonCard(cbOperateCard)==false)) 
				{
					return false;
				}

				//���Ʒ���
				BYTE cbCardIndex=m_GameLogic.SwitchToCardIndex(cbOperateCard);
				BYTE cbWeaveIndex = 0XFF;
				if (m_cbCardIndex[m_wCurrentUser][cbCardIndex]!=4)
				{
					//Ч����Ŀ
					ASSERT(m_cbCardIndex[m_wCurrentUser][cbCardIndex]==1);
					if (m_cbCardIndex[m_wCurrentUser][cbCardIndex]!=1) //Ҫô�ǰ��ܣ�Ҫô��������
					{
						return false;
					}

					//Ѱ�����
					for (BYTE i=0;i<m_bWeaveItemCount[m_wCurrentUser];i++)
					{
						BYTE cbWeaveKind=m_WeaveItemArray[m_wCurrentUser][i].cbWeaveKind;
						BYTE cbCenterCard=m_WeaveItemArray[m_wCurrentUser][i].cbCenterCard;
						if ((cbCenterCard==cbOperateCard)&&(cbWeaveKind==ACK_PENG))
						{
							cbWeaveIndex=i;
							break;
						}
					}
					
					//Ч�鶯��
					ASSERT(cbWeaveIndex!=0xFF);
					if (cbWeaveIndex==0xFF) 
					{
						return false;
					}

					//����˿�
					//m_WeaveItemArray[m_wCurrentUser][cbWeaveIndex].wProvideUser=wChairID;
					m_WeaveItemArray[m_wCurrentUser][cbWeaveIndex].cbWeaveKind=cbOperateCode;
					m_WeaveItemArray[m_wCurrentUser][cbWeaveIndex].cbCenterCard=cbOperateCard;
				}
				else
				{
					//���ñ���
					cbWeaveIndex=m_bWeaveItemCount[m_wCurrentUser]++;
					m_WeaveItemArray[m_wCurrentUser][cbWeaveIndex].cbWeaveKind=ACK_GANG;
					m_WeaveItemArray[m_wCurrentUser][cbWeaveIndex].wProvideUser=m_wCurrentUser;
				}

				//Ч�鶯��
				ASSERT(cbWeaveIndex!=0xFF);
				if (cbWeaveIndex==0xFF) 
				{
					return false;
				}

				//ɾ���˿�
				m_cbCardIndex[m_wCurrentUser][cbCardIndex]=0;

				//���Ʋ���
				m_bSendCardCount++;
				m_cbSendCardData = _DealOneCard(false);
				m_cbCardIndex[m_wCurrentUser][m_GameLogic.SwitchToCardIndex(m_cbSendCardData)]++;

				//������Ϣ
				m_LastGangInfo.cbGangCardData		= cbOperateCard;
				m_LastGangInfo.wPerformUser			= m_wCurrentUser;
				m_LastGangInfo.wProvideUser			= m_WeaveItemArray[m_wCurrentUser][cbWeaveIndex].wProvideUser;
				m_LastGangInfo.cbSendData			= m_cbSendCardData;
				m_LastGangInfo.cbCurrentOutCount	= m_bOutCardCount;

				//������Ϣ
				CMD_S_OperateCard OperateCard;
				OperateCard.wProvideUser=wChairID;
				OperateCard.wOperateUser=m_wCurrentUser;
				OperateCard.wCurrentUser=m_wCurrentUser;
				OperateCard.cbOperateCard=cbOperateCard;
				OperateCard.cbOperateCode=cbOperateCode;
				OperateCard.cbCurrentCard=m_cbSendCardData;

				//������Ϣ
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OPERATE_CARD,&OperateCard,sizeof(OperateCard));
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OPERATE_CARD,&OperateCard,sizeof(OperateCard));

				return true;
			}
		case ACK_CHIHU:		//���Ʋ���
			{
				//��ͨ����
				tagHuInput HuInput;
				HuInput.bJudgeHuOnly		= false;
				HuInput.bJudgeKingWait		= true;
				HuInput.bInsertCurrentCard	= false;
				HuInput.cbCurrentCard		= m_cbSendCardData;
				HuInput.pWeaveItemSeq		= m_WeaveItemArray[wChairID];
				HuInput.cbWeaveItemCount	= m_bWeaveItemCount[wChairID];
				HuInput.pCardIndexSeq		= m_cbCardIndex[wChairID];
				tagHuCourt HuCourt;
				if(m_GameLogic.GetHuInfo(&HuInput, HuCourt))
				{
					//�жϸ��Ӻ� ����
					HuCourt.wHuAttachedType |= EN_HU_ATTACHED_ZIMO;
					//�жϸ��Ӻ� ��غ�
					if ((m_bOutCardCount == 0) && ( m_bSendCardCount == 0))
					{
						HuCourt.wHuAttachedType |= EN_HU_ATTACHED_TIANDI;
					}
					//�жϸ��ϻ�
					if(m_wCurrentUser == m_LastGangInfo.wPerformUser
						&& m_cbSendCardData == m_LastGangInfo.cbSendData
						&& m_bOutCardCount == m_LastGangInfo.cbCurrentOutCount)
					{
						HuCourt.wHuAttachedType |= EN_HU_ATTACHED_GANG_FLOWER;
					}

				}
				//���Ч��
				ASSERT(HuCourt.IsHu());
				if(HuCourt.IsHu() == false)
				{
					return false;
				}
				//������Ϣ
				HuCourt.wWinChair = m_wCurrentUser;
				HuCourt.wFuseChair = m_wCurrentUser;
				HuCourt.cbHuCardData = m_cbSendCardData;
				m_HuCourt = HuCourt;
	
				//������Ϸ
				OnEventGameEnd(HuCourt.wFuseChair,NULL,GER_NORMAL);
				return true;
			}
		}

		//�������
		ASSERT(FALSE);

		return false;
	}

	//�����û�
	if (m_wCurrentUser==INVALID_CHAIR)
	{
		//Ч��״̬
		//ASSERT((m_bResponse[wChairID]==false)&&((cbOperateCode==ACK_NULL)||(m_bUserAction[wChairID]&cbOperateCode)!=0));
		if ((m_bResponse[wChairID]==true)||((cbOperateCode!=ACK_NULL)&&(m_bUserAction[wChairID]&cbOperateCode)==0)) 
		{
			return true;
		}

		//���ñ���
		m_bResponse[wChairID]=true;
		m_bOperateCard[wChairID]=cbOperateCard;
		m_bPerformAction[wChairID]=cbOperateCode;

		//ִ���ж�
		WORD wTargetUser=wChairID;
		BYTE cbTargetAction=cbOperateCode;

		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			//��ȡ����
			BYTE cbUserAction=ACK_NULL;
			if (m_bResponse[i]==false)
			{
				cbUserAction=m_bUserAction[i];
				if (m_bUserAction[i]&ACK_CHIHU)
				{
					cbUserAction&=ACK_CHIHU;
				}
				else if (m_bUserAction[i]&ACK_GANG) 
				{
					cbUserAction&=ACK_GANG;
				}
				else if (m_bUserAction[i]&ACK_PENG) 
				{
					cbUserAction&=ACK_PENG;
				}
				else if (m_bUserAction[i]&(ACK_RIGHT|ACK_CENTER|ACK_LEFT)) 
				{
					m_bUserAction[i]&=(ACK_RIGHT|ACK_CENTER|ACK_LEFT);
				}
			}
			else 
			{
				cbUserAction = m_bPerformAction[i];
				if (cbUserAction&ACK_GANG) 
				{
					cbUserAction=ACK_GANG;
				}
			}

			//�����ж�
			if (cbUserAction>cbTargetAction)
			{
				wTargetUser=i;
				cbTargetAction=cbUserAction;
			}
		}
		if (m_bResponse[wTargetUser]==false) 
		{
			return true;
		}

		//�Ժ���ʱ(һ�ڶ��죩
		//��Ϫ�齫����һ�ڶ��죬��˳�����Ⱥ���
		if (m_bPerformAction[wTargetUser]==ACK_CHIHU)
		{
			ASSERT(m_bOutCardCount > 0);
			WORD wOutCardChair = m_wOutCardUser[m_bOutCardCount-1];
			ASSERT(wOutCardChair != wTargetUser);
			
			WORD wTurnChair = ((wOutCardChair+m_wPlayerCount-1)%m_wPlayerCount);			
			while(wTurnChair != wTargetUser)
			{			
				if ((m_bResponse[wTurnChair]==false)&&(m_bUserAction[wTurnChair]&ACK_CHIHU))
				{
					return true;
				}
				wTurnChair = ((wTurnChair+m_wPlayerCount-1)%m_wPlayerCount);
			}

		}

		//��������
		if (cbTargetAction==ACK_NULL)
		{

			//�����׺�
			if ((m_bOutCardCount==0)&&(m_bSendCardCount==0))
			{
				//�л��û�
				m_wCurrentUser=m_wBankerUser;

				//������Ϣ
				CMD_S_OperateCard OperateCard;
				OperateCard.cbCurrentCard=0;
				OperateCard.cbOperateCard=0;
				OperateCard.cbOperateCode=ACK_NULL;
				OperateCard.wOperateUser=INVALID_CHAIR;
				OperateCard.wProvideUser=INVALID_CHAIR;
				OperateCard.wCurrentUser=m_wCurrentUser;

				//������Ϣ
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OPERATE_CARD,&OperateCard,sizeof(OperateCard));
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OPERATE_CARD,&OperateCard,sizeof(OperateCard));
			}
			else if (m_bLeftCardCount>1)
			{
				//���ñ���
				m_bSendCardCount++;
				m_cbSendCardData= _DealOneCard();
				m_wCurrentUser=(m_wOutCardUser[m_bOutCardCount-1]+m_wPlayerCount-1)%m_wPlayerCount;
				m_cbCardIndex[m_wCurrentUser][m_GameLogic.SwitchToCardIndex(m_cbSendCardData)]++;

				//������Ϣ
				CMD_S_SendCard SendCard;
				SendCard.cbCardData=m_cbSendCardData;
				SendCard.wCurrentUser=m_wCurrentUser;

				//������Ϣ
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
			}
			else
			{
				//��ׯ����
				m_bLeftCardCount=0;
				m_HuCourt.Reset();				
				OnEventGameEnd(m_HuCourt.wFuseChair, NULL,GER_NORMAL);
			}

			//�����������λ�������
			for(i=0; i<m_wPlayerCount; i++)
			{
				m_bResponse[i]=true;
				m_bUserAction[i]=ACK_NULL;
				m_bPerformAction[i]=ACK_NULL;
			}

			m_pITableFrame->KillGameTimer(IDI_DECLARE_OVERTIME);

			return true;
		}

		//�Ժ�����
		if (cbTargetAction==ACK_CHIHU)
		{
			//������Ϣ
			BYTE cbHuCardData =(m_bOutCardCount>0)?m_cbOutCardData[m_bOutCardCount-1]:0;
			ASSERT(cbHuCardData > 0);
			WORD wProvideUser=(m_bOutCardCount>0)?m_wOutCardUser[m_bOutCardCount-1]:INVALID_CHAIR;
			ASSERT(wProvideUser != INVALID_CHAIR);

			//�����ж� �����Ⱥ�)
			WORD wTurnChair = ((wProvideUser+m_wPlayerCount-1)%m_wPlayerCount);			
			while(wTurnChair != wProvideUser)
			{
				if ((m_bResponse[wTurnChair])&&(m_bUserAction[wTurnChair]&ACK_CHIHU))
				{
					break;
				}
				wTurnChair = ((wTurnChair+m_wPlayerCount-1)%m_wPlayerCount);
			}

			ASSERT(wTurnChair != wProvideUser);

			//��ͨ����
			tagHuInput HuInput;
			HuInput.bJudgeHuOnly		= false;
			HuInput.bJudgeKingWait		= false;
			HuInput.bInsertCurrentCard	= true;
			HuInput.cbCurrentCard		= cbHuCardData;
			HuInput.pWeaveItemSeq		= m_WeaveItemArray[wTurnChair];
			HuInput.cbWeaveItemCount	= m_bWeaveItemCount[wTurnChair];
			HuInput.pCardIndexSeq		= m_cbCardIndex[wTurnChair];
			tagHuCourt HuCourt;
			if(m_GameLogic.GetHuInfo(&HuInput, HuCourt))
			{
				//�жϸ��Ӻ� ��غ�
				if ((m_bOutCardCount == 0) && ( m_bSendCardCount == 0))
				{
					HuCourt.wHuAttachedType |= EN_HU_ATTACHED_TIANDI;
				}
			}
			//Ч������
			ASSERT(HuCourt.IsHu());
			if(HuCourt.IsHu() == false)	//թ��
			{
				return  false;
			}

			//������Ϣ
			HuCourt.wWinChair = wTurnChair;
			HuCourt.wFuseChair = wProvideUser;
			HuCourt.cbHuCardData = cbHuCardData;
			m_HuCourt = HuCourt;

			//������Ϸ
			OnEventGameEnd(HuCourt.wFuseChair,NULL,GER_NORMAL);

			//�����������λ�������
			for(i=0; i<m_wPlayerCount; i++)
			{
				m_bResponse[i]=true;
				m_bUserAction[i]=ACK_NULL;
				m_bPerformAction[i]=ACK_NULL;
			}


			m_pITableFrame->KillGameTimer(IDI_DECLARE_OVERTIME);
			return true;
		}

		//�����������λ�������
		for(i=0; i<m_wPlayerCount; i++)
		{
			m_bResponse[i]=true;
			m_bUserAction[i]=ACK_NULL;
			m_bPerformAction[i]=ACK_NULL;
		}

		m_pITableFrame->KillGameTimer(IDI_DECLARE_OVERTIME);

		//��������
		if (cbTargetAction&ACK_GANG) 
		{
			cbTargetAction=ACK_GANG;
		}
		cbOperateCard = (m_bOutCardCount==0)?m_bOperateCard[wTargetUser]:m_cbOutCardData[m_bOutCardCount-1];

		//����˿�
		ASSERT(m_bWeaveItemCount[wTargetUser]<4);
		WORD wIndex=m_bWeaveItemCount[wTargetUser]++;
		m_WeaveItemArray[wTargetUser][wIndex].cbWeaveKind=cbTargetAction;
		m_WeaveItemArray[wTargetUser][wIndex].cbCenterCard=cbOperateCard;
		m_WeaveItemArray[wTargetUser][wIndex].wProvideUser=(m_bOutCardCount>0)?m_wOutCardUser[m_bOutCardCount-1]:wTargetUser;

		//ɾ���˿�
		switch (cbTargetAction)
		{
		case ACK_LEFT:		//���Ʋ���
			{
				//ɾ���˿�
				BYTE cbRemoveCard[]={cbOperateCard+1,cbOperateCard+2};
				m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,CountArray(cbRemoveCard));

				break;
			}
		case ACK_RIGHT:		//���Ʋ���
			{
				//ɾ���˿�
				BYTE cbRemoveCard[]={cbOperateCard-2,cbOperateCard-1};
				m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,CountArray(cbRemoveCard));

				break;
			}
		case ACK_CENTER:	//���Ʋ���
			{
				//ɾ���˿�
				BYTE cbRemoveCard[]={cbOperateCard-1,cbOperateCard+1};
				m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,CountArray(cbRemoveCard));

				break;
			}
		case ACK_PENG:		//���Ʋ���
			{
				//ɾ���˿�
				BYTE cbRemoveCard[]={cbOperateCard,cbOperateCard};
				m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,CountArray(cbRemoveCard));

				break;
			}
		case ACK_GANG:		//���Ʋ���
			{
				//ɾ���˿�
				if (m_bOutCardCount==0)
				{
					BYTE cbRemoveCard[]={cbOperateCard,cbOperateCard,cbOperateCard,cbOperateCard};
					m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,CountArray(cbRemoveCard));
				}
				else
				{
					BYTE cbRemoveCard[]={cbOperateCard,cbOperateCard,cbOperateCard};
					m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,CountArray(cbRemoveCard));
				}

				//������Ϣ
				m_LastGangInfo.cbGangCardData		= cbOperateCard;
				m_LastGangInfo.wPerformUser			= wTargetUser;
				m_LastGangInfo.wProvideUser			= m_WeaveItemArray[wTargetUser][wIndex].wProvideUser;
				m_LastGangInfo.cbCurrentOutCount	= m_bOutCardCount;

				break;
			}
		}

		//���Ʋ���
		BYTE cbCurrentCard=0;
		if (cbTargetAction==ACK_GANG)
		{
			m_bSendCardCount++;
			cbCurrentCard = m_cbSendCardData = _DealOneCard(false);
			m_cbCardIndex[wTargetUser][m_GameLogic.SwitchToCardIndex(cbCurrentCard)]++;

			//������Ϣ
			m_LastGangInfo.cbSendData			= m_cbSendCardData;
		}

		//���ñ���
		m_wCurrentUser=wTargetUser;
		ZeroMemory(m_bResponse,sizeof(m_bResponse));
		ZeroMemory(m_bUserAction,sizeof(m_bUserAction));
		ZeroMemory(m_bOperateCard,sizeof(m_bOperateCard));
		ZeroMemory(m_bPerformAction,sizeof(m_bPerformAction));

		//������Ϣ
		CMD_S_OperateCard OperateCard;
		OperateCard.wOperateUser=wTargetUser;
		OperateCard.wCurrentUser=m_wCurrentUser;
		OperateCard.cbCurrentCard=cbCurrentCard;
		OperateCard.cbOperateCard=cbOperateCard;
		OperateCard.cbOperateCode=cbTargetAction;
		OperateCard.wProvideUser=(m_bOutCardCount>0)?m_wOutCardUser[m_bOutCardCount-1]:wTargetUser;

		//������Ϣ
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OPERATE_CARD,&OperateCard,sizeof(OperateCard));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OPERATE_CARD,&OperateCard,sizeof(OperateCard));

		return true;
	}

	//������� (����֮�󣬳Ե���Ϣ�����ܷ�������
//	ASSERT(FALSE);

	return true;
}


bool CTableFrameSink::_OnGameEndNormal(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	//���ready�ź�
	ZeroMemory(m_bReady, sizeof(m_bReady));
	//��������
	CMD_S_GameEnd GameEnd;
	ZeroMemory(&GameEnd,sizeof(GameEnd));
	GameEnd.stHuCourt.Reset();

	//��ׯ����
	if (m_HuCourt.wFuseChair == INVALID_CHAIR)
	{
		//��ׯ����
		m_wBankerUser=(m_wBankerUser+1)%m_wPlayerCount;
		//�����˿�
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			const BYTE cbMaxCount=CountArray(GameEnd.cbCardData[i]);
			GameEnd.cbCardCount[i]=m_GameLogic.SwitchToCardData(m_cbCardIndex[i],GameEnd.cbCardData[i],cbMaxCount);
		}

#ifdef MJ_DESPERATE
		//ʣ��Ƹ�
		CopyMemory(GameEnd.lLeftScore, m_lScoreLeft, sizeof(m_lScoreLeft));
		//ready����
		GameEnd.cbSelfReady = 1;
		//������Ϣ
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

		//����α����״̬
		m_pITableFrame->SetGameStatus(GS_FREE_FAKE);
		//��λ����
		_OnFakeOver();

		return true;
#else

		//������Ϣ
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
		//������Ϸ
		m_pITableFrame->ConcludeGame();

		return true;
#endif
	}
	//ͳ�ƻ���
	else
	{
#ifdef MJ_DESPERATE	

		//����ȫ���� ʣ����ȫ��Ӯ���
		if(m_HuCourt.wHuCardQueueType & EN_HU_CARD_QUEUE_WW)
		{
			//ׯ������ (Ӯ����ׯ)
			if(m_wBankerUser == m_HuCourt.wWinChair)
			{
				m_cbBankerContinuCount[m_wBankerUser] ++;
				m_HuCourt.cbBankerCount = m_cbBankerContinuCount[m_wBankerUser];
			}
			else
			{
				//�мҺ�����ׯ����ׯ��Ŀ+1
				m_HuCourt.cbBankerCount = m_cbBankerContinuCount[m_wBankerUser] + 1;

				//��ׯ�������ׯ��Ŀ
				ZeroMemory(m_cbBankerContinuCount, sizeof(m_cbBankerContinuCount));
				m_wBankerUser = (m_wBankerUser+1)%m_wPlayerCount;
			}				
			
			//ȫ������
			for(WORD i=0; i<GAME_PLAYER; i++)
			{
				if(i == m_HuCourt.wWinChair)
				{
					continue;
				}
				GameEnd.lGameScore[i] = -m_lScoreLeft[i];
			}
		}
		else

#endif
		
		{
			//�������
			LONG lGain = 0, lTimes = 0;
			m_GameLogic.CalScore(m_HuCourt, lGain, lTimes);

			//ׯ������ (Ӯ����ׯ)
			if(m_wBankerUser == m_HuCourt.wWinChair)
			{
				m_cbBankerContinuCount[m_wBankerUser] ++;
				m_HuCourt.cbBankerCount = m_cbBankerContinuCount[m_wBankerUser];
			}
			else
			{
				//�мҺ�����ׯ����ׯ��Ŀ+1
				m_HuCourt.cbBankerCount = m_cbBankerContinuCount[m_wBankerUser] + 1;

				//��ׯ�������ׯ��Ŀ
				ZeroMemory(m_cbBankerContinuCount, sizeof(m_cbBankerContinuCount));
				m_wBankerUser = (m_wBankerUser+1)%m_wPlayerCount;
			}				

			if(m_HuCourt.cbBankerCount > 0)
			{
				lGain += (m_HuCourt.cbBankerCount-1) * 2;
			}			

			//���ƴ���
			_CheckAfford(m_HuCourt);
			LONG lAffordWeight = 1;
			bool bHaveAfford = false;

			switch(m_HuCourt.AffordItem[0].cbAffordType) 
			{
			case EN_AFFORD_ACCEPT3:		//��������
				bHaveAfford = true;
				lAffordWeight = 4;
				lGain += LONG_AFFORD_ADD_ACCEPT3;
				lTimes += LONG_AFFORD_TIME_ACCEPT3;
				break;
			case EN_AFFORD_GIVE3:		//��������
				bHaveAfford = true;
				lAffordWeight = 4;
				lGain += LONG_AFFORD_ADD_GIVE3;
				lTimes += LONG_AFFORD_TIME_GIVE3;
				break;
			case EN_AFFORD_GANG_APPEND:	//�ܲ�����
				bHaveAfford = true;
				lAffordWeight = 4;
				lGain += LONG_AFFORD_ADD_GANG_APPEND;
				lTimes += LONG_AFFORD_TIME_GANG_APPEND;
				break;
			case EN_AFFORD_HIDE_WIND:	//�ط����
				bHaveAfford = true;
				lAffordWeight = 4;
				lGain += LONG_AFFORD_ADD_HIDE_WIND;
				lTimes += LONG_AFFORD_TIME_HIDE_WIND;
				break;
			}
			
			LONG lWindScore = lGain * m_lCellScore * ((LONG)pow(2, lTimes)) * lAffordWeight;

			//�Ӽ�����
			if(bHaveAfford)
			{
				GameEnd.lGameScore[m_HuCourt.AffordItem[0].wChair] -= lWindScore;
			}
			else
			//��ͨ��
			{
				//����
				if(m_HuCourt.IsZimo())
				{
					for(WORD i=0; i<m_wPlayerCount; i++)
					{
						if(i == m_HuCourt.wWinChair)
						{
							continue;
						}
						GameEnd.lGameScore[i] -= lWindScore;
					}
				}
				//�ں�
				else
				{
					for(WORD i=0; i<m_wPlayerCount; i++)
					{
						if(i == m_HuCourt.wWinChair)
						{
							continue;
						}
						if(i == m_HuCourt.wFuseChair)
						{
							GameEnd.lGameScore[i] -= lWindScore*2;
						}
						else
						{
							GameEnd.lGameScore[i] -= lWindScore;
						}						
					}
				}
			}
		}
	}

	//��������
	GameEnd.stHuCourt = m_HuCourt;

	//�����˿�
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		const BYTE cbMaxCount=CountArray(GameEnd.cbCardData[i]);
		GameEnd.cbCardCount[i]=m_GameLogic.SwitchToCardData(m_cbCardIndex[i],GameEnd.cbCardData[i],cbMaxCount);
	}

	//��������
	IServerUserItem* pUserItem = 0;
	TCHAR szMsg[256];
	bool bHaveLostAll = false;
	bool bLostAll[4];
	ZeroMemory(bLostAll, sizeof(bLostAll));

#ifdef MJ_DESPERATE

	//�������� (�ӷִ���)
	GameEnd.lGameScore[m_HuCourt.wWinChair] = 0;
	for(i=0; i<GAME_PLAYER; i++)
	{
		if(i == m_HuCourt.wWinChair)
		{
			continue;
		}
		LONG lScoreLeft = m_lScoreLeft[i];
		lScoreLeft += GameEnd.lGameScore[i];
		if(lScoreLeft <= 0)
		{			
			bHaveLostAll = true;
			bLostAll[i] = true;
			GameEnd.lGameScore[i] = -m_lScoreLeft[i];
			m_lScoreLeft[i] = 0;
		}
		else
		{
			m_lScoreLeft[i] = lScoreLeft;
		}
		GameEnd.lGameScore[m_HuCourt.wWinChair] -= GameEnd.lGameScore[i];  
	}
	m_lScoreLeft[m_HuCourt.wWinChair] += GameEnd.lGameScore[m_HuCourt.wWinChair];

	//������Ϣ
	CopyMemory(GameEnd.lLeftScore, m_lScoreLeft, sizeof(m_lScoreLeft));
	//���⿪ʼ���
	if(bHaveLostAll == false)
	{
		GameEnd.cbSelfReady = 1;
	}

	//������Ϣ
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

	//д����
	tagScoreInfo ScoreInfo[GAME_PLAYER];
	ZeroMemory(&ScoreInfo,sizeof(ScoreInfo));
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		ScoreInfo[i].lScore = GameEnd.lGameScore[i];
		if (GameEnd.lGameScore[i]==0L) 
		{
			ScoreInfo[i].ScoreKind=enScoreKind_Draw;
		}
		else 
		{
			ScoreInfo[i].ScoreKind=(GameEnd.lGameScore[i]>0L)?enScoreKind_Win:enScoreKind_Lost;
		}
	}
	m_pITableFrame->WriteTableScore(ScoreInfo,m_wPlayerCount,0L);

	//�Ƿ��������
	if(bHaveLostAll)
	{
		TCHAR szUserName[128];
		szUserName[0] = 0;
		//ȡ����������
		for(WORD i=0; i<GAME_PLAYER; i++)
		{	
			pUserItem = m_pITableFrame->GetServerUserItem(i);
			if(pUserItem == 0)
			{
				ASSERT(0 && "ȡ�����Ϣ����");
				continue;
			}

			if(bLostAll[i])	
			{	
				strcat(szUserName, _T(" ["));
				strcat(szUserName, pUserItem->GetAccounts());
				strcat(szUserName, _T("] "));
			}
			
		}
		TCHAR szTmp[64];
		sprintf(szTmp, "�Ѿ����%d���ӣ���ӭ�´ι��٣�", m_pGameServiceOption->lLessScore);
		szMsg[0] = 0;
		strcat(szMsg, _T("��Ϸ���������"));
		strcat(szMsg, szUserName);
		strcat(szMsg, szTmp);

		//֪ͨ���
		for(i=0; i<GAME_PLAYER; i++)
		{
			pUserItem = m_pITableFrame->GetServerUserItem(i);
			if(pUserItem == 0)
			{
				ASSERT(0 && "ȡ�����Ϣ����");
				continue;
			}

			m_pITableFrame->SendGameMessage(pUserItem, szMsg,  SMT_CLOSE_GAME|SMT_EJECT);
		}

		//֪ͨ�Թ���
		WORD wIndex=0;
		do
		{
			pUserItem = m_pITableFrame->EnumLookonUserItem(wIndex++);
			if (pUserItem == 0) 
			{
				break;
			}
			m_pITableFrame->SendGameMessage(pUserItem,szMsg, SMT_CLOSE_GAME|SMT_EJECT);
		} while (true);

		//������Ϸ
		m_pITableFrame->ConcludeGame();

	}
	else
	{
		//����α����״̬
		m_pITableFrame->SetGameStatus(GS_FREE_FAKE);
		//��λ����
		_OnFakeOver();
	}

#else

	//�������� (�ӷִ���)
	GameEnd.lGameScore[m_HuCourt.wWinChair] = 0;
	for(i=0; i<GAME_PLAYER; i++)
	{
		if(i == m_HuCourt.wWinChair)
		{
			continue;
		}
		pUserItem = m_pITableFrame->GetServerUserItem(i);
		if(pUserItem)
		{
			if(GameEnd.lGameScore[i] <= -pUserItem->GetUserScore()->lScore)
			{
				bHaveLostAll = true;
				bLostAll[i] = true;
				GameEnd.lGameScore[i] = -pUserItem->GetUserScore()->lScore;
			}
		}
		
		GameEnd.lGameScore[m_HuCourt.wWinChair] -= GameEnd.lGameScore[i];  
	}

	//������Ϣ
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));


	//д��
	for(int i=0;i<GAME_PLAYER;i++)
	{
		LONG lScore=GameEnd.lGameScore[i];
		LONG lRevenue=0;
		enScoreKind	ScoreKind;
		if(lScore>0)
		{
			ScoreKind=enScoreKind_Win;
			if(m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)
				lRevenue=lScore*m_pGameServiceOption->wRevenue/100;
			lScore-=lRevenue;
		}
		else if(lScore==0) 	ScoreKind=enScoreKind_Draw;
		else				ScoreKind=enScoreKind_Lost;
		
		m_pITableFrame->WriteUserScore(i,lScore,lRevenue,ScoreKind);
	}

	//�Ƿ��������
	if(bHaveLostAll)
	{
		TCHAR szUserName[128];
		szUserName[0] = 0;
		//ȡ����������
		for(WORD i=0; i<GAME_PLAYER; i++)
		{	
			pUserItem = m_pITableFrame->GetServerUserItem(i);
			if(pUserItem == 0)
			{
				ASSERT(0 && "ȡ�����Ϣ����");
				continue;
			}

			if(bLostAll[i])	
			{	
				strcat(szUserName, _T(" ["));
				strcat(szUserName, pUserItem->GetAccounts());
				strcat(szUserName, _T("] "));
			}

		}
		
		//֪ͨ���
		for(i=0; i<GAME_PLAYER; i++)
		{
			pUserItem = m_pITableFrame->GetServerUserItem(i);
			if(pUserItem == 0)
			{
				ASSERT(0 && "ȡ�����Ϣ����");
				continue;
			}
			if(bLostAll[i])
			{
				TCHAR szTmp[64];
				sprintf(szTmp, "�Ѿ�������ӣ���ӭ�´ι��٣�");
				szMsg[0] = 0;
				strcat(szMsg, _T("��Ϸ���������"));
				strcat(szMsg, szUserName);
				strcat(szMsg, szTmp);
				m_pITableFrame->SendGameMessage(pUserItem, szMsg,  SMT_CLOSE_GAME|SMT_EJECT);
			}
			else
			{
				TCHAR szTmp[64];
				sprintf(szTmp, "�Ѿ�������ӣ��˳���Ϸ��");
				szMsg[0] = 0;
				strcat(szMsg, _T("���"));
				strcat(szMsg, szUserName);
				strcat(szMsg, szTmp);
				m_pITableFrame->SendGameMessage(pUserItem, szMsg,  SMT_INFO);
			}			
		}

		TCHAR szTmp[64];
		sprintf(szTmp, "�Ѿ�������ӣ��˳���Ϸ��");
		szMsg[0] = 0;
		strcat(szMsg, _T("���"));
		strcat(szMsg, szUserName);
		strcat(szMsg, szTmp);

		//֪ͨ�Թ���
		WORD wIndex=0;
		do
		{
			pUserItem = m_pITableFrame->EnumLookonUserItem(wIndex++);
			if (pUserItem == 0) 
			{
				break;
			}
			m_pITableFrame->SendGameMessage(pUserItem,szMsg, SMT_INFO);
		} while (true);
	}

	//������Ϸ
	m_pITableFrame->ConcludeGame();

#endif

	return true;
}

bool CTableFrameSink::_OnGameEndAbnormal(WORD wChairID, IServerUserItem * pIServerUserItem)
{	
	//���ready�ź�
	ZeroMemory(m_bReady, sizeof(m_bReady));

	//֪ͨ��Ϣ
	TCHAR szMessage[128]=TEXT("");
	_snprintf(szMessage,CountArray(szMessage),TEXT("���� [ %s ] �뿪��Ϸ����Ϸ������"),pIServerUserItem->GetAccounts());
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

	//���Ļ���
	tagScoreInfo ScoreInfo;
	ScoreInfo.lScore = -10*m_lCellScore;
	if(pIServerUserItem)
	{
		if(ScoreInfo.lScore <= -pIServerUserItem->GetUserScore()->lScore)
		{
			ScoreInfo.lScore = -pIServerUserItem->GetUserScore()->lScore;
		}
	}
	ScoreInfo.ScoreKind = enScoreKind_Flee;


	//��������
	CMD_S_GameEnd GameEnd;
	ZeroMemory(&GameEnd,sizeof(GameEnd));
	GameEnd.stHuCourt.Reset();
	GameEnd.lGameScore[wChairID] = ScoreInfo.lScore;

	m_pITableFrame->WriteUserScore(wChairID,GameEnd.lGameScore[wChairID],0L,enScoreKind_Flee);

	//�����˿�
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		const BYTE cbMaxCount=CountArray(GameEnd.cbCardData[i]);
		GameEnd.cbCardCount[i]=m_GameLogic.SwitchToCardData(m_cbCardIndex[i],GameEnd.cbCardData[i],cbMaxCount);
	}

	//������Ϣ
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

	//������Ϸ
	m_pITableFrame->ConcludeGame();

	return true;

}

bool CTableFrameSink::_OnGameEndDismiss()
{
	//���ready�ź�
	ZeroMemory(m_bReady, sizeof(m_bReady));
	//��������
	CMD_S_GameEnd GameEnd;
	ZeroMemory(&GameEnd,sizeof(GameEnd));
	GameEnd.stHuCourt.Reset();

	//�����˿�
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		const BYTE cbMaxCount=CountArray(GameEnd.cbCardData[i]);
		GameEnd.cbCardCount[i]=m_GameLogic.SwitchToCardData(m_cbCardIndex[i],GameEnd.cbCardData[i],cbMaxCount);
	}

	//������Ϣ
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
	//������Ϸ
	m_pITableFrame->ConcludeGame();

	//֪ͨ��Ϣ
	TCHAR szMessage[128]=TEXT("");
	_snprintf(szMessage,CountArray(szMessage),TEXT("���ڳ����������������Ա��ɢ���ӣ���Ϸ������"));
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		IServerUserItem * pISendUserItem=m_pITableFrame->GetServerUserItem(i);
		if (pISendUserItem!=NULL) m_pITableFrame->SendGameMessage(pISendUserItem,szMessage,SMT_CLOSE_GAME|SMT_EJECT);
	}
	WORD wIndex=0;
	do
	{
		IServerUserItem * pISendUserItem=m_pITableFrame->EnumLookonUserItem(wIndex++);
		if (pISendUserItem==NULL) break;
		m_pITableFrame->SendGameMessage(pISendUserItem,szMessage,SMT_CLOSE_GAME|SMT_EJECT);
	} while (true);

	return true;

}

//������
BYTE CTableFrameSink::_CheckAfford(tagHuCourt& HuCourt)
{
	//Ч������
	if(HuCourt.IsHu() == false || m_bOutCardCount == 0)
	{
		return 0;
	}
	
	//���ƴ���
	BYTE cbAffordCount = 0;

#ifdef MJ_DESPERATE

	//�ط����
	if(m_HuCourt.wHuCardQueueType == EN_HU_CARD_QUEUE_13A
		&& m_GameLogic.IsWindCard(m_HuCourt.cbHuCardData))
	{
		if(m_HuCourt.cbHuCardData != m_cbSendCardData)
		{
			bool bKeepWind = true;
			for(int i=0; i<m_bOutCardCount; i++)
			{
				if(m_wOutCardUser[i] == m_HuCourt.wFuseChair)
				{
					if(m_GameLogic.IsWindCard(m_cbOutCardData[i]) == false)
					{
						bKeepWind = false;
						break;
					}
				}
			}
			if(bKeepWind == false)
			{
				HuCourt.AffordItem[0].cbAffordType = EN_AFFORD_HIDE_WIND;
				HuCourt.AffordItem[0].wChair = HuCourt.wFuseChair;
			}
		}
	}

	if(HuCourt.AffordItem[0].wChair == INVALID_CHAIR)
	{
		//���ϻ�����
		if(m_HuCourt.wFuseChair == m_LastGangInfo.wPerformUser
			&& m_HuCourt.cbHuCardData == m_LastGangInfo.cbSendData
			&& (m_bOutCardCount-1) == m_LastGangInfo.cbCurrentOutCount)
		{
			HuCourt.AffordItem[0].cbAffordType = EN_AFFORD_GANG_APPEND;
			HuCourt.AffordItem[0].wChair = HuCourt.wFuseChair;
		}
	}	

#else

	//��������
	BYTE cbCount[4] = {0,0,0,0};
	for(BYTE i=0; i<m_bWeaveItemCount[m_HuCourt.wWinChair]; i++)
	{
		cbCount[m_WeaveItemArray[m_HuCourt.wWinChair][i].wProvideUser] ++;
	}
	for(i=0; i<sizeof(cbCount); i++)
	{
		if(i == m_HuCourt.wWinChair)
		{
			continue;
		}
		if(cbCount[i] >= 3)
		{
			HuCourt.AffordItem[0].cbAffordType = EN_AFFORD_ACCEPT3;
			HuCourt.AffordItem[0].wChair = i;
			break;
		}
	}
	
	if(HuCourt.AffordItem[0].wChair == INVALID_CHAIR)
	{
		//��������
		ZeroMemory(cbCount, sizeof(cbCount));
		for(i=0; i<m_wPlayerCount; i++)
		{
			if(i == m_HuCourt.wWinChair)
			{
				continue;
			}
			for(BYTE j=0; j<m_bWeaveItemCount[i]; j++)
			{
				if(m_WeaveItemArray[i][j].wProvideUser == m_HuCourt.wWinChair)
				{
					cbCount[i] ++;
				}
			}
		}
		for(i=0; i<sizeof(cbCount); i++)
		{
			if(i == m_HuCourt.wWinChair)
			{
				continue;
			}
			if(cbCount[i] >= 3)
			{
				HuCourt.AffordItem[0].cbAffordType = EN_AFFORD_GIVE3;
				HuCourt.AffordItem[0].wChair = i;
			}
		}
	}	

#endif

	return 1;
}

//ready�ź�
bool CTableFrameSink::OnUserReady(WORD wChairID)
{
	m_bReady[wChairID] = true;
	CMD_S_UserReady Msg;
	Msg.wChairID = wChairID;
	//������Ϣ
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_USER_READY,&Msg,sizeof(Msg));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_USER_READY,&Msg,sizeof(Msg));

	//��鿪ʼ�ź�
	for(BYTE i=0; i<4; i++)
	{
		if(m_bReady[i] == false)
		{
			return true;
		}
	}

	//��ʼ��Ϸ
	OnEventGameStart();

	return true;
}

//�������
void CTableFrameSink::_OnFakeOver()
{
	//��Ϸ����
	m_wSiceCount=MAKEWORD(1,1);
	m_wCurrentUser=INVALID_CHAIR;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));

	//������Ϣ
	m_bOutCardCount=0;
	ZeroMemory(m_wOutCardUser,sizeof(m_wOutCardUser));
	ZeroMemory(m_cbOutCardData,sizeof(m_cbOutCardData));

	//������Ϣ
	m_cbSendCardData=0;
	m_bSendCardCount=0;
	m_bLeftCardCount=0;
	ZeroMemory(m_cbRepertoryCard,sizeof(m_cbRepertoryCard));

	//�û�״̬
	//	ZeroMemory(m_bChiHu,sizeof(m_bChiHu));
	for(int i=0; i<4; i++)
	{
		m_bChiHu[i] = true;
	}
	ZeroMemory(m_bResponse,sizeof(m_bResponse));
	ZeroMemory(m_bUserAction,sizeof(m_bUserAction));
	ZeroMemory(m_bOperateCard,sizeof(m_bOperateCard));
	ZeroMemory(m_bPerformAction,sizeof(m_bPerformAction));

	//����˿�
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));
	ZeroMemory(m_bWeaveItemCount,sizeof(m_bWeaveItemCount));

	//������Ϣ
	m_LastGangInfo.Reset();

	//������Ϣ
	m_HuCourt.Reset();
}

//��һ����
BYTE CTableFrameSink::_DealOneCard(bool bHead)
{
	m_bLeftCardCount --;
	//��������ܵܵ�λ��
	if(bHead)
	{		
		if(m_bLeftCardCount == m_cbKingBrothterPos)
		{
			m_bLeftCardCount --;
			BYTE cbKingBrother = m_GameLogic.GetBrotherFromKing(m_GameLogic.GetKingCardData());
			ASSERT(cbKingBrother == m_cbRepertoryCard[m_bLeftCardCount+1]);
		}

		return (m_cbRepertoryCard[m_bLeftCardCount]);
	}
	else
	{
		static bool bEven = true;
		BYTE cbCardData = 0XFF;
		if(bEven)
		{			
			if(m_cbKingBrothterPos == 1)
			{
				m_cbKingBrothterPos = 0XFF;
				m_bLeftCardCount --;
				cbCardData = m_cbRepertoryCard[0];
				for(int i=0; i<m_bLeftCardCount; i++)
				{
					m_cbRepertoryCard[i] = m_cbRepertoryCard[i+2];
				}				
			}
			else
			{
				m_cbKingBrothterPos --;
				cbCardData = m_cbRepertoryCard[1];
				for(int i=1; i<m_bLeftCardCount; i++)
				{
					m_cbRepertoryCard[i] = m_cbRepertoryCard[i+1];
				}				
			}			
		}
		else
		{
			cbCardData = m_cbRepertoryCard[0];
			for(int i=0; i<m_bLeftCardCount; i++)
			{
				m_cbRepertoryCard[i] = m_cbRepertoryCard[i+1];
			}	
		}

		bEven = !bEven;
		return cbCardData;
	}	
}

//////////////////////////////////////////////////////////////////////////
