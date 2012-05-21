#include "Stdafx.h"
#include "GameClient.h"
#include "GameClientDlg.h"
//IDB_WAIT_CONTINUE IDB_WAIT_BANKER
//////////////////////////////////////////////////////////////////////////

//ʱ���ʶ
#define IDI_BANKER_START			200									//ׯ�ҿ�ʼ
#define IDI_PLAYER_START			201									//��ҿ�ʼ
#define IDI_CHANGE_BANKER			202									//��ׯʱ��

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientDlg, CGameFrameDlg)
	ON_MESSAGE(IDM_START,OnStart)
	ON_MESSAGE(IDM_SCORE,OnScore)
	ON_MESSAGE(IDM_CLEAN,OnClean)
	ON_MESSAGE(IDM_BANKER,OnBanker)
	ON_MESSAGE(IDM_CHIP_IN,OnClipin)
	ON_MESSAGE(IDM_CONTINUE,OnContinue)
	ON_MESSAGE(IDM_SEND_CARD_FINISH,OnSendCardFinish)
	ON_MESSAGE(IDM_RECLAIM_CARD_FINISH,OnReclaimCardFinish)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientDlg::CGameClientDlg() : CGameFrameDlg(&m_GameClientView)
{
	//��ע��Ϣ
	m_lMaxScore=0L;
	m_lLessScore=0L;

	//��ʷ��¼
	m_wDrawCount=0;
	m_lGameScore=0L;

	//��Ϸ����
	m_wBankerUser=INVALID_CHAIR;
	ZeroMemory(m_szAccounts,sizeof(m_szAccounts));

	//��������
	m_bGameScene=false;
	ZeroMemory(m_lChipInCount,sizeof(m_lChipInCount));
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

	UINT uIDButton=m_btControl.GetDlgCtrlID();
	PostMessage(WM_COMMAND,uIDButton,0);

	return true;
}

//���ÿ��
void CGameClientDlg::ResetGameFrame()
{
	//��ע��Ϣ
	m_lMaxScore=0L;
	m_lLessScore=0L;

	//��ʷ��¼
	m_wDrawCount=0;
	m_lGameScore=0L;

	//��Ϸ����
	m_wBankerUser=INVALID_CHAIR;
	ZeroMemory(m_szAccounts,sizeof(m_szAccounts));

	//��������
	m_bGameScene=false;
	ZeroMemory(m_lChipInCount,sizeof(m_lChipInCount));
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));

	return;
}

//��Ϸ����
void CGameClientDlg::OnGameOptionSet()
{
	return;
}

//ʱ����Ϣ
bool CGameClientDlg::OnTimerMessage(WORD wChairID, UINT nElapse, UINT nTimerID)
{
	switch (nTimerID)
	{
	case IDI_BANKER_START:		//ׯ�ҿ�ʼ
		{
			if ((IsLookonMode()==false)&&(nElapse==0)&&(m_wBankerUser==GetMeChairID()))
			{
				OnStart(0,0);
			}

			return true;
		}
	case IDI_PLAYER_START:		//�мҿ�ʼ
		{
			if ((IsLookonMode()==false)&&(nElapse==0)&&(m_wBankerUser!=GetMeChairID()))
			{
				WORD wMeChairID=GetMeChairID();
				const tagUserData * pUserData=GetUserData(wMeChairID);
				tagUserData * pMeUserData=(tagUserData *)pUserData;
				pMeUserData->cbUserStatus=US_SIT;
				PostMessage(WM_CLOSE,0,0);
			}

			return true;
		}
	case IDI_CHANGE_BANKER:		//��ׯʱ��
		{
			if ((IsLookonMode()==false)&&(nElapse==0)&&(m_wBankerUser!=GetMeChairID()))
			{
				OnBanker(0,0);
			}

			return true;
		}
	}

	return false;
}

//�Թ�״̬
void CGameClientDlg::OnLookonChanged(bool bLookonUser, const void * pBuffer, WORD wDataSize)
{
}

