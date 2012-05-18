#include "StdAfx.h"
#include "Resource.h"
#include "GameClient.h"
#include "GameClientDlg.h"

//////////////////////////////////////////////////////////////////////////

//��ʱ����
#define MAX_TIME_OUT				3									//��ʱ����

//��Ϸʱ��
#define IDI_OUT_CARD				200									//���ƶ�ʱ��
#define IDI_START_GAME				203									//��ʼ��ʱ��

//��Ϸʱ��
#ifdef _DEBUG
#define TIME_OUT_CARD				300									//���ƶ�ʱ��
#else
#define TIME_OUT_CARD				30									//���ƶ�ʱ��
#endif
#define TIME_START_GAME				30									//��ʼ��ʱ��

//��Ϸʱ��
#define IDI_MOST_CARD				300									//���ʱ��
#define IDI_HIDE_CARD				301									//���ض�ʱ��
#define IDI_LAST_TURN				302									//���ֶ�ʱ��
#define IDI_DISPATCH_CARD			303									//���ƶ�ʱ��


//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientDlg, CGameFrameDlg)
	ON_WM_TIMER()
	ON_MESSAGE(IDM_START,OnMessageStart)
	ON_MESSAGE(IDM_OUT_CARD,OnMessageOutCard)
	ON_MESSAGE(IDM_PASS_CARD,OnMessagePassCard)
	ON_MESSAGE(IDM_OUT_PROMPT,OnMessageOutPrompt)
	ON_MESSAGE(IDM_LAST_TURN_CARD,OnMessageLastTurn)
	ON_MESSAGE(IDM_SORT_HAND_CARD,OnMessageSortCard)
	ON_MESSAGE(IDM_TRUSTEE_CONTROL,OnMessageStustee)
	ON_MESSAGE(IDM_LEFT_HIT_CARD,OnMessageLeftHitCard)
	ON_MESSAGE(WM_ENABLE_HISTORY,OnMessageEnableHistory)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientDlg::CGameClientDlg(): CGameFrameDlg(&m_GameClientView)
{
	//��Ϸ����
	m_cbMainValue=0;
	m_wCurrentUser=INVALID_CHAIR;
	ZeroMemory(m_cbValueOrder,sizeof(m_cbValueOrder));
	ZeroMemory(m_bAutoStatus,sizeof(m_bAutoStatus));
	ZeroMemory(m_510KCardRecord,sizeof(m_510KCardRecord));

	m_bTongHuaCount=0;
	m_bTongHuaIndex=0;
	ZeroMemory(m_bTongHuaCard,sizeof(m_bTongHuaCard));
	m_b510KCount=0;
	m_b510KIndex=0;
	ZeroMemory(m_b510KCard,sizeof(m_b510KCard));
	m_cbOutPromptCardCount = 0;
	m_cbPromptType = 0;
	//ʤ����Ϣ
	m_wWinCount=0;
	ZeroMemory(m_wWinOrder,sizeof(m_wWinOrder));

	//�÷ֱ���
	m_TurnScore=0;
	ZeroMemory(m_PlayerScore,sizeof(m_PlayerScore));

	//�˿˱���
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
	ZeroMemory(m_cbHandCardCount,sizeof(m_cbHandCardCount));

	//���Ʊ���
	m_cbTurnCardCount=0;
	m_cbTurnCardType=CT_ERROR;
	ZeroMemory(m_cbTurnCardData,sizeof(m_cbTurnCardData));
	ZeroMemory(m_cbMagicCardData,sizeof(m_cbMagicCardData));

	//��������
	m_bStustee=false;
	m_bLastTurn=false;
	m_wTimeOutCount=0;
	m_cbSortType=ST_ORDER;
	m_wMostUser=INVALID_CHAIR;
	
	//���Ʊ���
	m_cbRemnantCardCount=0;
	m_cbDispatchCardCount=0;
	ZeroMemory(m_cbDispatchCardData,sizeof(m_cbDispatchCardData));

	m_dwCardHSpace=DEFAULT_PELS;
	return;
}

//��������
CGameClientDlg::~CGameClientDlg()
{
}

//��ʼ����
bool  CGameClientDlg::InitGameFrame()
{
	//����ͼ��
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);

	return true;
}

//��Ϸ����
void CGameClientDlg::OnGameOptionSet()
{
	//��������
	CGameOption GameOption;
	GameOption.m_dwCardHSpace=m_dwCardHSpace;
	GameOption.m_bEnableSound=IsEnableSound();

	//��������
	if (GameOption.DoModal()==IDOK)
	{
		//��ȡ����
		m_dwCardHSpace=GameOption.m_dwCardHSpace;

		//���ÿؼ�
		EnableSound(GameOption.m_bEnableSound);
		m_GameClientView.m_HandCardControl[2].SetCardDistance(m_dwCardHSpace,0,20);

		//��������
		AfxGetApp()->WriteProfileInt(TEXT("GameOption"),TEXT("CardSpace"),m_dwCardHSpace);
	}

	//��������

	return;
}
//�Թ�״̬
void CGameClientDlg::OnLookonChanged(bool bLookonUser, const void * pBuffer, WORD wDataSize)
{
	//�˿˿���
	m_GameClientView.m_HandCardControl[2].SetDisplayItem((IsAllowLookon()==true));

	return;
}

bool CGameClientDlg::OnSubUserAutomatism(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_UserAutomatism));
	if (wDataSize!=sizeof(CMD_S_UserAutomatism)) return false;

	//��Ϣ����
	CMD_S_UserAutomatism * pUserAutomatism =(CMD_S_UserAutomatism *)pBuffer;

	for(WORD i=0;i<GAME_PLAYER;i++)
	{
		m_bAutoStatus[SwitchViewChairID(i)]=pUserAutomatism->bAutoStatus[i];
	}

	m_GameClientView.SetAutoUser(m_bAutoStatus);

	return true;
}
//��������
void  CGameClientDlg::ResetGameFrame()
{
	//�ں�����
	//�÷ֱ���
	m_TurnScore=0;
	ZeroMemory(m_PlayerScore,sizeof(m_PlayerScore));

	//ɾ��ʱ��
	KillTimer(IDI_MOST_CARD);
	KillTimer(IDI_LAST_TURN);
	KillTimer(IDI_DISPATCH_CARD);

	//��Ϸ����
	m_cbMainValue=0;
	m_wCurrentUser=INVALID_CHAIR;
	ZeroMemory(m_cbValueOrder,sizeof(m_cbValueOrder));
	ZeroMemory(m_bAutoStatus,sizeof(m_bAutoStatus));
	ZeroMemory(m_510KCardRecord,sizeof(m_510KCardRecord));

	m_bTongHuaCount=0;
	m_bTongHuaIndex=0;
	ZeroMemory(m_bTongHuaCard,sizeof(m_bTongHuaCard));
	m_b510KCount=0;
	m_b510KIndex=0;
	ZeroMemory(m_b510KCard,sizeof(m_b510KCard));
	m_cbOutPromptCardCount = 0;
	m_cbPromptType = 0;
	//ʤ����Ϣ
	m_wWinCount=0;
	ZeroMemory(m_wWinOrder,sizeof(m_wWinOrder));

	//�˿˱���
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
	ZeroMemory(m_cbHandCardCount,sizeof(m_cbHandCardCount));

	//���Ʊ���
	m_cbTurnCardCount=0;
	m_cbTurnCardType=CT_ERROR;
	ZeroMemory(m_cbTurnCardData,sizeof(m_cbTurnCardData));
	ZeroMemory(m_cbMagicCardData,sizeof(m_cbMagicCardData));

	//��������
	m_bStustee=false;
	m_bLastTurn=false;
	m_wTimeOutCount=0;
	m_cbSortType=ST_ORDER;
	m_wMostUser=INVALID_CHAIR;
	
	//���Ʊ���
	m_cbRemnantCardCount=0;
	m_cbDispatchCardCount=0;
	ZeroMemory(m_cbDispatchCardData,sizeof(m_cbDispatchCardData));

	//��ʷ����
	m_HistoryCard.ResetData();

	return ;
}

//ʱ����Ϣ
bool CGameClientDlg::OnTimerMessage(WORD wChairID, UINT nElapse, UINT nTimerID)
{
	switch (nTimerID)
	{
	case IDI_OUT_CARD:		//�û�����
		{
			//�Զ�����
			if ((((nElapse==0)||((m_bStustee==true)&&(nElapse<TIME_OUT_CARD))))&&(wChairID==GetMeChairID()))
			{
				//��ʱ�ж�
				if ((IsLookonMode()==false)&&(m_bStustee==false))
				{
					//��ʱ�ж�
					BYTE cbCardCount=m_cbHandCardCount[wChairID];
					if ((cbCardCount>3)||(cbCardCount>=m_cbTurnCardCount)) m_wTimeOutCount++;

					//״̬�л�
					if (m_wTimeOutCount>=MAX_TIME_OUT)
					{
						m_wTimeOutCount=0;
						OnMessageStustee(0,0);

						InsertSystemString(TEXT("��������γ�ʱ���л�Ϊ��ϵͳ�йܡ�ģʽ"));
					}
				}

				//�Զ�����
				AutomatismOutCard();

				return true;
			}

			//��ʱ����
			if ((nElapse<=5)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false)) 
			{
				PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));
			}

			return true;
		}
	case IDI_START_GAME:	//��Ϸ��ʼ
		{
			//�ر���Ϸ
			if ((IsLookonMode()==false)&&(nElapse==0))
			{
				OnMessageStart(0,0);
				return true;
			}

			//��ʼ�ж�
			if ((IsLookonMode()==false)&&(nElapse<TIME_START_GAME)&&(m_bStustee==true))
			{
				OnMessageStart(0,0);
				return true;
			}

			//��ʱ����
			if ((IsLookonMode()==false)&&(nElapse<=5))
			{
				PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));
			}

			return true;
		}
	}

	return false;
}

