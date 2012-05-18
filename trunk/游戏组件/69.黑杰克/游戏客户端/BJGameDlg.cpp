#include "Stdafx.h"
#include "BJ.h"
#include "BJGameDlg.h"
#include "BJGameView.h"
#include "..\��Ϣ����\CMD_BlackJack.h"


//////////////////////////////////////////////////////////////////////////

//��ʱ����ʶ
#define IDI_STRAT_GAME					200								//������Ϸ��ʱ��
#define IDI_GIVE_UP						201								//������ʱ��

#define IDI_ADD_GOLD					202								//��ע��ʱ��
#define IDI_GET_CARD					203								//Ҫ�ƶ�ʱ��

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CBJGameDlg, CGameFrameDlg)
	ON_MESSAGE(IDM_GAME_START,OnStart)
	ON_MESSAGE(IDM_ADD_GOLD,OnAddGold)
	ON_MESSAGE(IDM_GIVE_UP,OnGiveUp)
	ON_MESSAGE(IDM_GET_CARD,OnGetCard)
	ON_MESSAGE(IDM_STOP_CARD,OnStopCard)
	ON_MESSAGE(IDM_DOUBLE_CARD,OnDoubleCard)	
	ON_MESSAGE(IDM_CUT_CARD,OnCutCard)
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////

//���캯��
CBJGameDlg::CBJGameDlg() : CGameFrameDlg(&m_GameView)
{	
	//��������
	m_lGoldShow    = 0L;	
	m_lTurnMaxGold = 0L;
	m_bMeAddGold   = false;
	m_wCardSum     = 0;
	m_bCutCard     = false;
	memset(m_szName, 0, sizeof(m_szName));
	return;
}

//��������
CBJGameDlg::~CBJGameDlg()
{
}

//��ʼ����
bool CBJGameDlg::InitGameFrame()
{
	//���ñ���
	SetWindowText(TEXT("21��-�ڽܿ���Ϸ"));

	//����ͼ��
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);

#ifdef	DEBUG_GAME
	m_GameView.m_btAddGold.ShowWindow(SW_SHOW);
	m_GameView.m_GoldControl.ShowWindow(SW_SHOW);
	for(int i=0; i<4;i++)
	{
		m_GameView.SetUserGoldInfo(i,false,1000);
	}
#endif
	return true;
}

//���ÿ��
void CBJGameDlg::ResetGameFrame()
{	
	//��������
	m_lGoldShow    = 0L;	
	m_lTurnMaxGold = 0L;
	m_bMeAddGold   = false;
	m_wCardSum     = 0;
	memset(m_szName, 0, sizeof(m_szName));
	memset(m_GameView.m_cbPlayStatus,0,sizeof(m_GameView.m_cbPlayStatus));
	return;
}

//��Ϸ����
void CBJGameDlg::OnGameOptionSet()
{
	return;
}

//ʱ����Ϣ
bool CBJGameDlg::OnTimerMessage(WORD wChairID, UINT nElapse, UINT nTimerID)
{
	switch (nTimerID)
	{
	case IDI_ADD_GOLD:		    //��ע��ʱ��
		{
			if (nElapse == 0)
			{
				PlayGameSound(AfxGetInstanceHandle(),TEXT("TIME_OUT"));
				
				OnAddGold(0,0);			
				return true;
			}
			if (nElapse <= 10) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));

			return true;
		}
	case IDI_GET_CARD:			//Ҫ�ƶ�ʱ��
		{	
			if (nElapse == 0)
			{			
				PlayGameSound(AfxGetInstanceHandle(),TEXT("TIME_OUT"));
				
				OnStopCard(0,0);
				return true;
			}
			if (nElapse <= 10) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));

			return true;
		}
	case IDI_STRAT_GAME:       //������Ϸ��ʱ��
		{
			if (nElapse==0)
			{
				OnStart(0,0);
				return false;
			}
			if (nElapse<=10) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));
			return true;
		}
	}
	return false;
}

//�Թ�״̬
void CBJGameDlg::OnLookonChanged(bool bLookonUser, const void * pBuffer, WORD wDataSize)
{

}