//������Ϣ
bool CGameClientDlg::OnGameMessage(WORD wSubCmdID, const void * pBuffer, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_TIMER_INFO:		//ʱ����Ϣ
		{
			if (m_bGameScene==false) return true;
			return OnSubTimeInfo(pBuffer,wDataSize);
		}
	case SUB_S_SCORE_INFO:		//��ע��Ϣ
		{
			if (m_bGameScene==false) return true;
			return OnSubScoreInfo(pBuffer,wDataSize);
		}
	case SUB_S_BANKER_USER:		//ׯ����Ϣ
		{
			if (m_bGameScene==false) return true;
			return OnSubBankerUser(pBuffer,wDataSize);
		}
	case SUB_S_CHANGE_INFO:		//��ׯ��Ϣ
		{
			if (m_bGameScene==false) return true;
			return OnSubChangeInfo(pBuffer,wDataSize);
		}
	case SUB_S_GAME_START:		//��Ϸ��ʼ
		{
			if (m_bGameScene==false) return true;
			return OnSubGameStart(pBuffer,wDataSize);
		}
	case SUB_S_USER_CHIPIN:		//�û���ע
		{
			if (m_bGameScene==false) return true;
			return OnSubUserChipin(pBuffer,wDataSize);
		}
	case SUB_S_SEND_CARD:		//�����˿�
		{
			if (m_bGameScene==false) return true;
			return OnSubSendCard(pBuffer,wDataSize);
		}
	case SUB_S_GAME_END:		//��Ϸ����
		{
			if (m_bGameScene==false) return true;
			return OnSubGameEnd(pBuffer,wDataSize);
		}
	}

	return false;
}

//��Ϸ����
bool CGameClientDlg::OnGameSceneMessage(BYTE cbGameStation, bool bLookonOther, const void * pBuffer, WORD wDataSize)
{
	for(WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.SetUserPost(i,SwitchViewChairID(i));
	}

	switch (cbGameStation)
	{
	case GS_FREE:		//����״̬
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_StatusFree));
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;

			//��������
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pBuffer;

			//��������
			m_bGameScene=true;

			//���ñ���
			m_lMaxScore=pStatusFree->lMaxScore;
			m_lLessScore=pStatusFree->lLessScore;
			m_wBankerUser=pStatusFree->wBankerUser;

			//����λ��
			WORD wMeChairID=GetMeChairID();
			m_GameClientView.SetUserChairInfo(SwitchViewChairID(m_wBankerUser),SwitchViewChairID(wMeChairID));

			//���ý���
			m_GameClientView.SetChipInInfo(m_lMaxScore,m_lLessScore);
			m_GameClientView.SetLongBanker((pStatusFree->bLongBanker==TRUE)?true:false);

			//���밴ť
			if ((IsLookonMode()==false)&&(m_lMaxScore!=0L)) UpdateClipinText(m_lMaxScore);

			//��ʼ����
			if (pStatusFree->wLeaveTime>0)
			{
				//���ý���
				m_GameClientView.SetWaitBanker(true);

				//����ʱ��
				ASSERT(m_wBankerUser!=INVALID_CHAIR);
				SetGameTimer(m_wBankerUser,IDI_BANKER_START,pStatusFree->wLeaveTime);
			}
			if((GetUserData(GetMeChairID())->dwUserRight&UR_GAME_CONTROL)!=0)
			{
				m_GameClientView.m_btControlMax.ShowWindow(SW_SHOW);
			}
			return true;
		}
	case GS_TK_PLAYING:	//��Ϸ״̬
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_StatusPlay));
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;

			//��������
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pBuffer;

			//��������
			m_bGameScene=true;

			//���ñ���
			m_lMaxScore=pStatusPlay->lMaxScore;
			m_lLessScore=pStatusPlay->lLessScore;
			m_wBankerUser=pStatusPlay->wBankerUser;
			CopyMemory(m_cbPlayStatus,pStatusPlay->cbPlayStatus,sizeof(m_cbPlayStatus));

			//�����û�
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				const tagUserData * pUserData=GetUserData(i);
				if (pUserData!=NULL) lstrcpyn(m_szAccounts[i],pUserData->szName,CountArray(m_szAccounts[i]));
			}

			//����λ��
			WORD wMeChairID=GetMeChairID();
			m_GameClientView.SetUserChairInfo(SwitchViewChairID(m_wBankerUser),SwitchViewChairID(wMeChairID));

			//���ý���
			m_GameClientView.SetChipInInfo(m_lMaxScore,m_lLessScore);
			m_GameClientView.SetWaitPlayer((pStatusPlay->cbSendCard==FALSE)?true:false);
			m_GameClientView.SetLongBanker((pStatusPlay->bLongBanker==TRUE)?true:false);

			//������ע
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				if (pStatusPlay->lUserChipIn[i]>0L)
				{
					WORD wViewChairID=SwitchViewChairID(i);
					m_GameClientView.SetUserClipInInfo(wViewChairID,pStatusPlay->lUserChipIn[i]);
				}
			}

			//�м�����
			if (GetMeChairID()!=m_wBankerUser)
			{
				//���ó���
				UpdateClipinText(m_lMaxScore);

				//����״̬
				if ((pStatusPlay->cbSendCard==FALSE)&&(pStatusPlay->lUserChipIn[GetMeChairID()]==0))
				{
					//����״̬
					UpdateClipinStatus(m_lMaxScore);

					//��ʾ����
					m_GameClientView.m_btScore1.ShowWindow(SW_SHOW);
					m_GameClientView.m_btScore2.ShowWindow(SW_SHOW);
					m_GameClientView.m_btScore3.ShowWindow(SW_SHOW);
					m_GameClientView.m_btScore4.ShowWindow(SW_SHOW);
					m_GameClientView.m_btScore5.ShowWindow(SW_SHOW);

					//��ע��ť
					m_GameClientView.m_btClean.ShowWindow(SW_SHOW);
					m_GameClientView.m_btChipIn.ShowWindow(SW_SHOW);
					m_GameClientView.m_btClean.EnableWindow(FALSE);
					m_GameClientView.m_btChipIn.EnableWindow(FALSE);

					//����ʱ��
					SetGameTimer(GetMeChairID(),IDI_PLAYER_START,30);
				}
			}

			//�����˿�
			if (pStatusPlay->cbSendCard==TRUE)
			{
				//�ɷ��˿�
				if (pStatusPlay->cbNeedFinish==TRUE)
				{
					//�ɷ��˿�
					for (BYTE cbIndex=0;cbIndex<3;cbIndex++)
					{
						for (WORD i=0;i<GAME_PLAYER;i++)
						{
							//�û�����
							WORD wChairID=(m_wBankerUser+i+1)%GAME_PLAYER;

							//���ƴ���
							if (m_cbPlayStatus[wChairID]==TRUE)
							{
								//��������
								WORD wViewChairID=SwitchViewChairID(wChairID);

								//�ɷ��˿�
								BYTE cbCardData=pStatusPlay->cbUserCardData[wChairID][cbIndex];
								if (cbCardData!=0) m_GameClientView.DispatchUserCard(wViewChairID,cbCardData);
							}
						}
					}
				}
				else
				{
					//�����˿�
					for (WORD i=0;i<GAME_PLAYER;i++)
					{
						//�û�����
						WORD wChairID=(m_wBankerUser+i+1)%GAME_PLAYER;

						//���ƴ���
						if (m_cbPlayStatus[wChairID]==TRUE)
						{
							WORD wViewChairID=SwitchViewChairID(wChairID);
							WORD wViewChairIndex=m_GameClientView.m_wChairIndex[wViewChairID];
							m_GameClientView.m_CardControl[wViewChairIndex].SetCardData(pStatusPlay->cbUserCardData[wChairID],MAX_COUNT);
						}
					}
				}
			}
			if((GetUserData(GetMeChairID())->dwUserRight&UR_GAME_CONTROL)!=0)
			{
				m_GameClientView.m_btControlMax.ShowWindow(SW_SHOW);
			}

			return true;
		}
	}

	return false;
}

