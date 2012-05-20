#include "Stdafx.h"
#include "GameOption.h"
#include "GameClient.h"
#include "GameClientDlg.h"

//////////////////////////////////////////////////////////////////////////
//�궨��

//��Ϸ��ʱ��
#define IDI_OUT_CARD					200								//���ƶ�ʱ��
#define IDI_MOST_CARD					201								//���ʱ��
#define IDI_START_GAME					202								//��ʼ��ʱ��
#define IDI_LAND_SCORE					203								//�зֶ�ʱ��

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientDlg, CGameFrameDlg)
	ON_WM_TIMER()
	ON_MESSAGE(IDM_START,OnStart)
	ON_MESSAGE(IDM_OUT_CARD,OnOutCard)
	ON_MESSAGE(IDM_PASS_CARD,OnPassCard)
	ON_MESSAGE(IDM_LAND_SCORE,OnLandScore)
	ON_MESSAGE(IDM_LEFT_HIT_CARD,OnLeftHitCard)
	ON_MESSAGE(IDM_RIGHT_HIT_CARD,OnRightHitCard)
	ON_MESSAGE(IDM_SORT_BY_NUM,OnSorByNumCard)
	ON_MESSAGE(IDM_SORT_BY_SIZE,OnSorBySizeCard)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientDlg::CGameClientDlg() : CGameFrameDlg(&m_GameClientView)
{
	//��Ϸ����
	m_bOutBombCount=0;
	m_bMaxBombCount=0;
	m_wLandUser=INVALID_CHAIR;
	memset(m_bCardCount,0,sizeof(m_bCardCount));
	memset(m_bHandCardData,0,sizeof(m_bHandCardData));

	//���ñ���
	m_bDeasilOrder=false;
	m_dwCardHSpace=DEFAULT_PELS;

	//���Ʊ���
	m_bTurnCardCount=0;
	m_bTurnOutType=CT_ERROR;
	memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));

	//��������
	m_wTimeOutCount=0;
	m_wMostUser=INVALID_CHAIR;

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
	SetWindowText(TEXT("����һ��Ϸ  --  Ver��6.5.1.0"));

	//����ͼ��
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);

	//��ȡ����
	m_dwCardHSpace=AfxGetApp()->GetProfileInt(TEXT("GameOption"),TEXT("CardSpace"),DEFAULT_PELS);
	m_bDeasilOrder=AfxGetApp()->GetProfileInt(TEXT("GameOption"),TEXT("DeasilOrder"),FALSE)?true:false;

	//��������
	if ((m_dwCardHSpace>MAX_PELS)||(m_dwCardHSpace<LESS_PELS)) m_dwCardHSpace=DEFAULT_PELS;

	//���ÿؼ�
	m_GameClientView.SetUserOrder(m_bDeasilOrder);
	m_GameClientView.m_HandCardControl[2].SetCardSpace(m_dwCardHSpace,0,20);

	return true;
}

//���ÿ��
void CGameClientDlg::ResetGameFrame()
{
	//��Ϸ����
	m_bOutBombCount=0;
	m_bMaxBombCount=0;
	m_wLandUser=INVALID_CHAIR;
	memset(m_bCardCount,0,sizeof(m_bCardCount));
	memset(m_bHandCardData,0,sizeof(m_bHandCardData));

	//���Ʊ���
	m_bTurnCardCount=0;
	m_bTurnOutType=CT_ERROR;
	memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));

	//��������
	m_wTimeOutCount=0;
	m_wMostUser=INVALID_CHAIR;

	//ɾ����ʱ
	KillGameTimer(0);
	KillTimer(IDI_MOST_CARD);

	return;
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
			if (nElapse==0)
			{
				if ((IsLookonMode()==false)&&(wChairID==GetMeChairID())) AutomatismOutCard();
				return false;
			}

			//��������
			WORD wMeChairID=GetMeChairID();
			if (m_bCardCount[wMeChairID]<m_bTurnCardCount) return true;
			if ((nElapse<=10)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false)) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));

			return true;
		}
	case IDI_START_GAME:		//��ʼ��Ϸ
		{
			if (nElapse==0)
			{
				if ((IsLookonMode()==false)&&(wChairID==GetMeChairID())) OnStart(0,0);
				return false;
			}
			if ((nElapse<=10)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false)) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));

			return true;
		}
	case IDI_LAND_SCORE:		//�ڿӽз�
		{
			if (nElapse==0)
			{
				if ((IsLookonMode()==false)&&(wChairID==GetMeChairID()))
				{
					if (m_GameClientView.m_btGiveUpScore.IsWindowVisible())
					{
						OnLandScore(255,255);
					}
					else
					{
						OnLandScore(1,1);
					}
				}
				return false;
			}
			if ((nElapse<=10)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false)) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));

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
	case SUB_S_SEND_CARD:		//�����˿�
		{
			return OnSubSendCard(pBuffer,wDataSize);
		}
	case SUB_S_LAND_SCORE:	//�û��з�
		{
			return OnSubLandScore(pBuffer,wDataSize);
		}
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
	}

	return false;
}

