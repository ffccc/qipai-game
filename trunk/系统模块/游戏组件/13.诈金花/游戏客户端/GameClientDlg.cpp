#include "Stdafx.h"
#include "GameOption.h"
#include "GameClient.h"
#include "GameClientDlg.h"

//////////////////////////////////////////////////////////////////////////

//��ʱ����ʶ
#define IDI_START_GAME				200									//��ʼ��ʱ��
#define IDI_USER_ADD_SCORE			201									//��ע��ʱ��
#define IDI_USER_COMPARE_CARD		202									//ѡ�����û���ʱ��
#define IDI_DISABLE					203									//���˶�ʱ��

//ʱ���ʶ
//#define TIME_START_GAME				30									//��ʼ��ʱ��
//#define TIME_USER_ADD_SCORE			30									//��ע��ʱ��
//#define	TIME_USER_COMPARE_CARD		30									//���ƶ�ʱ��
#define TIME_START_GAME				30									//��ʼ��ʱ��
#define TIME_USER_ADD_SCORE			30									//��ע��ʱ��
#define	TIME_USER_COMPARE_CARD		30									//���ƶ�ʱ��

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientDlg, CGameFrameDlg)
	ON_MESSAGE(IDM_START,OnStart)
	ON_MESSAGE(IDM_ADD_SCORE,OnAddScore)
	ON_MESSAGE(IDM_MIN_SCORE,OnMinScore)
	ON_MESSAGE(IDM_MAX_SCORE,OnMaxScore)
	ON_MESSAGE(IDM_CONFIRM,OnConfirmScore)
	ON_MESSAGE(IDM_CANCEL_ADD,OnCancelAdd)
	ON_MESSAGE(IDM_LOOK_CARD,OnLookCard)
	ON_MESSAGE(IDM_COMPARE_CARD,OnCompareCard)
	ON_MESSAGE(IDM_OPEN_CARD,OnOpenCard)
	ON_MESSAGE(IDM_GIVE_UP,OnGiveUp)
	ON_MESSAGE(IDM_SEND_CARD_FINISH,OnSendCardFinish)
	ON_MESSAGE(IDM_FALSH_CARD_FINISH,OnFlashCardFinish)
	ON_MESSAGE(IDM_COMPARE_USER,OnChooseCUser)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientDlg::CGameClientDlg() : CGameFrameDlg(&m_GameClientView)
{
	//���ñ���
	m_dwCardHSpace=DEFAULT_PELS;

	//�û���Ϣ
	m_wCurrentUser=INVALID_CHAIR;
	m_wBankerUser=INVALID_CHAIR;
	m_wWinnerUser=INVALID_CHAIR;

	//��ע��Ϣ
	m_lMaxScore=0L;
	m_lMaxCellScore=0L;
	m_lCellScore=0L;
	m_lCurrentTimes=1L;
	m_lUserMaxScore=0L;
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));

	//������Ϣ
	for (int i=0;i<GAME_PLAYER;i++)
	{
		m_wLostUserID[i]=INVALID_CHAIR;
		m_wViewChairID[i]=INVALID_CHAIR;
		m_bMingZhu[i]=false;
	}

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
	//���ñ���
	SetWindowText(TEXT("թ����Ϸ  --  Ver��6.5.1.0"));

	//����ͼ��
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));

	//��ȡ����
	m_dwCardHSpace=AfxGetApp()->GetProfileInt(TEXT("GameOption"),TEXT("CardSpace"),DEFAULT_PELS);

	//��������
	if ((m_dwCardHSpace>MAX_PELS)||(m_dwCardHSpace<LESS_PELS)) m_dwCardHSpace=DEFAULT_PELS;

	//���ÿؼ�
	for (int i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.m_CardControl[i].SetCardSpace(m_dwCardHSpace);
	}

	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);

	//���Դ���
#ifdef _DEBUG
	//TCHAR szBuffer[32];
	//_snprintf(szBuffer,CountArray(szBuffer),TEXT("%s��%+ld"),TEXT("�������"),-5);
	//InsertGeneralString(szBuffer,RGB(0,0,255),true);
	//m_GameClientView.m_btStart.ShowWindow(5);
	//m_GameClientView.m_ScoreView.ResetScore();
	//LONG lGameScore = 10;
	//LONG lGameTax = 10;
	//for (WORD i=0;i<GAME_PLAYER;i++)
	//{	
	//	BYTE bTemp[3]={1,2,3};
	//	m_GameClientView.m_CardControl[i].SetCardData(bTemp,3);
	//	m_GameClientView.m_CardControl[i].SetDisplayHead(true);
	//	//������Ϣ
	//	m_GameClientView.m_ScoreView.SetGameTax(lGameTax,i);
	//	if (m_szAccounts[i][0]==0) m_GameClientView.m_ScoreView.SetGameScore(i,TEXT("���뿪"),lGameScore);
	//	else m_GameClientView.m_ScoreView.SetGameScore(i,m_szAccounts[i],lGameScore);
	//}

	//m_GameClientView.m_ScoreView.ShowWindow(SW_SHOW);
	//m_GameClientView.m_btMinScore.ShowWindow(5);
	//BYTE bTemp[3]={1,2,3};
	//m_GameClientView.m_CardControl[2].SetCardData(bTemp,3);
	//m_GameClientView.m_CardControl[2].SetPositively(true);
	//0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,	//���� A - K
	//0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	//÷�� A - K
	//0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	//���� A - K
	//0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D	//���� A - K

	//BYTE bFirstTemp[3]={0x32,0x11,0x23};
	//BYTE bNextTemp[3]={0x3b,0x06,0x26};
	//if(m_GameLogic.CompareCard(bFirstTemp,bNextTemp,3))
	//	MessageBox("FirstTemp Win!");
	//else MessageBox("NextTemp Win!");
	/*m_GameClientView.m_ScoreView.SetGameScore(2,"abc",1122);
	m_GameClientView.m_ScoreView.SetGameScore(1,"abc",1221);
	m_GameClientView.m_ScoreView.SetGameScore(0,"ab122221c",122222221);
	m_GameClientView.m_ScoreView.SetGameScore(3,"67",11);
	for (WORD i=0;i<GAME_PLAYER;i++)m_GameClientView.m_ScoreView.SetGameTax(12222223,i);
	m_GameClientView.m_ScoreView.ShowWindow(SW_SHOW);
	m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
	m_GameClientView.m_btConfirm.ShowWindow(SW_SHOW);
	m_GameClientView.m_btMaxScore.ShowWindow(SW_SHOW);
	m_GameClientView.m_btMinScore.ShowWindow(SW_SHOW);
	m_GameClientView.m_btOpenCard.ShowWindow(SW_SHOW);
	*/
	//BYTE bFirstTemp[3]={0x31,0x23,0x03};

	//for(int i=0;i<GAME_PLAYER;i++)
	//{
	//	//m_GameClientView.SetUserTableScore(2,50,999990);
	//	//m_GameClientView.m_CardControl[i].SetDisplayHead(true);
	//	m_GameClientView.m_CardControl[i].SetCardData(bFirstTemp,3);
	//}
	//m_GameClientView.m_CardControl[2].SetCardColor(2);

	//m_GameClientView.m_btConfirm.ShowWindow(SW_SHOW);
	//m_GameClientView.m_btCancel.ShowWindow(SW_SHOW);
	//m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
	//m_GameClientView.m_btMinScore.ShowWindow(SW_SHOW);
	//m_GameClientView.m_btMaxScore.ShowWindow(SW_SHOW);
	//m_GameClientView.m_GoldControl.SetGoldCount(10000L);
	//m_GameClientView.m_GoldControl.SetGoldCount(10000);
	//m_GameClientView.m_GoldControl.SetGold(44000L);
	//m_GameClientView.m_GoldControl.SetGoldCount(10);
	//m_GameClientView.m_GoldControl.ShowWindow(SW_SHOW);
	//m_GameClientView.m_ScoreView.ShowWindow(SW_SHOW);
	//m_GameClientView.SetScoreInfo(100,100);

	//m_GameClientView.m_btMinScore.ShowWindow(5);