//ȡ����Ϣ
void CGameClientDlg::OnCancel()
{
	if (m_wBankerUser!=GetMeChairID()&&(m_GameClientView.m_btChipIn.IsWindowVisible()))
	{
		WORD wMeChairID=GetMeChairID();
		const tagUserData * pUserData=GetUserData(wMeChairID);
		tagUserData * pMeUserData=(tagUserData *)pUserData;
		pMeUserData->cbUserStatus=US_SIT;
	}

	__super::OnCancel();
}

//ʱ����Ϣ
bool CGameClientDlg::OnSubTimeInfo(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_TimerInfo));
	if (wDataSize!=sizeof(CMD_S_TimerInfo)) return false;

	//��Ϣ����
	CMD_S_TimerInfo * pTimerInfo=(CMD_S_TimerInfo *)pBuffer;

	//����ʱ��
	if (pTimerInfo->wLeaveTime!=0L)
	{
		//����ʱ��
		SetGameTimer(m_wBankerUser,IDI_BANKER_START,pTimerInfo->wLeaveTime);

		//��ʼ��ť
		if (IsLookonMode()==false)
		{
			WORD wMeChairID=GetMeChairID();
			if (wMeChairID==m_wBankerUser) m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
		}

		//���ý���
		m_GameClientView.SetWaitBanker(true);
	}
	else
	{
		//ɾ��ʱ��
		KillGameTimer(IDI_BANKER_START);

		//��ʼ��ť
		if (IsLookonMode()==false)
		{
			WORD wMeChairID=GetMeChairID();
			if (wMeChairID==m_wBankerUser) m_GameClientView.m_btStart.ShowWindow(SW_HIDE);
		}

		//���ý���
		m_GameClientView.SetWaitBanker(false);
	}

	return true;
}