//��Ϸ����
bool CGameClientDlg::OnGameSceneMessage(BYTE cbGameStation, bool bLookonOther, const void * pBuffer, WORD wDataSize)
{
	switch (cbGameStation)
	{
	case GS_WK_FREE:	//����״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pBuffer;

			//���ý���
			m_GameClientView.SetBaseScore(pStatusFree->lBaseScore);

			//���ÿؼ�
			if (IsLookonMode()==false)
			{
				m_GameClientView.m_btStart.ShowWindow(TRUE);
				m_GameClientView.m_btStart.SetFocus();
			}

			//�����˿�
			if (IsLookonMode()==false) m_GameClientView.m_HandCardControl[2].SetDisplayFlag(true);

			return true;
		}
	case GS_WK_SCORE:	//�з�״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusScore)) return false;
			CMD_S_StatusScore * pStatusScore=(CMD_S_StatusScore *)pBuffer;
			WORD wMeChairID=GetMeChairID();
			//���ñ���
			for (WORD i=0;i<GAME_PLAYER;i++) {
				m_GameClientView.m_bCardCount[SwitchViewChairID(i)] = m_bCardCount[i]=25;
			}
			CopyMemory(m_bHandCardData[wMeChairID],pStatusScore->bCardData,25);

			//���ý���
			for (WORD i=0;i<GAME_PLAYER;i++)	
			{
				WORD wViewChairID=SwitchViewChairID(i);
				m_GameClientView.SetCardCount(wViewChairID,m_bCardCount[i]);
				m_GameClientView.SetLandScore(wViewChairID,pStatusScore->bScoreInfo[i]);
			}
			m_GameClientView.ShowLandTitle(true);
			m_GameClientView.SetBaseScore(pStatusScore->lBaseScore);

			//��ť����
			if ((IsLookonMode()==false)&&(pStatusScore->wCurrentUser==GetMeChairID()))
			{
				m_GameClientView.m_btGiveUpScore.ShowWindow(SW_SHOW);
				m_GameClientView.m_btOneScore.EnableWindow(pStatusScore->bLandScore==0?true:false);
				m_GameClientView.m_btTwoScore.EnableWindow(pStatusScore->bLandScore<=1?true:false);
				m_GameClientView.m_btThreeScore.EnableWindow(pStatusScore->bLandScore<=2?true:false);
				m_GameClientView.m_btShowCard.EnableWindow(pStatusScore->bLandScore<=3?true:false);
			}

			//�����˿�
			//BYTE bCardData[8];

			//ZeroMemory(bCardData,sizeof(bCardData));
			m_GameClientView.m_BackCardControl.SetCardData(pStatusScore->bBackCard,8);
			m_GameClientView.m_HandCardControl[2].SetCardData(m_bHandCardData[wMeChairID],m_bCardCount[wMeChairID]);
			m_GameClientView.m_HandCardControl[2].SetDisplayFlag(true);

			//����ʱ��
			SetGameTimer(pStatusScore->wCurrentUser,IDI_LAND_SCORE,30);

			return true;
		}
	case GS_WK_PLAYING:		//��Ϸ״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pBuffer;

			m_GameClientView.m_BackCardControl.ShowWindow(false);
			//���ñ���
			WORD wMeChairID=GetMeChairID();
			m_bTurnCardCount=pStatusPlay->bTurnCardCount;
			m_bOutBombCount=(BYTE)pStatusPlay->bOutBombCount;
			m_bMaxBombCount=pStatusPlay->bMaxBombCount[wMeChairID];
			m_bTurnOutType=m_GameLogic.GetCardType(pStatusPlay->bTurnCardData,m_bTurnCardCount);
			CopyMemory(m_bHandCardData,pStatusPlay->bCardData,sizeof(m_bHandCardData));
			CopyMemory(m_bTurnCardData,pStatusPlay->bTurnCardData,pStatusPlay->bTurnCardCount);
			//CopyMemory(m_GameClientView.m_bMaxBombCount, pStatusPlay->bMaxBombCount, sizeof(m_GameClientView.m_bMaxBombCount));
			for(int i=0; i< GAME_PLAYER; i++){
				m_GameClientView.m_bMaxBombCount[SwitchViewChairID(i)] = pStatusPlay->bMaxBombCount[i];
				m_GameClientView.m_bOutBombCount[SwitchViewChairID(i)] = pStatusPlay->bOutBombCount2[i];
			}
			m_GameClientView.m_bMaxBombCountShow =true;

			//���ý���
			for (BYTE i=0;i<GAME_PLAYER;i++)
			{
				WORD wViewChairID=SwitchViewChairID(i);
				m_GameClientView.m_bCardCount[SwitchViewChairID(i)] =m_bCardCount[i]=pStatusPlay->bCardCount[i];
				m_GameClientView.SetCardCount(wViewChairID,pStatusPlay->bCardCount[i]);
			}
			m_GameClientView.SetBombTime(pStatusPlay->bOutBombCount);
			m_GameClientView.SetBaseScore(pStatusPlay->lBaseScore);
			m_GameClientView.m_BackCardControl.SetCardData(pStatusPlay->bBackCard,8);
			m_GameClientView.SetLandUser(SwitchViewChairID(pStatusPlay->wLandUser),pStatusPlay->bLandScore);

			//�������
			if ((IsLookonMode()==false)&&(pStatusPlay->wCurrentUser==GetMeChairID()))
			{
				m_GameClientView.m_btOutCard.EnableWindow(FALSE);
				m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
				m_GameClientView.m_btPassCard.ShowWindow(SW_SHOW);
				m_GameClientView.m_btPassCard.EnableWindow((m_bTurnCardCount!=0)?TRUE:FALSE);
			}

			//��ʾ����
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				WORD wViewChairID=SwitchViewChairID(i);
				if ((pStatusPlay->bLandScore==6))
				{
					m_GameClientView.m_HandCardControl[wViewChairID].SetDisplayFlag(true);
				}
				else if ((i==wMeChairID)&&(IsLookonMode()==false))
				{
					m_GameClientView.m_HandCardControl[wViewChairID].SetDisplayFlag(true);
				}
				else if ((i==wMeChairID)&&(IsLookonMode()==true)&&(IsAllowLookon()==true))
				{
					m_GameClientView.m_HandCardControl[wViewChairID].SetDisplayFlag(true);
				}
				else m_GameClientView.m_HandCardControl[wViewChairID].SetDisplayFlag(false);

				//�����˿�
				m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(m_bHandCardData[i],m_bCardCount[i]);
			}

			//��������
			if (m_bTurnCardCount!=0)
			{
				WORD wViewChairID=SwitchViewChairID(pStatusPlay->wLastOutUser);
				m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(m_bTurnCardData,m_bTurnCardCount);
			}

			//���ö�ʱ��
			SetGameTimer(pStatusPlay->wCurrentUser,IDI_OUT_CARD,30);

			//�����˿�
			//if (IsLookonMode()==false) 
			{
				m_GameClientView.m_HandCardControl[2].SetPositively(true);
				m_GameClientView.m_HandCardControl[2].SetDisplayFlag(true);
			}
			if(IsLookonMode() == false){
				m_GameClientView.m_btSortByNum.ShowWindow(SW_SHOW);
				m_GameClientView.m_btSortBySize.ShowWindow(SW_SHOW);
				m_GameClientView.m_btSortByNum.EnableWindow(TRUE);
				m_GameClientView.m_btSortBySize.EnableWindow(TRUE);
			}

			return true;
		}
	}

	return false;
}

