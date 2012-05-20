#include "Stdafx.h"
#include "GameOption.h"
#include "GameClient.h"
#include "GameClientDlg.h"
#include ".\gameclientdlg.h"

//////////////////////////////////////////////////////////////////////////

//��ʱ����ʶ
#define IDI_NULLITY					200									//��Ч��ʱ��
#define IDI_START_GAME				201									//��ʼ��ʱ��
#define IDI_CALL_BANKER				202									//��ʼ��ʱ��

//��ʱ����ʶ
#define IDI_TIME_USER_ADD_SCORE		1									//��ע��ʱ��
#define IDI_TIME_OPEN_CARD			2									//̯�ƶ�ʱ��

//ʱ���ʶ
#ifdef _DEBUG
#define TIME_USER_CALL_BANKER		99									//��ׯ��ʱ��
#define TIME_USER_START_GAME		99									//��ʼ��ʱ��
#define TIME_USER_ADD_SCORE			99									//������ʱ��
#define TIME_USER_OPEN_CARD			99									//̯�ƶ�ʱ��
#else
#define TIME_USER_CALL_BANKER		5									//��ׯ��ʱ��
#define TIME_USER_START_GAME		5									//��ʼ��ʱ��
#define TIME_USER_ADD_SCORE			5									//������ʱ��
#define TIME_USER_OPEN_CARD			15									//̯�ƶ�ʱ��

#endif

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientDlg, CGameFrameDlg)

	ON_MESSAGE(IDM_HINT_OX,OnHintOx)
	ON_MESSAGE(IDM_START,OnStart)
	ON_MESSAGE(IDM_ADD_SCORE,OnAddScore)
	ON_MESSAGE(IDM_OX,OnOx)
	ON_MESSAGE(IDM_SHORTCUT_KEY,OnShortcut)
	ON_MESSAGE(IDM_READY,OnOpenCard)
	ON_MESSAGE(IDM_SEND_CARD_FINISH,OnSendCardFinish)
	ON_MESSAGE(IDM_SHOW_VALUE,OnSetUserOxValue)
	ON_MESSAGE(IDM_SHOW_INFO,OnShowInfo)
	ON_MESSAGE(IDM_OX_ENABLE,OnOxEnable)
	ON_MESSAGE(IDM_SORT,OnSortCard)
	ON_MESSAGE(IDM_RESORT,OnReSortCard)
	ON_MESSAGE(IDM_BANKER,OnBanker)
	ON_WM_TIMER()

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientDlg::CGameClientDlg() : CGameFrameDlg(&m_GameClientView)
{
	//���ñ���
	m_dwCardHSpace=DEFAULT_PELS;

	//��ע��Ϣ
	m_lTurnMaxScore=0L;
	//m_lTurnLessScore=0L;
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
	ZeroMemory(m_bUserOxCard,sizeof(m_bUserOxCard));

	//״̬����
	m_wBankerUser=INVALID_CHAIR;
	ZeroMemory(m_szAccounts,sizeof(m_szAccounts));
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));

	return;
}

//��������
CGameClientDlg::~CGameClientDlg()
{
}

//��ʼ����
bool CGameClientDlg::InitGameFrame()
{
	//����ͼ��
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);

	//��ȡ����
	m_dwCardHSpace=AfxGetApp()->GetProfileInt(TEXT("GameOption"),TEXT("CardSpace"),DEFAULT_PELS);

	//��������
	if ((m_dwCardHSpace>MAX_PELS)||(m_dwCardHSpace<LESS_PELS)) m_dwCardHSpace=18;

	//���ÿؼ�
	for (int i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.m_CardControlOx[i].SetDisplayFlag(true);
		m_GameClientView.m_CardControlOx[i].SetCardSpace(m_dwCardHSpace);
		m_GameClientView.m_CardControl[i].SetCardSpace(m_dwCardHSpace);
	}

	//���Դ���
#ifdef _DEBUG
	//m_GameClientView.m_btShortcut.ShowWindow(SW_SHOW);
	//BYTE bTemp[]={1,2,3,4,5};
	//for (BYTE i=0;i<GAME_PLAYER;i++)
	//{
	//	m_GameClientView.m_CardControl[i].SetDisplayFlag(true);
	//	m_GameClientView.m_CardControl[i].SetCardData(bTemp,5);
	//	m_GameClientView.m_CardControlOx[i].SetDisplayFlag(true);
	//	m_GameClientView.m_CardControlOx[i].SetCardData(bTemp,2);
	//}

#endif
	return true;
}

//���ÿ��
void CGameClientDlg::ResetGameFrame()
{
	//��ע��Ϣ
	m_lTurnMaxScore=0L;
	//m_lTurnLessScore=0L;
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
	ZeroMemory(m_bUserOxCard,sizeof(m_bUserOxCard));

	//״̬����
	m_wBankerUser=INVALID_CHAIR;
	ZeroMemory(m_szAccounts,sizeof(m_szAccounts));
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));

	return;
}

//��Ϸ����
void CGameClientDlg::OnGameOptionSet()
{
	//��������
	CGameOption GameOption;
	GameOption.m_dwCardHSpace=m_dwCardHSpace;
	GameOption.m_bEnableSound=IsEnableSound();
	GameOption.m_bAllowLookon=IsAllowUserLookon();

	//��������
	if (GameOption.DoModal()==IDOK)
	{
		//�����˿�
		m_dwCardHSpace=GameOption.m_dwCardHSpace;
		for (int i=0;i<GAME_PLAYER;i++) 
		{
			m_GameClientView.m_CardControl[i].SetCardSpace(m_dwCardHSpace);
			m_GameClientView.m_CardControlOx[i].SetCardSpace(m_dwCardHSpace);
		}

		//��������
		EnableSound(GameOption.m_bEnableSound);
		AllowUserLookon(0,GameOption.m_bAllowLookon);

		//��������
		AfxGetApp()->WriteProfileInt(TEXT("GameOption"),TEXT("CardSpace"),m_dwCardHSpace);
	}

	return;
}