//��ע��Ϣ
bool CGameClientDlg::OnSubScoreInfo(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_ScoreInfo));
	if (wDataSize!=sizeof(CMD_S_ScoreInfo)) return false;

	//��Ϣ����
	CMD_S_ScoreInfo * pScoreInfo=(CMD_S_ScoreInfo *)pBuffer;

	//���ñ���
	m_lMaxScore=pScoreInfo->lMaxScore;
	m_lLessScore=pScoreInfo->lLessScore;

	//���³���
	UpdateClipinText(m_lMaxScore);

	//���ý���
	m_GameClientView.SetChipInInfo(m_lMaxScore,m_lLessScore);

	return true;
}

//ׯ����Ϣ
bool CGameClientDlg::OnSubBankerUser(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_BankerUser));
	if (wDataSize!=sizeof(CMD_S_BankerUser)) return false;

	//��Ϣ����
	CMD_S_BankerUser * pBankerUser=(CMD_S_BankerUser *)pBuffer;

	//���ñ���
	m_lMaxScore=pBankerUser->lMaxScore;
	m_lLessScore=pBankerUser->lLessScore;
	m_wBankerUser=pBankerUser->wBankerUser;

	//����λ��
	WORD wMeChairID=GetMeChairID();
	m_GameClientView.SetUserChairInfo(SwitchViewChairID(m_wBankerUser),SwitchViewChairID(wMeChairID));

	//ɾ��ʱ��
	KillGameTimer(IDI_BANKER_START);

	//��ׯ����
	if (GetMeChairID()==m_wBankerUser)
	{
		//ɾ��ʱ��
		KillGameTimer(IDI_CHANGE_BANKER);

		//���ð�ť
		m_GameClientView.m_btBanker.ShowWindow(SW_HIDE);
		m_GameClientView.m_btNoBanker.ShowWindow(SW_HIDE);
	}

	//���ó���
	UpdateClipinText(m_lMaxScore);

	//���ý���
	m_GameClientView.SetWaitBanker(false);
	m_GameClientView.SetChipInInfo(m_lMaxScore,m_lLessScore);
	m_GameClientView.SetLongBanker((pBankerUser->bLongBanker==TRUE)?true:false);

	//��ͼ����
	for (WORD i=0;i<GAME_PLAYER;i++) m_GameClientView.m_GoldView[i].SetGold(0L);

	return true;
}

//��ׯ��Ϣ
bool CGameClientDlg::OnSubChangeInfo(const void * pBuffer, WORD wDataSize)
{
	//����ʱ��
	SetGameTimer(GetMeChairID(),IDI_CHANGE_BANKER,30);

	//��ʾ��ť
	m_GameClientView.m_btBanker.ShowWindow(SW_SHOW);
	m_GameClientView.m_btNoBanker.ShowWindow(SW_SHOW);

	//�����
	if (IsLookonMode()==false) ActiveGameFrame();

	return true;
}

//��ע��Ϣ
bool CGameClientDlg::OnSubUserChipin(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_UserChipin));
	if (wDataSize!=sizeof(CMD_S_UserChipin)) return false;

	//��Ϣ����
	CMD_S_UserChipin * pUserChipin=(CMD_S_UserChipin *)pBuffer;

	//��������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("CHOOSE_SCORE"));

	//���ó���
	WORD wViewChairID=SwitchViewChairID(pUserChipin->wChairID);
	m_GameClientView.SetUserClipInInfo(wViewChairID,pUserChipin->lUserChipin);

	return true;
}

