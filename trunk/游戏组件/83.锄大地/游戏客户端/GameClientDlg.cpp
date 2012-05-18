#include "Stdafx.h"
#include "GameOption.h"
#include "GameClient.h"
#include "GameClientDlg.h"

//////////////////////////////////////////////////////////////////////////
//�궨��

//��Ϸ��ʱ��
#define IDI_OUT_CARD					200								//���ƶ�ʱ��
#define IDI_START_GAME					201								//��ʼ��ʱ��

//��Ϸ��ʱ��
#define IDI_MOST_CARD					300								//���ʱ��
#define IDI_DISPATCH_CARD				301								//���ƶ�ʱ��


#define TIME_OUT_CARD					30								//��ʱ��ʱ��
#define TIME_START_GAME					30								//��ʱ��ʱ��

#define MAX_TIME_OUT					3								//��ʱ����

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientDlg, CGameFrameDlg)
	ON_WM_TIMER()
	ON_MESSAGE(IDM_START,OnStart)
	ON_MESSAGE(IDM_OUT_CARD,OnOutCard)
	ON_MESSAGE(IDM_PASS_CARD,OnPassCard)
	ON_MESSAGE(IDM_OUT_PROMPT,OnOutPrompt)
	ON_MESSAGE(IDM_SORT_BY_COLOR,OnSortByColor)
	ON_MESSAGE(IDM_SORT_BY_VALUE,OnSortByValue)
	ON_MESSAGE(IDM_STRUSTEE,OnStrustee)
	ON_MESSAGE(IDM_LEFT_HIT_CARD,OnLeftHitCard)
	ON_MESSAGE(IDM_RIGHT_HIT_CARD,OnRightHitCard)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientDlg::CGameClientDlg() : CGameFrameDlg(&m_GameClientView)
{
	//��Ϸ����
	m_bOutCard3=TRUE;
	m_bHandCardCount=0;
	m_wBankerUser=INVALID_CHAIR;
	memset(m_bCardCount,0,sizeof(m_bCardCount));
	memset(m_bHandCardData,0,sizeof(m_bHandCardData));
	m_bStustee = false;
	m_wTimeOutCount = 0;

	//���ñ���
	m_dwCardHSpace=DEFAULT_PELS;

	//���Ʊ���
	m_bTurnCardCount=0;
	m_bTurnOutType=CT_ERROR;
	memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));

	//��������
	m_wTimeOutCount=0;
	m_wMostUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;

	//��������
	m_cbRemnantCardCount=0;
	m_cbDispatchCardCount=0;
	ZeroMemory(m_cbDispatchCardData,sizeof(m_cbDispatchCardData));

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
	if ((m_dwCardHSpace>MAX_PELS)||(m_dwCardHSpace<LESS_PELS)) m_dwCardHSpace=DEFAULT_PELS;

	//���ÿؼ�
	m_GameClientView.m_HandCardControl[2].SetCardSpace(m_dwCardHSpace,0,20);

	//test
	//BYTE byFirstCard[] = {
	//	0x01,0x03,0x04,0x05,0x08
	//};
	//BYTE bySecondCard[] = {
	//	0x13,0x24,0x15,0x16,0x17
	//};
	//m_GameLogic.SortCardList(byFirstCard,5);
	//m_GameLogic.SortCardList(bySecondCard,5);
	//bool bLarger = m_GameLogic.CompareCard(byFirstCard,bySecondCard,5,5);
	//end test

	return true;
}

//���ÿ��
void CGameClientDlg::ResetGameFrame()
{
	//��Ϸ����
	m_bOutCard3=TRUE;
	m_bHandCardCount=0;
	m_wBankerUser=INVALID_CHAIR;
	memset(m_bCardCount,0,sizeof(m_bCardCount));
	memset(m_bHandCardData,0,sizeof(m_bHandCardData));
	m_bStustee = false;
	m_wTimeOutCount = 0;

	//���Ʊ���
	m_bTurnCardCount=0;
	m_bTurnOutType=CT_ERROR;
	memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));

	//��������
	m_wTimeOutCount=0;
	m_wMostUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;

	//ɾ����ʱ
	KillGameTimer(0);
	KillTimer(IDI_MOST_CARD);
	KillTimer(IDI_DISPATCH_CARD);

	//��������
	m_cbRemnantCardCount=0;
	m_cbDispatchCardCount=0;
	ZeroMemory(m_cbDispatchCardData,sizeof(m_cbDispatchCardData));

	return;
}