#endif

	return true;
}

//���ÿ��
void CGameClientDlg::ResetGameFrame()
{
	//�û���Ϣ
	m_wCurrentUser=INVALID_CHAIR;
	m_wBankerUser=INVALID_CHAIR;
	m_wWinnerUser=INVALID_CHAIR;

	//��ע��Ϣ
	m_lMaxScore=0L;
	m_lMaxCellScore=0L;
	m_lCellScore=0L;
	m_lCurrentTimes=1L;
	m_lUserMaxScore=0L;
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));

	//������Ϣ
	for (int i=0;i<GAME_PLAYER;i++)
	{
		m_wLostUserID[i]=INVALID_CHAIR;
		m_wViewChairID[i]=INVALID_CHAIR;
		m_bMingZhu[i]=false;
	}

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
		for (int i=0;i<GAME_PLAYER;i++) m_GameClientView.m_CardControl[i].SetCardSpace(m_dwCardHSpace);

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
				PostMessage(WM_CLOSE);
				return false;
			}

			//����֪ͨ
			if ((nElapse<=5)&&(IsLookonMode()==false)) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));

			return true;
		}
	case IDI_USER_ADD_SCORE:	//��ע��ʱ��
		{
			WORD wMeChairID=GetMeChairID();

			//��ֹ�ж�
			if (nElapse==0)
			{
				//������ע
				if ((IsLookonMode()==false)&&(wMeChairID==m_wCurrentUser))
				{
					//�Զ�����
					OnGiveUp(0,0);		
				}
				return false;
			}

			//����֪ͨ
			if ((nElapse<=5)&&(wMeChairID==m_wCurrentUser)&&(IsLookonMode()==false))
			{
				PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));
				return true;
			}

			return true;
		}
	case IDI_DISABLE:			//���˶�ʱ��
		{
			if (nElapse==0)
			{
				//ɾ����ʱ��
				KillGameTimer(IDI_DISABLE);
			}
			return true;
		}
	case IDI_USER_COMPARE_CARD:	//ѡ��ʱ��
		{
			//��ֹ�ж�
			if (nElapse==0)
			{
				//ɾ����ʱ��
				KillGameTimer(IDI_USER_COMPARE_CARD);
				WORD wMeChairID=GetMeChairID();

				//�������
				m_GameClientView.SetCompareCard(false,NULL);

				//�������
				CMD_C_CompareCard CompareCard;
				ZeroMemory(&CompareCard,sizeof(CompareCard));

				//�����ϼ�
				for (LONG i=wMeChairID-1;;i--)
				{
					if(i==-1)i=GAME_PLAYER-1;
					if(m_cbPlayStatus[i]==TRUE)
					{
						CompareCard.wCompareUser=(WORD)i;
						break;
					}
				}

				//������Ϣ
				SendData(SUB_C_COMPARE_CARD,&CompareCard,sizeof(CompareCard));

				return false;
			}

			//����֪ͨ
			if ((nElapse<=5)&&(GetMeChairID()==m_wCurrentUser)&&(IsLookonMode()==false))
			{
				PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));
				return true;
			}

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
		if(IsAllowLookon() 
			&& m_GameClientView.m_CardControl[m_wViewChairID[GetMeChairID()]].GetLookCard())
			m_GameClientView.m_CardControl[m_wViewChairID[GetMeChairID()]].SetDisplayHead(true);
		else m_GameClientView.m_CardControl[m_wViewChairID[GetMeChairID()]].SetDisplayHead(false);
	}
	return;
}

//������Ϣ
bool CGameClientDlg::OnGameMessage(WORD wSubCmdID, const void * pBuffer, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_GAME_START:		//��Ϸ��ʼ
		{
			//��Ϣ����
			return OnSubGameStart(pBuffer,wDataSize);
		}
	case SUB_S_ADD_SCORE:		//�û���ע
		{
			//��Ϣ����
			return OnSubAddScore(pBuffer,wDataSize);
		}
	case SUB_S_LOOK_CARD:		//������Ϣ
		{
			//��Ϣ����
			return OnSubLookCard(pBuffer,wDataSize);
		}
	case SUB_S_COMPARE_CARD:	//������Ϣ
		{
			//��Ϣ����
			return OnSubCompareCard(pBuffer,wDataSize);
		}
	case SUB_S_OPEN_CARD:		//������Ϣ
		{
			//��Ϣ����
			return OnSubOpenCard(pBuffer,wDataSize);
		}
	case SUB_S_GIVE_UP:			//�û�����
		{
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				if(m_wLostUserID[i]==INVALID_CHAIR)continue;
				//�����ѿ�
				WORD wViewChairID=m_wViewChairID[m_wLostUserID[i]];
				m_GameClientView.m_CardControl[wViewChairID].SetCardColor(2);
			}

			//��Ϣ����
			return OnSubGiveUp(pBuffer,wDataSize);
		}
	case SUB_S_PLAYER_EXIT:		//�û�ǿ��
		{
			//��Ϣ����
			return OnSubPlayerExit(pBuffer,wDataSize);
		}
	case SUB_S_GAME_END:		//��Ϸ����
		{
			//��������
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				if(m_wLostUserID[i]==INVALID_CHAIR)continue;

				//�˿˱���
				WORD wViewChairID=m_wViewChairID[m_wLostUserID[i]];				
				m_GameClientView.m_CardControl[wViewChairID].SetCardColor(2);
			}

			//��Ϣ����
			return OnSubGameEnd(pBuffer,wDataSize);
		}
	case SUB_S_WAIT_COMPARE:	//�ȴ�����
		{
			//��Ϣ����
			if (wDataSize!=sizeof(CMD_S_WaitCompare)) return false;
			CMD_S_WaitCompare * pWaitCompare=(CMD_S_WaitCompare *)pBuffer;	
			ASSERT(pWaitCompare->wCompareUser==m_wCurrentUser);

			WORD wMeChairID=GetMeChairID();

			if(wMeChairID!=m_wCurrentUser)
			{
				//���Ʊ���
				m_GameClientView.m_CardControl[m_wViewChairID[m_wCurrentUser]].SetCompareBack(true);

				SetGameTimer(m_wCurrentUser,IDI_DISABLE,TIME_USER_COMPARE_CARD);
			}

			//��ʾ��־
			if(wMeChairID!=m_wCurrentUser || IsLookonMode()) m_GameClientView.SetWaitUserChoice(TRUE);

			return true;
		}
	}

	return false;
}