//������Ϣ
bool CGameClientDlg::OnGameMessage(WORD wSubCmdID, const void * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_AUTOMATISM:
		{
			return OnSubUserAutomatism(pData, wDataSize);
		}
	case SUB_S_GAME_START:		//��Ϸ��ʼ
		{
			return OnSubGameStart(pData,wDataSize);
		}
	case SUB_S_OUT_CARD:		//�û�����
		{
			return OnSubOutCard(pData,wDataSize);
		}
	case SUB_S_PASS_CARD:		//�û�����
		{
			return OnSubPassCard(pData,wDataSize);
		}
	case SUB_S_CARD_INFO:		//�˿���Ϣ
		{
			return OnSubCardInfo(pData,wDataSize);
		}
	case SUB_S_GAME_END:		//��Ϸ����
		{
			return OnSubGameEnd(pData,wDataSize);
		}
	case SUB_S_CONTINUE_GAME:	//������Ϸ
		{
			return OnSubContinueGame(pData,wDataSize);
		}
	}

	//�������
	ASSERT(FALSE);

	return true;
}

//��Ϸ����
bool CGameClientDlg::OnGameSceneMessage(BYTE cbGameStatus, bool bLookonOther, const void * pData, WORD wDataSize)
{
	switch (cbGameStatus)
	{
	case GAME_SCENE_FREE:	//����״̬
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_StatusFree));
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;

			//��������
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pData;

			for(WORD i=0;i<GAME_PLAYER;i++)
			{
				m_bAutoStatus[i]=pStatusFree->bAutoStatus[i];
			}

			m_GameClientView.SetAutoUser(m_bAutoStatus);

			//�������
			if (IsLookonMode()==false)
			{
				//���ư�ť
				m_GameClientView.m_btStustee.EnableWindow(TRUE);

				//��ʼ����
				if (GetUserData(GetMeChairID())->cbUserStatus!=US_READY)
				{
					PerformAutoStart();
					m_GameClientView.m_btStart.ShowWindow(TRUE);
					m_GameClientView.m_btStart.SetFocus();
				}
			}

			//���ý���
			m_GameClientView.SetCellScore(pStatusFree->lCellScore);

			return true;
		}
	case GAME_SCENE_PLAY:	//��Ϸ״̬
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_StatusPlay));
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;

			//��������
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pData;

			for(WORD i=0;i<GAME_PLAYER;i++)
			{
				m_bAutoStatus[i]=pStatusPlay->bAutoStatus[i];
			}

			m_GameClientView.SetAutoUser(m_bAutoStatus);

			//��������
			WORD wMeChairID=GetMeChairID();

			//���ñ���
			m_cbMainValue=pStatusPlay->cbMainValue;
			m_wCurrentUser=pStatusPlay->wCurrentUser;
			CopyMemory(m_cbValueOrder,pStatusPlay->cbValueOrder,sizeof(m_cbValueOrder));
			CopyMemory(m_cbHandCardCount,pStatusPlay->cbHandCardCount,sizeof(m_cbHandCardCount));

			//ʤ����Ϣ
			m_wWinCount=pStatusPlay->wWinCount;
			CopyMemory(m_wWinOrder,pStatusPlay->wWinOrder,sizeof(m_wWinOrder));

			//���Ʊ���
			m_cbTurnCardType=pStatusPlay->cbTurnCardType;
			m_cbTurnCardCount=pStatusPlay->cbTurnCardCount;
			CopyMemory(m_cbTurnCardData,pStatusPlay->cbTurnCardData,m_cbTurnCardCount*sizeof(BYTE));
			CopyMemory(m_cbMagicCardData,pStatusPlay->cbMagicCardData,m_cbTurnCardCount*sizeof(BYTE));

			//�˿�����
			WORD wUserIndex=(pStatusPlay->cbHandCardCount[wMeChairID]==0)?(wMeChairID+2)%GAME_PLAYER:wMeChairID;
			CopyMemory(m_cbHandCardData,pStatusPlay->cbHandCardData,sizeof(BYTE)*m_cbHandCardCount[wUserIndex]);

			CopyMemory(m_510KCardRecord,pStatusPlay->b510KCardRecord,sizeof(m_510KCardRecord));
			m_GameClientView.Set510KRecord(m_510KCardRecord);

			//���ý���
			m_GameClientView.SetCellScore(pStatusPlay->lCellScore);
			m_GameClientView.SetMainValue(pStatusPlay->cbMainValue);
			m_GameClientView.SetUserValueOrder(m_cbValueOrder[wMeChairID%2],m_cbValueOrder[(wMeChairID+1)%2]);

			//�����߼�
			m_GameLogic.SetMainValue(m_cbMainValue);

			//��������
			for (WORD i=0;i<m_wWinCount;i++)
			{
				m_GameClientView.SetUserWinOrder(SwitchViewChairID(m_wWinOrder[i]),i+1);
			}

			//������ͼ
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//��ȡλ��
				WORD wViewChairID=SwitchViewChairID(i);

				//�Լ��˿�
				if ((i==wMeChairID)&&(m_cbHandCardCount[wMeChairID]!=0))
				{
					m_GameLogic.SortCardList(m_cbHandCardData,m_cbHandCardCount[i],m_cbSortType);
					m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(m_cbHandCardData,m_cbHandCardCount[i]);
				}

				//ͬ���˿�
				if (i==(wMeChairID+2)%GAME_PLAYER)
				{
					//�����˿�
					if (m_cbHandCardCount[wMeChairID]==0)
					{
						//��ʾ����
						bool bDisplayItem=((IsLookonMode()==false)||(IsAllowLookon()==true));
						m_GameClientView.m_HandCardControl[wViewChairID].SetDisplayItem(bDisplayItem);

						//�����˿�
						m_GameLogic.SortCardList(m_cbHandCardData,m_cbHandCardCount[i],m_cbSortType);
						m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(m_cbHandCardData,m_cbHandCardCount[i]);
					}
					else m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(m_cbHandCardCount[i]);
				}

				//����˿�
				if ((i==(wMeChairID+1)%GAME_PLAYER)||(i==(wMeChairID+3)%GAME_PLAYER))
				{
					m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(m_cbHandCardCount[i]);
				}

				//��������
				m_GameClientView.SetUserPersist(wViewChairID,pStatusPlay->wPersistInfo[i]);
			}

			//���ƽ���
			if (pStatusPlay->wTurnWiner!=INVALID_CHAIR)
			{
				WORD wViewChairID=SwitchViewChairID(pStatusPlay->wTurnWiner);
				m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(m_cbTurnCardData,m_cbTurnCardCount);
			}

			//��ҿ���
			if (IsLookonMode()==false)	
			{
				//���ð�ť
				m_GameClientView.m_btStustee.EnableWindow(TRUE);
				m_GameClientView.m_btSort.EnableWindow(TRUE);
				m_GameClientView.m_btSortCardColor.EnableWindow(TRUE);
				m_GameClientView.m_btSortCardCount.EnableWindow(TRUE);
				m_GameClientView.m_btSortCardOrder.EnableWindow(TRUE);
			}

			//��������
			bool bPositively=(IsLookonMode()==false);
			bool bDisplayItem=((IsLookonMode()==false)||(IsAllowLookon()==true));
			m_GameClientView.m_HandCardControl[MYSELF_VIEW_ID].SetPositively(bPositively);
			m_GameClientView.m_HandCardControl[MYSELF_VIEW_ID].SetDisplayItem(bDisplayItem);

			//��ǰ���
			if ((IsLookonMode()==false)&&(m_wCurrentUser==wMeChairID))
			{
				//��ʾ��ť
				m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
				m_GameClientView.m_btPassCard.ShowWindow(SW_SHOW);
				m_GameClientView.m_btOutPrompt.ShowWindow(SW_SHOW);

				//���ð�ť
				m_GameClientView.m_btOutCard.EnableWindow((VerdictOutCard()==true)?TRUE:FALSE);
				m_GameClientView.m_btPassCard.EnableWindow((VerdictPassCard()==true)?TRUE:FALSE);
			}

			//�˿˿���
			if(IsLookonMode()==true) m_GameClientView.m_HandCardControl[2].SetDisplayItem((IsAllowLookon()==true));

			//���½���
			m_GameClientView.UpdateGameView(NULL);

			//����ʱ��
			if (m_wCurrentUser!=INVALID_CHAIR) SetGameTimer(m_wCurrentUser,IDI_OUT_CARD,TIME_OUT_CARD);

			return true;
		}
	case GAME_SCENE_WAIT:		//�ȴ�״̬
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_StatusWait));
			if (wDataSize!=sizeof(CMD_S_StatusWait)) return false;

			//��������
			CMD_S_StatusWait * pStatusWait=(CMD_S_StatusWait *)pData;

			//��������
			WORD wMeChairID=GetMeChairID();

			//���ñ���
			m_cbMainValue=pStatusWait->cbMainValue;
			CopyMemory(m_cbValueOrder,pStatusWait->cbValueOrder,sizeof(m_cbValueOrder));

			//���ý���
			m_GameClientView.SetCellScore(pStatusWait->lCellScore);
			m_GameClientView.SetMainValue(pStatusWait->cbMainValue);
			m_GameClientView.SetUserValueOrder(m_cbValueOrder[wMeChairID%2],m_cbValueOrder[(wMeChairID+1)%2]);

			for(WORD i=0;i<GAME_PLAYER;i++)
			{
				m_bAutoStatus[i]=pStatusWait->bAutoStatus[i];
			}

			m_GameClientView.SetAutoUser(m_bAutoStatus);

			//�������
			if (IsLookonMode()==false)
			{
				//���ð�ť
				m_GameClientView.m_btStart.ShowWindow(TRUE);
				m_GameClientView.m_btStustee.EnableWindow(TRUE);

				//����ʱ��
				SetGameTimer(GetMeChairID(),IDI_START_GAME,TIME_START_GAME);
			}

			//���ÿ�ʼ
			for (BYTE i=0;i<GAME_PLAYER;i++)
			{
				if (pStatusWait->cbContinue[i]==TRUE)
				{
					m_GameClientView.SetUserContinue(SwitchViewChairID(i),true);
				}
			}

			//���ý���
			m_GameClientView.SetCellScore(pStatusWait->lCellScore);

			return true;
		}
	}

	return false;
}

