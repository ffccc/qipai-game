#include "StdAfx.h"
#include "TableFrameSink.h"

//////////////////////////////////////////////////////////////////////////

#define RECTIFY_TIME				3										//����ʱ��
#define RULE_STEP_TIME				30										//����ʱ
#define RULE_LIMIT_TIME				1800									//������ʱ
#define RULE_TIME_OUT_COUNT			3										//����ʱ

//////////////////////////////////////////////////////////////////////////

//��̬����
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;			//��Ϸ����
const enStartMode	CTableFrameSink::m_GameStartMode=enStartMode_FullReady;	//��ʼģʽ

//////////////////////////////////////////////////////////////////////////

//���캯��
CTableFrameSink::CTableFrameSink()
{
	//�������
	m_bGameRule=false;
	m_GameRuleInfo.cbDirections=TRUE;
	m_GameRuleInfo.cbRegretFlag=TRUE;
	m_GameRuleInfo.cbGameMode=GM_MATCH;
	m_GameRuleInfo.wRuleStepTime=RULE_STEP_TIME;
	m_GameRuleInfo.wRuleLimitTime=RULE_LIMIT_TIME;
	m_GameRuleInfo.wRuleTimeOutCount=RULE_TIME_OUT_COUNT;

	//ʱ��״̬
	m_dwLastClock=0L;
	memset(m_wUseClock,0,sizeof(m_wUseClock));
	memset(m_wLeftClock,0,sizeof(m_wLeftClock));
	memset(m_wTimeOutCount,0,sizeof(m_wTimeOutCount));

	//��Ϸ����
	m_wBlackUser=0;
	m_wCurrentUser=INVALID_CHAIR;
	memset(m_cbBegStatus,0,sizeof(m_cbBegStatus));
	memset(m_cbPeaceCount,0,sizeof(m_cbPeaceCount));
	memset(m_cbRegretCount,0,sizeof(m_cbRegretCount));

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

	return true;
}

//��λ����
void __cdecl CTableFrameSink::RepositTableFrameSink()
{
	//�������
	m_bGameRule=false;

	//ʱ�����
	m_dwLastClock=0L;
	memset(m_wUseClock,0,sizeof(m_wUseClock));
	memset(m_wLeftClock,0,sizeof(m_wLeftClock));
	memset(m_wTimeOutCount,0,sizeof(m_wTimeOutCount));

	//��Ϸ����
	m_wCurrentUser=INVALID_CHAIR;
	memset(m_cbBegStatus,0,sizeof(m_cbBegStatus));
	memset(m_cbPeaceCount,0,sizeof(m_cbPeaceCount));
	memset(m_cbRegretCount,0,sizeof(m_cbRegretCount));

	//�����߼�
	m_GameLogic.InitGameLogic();

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
	//ʱ�����
	m_wCurrentUser=m_wBlackUser;
	m_dwLastClock=(DWORD)time(NULL);
	m_wLeftClock[0]=m_wLeftClock[1]=m_GameRuleInfo.wRuleLimitTime;
	m_wTimeOutCount[0]=m_wTimeOutCount[1]=m_GameRuleInfo.wRuleTimeOutCount;

	//����״̬
	m_pITableFrame->SetGameStatus(GS_PLAYING);

	//���Ϳ�ʼ
	CMD_S_GameStart GameStart;
	GameStart.wBlackUser=m_wBlackUser;
	GameStart.GameRuleInfo=m_GameRuleInfo;
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));

	return true;
}