//��Ϸ����
bool CGameClientDlg::OnGameSceneMessage(BYTE cbGameStation, bool bLookonOther, const void * pBuffer, WORD wDataSize)
{
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
				m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
				m_GameClientView.m_btStart.SetFocus();
				SetGameTimer(GetMeChairID(),IDI_START_GAME,TIME_START_GAME);
			}

			return true;
		}
	case GS_PLAYING:	//��Ϸ״̬
		{
			for (WORD i=0;i<GAME_PLAYER;i++)m_wViewChairID[i]=SwitchViewChairID(i);

			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pBuffer;
			WORD wMeChairID=GetMeChairID();

			//��ע��Ϣ
			m_lMaxCellScore=pStatusPlay->lMaxCellScore;
			m_lCellScore=pStatusPlay->lCellScore;
			m_lCurrentTimes=pStatusPlay->lCurrentTimes;
			m_lUserMaxScore=pStatusPlay->lUserMaxScore;
			m_GameClientView.m_GoldControl.SetGoldCount(m_lCellScore);

			//���ñ���
			m_wBankerUser=pStatusPlay->wBankerUser;
			m_wCurrentUser=pStatusPlay->wCurrentUser;
			CopyMemory(m_bMingZhu,pStatusPlay->bMingZhu,sizeof(pStatusPlay->bMingZhu));
			CopyMemory(m_lTableScore,pStatusPlay->lTableScore,sizeof(pStatusPlay->lTableScore));
			CopyMemory(m_cbPlayStatus,pStatusPlay->cbPlayStatus,sizeof(pStatusPlay->cbPlayStatus));

			//�����˿�
			CopyMemory(&m_cbHandCardData[wMeChairID],&pStatusPlay->cbHandCardData,MAX_COUNT);

			//���ñ���
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				m_GameClientView.m_CardControl[i].SetDisplayHead(false);
				if(m_lTableScore[i]>0L)m_GameClientView.SetUserTableScore(m_wViewChairID[i],m_lTableScore[i],m_lTableScore[i]);

				//��ȡ�û�
				const tagUserData * pUserData=GetUserData(i);
				if (pUserData==NULL) continue;

				//Ч��״̬
				//if(pUserData->cbUserStatus==US_PLAY) ASSERT(m_cbPlayStatus[i]==TRUE);
				//����Ч��,����:�ڽ���ʱ,Ӯ�Ҷ������½���,״̬SIT

				//�û�����
				lstrcpyn(m_szAccounts[i],pUserData->szName,CountArray(m_szAccounts[i]));
			}

			//ׯ�ұ�־
			WORD wID=m_wViewChairID[m_wBankerUser];
			m_GameClientView.SetBankerUser(wID);

			//������Ϣ
			m_GameClientView.SetScoreInfo(m_lMaxCellScore,m_lCellScore);

			//���ý���
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//����λ��
				WORD wViewChairID=m_wViewChairID[i];

				//�����˿�
				if (m_cbPlayStatus[i]==TRUE) 
				{
					m_GameClientView.m_CardControl[wViewChairID].SetCardData(m_cbHandCardData[i],MAX_COUNT);
					if((i!=wMeChairID ||IsLookonMode()) && m_bMingZhu[i])
					{
						m_GameClientView.m_CardControl[wViewChairID].SetLookCard(true);
					}
				}
			}
			if(m_bMingZhu[wMeChairID] && (!IsLookonMode() || IsAllowLookon()))
			{
				m_GameClientView.m_CardControl[m_wViewChairID[wMeChairID]].SetDisplayHead(true);
			}

			//�ؼ�����
			m_GameClientView.m_btStart.ShowWindow(SW_HIDE);

			//�жϿؼ�
			if(m_wCurrentUser<GAME_PLAYER)
			{
				if(!(pStatusPlay->bCompareState))
				{
					//�ؼ���Ϣ
					if(!IsLookonMode() && wMeChairID==m_wCurrentUser)
					{
						UpdataControl();
					}

					//����ʱ��
					SetGameTimer(m_wCurrentUser,IDI_USER_ADD_SCORE,TIME_USER_ADD_SCORE);
				}
				else
				{	
					if(!IsLookonMode() && wMeChairID==m_wCurrentUser)
					{
						//ѡ����ұ���
						BOOL bCompareUser[GAME_PLAYER];
						for(int i=0;i<GAME_PLAYER;i++)
						{
							if(m_cbPlayStatus[i]==TRUE && wMeChairID!=i)
							{
								bCompareUser[m_wViewChairID[i]]=TRUE;
							}
							else bCompareUser[m_wViewChairID[i]]=FALSE;
						}
						//���ü�ͷ
						m_GameClientView.SetCompareCard(true,bCompareUser);

						//�ȴ�����
						SendData(SUB_C_WAIT_COMPARE,NULL,0);

						//����ʱ��
						SetGameTimer(wMeChairID,IDI_USER_COMPARE_CARD,TIME_USER_COMPARE_CARD);

						//��ʾ��־
						m_GameClientView.SetWaitUserChoice(FALSE);
					}
					else 
					{
						//���Ʊ���
						m_GameClientView.m_CardControl[m_wViewChairID[m_wCurrentUser]].SetCompareBack(true);

						SetGameTimer(m_wCurrentUser,IDI_DISABLE,TIME_USER_COMPARE_CARD);

						//��ʾ��־
						m_GameClientView.SetWaitUserChoice(TRUE);
					}
				}
			}
			return true;
		}
	}

	return false;
}

//��Ϸ��ʼ
bool CGameClientDlg::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

	//�Թ۽���
	if(IsLookonMode())
	{	
		//m_GameClientView.DisplayType(false);
		m_GameClientView.SetBankerUser(INVALID_CHAIR);
		m_GameClientView.SetScoreInfo(0,0);
		m_GameClientView.m_btStart.ShowWindow(SW_HIDE);
		m_GameClientView.m_ScoreView.ShowWindow(SW_HIDE);
		m_GameClientView.SetUserTableScore(INVALID_CHAIR,0L,0L);
		for (WORD i=0;i<GAME_PLAYER;i++) 
		{
			m_GameClientView.m_CardControl[i].SetCardData(NULL,0);
			m_GameClientView.m_CardControl[i].SetDisplayHead(false);
			m_GameClientView.m_CardControl[i].SetCardColor(INVALID_CHAIR);
			m_GameClientView.m_CardControl[i].SetLookCard(false);
			m_GameClientView.m_CardControl[i].SetCompareCard(false);
		}

		//״̬����
		m_wCurrentUser=INVALID_CHAIR;
		m_wBankerUser=INVALID_CHAIR;
		ZeroMemory(m_szAccounts,sizeof(m_szAccounts));
		ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));
		ZeroMemory(m_lTableScore,sizeof(m_lTableScore));

		//��ע��Ϣ
		m_lMaxCellScore=0L;
		m_lCellScore=0L;
	}

	//������Ϣ
	m_lCellScore=pGameStart->lCellScore;
	m_lMaxCellScore=pGameStart->lMaxScore;
	m_lCurrentTimes=pGameStart->lCurrentTimes;
	m_wCurrentUser=pGameStart->wCurrentUser;
	m_wBankerUser=pGameStart->wBankerUser;
	m_lUserMaxScore=pGameStart->lUserMaxScore;
	m_GameClientView.m_GoldControl.SetGoldCount(m_lCellScore);

	//���ñ���
	LONG lTableScore=0L;
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_wLostUserID[i]=INVALID_CHAIR;
		m_wViewChairID[i]=SwitchViewChairID(i);

		//��ȡ�û�
		const tagUserData * pUserData=GetUserData(i);
		if (pUserData==NULL) continue;

		//��Ϸ��Ϣ		
		m_cbPlayStatus[i]=TRUE;
		lTableScore+=m_lCellScore;
		m_lTableScore[i]=m_lCellScore;
		m_GameClientView.SetUserTableScore(m_wViewChairID[i],m_lTableScore[i],m_lTableScore[i]);

		//�û���Ϣ
		lstrcpyn(m_szAccounts[i],pUserData->szName,CountArray(m_szAccounts[i]));
	}

	//��������
	m_GameClientView.SetBankerUser(m_wViewChairID[m_wBankerUser]);
	m_GameClientView.SetScoreInfo(m_lMaxCellScore,m_lCellScore);

	//�ɷ��˿�
	for(WORD i=0;i<MAX_COUNT;i++)
	{
		for (WORD j=m_wBankerUser;j<m_wBankerUser+GAME_PLAYER;j++)
		{
			WORD w=j%GAME_PLAYER;
			if (m_cbPlayStatus[w]==TRUE)
			{
				m_GameClientView.DispatchUserCard(m_wViewChairID[w],0);
			}
		}
	}

	//����Ч��
	PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));

	return true;
}