//��Ϸ����
void CGameClientDlg::OnGameOptionSet()
{
	//��������
	CGameOption GameOption;
	GameOption.m_dwCardHSpace=m_dwCardHSpace;
	GameOption.m_bEnableSound=IsEnableSound();
	GameOption.m_bAllowLookon = IsAllowUserLookon();

	//��������
	if (GameOption.DoModal()==IDOK)
	{
		//��ȡ����
		m_dwCardHSpace=GameOption.m_dwCardHSpace;

		//���ÿؼ�
		EnableSound(GameOption.m_bEnableSound);
		AllowUserLookon(0,GameOption.m_bAllowLookon);
		m_GameClientView.m_HandCardControl[2].SetCardSpace(m_dwCardHSpace,0,20);

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
	case IDI_OUT_CARD:			//�û�����
		{
			//��ʱ�ж�
			if ((nElapse==0)||((m_bStustee==true)&&(nElapse<(TIME_OUT_CARD))))
			{
				if ((IsLookonMode()==false)&&(wChairID==GetMeChairID())) 
				{
					if( !m_bStustee && ++m_wTimeOutCount>=MAX_TIME_OUT)
					{
						m_wTimeOutCount = 0;
						OnStrustee((WPARAM)TRUE,0);
						//��ʾ��Ϣ
						InsertSystemString(TEXT("��������γ�ʱ���л�Ϊ��ϵͳ�йܡ�ģʽ"));
					}
					AutomatismOutCard();
				}
				return true;
			}

			//��������
			if ((nElapse<=3)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false)) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));

			return true;
		}
	case IDI_START_GAME:		//��ʼ��Ϸ
		{
			if ((nElapse==0)||((m_bStustee==true)&&(nElapse<(TIME_START_GAME))))
			{
				if ((IsLookonMode()==false)&&(wChairID==GetMeChairID())) 
				{
					if( m_bStustee )
						OnStart(0,0);
					else 
						PostMessage(WM_CLOSE);
				}
				return true;
			}
			if ((nElapse<=3)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false)) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));

			return true;
		}
	}

	return false;
}

//�Թ�״̬
void CGameClientDlg::OnLookonChanged(bool bLookonUser, const void * pBuffer, WORD wDataSize)
{
	//�Թ�����
	m_GameClientView.m_HandCardControl[2].SetDisplayFlag((IsLookonMode()==false)||(IsAllowLookon()==true));

	return;
}

//������Ϣ
bool CGameClientDlg::OnGameMessage(WORD wSubCmdID, const void * pBuffer, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_GAME_START:		//��Ϸ��ʼ
		{
			return OnSubGameStart(pBuffer,wDataSize);
		}
	case SUB_S_OUT_CARD:		//�û�����
		{
			return OnSubOutCard(pBuffer,wDataSize);
		}
	case SUB_S_PASS_CARD:		//��������
		{
			return OnSubPassCard(pBuffer,wDataSize);
		}
	case SUB_S_GAME_END:		//��Ϸ����
		{
			return OnSubGameEnd(pBuffer,wDataSize);
		}
	case SUB_S_STRUSTEE:		//�û��й�
		{
			return OnSubStrustee(pBuffer,wDataSize);
		}
	}

	return true;
}

//��Ϸ����
bool CGameClientDlg::OnGameSceneMessage(BYTE cbGameStation, bool bLookonOther, const void * pBuffer, WORD wDataSize)
{
	switch (cbGameStation)
	{
	case GS_RF_FREE:	//����״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pBuffer;

			//���ÿؼ�
			if (IsLookonMode()==false)
			{
				m_GameClientView.m_btStart.ShowWindow(TRUE);
				m_GameClientView.m_btStart.SetFocus();
			}
			//�����˿�
			if (IsLookonMode()==false) m_GameClientView.m_HandCardControl[2].SetDisplayFlag(true);

			//�����й�״̬
			for( WORD j = 0; j < GAME_PLAYER; j++ )
				m_GameClientView.SetStrustee(SwitchViewChairID(j),pStatusFree->bStrustee[j]);
			
			//����ʱ��
			SetGameTimer(GetMeChairID(),IDI_START_GAME,TIME_START_GAME);

			return true;
		}
	case GS_RF_PLAYING:		//��Ϸ״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pBuffer;

			//���ñ���
			m_wBankerUser=pStatusPlay->wBankerUser;
			m_bTurnCardCount=pStatusPlay->bTurnCardCount;
			m_bHandCardCount=pStatusPlay->bCardCount[GetMeChairID()];
			m_bTurnOutType=m_GameLogic.GetCardType(pStatusPlay->bTurnCardData,m_bTurnCardCount);

			CopyMemory(m_bHandCardData,pStatusPlay->bCardData,sizeof(BYTE)*m_bHandCardCount);

			CopyMemory(m_bTurnCardData,pStatusPlay->bTurnCardData,sizeof(BYTE)*pStatusPlay->bTurnCardCount);
			CopyMemory(m_bCardCount,pStatusPlay->bCardCount,sizeof(m_bCardCount));

			//���ý���
			for (BYTE i=0;i<GAME_PLAYER;i++)
			{
				WORD wViewChairID=SwitchViewChairID(i);
				m_bCardCount[i]=pStatusPlay->bCardCount[i];
				m_GameClientView.SetCardCount(wViewChairID,pStatusPlay->bCardCount[i]);
				m_GameClientView.m_lAllTurnScore[wViewChairID]=pStatusPlay->lAllTurnScore[i];
				m_GameClientView.m_lLastTurnScore[wViewChairID]=pStatusPlay->lLastTurnScore[i];
			}

			//�����˿�
			BYTE cbCardCount[MAX_COUNT];
			ZeroMemory(cbCardCount,sizeof(cbCardCount));

			//�����˿�
			for (BYTE i=0;i<GAME_PLAYER;i++)
			{
				WORD wViewChairID=SwitchViewChairID(i);
				if (wViewChairID!=2)
				{
					m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(cbCardCount,pStatusPlay->bCardCount[i]);
				}
				else m_GameClientView.m_HandCardControl[2].SetCardData(m_bHandCardData,m_bHandCardCount);
			}

			//�������
			if ((IsLookonMode()==false)&&(pStatusPlay->wCurrentUser==GetMeChairID()))
			{
				m_GameClientView.m_btOutCard.EnableWindow(FALSE);
				m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
				m_GameClientView.m_btOutPrompt.ShowWindow(SW_SHOW);
				m_GameClientView.m_btPassCard.ShowWindow(SW_SHOW);
				m_GameClientView.m_btPassCard.EnableWindow((m_bTurnCardCount!=0)?TRUE:FALSE);
			}

			//��������
			if (m_bTurnCardCount!=0)
			{
				WORD wViewChairID=SwitchViewChairID(pStatusPlay->wLastOutUser);
				m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(m_bTurnCardData,m_bTurnCardCount);
			}

			//�����й�״̬
			for( WORD j = 0; j < GAME_PLAYER; j++ )
				m_GameClientView.SetStrustee(SwitchViewChairID(j),pStatusPlay->bStrustee[j]);
			if( !IsLookonMode() && pStatusPlay->bStrustee[GetMeChairID()] )
			{
				m_bStustee = true;
				OnStrustee(0,0);
			}

			//���ö�ʱ��
			SetGameTimer(pStatusPlay->wCurrentUser,IDI_OUT_CARD,TIME_OUT_CARD);

			//�����˿�
			if (IsLookonMode()==false) 
			{
				m_GameClientView.m_HandCardControl[2].SetPositively(true);
				m_GameClientView.m_HandCardControl[2].SetDisplayFlag(true);
			}

			return true;
		}
	}

	return true;
}