//�����˿�
bool CGameClientDlg::OnSubSendCard(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_SendCard));
	if (wDataSize!=sizeof(CMD_S_SendCard)) return false;

	//��������
	CMD_S_SendCard * pSendCard=(CMD_S_SendCard *)pBuffer;

	ZeroMemory(m_bHandCardData,sizeof(m_bHandCardData));
	ZeroMemory(m_GameClientView.m_bOutBombCount, sizeof(m_GameClientView.m_bOutBombCount));

	//��������
	WORD wMeChairID=GetMeChairID();
	for (WORD i=0;i<GAME_PLAYER;i++) {
		m_GameClientView.m_bCardCount[SwitchViewChairID(i)] =m_bCardCount[i]=CountArray(pSendCard->bCardData);

	}
	CopyMemory(m_bHandCardData[wMeChairID],pSendCard->bCardData,sizeof(pSendCard->bCardData));

	//���ý���
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.SetLandScore(i,0);
		m_GameClientView.SetPassFlag(i,false);
		m_GameClientView.SetCardCount(i,m_bCardCount[i]);
		m_GameClientView.m_UserCardControl[i].SetCardData(NULL,0);
	}

	BYTE cbTemp[25];
	ZeroMemory(cbTemp,sizeof(cbTemp));
	m_GameClientView.SetLandUser(INVALID_CHAIR,0);
	m_GameClientView.m_ScoreView.ShowWindow(SW_HIDE);
	m_GameClientView.m_HandCardControl[0].SetCardData(cbTemp,CountArray(cbTemp));
	m_GameClientView.m_HandCardControl[1].SetCardData(cbTemp,CountArray(cbTemp));
	m_GameClientView.m_HandCardControl[3].SetCardData(cbTemp,CountArray(cbTemp));

	m_GameClientView.ShowLandTitle(true);
	m_GameClientView.SetBombTime(m_bOutBombCount);
	if ((IsLookonMode()==false)){
		m_GameClientView.m_btSortByNum.ShowWindow(SW_SHOW);
		m_GameClientView.m_btSortBySize.ShowWindow(SW_SHOW);
		m_GameClientView.m_btSortByNum.EnableWindow(TRUE);
		m_GameClientView.m_btSortBySize.EnableWindow(TRUE);
	}

	//�����˿�
	//BYTE bBackCard[8];
	//ZeroMemory(bBackCard,sizeof(bBackCard));
	m_GameClientView.m_HandCardControl[2].SetCardData(pSendCard->bCardData,25);
	m_GameClientView.m_BackCardControl.SetCardData(pSendCard->bBackCard,CountArray(pSendCard->bBackCard));
	m_GameClientView.m_BackCardControl.ShowWindow(true);
	//m_GameClientView.m_BackCardControl.SetDisplayFlag(true);
	//if (IsLookonMode()==true) m_GameClientView.m_HandCardControl[2].SetDisplayFlag(false);

	//��ǰ���
	if ((IsLookonMode()==false)&&(pSendCard->wCurrentUser==GetMeChairID()))
	{
		ActiveGameFrame();
		m_GameClientView.m_btOneScore.ShowWindow(SW_SHOW);
		m_GameClientView.m_btTwoScore.ShowWindow(SW_SHOW);
		m_GameClientView.m_btThreeScore.ShowWindow(SW_SHOW);
		m_GameClientView.m_btShowCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btGiveUpScore.ShowWindow(SW_SHOW);
		m_GameClientView.m_btGiveUpScore.EnableWindow(false);

#ifdef VIDEO_GAME
		m_GameClientView.m_btShowCard.ShowWindow(SW_HIDE);
#endif
	}

	//��������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));

	//����ʱ��
	SetGameTimer(pSendCard->wCurrentUser,IDI_LAND_SCORE,30);

	return true;
}