//ʱ����Ϣ
bool CGameClientDlg::OnTimerMessage(WORD wChairID, UINT nElapse, UINT nTimerID)
{
	switch (nTimerID)
	{
	case IDI_START_GAME:		//��ʼ��ʱ��
		{
			//��ֹ�ж�
			if (nElapse==0)
			{
				PostMessage(WM_CLOSE,0,0);
				return false;
			}

			//����֪ͨ
			if (nElapse<=5) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));

			return true;
		}
	case IDI_NULLITY:			//����ʱ��
		{
			//��ֹ�ж�
			if (nElapse==0)
			{
				KillGameTimer(IDI_NULLITY);
				return false;
			}

			//����֪ͨ
			if (nElapse<=5) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));
			return true;
		}
	case IDI_CALL_BANKER:		//��ׯ��ʱ
		{
			//��ֹ�ж�
			if (nElapse==0)
			{
				//���Դ���
				//������ׯ
				OnBanker(0,0);
				return false;
			}

			//����֪ͨ
			if (nElapse<=5) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));
			return true;			
		}
	}

	return false;
}

//�Թ�״̬
void CGameClientDlg::OnLookonChanged(bool bLookonUser, const void * pBuffer, WORD wDataSize)
{
	//�ؼ�����
	if(bLookonUser)
	{
		if(IsAllowLookon() )
			m_GameClientView.m_CardControl[m_wViewChairID[GetMeChairID()]].SetDisplayFlag(true);
		else m_GameClientView.m_CardControl[m_wViewChairID[GetMeChairID()]].SetDisplayFlag(false);
	}
	return;
}

//������Ϣ
bool CGameClientDlg::OnGameMessage(WORD wSubCmdID, const void * pBuffer, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_CALL_BANKER:	//�û���ׯ
		{
			//��Ϣ����
			return OnSubCallBanker(pBuffer,wDataSize);
		}
	case SUB_S_GAME_START:	//��Ϸ��ʼ
		{
			//��Ϣ����
			return OnSubGameStart(pBuffer,wDataSize);
		}
	case SUB_S_ADD_SCORE:	//�û���ע
		{
			//��Ϣ����
			return OnSubAddScore(pBuffer,wDataSize);
		}
	case SUB_S_SEND_CARD:	//������Ϣ
		{
			//��Ϣ����
			return OnSubSendCard(pBuffer,wDataSize);
		}
	case SUB_S_OPEN_CARD:	//�û�̯��
		{
			//��Ϣ����
			return OnSubOpenCard(pBuffer,wDataSize);
		}
	case SUB_S_PLAYER_EXIT:	//�û�ǿ��
		{
			//��Ϣ����
			return OnSubPlayerExit(pBuffer,wDataSize);
		}
	case SUB_S_GAME_END:	//��Ϸ����
		{
			//��������
			m_GameClientView.FinishDispatchCard();

			//��Ϣ����
			return OnSubGameEnd(pBuffer,wDataSize);
		}
	}

	return false;
}