//��Ϸ��ʼ
bool CGameClientDlg::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;

	//��Ϣ����
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

	//���ñ���
	m_bTurnCardCount=0;
	m_bTurnOutType=CT_ERROR;
	m_bOutCard3=pGameStart->cbOutCard3;
	m_wBankerUser=pGameStart->wBankerUser;
	m_wCurrentUser=pGameStart->wCurrentUser;
	memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));
	memset(m_bCardCount,13,sizeof(m_bCardCount) );

	//�����˿�
	m_bHandCardCount=13;

	CopyMemory(m_bHandCardData,pGameStart->cbCardData,sizeof(BYTE)*m_bHandCardCount);

	//���ý���
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.SetPassFlag(i,false);
		m_GameClientView.m_UserCardControl[i].SetCardData(NULL,0);
	}
	m_GameClientView.m_ScoreView.ShowWindow(SW_HIDE);

	//�˿���Ŀ
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.SetCardCount(i,13);

		if (i!=2) m_GameClientView.m_HandCardControl[i].SetDisplayFlag(false);
	}

	//�����˿�
	DispatchUserCard(m_bHandCardData,m_bHandCardCount);
	m_GameLogic.SortCardList(m_bHandCardData,m_bHandCardCount);


	//��������
	bool bPlayerMode=(IsLookonMode()==false);
	m_GameClientView.m_HandCardControl[2].SetPositively(IsLookonMode()==false);
	m_GameClientView.m_HandCardControl[2].SetDisplayFlag((IsLookonMode()==false)||(IsAllowLookon()==true));

	return true;
}