//��Ϸ��ʼ
bool CGameClientDlg::OnSubGameStart(const void * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;

	//��������
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pData;

	//����״̬
	SetGameStatus(GAME_SCENE_PLAY);

	//��Ϸ����
	m_wTimeOutCount=0;
	m_wCurrentUser=pGameStart->wCurrentUser;

	//�ȼ�����
	m_cbMainValue=pGameStart->cbMainValue;
	m_cbValueOrder[0]=pGameStart->cbValueOrder[0];
	m_cbValueOrder[1]=pGameStart->cbValueOrder[1];

	//ʤ����Ϣ
	m_wWinCount=0;
	ZeroMemory(m_wWinOrder,sizeof(m_wWinOrder));

	//���Ʊ���
	m_cbTurnCardCount=0;
	m_cbSortType=ST_ORDER;
	m_cbTurnCardType=CT_ERROR;
	ZeroMemory(m_cbTurnCardData,sizeof(m_cbTurnCardData));
	ZeroMemory(m_cbMagicCardData,sizeof(m_cbMagicCardData));

	//��������
	m_GameClientView.m_ScoreControl.RestorationData();
	m_GameClientView.m_btSort.EnableWindow(TRUE);
	m_GameClientView.m_btSortCardColor.EnableWindow(TRUE);
	m_GameClientView.m_btSortCardCount.EnableWindow(TRUE);
	m_GameClientView.m_btSortCardOrder.EnableWindow(TRUE);

	//���ý���
	WORD wMeChairID=GetMeChairID();
	m_GameClientView.SetUserWinOrder(INVALID_CHAIR,0);
	m_GameClientView.SetUserContinue(INVALID_CHAIR,false);
	m_GameClientView.SetMainValue(pGameStart->cbMainValue);
	m_GameClientView.SetUserValueOrder(m_cbValueOrder[wMeChairID%2],m_cbValueOrder[(wMeChairID+1)%2]);

	//��ʷ��¼
	m_HistoryCard.ResetData();

	//�����߼�
	m_GameLogic.SetMainValue(m_cbMainValue);

	//����˿�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��ȡλ��
		WORD wViewChairID=SwitchViewChairID(i);

		//�����˿�
		if (wViewChairID==MYSELF_VIEW_ID)
		{
			bool bShowCard=((IsLookonMode()==false)||(IsAllowLookon()==true));
			m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(NULL,0);
			m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(NULL,0);
			m_GameClientView.m_HandCardControl[wViewChairID].SetPositively(false);
			m_GameClientView.m_HandCardControl[wViewChairID].SetDisplayItem(bShowCard);
		}
		else
		{
			m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(NULL,0);
			m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(NULL,0);
			m_GameClientView.m_HandCardControl[wViewChairID].SetDisplayItem(false);
		}

		//��������
		m_GameClientView.SetUserPersist(wViewChairID,pGameStart->wPersistInfo[i]);
	}

	//�����˿�
	for (WORD i=0;i<GAME_PLAYER;i++) m_cbHandCardCount[i]=CountArray(pGameStart->cbCardData);
	CopyMemory(m_cbHandCardData,pGameStart->cbCardData,sizeof(BYTE)*m_cbHandCardCount[wMeChairID]);

	//�����˿�
	m_GameLogic.SortCardList(m_cbHandCardData,m_cbHandCardCount[wMeChairID],m_cbSortType);

	//�ɷ��˿�
	//DispatchUserCard(m_cbHandCardData,m_cbHandCardCount[wMeChairID]);
	for (WORD i=0;i<GAME_PLAYER;i++) m_GameClientView.m_HandCardControl[SwitchViewChairID(i)].SetCardData(m_cbHandCardCount[i]);
	m_GameClientView.m_HandCardControl[MYSELF_VIEW_ID].SetCardData(m_cbHandCardData,m_cbHandCardCount[GetMeChairID()]);

	//�������
	if ((IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID()))
	{
		m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btPassCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btOutPrompt.ShowWindow(SW_SHOW);
		m_GameClientView.m_btOutCard.EnableWindow((VerdictOutCard()==true)?TRUE:FALSE);
		m_GameClientView.m_btPassCard.EnableWindow((VerdictPassCard()==true)?TRUE:FALSE);
	}

	//ʱ������
	SetGameTimer(m_wCurrentUser,IDI_OUT_CARD,TIME_OUT_CARD+20);

	//��������
	if ((IsLookonMode()==false)&&(m_bStustee==false)) ActiveGameFrame();

	//��������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));

	//�������
	if (IsLookonMode()==false) m_GameClientView.m_HandCardControl[MYSELF_VIEW_ID].SetPositively(true);

	//////////////////////////////////////////////////////////////////////////

	//���½���
	m_GameClientView.UpdateGameView(NULL);

	//��������
	if ((IsLookonMode()==false)&&(m_bStustee==false)) ActiveGameFrame();

	return true;
}