//�û��з�
bool CGameClientDlg::OnSubLandScore(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_LandScore));
	if (wDataSize!=sizeof(CMD_S_LandScore)) return false;

	//��Ϣ����
	CMD_S_LandScore * pLandScore=(CMD_S_LandScore *)pBuffer;

	//���ý���
	WORD wViewChairID=SwitchViewChairID(pLandScore->bLandUser);
	m_GameClientView.SetLandScore(wViewChairID,pLandScore->bLandScore);

	//�������
	if ((IsLookonMode()==false)&&(pLandScore->wCurrentUser==GetMeChairID()))
	{
		ActiveGameFrame();
		m_GameClientView.m_btGiveUpScore.ShowWindow(SW_SHOW);
		m_GameClientView.m_btOneScore.ShowWindow(SW_SHOW);
		m_GameClientView.m_btTwoScore.ShowWindow(SW_SHOW);
		m_GameClientView.m_btThreeScore.ShowWindow(SW_SHOW);
		m_GameClientView.m_btShowCard.ShowWindow(SW_SHOW);

		m_GameClientView.m_btOneScore.EnableWindow(pLandScore->bCurrentScore==0?true:false);
		m_GameClientView.m_btTwoScore.EnableWindow(pLandScore->bCurrentScore<=1?true:false);
		m_GameClientView.m_btThreeScore.EnableWindow(pLandScore->bCurrentScore<=2?true:false);
		m_GameClientView.m_btShowCard.EnableWindow(pLandScore->bCurrentScore<=3?true:false);

#ifdef VIDEO_GAME
		m_GameClientView.m_btShowCard.ShowWindow(SW_HIDE);
#endif
	}

	//��������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));

	//����ʱ��
	SetGameTimer(pLandScore->wCurrentUser,IDI_LAND_SCORE,30);

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

	m_bOutBombCount=0;
	m_bMaxBombCount=0;

	//���ñ���
	m_bTurnCardCount=0;
	m_bTurnOutType=CT_ERROR;
	m_wLandUser=pGameStart->wLandUser;
	m_GameClientView.m_bMaxBombCountShow = true;
	ZeroMemory(m_bTurnCardData,sizeof(m_bTurnCardData));
	ZeroMemory(m_GameClientView.m_bOutBombCount, sizeof(m_GameClientView.m_bOutBombCount));
	//CopyMemory(m_GameClientView.m_bMaxBombCount, pGameStart->bMaxBombCount, sizeof(m_GameClientView.m_bMaxBombCount));
	for(int i=0; i< GAME_PLAYER; i++){
		m_GameClientView.m_bMaxBombCount[SwitchViewChairID(i)] = pGameStart->bMaxBombCount[i];
	}

	//ׯ��ը��
	if (pGameStart->wLandUser==GetMeChairID()) m_bMaxBombCount=255;

	//�����˿�
	WORD wMeChairID=GetMeChairID();
	CopyMemory(m_bHandCardData,pGameStart->bCardData,sizeof(m_bHandCardData));

	m_bMaxBombCount=pGameStart->bMaxBombCount[wMeChairID];

	//���ÿؼ�
	m_GameClientView.ShowLandTitle(false);
	//m_GameClientView.m_BackCardControl.SetCardData(pGameStart->bBackCard,CountArray(pGameStart->bBackCard));
	m_GameClientView.m_BackCardControl.ShowWindow(false);



	//���ý���
	m_GameClientView.SetLandScore(INVALID_CHAIR,0);
	m_GameClientView.SetCardCount(SwitchViewChairID(pGameStart->wLandUser),m_bCardCount[pGameStart->wLandUser]);

	//��������
	m_GameClientView.m_bCardCount[SwitchViewChairID(m_wLandUser)] = m_bCardCount[m_wLandUser]=33;

	CopyMemory(m_bHandCardData,pGameStart->bCardData,sizeof(pGameStart->bCardData));

	//��ʾ����
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		WORD wViewChairID=SwitchViewChairID(i);
		if ((pGameStart->bLandScore==6))
		{
			//for(int i=0; i< GAME_PLAYER; i++){
			m_GameClientView.m_HandCardControl[i].SetDisplayFlag(true);
			//}
		}
		else if ((i==wMeChairID)&&(IsLookonMode()==false))
		{
			m_GameClientView.m_HandCardControl[wViewChairID].SetDisplayFlag(true);
		}
		else if ((i==wMeChairID)&&(IsLookonMode()==true)&&(IsAllowLookon()==true))
		{
			m_GameClientView.m_HandCardControl[wViewChairID].SetDisplayFlag(true);
		}
		else m_GameClientView.m_HandCardControl[wViewChairID].SetDisplayFlag(false);

		//�����˿�
		m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(m_bHandCardData[i],m_bCardCount[i]);
	}

	m_GameClientView.SetLandUser(SwitchViewChairID(pGameStart->wLandUser),pGameStart->bLandScore);

	//�������
	if (IsLookonMode()==false) m_GameClientView.m_HandCardControl[2].SetPositively(true);

	//��ǰ���
	if ((IsLookonMode()==false)&&(pGameStart->wCurrentUser==GetMeChairID()))
	{
		ActiveGameFrame();
		m_GameClientView.m_btOutCard.EnableWindow(FALSE);
		m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btPassCard.EnableWindow(FALSE);
		m_GameClientView.m_btPassCard.ShowWindow(SW_SHOW);
	}

	//��������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));

	//����ʱ��
	SetGameTimer(pGameStart->wCurrentUser,IDI_OUT_CARD,60);

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

	//ɾ����ʱ��
	KillTimer(IDI_MOST_CARD);
	KillGameTimer(IDI_OUT_CARD);

	//��������
	WORD wMeChairID=GetMeChairID();
	WORD wOutViewChairID=SwitchViewChairID(pOutCard->wOutCardUser);

	//��������
	if ((IsLookonMode()==true)||(wMeChairID!=pOutCard->wOutCardUser))
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

	//ը���ж�
	if ((m_bTurnOutType==CT_BOMB_CARD)||(m_bTurnOutType==CT_MISSILE_CARD))
	{
		if (GetMeChairID()==pOutCard->wCurrentUser)
		{
			m_GameClientView.SetBombTime(m_bOutBombCount);
		} 

		if(m_GameClientView.m_bMaxBombCount[SwitchViewChairID(pOutCard->wOutCardUser)] != 255){
			m_GameClientView.m_bOutBombCount[SwitchViewChairID(pOutCard->wOutCardUser)] = pOutCard->bOutBombCount;
			ASSERT(m_GameClientView.m_bMaxBombCount[SwitchViewChairID(pOutCard->wOutCardUser)]>=pOutCard->bOutBombCount);
		}

	}

	//�Լ��˿�
	if ((IsLookonMode()==true)||(pOutCard->wOutCardUser!=wMeChairID))
	{
		//ɾ���˿�
		BYTE bSourceCount=m_bCardCount[pOutCard->wOutCardUser];
		m_bCardCount[pOutCard->wOutCardUser]-=pOutCard->bCardCount;
		if (m_bHandCardData[pOutCard->wOutCardUser][0]!=0)
		{
			m_GameLogic.RemoveCard(pOutCard->bCardData,pOutCard->bCardCount,m_bHandCardData[pOutCard->wOutCardUser],bSourceCount);
		}
		m_GameClientView.SetCardCount(wOutViewChairID,m_bCardCount[pOutCard->wOutCardUser]);
		m_GameClientView.m_HandCardControl[wOutViewChairID].SetCardData(m_bHandCardData[pOutCard->wOutCardUser],m_bCardCount[pOutCard->wOutCardUser]);
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
		PlayGameSound(AfxGetInstanceHandle(),TEXT("MOST_CARD"));

		//���ö�ʱ��
		SetTimer(IDI_MOST_CARD,3000,NULL);

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
		m_GameClientView.m_btOutCard.EnableWindow((VerdictOutCard()==true)?TRUE:FALSE);
	}

	//����ʱ��
	if (pOutCard->wCurrentUser!=INVALID_CHAIR)
	{
		BYTE bCardCount=m_bCardCount[pOutCard->wCurrentUser];
		SetGameTimer(pOutCard->wCurrentUser,IDI_OUT_CARD,(bCardCount<m_bTurnCardCount)?3:30);
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
		m_GameClientView.m_btPassCard.EnableWindow((m_bTurnCardCount>0)?TRUE:FALSE);
		m_GameClientView.m_btOutCard.EnableWindow((VerdictOutCard()==true)?TRUE:FALSE);
	}

	//��������
	if ((IsLookonMode()==true)||(pPassCard->wPassUser!=GetMeChairID()))	PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));

	//����ʱ��
	if (m_bTurnCardCount!=0)
	{
		BYTE bCardCount=m_bCardCount[pPassCard->wCurrentUser];
		SetGameTimer(pPassCard->wCurrentUser,IDI_OUT_CARD,(bCardCount<m_bTurnCardCount)?3:30);
	}
	else SetGameTimer(pPassCard->wCurrentUser,IDI_OUT_CARD,30);

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
	ZeroMemory(m_GameClientView.m_bOutBombCount, sizeof(m_GameClientView.m_bOutBombCount));

	//ɾ����ʱ��
	KillTimer(IDI_MOST_CARD);
	KillGameTimer(IDI_OUT_CARD);
	KillGameTimer(IDI_LAND_SCORE);

	//���ؿؼ�
	m_GameClientView.m_btOutCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btPassCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btOneScore.ShowWindow(SW_HIDE);
	m_GameClientView.m_btTwoScore.ShowWindow(SW_HIDE);
	m_GameClientView.m_btThreeScore.ShowWindow(SW_HIDE);
	m_GameClientView.m_btShowCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btGiveUpScore.ShowWindow(SW_HIDE);

	//���ÿؼ�
	m_GameClientView.m_btOutCard.EnableWindow(FALSE);
	m_GameClientView.m_btPassCard.EnableWindow(FALSE);

	m_GameClientView.m_btSortByNum.EnableWindow(FALSE);
	m_GameClientView.m_btSortBySize.EnableWindow(FALSE);
	m_GameClientView.m_bMaxBombCountShow = false;

	//���û���
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		const tagUserData * pUserData=GetUserData(i);
		m_GameClientView.m_ScoreView.SetGameScore(i,pUserData->szName,pGameEnd->lGameScore[i]);
	}
	m_GameClientView.m_ScoreView.SetGameTax(pGameEnd->lGameTax);
	m_GameClientView.m_ScoreView.ShowWindow(SW_SHOW);

	//�����˿�
	BYTE bCardPos=0;
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		WORD wViewChairID=SwitchViewChairID(i);
		m_GameClientView.m_HandCardControl[wViewChairID].SetDisplayFlag(true);
		m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(&pGameEnd->bCardData[bCardPos],pGameEnd->bCardCount[i]);
		bCardPos+=pGameEnd->bCardCount[i];
		if (pGameEnd->bCardCount[i]!=0)
		{
			m_GameClientView.SetPassFlag(wViewChairID,false);
			m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(NULL,0);
		}
	}

	//��ʾ�˿�
	if (IsLookonMode()==true) m_GameClientView.m_HandCardControl[2].SetDisplayFlag(true);

	//��������
	WORD wMeChairID=GetMeChairID();
	LONG lMeScore=pGameEnd->lGameScore[GetMeChairID()];
	if (lMeScore>0L) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WIN"));
	else if (lMeScore<0L) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_LOST"));
	else PlayGameSound(GetModuleHandle(NULL),TEXT("GAME_END"));

	//���ý���
	if (IsLookonMode()==false)
	{
		m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
		SetGameTimer(GetMeChairID(),IDI_START_GAME,90);
	}
	m_GameClientView.ShowLandTitle(false);

	return true;
}