//�û�����
bool CGameClientDlg::OnSubGiveUp(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_GiveUp)) return false;
	CMD_S_GiveUp * pGiveUp=(CMD_S_GiveUp *)pBuffer;

	//ɾ����ʱ��
	if(IsLookonMode() || pGiveUp->wGiveUpUser!=GetMeChairID())KillGameTimer(IDI_USER_ADD_SCORE);	

	//���ñ���
	m_cbPlayStatus[pGiveUp->wGiveUpUser]=FALSE;

	//��������
	WORD wGiveUpUser=pGiveUp->wGiveUpUser;
	WORD wViewChairID=m_wViewChairID[wGiveUpUser];

	//�˿˱��
	m_GameClientView.m_CardControl[wViewChairID].SetCardColor(1);

	//�������ñ�ѡ�����û�
	if(m_wCurrentUser==GetMeChairID() && m_GameClientView.GetCompareInfo())
	{
		BOOL bCompareUser[GAME_PLAYER];
		for(int i=0;i<GAME_PLAYER;i++)
		{
			if(m_cbPlayStatus[i]==TRUE && GetMeChairID()!=i)
			{
				bCompareUser[m_wViewChairID[i]]=TRUE;
			}
			else bCompareUser[m_wViewChairID[i]]=FALSE;
		}

		//���ü�ͷ
		m_GameClientView.SetCompareCard(true,bCompareUser);
	}

	//״̬����
	if ((IsLookonMode()==false)&&(pGiveUp->wGiveUpUser==GetMeChairID())) SetGameStatus(GS_FREE);

	//��������
	if ((IsLookonMode()==true)||(wGiveUpUser!=GetMeChairID())) PlayGameSound(AfxGetInstanceHandle(),TEXT("GIVE_UP"));

	return true;
}

//�û���ע
bool CGameClientDlg::OnSubAddScore(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_AddScore));
	if (wDataSize!=sizeof(CMD_S_AddScore)) return false;
	CMD_S_AddScore * pAddScore=(CMD_S_AddScore *)pBuffer;
	ASSERT(pAddScore->wCurrentUser < GAME_PLAYER);

	//ɾ����ʱ��
	if(IsLookonMode() || pAddScore->wAddScoreUser!=GetMeChairID())KillGameTimer(IDI_USER_ADD_SCORE);

	//��������
	WORD wMeChairID=GetMeChairID();
	WORD wAddScoreUser=pAddScore->wAddScoreUser;
	WORD wViewChairID=m_wViewChairID[wAddScoreUser];
	m_lCurrentTimes=pAddScore->lCurrentTimes;
	ASSERT(m_lCurrentTimes<=m_lMaxCellScore/m_lCellScore);

	//��ע����
	if ((IsLookonMode()==true)||(wAddScoreUser!=wMeChairID))
	{
		//��ע���
		m_lTableScore[pAddScore->wAddScoreUser]+=pAddScore->lAddScoreCount;

		//��ע����
		m_GameClientView.SetUserTableScore(wViewChairID,m_lTableScore[pAddScore->wAddScoreUser],pAddScore->lAddScoreCount);

		//��������
		if (m_cbPlayStatus[wAddScoreUser]==TRUE)
		{
			 PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_SCORE"));
		}
	}

	//��ǰ�û�
	m_wCurrentUser=pAddScore->wCurrentUser;

	//�ؼ���Ϣ
	if(pAddScore->wCompareState==FALSE && (!IsLookonMode()) && wMeChairID==m_wCurrentUser)
	{
		UpdataControl();
	}

	//����ʱ��
	if(pAddScore->wCompareState==FALSE)
	{//if(IsLookonMode() || wMeChairID!=m_wCurrentUser)
		SetGameTimer(m_wCurrentUser,IDI_USER_ADD_SCORE,TIME_USER_ADD_SCORE);
	}

	return true;
}

//�û�����
bool CGameClientDlg::OnSubLookCard(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_LookCard)) return false;
	CMD_S_LookCard * pLookCard=(CMD_S_LookCard *)pBuffer;

	//���¶�ʱ
	if(IsLookonMode() || m_wCurrentUser!=GetMeChairID())
	{
		//ɾ����ʱ
		KillGameTimer(IDI_USER_ADD_SCORE);
		SetGameTimer(m_wCurrentUser,IDI_USER_ADD_SCORE,TIME_USER_ADD_SCORE);
	}
	else
	{
		//�����˿�
		CopyMemory(m_cbHandCardData[GetMeChairID()],pLookCard->cbCardData,sizeof(pLookCard->cbCardData));
		m_GameClientView.m_CardControl[m_wViewChairID[GetMeChairID()]].SetCardData(m_cbHandCardData[GetMeChairID()],MAX_COUNT);
		m_GameClientView.m_CardControl[m_wViewChairID[GetMeChairID()]].SetDisplayHead(true);
	}

	//���ý���
	WORD wId=pLookCard->wLookCardUser;
	if(GetMeChairID()!=wId || IsLookonMode())m_GameClientView.m_CardControl[m_wViewChairID[wId]].SetLookCard(true);

	//�����Թ�
	if(IsLookonMode() && IsAllowLookon() && m_GameClientView.m_CardControl[m_wViewChairID[GetMeChairID()]].GetLookCard())
	{
		m_GameClientView.m_CardControl[m_wViewChairID[GetMeChairID()]].SetDisplayHead(true);
	}

	//��������
	if ((IsLookonMode()==true)||(wId!=GetMeChairID())) PlayGameSound(AfxGetInstanceHandle(),TEXT("CENTER_SEND_CARD"));

	return true;
}