//��Ϸ����
bool CGameClientDlg::OnGameSceneMessage(BYTE cbGameStation, bool bLookonOther, const void * pBuffer, WORD wDataSize)
{
	if(IsLookonMode())m_GameClientView.m_bLookOnUser=true;
	switch (cbGameStation)
	{
	case GS_FREE:		//����״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pBuffer;

			//���ÿؼ�
			if (IsLookonMode()==false)
			{
				SetGameTimer(GetMeChairID(),IDI_START_GAME,TIME_USER_START_GAME);
				m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
				m_GameClientView.m_btStart.SetFocus();
			}

			return true;
		}
	case GS_TK_CALL:	// ��ׯ״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusCall)) return false;
			CMD_S_StatusCall * pStatusCall=(CMD_S_StatusCall *)pBuffer;

			//�û���Ϣ
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//��ͼλ��
				m_wViewChairID[i]=SwitchViewChairID(i);

				//��ȡ�û�
				const tagUserData * pUserData=GetUserData(i);
				if (pUserData==NULL) continue;

				//��Ϸ��Ϣ
				m_cbPlayStatus[i]=TRUE;

				//�û�����
				lstrcpyn(m_szAccounts[i],pUserData->szName,CountArray(m_szAccounts[i]));
			}
			
			//ʼ���û�
			if(!IsLookonMode() && pStatusCall->wCallBanker==GetMeChairID())
			{
				//�ؼ���ʾ
				//ActiveGameFrame();
				m_GameClientView.m_btBanker.ShowWindow(SW_SHOW);
				m_GameClientView.m_btIdler.ShowWindow(SW_SHOW);
			}

			//�ȴ���־
			WORD wViewID=m_wViewChairID[pStatusCall->wCallBanker];
			m_GameClientView.SetWaitCall((BYTE)wViewID);

			//ʵ�ʶ�ʱ��
			if(pStatusCall->wCallBanker==GetMeChairID())
			{
				SetGameTimer(pStatusCall->wCallBanker,IDI_CALL_BANKER,TIME_USER_CALL_BANKER);
			}
			else SetGameTimer(pStatusCall->wCallBanker,IDI_NULLITY,TIME_USER_CALL_BANKER);

			return true;
		}
	case GS_TK_SCORE:	//��ע״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusScore)) return false;
			CMD_S_StatusScore * pStatusScore=(CMD_S_StatusScore *)pBuffer;

			//���ñ���
			m_lTurnMaxScore=pStatusScore->lTurnMaxScore;
			m_wBankerUser=pStatusScore->wBankerUser;			
			CopyMemory(m_lTableScore,pStatusScore->lTableScore,sizeof(m_lTableScore));
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//��ͼλ��
				m_wViewChairID[i]=SwitchViewChairID(i);

				//�������
				if(m_lTableScore[i]>0L)m_GameClientView.SetUserTableScore(m_wViewChairID[i],m_lTableScore[i]);
				
				//��ȡ�û�
				const tagUserData * pUserData=GetUserData(i);
				if (pUserData==NULL) continue;
				
				//��Ϸ��Ϣ
				m_cbPlayStatus[i]=TRUE;
				
				//�û�����
				lstrcpyn(m_szAccounts[i],pUserData->szName,CountArray(m_szAccounts[i]));
			}

			//���ó���
			if (!IsLookonMode() && pStatusScore->lTurnMaxScore>0L && m_lTableScore[GetMeChairID()]==0L)
			{
				LONG lUserMaxScore[GAME_PLAYER];
				ZeroMemory(lUserMaxScore,sizeof(lUserMaxScore));
				LONG lTemp=m_lTurnMaxScore;
				for (WORD i=0;i<GAME_PLAYER;i++)
				{
					if(i>0)lTemp/=2;
					lUserMaxScore[i]=__max(lTemp,1L);
				}

				//���¿ؼ�
				UpdateScoreControl(lUserMaxScore,SW_SHOW);

				//ʵ�ʶ�ʱ��
				SetTimer(IDI_TIME_USER_ADD_SCORE,(TIME_USER_ADD_SCORE)*1000,NULL);
			}

			//ׯ�ұ�־
			WORD wID=m_wViewChairID[m_wBankerUser];
			m_GameClientView.SetBankerUser(wID);

			//�ȴ���־
			m_GameClientView.SetWaitInvest(true);


			//������ʾ����ʱ��
			SetGameTimer(GetMeChairID(),IDI_NULLITY,TIME_USER_ADD_SCORE);

			return true;
		}
	case GS_TK_PLAYING:	//��Ϸ״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pBuffer;

			//���ñ���
			m_lTurnMaxScore=pStatusPlay->lTurnMaxScore;
			m_wBankerUser=pStatusPlay->wBankerUser;
			CopyMemory(m_lTableScore,pStatusPlay->lTableScore,sizeof(m_lTableScore));
			CopyMemory(m_bUserOxCard,pStatusPlay->bOxCard,sizeof(m_bUserOxCard));
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//��ͼλ��
				m_wViewChairID[i]=SwitchViewChairID(i);

				//�������
				if(m_lTableScore[i]>0L)m_GameClientView.SetUserTableScore(m_wViewChairID[i],m_lTableScore[i]);
				
				//��ȡ�û�
				const tagUserData * pUserData=GetUserData(i);
				if (pUserData==NULL) continue;
				
				//��Ϸ��Ϣ
				m_cbPlayStatus[i]=TRUE;
				
				//�˿�����
				CopyMemory(m_cbHandCardData[i],pStatusPlay->cbHandCardData[i],MAX_COUNT);

				//�û�����
				lstrcpyn(m_szAccounts[i],pUserData->szName,CountArray(m_szAccounts[i]));
			}

			//ׯ�ұ�־
			WORD wID=m_wViewChairID[m_wBankerUser];
			m_GameClientView.SetBankerUser(wID);

			//������Ϣ
			//m_GameClientView.SetScoreInfo(m_lTurnMaxScore,0);

			//���ý���
			LONG lTableScore=0L;
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//����λ��
				WORD wViewChairID=m_wViewChairID[i];

				//�����˿�
				if (m_cbPlayStatus[i]==TRUE) 
				{
					m_GameClientView.m_CardControl[wViewChairID].SetCardData(pStatusPlay->cbHandCardData[i],MAX_COUNT);
				}
			}

			WORD wMeChiarID=GetMeChairID();
			WORD wViewChairID=m_wViewChairID[wMeChiarID];
			if(!IsLookonMode())m_GameClientView.m_CardControl[wViewChairID].SetPositively(true);
			if(IsAllowLookon() || !IsLookonMode())
			{				
				m_GameClientView.m_CardControl[wViewChairID].SetDisplayFlag(true);
			}

			//̯�Ʊ�־
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				if(m_bUserOxCard[i]!=0xff)
				{
					m_GameClientView.ShowOpenCard(m_wViewChairID[i]);
				}
			}

			//�ؼ�����
			if(!IsLookonMode())
			{
				//��ʾ�ؼ�
				if(m_bUserOxCard[wMeChiarID]==0xff)
				{
					OnSendCardFinish(0,0);
				}
				else 
				{
					m_GameClientView.m_CardControl[wViewChairID].SetPositively(false);
					if(m_bUserOxCard[wMeChiarID]==TRUE)//ţ�Ʒ���
					{
						//���ñ���
						BYTE bTemp[MAX_COUNT];
						CopyMemory(bTemp,m_cbHandCardData[wMeChiarID],sizeof(bTemp));

						//��ȡţ��
						m_GameLogic.GetOxCard(bTemp,MAX_COUNT);

						//���ÿؼ�
						m_GameClientView.m_CardControl[wViewChairID].SetCardData(bTemp,3);
						m_GameClientView.m_CardControlOx[wViewChairID].SetCardData(&bTemp[3],2);						

						//��ʾ����
						BYTE bValue=m_GameLogic.GetCardLogicValue(bTemp[3])+m_GameLogic.GetCardLogicValue(bTemp[4]);
						ASSERT(bValue>0);
						if(bValue>10)bValue-=10;
						m_GameClientView.SetUserOxValue(wViewChairID,bValue);
					}
					else 
					{
						//��ţ
						m_GameClientView.SetUserOxValue(wViewChairID,0);
					}					
				}
			}
			return true;
		}
	}

	return false;
}