//��Ϸ����
bool CBJGameDlg::OnGameSceneMessage(BYTE cbGameStation, bool bLookonOther, const void * pBuffer, WORD wDataSize)
{
	switch (cbGameStation)
	{
	case GS_FREE:		//����״̬
		{		
			//Ч������
			if (wDataSize != sizeof(CMD_S_StatusFree)) return false;

			CMD_S_StatusFree * pStatusFree = (CMD_S_StatusFree *)pBuffer;

			//��������
			m_bMeAddGold = false;

			//���ý���
			m_lGoldShow      = 0L;
			m_lTurnBasicGold = 0L;
			m_GameView.SetGoldTitleInfo(0, pStatusFree->dwBasicGold);

			//���ÿؼ�
			if (IsLookonMode() == false)
			{
				m_GameView.m_btBegin.ShowWindow(SW_SHOW);
				m_GameView.m_btBegin.SetFocus();
			}
			return true;
		}
	case GS_ADDGOLDING:		//��ע״̬
		{		
			//Ч������
			if (wDataSize != sizeof(CMD_S_StatusAddGold)) return false;

			CMD_S_StatusAddGold * pStatusAddGold = (CMD_S_StatusAddGold *)pBuffer;	

			for(WORD i = 0; i < GAME_PLAYER; i++)
			{
				WORD wViewChairID = SwitchViewChairID(i);			

				//��ע���
				m_GameView.SetUserGoldInfo(wViewChairID,true,pStatusAddGold->lTableGold[2 * i]);
				m_GameView.SetUserGoldInfo(wViewChairID,false,pStatusAddGold->lTableGold[2 * i + 1]);
				
				const tagUserData * pUserData = GetUserData(i);
				if (pUserData != NULL)
				{
					lstrcpyn(m_szName[i],pUserData->szName,CountArray(m_szName[i]));
					if((IsLookonMode() == false) && (GetMeChairID() != pStatusAddGold->wBankerStation))
					{
						//��ע����
						if(!pStatusAddGold->bAddGoldEnd[GetMeChairID()])
						{
							m_GameView.m_GoldControl.SetMaxGold(pStatusAddGold->lMaxGold);
							m_GameView.m_GoldControl.ShowWindow(SW_SHOW);	
							m_GameView.m_btAddGold.ShowWindow(SW_SHOW);
							m_GameView.m_btGiveUp.ShowWindow(SW_HIDE);	
						}
						
						//���ö�ʱ��
						if(GetMeChairID() == pStatusAddGold->wCurrentUser)
						{
							SetGameTimer(GetMeChairID(), IDI_ADD_GOLD, 30);
						}						
					}
					//��ʶ״̬
					if(!pStatusAddGold->bAddGoldEnd[i] && i != pStatusAddGold->wBankerStation )
					{
						m_GameView.m_cbPlayStatus[wViewChairID] = 1;
					}		
					m_GameView.m_wBankerStation = SwitchViewChairID(pStatusAddGold->wBankerStation);
				}			
			}			
			return true;
		}

	case GS_PLAYING:	//��Ϸ״̬
		{
			//Ч������
			if (wDataSize != sizeof(CMD_S_StatusPlay)) return false;

			CMD_S_StatusPlay * pStatusPlay = (CMD_S_StatusPlay *)pBuffer;

			for(WORD i = 0; i < GAME_PLAYER; i++)
			{
				//��������
				WORD wViewChairID = SwitchViewChairID(i);
				const tagUserData * pUserData = GetUserData(i);

				if (pUserData != NULL)
				{
					lstrcpyn(m_szName[i],pUserData->szName,CountArray(m_szName[i]));			
					m_GameView.m_CardControl[wViewChairID].SetCardData(pStatusPlay->cbTableCardArray[i],pStatusPlay->cbTableCardCount[i]);

					if(i == pStatusPlay->wBankerStation) continue;
					
					//��ʶ״̬			
					if(pStatusPlay->cbPlayStatus[i] == STOP_CARD)
					{
						m_GameView.m_cbPlayStatus[wViewChairID] = 3;
					}	
					else if(pStatusPlay->cbPlayStatus[i] == BRUST_CARD)
					{
						m_GameView.m_cbPlayStatus[wViewChairID] = 4;
					}
					else					
					{
						m_GameView.m_cbPlayStatus[wViewChairID] = 2;
					}	

					//ׯ�ұ�ʶ
					m_GameView.m_wBankerStation = SwitchViewChairID(pStatusPlay->wBankerStation);
				}	
				else
				{
					m_GameView.m_CardControl[wViewChairID].SetCardData(NULL,0);
				}

				//��ע���
				m_GameView.SetUserGoldInfo(wViewChairID, true,  pStatusPlay->lTableGold[2 * i]);
				m_GameView.SetUserGoldInfo(wViewChairID, false, pStatusPlay->lTableGold[2 * i + 1]);							
			}			

			//���ý���
			if((IsLookonMode() == false) && (GetMeChairID() != pStatusPlay->wBankerStation ))
			{
				if(pStatusPlay->cbPlayStatus[GetMeChairID()] == STOP_CARD || 
				   pStatusPlay->cbPlayStatus[GetMeChairID()] == BRUST_CARD)
				{
					m_GameView.m_btGetCard.ShowWindow(SW_HIDE);
					m_GameView.m_btDouble.ShowWindow(SW_HIDE);
					m_GameView.m_btStop.ShowWindow(SW_HIDE);
					KillGameTimer(IDI_GET_CARD);
				}
				else
				{
					m_GameView.m_btGetCard.ShowWindow(SW_SHOW);
					m_GameView.m_btDouble.ShowWindow(SW_SHOW);
					m_GameView.m_btStop.ShowWindow(SW_SHOW);	
					SetGameTimer(GetMeChairID(), IDI_GET_CARD, 30);
				}			
		
			}	

			//���ý���
			if((IsLookonMode() == false) && (GetMeChairID() == pStatusPlay->wBankerStation))
			{
				if(pStatusPlay->bBankerGetCard)
				{					
					m_GameView.m_btGetCard.ShowWindow(SW_SHOW);		
					m_GameView.m_btStop.ShowWindow(SW_SHOW);

					//���ö�ʱ��
					SetGameTimer(GetMeChairID(), IDI_GET_CARD, 20);
				}
				else
				{
					m_GameView.m_btGetCard.ShowWindow(SW_HIDE);		
					m_GameView.m_btStop.ShowWindow(SW_HIDE);

					//���ö�ʱ��
					KillGameTimer(IDI_GET_CARD);
				}
			}

			//���ÿؼ�
			m_GameView.m_CardControl[SwitchViewChairID(GetMeChairID())].ShowFirstCard(true);
			
			if (IsLookonMode() == false) 
			{
				ActiveGameFrame();
				m_GameView.m_CardControl[2].AllowPositively(true);
			}	

			m_GameView.UpdateGameView(NULL);

			return true;
		}		
	}
	return false;
}