//��Ϸ����
bool __cdecl CTableFrameSink::OnEventGameEnd(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_NORMAL:		//�������
		{
			//��������
			CMD_S_GameEnd GameEnd;
			memset(&GameEnd,0,sizeof(GameEnd));

			//ʤ�����
			GameEnd.wWinUser=wChairID;

			//д���ж�
			if (m_GameRuleInfo.cbGameMode==GM_MATCH)
			{
				//����ֲ�
				LONG lUserScore[2];
				IServerUserItem * pIServerUserItem1=m_pITableFrame->GetServerUserItem(0);
				IServerUserItem * pIServerUserItem2=m_pITableFrame->GetServerUserItem(1);
				lUserScore[0]=pIServerUserItem1->GetUserData()->UserScoreInfo.lScore;
				lUserScore[1]=pIServerUserItem2->GetUserData()->UserScoreInfo.lScore;
				LONG lScoreMargin=(lUserScore[0]>lUserScore[1])?(lUserScore[0]-lUserScore[1]):(lUserScore[1]-lUserScore[0]);

				//�������
				if (wChairID!=INVALID_CHAIR)
				{
					//Ӯ�����
					WORD wWinUser=wChairID;
					WORD wLostUser=(wChairID+1)%GAME_PLAYER;
					if (lScoreMargin<100L)
					{
						if (lUserScore[wWinUser]>=lUserScore[wLostUser])
						{
							GameEnd.lUserScore[wWinUser]=10L-lScoreMargin/10L;
							GameEnd.lUserScore[wLostUser]=-GameEnd.lUserScore[wWinUser];
						}
						else
						{
							GameEnd.lUserScore[wWinUser]=10L+lScoreMargin/10L;
							GameEnd.lUserScore[wLostUser]=-GameEnd.lUserScore[wWinUser];
						}
					}
					else if (lScoreMargin<200L)
					{
						GameEnd.lUserScore[wWinUser]=1L;
						GameEnd.lUserScore[wLostUser]=-1L;
					}
					else
					{
						GameEnd.lUserScore[0]=0L;
						GameEnd.lUserScore[1]=0L;
					}

					//д�����
					m_pITableFrame->WriteUserScore((WORD)0, GameEnd.lUserScore[0], 0, wWinUser == 0 ? enScoreKind_Win : enScoreKind_Lost );
					m_pITableFrame->WriteUserScore((WORD)1, GameEnd.lUserScore[1], 0, wWinUser == 1 ? enScoreKind_Win : enScoreKind_Lost );
				}
				else	
				{
					//�������
					if (lScoreMargin<100L)
					{
						GameEnd.lUserScore[0]=(lUserScore[1]-lUserScore[0])/10L;
						GameEnd.lUserScore[1]=-GameEnd.lUserScore[0];
					}
					else if (lScoreMargin<200L)
					{
						GameEnd.lUserScore[0]=(lUserScore[1]-lUserScore[0])?1L:-1L;
						GameEnd.lUserScore[1]=-GameEnd.lUserScore[0];
					}
					else
					{
						GameEnd.lUserScore[0]=0L;
						GameEnd.lUserScore[1]=0L;
					}

					//д�����
					m_pITableFrame->WriteUserScore((WORD)0, GameEnd.lUserScore[0], 0, enScoreKind_Draw );
					m_pITableFrame->WriteUserScore((WORD)1, GameEnd.lUserScore[1], 0, enScoreKind_Draw );
				}
			}

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//������Ϸ
			m_pITableFrame->ConcludeGame();

			//�л��û�
			m_wBlackUser=(m_wBlackUser+1)%GAME_PLAYER;

			//��������
			CMD_S_Req_SetRult SetRult;
			SetRult.GameRuleInfo=m_GameRuleInfo;
			m_pITableFrame->SendTableData(m_wBlackUser,SUB_S_REQ_SET_RULE,&SetRult,sizeof(SetRult));

			return true;
		}
	case GER_USER_LEFT:		//�û�ǿ��
		{
			//��������
			CMD_S_GameEnd GameEnd;
			memset(&GameEnd,0,sizeof(GameEnd));

			//д���ж�
			if (m_GameRuleInfo.cbGameMode==GM_MATCH)
			{
				//���Ļ���
				GameEnd.lUserScore[wChairID]=-30L;
				m_pITableFrame->WriteUserScore(wChairID,GameEnd.lUserScore[wChairID], 0, enScoreKind_Flee);
			}

			//������Ϣ
			GameEnd.wWinUser=INVALID_CHAIR;
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//������Ϸ
			m_pITableFrame->ConcludeGame();

			//�л��û�
			m_wBlackUser=(wChairID+1)%GAME_PLAYER;

			//��������
			CMD_S_Req_SetRult SetRult;
			SetRult.GameRuleInfo=m_GameRuleInfo;
			m_pITableFrame->SendTableData(m_wBlackUser,SUB_S_REQ_SET_RULE,&SetRult,sizeof(SetRult));

			return true;
		}
	}

	//Ч����
	ASSERT(FALSE);

	return false;
}