//�û���ׯ
bool CGameClientDlg::OnSubCallBanker(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_CallBanker)) return false;
	CMD_S_CallBanker * pCallBanker=(CMD_S_CallBanker *)pBuffer;

	//�û���Ϣ
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��ͼλ��
		m_wViewChairID[i]=SwitchViewChairID(i);

		//��ȡ�û�
		const tagUserData * pUserData=GetUserData(i);
		if (pUserData==NULL) continue;

		//��Ϸ��Ϣ
		m_cbPlayStatus[i]=TRUE;

		//�û�����
		lstrcpyn(m_szAccounts[i],pUserData->szName,CountArray(m_szAccounts[i]));
	}

	//�Թ���
	if (IsLookonMode())
	{
		//���ý���
		m_GameClientView.DisplayType(false);
		m_GameClientView.SetBankerUser(INVALID_CHAIR);
		//m_GameClientView.SetScoreInfo(0,0);
		m_GameClientView.m_btStart.ShowWindow(SW_HIDE);
		m_GameClientView.m_ScoreView.ShowWindow(SW_HIDE);
		m_GameClientView.SetUserTableScore(INVALID_CHAIR,0L);

		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			m_GameClientView.ShowScore(i,false);
			m_GameClientView.m_CardControl[i].SetOX(false);
			m_GameClientView.SetUserOxValue(i,0xff);
			m_GameClientView.m_CardControl[i].SetCardData(NULL,0);
			if(i!=m_wViewChairID[GetMeChairID()])m_GameClientView.m_CardControl[i].SetDisplayFlag(false);
			m_GameClientView.m_CardControlOx[i].SetCardData(NULL,0);
		}

		//��ע��Ϣ
		m_lTurnMaxScore=0L;
		ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
		ZeroMemory(m_bUserOxCard,sizeof(m_bUserOxCard));
		ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
	}

	//ʼ���û�
	if(!IsLookonMode() && pCallBanker->wCallBanker==GetMeChairID())
	{
		//�ؼ���ʾ
		//ActiveGameFrame();
		m_GameClientView.m_btBanker.ShowWindow(SW_SHOW);
		m_GameClientView.m_btIdler.ShowWindow(SW_SHOW);
	}

	//�ȴ���־
	WORD wViewID=m_wViewChairID[pCallBanker->wCallBanker];
	m_GameClientView.SetWaitCall((BYTE)wViewID);

	//ʵ�ʶ�ʱ��
	if(pCallBanker->wCallBanker==GetMeChairID())
	{
		SetGameTimer(pCallBanker->wCallBanker,IDI_CALL_BANKER,TIME_USER_CALL_BANKER);
	}
	else SetGameTimer(pCallBanker->wCallBanker,IDI_NULLITY,TIME_USER_CALL_BANKER);

	return true;
}

//��Ϸ��ʼ
bool CGameClientDlg::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

	//���ñ���
	m_lTurnMaxScore=pGameStart->lTurnMaxScore;
	m_wBankerUser=pGameStart->wBankerUser;
	m_GameClientView.SetWaitCall(0xff);

	//���ó���
	if (!IsLookonMode() && pGameStart->lTurnMaxScore>0)
	{
		LONG lUserMaxScore[GAME_PLAYER];
		ZeroMemory(lUserMaxScore,sizeof(lUserMaxScore));
		LONG lTemp=m_lTurnMaxScore;
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			if(i>0)lTemp/=2;
			lUserMaxScore[i]=__max(lTemp,1L);
		}

		//���¿ؼ�
		//ActiveGameFrame();
		UpdateScoreControl(lUserMaxScore,SW_SHOW);

		//ʵ�ʶ�ʱ��
		SetTimer(IDI_TIME_USER_ADD_SCORE,(TIME_USER_ADD_SCORE)*1000,NULL);
	}

	//ׯ�ұ�־
	WORD wID=m_wViewChairID[m_wBankerUser];
	m_GameClientView.SetBankerUser(wID);

	//�ȴ���־
	m_GameClientView.SetWaitInvest(true);

	//������Ϣ
	//m_GameClientView.SetScoreInfo(m_lTurnMaxScore,m_lTurnLessScore);

	//������ʾ����ʱ��
	SetGameTimer(GetMeChairID(),IDI_NULLITY,TIME_USER_ADD_SCORE);

	//��������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));

	return true;
}

//�û���ע
bool CGameClientDlg::OnSubAddScore(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_AddScore)) return false;
	CMD_S_AddScore * pAddScore=(CMD_S_AddScore *)pBuffer;

	//��������
	WORD wMeChairID=GetMeChairID();
	WORD wAddScoreUser=pAddScore->wAddScoreUser;
	WORD wViewChairID=m_wViewChairID[wAddScoreUser];

	//��ע����
	if ((IsLookonMode()==true)||(pAddScore->wAddScoreUser!=wMeChairID))
	{
		//��ע���
		m_lTableScore[pAddScore->wAddScoreUser]=pAddScore->lAddScoreCount;

		//��ע����
		m_GameClientView.SetUserTableScore(wViewChairID,pAddScore->lAddScoreCount);

		//��������
		if (m_cbPlayStatus[wAddScoreUser]==TRUE)
		{
			 PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_SCORE"));
		}
	}

	//���ó���
	m_GameClientView.SetUserTableScore(wViewChairID,m_lTableScore[wAddScoreUser]);

	return true;
}

//������Ϣ
bool CGameClientDlg::OnSubSendCard(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_SendCard)) return false;
	CMD_S_SendCard * pSendCard=(CMD_S_SendCard *)pBuffer;

	//ɾ����ʱ��
	KillGameTimer(IDI_NULLITY);

	m_GameClientView.SetWaitInvest(false);

	CopyMemory(m_cbHandCardData,pSendCard->cbCardData,sizeof(m_cbHandCardData));

	WORD wMeChiarID=GetMeChairID();
	WORD wViewChairID=m_wViewChairID[wMeChiarID];
	if(IsAllowLookon() || !IsLookonMode())
	{
		m_GameClientView.m_CardControl[wViewChairID].SetDisplayFlag(true);
	}

	//�ɷ��˿�
	for(WORD i=0;i<MAX_COUNT;i++)
	{
		for (WORD j=m_wBankerUser;j<m_wBankerUser+GAME_PLAYER;j++)
		{
			WORD w=j%GAME_PLAYER;
			if (m_cbPlayStatus[w]==TRUE)
			{
				WORD wViewChairID=m_wViewChairID[w];
				if(pSendCard->cbCardData[w][i]!=0)
				{
					m_GameClientView.DispatchUserCard(wViewChairID,m_cbHandCardData[w][i]);
				}
			}
		}
	}

	return true;
}