//������Ϣ
bool CBJGameDlg::OnGameMessage(WORD wSubCmdID, const void * pBuffer, WORD wDataSize)
{
	switch(wSubCmdID)
	{
	case SUB_S_GAME_START:           //��Ϸ��ʼ
		{
			return OnSubGameStart(pBuffer,wDataSize);
		}
	case SUB_S_ADD_GOLD:			//��ע���
		{
			return OnSubAddGold(pBuffer,wDataSize);
		}
	case SUB_S_INIT_CARD:           //��ʼ����
		{
			return OnSubInitCard(pBuffer,wDataSize);
		}
	case SUB_S_CUT_CARD:			//���ƽ��
		{
			return OnSubCutCard(pBuffer,wDataSize);
		}
	case SUB_S_SEND_CARD:			//��ע���
		{
			return OnSubSendCard(pBuffer,wDataSize);
		}
	case SUB_S_STOP_CARD:			//�û�ͣ��
		{
			return OnSubStopCard(pBuffer,wDataSize);
		}
	case SUB_S_BRUST_CARD:			//�û�����
		{
			return OnSubBrustCard(pBuffer,wDataSize);
		}
	case SUB_S_BANKER_SEND_CARD:    //ׯ�ҷ���
		{			
			return OnSubBankerSendCard(pBuffer,wDataSize);
		}	
	case SUB_S_USER_LEFT:		   //�û�ǿ��
		{						
			return OnSubUserLeft(pBuffer,wDataSize);
		}	
	case SUB_S_GAME_END:		   //��Ϸ����
		{			
			return OnSubGameEnd(pBuffer,wDataSize);
		}	
	case SUB_S_CUT_CARD_MARK:     //�л�����
		{
			return OnCutCardMark(pBuffer,wDataSize);
		}
	}
	return false;
}