//���ͳ���
bool __cdecl CTableFrameSink::SendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStation, bool bSendSecret)
{
	switch (cbGameStation)
	{
	case GS_FREE:		//����״̬
		{
			//��������
			CMD_S_StatusFree StatusFree;
			memset(&StatusFree,0,sizeof(StatusFree));

			//�û��л�
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_LOOKON)
			{
				WORD wUserCount=0;
				for (WORD i=0;i<GAME_PLAYER;i++)
				{
					if (m_pITableFrame->GetServerUserItem(i)!=NULL) wUserCount++;
				}
				if (wUserCount==1)
				{
					m_bGameRule=false;
					m_wBlackUser=wChiarID;
				}
			}
			
			//���ñ���
			StatusFree.wBlackUser=m_wBlackUser;
			StatusFree.GameRuleInfo=m_GameRuleInfo;
			StatusFree.cbGameRule=(m_bGameRule==true)?TRUE:FALSE;

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	case GS_PLAYING:	//��Ϸ״̬
		{
			//��������
			WORD wBufferSize=0;
			BYTE cbBuffer[4096];
			for (INT_PTR i=0;i<m_GameLogic.m_ChessManualInfo.GetCount();i++)
			{
				if ((wBufferSize+sizeof(tagChessManual))>sizeof(cbBuffer))
				{
					m_pITableFrame->SendUserData(pIServerUserItem,SUB_S_CHESS_MANUAL,cbBuffer,wBufferSize);
					wBufferSize=0;
				}
				CopyMemory(cbBuffer+wBufferSize,&m_GameLogic.m_ChessManualInfo[i],sizeof(tagChessManual));
				wBufferSize+=sizeof(tagChessManual);
			}
			if (wBufferSize>0) m_pITableFrame->SendUserData(pIServerUserItem,SUB_S_CHESS_MANUAL,cbBuffer,wBufferSize);

			//���쳡��
			CMD_S_StatusPlay StatusPlay;
			memset(&StatusPlay,0,sizeof(StatusPlay));
			WORD wWhiteUser=(m_wBlackUser+1)%GAME_PLAYER;

			//���ó���
			StatusPlay.wBlackUser=m_wBlackUser;
			StatusPlay.wCurrentUser=m_wCurrentUser;
			StatusPlay.GameRuleInfo=m_GameRuleInfo;
			StatusPlay.wTimeOutCount=m_wTimeOutCount[wChiarID];
			CopyMemory(StatusPlay.cbBegStatus,m_cbBegStatus,sizeof(StatusPlay.cbBegStatus));

			//ʱ�����
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				if (m_wCurrentUser==i)
				{
					WORD wUserClock=(WORD)((DWORD)time(NULL)-m_dwLastClock);
					StatusPlay.wUseClock[i]=m_wUseClock[i]+wUserClock;
					if (m_wLeftClock[i]>wUserClock) StatusPlay.wLeftClock[i]=m_wLeftClock[i]-wUserClock;
				}
				else 
				{
					StatusPlay.wUseClock[i]=m_wUseClock[i];
					StatusPlay.wLeftClock[i]=m_wLeftClock[i];
				}
			}

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));
		}
	}

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
	case SUB_C_PLACE_CHESS:		//��������
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_PlaceChess));
			if (wDataSize!=sizeof(CMD_C_PlaceChess)) return false;

			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;

			//��Ϣ����
			CMD_C_PlaceChess * pPlaceChess=(CMD_C_PlaceChess *)pDataBuffer;
			return OnUserPlaceChess(pUserData->wChairID,pPlaceChess->cbXPos,pPlaceChess->cbYPos);
		}
	case SUB_C_PASS_CHESS:		//��������
		{
			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;

			//��Ϣ����
			return OnUserPassChess(pUserData->wChairID);
		}
	case SUB_C_TIME_OUT:		//��Ϸ��ʱ
		{
			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;

			//��Ϣ����
			return OnUserTimeOut(pUserData->wChairID);
		}
	case SUB_C_REGRET_REQ:		//��������
		{
			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;

			//��Ϣ����
			return OnUserRegretReq(pUserData->wChairID);
		}
	case SUB_C_REGRET_ANSWER:	//����Ӧ��
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_RegretAnswer));
			if (wDataSize!=sizeof(CMD_C_RegretAnswer)) return false;

			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;

			//��Ϣ����
			CMD_C_RegretAnswer * pRegretAnswer=(CMD_C_RegretAnswer *)pDataBuffer;
			return OnUserRegretAnswer(pUserData->wChairID,pRegretAnswer->cbApprove);
		}
	case SUB_C_PEACE_REQ:		//�������
		{
			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;

			//��Ϣ����
			return OnUserPeaceReq(pUserData->wChairID);
		}
	case SUB_C_PEACE_ANSWER:	//���Ӧ��
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_PeaceAnswer));
			if (wDataSize!=sizeof(CMD_C_PeaceAnswer)) return false;

			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;

			//��Ϣ����
			CMD_C_PeaceAnswer * pPeaceAnswer=(CMD_C_PeaceAnswer *)pDataBuffer;
			return OnUserPeaceAnswer(pUserData->wChairID,pPeaceAnswer->cbApprove);
		}
	case SUB_C_GIVEUP_REQ:		//��������
		{
			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;

			//��Ϣ����
			return OnUserGiveUp(pUserData->wChairID);
		}
	case SUB_C_REQ_CHESS_COUNT:	//��Ŀ����
		{
			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;

			//��Ϣ����
			return OnUserChessCount(pUserData->wChairID);
		}
	case SUB_C_UNAGREE_CHESS_COUNT:	//��ͬ���Ŀ
		{
			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;

			//��Ϣ����
			return OnUserUnAgreeChessCount(pUserData->wChairID);
		}
	}

	return false;
}