//�û�̯��
bool CGameClientDlg::OnSubOpenCard(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_Open_Card)) return false;
	CMD_S_Open_Card * pOpenCard=(CMD_S_Open_Card *)pBuffer;

	//��Ϸ��Ϣ
	WORD wID=pOpenCard->wPlayerID;
	//ASSERT(m_cbPlayStatus[wID]==TRUE);

	//̯�Ʊ�־
	if(wID!=GetMeChairID() || IsLookonMode())
	{
		WORD wViewChairID=m_wViewChairID[wID];
		m_GameClientView.ShowOpenCard(wViewChairID);
		//����ţ��Ϣ
		m_bUserOxCard[wID]=pOpenCard->bOpen;
		PlayGameSound(AfxGetInstanceHandle(),TEXT("OPEN_CARD"));
	}

	return true;
}

//�û�ǿ��
bool CGameClientDlg::OnSubPlayerExit(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_PlayerExit)) return false;
	CMD_S_PlayerExit * pPlayerExit=(CMD_S_PlayerExit *)pBuffer;

	WORD wID=pPlayerExit->wPlayerID;

	//��Ϸ��Ϣ
	ASSERT(m_cbPlayStatus[wID]==TRUE);
	m_cbPlayStatus[wID]=FALSE;

	//�û�����
	for(WORD i=0;i<32;i++)m_szAccounts[wID][i]=0;
	m_szAccounts[wID][0] = '#';

	BYTE bCount=0;
	for(WORD i=0;i<GAME_PLAYER;i++)if(m_cbPlayStatus[i]!=FALSE)bCount++;
	if(bCount==1)m_GameClientView.SetBankerUser(INVALID_CHAIR);

	return true;

}

//��Ϸ����
bool CGameClientDlg::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//Ч�����
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;

	//ɾ����ʱ��
	KillGameTimer(IDI_NULLITY);
	KillGameTimer(IDI_CALL_BANKER);
	KillTimer(IDI_TIME_USER_ADD_SCORE);
	KillTimer(IDI_TIME_OPEN_CARD);

	//��������
	for(WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.ShowOpenCard(i,FALSE);
	}	
	m_GameClientView.SetWaitInvest(false);
	UpdateScoreControl(NULL,SW_HIDE);
	m_GameClientView.m_btOx.ShowWindow(SW_HIDE);
	m_GameClientView.m_btHintOx.ShowWindow(SW_HIDE);
	m_GameClientView.m_btOpenCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btReSort.ShowWindow(SW_HIDE);
	m_GameClientView.m_btShortcut.ShowWindow(SW_HIDE);
	m_GameClientView.m_btBanker.ShowWindow(SW_HIDE);
	m_GameClientView.m_btIdler.ShowWindow(SW_HIDE);
	m_GameClientView.SetWaitCall(0xff);
	m_GameClientView.m_bOpenCard=false;

	//״̬����
	SetGameStatus(GS_FREE);

	//��������
	if (IsLookonMode()==false)
	{
		if (pGameEnd->lGameScore[GetMeChairID()]>0L) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WIN"));
		else PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_LOST"));
	}
	else PlayGameSound(GetModuleHandle(NULL),TEXT("GAME_END"));

	//��ʾ����
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if(GetUserData(i)!=NULL)m_GameClientView.ShowScore(m_wViewChairID[i],true);
		m_GameClientView.SetUserTableScore(m_wViewChairID[i],pGameEnd->lGameScore[i]);
	}
	m_GameClientView.m_ScoreView.ResetScore();
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//������Ϣ
		if (pGameEnd->lGameScore[i]!=0L || i==m_wBankerUser)
		{
			m_GameClientView.m_ScoreView.SetGameTax(pGameEnd->lGameTax[i],i);
			if (m_szAccounts[i][0]=='#') m_GameClientView.m_ScoreView.SetGameScore(i,TEXT("���뿪"),pGameEnd->lGameScore[i]);
			else m_GameClientView.m_ScoreView.SetGameScore(i,m_szAccounts[i],pGameEnd->lGameScore[i]);
		}
	}
	m_GameClientView.m_ScoreView.ShowWindow(SW_SHOW);

	//��ʾ����
	bool bOxSound=false;
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		WORD wViewChairID=m_wViewChairID[i];
		if(i==GetMeChairID() && !IsLookonMode())continue;
		m_GameClientView.m_CardControl[wViewChairID].SetDisplayFlag(true);

		//ţţ����
		if(m_cbHandCardData[i][0]>0)
		{
			if(m_bUserOxCard[i]==TRUE)
			{
				//�˿�����
				BYTE bCardData[MAX_COUNT];
				CopyMemory(bCardData,m_cbHandCardData[i],sizeof(bCardData));

				//��ȡţţ����
				bool bOx=m_GameLogic.GetOxCard(bCardData,MAX_COUNT);
				ASSERT(bOx);

				//��������
				m_GameClientView.m_CardControl[wViewChairID].SetCardData(bCardData,3);
				m_GameClientView.m_CardControlOx[wViewChairID].SetCardData(&bCardData[3],2);

				//��ʾ����
				BYTE bValue=m_GameLogic.GetCardLogicValue(bCardData[3])+m_GameLogic.GetCardLogicValue(bCardData[4]);
				if(bValue>10)bValue-=10;
				ASSERT(bValue>0);
				if(bValue>=10)bOxSound=true;
				m_GameClientView.SetUserOxValue(wViewChairID,bValue);
			}
			else
			{
				//��ţ
				m_GameClientView.SetUserOxValue(wViewChairID,0);
			}
		}
	}
	m_GameClientView.DisplayType(true);

	//ţţ����
	if(bOxSound)
	{
		PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_OXOX"));
	}

	//��ʼ��ť
	if (IsLookonMode()==false)
	{
		m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
		SetGameTimer(GetMeChairID(),IDI_START_GAME,TIME_USER_START_GAME);
	}

	//�ɼ���ʾ�ڼ�ʱ����Ի���
	TCHAR szBuffer[512]=TEXT("");
	_snprintf(szBuffer,CountArray(szBuffer),TEXT("\n���ֽ���,�ɼ�ͳ��:"));
	InsertGeneralString(szBuffer,RGB(0,128,255),true);
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��������
		const tagUserData * pUserData=GetUserData(i);
		//�ɼ����
		if (pUserData!=NULL)
		{
			_snprintf(szBuffer,CountArray(szBuffer),TEXT("%s:%+d"),pUserData->szName,pGameEnd->lGameScore[i]);
			InsertGeneralString(szBuffer,RGB(0,128,255),true);
		}
		else if(m_szAccounts[i][0]=='#')
		{
			_snprintf(szBuffer,CountArray(szBuffer),TEXT("%s:%+d"),TEXT("�û��뿪"),pGameEnd->lGameScore[i]);
			InsertGeneralString(szBuffer,RGB(0,128,255),true);
		}
	}

	//״̬����
	m_wBankerUser=INVALID_CHAIR;
	ZeroMemory(m_szAccounts,sizeof(m_szAccounts));
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));

	return true;
}