//�û�����
bool CGameClientDlg::OnSubOutCard(const void * pData, WORD wDataSize)
{
	//��������
	CMD_S_OutCard * pOutCard=(CMD_S_OutCard *)pData;
	WORD wHeadSize=sizeof(CMD_S_OutCard)-sizeof(pOutCard->cbCardData);

	//Ч������
	ASSERT((wDataSize>=wHeadSize)&&(wDataSize==(wHeadSize+pOutCard->cbCardCount*sizeof(BYTE))));
	if ((wDataSize<wHeadSize)||(wDataSize!=(wHeadSize+pOutCard->cbCardCount*sizeof(BYTE)))) return false;

	//�����л�
	SwitchToCurrentCard();
	DoncludeDispatchCard();

	//ɾ����ʱ��
	KillTimer(IDI_MOST_CARD);
	KillTimer(IDI_HIDE_CARD);
	KillGameTimer(IDI_OUT_CARD);

	//��������
	WORD wMeChairID=GetMeChairID();
	WORD wViewChairID=SwitchViewChairID(pOutCard->wOutCardUser);
	bool bPlayGameSound=((IsLookonMode()==false)&&(wMeChairID==pOutCard->wOutCardUser));

	m_GameClientView.m_bShowFirstCard=false;
	//��ʷ����
	if (m_cbTurnCardCount==0)
	{
		//�û��˿�
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			//��������
			m_HistoryCard.OnEventSaveData(i);

			//��������
			if (i!=pOutCard->wOutCardUser)
			{
				WORD wViewChairID=SwitchViewChairID(i);
				m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(0);
			}
		}

		//������־
		m_GameClientView.SetUserPass(INVALID_CHAIR,false);
	}

	//���Ʊ���
	m_wCurrentUser=pOutCard->wCurrentUser;
	m_cbTurnCardCount=pOutCard->cbCardCount;
	CopyMemory(m_cbTurnCardData,pOutCard->cbCardData,pOutCard->cbCardCount*sizeof(BYTE));

	//����˿�
	m_GameLogic.MagicCardData(pOutCard->cbCardData,pOutCard->cbCardCount,m_cbMagicCardData,m_cbTurnCardType);

	//�˿�����
	m_cbTurnCardType=m_GameLogic.GetCardType(m_cbMagicCardData,pOutCard->cbCardCount);

	//ը���ж�
	if (m_cbTurnCardType>=CT_BOMB_4)
	{
		//���Ŷ���
		m_GameClientView.SetBombEffect(true);

		//��������
		if (bPlayGameSound==false)
		{
			bPlayGameSound=true;
			PlayGameSound(AfxGetInstanceHandle(),TEXT("BOMB"));
		}
	}

	//��������
	if ((IsLookonMode()==true)||(wMeChairID!=pOutCard->wOutCardUser))
	{
		//���ƽ���
		m_HistoryCard.OnEventUserOutCard(pOutCard->wOutCardUser,pOutCard->cbCardData,pOutCard->cbCardCount);
		m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(pOutCard->cbCardData,pOutCard->cbCardCount);

		//ɾ���˿�
		if (wViewChairID==MYSELF_VIEW_ID)
		{
			//��ȡ�˿�
			BYTE cbHandCardData[MAX_COUNT];
			BYTE cbHandCardCount=(BYTE)m_GameClientView.m_HandCardControl[MYSELF_VIEW_ID].GetCardData(cbHandCardData,CountArray(cbHandCardData));

			//ɾ���˿�
			m_cbHandCardCount[wMeChairID]-=pOutCard->cbCardCount;
			m_GameLogic.RemoveCard(pOutCard->cbCardData,pOutCard->cbCardCount,cbHandCardData,cbHandCardCount);
			m_GameLogic.RemoveCard(pOutCard->cbCardData,pOutCard->cbCardCount,m_cbHandCardData,cbHandCardCount);

			//�����˿�
			m_GameLogic.SortCardList(cbHandCardData,m_cbHandCardCount[wMeChairID],m_cbSortType);
			m_GameClientView.m_HandCardControl[MYSELF_VIEW_ID].SetCardData(cbHandCardData,m_cbHandCardCount[wMeChairID]);
		}
		else
		{
			//�������
			if ((m_cbHandCardCount[wMeChairID]==0)&&((pOutCard->wOutCardUser+2)%GAME_PLAYER==wMeChairID))
			{
				//ɾ���˿�
				WORD wPardnerUser=(wMeChairID+2)%GAME_PLAYER;
				BYTE cbSourceCount=m_cbHandCardCount[wPardnerUser];
				m_cbHandCardCount[wPardnerUser]-=pOutCard->cbCardCount;
				m_GameLogic.RemoveCard(pOutCard->cbCardData,pOutCard->cbCardCount,m_cbHandCardData,cbSourceCount);

				//�����˿�
				m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(m_cbHandCardData,m_cbHandCardCount[wPardnerUser]);
			}
			else
			{
				//��ͨ���
				m_cbHandCardCount[pOutCard->wOutCardUser]-=pOutCard->cbCardCount;
				m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(m_cbHandCardCount[pOutCard->wOutCardUser]);
			}
		}
	}

	//��������
	if (m_cbHandCardCount[pOutCard->wOutCardUser]==0)
	{
		m_wWinOrder[m_wWinCount++]=pOutCard->wOutCardUser;
		m_GameClientView.SetUserWinOrder(SwitchViewChairID(pOutCard->wOutCardUser),m_wWinCount);
	}

	//���ַ���
	m_TurnScore=pOutCard->TurnScore;
	int tempScore[4];
	for(WORD i=0;i<GAME_PLAYER;i++)	tempScore[SwitchViewChairID(i)]=m_PlayerScore[i];
	m_GameClientView.SetScoreInfo(m_TurnScore,tempScore);

	CopyMemory(m_510KCardRecord,pOutCard->b510KCardRecord,sizeof(m_510KCardRecord));
	m_GameClientView.Set510KRecord(m_510KCardRecord);
		
	//���½���
	m_GameClientView.UpdateGameView(NULL);

	//����ж�
	if (m_wCurrentUser==pOutCard->wOutCardUser)
	{
		//���ñ���
		m_cbTurnCardCount=0;
		m_wCurrentUser=INVALID_CHAIR;
		m_wMostUser=pOutCard->wCurrentUser;
		ZeroMemory(m_cbTurnCardData,sizeof(m_cbTurnCardData));
		ZeroMemory(m_cbMagicCardData,sizeof(m_cbMagicCardData));

		//��������
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			//�û�����
			if (pOutCard->wOutCardUser==i) continue;

			//��������
			WORD wViewChairID=SwitchViewChairID(i);
			m_GameClientView.SetUserPass(wViewChairID,true);
			m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(NULL,0);

			//������¼
			m_HistoryCard.OnEventSaveData(i);
			m_HistoryCard.OnEventUserPass(i);
		}

		//��������
		if (bPlayGameSound==false)
		{
			bPlayGameSound=true;
			PlayGameSound(AfxGetInstanceHandle(),TEXT("MOST_CARD"));
		}

		//���ö�ʱ��
		SetTimer(IDI_MOST_CARD,3000,NULL);

		return true;
	}

	//�������
	if (m_wCurrentUser!=INVALID_CHAIR)
	{
		//��������
		m_HistoryCard.OnEventSaveData(m_wCurrentUser);

		//��������
		WORD wViewChairID=SwitchViewChairID(m_wCurrentUser);

		//��ǰ����
		m_GameClientView.SetUserPass(wViewChairID,false);
		m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(NULL,0);
	}

	//�������
	if ((IsLookonMode()==false)&&(m_wCurrentUser==wMeChairID))
	{
		//������
		if (m_bStustee==false) ActiveGameFrame();

		//��ʾ��ť
		m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btPassCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btOutPrompt.ShowWindow(SW_SHOW);
		m_GameClientView.m_btOutCard.EnableWindow((VerdictOutCard()==true)?TRUE:FALSE);
		m_GameClientView.m_btPassCard.EnableWindow((VerdictPassCard()==true)?TRUE:FALSE);
	}

	//��������
	if ((bPlayGameSound==false)&&((IsLookonMode()==true)||(wMeChairID!=pOutCard->wOutCardUser)))
	{
		bPlayGameSound=true;
		PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));
	}

	//����ʱ��
	if (m_wCurrentUser!=INVALID_CHAIR)
	{
		if (m_cbTurnCardCount!=0)
		{
			BYTE cbCardCount=m_cbHandCardCount[m_wCurrentUser];
			SetGameTimer(m_wCurrentUser,IDI_OUT_CARD,TIME_OUT_CARD);
		}
		else SetGameTimer(m_wCurrentUser,IDI_OUT_CARD,TIME_OUT_CARD);
	}

	return true;
}

//�û�����
bool CGameClientDlg::OnSubPassCard(const void * pData, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_PassCard));
	if (wDataSize!=sizeof(CMD_S_PassCard)) return false;

	//��������
	CMD_S_PassCard * pPassCard=(CMD_S_PassCard *)pData;

	//���ַ���
	m_TurnScore=pPassCard->TurnScore;
	CopyMemory(m_PlayerScore,pPassCard->PlayerScore,sizeof(m_PlayerScore));
	int tempScore[4];
	for(WORD i=0;i<GAME_PLAYER;i++)	tempScore[SwitchViewChairID(i)]=m_PlayerScore[i];
	m_GameClientView.SetScoreInfo(m_TurnScore,tempScore);

	//��ǰ�˿�
	SwitchToCurrentCard();

	//ɾ��ʱ��
	KillGameTimer(IDI_OUT_CARD);

	//���ñ���
	m_wCurrentUser=pPassCard->wCurrentUser;
	
	//��������
	if ((IsLookonMode()==true)||(pPassCard->wPassCardUser!=GetMeChairID()))
	{
		WORD wPassCardUser=pPassCard->wPassCardUser;
		m_HistoryCard.OnEventUserPass(wPassCardUser);
		m_GameClientView.SetUserPass(SwitchViewChairID(wPassCardUser),true);
	}

	//���ý���
	if (m_wCurrentUser!=INVALID_CHAIR)
	{
		//��������
		m_HistoryCard.OnEventSaveData(m_wCurrentUser);

		//��ͼλ��
		WORD wViewChairID=SwitchViewChairID(m_wCurrentUser);

		//���ý���
		m_GameClientView.SetUserPass(wViewChairID,false);
		m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(0);
	}

	//һ���ж�
	if (pPassCard->cbTurnOver==TRUE)
	{
		m_cbTurnCardCount=0;
		ZeroMemory(m_cbTurnCardData,sizeof(m_cbTurnCardData));
		ZeroMemory(m_cbMagicCardData,sizeof(m_cbMagicCardData));
	}

	//�������
	if ((IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID()))
	{
		//������
		if (m_bStustee==false) ActiveGameFrame();

		//���ð�ť
		m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btPassCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btOutPrompt.ShowWindow(SW_SHOW);
		m_GameClientView.m_btOutCard.EnableWindow((VerdictOutCard()==true)?TRUE:FALSE);
		m_GameClientView.m_btPassCard.EnableWindow((VerdictPassCard()==true)?TRUE:FALSE);
	}

	//��������
	if ((IsLookonMode()==true)||(pPassCard->wPassCardUser!=GetMeChairID()))
	{
		PlayGameSound(AfxGetInstanceHandle(),TEXT("PASS_CARD"));
	}

	//����ʱ��
	if (m_cbTurnCardCount!=0)
	{
		BYTE cbCardCount=m_cbHandCardCount[m_wCurrentUser];
		SetGameTimer(m_wCurrentUser,IDI_OUT_CARD,TIME_OUT_CARD);
	}
	else SetGameTimer(m_wCurrentUser,IDI_OUT_CARD,TIME_OUT_CARD);

	return true;
}

//�˿���Ϣ
bool CGameClientDlg::OnSubCardInfo(const void * pData, WORD wDataSize)
{
	//��������
	CMD_S_CardInfo * pCardInfo=(CMD_S_CardInfo *)pData;
	WORD wHeadSize=sizeof(CMD_S_CardInfo)-sizeof(pCardInfo->cbCardData);

	//Ч������
	ASSERT((wDataSize>=wHeadSize)&&(wDataSize==(wHeadSize+pCardInfo->cbCardCount*sizeof(BYTE))));
	if ((wDataSize<wHeadSize)||(wDataSize!=(wHeadSize+pCardInfo->cbCardCount*sizeof(BYTE)))) return false;

	//���ñ���
	WORD wPardnerUser=(GetMeChairID()+2)%GAME_PLAYER;
	m_cbHandCardCount[wPardnerUser]=pCardInfo->cbCardCount;
	CopyMemory(m_cbHandCardData,pCardInfo->cbCardData,sizeof(BYTE)*pCardInfo->cbCardCount);

	//�����˿�
	WORD wPardnerViewID=SwitchViewChairID(wPardnerUser);
	m_GameClientView.m_HandCardControl[wPardnerViewID].SetCardData(m_cbHandCardData,m_cbHandCardCount[wPardnerUser]);

	//��ʾ����
	if ((IsLookonMode()==false)||(IsAllowLookon()==true))
	{
		m_GameClientView.m_HandCardControl[wPardnerViewID].SetDisplayItem(true);
	}

	//���½���
	m_GameClientView.UpdateGameView(NULL);

	return true;
}