//�����Ϣ����
bool __cdecl CTableFrameSink::OnFrameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	switch (wSubCmdID)
	{
	case SUB_GF_USER_READY:		//�û�ͬ��
		{
			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus==US_LOOKON) return false;

			//�����û�
			if (pUserData->wChairID==m_wBlackUser)
			{
				//Ч���С
				ASSERT(wDataSize==sizeof(CMD_C_CustomizeRult));
				if (wDataSize!=sizeof(CMD_C_CustomizeRult)) throw 0;

				//Ч��״̬
				ASSERT(m_bGameRule==false);
				if (m_bGameRule!=false) throw 0;

				//��Ϣ����
				CMD_C_CustomizeRult * pCustomizeRult=(CMD_C_CustomizeRult *)pDataBuffer;

				//����Ч��

				//������Ϣ
				m_bGameRule=true;
				m_GameRuleInfo=pCustomizeRult->GameRuleInfo;
			
				//����ͬ��
				WORD wAgreeChairID=(m_wBlackUser+1)%GAME_PLAYER;
				if (m_pITableFrame->GetServerUserItem(wAgreeChairID)!=NULL)
				{
					CMD_S_Req_AgreeRult AgreeRult;
					AgreeRult.GameRuleInfo=m_GameRuleInfo;
					m_pITableFrame->SendTableData(wAgreeChairID,SUB_S_REQ_AGREE_RULE,&AgreeRult,sizeof(AgreeRult));
				}
			}

			//�����û�
			if ((pUserData->wChairID!=m_wBlackUser)&&(m_bGameRule==false)) return true;

			return false;
		}
	case SUB_GF_USER_CHAT:	//�û�����
		{
			//��Ϸ״̬
			if (m_pITableFrame->GetGameStatus()!=GS_PLAYING) return false;

			//��ָֹ��
			if (m_GameRuleInfo.cbDirections==FALSE)
			{
				//Ȩ���ж�
				tagServerUserData * pUserData=pIServerUserItem->GetUserData();
				if (pUserData->dwMasterRight!=0) return false;

				//������Ϣ
				LPCTSTR pszMessage=TEXT("������ҹ����趨����Ϸ���ڽ����У���ֹ��̸�");
				m_pITableFrame->SendGameMessage(pIServerUserItem,pszMessage,SMT_INFO);

				return true;
			}

			return false;
		}
	}

	return false;
}