//�û�����
bool CGameClientDlg::OnSubCompareCard(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_CompareCard)) return false;
	CMD_S_CompareCard * pCompareCard=(CMD_S_CompareCard *)pBuffer;

	//ɾ��ʱ��
	KillGameTimer(IDI_DISABLE);

	//ȡ���ȴ�
	m_GameClientView. SetWaitUserChoice(INVALID_CHAIR);

	//�����û�
	m_wLostUser=pCompareCard->wLostUser;
	for(WORD i=0;i<GAME_PLAYER;i++)
	{
		if(m_wLostUserID[i]==INVALID_CHAIR)			
		{
			m_wLostUserID[i] = m_wLostUser;
			break;
		}
	}

	//���ñ���
	m_cbPlayStatus[m_wLostUser]=FALSE;

	//״̬����
	if ((IsLookonMode()==false)&&(m_wLostUser==GetMeChairID())) SetGameStatus(GS_FREE);

	//��ǰ�û�
	m_wCurrentUser=pCompareCard->wCurrentUser;

	//ȡ�����Ʊ���
	WORD wViewChair1=m_wViewChairID[pCompareCard->wCompareUser[0]];
	WORD wViewChair2=m_wViewChairID[pCompareCard->wCompareUser[1]];
	m_GameClientView.m_CardControl[wViewChair1].SetCompareBack(false);
	m_GameClientView.m_CardControl[wViewChair2].SetCompareBack(false);

	//��Ϸ����
	if(m_wCurrentUser==INVALID_CHAIR)m_GameClientView.StopUpdataScore(true);

	//���˶���Ч��
	//OnFlashCardFinish(0,0);

	//���ؿؼ�
	ScoreControl(SW_HIDE);
	WORD wFalshUser[]={wViewChair1,wViewChair2};

	//����Ч��
	m_GameClientView.PerformCompareCard(wFalshUser,m_wViewChairID[m_wLostUser]);

	return true;
}

//�û�����
bool CGameClientDlg::OnSubOpenCard(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_OpenCard)) return false;
	CMD_S_OpenCard* pOpenCard=(CMD_S_OpenCard *)pBuffer;

	//ȡ���ȴ�
	m_GameClientView.SetWaitUserChoice(INVALID_CHAIR);

	//ʤ���û�
	m_wWinnerUser=pOpenCard->wWinner;

	//���ؿؼ�
	ScoreControl(SW_HIDE);
	m_GameClientView.m_btOpenCard.ShowWindow(SW_HIDE);

	//��Ϸ����
	m_GameClientView.StopUpdataScore(true);

	//���˶���Ч��
	//OnFlashCardFinish(0,0);

	//����Ч��
	WORD wFalshUser[GAME_PLAYER];
	for (LONG i=0;i<GAME_PLAYER;i++)if(m_cbPlayStatus[i]==TRUE)wFalshUser[i]=m_wViewChairID[i];
	m_GameClientView.bFalshCard(wFalshUser);

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

	return true;
}

//��Ϸ����
bool CGameClientDlg::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//Ч�����
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;
	WORD wMeChiar=GetMeChairID();

	//ɾ����ʱ��
	KillGameTimer(IDI_USER_ADD_SCORE);

	//״̬����
	m_GameClientView.SetWaitUserChoice(INVALID_CHAIR);
	SetGameStatus(GS_FREE);

	//����ؼ�
	AddScoreControl(SW_HIDE);
	ScoreControl(SW_HIDE);
	m_GameClientView.m_btOpenCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btAddScore.EnableWindow(FALSE);
	m_GameClientView.m_btCompareCard.EnableWindow(FALSE);
	m_GameClientView.m_btGiveUp.EnableWindow(FALSE);
	m_GameClientView.m_btLookCard.EnableWindow(FALSE);
	m_GameClientView.m_btFollow.EnableWindow(FALSE);
	m_GameClientView.m_GoldControl.SetGoldCount(0);

	//��������
	m_GameClientView.m_ScoreView.ResetScore();
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//������Ϣ
		if (pGameEnd->lGameScore[i]!=0L)
		{
			if(pGameEnd->lGameScore[i]>0L) m_GameClientView.m_ScoreView.SetGameTax(pGameEnd->lGameTax,i);
			if (m_szAccounts[i][0]==0) m_GameClientView.m_ScoreView.SetGameScore(i,TEXT("���뿪"),pGameEnd->lGameScore[i]);
			else m_GameClientView.m_ScoreView.SetGameScore(i,m_szAccounts[i],pGameEnd->lGameScore[i]);
		}
	}

	m_GameClientView.m_ScoreView.ShowWindow(SW_SHOW);

	//����Ч��
	m_GameClientView.StopUpdataScore(false);

	//�����뱻�����û�������������
	for (WORD j=0;j<4;j++)
	{
		WORD wUserID=pGameEnd->wCompareUser[wMeChiar][j];
		if(wUserID==INVALID_CHAIR)continue;

		WORD wViewChiar=m_wViewChairID[wUserID];
		m_GameClientView.m_CardControl[wViewChiar].SetDisplayHead(true);
		m_GameClientView.m_CardControl[wViewChiar].SetLookCard(false);
		m_GameClientView.m_CardControl[wViewChiar].SetCardColor(INVALID_CHAIR);
		m_GameClientView.m_CardControl[wViewChiar].SetCardData(pGameEnd->cbCardData[wUserID],MAX_COUNT);
	}

	//�Լ��˿�
	if(pGameEnd->wCompareUser[wMeChiar][0]!=INVALID_CHAIR || m_bMingZhu[wMeChiar])
	{
		WORD wViewChiar=m_wViewChairID[wMeChiar];
		m_GameClientView.m_CardControl[wViewChiar].SetCardData(pGameEnd->cbCardData[wMeChiar],MAX_COUNT);
		m_GameClientView.m_CardControl[wViewChiar].SetCardColor(INVALID_CHAIR);
		m_GameClientView.m_CardControl[wViewChiar].SetDisplayHead(true);
	}

	//���ƽ���
	if(pGameEnd->wEndState==1)
	{
		//���б����û�
		if(m_cbPlayStatus[wMeChiar]==TRUE)
		{
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				if(m_cbPlayStatus[i]==TRUE)
				{
					WORD wViewChiar=m_wViewChairID[i];
					m_GameClientView.m_CardControl[wViewChiar].SetLookCard(false);
					m_GameClientView.m_CardControl[wViewChiar].SetCardColor(INVALID_CHAIR);
					m_GameClientView.m_CardControl[wViewChiar].SetDisplayHead(true);
					m_GameClientView.m_CardControl[wViewChiar].SetCardData(pGameEnd->cbCardData[i],MAX_COUNT);
				}
			}
		}
	}

	//��������
	if (IsLookonMode()==false)
	{
		if (pGameEnd->lGameScore[GetMeChairID()]>0L) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WIN"));
		else PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_LOST"));
	}
	else PlayGameSound(GetModuleHandle(NULL),TEXT("GAME_END"));

	//��ʼ��ť
	if (IsLookonMode()==false)
	{
		m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
		SetGameTimer(GetMeChairID(),IDI_START_GAME,TIME_START_GAME);
	}

	//�ɼ���ʾ�ڼ�ʱ����Ի���
	TCHAR szBuffer[64];
	_snprintf(szBuffer,CountArray(szBuffer),TEXT("\n���ֽ���,�ɼ�ͳ��:"));
	InsertGeneralString(szBuffer,RGB(0,0,255),true);
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��������
		if (m_szAccounts[i][0]!=0)
		{
			_snprintf(szBuffer,CountArray(szBuffer),TEXT("%s��%+ld"),m_szAccounts[i],pGameEnd->lGameScore[i]);
			InsertGeneralString(szBuffer,RGB(0,123,183),true);
			//_snprintf(szBuffer,CountArray(szBuffer),TEXT("�˿����ݣ�%d,%d,%d;��ɫ���ݣ�%d,%d,%d "),
			//	pGameEnd->cbCardData[i][0]&0x0f,pGameEnd->cbCardData[i][1]&0x0f,pGameEnd->cbCardData[i][2]&0x0f,
			//	(pGameEnd->cbCardData[i][0]&0xf0)>>4,(pGameEnd->cbCardData[i][1]&0xf0)>>4,
			//	(pGameEnd->cbCardData[i][2]&0xf0)>>4);
			//InsertGeneralString(szBuffer,RGB(0,123,183),true);
		}
	}

	return true;
}