//�����˿�
bool CGameClientDlg::OnSubSendCard(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_SendCard));
	if (wDataSize!=sizeof(CMD_S_SendCard)) return false;

	//��Ϣ����
	CMD_S_SendCard * pSendCard=(CMD_S_SendCard *)pBuffer;

	//����״̬
	m_GameClientView.SetWaitBanker(false);
	m_GameClientView.SetWaitPlayer(false);

	//�ɷ��˿�
	for (BYTE cbIndex=0;cbIndex<3;cbIndex++)
	{
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			//�û�����
			WORD wChairID;
			if(GetMeChairID()==m_wBankerUser) 
			{
				wChairID=(m_wBankerUser+i+1)%GAME_PLAYER;
			}
			else 
			{
				WORD wBankerViewID=m_GameClientView.GetBankerViewID();
				wChairID=m_GameClientView.GetUserPost((wBankerViewID+i+1)%GAME_PLAYER);
			}

			//���ƴ���
			if ((m_cbPlayStatus[wChairID]==TRUE)&&(GetUserData(wChairID)!=NULL))
			{
				//��������
				WORD wViewChairID=SwitchViewChairID(wChairID);

				//�ɷ��˿�
				BYTE cbCardData=pSendCard->cbUserCardData[wChairID][cbIndex];
				if (cbCardData!=0) m_GameClientView.DispatchUserCard(wViewChairID,cbCardData);
			}
		}
	}

	//ɾ��ʱ��
	KillGameTimer(0);

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
	m_lMaxScore=pGameStart->lMaxScore;
	m_lLessScore=pGameStart->lLessScore;
	CopyMemory(m_cbPlayStatus,pGameStart->cbPlayStatus,sizeof(m_cbPlayStatus));

	//��ʷ��¼
	m_wDrawCount++;
	m_GameClientView.SetUserDrawInfo(m_wDrawCount,m_lGameScore);

	//�����û�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		const tagUserData * pUserData=GetUserData(i);
		if (pUserData!=NULL) lstrcpyn(m_szAccounts[i],pUserData->szName,CountArray(m_szAccounts[i]));
	}

	//���ý���
	KillGameTimer(IDI_BANKER_START);
	m_GameClientView.SetWaitPlayer(true);
	m_GameClientView.SetWaitBanker(false);
	m_GameClientView.SetChipInInfo(m_lMaxScore,m_lLessScore);

	//���ý���
	m_GameClientView.m_ScoreView.ResetScore();

	//�м�����
	if (m_wBankerUser!=GetMeChairID())
	{
		//���³���
		UpdateClipinText(m_lMaxScore);
		UpdateClipinStatus(m_lMaxScore);

		//���ð�ť
		m_GameClientView.m_btClean.ShowWindow(SW_SHOW);
		m_GameClientView.m_btChipIn.ShowWindow(SW_SHOW);
		m_GameClientView.m_btClean.EnableWindow(FALSE);
		m_GameClientView.m_btChipIn.EnableWindow(FALSE);
	}

	//����ʱ��
	SetGameTimer(GetMeChairID(),IDI_PLAYER_START,30);

	//������
	if (IsLookonMode()==false) ActiveGameFrame();

	return true;
}