//���¿���
void CGameClientDlg::UpdateScoreControl(LONG lScore[],BOOL bShow)
{
	if(lScore!=NULL)
	{
		TCHAR tsz[GAME_PLAYER][NAME_LEN];
		ZeroMemory(tsz,sizeof(tsz));
		for(BYTE i=0;i<GAME_PLAYER;i++)
		{
			sprintf(tsz[i],"%d",lScore[i]);
		}
		m_GameClientView.m_btOneScore.SetWindowText(tsz[0]);
		m_GameClientView.m_btTwoScore.SetWindowText(tsz[1]);
		m_GameClientView.m_btThreeScore.SetWindowText(tsz[2]);
		m_GameClientView.m_btFourScore.SetWindowText(tsz[3]);
	}

	m_GameClientView.m_btOneScore.ShowWindow(bShow);
	m_GameClientView.m_btTwoScore.ShowWindow(bShow);
	m_GameClientView.m_btThreeScore.ShowWindow(bShow);
	m_GameClientView.m_btFourScore.ShowWindow(bShow);

	return ;
}

//��ʾ��ť
LRESULT	CGameClientDlg::OnHintOx(WPARAM wParam, LPARAM lParam)
{
	if(m_GameLogic.GetOxCard(m_cbHandCardData[GetMeChairID()],MAX_COUNT))
	{
		m_GameClientView.m_CardControl[MY_VIEW_CHAIRID].SetShootCard(m_cbHandCardData[GetMeChairID()],3);
		if(m_GameClientView.m_btOx.IsWindowVisible()==FALSE)OnShortcut(0,0);
		OnOxEnable(1,1);
	}
	else 
	{
		m_GameClientView.SetUserOxValue(MY_VIEW_CHAIRID,0);
	}

	return 0;
}

//��ʼ��ť
LRESULT	CGameClientDlg::OnStart(WPARAM wParam, LPARAM lParam)
{
	//ɾ����ʱ��
	KillGameTimer(IDI_START_GAME);

	//���ý���
	m_GameClientView.DisplayType(false);
	m_GameClientView.SetBankerUser(INVALID_CHAIR);
	m_GameClientView.m_btStart.ShowWindow(SW_HIDE);
	m_GameClientView.m_ScoreView.ShowWindow(SW_HIDE);
	m_GameClientView.SetUserTableScore(INVALID_CHAIR,0L);

	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.ShowScore(i,false);
		m_GameClientView.m_CardControl[i].SetOX(false);
		m_GameClientView.SetUserOxValue(i,0xff);
		m_GameClientView.m_CardControl[i].SetCardData(NULL,0);
		if(i!=m_wViewChairID[GetMeChairID()])m_GameClientView.m_CardControl[i].SetDisplayFlag(false);
		m_GameClientView.m_CardControlOx[i].SetCardData(NULL,0);
	}

	//��ע��Ϣ
	m_lTurnMaxScore=0L;
	//m_lTurnLessScore=0L;m_cbHandCardData
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	ZeroMemory(m_bUserOxCard,sizeof(m_bUserOxCard));
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));

	//������Ϣ
	SendUserReady(NULL,0);

	return 0;
}

//��ע��ť
LRESULT	CGameClientDlg::OnAddScore(WPARAM wParam, LPARAM lParam)
{
	//ɾ����ʱ��
	KillTimer(IDI_TIME_USER_ADD_SCORE);

	//��ȡ����
	WORD wMeChairID=GetMeChairID();

	BYTE bTemp=(BYTE)wParam;
	LONG lCurrentScore=0;
	if(bTemp==1)lCurrentScore=__max(m_lTurnMaxScore/8,1L);
	else if(bTemp==2)lCurrentScore=__max(m_lTurnMaxScore/4,1L);
	else if(bTemp==3)lCurrentScore=__max(m_lTurnMaxScore/2,1L);
	else if(bTemp==4)lCurrentScore=__max(m_lTurnMaxScore,1L);

	//���ư�ť
	UpdateScoreControl(NULL,SW_HIDE);

	//��������
	m_GameClientView.SetUserTableScore(m_wViewChairID[wMeChairID],m_lTableScore[wMeChairID]);
	m_GameClientView.UpdateGameView(NULL);
	m_lTableScore[wMeChairID]=lCurrentScore;
	PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_SCORE"));

	//������Ϣ
	CMD_C_AddScore AddScore;
	AddScore.lScore=lCurrentScore;
	SendData(SUB_C_ADD_SCORE,&AddScore,sizeof(AddScore));

	return 0;
}