//�û�����
bool CGameClientDlg::OnSubOutCard(const void * pBuffer, WORD wDataSize)
{
	//��������
	CMD_S_OutCard * pOutCard=(CMD_S_OutCard *)pBuffer;
	WORD wHeadSize=sizeof(CMD_S_OutCard)-sizeof(pOutCard->bCardData);

	//Ч������
	ASSERT(wDataSize>=wHeadSize);
	if (wDataSize<wHeadSize) return false;
	ASSERT(wDataSize==(wHeadSize+pOutCard->bCardCount*sizeof(pOutCard->bCardData[0])));
	if (wDataSize!=(wHeadSize+pOutCard->bCardCount*sizeof(pOutCard->bCardData[0]))) return false;

	//ֹͣ����
	DoncludeDispatchCard();

	//ɾ����ʱ��
	KillTimer(IDI_MOST_CARD);
	KillGameTimer(IDI_OUT_CARD);

	//��������
	WORD wOutViewChairID=SwitchViewChairID(pOutCard->wOutCardUser);
	m_bCardCount[pOutCard->wOutCardUser]-=pOutCard->bCardCount;
	m_GameClientView.SetCardCount(wOutViewChairID,m_bCardCount[pOutCard->wOutCardUser]);

	//��������
	if ((IsLookonMode()==true)||(GetMeChairID()!=pOutCard->wOutCardUser))
	{
		m_GameClientView.m_UserCardControl[wOutViewChairID].SetCardData(pOutCard->bCardData,pOutCard->bCardCount);
	}

	//��������
	if (m_bTurnCardCount==0)
	{
		for (WORD i=0;i<GAME_PLAYER;i++) 
		{
			if (i==pOutCard->wOutCardUser) continue;
			m_GameClientView.m_UserCardControl[SwitchViewChairID(i)].SetCardData(NULL,0);
		}
		m_GameClientView.SetPassFlag(INVALID_CHAIR,false);
	}

	//��¼����
	m_bTurnCardCount=pOutCard->bCardCount;
	m_bTurnOutType=m_GameLogic.GetCardType(pOutCard->bCardData,pOutCard->bCardCount);
	CopyMemory(m_bTurnCardData,pOutCard->bCardData,sizeof(BYTE)*pOutCard->bCardCount);

	//�Լ��˿�
	if ((IsLookonMode()==true)&&(pOutCard->wOutCardUser==GetMeChairID()))
	{
		//ɾ���˿� 
		BYTE bSourceCount=m_bHandCardCount;
		m_bHandCardCount-=pOutCard->bCardCount;
		m_GameLogic.RemoveCard(pOutCard->bCardData,pOutCard->bCardCount,m_bHandCardData,bSourceCount);

		//���ý���
		m_GameClientView.m_HandCardControl[2].SetCardData(m_bHandCardData,m_bHandCardCount);
	}

	//�����˿�
	if (pOutCard->wOutCardUser!=GetMeChairID())
	{
		BYTE cbCardData[MAX_COUNT];
		ZeroMemory(cbCardData,sizeof(cbCardData));
		m_GameClientView.m_HandCardControl[wOutViewChairID].SetCardData(cbCardData,m_bCardCount[pOutCard->wOutCardUser]);
	}

	//����ж�
	if (pOutCard->wCurrentUser==pOutCard->wOutCardUser)
	{
		//���ñ���
		m_bTurnCardCount=0;
		m_bTurnOutType=CT_ERROR;
		m_wMostUser=pOutCard->wCurrentUser;
		memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));

		//���ý���
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			if (i!=pOutCard->wOutCardUser)
			{
				WORD wViewChairID=SwitchViewChairID(i);
				m_GameClientView.SetPassFlag(wViewChairID,true);
				m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(NULL,0);
			}
		}

		//��������
		SetTimer(IDI_MOST_CARD,3000,NULL);
		PlayGameSound(AfxGetInstanceHandle(),TEXT("MOST_CARD"));

		return true;
	}

	//��������
	if ((IsLookonMode()==true)||(GetMeChairID()!=pOutCard->wOutCardUser)) PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));

	//�������
	if (pOutCard->wCurrentUser!=INVALID_CHAIR)
	{
		WORD wViewChairID=SwitchViewChairID(pOutCard->wCurrentUser);
		m_GameClientView.SetPassFlag(wViewChairID,false);
		m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(NULL,0);
	}

	//�������
	if ((IsLookonMode()==false)&&(pOutCard->wCurrentUser==GetMeChairID()))
	{
		ActiveGameFrame();
		m_GameClientView.m_btPassCard.EnableWindow(TRUE);
		m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btPassCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btOutPrompt.ShowWindow(SW_SHOW);
		m_GameClientView.m_btOutCard.EnableWindow((VerdictOutCard()==true)?TRUE:FALSE);
		m_GameClientView.m_btPassCard.EnableWindow((VerdictPassCard()==true)?TRUE:FALSE);
		//�Զ�PASS
		if( m_bTurnCardCount > m_bHandCardCount )
			OnPassCard(0,0);
	}

	//���ö�ʱ��
	if (pOutCard->wCurrentUser!=INVALID_CHAIR)
	{
		if (m_bCardCount[pOutCard->wCurrentUser] <m_bTurnCardCount)
		{
			SetGameTimer(pOutCard->wCurrentUser,IDI_OUT_CARD,3);
		}
		else
		{
			SetGameTimer(pOutCard->wCurrentUser,IDI_OUT_CARD,TIME_OUT_CARD);
		}
	}

	return true;
}

//��������
bool CGameClientDlg::OnSubPassCard(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_PassCard)) return false;
	CMD_S_PassCard * pPassCard=(CMD_S_PassCard *)pBuffer;

	//ɾ����ʱ��
	KillGameTimer(IDI_OUT_CARD);

	//�������
	if ((IsLookonMode()==true)||(pPassCard->wPassUser!=GetMeChairID()))
	{
		WORD wViewChairID=SwitchViewChairID(pPassCard->wPassUser);
		m_GameClientView.SetPassFlag(wViewChairID,true);
		m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(NULL,0);
	}

	//һ���ж�
	if (pPassCard->bNewTurn==TRUE)
	{
		m_bTurnCardCount=0;
		m_bTurnOutType=CT_ERROR;
		memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));
	}

	//���ý���
	WORD wViewChairID=SwitchViewChairID(pPassCard->wCurrentUser);
	m_GameClientView.SetPassFlag(wViewChairID,false);
	m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(NULL,0);

	//�������
	if ((IsLookonMode()==false)&&(pPassCard->wCurrentUser==GetMeChairID()))
	{
		ActiveGameFrame();
		m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btPassCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btOutPrompt.ShowWindow(SW_SHOW);
		m_GameClientView.m_btOutCard.EnableWindow((VerdictOutCard()==true)?TRUE:FALSE);
		m_GameClientView.m_btPassCard.EnableWindow((VerdictPassCard()==true)?TRUE:FALSE);
	}

	//��������
	if ((IsLookonMode()==true)||(pPassCard->wPassUser!=GetMeChairID()))	PlayGameSound(AfxGetInstanceHandle(),TEXT("PASS_CARD"));

	//����ʱ��
	if (m_bCardCount[pPassCard->wCurrentUser]<m_bTurnCardCount)
	{
		SetGameTimer(pPassCard->wCurrentUser,IDI_OUT_CARD,3);
	}
	else
	{
		SetGameTimer(pPassCard->wCurrentUser,IDI_OUT_CARD,TIME_OUT_CARD);
	}

	return true;
}