//��Ϸ��ʼ
bool CBJGameDlg::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{	
	//Ч������
	if (wDataSize != sizeof(CMD_S_GameStart)) return false;
	CMD_S_GameStart * pGameStart = (CMD_S_GameStart *)pBuffer;

	//��Ϸ����
	m_lTurnMaxGold   = pGameStart->lMaxGold;
	m_lTurnBasicGold = pGameStart->lTurnBasicGold;

	memset(m_szName, 0, sizeof(m_szName));

	//��������
	WORD wMeChairID  = GetMeChairID();
	bool bLookonMode = IsLookonMode();
	LONG lBaseGold   = m_lTurnBasicGold;

	//����״̬
	SetGameStatus(GS_ADDGOLDING);
	m_GameView.SetGoldTitleInfo(pGameStart->lMaxGold,lBaseGold);

	//���ý���
	m_GameView.m_wBankerStation = SwitchViewChairID(pGameStart->wBankerStation);	
	
	//���ÿ���
	if (bLookonMode == false && wMeChairID != pGameStart->wBankerStation)
	{
		//���ý���
		m_GameView.m_btGiveUp.ShowWindow(SW_HIDE);
		m_GameView.m_btAddGold.ShowWindow(SW_SHOW);
		
		//m_GameView.SetUserGoldInfo(2, false, m_lTurnBasicGold);
		m_GameView.SetUserGoldInfo(SwitchViewChairID(wMeChairID),false,m_lTurnBasicGold);
		m_GameView.m_GoldControl.SetMaxGold(pGameStart->lMaxGold - m_lTurnBasicGold);
		m_GameView.m_GoldControl.ShowWindow(SW_SHOW);

		//���ö�ʱ��	
		SetGameTimer(GetMeChairID(), IDI_ADD_GOLD, 30);		
	}

	for (WORD i = 0; i < GAME_PLAYER; i++)
	{
		//��������
		WORD wViewChairID = SwitchViewChairID(i);
		const tagUserData * pUserData = GetUserData(i);

		//���ý���
		if (pUserData != NULL)
		{

			//m_GameView.SetUserGoldInfo(wViewChairID,true,lBaseGold);
			lstrcpyn(m_szName[i],pUserData->szName,CountArray(m_szName[i]));			
			if(wViewChairID != m_GameView.m_wBankerStation)
				m_GameView.m_cbPlayStatus[wViewChairID] = 1;
		}
		else
		{	
			m_GameView.SetUserGoldInfo(wViewChairID,true,0L);
			m_GameView.SetUserGoldInfo(wViewChairID,false,0L);	
		}

	}
	if (bLookonMode == false) 
	{
		ActiveGameFrame();
		m_GameView.m_CardControl[2].AllowPositively(true);
	}	
	else
	{	
		//���ý���			
		m_GameView.m_ScoreView.ShowWindow(SW_HIDE);	
		for (int i=0;i<CountArray(m_GameView.m_CardControl);i++) m_GameView.m_CardControl[i].SetCardData(NULL,0);
		memset(m_GameView.m_cbTopCardSum,0,sizeof(m_GameView.m_cbTopCardSum));
		memset(m_GameView.m_cbBottomCardSum,0,sizeof(m_GameView.m_cbBottomCardSum));
	}
	m_GameView.UpdateGameView(NULL);
	
	//��������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_BEGIN"));

	return true;
}

//��ʼ����
bool CBJGameDlg::OnSubInitCard(const void * pBuffer, WORD wDataSize)
{
	//��������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("SEND_CARD"));

	//Ч������
	if (wDataSize != sizeof(CMD_S_InitCard)) return false;
	CMD_S_InitCard * pInitCard = (CMD_S_InitCard *)pBuffer;

	BYTE cbCardData[2];	
	
	for (WORD i = 0; i < GAME_PLAYER; i++)
	{
		//��������
		WORD wViewChairID = SwitchViewChairID(i);
		const tagUserData * pUserData = GetUserData(i);

		//���ý���
		if (pUserData != NULL)
		{					
			cbCardData[1] = pInitCard->cbCardData[i];			
			cbCardData[0] = (wViewChairID == 2) ? pInitCard->cbFundusCard : 0;

			m_GameView.m_CardControl[wViewChairID].SetCardData(cbCardData,2);	

			//��ʶ״̬
			if(wViewChairID != m_GameView.m_wBankerStation)
				m_GameView.m_cbPlayStatus[wViewChairID] = 2;
		}
		else
		{
			m_GameView.m_CardControl[wViewChairID].SetCardData(NULL,0);
		}		

		//���ÿؼ�
		m_GameView.m_CardControl[wViewChairID].ShowFirstCard(true);
	}

	//��������
	if(SwitchViewChairID(GetMeChairID()) != m_GameView.m_wBankerStation && !IsLookonMode())
	{
		m_GameView.m_btGetCard.ShowWindow(SW_SHOW);
		m_GameView.m_btDouble.ShowWindow(SW_SHOW);
		m_GameView.m_btStop.ShowWindow(SW_SHOW);	
	
		//�ܷ����
		if(pInitCard->bCanCutCard) m_GameView.m_btCutCard.ShowWindow(SW_SHOW);	

		//���ö�ʱ��
		SetGameTimer(GetMeChairID(), IDI_GET_CARD, 30);
	}	

	//��ʾ�˿���ֵ
	m_GameView.m_cbTopCardSum[2] = pInitCard->cbCardSum;
	m_GameView.UpdateGameView(NULL);

	return true;
}