//�û�����
bool __cdecl CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//�����û�
	if ((bLookonUser==false)&&(wChairID==m_wBlackUser))
	{
		//���ñ���
		m_wBlackUser=(wChairID+1)%GAME_PLAYER;

		//�������
		m_bGameRule=false;
		m_GameRuleInfo.cbDirections=TRUE;
		m_GameRuleInfo.cbRegretFlag=TRUE;
		m_GameRuleInfo.cbGameMode=GM_MATCH;
		m_GameRuleInfo.wRuleStepTime=RULE_STEP_TIME;
		m_GameRuleInfo.wRuleLimitTime=RULE_LIMIT_TIME;
		m_GameRuleInfo.wRuleTimeOutCount=RULE_TIME_OUT_COUNT;

		//��������
		if (m_pITableFrame->GetServerUserItem(m_wBlackUser)!=NULL)
		{
			CMD_S_Req_SetRult SetRult;
			SetRult.GameRuleInfo=m_GameRuleInfo;
			m_pITableFrame->SendTableData(m_wBlackUser,SUB_S_REQ_SET_RULE,&SetRult,sizeof(SetRult));
		}
	}

	return true;
}

//�����¼�
bool CTableFrameSink::OnUserPlaceChess(WORD wChairID, BYTE cbXPos, BYTE cbYPos)
{
	//��Ϸ״̬
	if (m_pITableFrame->GetGameStatus()!=GS_PLAYING) return true;

	//Ч���û�
	ASSERT(m_wCurrentUser==wChairID);
	if (m_wCurrentUser!=wChairID) return false;

	//Ч�����
	ASSERT((cbXPos<19)&&(cbYPos<19));
	if ((cbXPos>=19)||(cbYPos>=19)) return false;

	//Ч������
	ASSERT(m_GameLogic.m_cbChessBorad[cbXPos][cbYPos]==NO_CHESS);
	if (m_GameLogic.m_cbChessBorad[cbXPos][cbYPos]!=NO_CHESS) return false;

	//��������
	WORD wDeadChess[361],wDeadChessCount=0;
	BYTE cbColor=(wChairID==m_wBlackUser)?BLACK_CHESS:WHITE_CHESS;
	WORD wIdentifier=m_GameLogic.PlaceChess(cbXPos,cbYPos,cbColor,wDeadChess,wDeadChessCount);

	//����Ч��
	if ((wIdentifier==0)||(wIdentifier==0xFFFF)) return false;

	//����ʱ��
	DWORD dwNowClock=(DWORD)time(NULL);
	WORD wUseClock=(WORD)(dwNowClock-m_dwLastClock);
	m_wUseClock[m_wCurrentUser]+=(WORD)(dwNowClock-m_dwLastClock);
	if (m_wLeftClock[m_wCurrentUser]>=RECTIFY_TIME)
	{
		m_wLeftClock[m_wCurrentUser]-=__min(wUseClock,m_wLeftClock[m_wCurrentUser]-RECTIFY_TIME);
	}

	//���ñ���
	m_dwLastClock=dwNowClock;
	m_wCurrentUser=(m_wCurrentUser+1)%GAME_PLAYER;

	//��������
	CMD_S_PlaceChess PlaceChess;
	PlaceChess.cbXPos=cbXPos;
	PlaceChess.cbYPos=cbYPos;
	PlaceChess.wPlaceUser=wChairID;
	PlaceChess.wCurrentUser=m_wCurrentUser;
	CopyMemory(PlaceChess.wUseClock,m_wUseClock,sizeof(PlaceChess.wUseClock));
	CopyMemory(PlaceChess.wLeftClock,m_wLeftClock,sizeof(PlaceChess.wLeftClock));

	//������Ϣ
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_PLACE_CHESS,&PlaceChess,sizeof(PlaceChess));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_PLACE_CHESS,&PlaceChess,sizeof(PlaceChess));

	return true;
}