//��Ϸ����
bool CGameClientDlg::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;

	//��Ϣ����
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;

	//����״̬
	SetGameStatus(GS_RF_FREE);

	//ֹͣ����
	DoncludeDispatchCard();

	//ɾ����ʱ��
	KillTimer(IDI_MOST_CARD);
	KillGameTimer(IDI_OUT_CARD);
	KillTimer(IDI_DISPATCH_CARD);

	//���ؿؼ�
	m_GameClientView.m_btOutCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btPassCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btOutPrompt.ShowWindow(SW_HIDE);

	//���ÿؼ�
	m_GameClientView.m_btOutCard.EnableWindow(FALSE);
	m_GameClientView.m_btPassCard.EnableWindow(FALSE);

	//���û���
	TCHAR szBuffer[128] = TEXT("");
	InsertGeneralString(TEXT("���ֽ���,�ɼ�ͳ��:"),RGB(255,0,255),true);
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		const tagUserData * pUserData=GetUserData(i);
		m_GameClientView.m_ScoreView.SetGameScore(i,pUserData->szName,pGameEnd->lGameScore[i]);
		m_GameClientView.m_lAllTurnScore[SwitchViewChairID(i)]+=pGameEnd->lGameScore[i];
		m_GameClientView.m_lLastTurnScore[SwitchViewChairID(i)]=pGameEnd->lGameScore[i];

		//���������ʾ�ɼ�
		_snprintf(szBuffer,CountArray(szBuffer),TEXT(" %s: %+ld��"),pUserData->szName,pGameEnd->lGameScore[i]);
		InsertGeneralString(szBuffer,RGB(255,0,255),true);
	}
	m_GameClientView.m_ScoreView.ShowWindow(SW_SHOW);

	//��ʾ�˿�
	if (IsLookonMode()==true) m_GameClientView.m_HandCardControl[2].SetDisplayFlag(true);

	//�����й�
	if( m_bStustee )
		OnStrustee(0,0);

	//��������
	WORD wMeChairID=GetMeChairID();
	DOUBLE lMeScore=pGameEnd->lGameScore[GetMeChairID()];
	if (lMeScore>0L) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WIN"));
	else if (lMeScore<0L) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_LOST"));
	else PlayGameSound(GetModuleHandle(NULL),TEXT("GAME_END"));

	//���ý���
	if (IsLookonMode()==false)
	{
		m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
		SetGameTimer(GetMeChairID(),IDI_START_GAME,30);
	}

	return true;
}

//�����ж�
bool CGameClientDlg::VerdictOutCard()
{
	//״̬�ж�
	if (m_GameClientView.m_btOutCard.IsWindowVisible()==FALSE) return false;

	//��ȡ�˿�
	BYTE bCardData[13];
	BYTE bShootCount=(BYTE)m_GameClientView.m_HandCardControl[2].GetShootCard(bCardData,CountArray(bCardData));

	//ǿ�Ƴ���
	WORD wNextUser=(GetMeChairID()+1)%GAME_PLAYER;
	if ((m_bCardCount[wNextUser]==1)&&(bShootCount==1))
	{
		BYTE cbTempCard[MAX_COUNT];
		CopyMemory(cbTempCard,m_bHandCardData,m_bHandCardCount*sizeof(BYTE));
		m_GameLogic.SortCardList(cbTempCard,m_bHandCardCount);
		if (bCardData[0]!=cbTempCard[0]) return false;
	}

	//�����ж�
	if (bShootCount>0L)
	{
		//�����˿�
		m_GameLogic.SortCardList(bCardData,bShootCount);

		//��������
		BYTE bCardType=m_GameLogic.GetCardType(bCardData,bShootCount);

		//�����ж�
		if (bCardType==CT_ERROR) return false;

		//�׳��ж�
		if ((m_bOutCard3==TRUE)&&(m_wBankerUser==GetMeChairID())&&(m_bCardCount[m_wBankerUser]==13))
		{
			//�����ж�
			for (BYTE i=0;i<bShootCount;i++)
			{
				if (bCardData[i]==0x03) break;
			}
			if (i==bShootCount) return false;
		}

		//�����ж�
		if (m_bTurnCardCount==0) return true;
		return m_GameLogic.CompareCard(m_bTurnCardData,bCardData,m_bTurnCardCount,bShootCount);
	}

	return false;
}