//��Ϸ����
bool CGameClientDlg::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;

	//ɾ��ʱ��
	KillGameTimer(IDI_PLAYER_START);

	//��Ϣ����
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;

	//��������
	m_GameClientView.FinishDispatchCard();

	//���óɼ�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��ȡ����
		WORD wIndex=(m_wBankerUser+i)%GAME_PLAYER;

		//������Ϣ
		if (m_cbPlayStatus[wIndex]==TRUE)
		{
			//�����û�
			LPCTSTR pszUserName=m_szAccounts[wIndex];
			if (m_szAccounts[wIndex][0]==0) pszUserName=TEXT("���뿪");

			//���óɼ�
			m_GameClientView.m_ScoreView.SetGameScore(i,pszUserName,pGameEnd->lGameScore[wIndex],pGameEnd->lGameTax[wIndex]);
		}
	}

	//��ʾ�ɼ�
	if (m_wBankerUser==GetMeChairID())
	{
		m_GameClientView.m_ScoreView.Create(IDD_GAME_SCORE_BANKER,this);
	}
	else
	{
		m_GameClientView.m_ScoreView.Create(IDD_GAME_SCORE_PLAYER,this);
	}

	//���ÿؼ�
	m_GameClientView.m_btClean.ShowWindow(SW_HIDE);
	m_GameClientView.m_btChipIn.ShowWindow(SW_HIDE);
	m_GameClientView.m_btScore1.EnableWindow(FALSE);
	m_GameClientView.m_btScore2.EnableWindow(FALSE);
	m_GameClientView.m_btScore3.EnableWindow(FALSE);
	m_GameClientView.m_btScore4.EnableWindow(FALSE);
	m_GameClientView.m_btScore5.EnableWindow(FALSE);

	//�������
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if (pGameEnd->lGameScore[i]>0)
		{
			WORD wViewChairID=SwitchViewChairID(i);
			WORD wIndex=m_GameClientView.m_wChairIndex[wViewChairID];
			m_GameClientView.m_bBankerScore[wIndex]=false;
		}
		else
		{
			WORD wViewChairID=SwitchViewChairID(i);
			WORD wIndex=m_GameClientView.m_wChairIndex[wViewChairID];
			m_GameClientView.m_bBankerScore[wIndex]=true;
		}
	}

	m_GameClientView.ReclaimUserScore();

	//��������
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��ȡ�˿�
		BYTE cbCardData[MAX_COUNT];
		WORD wCardCount=m_GameClientView.m_CardControl[i].GetCardData(cbCardData,CountArray(cbCardData));

		//��������
		if (wCardCount==MAX_COUNT)
		{
			//��������
			TCHAR szCardType[16]=TEXT("");
			BYTE cbCardType=m_GameLogic.GetCardType(cbCardData,MAX_COUNT);

			//��������
			switch (cbCardType)
			{
			case CT_VALUES:		//��ֵ����
				{
					//��ȡ�Ƶ�
					BYTE cbValueList=0;
					BYTE cbValueCard=0;
					for (BYTE j=0;j<MAX_COUNT;j++)
					{
						cbValueCard=m_GameLogic.GetCardValue(cbCardData[j]);
						if (cbValueCard<10) cbValueList=(cbValueList+cbValueCard)%10;
					}

					//��������
					_snprintf(szCardType,CountArray(szCardType),TEXT("%d ��"),cbValueList);

					break;
				}
			case CT_THREE_JQK:	//��������
				{
					//��������
					lstrcpyn(szCardType,TEXT("�� ��"),CountArray(szCardType));

					break;
				}
			case CT_THREE_KING:	//��������
				{
					//��������
					lstrcpyn(szCardType,TEXT("�� ��"),CountArray(szCardType));

					break;
				}
			}

			//��������
			m_GameClientView.SetCardTypeInfo(i,szCardType);
		}
		else 
		{
			//��������
			m_GameClientView.SetCardTypeInfo(i,TEXT(""));
		}
	}

	//������ʾ
	m_GameClientView.SetCardType(true);
	m_GameClientView.SetWaitBanker(false);
	m_GameClientView.SetWaitPlayer(false);

	//��ʷ��¼
	m_lGameScore+=pGameEnd->lGameScore[GetMeChairID()];
	m_GameClientView.SetUserDrawInfo(m_wDrawCount,m_lGameScore);

	//��ȡ�˿�
	BYTE cbCardData[MAX_COUNT];
	WORD wViewBanker=SwitchViewChairID(m_wBankerUser);
	WORD wUserIndex=m_GameClientView.m_wChairIndex[wViewBanker];
	WORD wCardCount=m_GameClientView.m_CardControl[wUserIndex].GetCardData(cbCardData,CountArray(cbCardData));

	//���Ž��
	if(wCardCount>0)
	{
		//ׯ������
		BYTE cbBankerValue=0;
		BYTE cbBankerType=m_GameLogic.GetCardType(cbCardData,MAX_COUNT);

		//��ȡ�Ƶ�
		if (cbBankerType==CT_VALUES)
		{
			for (BYTE i=0;i<MAX_COUNT;i++)
			{
				BYTE cbValueCard=m_GameLogic.GetCardValue(cbCardData[i]);
				if (cbValueCard<10) cbBankerValue=(cbBankerValue+cbValueCard)%10;
			}
		}

		//��������
		if (m_wBankerUser==GetMeChairID())
		{
			if (cbBankerType==CT_VALUES)
			{
				TCHAR szSound[32]=TEXT("");
				_snprintf(szSound,CountArray(szSound),TEXT("POINT_%d"),cbBankerValue);
				PlayGameSound(AfxGetInstanceHandle(),szSound);
			}
			else if (cbBankerType==CT_THREE_KING)
			{
				TCHAR szSound[32]=TEXT("");
				_snprintf(szSound,CountArray(szSound),TEXT("THREE_POINT_%d"),m_GameLogic.GetCardValue(cbCardData[0]));
				PlayGameSound(AfxGetInstanceHandle(),szSound);
			}
			else PlayGameSound(AfxGetInstanceHandle(),TEXT("KING"));
		}
		else
		{
			if (pGameEnd->lGameScore[GetMeChairID()]>0)
			{
				if (cbBankerType==CT_VALUES)
				{
					TCHAR szSound[32]=TEXT("");
					_snprintf(szSound,CountArray(szSound),TEXT("LOSE_POINT_%d"),cbBankerValue);
					PlayGameSound(AfxGetInstanceHandle(),szSound);
				}
				else if (cbBankerType==CT_THREE_KING)
				{
					TCHAR szSound[32]=TEXT("");
					_snprintf(szSound,CountArray(szSound),TEXT("LOSE_THREE_POINT_%d"),m_GameLogic.GetCardValue(cbCardData[0]));
					PlayGameSound(AfxGetInstanceHandle(),szSound);
				}
				else PlayGameSound(AfxGetInstanceHandle(),TEXT("LOSE_KING"));
			}
			else
			{
				if (cbBankerType==CT_VALUES)
				{
					TCHAR szSound[32]=TEXT("");
					_snprintf(szSound,CountArray(szSound),TEXT("WIN_POINT_%d"),cbBankerValue);
					PlayGameSound(AfxGetInstanceHandle(),szSound);
				}
				else if (cbBankerType==CT_THREE_KING)
				{
					TCHAR szSound[32]=TEXT("");
					_snprintf(szSound,CountArray(szSound),TEXT("WIN_THREE_POINT_%d"),m_GameLogic.GetCardValue(cbCardData[0]));
					PlayGameSound(AfxGetInstanceHandle(),szSound);
				}
				else PlayGameSound(AfxGetInstanceHandle(),TEXT("WIN_KING"));
			}
		}
	}

	return true;
}