//�����ж�
bool CGameClientDlg::VerdictOutCard()
{
	//״̬�ж�
	if (m_GameClientView.m_btOutCard.IsWindowVisible()==FALSE) return false;

	//��ȡ�˿�
	BYTE bCardData[33];
	BYTE bShootCount=(BYTE)m_GameClientView.m_HandCardControl[2].GetShootCard(bCardData,CountArray(bCardData));

	
	//�����ж�
	if (bShootCount>0L)
	{
		m_GameLogic.SortCardList(bCardData, bShootCount, true);
		//��������
		BYTE bCardType=m_GameLogic.GetCardType(bCardData,bShootCount);

		//�����ж�
		if (bCardType==CT_ERROR) return false;

		//�����ж�
		if ((bCardType==CT_MISSILE_CARD)||(bCardType==CT_BOMB_CARD)) 
		{
			if (m_bOutBombCount>=m_bMaxBombCount) return false;
		}

		//�����ж�
		if (m_bTurnCardCount==0) return true;
		return m_GameLogic.CompareCard(bCardData,m_bTurnCardData,bShootCount,m_bTurnCardCount);
	}

	return false;
}

//�Զ�����
bool CGameClientDlg::AutomatismOutCard()
{
	WORD wMeChairID=GetMeChairID();

	//�ȳ�����
	if (m_bTurnCardCount==0)
	{
		//���ƽ���
		KillGameTimer(IDI_OUT_CARD);
		m_GameClientView.m_btOutCard.ShowWindow(SW_HIDE);
		m_GameClientView.m_btOutCard.EnableWindow(FALSE);
		m_GameClientView.m_btPassCard.ShowWindow(SW_HIDE);
		m_GameClientView.m_btPassCard.EnableWindow(FALSE);

		//��������
		CMD_C_OutCard OutCard;
		OutCard.bCardCount=1;
		OutCard.bCardData[0]=m_bHandCardData[wMeChairID][m_bCardCount[wMeChairID]-1];
		SendData(SUB_C_OUT_CART,&OutCard,sizeof(OutCard)-sizeof(OutCard.bCardData)+OutCard.bCardCount*sizeof(BYTE));

		//Ԥ�ȴ���
		PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));
		m_GameClientView.m_UserCardControl[2].SetCardData(OutCard.bCardData,OutCard.bCardCount);

		//Ԥ��ɾ��
		BYTE bSourceCount=m_bCardCount[wMeChairID];
		m_bCardCount[wMeChairID]-=OutCard.bCardCount;
		m_GameClientView.m_bCardCount[SwitchViewChairID(wMeChairID)] =m_bCardCount[wMeChairID];
		m_GameLogic.RemoveCard(OutCard.bCardData,OutCard.bCardCount,m_bHandCardData[wMeChairID],bSourceCount);
		m_GameClientView.m_HandCardControl[2].SetCardData(m_bHandCardData[wMeChairID],m_bCardCount[wMeChairID]);
	}
	else OnPassCard(0,0);

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
			m_GameClientView.m_btPassCard.EnableWindow(FALSE);
			m_GameClientView.m_btOutCard.EnableWindow((VerdictOutCard()==true)?TRUE:FALSE);
		}

		//����ʱ��
		SetGameTimer(wCurrentUser,IDI_OUT_CARD,30);

		return;
	}

	__super::OnTimer(nIDEvent);
}