//�����ж�
bool CGameClientDlg::VerdictPassCard()
{
	//�׳��ж�
	if (m_bTurnCardCount==0) return false;

	//�����ж�
	WORD wNextUser=(GetMeChairID()+1)%GAME_PLAYER;
	if ((m_bCardCount[wNextUser]==1)&&(m_bTurnCardCount==1))
	{
		if (m_GameLogic.CompareCard(m_bTurnCardData[0],m_bHandCardData[0])==true) return false;
	}

	//ʤ���ж�
	if ((m_bTurnCardCount==1)&&(m_bHandCardCount==1))
	{
		if (m_GameLogic.CompareCard(m_bTurnCardData[0],m_bHandCardData[0])==true) return false;
	}

	return true;
}

//�Զ�����
bool CGameClientDlg::AutomatismOutCard()
{
	//���ƽ���
	KillGameTimer(IDI_OUT_CARD);
	//�������
	WORD wMeChairId = GetMeChairID();
	//��������
	tagOutCardResult OutCardResult;
	if( m_GameLogic.StrusteeOutCard(m_bHandCardData,m_bHandCardCount,m_bTurnCardData,m_bTurnCardCount,
		wMeChairId==m_wBankerUser&&m_bHandCardCount==13,m_bCardCount[(wMeChairId+1)%GAME_PLAYER]==1,OutCardResult) )
	{
		//�����˿�
		m_GameClientView.m_HandCardControl[2].SetShootCard(OutCardResult.cbResultCard,OutCardResult.cbCardCount);
		//Ч��
		ASSERT( VerdictOutCard() );
		m_GameClientView.m_btOutCard.EnableWindow(VerdictOutCard()?TRUE:FALSE);
		OnOutCard(0,0);
	}
	else
	{
		ASSERT( VerdictPassCard() );
		OnPassCard(0,0);
	}

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

		//�˿�����
		BYTE cbUserCard[MAX_COUNT];
		ZeroMemory(cbUserCard,sizeof(cbUserCard));

		//���ý���
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			if (i!=2)
			{
				m_GameClientView.m_HandCardControl[i].SetCardData(cbUserCard,MAX_COUNT);
			}
			else m_GameClientView.m_HandCardControl[i].SetCardData(m_bHandCardData,m_bHandCardCount); 
		}

		//���ñ���
		m_cbRemnantCardCount=0;
		m_cbDispatchCardCount=0;
		ZeroMemory(m_cbDispatchCardData,sizeof(m_cbDispatchCardData));
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
	SetTimer(IDI_DISPATCH_CARD,200,NULL);

	return true;
}

//��ʱ����Ϣ
void CGameClientDlg::OnTimer(UINT nIDEvent)
{
	if ((nIDEvent==IDI_MOST_CARD)&&(m_wMostUser!=INVALID_CHAIR))
	{
		//��������
		WORD wCurrentUser=m_wMostUser;
		m_wMostUser=INVALID_CHAIR;

		//ɾ����ʱ��
		KillTimer(IDI_MOST_CARD);

		//���ý���
		m_GameClientView.SetPassFlag(INVALID_CHAIR,false);
		for (WORD i=0;i<GAME_PLAYER;i++) m_GameClientView.m_UserCardControl[i].SetCardData(NULL,0);

		//�������
		if ((IsLookonMode()==false)&&(wCurrentUser==GetMeChairID()))
		{
			ActiveGameFrame();
			m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
			m_GameClientView.m_btPassCard.ShowWindow(SW_SHOW);
			m_GameClientView.m_btOutPrompt.ShowWindow(SW_SHOW);
			m_GameClientView.m_btPassCard.EnableWindow(FALSE);
			m_GameClientView.m_btOutCard.EnableWindow((VerdictOutCard()==true)?TRUE:FALSE);
			m_GameClientView.m_btPassCard.EnableWindow((VerdictPassCard()==true)?TRUE:FALSE);
		}

		//����ʱ��
		SetGameTimer(wCurrentUser,IDI_OUT_CARD,TIME_OUT_CARD);

		return;
	}

	//�ɷ��˿�
	if (nIDEvent==IDI_DISPATCH_CARD)
	{
		//�ɷ�����
		if (m_cbRemnantCardCount>0)
		{
			//�˿�����
			BYTE cbUserCard[MAX_COUNT];
			ZeroMemory(cbUserCard,sizeof(cbUserCard));

			//�ɷ��˿�
			m_cbRemnantCardCount--;
			m_cbDispatchCardCount++;
			m_GameLogic.SortCardList(m_cbDispatchCardData,m_cbDispatchCardCount);

			//���ý���
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				if (i!=2)
				{
					m_GameClientView.m_HandCardControl[i].SetCardData(cbUserCard,m_cbDispatchCardCount);
				}
				else m_GameClientView.m_HandCardControl[i].SetCardData(m_cbDispatchCardData,m_cbDispatchCardCount);
				//����������
				m_GameClientView.SetCardCount(SwitchViewChairID(i),m_cbDispatchCardCount);
			}

			//��������
			PlayGameSound(AfxGetInstanceHandle(),TEXT("SEND_CARD"));
		}

		//��ֹ�ж�
		if (m_cbRemnantCardCount==0)
		{
			//ɾ����ʱ��
			KillTimer(IDI_DISPATCH_CARD);

			//�˿���Ŀ
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				m_GameClientView.SetCardCount(SwitchViewChairID(i),m_bCardCount[i]);
			}

			//��ǰ���
			if ((IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID()))
			{
				ActiveGameFrame();
				m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
				m_GameClientView.m_btOutCard.EnableWindow(FALSE);
				m_GameClientView.m_btPassCard.EnableWindow(FALSE);
				m_GameClientView.m_btPassCard.ShowWindow(SW_SHOW);
				m_GameClientView.m_btOutPrompt.ShowWindow(SW_SHOW);
			}

			//����ʣ��
			PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));

			//����ʱ��
			if (m_wCurrentUser!=INVALID_CHAIR) SetGameTimer(m_wCurrentUser,IDI_OUT_CARD,TIME_OUT_CARD);
		}

		return;
	}

	__super::OnTimer(nIDEvent);
}