//�����¼�
bool CTableFrameSink::OnUserPassChess(WORD wChairID)
{
	//��Ϸ״̬
	if (m_pITableFrame->GetGameStatus()!=GS_PLAYING) return true;

	//Ч���û�
	ASSERT(m_wCurrentUser==wChairID);
	if (m_wCurrentUser!=wChairID) return false;

	//����ʱ��
	DWORD dwNowClock=(DWORD)time(NULL);
	WORD wUseClock=(WORD)(dwNowClock-m_dwLastClock);
	m_wUseClock[m_wCurrentUser]+=(WORD)(dwNowClock-m_dwLastClock);
	if (m_wLeftClock[m_wCurrentUser]>=RECTIFY_TIME)
	{
		m_wLeftClock[m_wCurrentUser]-=__min(wUseClock,m_wLeftClock[m_wCurrentUser]-RECTIFY_TIME);
	}

	//���ñ���
	m_dwLastClock=dwNowClock;
	m_wCurrentUser=(m_wCurrentUser+1)%GAME_PLAYER;

	//��������
	CMD_S_PassChess PassChess;
	PassChess.wPassUser=wChairID;
	PassChess.wCurrentUser=m_wCurrentUser;
	CopyMemory(PassChess.wUseClock,m_wUseClock,sizeof(PassChess.wUseClock));
	CopyMemory(PassChess.wLeftClock,m_wLeftClock,sizeof(PassChess.wLeftClock));

	//������Ϣ
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_PASS_CHESS,&PassChess,sizeof(PassChess));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_PASS_CHESS,&PassChess,sizeof(PassChess));

	return true;
}

//��ʱ�¼�
bool CTableFrameSink::OnUserTimeOut(WORD wChairID)
{
	//��Ϸ״̬
	if (m_pITableFrame->GetGameStatus()!=GS_PLAYING) return true;

	//Ч��״̬
	ASSERT(wChairID==m_wCurrentUser);
	if (wChairID!=m_wCurrentUser) return false;

	//���ñ���
	if (m_wTimeOutCount[wChairID]>0) m_wTimeOutCount[wChairID]--;
	else OnEventGameEnd((wChairID+1)%GAME_PLAYER,NULL,GER_NORMAL);

	return true;
}

//��������
bool CTableFrameSink::OnUserRegretReq(WORD wChairID)
{
	//��Ϸ״̬
	if (m_pITableFrame->GetGameStatus()!=GS_PLAYING) return true;

	//״̬�ж�
	if ((m_cbBegStatus[wChairID]&GU_WAIT_REGRET)!=0) return false;
	if (m_GameLogic.GetStepCount((wChairID==m_wBlackUser)?BLACK_CHESS:WHITE_CHESS)==0) return false;

	//��������
	if (m_GameRuleInfo.cbRegretFlag==FALSE)
	{
		//������Ϣ
		CMD_S_RegretFaile RegretFaile;
		RegretFaile.cbFaileReason=FR_RULE_LIMIT;
		m_pITableFrame->SendTableData(wChairID,SUB_S_REGRET_FAILE,&RegretFaile,sizeof(RegretFaile));

		//ϵͳ��Ϣ
		LPCTSTR pszMessage=TEXT("���ݹ������ã�������Ϸ��ֹ���塣");
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);
		m_pITableFrame->SendGameMessage(pIServerUserItem,pszMessage,SMT_INFO);

		return true;
	}

	//�жϴ���
	if (m_cbRegretCount[wChairID]==5)
	{
		//������Ϣ
		CMD_S_RegretFaile RegretFaile;
		RegretFaile.cbFaileReason=FR_COUNT_LIMIT;
		m_pITableFrame->SendTableData(wChairID,SUB_S_REGRET_FAILE,&RegretFaile,sizeof(RegretFaile));

		//ϵͳ��Ϣ
		LPCTSTR pszMessage=TEXT("���Ѿ����� [ ���� ] ���� 5 ���ˣ������ٴλ��塣");
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);
		m_pITableFrame->SendGameMessage(pIServerUserItem,pszMessage,SMT_INFO);

		return true;
	}

	//���ñ���
	m_cbRegretCount[wChairID]++;
	m_cbBegStatus[wChairID]|=GU_WAIT_REGRET;

	//������Ϣ
	m_pITableFrame->SendTableData((wChairID+1)%GAME_PLAYER,SUB_S_REGRET_REQ);

	return true;
}