//��Ϸ����
bool CGameClientDlg::OnSubGameEnd(const void * pData, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;

	//��������
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pData;

	//���ַ���
	m_TurnScore=pGameEnd->TurnScore;
	CopyMemory(m_PlayerScore,pGameEnd->PlayerScore,sizeof(m_PlayerScore));
	int tempScore[4];
	for(WORD i=0;i<GAME_PLAYER;i++)	tempScore[SwitchViewChairID(i)]=m_PlayerScore[i];
	m_GameClientView.SetScoreInfo(m_TurnScore,tempScore);

	//����״̬
	SetGameStatus(GAME_SCENE_FREE);

	//ɾ��ʱ��
	KillTimer(IDI_MOST_CARD);
	KillTimer(IDI_HIDE_CARD);
	KillTimer(IDI_DISPATCH_CARD);

	//��Ϸʱ��
	KillGameTimer(IDI_OUT_CARD);

	//���ư�ť
	m_GameClientView.m_btOutCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btPassCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btOutPrompt.ShowWindow(SW_HIDE);

	//���ÿؼ�
	m_GameClientView.m_btOutCard.EnableWindow(FALSE);
	m_GameClientView.m_btPassCard.EnableWindow(FALSE);
	m_GameClientView.m_btLastTurn.EnableWindow(FALSE);
	m_GameClientView.m_btSort.EnableWindow(FALSE);
	m_GameClientView.m_btSortCardColor.EnableWindow(FALSE);
	m_GameClientView.m_btSortCardCount.EnableWindow(FALSE);
	m_GameClientView.m_btSortCardOrder.EnableWindow(FALSE);

	//�����˿�
	m_GameClientView.m_HandCardControl[MYSELF_VIEW_ID].SetPositively(false);

	//��������
	tagScoreInfoC ScoreInfo;
	ZeroMemory(&ScoreInfo,sizeof(ScoreInfo));

	//�ɼ��û�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��ȡ�û�
		const tagUserData * pUserData=GetUserData(i);
		if (pUserData==NULL) continue;

		//���óɼ�
		ScoreInfo.lGameScore[i]=pGameEnd->lGameScore[i];

		//��������
		lstrcpyn(ScoreInfo.szUserName[i],pUserData->szName,CountArray(ScoreInfo.szUserName[i]));
	}

	//���ý���
	m_GameClientView.SetUserPass(INVALID_CHAIR,false);
	m_GameClientView.SetUserContinue(INVALID_CHAIR,false);
	m_GameClientView.m_ScoreControl.SetScoreInfo(ScoreInfo);

	//���ý���
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��ͼλ��
		WORD wViewChairID=SwitchViewChairID(i);

		//�����˿�
		m_GameClientView.m_HandCardControl[wViewChairID].SetDisplayItem(true);
		m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(pGameEnd->cbCardData[i],pGameEnd->cbCardCount[i]);

		//��������
		m_GameClientView.SetUserPersist(wViewChairID,pGameEnd->wPersistInfo[i]);
	}

	//��������
	LONGLONG lMeScore=pGameEnd->lGameScore[GetMeChairID()];
	if (lMeScore>0L) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WIN"));
	else if (lMeScore<0L) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_LOSE"));
	else PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_END"));

	//��ʼ����
	if (IsLookonMode()==false)
	{
		if(m_bStustee==true) OnMessageStustee(0,0);
		//��ʾ��ť
		m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
		m_GameClientView.m_btStart.SetFocus();

		//����ʱ��
		SetGameTimer(GetMeChairID(),IDI_START_GAME,TIME_START_GAME);
	}

	//���½���
	m_GameClientView.UpdateGameView(NULL);

	return true;
}


//������Ϸ
bool CGameClientDlg::OnSubContinueGame(const void * pData, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_ContinueGame));
	if (wDataSize!=sizeof(CMD_S_ContinueGame)) return false;

	//��������
	CMD_S_ContinueGame * pContinueGame=(CMD_S_ContinueGame *)pData;

	//���ý���
	WORD wChairID=pContinueGame->wChairID;
	m_GameClientView.SetUserContinue(SwitchViewChairID(wChairID),true);

	return true;
}

//�����ж�
bool CGameClientDlg::VerdictOutCard()
{
	//״̬�ж�
	if (GetGameStatus()!=GAME_SCENE_PLAY) return false;
	if (m_wCurrentUser!=GetMeChairID()) return false;

	//��ȡ�˿�
	BYTE cbCardData[MAX_COUNT];
	BYTE cbShootCount=(BYTE)m_GameClientView.m_HandCardControl[MYSELF_VIEW_ID].GetShootCard(cbCardData,CountArray(cbCardData));

	//����˿�
	BYTE cbMagicCardData[MAX_COUNT];
	m_GameLogic.MagicCardData(cbCardData,cbShootCount,cbMagicCardData,m_cbTurnCardType);
	//�����ж�
	if (cbShootCount>0L)
	{
		//�����ж�
		m_GameLogic.SortCardList(cbMagicCardData,cbShootCount,ST_ORDER);
		if (m_GameLogic.GetCardType(cbMagicCardData,cbShootCount)==CT_ERROR) 
		{
			return false;
		}

		//�����ж�
		if (m_cbTurnCardCount==0) return true;

		return m_GameLogic.CompareCard(m_cbMagicCardData,cbMagicCardData,m_cbTurnCardCount,cbShootCount);
	}

	return false;
}

//�����ж�
bool CGameClientDlg::VerdictPassCard()
{
	//�����ж�
	if (m_cbTurnCardCount==0) return false;

	return true;
}

//�Զ���ʼ
bool CGameClientDlg::PerformAutoStart()
{
	//��ʼ����
	WORD wMeChairID=GetMeChairID();
	SetGameTimer(wMeChairID,IDI_START_GAME,TIME_START_GAME);

	return true;
	//��������
	WORD wTableUserCount=0,wReadyUserCount=0;
	//IClientUserItem * pIMySelfUserItem=GetMeUserItem();
	const tagUserData * pMeUserData=GetUserData(GetMeChairID());

	//״̬�ж�
	if ((IsLookonMode()==true)/*||(m_pIClientKernel->GetTimeID()==IDI_START_GAME)*/) return false;
	if ((pMeUserData==NULL)||(pMeUserData->cbUserStatus==US_READY)) return false;

	//ͳ���û�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��ȡ�û�
		const tagUserData * pUserData=GetUserData(i);

		//��Ŀͳ��
		if (pUserData!=NULL) 
		{
			wTableUserCount++;
			if (pUserData->cbUserStatus==US_READY) wReadyUserCount++;
		}
	}

	//��ʼ�ж�
	if ((wTableUserCount==GAME_PLAYER)&&((wReadyUserCount+1)==wTableUserCount))
	{
		//��ʼ����
		WORD wMeChairID=GetMeChairID();
		SetGameTimer(wMeChairID,IDI_START_GAME,TIME_START_GAME);

		return true;
	}

	return false;
}

//�Զ�����
bool CGameClientDlg::AutomatismOutCard()
{
	//״̬�ж�
	if (GetGameStatus()!=GAME_SCENE_PLAY) return false;
	if ((IsLookonMode()==true)||(m_wCurrentUser!=GetMeChairID())) return false;

	//��ǰ����
	CSkinButton & btOutCard=m_GameClientView.m_btOutCard;
	if ((btOutCard.IsWindowEnabled())&&(btOutCard.IsWindowVisible()))
	{
		OnMessageOutCard(0,0);
		return true;
	}

	//���ƴ���
	if ((m_cbTurnCardCount==0)/*||(m_bStustee==true)*/)
	{
		//�������
		tagOutCardResult OutCardResult;
		m_GameLogic.SearchOutCard(m_cbHandCardData,m_cbHandCardCount[GetMeChairID()],m_cbMagicCardData,m_cbTurnCardCount,m_cbPromptType,OutCardResult);

		//���ý���
		if (OutCardResult.cbCardCount>0)
		{
			//���ý���
			m_GameClientView.m_btOutCard.EnableWindow(TRUE);
			m_GameClientView.m_HandCardControl[MYSELF_VIEW_ID].SetShootCard(OutCardResult.cbResultCard,OutCardResult.cbCardCount);

			//���ƶ���
			OnMessageOutCard(0,0);

			return true;
		}
	}

	//��������
	if (m_cbTurnCardCount>0) OnMessagePassCard(0,0);

	return true;
}

//��ǰ�˿�
bool CGameClientDlg::SwitchToCurrentCard()
{
	//�����ж�
	if (m_bLastTurn==false) return false;

	//��������
	m_bLastTurn=false;
	KillTimer(IDI_LAST_TURN);
	m_GameClientView.SetLastTurnCard(false);

	//�����˿�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��������
		WORD wViewChairID=SwitchViewChairID(i);
		BYTE cbEventFlag=m_HistoryCard.GetCurrentEvent(i);

		//��ȡ�˿�
		BYTE cbCardData[MAX_COUNT];
		BYTE cbCardCount=m_HistoryCard.GetCurrentCard(i,cbCardData,CountArray(cbCardData));

		//���ý���
		m_GameClientView.SetUserPass(wViewChairID,cbEventFlag==AF_PASS);
		m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(cbCardData,cbCardCount);
	}

	//���½���
	m_GameClientView.UpdateGameView(NULL);

	return true;
}