//��ݼ���ť 
LRESULT CGameClientDlg::OnShortcut(WPARAM wParam, LPARAM lParam)
{
	//WORD wViewChairID=m_wViewChairID[GetMeChairID()];

	//if(m_GameClientView.m_btOx.IsWindowVisible()==FALSE)
	//{
	//	m_GameClientView.m_CardControl[wViewChairID].SetKey(false);
	//	m_GameClientView.m_btOx.ShowWindow(SW_SHOW);
	//}
	//else
	//{
	//	m_GameClientView.m_CardControl[wViewChairID].SetKey(true);
	//	m_GameClientView.m_btOx.ShowWindow(SW_HIDE);
	//}
	//m_GameClientView.m_btOx.EnableWindow(FALSE);

	return 0;
}

//ţţ��ť
LRESULT	CGameClientDlg::OnOx(WPARAM wParam, LPARAM lParam)
{
	//ţţ
	m_GameClientView.m_CardControl[MY_VIEW_CHAIRID].SetOX(true);

	//ȡ����Ӧ
	m_GameClientView.m_CardControl[MY_VIEW_CHAIRID].SetPositively(false);

	//����ţ��
	OnSortCard(0,0);

	//��ʾ����
	OnSetUserOxValue(0,0);

	m_GameClientView.m_btOx.EnableWindow(FALSE);

	//ֱ�ӿ���
	OnOpenCard(0,0);

	return 0;
}

//�������
LRESULT CGameClientDlg::OnSendCardFinish(WPARAM wParam, LPARAM lParam)
{
	//��Ӧ�˿�
	WORD wMeChairID=GetMeChairID();
	if(!IsLookonMode())m_GameClientView.m_CardControl[MY_VIEW_CHAIRID].SetPositively(true);

	//����ʱ��
	SetGameTimer(wMeChairID,IDI_NULLITY,TIME_USER_OPEN_CARD);

	//���ǹۿ�
	if (IsLookonMode())return 0;

	//�ؼ�����
	if(!m_GameClientView.m_CardControl[MY_VIEW_CHAIRID].GetKey())
	{
		//m_GameClientView.m_btOx.ShowWindow(SW_SHOW);
		m_GameClientView.m_btOx.EnableWindow(FALSE);
	}
	m_GameClientView.m_btOpenCard.ShowWindow(SW_SHOW);
	m_GameClientView.m_btHintOx.ShowWindow(SW_SHOW);
	//m_GameClientView.m_btReSort.ShowWindow(SW_SHOW);
	//m_GameClientView.m_btShortcut.ShowWindow(SW_SHOW);

	//�ȴ���־
	m_GameClientView.m_bOpenCard=true;

	//ʱ������
	SetTimer(IDI_TIME_OPEN_CARD,TIME_USER_OPEN_CARD*1000,NULL);

	return 0;
}

//̯�ư�ť
LRESULT	CGameClientDlg::OnOpenCard(WPARAM wParam, LPARAM lParam)
{
	if(m_GameClientView.m_btOx.IsWindowEnabled()==TRUE)
	{
		//ţţ
		m_GameClientView.m_CardControl[MY_VIEW_CHAIRID].SetOX(true);

		//ȡ����Ӧ
		m_GameClientView.m_CardControl[MY_VIEW_CHAIRID].SetPositively(false);

		//����ţ��
		OnSortCard(0,0);

		//��ʾ����
		//OnSetUserOxValue(0,0);

		m_GameClientView.m_btOx.EnableWindow(FALSE);
	}

	//ɾ��ʱ��
	KillTimer(IDI_TIME_OPEN_CARD);

	//����ؼ�
	m_GameClientView.m_btOx.ShowWindow(SW_HIDE);
	m_GameClientView.m_btOpenCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btHintOx.ShowWindow(SW_HIDE);
	m_GameClientView.m_btReSort.ShowWindow(SW_HIDE);
	m_GameClientView.m_btShortcut.ShowWindow(SW_HIDE);

	WORD wViewChairID=MY_VIEW_CHAIRID;

	//������Ϣ
	CMD_C_OxCard OxCard;
	OxCard.bOX=(m_GameClientView.m_CardControl[wViewChairID].GetOX())?TRUE:FALSE;
	SendData(SUB_C_OPEN_CARD,&OxCard,sizeof(OxCard));

	//Ԥ�ȴ���
	if(OxCard.bOX==FALSE)
	{
		//��ʾ������ţţ
		m_GameClientView.m_CardControl[wViewChairID].ShootAllCard(false);
		m_GameClientView.SetUserOxValue(wViewChairID,0);
	}
	m_GameClientView.ShowOpenCard(wViewChairID);
	m_GameClientView.m_CardControl[wViewChairID].SetPositively(false);

	//����ţ��Ϣ
	m_bUserOxCard[GetMeChairID()]=OxCard.bOX;

	PlayGameSound(AfxGetInstanceHandle(),TEXT("OPEN_CARD"));

	return 0;
}

//��ʾ����
LRESULT CGameClientDlg::OnSetUserOxValue(WPARAM wParam, LPARAM lParam)
{
	WORD wViewChairID=m_wViewChairID[GetMeChairID()];

	//��ȡ����
	BYTE bCardData[MAX_COUNT],bCardCount=0;
	ZeroMemory(bCardData,sizeof(bCardData));
	bCardCount=(BYTE)m_GameClientView.m_CardControlOx[wViewChairID].GetCardCount();
	ASSERT(bCardCount==2);
	m_GameClientView.m_CardControlOx[wViewChairID].GetCardData(bCardData,bCardCount);

	//��ʾ����
	BYTE bSum=m_GameLogic.GetCardLogicValue(bCardData[0])+m_GameLogic.GetCardLogicValue(bCardData[1]);
	if(bSum>10)bSum-=10;
	m_GameClientView.SetUserOxValue(wViewChairID,bSum);

	return 0;
}

//��ʾ��Ϣ
LRESULT CGameClientDlg::OnShowInfo(WPARAM wParam, LPARAM lParam)
{
	//��������
	TCHAR szBuffer[64];
	_snprintf(szBuffer,CountArray(szBuffer),TEXT("����ʾ��Ϣ��:���ܹ���ţ"));
	InsertGeneralString(szBuffer,RGB(255,128,0),true);

	return 0;
}