//��ʼ��ť
LRESULT CGameClientDlg::OnStart(WPARAM wParam, LPARAM lParam)
{
	//���ñ���
	m_wTimeOutCount=0;
	m_bTurnCardCount=0;
	m_bMaxBombCount=255;
	m_bOutBombCount=0;
	m_bTurnOutType=CT_ERROR;
	m_wMostUser=INVALID_CHAIR;
	memset(m_bHandCardData,0,sizeof(m_bHandCardData));
	memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));

	//���ý���
	KillGameTimer(IDI_START_GAME);
	m_GameClientView.SetBaseScore(0L);
	m_GameClientView.ShowLandTitle(false);
	m_GameClientView.SetBombTime(m_bOutBombCount);
	m_GameClientView.SetCardCount(INVALID_CHAIR,0);
	m_GameClientView.SetLandUser(INVALID_CHAIR,0);
	m_GameClientView.SetLandScore(INVALID_CHAIR,0);
	m_GameClientView.SetPassFlag(INVALID_CHAIR,false);

	//���ؿؼ�
	m_GameClientView.m_btStart.ShowWindow(FALSE);
	m_GameClientView.m_ScoreView.ShowWindow(SW_HIDE);

	//�����˿�
	m_GameClientView.m_BackCardControl.SetCardData(NULL,0);
	m_GameClientView.m_HandCardControl[2].SetPositively(false);
	for (WORD i=0;i<GAME_PLAYER;i++) 
	{
		m_GameClientView.m_HandCardControl[i].SetCardData(NULL,0);
		m_GameClientView.m_UserCardControl[i].SetCardData(NULL,0);
	}

	//������Ϣ
	SendUserReady(NULL,0);

	return 0;
}