//��ʼ��ť
LRESULT CGameClientDlg::OnStart(WPARAM wParam, LPARAM lParam)
{
	//���ñ���
	m_wTimeOutCount=0;
	m_bHandCardCount=0;
	m_bTurnCardCount=0;
	m_bTurnOutType=CT_ERROR;
	m_wMostUser=INVALID_CHAIR;
	memset(m_bHandCardData,0,sizeof(m_bHandCardData));
	memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));

	//���ý���
	KillGameTimer(IDI_START_GAME);
	m_GameClientView.SetCardCount(INVALID_CHAIR,0);
	m_GameClientView.SetPassFlag(INVALID_CHAIR,false);

	//���ؿؼ�
	m_GameClientView.m_btStart.ShowWindow(FALSE);
	m_GameClientView.m_ScoreView.ShowWindow(SW_HIDE);

	//�����˿�
	for (WORD i=0;i<GAME_PLAYER;i++) 
	{
		m_GameClientView.m_UserCardControl[i].SetCardData(NULL,0);
		m_GameClientView.m_HandCardControl[i].SetCardData(NULL,0);
	}
	m_GameClientView.m_HandCardControl[i].SetPositively(false);

	//������Ϣ
	SendUserReady(NULL,0);

	return 0;
}

//������Ϣ
LRESULT CGameClientDlg::OnOutCard(WPARAM wParam, LPARAM lParam)
{
	//״̬�ж�
	if (m_GameClientView.m_btOutCard.IsWindowVisible()==FALSE) return 0;
	if (m_GameClientView.m_btOutCard.IsWindowEnabled()==FALSE) return 0;

	//���ý���
	KillGameTimer(IDI_OUT_CARD);
	m_GameClientView.m_btOutCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btOutCard.EnableWindow(FALSE);
	m_GameClientView.m_btPassCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btPassCard.EnableWindow(FALSE);
	m_GameClientView.m_btOutPrompt.ShowWindow(SW_HIDE);
	
	//��������
	CMD_C_OutCard OutCard;
	OutCard.bCardCount=(BYTE)m_GameClientView.m_HandCardControl[2].GetShootCard(OutCard.bCardData,CountArray(OutCard.bCardData));
	m_GameLogic.SortCardList(OutCard.bCardData,OutCard.bCardCount);
	SendData(SUB_C_OUT_CART,&OutCard,sizeof(OutCard)-sizeof(OutCard.bCardData)+OutCard.bCardCount*sizeof(BYTE));

	//Ԥ����ʾ
	PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));
	m_GameClientView.m_UserCardControl[2].SetCardData(OutCard.bCardData,OutCard.bCardCount);

	//Ԥ��ɾ��
	BYTE bSourceCount=m_bHandCardCount;
	m_bHandCardCount-=OutCard.bCardCount;
	m_GameLogic.RemoveCard(OutCard.bCardData,OutCard.bCardCount,m_bHandCardData,bSourceCount);
	m_GameClientView.m_HandCardControl[2].SetCardData(m_bHandCardData,m_bHandCardCount);

	return 0;
}

//��������
LRESULT CGameClientDlg::OnPassCard(WPARAM wParam, LPARAM lParam)
{
	//״̬�ж�
	if (m_GameClientView.m_btPassCard.IsWindowVisible()==FALSE) return 0;
	if (m_GameClientView.m_btPassCard.IsWindowEnabled()==FALSE) return 0;

	//���ý���
	KillGameTimer(IDI_OUT_CARD);
	m_GameClientView.m_btOutCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btOutCard.EnableWindow(FALSE);
	m_GameClientView.m_btPassCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btPassCard.EnableWindow(FALSE);
	m_GameClientView.m_btOutPrompt.ShowWindow(SW_HIDE);

	//��������
	SendData(SUB_C_PASS_CARD);

	//Ԥ����ʾ
	m_GameClientView.m_UserCardControl[2].SetCardData(NULL,0);
	m_GameClientView.SetPassFlag(2,true);
	PlayGameSound(AfxGetInstanceHandle(),TEXT("PASS_CARD"));
	m_GameClientView.m_HandCardControl[2].SetCardData(m_bHandCardData,m_bHandCardCount);

	return 0;
}