//����Ӧ��
bool CTableFrameSink::OnUserRegretAnswer(WORD wChairID, BYTE cbApprove)
{
	//��Ϸ״̬
	if (m_pITableFrame->GetGameStatus()!=GS_PLAYING) return true;

	//״̬�ж�
	WORD wRegretUser=(wChairID+1)%GAME_PLAYER;
	if ((m_cbBegStatus[wRegretUser]&GU_WAIT_REGRET)==0) return false;

	//���ñ���
	m_cbBegStatus[wRegretUser]&=~GU_WAIT_REGRET;

	//��Ϣ����
	if (cbApprove==FALSE)
	{
		CMD_S_RegretFaile RegretFaile;
		RegretFaile.cbFaileReason=FR_PLAYER_OPPOSE;
		m_pITableFrame->SendTableData(wRegretUser,SUB_S_REGRET_FAILE,&RegretFaile,sizeof(RegretFaile));
		return true;
	}

	//��������
	CMD_S_RegretResult RegretResult;
	memset(&RegretResult,0,sizeof(RegretResult));

	//�������
	WORD wIndex=0;
	BYTE cbRegretColor=(wRegretUser==m_wBlackUser)?BLACK_CHESS:WHITE_CHESS;
	INT_PTR nChessManualCount=m_GameLogic.m_ChessManualInfo.GetCount();
	while (wIndex<nChessManualCount)
	{
		//��ȡ����
		tagChessManual * pChessManual=&m_GameLogic.m_ChessManualInfo[nChessManualCount-wIndex-1];
		if (pChessManual->cbColor==cbRegretColor) 
		{
			RegretResult.wRegretCount++;
			break;
		}
		
		//���ñ���
		wIndex++;
		RegretResult.wRegretCount++;
	}

	//�������
	m_GameLogic.RegretChess(RegretResult.wRegretCount);

	//����ʱ��
	DWORD dwNowClock=(DWORD)time(NULL);
	WORD wUseClock=(WORD)(dwNowClock-m_dwLastClock);
	m_wUseClock[m_wCurrentUser]+=(WORD)(dwNowClock-m_dwLastClock);
	if (m_wLeftClock[m_wCurrentUser]>=RECTIFY_TIME)
	{
		m_wLeftClock[m_wCurrentUser]-=__min(wUseClock,m_wLeftClock[m_wCurrentUser]-RECTIFY_TIME);
	}

	//�л��û�
	m_dwLastClock=dwNowClock;
	if (RegretResult.wRegretCount>0) m_wCurrentUser=wRegretUser;

	//��������
	RegretResult.wRegretUser=wRegretUser;
	RegretResult.wCurrentUser=m_wCurrentUser;
	CopyMemory(RegretResult.wUseClock,m_wUseClock,sizeof(RegretResult.wUseClock));
	CopyMemory(RegretResult.wLeftClock,m_wLeftClock,sizeof(RegretResult.wLeftClock));

	//������Ϣ
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_REGRET_RESULT,&RegretResult,sizeof(RegretResult));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_REGRET_RESULT,&RegretResult,sizeof(RegretResult));

	return true;
}

//�������
bool CTableFrameSink::OnUserPeaceReq(WORD wChairID)
{
	//��Ϸ״̬
	if (m_pITableFrame->GetGameStatus()!=GS_PLAYING) return true;

	//�û�״̬
	ASSERT((m_cbBegStatus[wChairID]&GU_WAIT_PEACE)==0);
	if ((m_cbBegStatus[wChairID]&GU_WAIT_PEACE)!=0) return false;

	//��ʹ���
	if (m_cbPeaceCount[wChairID]>=3)
	{
		LPCTSTR pszMessage=TEXT("��� [ ��� ] �����Ѿ����� 3 ���ˣ������ܴ���");
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);
		m_pITableFrame->SendGameMessage(pIServerUserItem,pszMessage,SMT_INFO);
		return true;
	}

	//���ñ���
	m_cbPeaceCount[wChairID]++;
	m_cbBegStatus[wChairID]|=GU_WAIT_PEACE;

	//������Ϣ
	m_pITableFrame->SendTableData((wChairID+1)%GAME_PLAYER,SUB_S_PEACE_REQ);

	return true;
}