//������Ϣ
LRESULT CGameClientDlg::OnOutCard(WPARAM wParam, LPARAM lParam)
{
	//״̬�ж�
	if ((m_GameClientView.m_btOutCard.IsWindowEnabled()==FALSE)||
		(m_GameClientView.m_btOutCard.IsWindowVisible()==FALSE)) return 0;

	//���ý���
	KillGameTimer(IDI_OUT_CARD);
	m_GameClientView.m_btOutCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btOutCard.EnableWindow(FALSE);
	m_GameClientView.m_btPassCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btPassCard.EnableWindow(FALSE);

	//��������
	CMD_C_OutCard OutCard;
	OutCard.bCardCount=(BYTE)m_GameClientView.m_HandCardControl[2].GetShootCard(OutCard.bCardData,CountArray(OutCard.bCardData));
	m_GameLogic.SortCardList(OutCard.bCardData, OutCard.bCardCount, true);
	SendData(SUB_C_OUT_CART,&OutCard,sizeof(OutCard)-sizeof(OutCard.bCardData)+OutCard.bCardCount*sizeof(BYTE));

	//Ԥ����ʾ
	PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));
	m_GameClientView.m_UserCardControl[2].SetCardData(OutCard.bCardData,OutCard.bCardCount);

	//Ԥ��ɾ��
	WORD wMeChairID=GetMeChairID();
	BYTE bSourceCount=m_bCardCount[wMeChairID];
	m_bCardCount[wMeChairID]-=OutCard.bCardCount;
	m_GameClientView.m_bCardCount[SwitchViewChairID(wMeChairID)] =m_bCardCount[wMeChairID];
	m_GameLogic.RemoveCard(OutCard.bCardData,OutCard.bCardCount,m_bHandCardData[wMeChairID],bSourceCount);
	m_GameClientView.m_HandCardControl[2].SetCardData(m_bHandCardData[wMeChairID],m_bCardCount[wMeChairID]);

	//�����ж�
	BYTE cbCardType=m_GameLogic.GetCardType(OutCard.bCardData,OutCard.bCardCount);
	if ((cbCardType==CT_MISSILE_CARD)||(cbCardType==CT_BOMB_CARD)) {
		m_bOutBombCount++;
		//if(m_GameClientView.m_bMaxBombCount[2] != 255){
		//	m_GameClientView.m_bMaxBombCount[2] --;
		//	ASSERT(m_GameClientView.m_bMaxBombCount[2]>=0);
		//}
	}

	return 0;
}