//�û�����
bool CBJGameDlg::OnSubCutCard(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize != sizeof(CMD_S_CutCard)) return false;
	CMD_S_CutCard * pCutCard = (CMD_S_CutCard *)pBuffer;

	BYTE cbFirstCard[1]; 
	BYTE cbSecondCard[1];
	cbFirstCard[0]     = pCutCard->cbFirstCard;
	cbSecondCard[0]    = pCutCard->cbSecondCard;
	WORD wViewChairID = SwitchViewChairID(pCutCard->wCutCardUser);

	m_GameView.m_CardControl[wViewChairID].SetCardData(cbFirstCard, 1);			
	m_GameView.m_CardControl[wViewChairID + 4].SetCardData(cbSecondCard,1);

	if(pCutCard->wCutCardUser == GetMeChairID())
	{
		m_GameView.m_CardControl[wViewChairID + 4].ShowFirstCard(true);
		m_GameView.m_cbCutCardMark  = 1;
		m_GameView.m_cbTopCardSum[wViewChairID]    = pCutCard->cbFirstCardSum;
		m_GameView.m_cbBottomCardSum[wViewChairID] = pCutCard->cbSecondCardSum;
	}
	else
	{
		m_GameView.m_CardControl[wViewChairID + 4].ShowFirstCard(false);
		m_GameView.m_CardControl[wViewChairID].ShowFirstCard(false);
	}
	m_GameView.UpdateGameView(NULL);

	//��ʶ����
	m_bCutCard = false;

	return true;
}

//�û���ע
bool CBJGameDlg::OnSubAddGold(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize != sizeof(CMD_S_AddGold)) return false;
	CMD_S_AddGold * pAddGold = (CMD_S_AddGold *)pBuffer;

	//����״̬
	SetGameStatus(GS_PLAYING);

	//��������
	WORD wMeChairID   = GetMeChairID();
	WORD wViewChairID = SwitchViewChairID(pAddGold->wCurrentUser);

	//��������
	m_GameView.SetUserGoldInfo(wViewChairID,false,pAddGold->lLastAddGold);

	if ((IsLookonMode() == false)&&(pAddGold->wCurrentUser == wMeChairID))
	{
		//��������
		m_bMeAddGold = true;
	
		//����˫��
		if(!pAddGold->bDoubleGold)
		{
			//��ȡ�˿�
			WORD wCardCount = m_GameView.m_CardControl[2].GetCardData(NULL,0);
		}
		else
		{
			PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_SCORE"));
		}
		ActiveGameFrame();
	}

	//��ʶ״̬
	if(wViewChairID != m_GameView.m_wBankerStation)
		m_GameView.m_cbPlayStatus[wViewChairID] = 0;
	m_GameView.UpdateGameView(NULL);

	return true;
}

//�л�����
bool CBJGameDlg::OnCutCardMark(const void * pBuffer, WORD wDataSize)
{	
	m_GameView.m_cbCutCardMark = 2;

	m_GameView.m_btGetCard.ShowWindow(SW_SHOW);
	m_GameView.m_btStop.ShowWindow(SW_SHOW);
	m_GameView.m_btDouble.ShowWindow(SW_SHOW);

	m_GameView.UpdateGameView(NULL);

	//��ʶ����
	m_bCutCard = true;

	return true;
}
//������Ϣ
bool CBJGameDlg::OnSubSendCard(const void * pBuffer, WORD wDataSize)
{		
	//��������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("SEND_CARD"));
	
	//Ч������
	if (wDataSize != sizeof(CMD_S_SendCard)) return false;
	CMD_S_SendCard * pSendCard = (CMD_S_SendCard *)pBuffer;

	//���½���
	BYTE cbCardData[8];
	LONG lUserTableGold = 0L;

	WORD wCardCount;
	WORD wViewChairID = SwitchViewChairID(pSendCard->wGetCardUser);

	//������
	if(!pSendCard->bCutCard)
	{
		wCardCount = m_GameView.m_CardControl[wViewChairID].GetCardData(cbCardData,CountArray(cbCardData));
		cbCardData[wCardCount++] = pSendCard->cbUserCard;
		m_GameView.m_CardControl[wViewChairID].SetCardData(cbCardData,wCardCount);

		//��ʾ�˿���ֵ
		if(wViewChairID == 2)
		{
			m_GameView.m_cbTopCardSum[2] = pSendCard->cbCardSum;	
		}	
	}
	else
	{
		//������
		wCardCount = m_GameView.m_CardControl[wViewChairID + 4].GetCardData(cbCardData,CountArray(cbCardData));
		cbCardData[wCardCount++] = pSendCard->cbUserCard;
		m_GameView.m_CardControl[wViewChairID + 4].SetCardData(cbCardData,wCardCount);

		//��ʾ�˿���ֵ
		if(wViewChairID == 2)
		{
			m_GameView.m_cbBottomCardSum[2] = pSendCard->cbCardSum;			
		}	
	}

	m_GameView.Invalidate();
	m_GameView.UpdateGameView(NULL);

	return true;
}