//���Ӧ��
bool CTableFrameSink::OnUserPeaceAnswer(WORD wChairID, BYTE cbApprove)
{
	//��Ϸ״̬
	if (m_pITableFrame->GetGameStatus()!=GS_PLAYING) return true;

	//�û�״̬
	WORD wPeaceUser=(wChairID+1)%GAME_PLAYER;
	if ((m_cbBegStatus[wPeaceUser]&GU_WAIT_PEACE)==0) return true;

	//���ñ���
	m_cbBegStatus[wPeaceUser]&=~GU_WAIT_PEACE;

	//��Ϣ����
	if (cbApprove==FALSE) m_pITableFrame->SendTableData(wPeaceUser,SUB_S_PEACE_ANSWER);
	else OnEventGameEnd(INVALID_CHAIR,NULL,GER_NORMAL);

	return true;
}

//�����¼�
bool CTableFrameSink::OnUserGiveUp(WORD wChairID)
{
	//��Ϸ״̬
	if (m_pITableFrame->GetGameStatus()!=GS_PLAYING) return true;

	//������Ϸ
	OnEventGameEnd((wChairID+1)%GAME_PLAYER,NULL,GER_NORMAL);

	return true;
}

//��Ŀ����
bool CTableFrameSink::OnUserChessCount(WORD wChairID)
{
	//��Ϸ״̬
	if (m_pITableFrame->GetGameStatus()!=GS_PLAYING) return true;

	//�û�״̬
	ASSERT((m_cbBegStatus[wChairID]&GU_WAIT_CHESS_COUNT)==0);
	if ((m_cbBegStatus[wChairID]&GU_WAIT_CHESS_COUNT)!=0) return false;

	//���ñ���
	m_cbBegStatus[wChairID]|=GU_WAIT_CHESS_COUNT;

	//�����ж�
	if ((m_cbBegStatus[0]&GU_WAIT_CHESS_COUNT)&&(m_cbBegStatus[1]&GU_WAIT_CHESS_COUNT))
	{
		//��Ŀͳ��
		tagCountResult CountResult;
		memset(&CountResult,0,sizeof(CountResult));
		if (m_GameLogic.GetChessCountInfo(CountResult)==false) return false;

		//�ж�ʤ��
		WORD wWinUser=INVALID_CHAIR;
		if (CountResult.wChessCount[0]>CountResult.wChessCount[1]) wWinUser=m_wBlackUser;
		else if (CountResult.wChessCount[0]<CountResult.wChessCount[1]) wWinUser=(m_wBlackUser+1)%GAME_PLAYER;

		//��ȡ�û�
		IServerUserItem * pIServerUserItem=NULL;
		if (wWinUser!=INVALID_CHAIR) pIServerUserItem=m_pITableFrame->GetServerUserItem(wWinUser);

		//������Ϸ
		OnEventGameEnd(wWinUser,pIServerUserItem,GER_NORMAL);
	}
	else
	{
		//������Ϣ
		m_pITableFrame->SendTableData((wChairID+1)%GAME_PLAYER,SUB_S_REQ_CHESS_COUNT);
	}

	return true;
}

//��ĿӦ��
bool CTableFrameSink::OnUserUnAgreeChessCount(WORD wChairID)
{
	//��Ϸ״̬
	if (m_pITableFrame->GetGameStatus()!=GS_PLAYING) return true;

	//�û�״̬
	WORD wCountUser=(wChairID+1)%GAME_PLAYER;
	if ((m_cbBegStatus[wCountUser]&GU_WAIT_CHESS_COUNT)==0) return true;

	//���ñ���
	m_cbBegStatus[wCountUser]&=~GU_WAIT_CHESS_COUNT;

	//��Ϣ����
	m_pITableFrame->SendTableData(wCountUser,SUB_S_UNAGREE_CHESS_COUNT);

	return true;
}

//////////////////////////////////////////////////////////////////////////