//��������
LRESULT CGameClientDlg::OnPassCard(WPARAM wParam, LPARAM lParam)
{
	//״̬�ж�
	if (m_GameClientView.m_btPassCard.IsWindowEnabled()==FALSE) return 0;

	//���ý���
	KillGameTimer(IDI_OUT_CARD);
	m_GameClientView.m_btOutCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btOutCard.EnableWindow(FALSE);
	m_GameClientView.m_btPassCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btPassCard.EnableWindow(FALSE);

	//��������
	SendData(SUB_C_PASS_CARD);

	//Ԥ����ʾ
	WORD wMeChairID=GetMeChairID();
	m_GameClientView.SetPassFlag(2,true);
	PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));
	m_GameClientView.m_HandCardControl[2].SetCardData(m_bHandCardData[wMeChairID],m_bCardCount[wMeChairID]);

	return 0;
}

//�з���Ϣ
LRESULT CGameClientDlg::OnLandScore(WPARAM wParam, LPARAM lParam)
{
	//���ý���
	KillGameTimer(IDI_LAND_SCORE);
	m_GameClientView.m_btOneScore.ShowWindow(SW_HIDE);
	m_GameClientView.m_btTwoScore.ShowWindow(SW_HIDE);
	m_GameClientView.m_btThreeScore.ShowWindow(SW_HIDE);
	m_GameClientView.m_btShowCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btGiveUpScore.ShowWindow(SW_HIDE);

	//��������
	CMD_C_LandScore LandScore;
	LandScore.bLandScore=(BYTE)wParam;
	SendData(SUB_C_LAND_SCORE,&LandScore,sizeof(LandScore));

	//ը������
	switch (LandScore.bLandScore)
	{
	case 1:
		{
			m_bMaxBombCount=0;
			break;
		}
	case 2:
		{
			m_bMaxBombCount=1;
			break;
		}
	case 3:
	case 6:
		{
			m_bMaxBombCount=255;
			break;
		}
	default:
		{
			m_bMaxBombCount=0;
			break;
		}
	}

	return 0;
}

//�Ҽ��˿�
LRESULT CGameClientDlg::OnLeftHitCard(WPARAM wParam, LPARAM lParam)
{
	//���ÿؼ�
	bool bOutCard=VerdictOutCard();
	m_GameClientView.m_btOutCard.EnableWindow(bOutCard?TRUE:FALSE);

	return 0;
}

//����˿�
LRESULT CGameClientDlg::OnRightHitCard(WPARAM wParam, LPARAM lParam)
{
	//�û�����
	OnOutCard(0,0);

	return 0;
}


//����������ť
LRESULT CGameClientDlg::OnSorByNumCard(WPARAM wParam, LPARAM lParam)
{
	static bool bMoreToLess = false;
	WORD wMeChairID=GetMeChairID();
	BYTE bHandCardData[33];				//�����˿�
	m_GameClientView.m_btOutCard.EnableWindow(false);
	PlayGameSound(AfxGetInstanceHandle(),TEXT("IDR_SHOOT_CARD"));
	bMoreToLess = bMoreToLess?false:true;
	memcpy(bHandCardData, m_bHandCardData[wMeChairID], sizeof(bHandCardData));
	m_GameLogic.SortCardByNum(bHandCardData,m_bCardCount[wMeChairID], bMoreToLess);	
	m_GameClientView.m_HandCardControl[2].SetCardData(bHandCardData,m_bCardCount[wMeChairID]);
	return 0;
}

//����С����ť
LRESULT CGameClientDlg::OnSorBySizeCard(WPARAM wParam, LPARAM lParam)
{
	static bool bLargeToSmall = true;
	WORD wMeChairID=GetMeChairID();
	BYTE bHandCardData[33];				//�����˿�
	m_GameClientView.m_btOutCard.EnableWindow(false);
	PlayGameSound(AfxGetInstanceHandle(),TEXT("IDR_SHOOT_CARD"));
	bLargeToSmall = bLargeToSmall?false:true;
	memcpy(bHandCardData, m_bHandCardData[wMeChairID], sizeof(bHandCardData));
	m_GameLogic.SortCardList(bHandCardData,m_bCardCount[wMeChairID], bLargeToSmall);	
	m_GameClientView.m_HandCardControl[2].SetCardData(bHandCardData,m_bCardCount[wMeChairID]);
	return 0;
}
//////////////////////////////////////////////////////////////////////////