//���³���
void CGameClientDlg::UpdateClipinText(LONG lMaxScore)
{
	//���ó���
	if (m_lMaxScore<=10L)
	{
		m_lChipInCount[0]=1L;
		m_lChipInCount[1]=2L;
		m_lChipInCount[2]=3L;
		m_lChipInCount[3]=5L;
		m_lChipInCount[4]=10L;
	}
	else if (m_lMaxScore<100L)
	{
		m_lChipInCount[0]=10L;
		m_lChipInCount[1]=20L;
		m_lChipInCount[2]=30L;
		m_lChipInCount[3]=40L;
		m_lChipInCount[4]=50L;
	}
	else if (m_lMaxScore<1000L)
	{
		m_lChipInCount[0]=10L;
		m_lChipInCount[1]=20L;
		m_lChipInCount[2]=30L;
		m_lChipInCount[3]=50L;
		m_lChipInCount[4]=100L;
	}
	else if (m_lMaxScore<10000L)
	{
		m_lChipInCount[0]=100L;
		m_lChipInCount[1]=200L;
		m_lChipInCount[2]=300L;
		m_lChipInCount[3]=500L;
		m_lChipInCount[4]=1000L;
	}
	else
	{
		m_lChipInCount[0]=1000L;
		m_lChipInCount[1]=2000L;
		m_lChipInCount[2]=3000L;
		m_lChipInCount[3]=5000L;
		m_lChipInCount[4]=10000L;
	}

	//�����ַ�
	TCHAR szScoreText[5][16];
	for (BYTE i=0;i<CountArray(m_lChipInCount);i++)
	{
		_snprintf(szScoreText[i],CountArray(szScoreText[i]),TEXT("%ld"),m_lChipInCount[i]);
	}

	//���ð�ť
	m_GameClientView.m_btScore1.SetWindowText(szScoreText[0]);
	m_GameClientView.m_btScore2.SetWindowText(szScoreText[1]);
	m_GameClientView.m_btScore3.SetWindowText(szScoreText[2]);
	m_GameClientView.m_btScore4.SetWindowText(szScoreText[3]);
	m_GameClientView.m_btScore5.SetWindowText(szScoreText[4]);

	return;
}

//���³���
void CGameClientDlg::UpdateClipinStatus(LONG lLeaveScore)
{
	//���ð�ť
	m_GameClientView.m_btScore1.EnableWindow((lLeaveScore>=m_lChipInCount[0])?TRUE:FALSE);
	m_GameClientView.m_btScore2.EnableWindow((lLeaveScore>=m_lChipInCount[1])?TRUE:FALSE);
	m_GameClientView.m_btScore3.EnableWindow((lLeaveScore>=m_lChipInCount[2])?TRUE:FALSE);
	m_GameClientView.m_btScore4.EnableWindow((lLeaveScore>=m_lChipInCount[3])?TRUE:FALSE);
	m_GameClientView.m_btScore5.EnableWindow((lLeaveScore>=m_lChipInCount[4])?TRUE:FALSE);

	return;
}

//��ʼ��ť
LRESULT CGameClientDlg::OnStart(WPARAM wParam, LPARAM lParam)
{
	//���ý���
	KillGameTimer(IDI_BANKER_START);

	//���ý���
	m_GameClientView.SetWaitPlayer(true);
	m_GameClientView.SetWaitBanker(false);

	//���ÿؼ�
	for (WORD i=0;i<GAME_PLAYER;i++) 
	{
		m_GameClientView.m_GoldView[i].SetGold(0L);
		m_GameClientView.m_CardControl[i].SetCardData(NULL,0);
	}

	//���ؿؼ�
	m_GameClientView.m_ScoreView.ResetScore();
	m_GameClientView.m_btStart.ShowWindow(SW_HIDE);
	m_GameClientView.m_btScore1.ShowWindow(SW_HIDE);
	m_GameClientView.m_btScore2.ShowWindow(SW_HIDE);
	m_GameClientView.m_btScore3.ShowWindow(SW_HIDE);
	m_GameClientView.m_btScore4.ShowWindow(SW_HIDE);
	m_GameClientView.m_btScore5.ShowWindow(SW_HIDE);

	//������Ϣ
	SendUserReady(NULL,0);

	return 0;
}