//ţBTΪ��
LRESULT CGameClientDlg::OnOxEnable(WPARAM wParam, LPARAM lParam)
{
	BYTE bEnable=BYTE(wParam);
	m_GameClientView.m_btOx.EnableWindow(bEnable);
	if(bEnable==TRUE)
	{
		bool bShoot = ((BYTE(lParam)==TRUE)?false:true);
		BYTE cbCardData[MAX_COUNT];
		m_GameClientView.m_CardControl[MY_VIEW_CHAIRID].GetShootCard(cbCardData,bShoot);
		BYTE cbValue=m_GameLogic.GetCardLogicValue(cbCardData[0]);
		cbValue+=m_GameLogic.GetCardLogicValue(cbCardData[1]);
		if(cbValue>10)cbValue-=10;
		m_GameClientView.SetUserOxValue(MY_VIEW_CHAIRID,cbValue);
	}
	else 
	{
		BYTE cbValue=m_GameClientView.GetValue(MY_VIEW_CHAIRID);
		if(cbValue!=0xff && cbValue!=0)
			m_GameClientView.SetUserOxValue(MY_VIEW_CHAIRID,0xff);
	}

	return 0;
}

//�˿˷���
LRESULT CGameClientDlg::OnSortCard(WPARAM wParam, LPARAM lParam)
{
	//���ؿؼ�
	m_GameClientView.m_btHintOx.ShowWindow(SW_HIDE);

	//���ñ���
	WORD wViewChairID=m_wViewChairID[GetMeChairID()];

	//������Ŀ
	BYTE bCardCount=(BYTE)m_GameClientView.m_CardControl[wViewChairID].GetShootCount();

	//���ñ���
	BYTE bCardOxData[MAX_COUNT];
	BYTE bCardValueData[MAX_COUNT];
	ZeroMemory(bCardOxData,sizeof(bCardOxData));
	ZeroMemory(bCardValueData,sizeof(bCardValueData));

	//��ȡţ��&������
	if(bCardCount==2)
	{
		BYTE bCount=(BYTE)m_GameClientView.m_CardControl[wViewChairID].GetShootCard(bCardOxData,false);
		ASSERT(bCount==3);
		bCount=(BYTE)m_GameClientView.m_CardControl[wViewChairID].GetShootCard(bCardValueData);
		ASSERT(bCount==2);
	}
	else
	{
		BYTE bCount=(BYTE)m_GameClientView.m_CardControl[wViewChairID].GetShootCard(bCardValueData,false);
		ASSERT(bCount==2);
		bCount=(BYTE)m_GameClientView.m_CardControl[wViewChairID].GetShootCard(bCardOxData);
		ASSERT(bCount==3);
	}

	//�����˿�
	m_GameClientView.m_CardControlOx[wViewChairID].SetCardData(bCardValueData,MAX_COUNT-3);
	m_GameClientView.m_CardControl[wViewChairID].SetCardData(bCardOxData,3);

	return 0;
}

//���Ű�ť
LRESULT CGameClientDlg::OnReSortCard(WPARAM wParam, LPARAM lParam)
{
	WORD wMeChiarID=GetMeChairID();
	WORD wViewChairID=m_wViewChairID[wMeChiarID];

	//ȡ��ţţ
	if(m_GameClientView.m_CardControl[wViewChairID].GetOX())
	{
		m_GameClientView.m_CardControlOx[wViewChairID].SetCardData(NULL,0);
		m_GameClientView.m_CardControl[wViewChairID].SetCardData(m_cbHandCardData[wMeChiarID],MAX_COUNT);
		m_GameClientView.m_CardControl[wViewChairID].SetPositively(true);
		m_GameClientView.m_CardControl[wViewChairID].SetOX(false);

		//ȡ����ʾ����
		m_GameClientView.SetUserOxValue(wViewChairID,0xff);
	}

	return 0;
}

//��ׯ��Ϣ
LRESULT CGameClientDlg::OnBanker(WPARAM wParam, LPARAM lParam)
{
	//ɾ����ʱ��
	KillGameTimer(IDI_CALL_BANKER);

	//�ؼ�����
	m_GameClientView.m_btBanker.ShowWindow(SW_HIDE);
	m_GameClientView.m_btIdler.ShowWindow(SW_HIDE);

	//���ñ���
	CMD_C_CallBanker CallBanker;
	CallBanker.bBanker = (BYTE)wParam;

	//������Ϣ
	SendData(SUB_C_CALL_BANKER,&CallBanker,sizeof(CallBanker));

	return 0;
}

//��ʱ����
void CGameClientDlg::OnTimer(UINT nIDEvent)
{
	switch(nIDEvent)
	{
	case IDI_TIME_USER_ADD_SCORE:
		{
			//���Դ���
			//m_GameClientView.m_btOpenCard.ShowWindow(SW_SHOW);
			//ɾ����ʱ��
			KillTimer(IDI_TIME_USER_ADD_SCORE);

			//��ȡλ��
			WORD wMeChairID=GetMeChairID();
			WORD wViewChairID=m_wViewChairID[wMeChairID];

			//���ư�ť
			UpdateScoreControl(NULL,SW_HIDE);

			//������Ϣ
			CMD_C_AddScore AddScore;
			AddScore.lScore=__max(m_lTurnMaxScore/8,1L);
			SendData(SUB_C_ADD_SCORE,&AddScore,sizeof(AddScore));

			//��������
			m_lTableScore[wMeChairID]=AddScore.lScore;
			m_GameClientView.SetUserTableScore(wViewChairID,m_lTableScore[wMeChairID]);
			PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_SCORE"));

			return;
		}
	case IDI_TIME_OPEN_CARD:
		{
			//ɾ����ʱ��
			KillTimer(IDI_TIME_OPEN_CARD);

			//̯��
			OnOpenCard(0,0);

			return;
		}
	}

	CGameFrameDlg::OnTimer(nIDEvent);
}
//////////////////////////////////////////////////////////////////////////