//ͣ����Ϣ
bool CBJGameDlg::OnSubStopCard(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize != sizeof(CMD_S_StopCard)) return false;
	CMD_S_StopCard * pStopCard = (CMD_S_StopCard *)pBuffer;

	WORD wViewChairID = SwitchViewChairID(pStopCard->wStopUser);

	//��ʶ״̬
	if((wViewChairID != m_GameView.m_wBankerStation) && (wViewChairID != 2)) 
	{
		if(!pStopCard->bDoubleBrsut)
			m_GameView.m_cbPlayStatus[wViewChairID] = 3;
		else
			m_GameView.m_cbPlayStatus[wViewChairID] = 4;
	}	

	if(wViewChairID == m_GameView.m_wBankerStation) m_GameView.m_cbPlayStatus[wViewChairID] = 0;

	m_GameView.UpdateGameView(NULL);

	return true;
}

//������Ϣ
bool CBJGameDlg::OnSubBrustCard(const void * pBuffer, WORD wDataSize)
{
	if (wDataSize != sizeof(CMD_S_BrustCard)) return false;
	CMD_S_BrustCard * pBrustCard = (CMD_S_BrustCard *)pBuffer;

	WORD wViewChairID = SwitchViewChairID(pBrustCard->wBrustUser);
	
	if(pBrustCard->wBrustUser == GetMeChairID())
	{
		//��������
		PlayGameSound(AfxGetInstanceHandle(),TEXT("BRUSH_CARD"));

		//���ö�ʱ��
		KillGameTimer(IDI_GET_CARD);
		
		//���½���
		m_GameView.m_btGetCard.ShowWindow(SW_HIDE);
		m_GameView.m_btStop.ShowWindow(SW_HIDE);
		m_GameView.m_btDouble.ShowWindow(SW_HIDE);
		m_GameView.m_btCutCard.ShowWindow(SW_HIDE);		
	}

	//���ý���
	m_GameView.m_CardControl[wViewChairID].ShowFirstCard(true);	

	//��ʶ״̬
	if(wViewChairID != m_GameView.m_wBankerStation)
		m_GameView.m_cbPlayStatus[wViewChairID] = 4;
	m_GameView.UpdateGameView(NULL);

	return true;
}

//ׯ�ҷ���
bool CBJGameDlg::OnSubBankerSendCard(const void * pBuffer, WORD wDataSize)
{
	//���ý���
	m_GameView.m_btGetCard.ShowWindow(SW_SHOW);
	m_GameView.m_btStop.ShowWindow(SW_SHOW);

	//���ö�ʱ��
	SetGameTimer(GetMeChairID(), IDI_GET_CARD, 30);
	m_GameView.UpdateGameView(NULL);

	return true;
}

//�û�ǿ��
bool CBJGameDlg::OnSubUserLeft(const void * pBuffer, WORD wDataSize)
{
	return true;
}