//ֹͣ����
bool CGameClientDlg::DoncludeDispatchCard()
{
	//ֹͣ����
	if (m_cbRemnantCardCount>0)
	{
		//ɾ��ʱ��
		KillTimer(IDI_DISPATCH_CARD);

		//�����˿�
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			//��ȡλ��
			WORD wViewChairID=SwitchViewChairID(i);

			//�����˿�
			if (wViewChairID==MYSELF_VIEW_ID)
			{
				m_GameLogic.SortCardList(m_cbHandCardData,m_cbHandCardCount[i],m_cbSortType);
				m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(m_cbHandCardData,m_cbHandCardCount[i]);
			}
			else m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(m_cbHandCardCount[i]);
		}

		//�������
		if (IsLookonMode()==false) m_GameClientView.m_HandCardControl[MYSELF_VIEW_ID].SetPositively(true);

		//���ñ���
		m_cbRemnantCardCount=0;
		m_cbDispatchCardCount=0;
		ZeroMemory(m_cbDispatchCardData,sizeof(m_cbDispatchCardData));

		//���½���
		m_GameClientView.UpdateGameView(NULL);
	}

	return true;
}

//�ɷ��˿�
bool CGameClientDlg::DispatchUserCard(BYTE cbCardData[], BYTE cbCardCount)
{
	//���ñ���
	m_cbDispatchCardCount=0;
	m_cbRemnantCardCount=cbCardCount;
	CopyMemory(m_cbDispatchCardData,cbCardData,sizeof(BYTE)*cbCardCount);

	//���ö�ʱ��
	SetTimer(IDI_DISPATCH_CARD,100,NULL);

	return true;
}

//��ʱ����Ϣ
VOID CGameClientDlg::OnTimer(UINT nIDEvent)
{
	switch (nIDEvent)
	{
	case IDI_MOST_CARD:			//����˿�
		{
			//�û�Ч��
			ASSERT(m_wMostUser!=INVALID_CHAIR);
			if (m_wMostUser==INVALID_CHAIR) return;

			//���ñ���
			m_wCurrentUser=m_wMostUser;
			m_wMostUser=INVALID_CHAIR;

			//ɾ��ʱ��
			KillTimer(IDI_MOST_CARD);

			//���ý���
			m_GameClientView.SetUserPass(INVALID_CHAIR,false);
			for (WORD i=0;i<GAME_PLAYER;i++) m_GameClientView.m_UserCardControl[i].SetCardData(0);

			//��������
			for (WORD i=0;i<GAME_PLAYER;i++) m_HistoryCard.OnEventSaveData(i);

			//�������
			if ((IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID()))
			{
				//������
				if (m_bStustee==false) ActiveGameFrame();

				//���ð�ť
				m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
				m_GameClientView.m_btPassCard.ShowWindow(SW_SHOW);
				m_GameClientView.m_btOutPrompt.ShowWindow(SW_SHOW);
				m_GameClientView.m_btOutCard.EnableWindow((VerdictOutCard()==true)?TRUE:FALSE);
				m_GameClientView.m_btPassCard.EnableWindow((VerdictPassCard()==true)?TRUE:FALSE);
			}

			//����ʱ��
			SetGameTimer(m_wCurrentUser,IDI_OUT_CARD,TIME_OUT_CARD+20);

			return;
		}
	case IDI_HIDE_CARD:			//�����˿�
		{
			//ɾ��ʱ��
			KillTimer(IDI_HIDE_CARD);

			//���ý���
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				m_GameClientView.m_UserCardControl[i].SetCardData(0);
			}

			//���½���
			m_GameClientView.UpdateGameView(NULL);

			return;
		}
	case IDI_LAST_TURN:			//�����˿�
		{
			//��ԭ����
			SwitchToCurrentCard();

			return;
		}
	case IDI_DISPATCH_CARD:		//�ɷ��˿�
		{
			//�ɷ�����
			if (m_cbRemnantCardCount>0)
			{
				//�ɷ��˿�
				m_cbRemnantCardCount--;
				m_cbDispatchCardCount++;
				m_GameLogic.SortCardList(m_cbDispatchCardData,m_cbDispatchCardCount,m_cbSortType);

				//�����˿�
				m_GameClientView.m_HandCardControl[0].SetCardData(m_cbDispatchCardCount);
				m_GameClientView.m_HandCardControl[1].SetCardData(m_cbDispatchCardCount);
				m_GameClientView.m_HandCardControl[3].SetCardData(m_cbDispatchCardCount);
				m_GameClientView.m_HandCardControl[MYSELF_VIEW_ID].SetCardData(m_cbDispatchCardData,m_cbDispatchCardCount);

				//���½���
				m_GameClientView.UpdateGameView(NULL);

				//��������
				PlayGameSound(AfxGetInstanceHandle(),TEXT("SEND_CARD"));
			}
			
			//��ֹ�ж�
			if (m_cbRemnantCardCount==0)
			{
				//ɾ��ʱ��
				KillTimer(IDI_DISPATCH_CARD);

				//��������
				if (m_wCurrentUser!=INVALID_CHAIR)
				{
					//�������
					if ((IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID()))
					{
						m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
						m_GameClientView.m_btPassCard.ShowWindow(SW_SHOW);
						m_GameClientView.m_btOutPrompt.ShowWindow(SW_SHOW);
						m_GameClientView.m_btOutCard.EnableWindow((VerdictOutCard()==true)?TRUE:FALSE);
						m_GameClientView.m_btPassCard.EnableWindow((VerdictPassCard()==true)?TRUE:FALSE);
					}

					//ʱ������
					SetGameTimer(m_wCurrentUser,IDI_OUT_CARD,TIME_OUT_CARD+20);
				}
				else
				{
					//����״̬
					//m_GameClientView.SetThrowStatus(true);
				}

				//��������
				if ((IsLookonMode()==false)&&(m_bStustee==false)) ActiveGameFrame();

				//��������
				PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));

				//�������
				if (IsLookonMode()==false) m_GameClientView.m_HandCardControl[MYSELF_VIEW_ID].SetPositively(true);
			}

			return;
		}
	}

	__super::OnTimer(nIDEvent);
}

//��ʼ��Ϣ
LRESULT CGameClientDlg::OnMessageStart(WPARAM wParam, LPARAM lParam)
{
	//ɾ��ʱ��
	KillGameTimer(IDI_START_GAME);

	//�û��˿�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.m_HandCardControl[i].SetCardData(NULL,0);
		m_GameClientView.m_UserCardControl[i].SetCardData(NULL,0);
		m_GameClientView.m_HandCardControl[i].SetDisplayItem(false);
	}

	//�û��˿�
	m_GameClientView.m_HandCardControl[MYSELF_VIEW_ID].SetPositively(false);

	//���ý���
	m_GameClientView.SetUserWinOrder(INVALID_CHAIR,0);
	m_GameClientView.SetUserPass(INVALID_CHAIR,false);
	m_GameClientView.SetUserContinue(INVALID_CHAIR,false);
	//��ʾ��������
	m_cbOutPromptCardCount = 0;
	m_cbPromptType = 0;
	//��������
	m_GameClientView.m_btStart.ShowWindow(SW_HIDE);
	m_GameClientView.m_ScoreControl.RestorationData();

	//���½���
	m_GameClientView.UpdateGameView(NULL);

	//�û���Ϣ
	WORD wMeChairID=GetMeChairID();
	const tagUserData * pUserData=GetUserData(GetMeChairID());

	//������Ϣ
	if (pUserData->cbUserStatus!=US_PLAY)
	{
		SendUserReady(NULL,0);
	}
	else
	{
		SendData(SUB_C_CONTINUE_GAME);
	}

	return 0;
}

//�Ϲܿ���
LRESULT CGameClientDlg::OnMessageStustee(WPARAM wParam, LPARAM lParam)
{
	//��Դ����
	HINSTANCE hResInstance=AfxGetInstanceHandle();

	//����״̬
	if (m_bStustee==true)
	{
		m_bStustee=false;
		m_GameClientView.m_btStustee.SetButtonImage(IDB_BT_START_TRUSTEE,hResInstance,false);
	}
	else
	{
		m_bStustee=true;
		m_GameClientView.m_btStustee.SetButtonImage(IDB_BT_STOP_TRUSTEE,hResInstance,false);
	}

	CMD_C_Automatism AutomatismData;
	AutomatismData.bAutomatism = m_bStustee;

	SendData(SUB_C_AUTOMATISM, &AutomatismData, sizeof(AutomatismData));

	return 0;
}