//��ע��Ϣ
LRESULT CGameClientDlg::OnScore(WPARAM wParam, LPARAM lParam)
{
	//��ȡ��ע
	ASSERT(wParam<CountArray(m_lChipInCount));
	LONG lScore=m_lChipInCount[(BYTE)(wParam)];

	//������ע
	WORD wMeChairID=GetMeChairID();
	WORD wMeViewChairID=SwitchViewChairID(wMeChairID);
	LONG lMeTableScore=m_GameClientView.GetUserClipInInfo(wMeViewChairID);
	m_GameClientView.SetUserClipInInfo(wMeViewChairID,lMeTableScore+lScore);

	//���³���
	ASSERT((m_lMaxScore-lMeTableScore-lScore)>=0L);
	UpdateClipinStatus(m_lMaxScore-lMeTableScore-lScore);

	//��ע��ť
	m_GameClientView.m_btClean.EnableWindow(TRUE);
	m_GameClientView.m_btChipIn.EnableWindow((lMeTableScore+lScore)>=m_lLessScore);

	return 0;
}

//�����Ϣ
LRESULT CGameClientDlg::OnClean(WPARAM wParam, LPARAM lParam)
{
	//���ó���
	m_GameClientView.m_GoldView[3].SetGold(0L);

	//���½���
	m_GameClientView.UpdateGameView(NULL);

	//���ÿؼ�
	UpdateClipinStatus(m_lMaxScore);
	m_GameClientView.m_btClean.EnableWindow(FALSE);
	m_GameClientView.m_btChipIn.EnableWindow(FALSE);

	return 0;
}

//��ע��Ϣ
LRESULT CGameClientDlg::OnClipin(WPARAM wParam, LPARAM lParam)
{
	//ɾ��ʱ��
	KillGameTimer(IDI_PLAYER_START);

	//����ؼ�
	m_GameClientView.m_btScore1.EnableWindow(FALSE);
	m_GameClientView.m_btScore2.EnableWindow(FALSE);
	m_GameClientView.m_btScore3.EnableWindow(FALSE);
	m_GameClientView.m_btScore4.EnableWindow(FALSE);
	m_GameClientView.m_btScore5.EnableWindow(FALSE);

	//���ÿռ�
	m_GameClientView.m_btClean.ShowWindow(SW_HIDE);
	m_GameClientView.m_btChipIn.ShowWindow(SW_HIDE);

	//��������
	CMD_C_UserChipin UserChipin;
	ZeroMemory(&UserChipin,sizeof(UserChipin));

	//�������
	WORD wViewChairID=SwitchViewChairID(GetMeChairID());
	UserChipin.lScore=m_GameClientView.GetUserClipInInfo(wViewChairID);

	//������Ϣ
	SendData(SUB_C_USER_CHIPIN,&UserChipin,sizeof(UserChipin));

	return 0;
}

//��ׯ��Ϣ
LRESULT CGameClientDlg::OnBanker(WPARAM wParam, LPARAM lParam)
{
	//ɾ��ʱ��
	KillGameTimer(IDI_CHANGE_BANKER);

	//���ذ�ť
	m_GameClientView.m_btBanker.ShowWindow(SW_HIDE);
	m_GameClientView.m_btNoBanker.ShowWindow(SW_HIDE);

	//�������
	CMD_C_ChangeInfo ChangeInfo;
	ZeroMemory(&ChangeInfo,sizeof(ChangeInfo));

	//���ñ���
	ChangeInfo.cbChange=(wParam!=0L)?TRUE:FALSE;

	//������Ϣ
	SendData(SUB_C_CHANGE_INFO,&ChangeInfo,sizeof(ChangeInfo));

	return 0;
}

//������Ϣ
LRESULT CGameClientDlg::OnContinue(WPARAM wParam, LPARAM lParam)
{
	//��ȡ�˿�
	m_GameClientView.ReclaimUserCard();

	return 0;
}

//�������
LRESULT CGameClientDlg::OnSendCardFinish(WPARAM wParam, LPARAM lParam)
{
	//��������
	if (IsLookonMode()==false) SendData(SUB_C_SEND_FINISH);

	return 0;
}

//�������
LRESULT CGameClientDlg::OnReclaimCardFinish(WPARAM wParam, LPARAM lParam)
{
	//���ÿؼ�
	for (WORD i=0;i<GAME_PLAYER;i++) 
	{
		m_GameClientView.m_GoldView[i].SetGold(0L);
		m_GameClientView.m_CardControl[i].SetCardData(NULL,0L);
	}

	//���ý���
	m_GameClientView.SetCardType(false);
	m_GameClientView.SetWaitBanker(true);

	//���ͼ���
	if (IsLookonMode()==false)/*OnStart(0,0)*/ SendData(SUB_C_CONTINUE_GAME); 

	return 0;
}

//////////////////////////////////////////////////////////////////////////