//�������
void CGameClientDlg::ScoreControl(BOOL bShow)
{
	m_GameClientView.m_btAddScore.ShowWindow(bShow);
	m_GameClientView.m_btCompareCard.ShowWindow(bShow);
	m_GameClientView.m_btGiveUp.ShowWindow(bShow);
	m_GameClientView.m_btLookCard.ShowWindow(bShow);
	m_GameClientView.m_btFollow.ShowWindow(bShow);

	return ;
}

//��ע����
void CGameClientDlg::AddScoreControl(BOOL bShow)
{
	m_GameClientView.m_btMinScore.ShowWindow(bShow);
	m_GameClientView.m_btMaxScore.ShowWindow(bShow);
	m_GameClientView.m_btCancel.ShowWindow(bShow);
	m_GameClientView.m_btConfirm.ShowWindow(bShow);
	m_GameClientView.m_GoldControl.ShowWindow(bShow);

	return;
}

//�������
void CGameClientDlg::UpdataControl()
{
	ScoreControl(SW_SHOW);
	ActiveGameFrame();

	//������ť
	WORD wMeChairID = GetMeChairID();
	m_GameClientView.m_btGiveUp.EnableWindow(TRUE);

	//���ư�ť
	m_GameClientView.m_btLookCard.EnableWindow((m_bMingZhu[wMeChairID])?FALSE:TRUE);

	//�жϿ���
	LONG lTemp=(m_bMingZhu[wMeChairID])?6:5;
	if((m_lUserMaxScore-m_lTableScore[wMeChairID]) >= (m_lMaxCellScore*lTemp))
	{
		//�����ϼ�
		for (LONG j=wMeChairID-1;;j--)
		{
			if(j==-1)j=GAME_PLAYER-1;
			if(m_cbPlayStatus[j]==TRUE)break;
		}

		//��ע��ť
		m_GameClientView.m_btFollow.EnableWindow((m_lTableScore[j]==m_lCellScore)?FALSE:TRUE);

		//��ע��ť
		m_GameClientView.m_btAddScore.EnableWindow(((m_lCurrentTimes*m_lCellScore) < m_lMaxCellScore)?TRUE:FALSE);

		//���ư�ť
		if(wMeChairID==m_wBankerUser || m_lTableScore[wMeChairID]>=2*m_lCellScore)
		{
			m_GameClientView.m_btCompareCard.EnableWindow(TRUE);
		}
		else m_GameClientView.m_btCompareCard.EnableWindow(FALSE);
	}
	else 
	{
		m_GameClientView.m_btFollow.EnableWindow(FALSE);
		m_GameClientView.m_btAddScore.EnableWindow(FALSE);
		m_GameClientView.m_btCompareCard.EnableWindow(FALSE);
		m_GameClientView.m_btOpenCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btCompareCard.ShowWindow(SW_HIDE);
	}

	return;
}

//��ʼ��ť
LRESULT	CGameClientDlg::OnStart(WPARAM wParam, LPARAM lParam)
{
	//ɾ����ʱ��
	KillGameTimer(IDI_START_GAME);

	//���ý���
	m_wBankerUser=INVALID_CHAIR;
	//m_GameClientView.DisplayType(false);
	m_GameClientView.SetBankerUser(INVALID_CHAIR);
	m_GameClientView.SetScoreInfo(0,0);
	m_GameClientView.m_btStart.ShowWindow(SW_HIDE);
	m_GameClientView.m_ScoreView.ShowWindow(SW_HIDE);
	m_GameClientView.SetUserTableScore(INVALID_CHAIR,0L,0L);

	for (WORD i=0;i<GAME_PLAYER;i++) 
	{
		m_bMingZhu[i]=false;
		m_GameClientView.m_CardControl[i].SetCardData(NULL,0);
		m_GameClientView.m_CardControl[i].SetDisplayHead(false);
		m_GameClientView.m_CardControl[i].SetCardColor(INVALID_CHAIR);
		m_GameClientView.m_CardControl[i].SetLookCard(false);
		m_GameClientView.m_CardControl[i].SetCompareCard(false);
	}

	//״̬����
	m_wCurrentUser=INVALID_CHAIR;
	m_wBankerUser=INVALID_CHAIR;
	ZeroMemory(m_szAccounts,sizeof(m_szAccounts));
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));

	//��ע��Ϣ
	m_lMaxCellScore=0L;
	m_lCellScore=0L;

	//������Ϣ
	SendUserReady(NULL,0);

	return 0;
}

//��ע��ť 
LRESULT	CGameClientDlg::OnAddScore(WPARAM wParam, LPARAM lParam)
{
	ASSERT(m_lCellScore>0L);

	//����ؼ�
	m_GameClientView.m_GoldControl.SetMaxGold(m_lMaxCellScore);
	m_GameClientView.m_GoldControl.SetMinGold(m_lCellScore*m_lCurrentTimes);
	m_GameClientView.m_GoldControl.SetGold(m_lCellScore*m_lCurrentTimes);
	AddScoreControl(SW_SHOW);
	ScoreControl(SW_HIDE);

	return 0;
}

//ȡ����Ϣ
LRESULT CGameClientDlg::OnCancelAdd(WPARAM wParam, LPARAM lParam)
{
	//����ؼ�
	AddScoreControl(SW_HIDE);
	ScoreControl(SW_SHOW);

	return 0;
}

//���ټ�ע
LRESULT	CGameClientDlg::OnMinScore(WPARAM wParam, LPARAM lParam)
{
#ifdef _DEBUG
	////����ʱ��
	//BYTE bTemp[]={1,2,3};
	//for (WORD j=0;j<GAME_PLAYER;j++)
	//{
	//	m_GameClientView.m_CardControl[j].SetCardData(bTemp,3);
	//	m_GameClientView.m_CardControl[j].SetDisplayHead(true);
	//}

	//WORD wFalshUser[GAME_PLAYER];
	//for (LONG i=0;i<GAME_PLAYER;i++)wFalshUser[i]=i;
	//m_GameClientView.bFalshCard(wFalshUser);

	//WORD wFalshUser1[]={1,2};

	//����Ч��
	//m_GameClientView.PerformCompareCard(wFalshUser1,1);

	////�ɷ��˿�
	//for(WORD i=0;i<MAX_COUNT;i++)
	//{
	//	for (WORD j=0;j<GAME_PLAYER;j++)
	//	{
	//		m_GameClientView.DispatchUserCard(j,0);
	//	}
	//}
	//return 0;
#endif

	ASSERT(m_lCellScore>0L);
	m_GameClientView.m_GoldControl.SetGold(m_lCellScore*m_lCurrentTimes);

	return 0;
}