//�����˿�
LRESULT CGameClientDlg::OnMessageSortCard(WPARAM wParam, LPARAM lParam)
{
	//���ñ���
	m_cbSortType=(BYTE)wParam;

	//������Ϣ
	if(m_cbSortType==255)
	{
		//��ȡ�˿�
		BYTE cbShootCard[MAX_COUNT];
		BYTE cbShootCount=(BYTE)m_GameClientView.m_HandCardControl[MYSELF_VIEW_ID].GetShootCard(cbShootCard,CountArray(cbShootCard));

		//�޵��𷵻�
		if(cbShootCount==0) return 0;

		m_b510KCount=0;
		m_b510KIndex=0;

		//�����˿�
		BYTE cbTempCard[MAX_COUNT];
		BYTE cbTempCount=MAX_COUNT;

		//�Ƴ�����
		m_GameClientView.m_HandCardControl[MYSELF_VIEW_ID].RemoveShootItem();
		cbTempCount=(BYTE)m_GameClientView.m_HandCardControl[MYSELF_VIEW_ID].GetCardData(cbTempCard,cbTempCount);
		
		//��������
		CopyMemory(m_cbHandCardData,cbShootCard,sizeof(BYTE)*cbShootCount);
		CopyMemory(&m_cbHandCardData[cbShootCount],cbTempCard,sizeof(BYTE)*m_cbHandCardCount[GetMeChairID()]-cbShootCount);
				
		//��������
		m_GameClientView.m_HandCardControl[MYSELF_VIEW_ID].SetCardData(m_cbHandCardData,m_cbHandCardCount[GetMeChairID()]);

		m_GameClientView.m_btOutCard.EnableWindow(false);
		//���½���
		m_GameClientView.UpdateGameView(NULL);
		return 0;
	}

	//�����˿�
	if (m_cbSortType<ST_CUSTOM)
	{
		//�����˿�
		WORD wMeChairID=GetMeChairID();
		m_GameLogic.SortCardList(m_cbHandCardData,m_cbHandCardCount[wMeChairID],m_cbSortType);

		//��ȡ�˿�
		BYTE cbShootCard[MAX_COUNT];
		BYTE cbShootCount=(BYTE)m_GameClientView.m_HandCardControl[MYSELF_VIEW_ID].GetShootCard(cbShootCard,CountArray(cbShootCard));

		//�����˿�
		if (m_cbRemnantCardCount>0)
		{
			m_GameLogic.SortCardList(m_cbDispatchCardData,m_cbDispatchCardCount,m_cbSortType);
			m_GameClientView.m_HandCardControl[MYSELF_VIEW_ID].SetCardData(m_cbDispatchCardData,m_cbDispatchCardCount);
		}
		else m_GameClientView.m_HandCardControl[MYSELF_VIEW_ID].SetCardData(m_cbHandCardData,m_cbHandCardCount[wMeChairID]); 

		//�����˿�
		m_GameClientView.m_HandCardControl[MYSELF_VIEW_ID].SetShootCard(cbShootCard,cbShootCount);

		//���½���
		m_GameClientView.UpdateGameView(NULL);
	}

	//�����˿�
	if (m_cbSortType==ST_510K)
	{
		//��������
		BYTE tempCardData[MAX_COUNT];ZeroMemory(tempCardData,sizeof(tempCardData));
		CopyMemory(tempCardData,m_cbHandCardData,sizeof(BYTE)*m_cbHandCardCount[GetMeChairID()]);

		//�Ȱ���С����
		m_GameLogic.SortCardList(tempCardData,m_cbHandCardCount[GetMeChairID()],ST_ORDER);

		//���õ���
		BYTE bShootCount=0;
		BYTE bShootCardData[MAX_COUNT];ZeroMemory(bShootCardData,sizeof(bShootCardData));

		//�Ѿ�����
		if(m_b510KCount>0)
		{
			bShootCount=3;
			CopyMemory(bShootCardData,m_b510KCard[m_b510KIndex],sizeof(BYTE)*3);
			m_b510KIndex++;
			if(m_b510KIndex>=m_b510KCount) m_b510KIndex=0;

			//���õ���
			m_GameClientView.m_HandCardControl[MYSELF_VIEW_ID].SetShootCard(bShootCardData,bShootCount);

			//��ǰ������Գ���
			if(m_wCurrentUser==GetMeChairID()&&m_GameClientView.m_btOutCard.IsWindowVisible()==TRUE)
			{
				if(VerdictOutCard()==true)
				{
					m_GameClientView.m_btOutCard.EnableWindow(true);
				}
			}

			//���½���
			m_GameClientView.UpdateGameView(NULL);
			return 0;
		}
		if(m_b510KCount==0)
		{
			////����510K
			////�����ֲ�
			//tagDistributing Distribute;
			//m_GameLogic.AnalysebDistributing(tempCardData,m_cbHandCardCount[GetMeChairID()],Distribute);

			////����ͬ��
			//for(BYTE i=0;i<4;i++)
			//{
			//	bool bHave5=Distribute.cbDistributing[4][i]>0;
			//	bool bHave10=Distribute.cbDistributing[9][i]>0;
			//	bool bHaveK=Distribute.cbDistributing[12][i]>0;

			//	if(bHave5&&bHave10&&bHaveK)
			//	{
			//		BYTE b510KCard[3];
			//		b510KCard[0]=m_GameLogic.MakeCardData(12,i);
			//		b510KCard[1]=m_GameLogic.MakeCardData(9,i);
			//		b510KCard[2]=m_GameLogic.MakeCardData(4,i);
			//		CopyMemory(m_b510KCard[m_b510KCount++],b510KCard,sizeof(BYTE)*3);
			//	}
			//}	

			//��¼5
			BYTE Card5[8];
			BYTE Card5Count=0;
			ZeroMemory(Card5,sizeof(Card5));
			//��¼10
			BYTE Card10[8];
			BYTE Card10Count=0;
			ZeroMemory(Card10,sizeof(Card10));
			//��¼K
			BYTE CardK[8];
			BYTE CardKCount=0;
			ZeroMemory(CardK,sizeof(CardK));
			//������510K
			WORD wMyID=GetMeChairID();
			for(int i=m_cbHandCardCount[wMyID]-1;i>=0;i--)
			{
				if(m_GameLogic.GetCardValue(m_cbHandCardData[i])==5) Card5[Card5Count++]=m_cbHandCardData[i];
				if(m_GameLogic.GetCardValue(m_cbHandCardData[i])==10) Card10[Card10Count++]=m_cbHandCardData[i];
				if(m_GameLogic.GetCardValue(m_cbHandCardData[i])==13) CardK[CardKCount++]=m_cbHandCardData[i];
			}

			BYTE temp510K[3];	ZeroMemory(temp510K,sizeof(temp510K));
			for(BYTE a=0;a<Card5Count;a++)
			for(BYTE b=0;b<Card10Count;b++)
			for(BYTE c=0;c<CardKCount;c++)
			{
				temp510K[0]=CardK[c];	
				temp510K[1]=Card10[b];  
				temp510K[2]=Card5[a];	

				if(m_GameLogic.GetCardType(temp510K,3)==CT_510K_DC||m_GameLogic.GetCardType(temp510K,3)==CT_510K_SC)
				{
					CopyMemory(m_b510KCard[m_b510KCount++],temp510K,sizeof(BYTE)*3);
				}
			}

			if(m_b510KCount>0)
			{
				bShootCount=3;
				CopyMemory(bShootCardData,m_b510KCard[0],sizeof(BYTE)*3);
				//���õ���
				m_GameClientView.m_HandCardControl[MYSELF_VIEW_ID].SetShootCard(bShootCardData,bShootCount);
				m_b510KIndex++;
				if(m_b510KIndex>=m_b510KCount) m_b510KIndex=0;
			}
			//��ǰ������Գ���
			if(m_wCurrentUser==GetMeChairID()&&m_GameClientView.m_btOutCard.IsWindowVisible()==TRUE)
			{
				if(VerdictOutCard()==true)
				{
					m_GameClientView.m_btOutCard.EnableWindow(true);
				}
			}

			//���½���
			m_GameClientView.UpdateGameView(NULL);
		}

		return 0;
	}

	//�����˿�
	if (m_cbSortType==ST_TONGHUA)
	{
		BYTE tempCardData[MAX_COUNT];ZeroMemory(tempCardData,sizeof(tempCardData));
		CopyMemory(tempCardData,m_cbHandCardData,sizeof(BYTE)*m_cbHandCardCount[GetMeChairID()]);

		m_GameLogic.SortCardList(tempCardData,m_cbHandCardCount[GetMeChairID()],ST_ORDER);

		BYTE bShootCount=0;
		BYTE bShootCardData[MAX_COUNT];ZeroMemory(bShootCardData,sizeof(bShootCardData));

		if(m_bTongHuaCount>0)
		{
			bShootCount=5;
			CopyMemory(bShootCardData,m_bTongHuaCard[m_bTongHuaIndex],sizeof(BYTE)*5);
			m_bTongHuaIndex++;
			if(m_bTongHuaIndex>=m_bTongHuaCount) m_bTongHuaIndex=0;

			//���õ���
			m_GameClientView.m_HandCardControl[MYSELF_VIEW_ID].SetShootCard(bShootCardData,bShootCount);

			//��ǰ������Գ���
			if(m_wCurrentUser==GetMeChairID()&&m_GameClientView.m_btOutCard.IsWindowVisible()==TRUE)
			{
				if(VerdictOutCard()==true)
				{
					m_GameClientView.m_btOutCard.EnableWindow(true);
				}
			}

			//���½���
			m_GameClientView.UpdateGameView(NULL);
			return 0;
		}

		//�����ֲ�
		tagDistributing Distribute;
		m_GameLogic.AnalysebDistributing(tempCardData,m_cbHandCardCount[GetMeChairID()],Distribute);
		//����A������
		CopyMemory(Distribute.cbDistributing[13],Distribute.cbDistributing[0],sizeof(Distribute.cbDistributing[0]));


		for(BYTE i=0;i<10;i++)
		{
			for(BYTE j=0;j<4;j++)
			{
				//�˴�û����
				if( Distribute.cbDistributing[i][j]==0) continue;

				if(Distribute.cbDistributing[i+1][j]>=1)
				if(Distribute.cbDistributing[i+2][j]>=1)
				if(Distribute.cbDistributing[i+3][j]>=1)
				if(Distribute.cbDistributing[i+4][j]>=1)
				{
					BYTE bTongHuaCard[5];
					bTongHuaCard[0]=m_GameLogic.MakeCardData(i,j);
					bTongHuaCard[1]=m_GameLogic.MakeCardData(i+1,j);
					bTongHuaCard[2]=m_GameLogic.MakeCardData(i+2,j);
					bTongHuaCard[3]=m_GameLogic.MakeCardData(i+3,j);
					bTongHuaCard[4]=m_GameLogic.MakeCardData(i+4,j);
					if((i+4)==13) bTongHuaCard[4]=m_GameLogic.MakeCardData(0,j);				
					CopyMemory(m_bTongHuaCard[m_bTongHuaCount++],bTongHuaCard,sizeof(BYTE)*5);
				}
			}
		}

		if(m_bTongHuaCount>0)
		{
			bShootCount=5;
			CopyMemory(bShootCardData,m_bTongHuaCard[m_bTongHuaIndex],sizeof(BYTE)*5);
			m_bTongHuaIndex++;
		}
		//���õ���
		m_GameClientView.m_HandCardControl[MYSELF_VIEW_ID].SetShootCard(bShootCardData,bShootCount);

		//��ǰ������Գ���
		if(m_wCurrentUser==GetMeChairID()&&m_GameClientView.m_btOutCard.IsWindowVisible()==TRUE)
		{
			if(VerdictOutCard()==true)
			{
				m_GameClientView.m_btOutCard.EnableWindow(true);
			}
		}

		//���½���
		m_GameClientView.UpdateGameView(NULL);
	}
	return 0;
}