//������ʾ
LRESULT CGameClientDlg::OnOutPrompt(WPARAM wParam, LPARAM lParam)
{
	//�������
	WORD wMeChairId = GetMeChairID();
	//��������
	tagOutCardResult OutCardResult;
	if( m_GameLogic.StrusteeOutCard(m_bHandCardData,m_bHandCardCount,m_bTurnCardData,m_bTurnCardCount,
		wMeChairId==m_wBankerUser&&m_bHandCardCount==13,m_bCardCount[(wMeChairId+1)%GAME_PLAYER]==1,OutCardResult) )
	{
		//�����˿�
		m_GameClientView.m_HandCardControl[2].SetShootCard(OutCardResult.cbResultCard,OutCardResult.cbCardCount);
		//Ч��
		ASSERT( VerdictOutCard() );
		m_GameClientView.m_btOutCard.EnableWindow(VerdictOutCard()?TRUE:FALSE);
	}
	else
	{
		ASSERT( VerdictPassCard() );
		OnPassCard(0,0);
	}
	return 0;
}

//��ɫ����
LRESULT CGameClientDlg::OnSortByColor(WPARAM wParam, LPARAM lParam)
{
	if (m_bHandCardCount>0)
	{
		m_GameLogic.SortCardList(m_bHandCardData,m_bHandCardCount,ST_COLOR);
		m_GameClientView.m_HandCardControl[2].SetCardData(m_bHandCardData,m_bHandCardCount);
	}

	return 0;
}

//��С����
LRESULT CGameClientDlg::OnSortByValue(WPARAM wParam, LPARAM lParam)
{
	if (m_bHandCardCount>0)
	{
		m_GameLogic.SortCardList(m_bHandCardData,m_bHandCardCount,ST_ORDER);
		m_GameClientView.m_HandCardControl[2].SetCardData(m_bHandCardData,m_bHandCardCount);
	}

	return 0;
}

//�й�
LRESULT CGameClientDlg::OnStrustee(WPARAM wParam, LPARAM lParam)
{
	//����״̬
	if (m_bStustee==true)
	{
		m_bStustee=false;
		m_GameClientView.m_btStrustee.SetButtonImage(IDB_START_TRUSTEE,AfxGetInstanceHandle(),false);
	}
	else
	{
		m_bStustee=true;
		m_GameClientView.m_btStrustee.SetButtonImage(IDB_STOP_TRUSTEE,AfxGetInstanceHandle(),false);
	}
	//��ʾ��Ϣ
	if( FALSE == (BOOL)wParam )
	{
		TCHAR szBuffer[32];
		_snprintf(szBuffer,CountArray(szBuffer),TEXT("��%s���йܹ���."),m_bStustee?TEXT("ѡ��"):TEXT("ȡ��"));
		InsertSystemString(szBuffer);
	}
	//��������
	CMD_C_Strustee Strustee;
	Strustee.bStrustee = m_bStustee;
	SendData(SUB_C_STRUSTEE,&Strustee,sizeof(Strustee));

	return 0;
}

//�Ҽ��˿�
LRESULT CGameClientDlg::OnLeftHitCard(WPARAM wParam, LPARAM lParam)
{
#ifndef	TEST
	//���ÿؼ�
	bool bOutCard=VerdictOutCard();
	m_GameClientView.m_btOutCard.EnableWindow(bOutCard?TRUE:FALSE);
#endif

	return 0;
}

//����˿�
LRESULT CGameClientDlg::OnRightHitCard(WPARAM wParam, LPARAM lParam)
{
	//�û�����
	OnOutCard(0,0);

	return 0;
}

//�û��й�
bool CGameClientDlg::OnSubStrustee(const void *pBuffer,WORD wDataSize)
{
	//��������
	CMD_S_Strustee *pStrustee = (CMD_S_Strustee *)pBuffer;

	//Ч������
	ASSERT( wDataSize == sizeof(CMD_S_Strustee) );
	if( wDataSize != sizeof(CMD_S_Strustee) ) return false;

	//�����й�
	m_GameClientView.SetStrustee(SwitchViewChairID(pStrustee->wChairId),pStrustee->bStrustee);

	//��ʾ��Ϣ
	if( IsLookonMode() || GetMeChairID() != pStrustee->wChairId )
	{
		const tagUserData *pUserData = GetUserData(pStrustee->wChairId);
		if( pUserData == NULL ) return true;
		TCHAR szBuffer[128] = TEXT("");
		_snprintf(szBuffer,CountArray(szBuffer),TEXT("���[ %s ]%s���йܹ���"),pUserData->szName,pStrustee->bStrustee?TEXT("ѡ��"):TEXT("ȡ��"));
		InsertSystemString(szBuffer);
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////