//��Ϸ����
bool CBJGameDlg::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{	
	//���ö�ʱ��
	KillGameTimer(IDI_GET_CARD);

	//Ч�����
	if (wDataSize != sizeof(CMD_S_GameEnd)) return false;
	CMD_S_GameEnd * pGameEnd = (CMD_S_GameEnd *)pBuffer;
    
	//��Ϸ��������
	BYTE cbCardData[13];
	m_GameView.m_ScoreView.ResetScore();

	for (WORD i = 0; i < GAME_PLAYER; i++)
	{
		//������Ϣ
		const tagUserData * pUserData = GetUserData(i);

		if (pUserData != NULL)
		{
			lstrcpyn(m_szName[i],pUserData->szName,CountArray(m_szName[i]));
		}
		
		if (m_szName[i][0] != 0) 
		{	
			m_GameView.m_ScoreView.SetGameScore(i,m_szName[i],pGameEnd->lGameGold[i]);

			if (pGameEnd->cbUserCard[i] != 0)
			{
				WORD wViewStation = SwitchViewChairID(i);
				WORD wCardCount = m_GameView.m_CardControl[wViewStation].GetCardData(cbCardData,CountArray(cbCardData));
				cbCardData[0] = pGameEnd->cbUserCard[i];
				m_GameView.m_CardControl[wViewStation].SetCardData(cbCardData,wCardCount);
				m_GameView.m_CardControl[wViewStation].SortCardData();
				m_GameView.m_CardControl[wViewStation].ShowFirstCard(true);	

				wCardCount = m_GameView.m_CardControl[wViewStation+4].GetCardData(cbCardData,CountArray(cbCardData));
				cbCardData[0] = pGameEnd->cbCutCard[i];
				m_GameView.m_CardControl[wViewStation+4].SetCardData(cbCardData,wCardCount);
				m_GameView.m_CardControl[wViewStation+4].SortCardData();
				m_GameView.m_CardControl[wViewStation+4].ShowFirstCard(true);		
				
				//��ʾ��ֵ
				m_GameView.m_cbTopCardSum[wViewStation]    = pGameEnd->cbCardSum[i];
				m_GameView.m_cbBottomCardSum[wViewStation] = pGameEnd->cbCutCardSum[i];
			}			
		}			
	}
	m_GameView.m_ScoreView.SetTax(pGameEnd->lTax);
	m_GameView.m_ScoreView.ShowWindow(SW_SHOW);	

	//��Ϸ����
	m_lTurnMaxGold   = 0L;
	m_lTurnBasicGold = 0L;

	//�������
	m_lGoldShow  = 0L;	
	m_GameView.m_wBankerStation = 255;
	m_GameView.m_cbCutCardMark  = 0;
	memset(m_szName,0,sizeof(m_szName));
	memset(m_GameView.m_cbPlayStatus,0,sizeof(m_GameView.m_cbPlayStatus));

	//��ʶ����
	m_bCutCard = false;

	//���ý���
	if (IsLookonMode() == false)
	{
		m_GameView.m_btBegin.ShowWindow(SW_SHOW);
		m_GameView.m_btBegin.SetFocus();
		//SetGameTimer(GetMeChairID(),IDI_STRAT_GAME,30);
	}
	
	m_GameView.m_GoldControl.ShowWindow(SW_HIDE);
	m_GameView.m_btGiveUp.ShowWindow(SW_HIDE);
	m_GameView.m_btAddGold.ShowWindow(SW_HIDE);

	m_GameView.m_btDouble.ShowWindow(SW_HIDE);
	m_GameView.m_btGetCard.ShowWindow(SW_HIDE);
	m_GameView.m_btStop.ShowWindow(SW_HIDE);
	m_GameView.m_btCutCard.ShowWindow(SW_HIDE);	
	
	m_GameView.UpdateGameView(NULL);

	//����״̬
	SetGameStatus(GS_FREE);

	//��������
	if(pGameEnd->lGameGold[GetMeChairID()] > 0)
		PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WIN"));
	else
		PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_END"));	

	return true;
}

//��Ϸ��ʼ
LRESULT	CBJGameDlg::OnStart(WPARAM wParam, LPARAM lParam)
{
	//��������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("HIT_BUTTON"));
	
	//���ý���	
	m_GameView.ResetGameView();
	m_GameView.m_btBegin.ShowWindow(SW_HIDE);
	m_GameView.m_ScoreView.ShowWindow(SW_HIDE);	
	for (int i=0;i<CountArray(m_GameView.m_CardControl);i++) m_GameView.m_CardControl[i].SetCardData(NULL,0);
	memset(m_GameView.m_cbTopCardSum,0,sizeof(m_GameView.m_cbTopCardSum));
	memset(m_GameView.m_cbBottomCardSum,0,sizeof(m_GameView.m_cbBottomCardSum));
	memset(m_GameView.m_cbPlayStatus,0,sizeof(m_GameView.m_cbPlayStatus));

	KillGameTimer(IDI_STRAT_GAME);

	//������Ϣ
    SendUserReady(NULL,0);

	return true;
}