//�鿴����
LRESULT CGameClientDlg::OnMessageLastTurn(WPARAM wParam, LPARAM lParam)
{
	if (m_bLastTurn==false)
	{
		//�����˿�
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			//��������
			WORD wViewChairID=SwitchViewChairID(i);
			BYTE cbEventFlag=m_HistoryCard.GetHistoryEvent(i);

			//��ȡ�˿�
			BYTE cbCardData[MAX_COUNT];
			BYTE cbCardCount=m_HistoryCard.GetHistoryCard(i,cbCardData,CountArray(cbCardData));

			//���ý���
			m_GameClientView.SetUserPass(wViewChairID,cbEventFlag==AF_PASS);
			m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(cbCardData,cbCardCount);
		}

		//��������
		m_bLastTurn=true;
		SetTimer(IDI_LAST_TURN,3000,NULL);
		m_GameClientView.SetLastTurnCard(true);

		//���½���
		m_GameClientView.UpdateGameView(NULL);
	}
	else SwitchToCurrentCard();

	return 0;
}

//������Ϣ
LRESULT CGameClientDlg::OnMessageOutCard(WPARAM wParam, LPARAM lParam)
{
	//״̬�ж�
	if (m_GameClientView.m_btOutCard.IsWindowEnabled()==FALSE) return 0;
	if (m_GameClientView.m_btOutCard.IsWindowVisible()==FALSE) return 0;

	//ɾ��ʱ��
	KillTimer(IDI_HIDE_CARD);
	KillGameTimer(IDI_OUT_CARD);

	//���ý���
	m_GameClientView.m_btOutCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btOutCard.EnableWindow(FALSE);
	m_GameClientView.m_btPassCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btPassCard.EnableWindow(FALSE);
	m_GameClientView.m_btOutPrompt.ShowWindow(SW_HIDE);

	m_bTongHuaCount=0;
	m_bTongHuaIndex=0;
	ZeroMemory(m_bTongHuaCard,sizeof(m_bTongHuaCard));
	m_b510KCount=0;
	m_b510KIndex=0;
	ZeroMemory(m_b510KCard,sizeof(m_b510KCard));
	m_cbOutPromptCardCount = 0;	//������ʾ����
	m_cbPromptType = 0;
	//��ȡ�˿�
	BYTE cbCardData[MAX_COUNT];
	BYTE cbCardCount=(BYTE)m_GameClientView.m_HandCardControl[MYSELF_VIEW_ID].GetShootCard(cbCardData,CountArray(cbCardData));

	//�����˿�
	m_GameLogic.SortCardList(cbCardData,cbCardCount,ST_ORDER);

	//��������
	WORD wMeChairID=GetMeChairID();
	BYTE cbHandCardCount=m_cbHandCardCount[wMeChairID];

	//ɾ���˿�
	m_cbHandCardCount[wMeChairID]-=cbCardCount;
	m_GameLogic.RemoveCard(cbCardData,cbCardCount,m_cbHandCardData,cbHandCardCount);

	//�����˿�
	if (m_cbSortType<ST_CUSTOM)
	{
		m_GameLogic.SortCardList(m_cbHandCardData,m_cbHandCardCount[wMeChairID],m_cbSortType);
		m_GameClientView.m_HandCardControl[MYSELF_VIEW_ID].SetCardData(m_cbHandCardData,m_cbHandCardCount[wMeChairID]);
	}
	else
	{
		m_GameClientView.m_HandCardControl[MYSELF_VIEW_ID].RemoveShootItem();
	}

	//���ƽ���
	m_GameClientView.m_UserCardControl[MYSELF_VIEW_ID].SetCardData(cbCardData,cbCardCount);

	//��ʷ�˿�
	m_HistoryCard.OnEventUserOutCard(GetMeChairID(),cbCardData,cbCardCount);

	//����˿�
	BYTE cbMagicCardData[MAX_COUNT];
	m_GameLogic.MagicCardData(cbCardData,cbCardCount,cbMagicCardData,m_cbTurnCardType);

	//��������
	BYTE cbCardType=m_GameLogic.GetCardType(cbMagicCardData,cbCardCount);
	PlayGameSound(AfxGetInstanceHandle(),(cbCardType>=CT_BOMB_4)?TEXT("BOMB"):TEXT("OUT_CARD"));

	//���½���
	m_GameClientView.UpdateGameView(NULL);

	//��������
	CMD_C_OutCard OutCard;
	OutCard.cbCardCount=cbCardCount;
	CopyMemory(OutCard.cbCardData,cbCardData,cbCardCount);
	SendData(SUB_C_OUT_CARD,&OutCard,sizeof(OutCard)-sizeof(OutCard.cbCardData)+OutCard.cbCardCount*sizeof(BYTE));

	return 0;
}

//PASS��Ϣ
LRESULT CGameClientDlg::OnMessagePassCard(WPARAM wParam, LPARAM lParam)
{
	//״̬�ж�
	if (m_GameClientView.m_btPassCard.IsWindowEnabled()==FALSE) return 0;
	if (m_GameClientView.m_btPassCard.IsWindowVisible()==FALSE) return 0;
	m_cbOutPromptCardCount = 0;
	m_cbPromptType = 0;
	//ɾ��ʱ��
	KillGameTimer(IDI_OUT_CARD);

	//���ý���
	m_GameClientView.m_btOutCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btOutCard.EnableWindow(FALSE);
	m_GameClientView.m_btPassCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btPassCard.EnableWindow(FALSE);
	m_GameClientView.m_btOutPrompt.ShowWindow(SW_HIDE);

	//��������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("PASS_CARD"));

	//��������
	WORD wMeChairID=GetMeChairID();
	m_HistoryCard.OnEventUserPass(wMeChairID);
	m_GameClientView.SetUserPass(MYSELF_VIEW_ID,true);

	//���½���
	m_GameClientView.UpdateGameView(NULL);

	//��������
	SendData(SUB_C_PASS_CARD);

	return 0;
}

//��ʾ��Ϣ
LRESULT CGameClientDlg::OnMessageOutPrompt(WPARAM wParam, LPARAM lParam)
{
	//�������
	tagOutCardResult OutCardResult;
	bool bValid ;
	if (m_cbOutPromptCardCount>0)
	 bValid = m_GameLogic.SearchOutCard(m_cbHandCardData,m_cbHandCardCount[GetMeChairID()],m_cbOutPromptCardData,m_cbTurnCardCount,m_cbPromptType,OutCardResult);
	else
	 bValid = m_GameLogic.SearchOutCard(m_cbHandCardData,m_cbHandCardCount[GetMeChairID()],m_cbMagicCardData,m_cbTurnCardCount,m_cbPromptType,OutCardResult);
	//���ý���
	if (bValid)
	{
		m_cbOutPromptCardCount = OutCardResult.cbCardCount;
		memcpy(m_cbOutPromptCardData,OutCardResult.cbResultCard,OutCardResult.cbCardCount);
		//���ý���
		m_GameClientView.m_HandCardControl[MYSELF_VIEW_ID].SetShootCard(OutCardResult.cbResultCard,OutCardResult.cbCardCount);

		//���ÿؼ�
		bool bOutCard=VerdictOutCard();
		m_GameClientView.m_btOutCard.EnableWindow((bOutCard==true)?TRUE:FALSE);

		//���½���
		m_GameClientView.UpdateGameView(NULL);
	}
	else 
	{
		//��������
		m_cbOutPromptCardCount = 0;
		m_cbPromptType = 0;
		OnMessagePassCard(0,0);
	}

	return 0;
}

//����˿�
LRESULT CGameClientDlg::OnMessageLeftHitCard(WPARAM wParam, LPARAM lParam)
{
	//����״̬
	if (m_GameClientView.m_btOutCard.IsWindowVisible())
	{
		//�����ж�
		bool bOutCard=VerdictOutCard();
		m_GameClientView.m_btOutCard.EnableWindow((bOutCard==true)?TRUE:FALSE);

		return 0;
	}

	return 0;
}

//��������
LRESULT CGameClientDlg::OnMessageEnableHistory(WPARAM wParam, LPARAM lParam)
{
	//��������
	m_GameClientView.m_btLastTurn.EnableWindow(TRUE);

	return 0;
}

//////////////////////////////////////////////////////////////////////////