//����ע
LRESULT	CGameClientDlg::OnMaxScore(WPARAM wParam, LPARAM lParam)
{
	if(m_lMaxCellScore>0)m_GameClientView.m_GoldControl.SetGold(m_lMaxCellScore);

	return 0;
}

//��ע��Ϣ && ��ע��Ϣ
LRESULT CGameClientDlg::OnConfirmScore(WPARAM wParam, LPARAM lParam)
{
	//һΪ��ע
	WORD wTemp=LOWORD(wParam);

	//Ч������
	ASSERT(wTemp==1 || m_GameClientView.m_GoldControl.GetGold() >= (m_lCellScore*m_lCurrentTimes)
		&& m_GameClientView.m_GoldControl.GetGold() <= m_lMaxCellScore);

	//ɾ��ʱ��
	KillGameTimer(IDI_USER_ADD_SCORE);

	//��ȡ����
	LONG lCurrentScore=(wTemp==0)?(m_GameClientView.m_GoldControl.GetGold()):(m_lCellScore*m_lCurrentTimes);
	WORD wMeChairID=GetMeChairID();
 
	//��ע�ӱ�
	if(m_bMingZhu[wMeChairID])lCurrentScore*=2;

	//Ԥ�ȴ���
	m_lTableScore[wMeChairID]+=lCurrentScore;
	m_GameClientView.SetUserTableScore(m_wViewChairID[wMeChairID],m_lTableScore[wMeChairID],lCurrentScore);

	//����ť
	AddScoreControl(SW_HIDE);

	m_GameClientView.m_btOpenCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btAddScore.SetButtonImage(IDB_USERCONTROL_INVEST,AfxGetInstanceHandle(),false);
	ScoreControl(SW_HIDE);
	m_GameClientView.m_btOpenCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btAddScore.EnableWindow(FALSE);
	m_GameClientView.m_btCompareCard.EnableWindow(FALSE);
	m_GameClientView.m_btGiveUp.EnableWindow(FALSE);
	m_GameClientView.m_btLookCard.EnableWindow(FALSE);
	m_GameClientView.m_btFollow.EnableWindow(FALSE);

	//��������
	m_GameClientView.SetUserTableScore(m_wViewChairID[wMeChairID],m_lTableScore[wMeChairID],lCurrentScore);
	m_GameClientView.UpdateGameView(NULL);
	PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_SCORE"));

	//������Ϣ
	CMD_C_AddScore AddScore;
	AddScore.wState=0;
	AddScore.lScore=lCurrentScore;
	SendData(SUB_C_ADD_SCORE,&AddScore,sizeof(AddScore));

	return 0;
}

//������Ϣ
LRESULT	CGameClientDlg::OnLookCard(WPARAM wParam, LPARAM lParam)
{
	//���ÿؼ�
	WORD wMeChairID=GetMeChairID();
	KillGameTimer(IDI_USER_ADD_SCORE);

	//���¶�ʱ
	SetGameTimer(wMeChairID,IDI_USER_ADD_SCORE,TIME_USER_ADD_SCORE);

	//��������
	m_bMingZhu[wMeChairID] = true;

	//������Ϣ
	SendData(SUB_C_LOOK_CARD,NULL,0);
	
	//�ؼ���Ϣ
	UpdataControl();

	//��������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("CENTER_SEND_CARD"));

	return 0;
}

//������Ϣ
LRESULT	CGameClientDlg::OnCompareCard(WPARAM wParam, LPARAM lParam)
{
	//ɾ����ʱ��
	KillGameTimer(IDI_USER_ADD_SCORE);

	//����ؼ�
	AddScoreControl(SW_HIDE);

	m_GameClientView.m_btAddScore.EnableWindow(FALSE);
	m_GameClientView.m_btCompareCard.EnableWindow(FALSE);
	m_GameClientView.m_btGiveUp.EnableWindow(FALSE);
	m_GameClientView.m_btLookCard.EnableWindow(FALSE);
	m_GameClientView.m_btFollow.EnableWindow(FALSE);

	//�ж���ע
	WORD wMeChairID=GetMeChairID();
	LONG lCurrentScore=(m_bMingZhu[wMeChairID])?(m_lCurrentTimes*m_lCellScore*4):(m_lCurrentTimes*m_lCellScore*2);
	m_lTableScore[wMeChairID]+=lCurrentScore;
	m_GameClientView.SetUserTableScore(m_wViewChairID[wMeChairID],m_lTableScore[wMeChairID],lCurrentScore);

	//������Ϣ
	CMD_C_AddScore AddScore;
	AddScore.wState=TRUE;
	AddScore.lScore=lCurrentScore;
	SendData(SUB_C_ADD_SCORE,&AddScore,sizeof(AddScore));

	//��ǰ����
	BYTE UserCount=0;
	for (BYTE i=0;i<GAME_PLAYER;i++)
	{
		if(m_cbPlayStatus[i]==TRUE)UserCount++;
	}

	//ׯ���ڵ�һ��û��עֻ�ܸ��ϼұ��� �� ֻʣ������
	if(m_wBankerUser==wMeChairID && (m_lTableScore[wMeChairID]-lCurrentScore)==m_lCellScore || UserCount==2)
	{
		//�������
		CMD_C_CompareCard CompareCard;

		//�����ϼ�
		for (LONG i=(LONG)wMeChairID-1;;i--)
		{
			if(i==-1)i=GAME_PLAYER-1;
			if(m_cbPlayStatus[i]==TRUE)
			{
				CompareCard.wCompareUser=(WORD)i;
				break;
			}
		}
		//������Ϣ
		SendData(SUB_C_COMPARE_CARD,&CompareCard,sizeof(CompareCard));
	}
	else	//ѡ����ұ���
	{
		BOOL bCompareUser[GAME_PLAYER];
		for(int i=0;i<GAME_PLAYER;i++)
		{
			if(m_cbPlayStatus[i]==TRUE && wMeChairID!=i)
			{
				bCompareUser[m_wViewChairID[i]]=TRUE;
			}
			else bCompareUser[m_wViewChairID[i]]=FALSE;
		}

		//���ü�ͷ
		m_GameClientView.SetCompareCard(true,bCompareUser);

		//���͵ȴ�"����"��Ϣ
		SendData(SUB_C_WAIT_COMPARE,NULL,0);

		//����ʱ��
		SetGameTimer(wMeChairID,IDI_USER_COMPARE_CARD,TIME_USER_COMPARE_CARD);

		//��ʾ��־
		m_GameClientView.SetWaitUserChoice(FALSE);
	}

	return 0;
}