//�û���ע
LRESULT	CBJGameDlg::OnAddGold(WPARAM wParam, LPARAM lParam)
{	
	//��������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_SCORE"));
	
	LONG lGold =  m_GameView.m_GoldView[4].GetGold() + m_GameView.m_GoldControl.GetGold();

	lGold = lGold;//+m_lTurnBasicGold;/*__max(lGold,m_lTurnBasicGold);*/

	//������Ϣ
	CMD_C_AddGold AddPoint;
	AddPoint.bDoubleGold = false;
	AddPoint.lGold       = lGold;

	SendData(SUB_C_ADD_GOLD,&AddPoint,sizeof(AddPoint));

	//���ö�ʱ��
	KillGameTimer(IDI_ADD_GOLD);	

	//���½���
	m_GameView.m_GoldControl.ShowWindow(SW_HIDE);
	m_GameView.m_btGiveUp.ShowWindow(SW_HIDE);
	m_GameView.m_btAddGold.ShowWindow(SW_HIDE);
	m_GameView.UpdateGameView(NULL);

	return 0;
}

//�û�ȡ��
LRESULT	CBJGameDlg::OnGiveUp(WPARAM wParam, LPARAM lParam)
{
	//��������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("HIT_BUTTON"));

	CMD_C_AddGold AddPoint;
	AddPoint.lGold       = m_lTurnBasicGold;
	AddPoint.bDoubleGold = false;

	//���ö�ʱ��
	KillGameTimer(IDI_ADD_GOLD);
	
	//���ý���
	m_GameView.m_GoldControl.ShowWindow(SW_HIDE);
	m_GameView.m_btGiveUp.ShowWindow(SW_HIDE);
	m_GameView.m_btAddGold.ShowWindow(SW_HIDE);
    
	//��������
	SendData(SUB_C_ADD_GOLD,&AddPoint,sizeof(AddPoint));

	return true;
}

//�û�Ҫ��
LRESULT CBJGameDlg::OnGetCard(WPARAM wParam, LPARAM lParam)
{	
	//��������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("HIT_BUTTON"));
	
	//���ö�ʱ��
	KillGameTimer(IDI_GET_CARD);
	SetGameTimer(GetMeChairID(), IDI_GET_CARD, 30);	

	//������Ϣ
	CMD_C_GetCard GetCard;
	GetCard.bCutCard = m_bCutCard;

	SendData(SUB_C_GET_CARD,&GetCard,sizeof(GetCard));

	//���ý���
	m_GameView.m_btCutCard.ShowWindow(SW_HIDE);	

	return true;
}

//�û�ͣ��
LRESULT CBJGameDlg::OnStopCard(WPARAM wParam, LPARAM lParam)
{
	//��������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("HIT_BUTTON"));
	
	//ֹͣ��ʱ��
	KillGameTimer(IDI_GET_CARD);

	//���ý���
	m_GameView.m_btGetCard.ShowWindow(SW_HIDE);
	m_GameView.m_btDouble.ShowWindow(SW_HIDE);
	m_GameView.m_btStop.ShowWindow(SW_HIDE);
	m_GameView.m_btCutCard.ShowWindow(SW_HIDE);

	//������Ϣ
	CMD_C_StopCard StopCard;
	StopCard.bCutCard = m_bCutCard;
	SendData(SUB_C_STOP_CARD,&StopCard,sizeof(StopCard));

	return true;
}

//�û�˫��
LRESULT CBJGameDlg::OnDoubleCard(WPARAM wParam, LPARAM lParam)
{
	//��������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_SCORE"));
	
	//ֹͣ��ʱ��
	KillGameTimer(IDI_GET_CARD);
	
	//������Ϣ
	CMD_C_AddGold AddPoint;
	AddPoint.bDoubleGold = true;
	AddPoint.lGold       = 0L;   
	SendData(SUB_C_ADD_GOLD,&AddPoint,sizeof(AddPoint));	
	
	CMD_C_GetCard GetCard;
	GetCard.bCutCard = m_bCutCard;
	SendData(SUB_C_GET_CARD,&GetCard,sizeof(GetCard));

	SendData(SUB_C_STOP_CARD);	

	//���ý���
	m_GameView.m_btGetCard.ShowWindow(SW_HIDE);
	m_GameView.m_btDouble.ShowWindow(SW_HIDE);
	m_GameView.m_btStop.ShowWindow(SW_HIDE);
	m_GameView.m_btCutCard.ShowWindow(SW_HIDE);

	return true;
}

//�û�����
LRESULT CBJGameDlg::OnCutCard(WPARAM wParam, LPARAM lParam)
{
	//��������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("HIT_BUTTON"));	

	//������Ϣ
	SendData(SUB_C_CUT_CARD);

	//���ý���
	m_GameView.m_btCutCard.ShowWindow(SW_HIDE);
	
	return true;
}