//��ѡ�����û�
LRESULT CGameClientDlg::OnChooseCUser(WPARAM wParam, LPARAM lParam)
{
	//Ч������
	WORD wTemp=LOWORD(wParam);
	WORD wChairID=INVALID_CHAIR;
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if(m_wViewChairID[i]==wTemp)
		{
			wChairID=i;
			break;
		}
	}
	ASSERT(wChairID!=INVALID_CHAIR && m_cbPlayStatus[wChairID]==TRUE && wChairID!=GetMeChairID());

	//ɾ����ʱ��
	KillGameTimer(IDI_USER_COMPARE_CARD);

	//�������
	m_GameClientView.SetCompareCard(false,NULL);

	//�������
	CMD_C_CompareCard CompareCard;
	ZeroMemory(&CompareCard,sizeof(CompareCard));
	CompareCard.wCompareUser=wChairID;

	//������Ϣ
	SendData(SUB_C_COMPARE_CARD,&CompareCard,sizeof(CompareCard));

	return 0;
}

//������Ϣ
LRESULT	CGameClientDlg::OnOpenCard(WPARAM wParam, LPARAM lParam)
{
	//ɾ����ʱ��
	KillGameTimer(IDI_USER_ADD_SCORE);

	//����ؼ�
	m_GameClientView.m_btGiveUp.EnableWindow(FALSE);
	m_GameClientView.m_btLookCard.EnableWindow(FALSE);
	//m_GameClientView.m_btOpenCard.EnableWindow(FALSE);
	m_GameClientView.m_btOpenCard.ShowWindow(SW_HIDE);

	//��������
	WORD bUserCount =0;
	for(WORD i=0;i<GAME_PLAYER;i++)
	{
		if(m_cbPlayStatus[i]==TRUE) bUserCount++;
	}

	//���˱���
	if(bUserCount==2)
	{
		OnCompareCard(0,0);
		return 0;
	}

	//�ж���ע
	WORD wMeChairID=GetMeChairID();
	LONG lCurrentScore=(m_bMingZhu[wMeChairID])?(m_lCurrentTimes*m_lCellScore*4):(m_lCurrentTimes*m_lCellScore*2);
	m_lTableScore[wMeChairID]+=lCurrentScore;
	m_GameClientView.SetUserTableScore(m_wViewChairID[wMeChairID],m_lTableScore[wMeChairID],lCurrentScore);

	//������Ϣ
	CMD_C_AddScore AddScore;
	AddScore.wState=2;
	AddScore.lScore=lCurrentScore;
	SendData(SUB_C_ADD_SCORE,&AddScore,sizeof(AddScore));

	//������Ϣ
	SendData(SUB_C_OPEN_CARD,NULL,0);

	return 0;
}

//������Ϣ
LRESULT	CGameClientDlg::OnGiveUp(WPARAM wParam, LPARAM lParam)
{
	//ɾ��ʱ��
	KillGameTimer(IDI_USER_ADD_SCORE);
	PlayGameSound(AfxGetInstanceHandle(),TEXT("GIVE_UP"));

	//����ť
	AddScoreControl(SW_HIDE);

	m_GameClientView.m_btOpenCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btAddScore.EnableWindow(FALSE);
	m_GameClientView.m_btCompareCard.EnableWindow(FALSE);
	m_GameClientView.m_btGiveUp.EnableWindow(FALSE);
	m_GameClientView.m_btLookCard.EnableWindow(FALSE);
	m_GameClientView.m_btFollow.EnableWindow(FALSE);

	//�˿˱��
	m_GameClientView.m_CardControl[m_wViewChairID[GetMeChairID()]].SetCardColor(1);

	//������Ϣ
	SendData(SUB_C_GIVE_UP,NULL,0);

	return 0;
}

//�������
LRESULT CGameClientDlg::OnSendCardFinish(WPARAM wParam, LPARAM lParam)
{
	//����ʱ��
	if(m_wCurrentUser==INVALID_CHAIR)return 0;
	//for(int i=0;i<GAME_PLAYER;i++)
	//{
	//	if(m_lTableScore[i]>0L)
	//		m_GameClientView.SetUserTableScore(m_wViewChairID[i],m_lTableScore[i],m_lTableScore[i]);
	//}
	SetGameTimer(m_wCurrentUser,IDI_USER_ADD_SCORE,TIME_USER_ADD_SCORE);

	//�����û�
	WORD wMeChairID=GetMeChairID();
	if (IsLookonMode() || wMeChairID!=m_wCurrentUser)return 0;

	//���ÿؼ�
	ActiveGameFrame();
	UpdataControl();
	//m_GameClientView.m_btAddScore.SetButtonImage(IDB_INVEST,AfxGetInstanceHandle(),false);
	//m_GameClientView.m_btAddScore.EnableWindow(TRUE);
	//m_GameClientView.m_btGiveUp.EnableWindow(TRUE);
	//m_GameClientView.m_btLookCard.EnableWindow(TRUE);

	return 0;
}

//�������
LRESULT CGameClientDlg::OnFlashCardFinish(WPARAM wParam, LPARAM lParam)
{
	//���ƽ���
	if(m_wWinnerUser>=GAME_PLAYER)
	{
		if(m_wLostUser>GAME_PLAYER)return 0;
		ASSERT(m_wLostUser<GAME_PLAYER);

		//�����û�
		WORD wLostUser=m_wLostUser;
		WORD wMeChairID=GetMeChairID();

		//��������
		WORD wViewChairID=m_wViewChairID[wLostUser];
		WORD wMeViewChairID=m_wViewChairID[wMeChairID];

		//�����˿�
		m_GameClientView.m_CardControl[wMeViewChairID].SetCardData(m_cbHandCardData[wMeChairID],MAX_COUNT);

		//��������
		if ((!IsLookonMode()) && wLostUser==wMeChairID) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_LOST"));

		//�������
		BYTE bCount=0;
		for (WORD i=0;i<GAME_PLAYER;i++)if(m_cbPlayStatus[i]==TRUE)bCount++;

		//�жϽ���
		if(bCount>1)
		{
			//�ؼ���Ϣ
			if((!IsLookonMode()) && wMeChairID==m_wCurrentUser)
			{
				UpdataControl();
			}

			//����ʱ��
			SetGameTimer(m_wCurrentUser,IDI_USER_ADD_SCORE,TIME_USER_ADD_SCORE);
		}
		else 
		{
			for(WORD i=0;i<GAME_PLAYER;i++)
			{
				if(m_cbPlayStatus[i]==TRUE)break;
			}
			WORD wMeChairID = GetMeChairID();
			if(i==wMeChairID || wMeChairID==m_wLostUser)
			{
				//������Ϣ
				SendData(SUB_C_FINISH_FLASH,NULL,0);
			}
		}
	}
	else			//���ƽ���
	{
		//ASSERT(m_wWinnerUser<GAME_PLAYER);
		if(m_wWinnerUser>GAME_PLAYER)return 0;		

		//����ʧ���û�
		for (int i=0;i<GAME_PLAYER;i++)
		{
			if(m_cbPlayStatus[i]==TRUE && i!=m_wWinnerUser)		
			{
				//�˿˱���
				WORD wViewChairID=m_wViewChairID[i];
				m_GameClientView.m_CardControl[wViewChairID].SetCardColor(2);

				//״̬����
				if ((!IsLookonMode())&&(i==GetMeChairID())) 
				{
					SetGameStatus(GS_FREE);
					//��������
					PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_LOST"));
				}
			}
		}
		m_wWinnerUser=INVALID_CHAIR;

		//������Ϣ
		SendData(SUB_C_FINISH_FLASH,NULL,0);
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
