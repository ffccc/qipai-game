#include "Stdafx.h"
#include "GameOption.h"
#include "GameClient.h"
#include "GameClientDlg.h"
#include ".\gameclientdlg.h"
#include "cassert"
//////////////////////////////////////////////////////////////////////////
//�궨��

//��Ϸ��ʱ��
#define IDI_OUT_CARD					200								//���ƶ�ʱ��
#define IDI_START_GAME					201								//��ʼ��ʱ��

//��Ϸ��ʱ��
#define IDI_MOST_CARD					300								//���ʱ��
#define IDI_DISPATCH_CARD				301								//���ƶ�ʱ��

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientDlg, CGameFrameDlg)
	ON_WM_TIMER()
	ON_MESSAGE(IDM_START,OnStart)
	ON_MESSAGE(IDM_OUT_CARD,OnOutCard)
	ON_MESSAGE(IDM_PASS_CARD,OnPassCard)
	ON_MESSAGE(IDM_OUT_PROMPT,OnOutPrompt)
	ON_MESSAGE(IDM_SORT_BY_COLOR,OnSortByColor)
	ON_MESSAGE(IDM_SORT_BY_VALUE,OnSortByValue)
	ON_MESSAGE(IDM_LEFT_HIT_CARD,OnLeftHitCard)
	ON_MESSAGE(IDM_RIGHT_HIT_CARD,OnRightHitCard)
	ON_MESSAGE(IDM_TRUSTEE_CONTROL,OnStusteeControl)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientDlg::CGameClientDlg() : CGameFrameDlg(&m_GameClientView)
{
	//��Ϸ����
	m_wBankerUser=INVALID_CHAIR;

	//��Ϸ����
	m_wCurrentUser=INVALID_CHAIR;

	//ʤ����Ϣ
	m_wWinCount=0;
	ZeroMemory(m_wWinOrder,sizeof(m_wWinOrder));

	//���ñ���
	m_bStustee=false;
	m_bDeasilOrder=false;
	m_dwCardHSpace=DEFAULT_PELS;

	//�˿˱���
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
	ZeroMemory(m_cbHandCardCount,sizeof(m_cbHandCardCount));
	ZeroMemory(m_cbPartnerCardData,sizeof(m_cbPartnerCardData));

	//���Ʊ���
	m_cbTurnCardCount=0;
	ZeroMemory(m_cbTurnCardData,sizeof(m_cbTurnCardData));

	//��������
	m_cbRemnantCardCount=0;
	m_cbDispatchCardCount=0;
	m_wMostUser=INVALID_CHAIR;
	m_wTimeOutCount=0;
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
	m_bDeasilOrder=AfxGetApp()->GetProfileInt(TEXT("GameOption"),TEXT("DeasilOrder"),FALSE)?true:false;

	//��������
	if ((m_dwCardHSpace>MAX_PELS)||(m_dwCardHSpace<LESS_PELS)) m_dwCardHSpace=DEFAULT_PELS;

	//���ÿؼ�
	m_GameClientView.SetUserOrder(m_bDeasilOrder);
	m_GameClientView.m_HandCardControl[2].SetCardSpace(m_dwCardHSpace,0,20);
	m_GameClientView.m_UserCardControl[1].SetCardSpace(0,18,20);
	m_GameClientView.m_UserCardControl[3].SetCardSpace(0,18,20);
	m_GameClientView.m_UserCardControl[0].SetCardSpace(18,18,20);
	m_GameClientView.m_UserCardControl[2].SetCardSpace(18,18,20);


	return true;
}

//���ÿ��
void CGameClientDlg::ResetGameFrame()
{
	//��Ϸ����
	m_wBankerUser=INVALID_CHAIR;

	//ɾ����ʱ
	KillGameTimer(0);
	KillTimer(IDI_MOST_CARD);
	KillTimer(IDI_DISPATCH_CARD);
	
	//��Ϸ����
	m_wCurrentUser=INVALID_CHAIR;

	//���ñ���
	m_bStustee=false;

	//ʤ����Ϣ
	m_wWinCount=0;
	ZeroMemory(m_wWinOrder,sizeof(m_wWinOrder));

	//�˿˱���
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
	ZeroMemory(m_cbHandCardCount,sizeof(m_cbHandCardCount));
	ZeroMemory(m_cbPartnerCardData,sizeof(m_cbPartnerCardData));


	//���Ʊ���
	m_cbTurnCardCount=0;
	ZeroMemory(m_cbTurnCardData,sizeof(m_cbTurnCardData));

	//��������
	m_cbRemnantCardCount=0;
	m_cbDispatchCardCount=0;
	m_wTimeOutCount=0;
	m_wMostUser=INVALID_CHAIR;
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
	GameOption.m_bDeasilOrder=m_GameClientView.IsDeasilOrder();

	//��������
	if (GameOption.DoModal()==IDOK)
	{
		//��ȡ����
		m_bDeasilOrder=GameOption.m_bDeasilOrder;
		m_dwCardHSpace=GameOption.m_dwCardHSpace;

		//���ÿؼ�
		EnableSound(GameOption.m_bEnableSound);
		m_GameClientView.SetUserOrder(GameOption.m_bDeasilOrder);
		m_GameClientView.m_HandCardControl[2].SetCardSpace(m_dwCardHSpace,0,20);

		//��������
		AfxGetApp()->WriteProfileInt(TEXT("GameOption"),TEXT("CardSpace"),m_dwCardHSpace);
		AfxGetApp()->WriteProfileInt(TEXT("GameOption"),TEXT("DeasilOrder"),m_bDeasilOrder?TRUE:FALSE);
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
			//�����й�
			if ((m_wTimeOutCount>=3)&&(m_bStustee==false))
				OnStusteeControl(0,0);

			//��ʱ�ж�
			if ((nElapse==0) || (m_bStustee==true&&nElapse<29))
			{
				if ((IsLookonMode()==false)&&(wChairID==GetMeChairID()))
				{
					if (m_bStustee==false)
					{
						m_wTimeOutCount++;
						if(m_wTimeOutCount==3)
						{
							InsertSystemString(TEXT(" ��������γ�ʱ���л�Ϊ��ϵͳ�йܡ�ģʽ."));
						}

					}
					AutomatismOutCard();
				}
				return true;
			}

			//��������
			if (m_cbHandCardCount[GetMeChairID()]<m_cbTurnCardCount) return true;
			if ((nElapse<=3)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false)) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));

			return true;
		}
	case IDI_START_GAME:		//��ʼ��Ϸ
		{
			if (nElapse==0)
			{
				if ((IsLookonMode()==false)&&(wChairID==GetMeChairID()))
						PostMessage(WM_CLOSE);
				return false;
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
	case SUB_S_TRUSTEE:			//�û��й�
		{
			return OnSubTrustee(pBuffer,wDataSize);
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
			if (wDataSize!=sizeof(CMD_S_StatusFree))
			{
				ASSERT(FALSE);
				return false;
			}
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pBuffer;

			//���ÿؼ�
			if (IsLookonMode()==false)
			{
				m_GameClientView.m_btStart.ShowWindow(TRUE);
				m_GameClientView.m_btStart.SetFocus();
				m_GameClientView.m_btStusteeControl.EnableWindow(TRUE);

			}
			//�����˿�
			if (IsLookonMode()==false) m_GameClientView.m_HandCardControl[2].SetDisplayFlag(true);
			//����ʱ��
			SetGameTimer(GetMeChairID(),IDI_START_GAME,30);
			return true;
		}

	case GS_RF_PLAYING:		//��Ϸ״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pBuffer;

			//��������
			WORD wMeChairID=GetMeChairID();

			//���ñ���
			m_wCurrentUser=pStatusPlay->wCurrentUser;

			//ʤ����Ϣ
			m_wWinCount=pStatusPlay->wWinCount;
			CopyMemory(m_wWinOrder,pStatusPlay->wWinOrder,sizeof(m_wWinOrder));

			//���Ʊ���
			m_cbTurnCardCount=pStatusPlay->cbTurnCardCount;
			CopyMemory(m_cbTurnCardData,pStatusPlay->cbTurnCardData,m_cbTurnCardCount*sizeof(BYTE));

			//�˿�����
			CopyMemory(m_cbHandCardCount,pStatusPlay->cbHandCardCount,sizeof(m_cbHandCardCount));
			CopyMemory(m_cbHandCardData,pStatusPlay->cbHandCardData,sizeof(BYTE)*m_cbHandCardCount[wMeChairID]);
		


			//���ý���
			for (BYTE i=0;i<GAME_PLAYER;i++)
			{
				WORD wViewChairID=SwitchViewChairID(i);
				m_GameClientView.SetCardCount(wViewChairID,pStatusPlay->cbHandCardCount[i]);
			}

			//��������
			for (WORD i=0;i<m_wWinCount;i++)
			{
				m_GameClientView.SetUserWinOrder(SwitchViewChairID(m_wWinOrder[i]),i+1);
			}

			//�й�����
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				m_GameClientView.SetTrustee(SwitchViewChairID(i),pStatusPlay->bTrustee[i]);
				m_bStustee=pStatusPlay->bTrustee[GetMeChairID()];
				m_wTimeOutCount=0;
			}
			//�����˿�
			BYTE cbCardCount[MAX_COUNT];
			ZeroMemory(cbCardCount,sizeof(cbCardCount));

			//�����˿�
			for (BYTE i=0;i<GAME_PLAYER;i++)
			{
				WORD wViewChairID=SwitchViewChairID(i);
#ifdef VIDEO_GAME
				if (wViewChairID!=2)
				{
					if(wViewChairID==0)
					{
						m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(cbCardCount,pStatusPlay->cbHandCardCount[i]);
					}
				}
#else
				if (wViewChairID!=2)
				{
					m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(cbCardCount,pStatusPlay->cbHandCardCount[i]);
				}
#endif
				else m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(m_cbHandCardData,m_cbHandCardCount[i]);
			}

			//�������
			if ((IsLookonMode()==false)&&(pStatusPlay->wCurrentUser==GetMeChairID()))
			{
				m_GameClientView.m_btOutCard.EnableWindow(FALSE);
				m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
				//m_GameClientView.m_btOutPrompt.ShowWindow(SW_SHOW);
				m_GameClientView.m_btPassCard.ShowWindow(SW_SHOW);
				m_GameClientView.m_btPassCard.EnableWindow((m_cbTurnCardCount!=0)?TRUE:FALSE);
				
			}
			if(IsLookonMode()==false)
			{
				m_GameClientView.m_btStusteeControl.SetButtonImage((m_bStustee==true)?IDB_BT_STOP_TRUSTEE:IDB_BT_START_TRUSTEE,AfxGetInstanceHandle(),false);
				m_GameClientView.m_btStusteeControl.EnableWindow(TRUE);

			}

			//��������
			if (m_cbTurnCardCount!=0)
			{
				WORD wViewChairID=SwitchViewChairID(pStatusPlay->wTurnWiner);
				m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(m_cbTurnCardData,m_cbTurnCardCount);
			}

			//���ö�ʱ��
			SetGameTimer(pStatusPlay->wCurrentUser,IDI_OUT_CARD,30);

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


	m_wBankerUser=pGameStart->wCurrentUser;

	//��Ϸ����
	m_wCurrentUser=pGameStart->wCurrentUser;

	//��Ϸ����
	m_cbTurnCardCount=0;


	//ʤ����Ϣ
	m_wWinCount=0;
	ZeroMemory(m_wWinOrder,sizeof(m_wWinOrder));

	//���Ʊ���
	m_cbTurnCardCount=0;
	ZeroMemory(m_cbTurnCardData,sizeof(m_cbTurnCardData));

	
	//����˿�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��ȡλ��
		WORD wViewChairID=SwitchViewChairID(i);

		//�����˿�
		if (wViewChairID==2)
		{
			bool bShowCard=((IsLookonMode()==false)||(IsAllowLookon()==true));
			m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(NULL,0);
			m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(NULL,0);
			m_GameClientView.m_HandCardControl[wViewChairID].SetPositively(false);
			m_GameClientView.m_HandCardControl[wViewChairID].SetDisplayFlag(bShowCard);
		}
		else if(wViewChairID==0)
		{
			m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(NULL,0);
			m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(NULL,0);
			m_GameClientView.m_HandCardControl[wViewChairID].SetDisplayFlag(false);
		}
		else
		{
			m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(NULL,0);
			m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(NULL,0);
			m_GameClientView.m_HandCardControl[wViewChairID].SetDisplayFlag(false);
		}
	}

	//�����˿�
	WORD wMeChairID=GetMeChairID();
	for (WORD i=0;i<GAME_PLAYER;i++) 
	{
		m_cbHandCardCount[i]=CountArray(pGameStart->cbCardData);
		m_GameClientView.SetCardCount(i,m_cbHandCardCount[i]);
	}
	CopyMemory(m_cbHandCardData,pGameStart->cbCardData,sizeof(BYTE)*m_cbHandCardCount[wMeChairID]);
	CopyMemory(m_cbPartnerCardData,pGameStart->cbPartnerCardData,sizeof(BYTE)*m_cbHandCardCount[(wMeChairID+2)%GAME_PLAYER]);

	//�ɷ��˿�
	DispatchUserCard(m_cbHandCardData,m_cbHandCardCount[wMeChairID]);

	//�����˿�
	m_GameLogic.SortCardList(m_cbHandCardData,m_cbHandCardCount[wMeChairID]);
	
	//�й�����
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.SetTrustee(SwitchViewChairID(i),pGameStart->bTrustee[i]);
	}

	//���½���
	m_GameClientView.SetUserWinOrder(INVALID_CHAIR,0);
	m_GameClientView.UpdateGameView(NULL);

	//��������
	if ((IsLookonMode()==false)) ActiveGameFrame(); 

 	m_GameClientView.m_ScoreView.ShowWindow(SW_HIDE);

	return true;
}

//�û�����
bool CGameClientDlg::OnSubOutCard(const void * pBuffer, WORD wDataSize)
{
 	//��������
 	CMD_S_OutCard * pOutCard=(CMD_S_OutCard *)pBuffer;
 	WORD wHeadSize=sizeof(CMD_S_OutCard)-sizeof(pOutCard->cbCardData);
 
 	//Ч������
 	ASSERT(wDataSize>=wHeadSize);
 	if (wDataSize<wHeadSize) return false;
 	ASSERT(wDataSize==(wHeadSize+pOutCard->cbCardCount*sizeof(pOutCard->cbCardData[0])));
 	if (wDataSize!=(wHeadSize+pOutCard->cbCardCount*sizeof(pOutCard->cbCardData[0]))) return false;
 
	//ֹͣ����
	DoncludeDispatchCard();

 	//ɾ����ʱ��
 	KillTimer(IDI_MOST_CARD);
 	KillGameTimer(IDI_OUT_CARD);

	//��������
	WORD wMeChairID=GetMeChairID();
	WORD wViewChairID=SwitchViewChairID(pOutCard->wOutCardUser); 
	bool bPlayGameSound=((IsLookonMode()==false)&&(wMeChairID==pOutCard->wOutCardUser));


	//��ʷ����
	if (m_cbTurnCardCount==0)
	{
		//�û��˿�
		for (WORD i=0;i<GAME_PLAYER;i++)
		{

			//��������
			if (i!=pOutCard->wOutCardUser)
			{
				WORD wViewChairID=SwitchViewChairID(i);
				m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(NULL,0);
			}
		}

		//������־
		m_GameClientView.SetPassFlag(INVALID_CHAIR,false);
	}
	
	//���Ʊ���
	m_wCurrentUser=pOutCard->wCurrentUser;
	m_cbTurnCardCount=pOutCard->cbCardCount;
 	CopyMemory(m_cbTurnCardData,pOutCard->cbCardData,sizeof(BYTE)*pOutCard->cbCardCount);

	//���ñ���
	BYTE cbStarLevel;

	//��ȡ����
	BYTE cbTurnCardType=m_GameLogic.GetCardType(pOutCard->cbCardData,pOutCard->cbCardCount,cbStarLevel);
	//��������
	if ((bPlayGameSound==false)&&((IsLookonMode()==true)||(wMeChairID!=pOutCard->wOutCardUser)))
	{
		if (cbTurnCardType>=CT_BOMB)
		{
			//��������
			if (bPlayGameSound==false)
			{
				bPlayGameSound=true;
				PlayBombSound(pOutCard->wOutCardUser,cbStarLevel);
			}
		}
		else
		{
			if (bPlayGameSound==false)
			{
				bPlayGameSound=true;
				PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));
			}
		}
	}

 
	//�����˿�
	//��������
	if ((IsLookonMode()==true)||(wMeChairID!=pOutCard->wOutCardUser))
	{

		m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(pOutCard->cbCardData,pOutCard->cbCardCount);
		//ɾ���˿�
		if (wViewChairID==2)
		{
			//ɾ���˿�
			BYTE cbSourceCount=m_cbHandCardCount[wMeChairID];
			m_cbHandCardCount[pOutCard->wOutCardUser]-=pOutCard->cbCardCount;
			m_GameLogic.RemoveCard(pOutCard->cbCardData,pOutCard->cbCardCount,m_cbHandCardData,cbSourceCount);

			//�����˿�
			m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(m_cbHandCardData,m_cbHandCardCount[wMeChairID]);
		}
		else if(wViewChairID==0)
		{
			//ɾ���˿�
			BYTE cbSourceCount=m_cbHandCardCount[pOutCard->wOutCardUser];
			m_cbHandCardCount[pOutCard->wOutCardUser]-=pOutCard->cbCardCount;
			m_GameLogic.RemoveCard(pOutCard->cbCardData,pOutCard->cbCardCount,m_cbPartnerCardData,cbSourceCount);

			//�����˿�
			m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(m_cbPartnerCardData,m_cbHandCardCount[pOutCard->wOutCardUser]);
		}
		else
		{
			m_cbHandCardCount[pOutCard->wOutCardUser]-=pOutCard->cbCardCount;
#ifdef VIDEO_GAME
#else
			//�����˿�
			BYTE cbCardCount[MAX_COUNT];
			ZeroMemory(cbCardCount,sizeof(cbCardCount));
			m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(cbCardCount,m_cbHandCardCount[pOutCard->wOutCardUser]);
#endif
		}

	}
	m_GameClientView.SetCardCount(wViewChairID,m_cbHandCardCount[pOutCard->wOutCardUser]);

	//��������
	if (m_cbHandCardCount[pOutCard->wOutCardUser]==0)
	{
		m_wWinOrder[m_wWinCount++]=pOutCard->wOutCardUser;
		m_GameClientView.SetUserWinOrder(SwitchViewChairID(pOutCard->wOutCardUser),m_wWinCount);

		if((IsLookonMode()==false)&&GetMeChairID()==pOutCard->wOutCardUser)
			m_GameClientView.m_HandCardControl[0].SetDisplayFlag(true);
	}

	//����ж�
	if (m_wCurrentUser==pOutCard->wOutCardUser)
	{
		//���ñ���
		m_cbTurnCardCount=0;
		m_wCurrentUser=INVALID_CHAIR;
		m_wMostUser=pOutCard->wCurrentUser;
		ZeroMemory(m_cbTurnCardData,sizeof(m_cbTurnCardData));

		//��������
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			//�û�����
			if (pOutCard->wOutCardUser==i) continue;

			//��������
			WORD wViewChairID=SwitchViewChairID(i);
			m_GameClientView.SetPassFlag(wViewChairID,true);
			m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(NULL,0);

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
		WORD wViewChairID=SwitchViewChairID(m_wCurrentUser);
		m_GameClientView.SetPassFlag(wViewChairID,false);
		m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(NULL,0);
	}

	//�������
	if ((IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID()))
	{
		//������
		if (m_bStustee==false) ActiveGameFrame();

		m_GameClientView.m_btPassCard.EnableWindow(TRUE);
		m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btPassCard.ShowWindow(SW_SHOW);
		//m_GameClientView.m_btOutPrompt.ShowWindow(SW_SHOW);
		m_GameClientView.m_btOutCard.EnableWindow((VerdictOutCard()==true)?TRUE:FALSE);
		m_GameClientView.m_btPassCard.EnableWindow((VerdictPassCard()==true)?TRUE:FALSE);
	}

	//���ö�ʱ��
	if (pOutCard->wCurrentUser!=INVALID_CHAIR)
	{
		BYTE cbCardCount=m_cbHandCardCount[pOutCard->wCurrentUser];
		SetGameTimer(m_wCurrentUser,IDI_OUT_CARD,(cbCardCount<m_cbTurnCardCount)?3:30);
	}

	return true;
}

//��������
bool CGameClientDlg::OnSubPassCard(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_PassCard));
	if (wDataSize!=sizeof(CMD_S_PassCard)) return false;

	//��������
	CMD_S_PassCard * pPassCard=(CMD_S_PassCard *)pBuffer;

	//ɾ����ʱ��
	KillGameTimer(IDI_OUT_CARD);

	//���ñ���
	m_wCurrentUser=pPassCard->wCurrentUser;

	//��������
	if ((IsLookonMode()==true)||(pPassCard->wPassCardUser!=GetMeChairID()))
	{
		WORD wPassCardUser=pPassCard->wPassCardUser;
		m_GameClientView.SetPassFlag(SwitchViewChairID(wPassCardUser),true);
	}
	if(m_wCurrentUser != INVALID_CHAIR)
	{
		//��ͼλ��
		WORD wViewChairID=SwitchViewChairID(m_wCurrentUser);

		//���ý���
		m_GameClientView.SetPassFlag(wViewChairID,false);
		m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(NULL,0);
	}

	//һ���ж�
	if (pPassCard->cbTurnOver==TRUE)
	{
		//���Ʊ���
		m_cbTurnCardCount=0;
		ZeroMemory(m_cbTurnCardData,sizeof(m_cbTurnCardData));
        
		//�������
		m_GameClientView.SetPassFlag(INVALID_CHAIR,false);

	}

	//�������
	if ((IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID()))
	{
		//������
		ActiveGameFrame();

		//���ð�ť
		m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btPassCard.ShowWindow(SW_SHOW);
		//m_GameClientView.m_btOutPrompt.ShowWindow(SW_SHOW);
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
		SetGameTimer(m_wCurrentUser,IDI_OUT_CARD,(cbCardCount<m_cbTurnCardCount)?3:30);
	}
	else 
		SetGameTimer(m_wCurrentUser,IDI_OUT_CARD,30);

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
	//m_GameClientView.m_btOutPrompt.ShowWindow(SW_HIDE);

	//���ÿؼ�
	m_GameClientView.m_btOutCard.EnableWindow(FALSE);
	m_GameClientView.m_btPassCard.EnableWindow(FALSE);

	//���û���
	CString strTemp ,strEnd = "\n";
	//���û���
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		const tagUserData * pUserData=GetUserData(i);
		m_GameClientView.m_ScoreView.SetGameScore(i,pUserData->szName,pGameEnd->lGameScore[i]);
		if(pGameEnd->lGameScore[i]>0)
			strTemp.Format("%s: %+ld\n",pUserData->szName,pGameEnd->lGameScore[i]);
		else
			strTemp.Format("%s: %ld\n",pUserData->szName,pGameEnd->lGameScore[i]);
		strEnd += strTemp;
	}

	//��Ϣ����
	InsertSystemString((LPCTSTR)strEnd);
	m_GameClientView.m_ScoreView.ShowWindow(SW_SHOW);

	//�����˿�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		WORD wViewChairID=SwitchViewChairID(i);

#ifdef  VIDEO_GAME
		if(wViewChairID%2==0)
		{
			m_GameClientView.m_HandCardControl[wViewChairID].SetDisplayFlag(true);
			m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(pGameEnd->cbCardData[i],pGameEnd->cbCardCount[i]);

		}
#else
		m_GameClientView.m_HandCardControl[wViewChairID].SetDisplayFlag(true);
		m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(pGameEnd->cbCardData[i],pGameEnd->cbCardCount[i]);

#endif	
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
		SetGameTimer(GetMeChairID(),IDI_START_GAME,30);
	}
	m_GameClientView.m_HandCardControl[2].SetPositively(false);

	//ȡ���й�
	if(m_bStustee)
		OnStusteeControl(0,0);

	return true;
}

//�����ж�
bool CGameClientDlg::VerdictOutCard()
{
	//״̬�ж�
	if (m_GameClientView.m_btOutCard.IsWindowVisible()==FALSE) return false;
	if (m_wCurrentUser!=GetMeChairID()) return false;


	//��ȡ�˿�
	BYTE cbCardData[MAX_COUNT];
	BYTE bShootCount=(BYTE)m_GameClientView.m_HandCardControl[2].GetShootCard(cbCardData,CountArray(cbCardData));

	//�����ж�
	if (bShootCount>0L)
	{
		//�����˿�
		m_GameLogic.SortCardList(cbCardData,bShootCount);

		BYTE cbStarLevel;
		//��������
		BYTE bCardType=m_GameLogic.GetCardType(cbCardData,bShootCount,cbStarLevel);

		//�����ж�
		if (bCardType==CT_ERROR) return false;
	
		//�����ж�
		if (m_cbTurnCardCount==0) return true;
		return m_GameLogic.CompareCard(m_cbTurnCardData,cbCardData,m_cbTurnCardCount,bShootCount);
	}

	return false;
}

//�����ж�
bool CGameClientDlg::VerdictPassCard()
{
	//�׳��ж�
	if (m_cbTurnCardCount==0) return false;


	return true;
}

//�Զ�����
bool CGameClientDlg::AutomatismOutCard()
{
	if ((IsLookonMode()==true)||(m_wCurrentUser!=GetMeChairID())) return false;

	//��ǰ����
	CSkinButton & btOutCard=m_GameClientView.m_btOutCard;
	if ((btOutCard.IsWindowEnabled())&&(btOutCard.IsWindowVisible()))
	{
		OnOutCard(0,0);
		return true;
	}

	//���ƴ���
	if ((m_cbTurnCardCount==0))
	{
		//�������
		tagOutCardResult OutCardResult;
		m_GameLogic.SearchOutCard(m_cbHandCardData,m_cbHandCardCount[GetMeChairID()],m_cbTurnCardData,m_cbTurnCardCount,OutCardResult);

		//���ý���
		if (OutCardResult.cbCardCount>0)
		{
			//���ý���
			m_GameClientView.m_btOutCard.EnableWindow(TRUE);
			m_GameClientView.m_HandCardControl[2].SetShootCard(OutCardResult.cbResultCard,OutCardResult.cbCardCount);

			//���ƶ���
			OnOutCard(0,0);

			return true;
		}
	}

	//��������
	if (m_cbTurnCardCount>0) 
		OnPassCard(0,0);

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
#ifdef VIDEO_GAME
			if (i ==2)
			{
				m_GameClientView.m_HandCardControl[i].SetCardData(m_cbHandCardData,m_cbHandCardCount[i]);
			}
			else
			{
				if(i==0)
				{
					m_GameClientView.m_HandCardControl[i].SetCardData(cbUserCard,MAX_COUNT);
				}

			}
#else
			if (i ==2)
			{
				m_GameClientView.m_HandCardControl[i].SetCardData(m_cbHandCardData,m_cbHandCardCount[i]);
			}
			else
			{
				m_GameClientView.m_HandCardControl[i].SetCardData(cbUserCard,MAX_COUNT);

			}
#endif

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
	SetTimer(IDI_DISPATCH_CARD,100,NULL);

	return true;
}

//��ʱ����Ϣ
void CGameClientDlg::OnTimer(UINT nIDEvent)
{
	if ((nIDEvent==IDI_MOST_CARD))
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
			//m_GameClientView.m_btOutPrompt.ShowWindow(SW_SHOW);
			m_GameClientView.m_btPassCard.EnableWindow(FALSE);
			m_GameClientView.m_btOutCard.EnableWindow((VerdictOutCard()==true)?TRUE:FALSE);
			m_GameClientView.m_btPassCard.EnableWindow((VerdictPassCard()==true)?TRUE:FALSE);
		}

		//����ʱ��
		SetGameTimer(wCurrentUser,IDI_OUT_CARD,30);

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
#ifdef  VIDEO_GAME
				if (i!=2)
				{
					if(i==0)
					{
						m_GameClientView.m_HandCardControl[i].SetCardData(cbUserCard,m_cbDispatchCardCount);
					}
				}
				else
					m_GameClientView.m_HandCardControl[i].SetCardData(m_cbDispatchCardData,m_cbDispatchCardCount); 

#else
				if (i!=2)
				{
					if(i==0)
					{
						m_GameClientView.m_HandCardControl[i].SetCardData(cbUserCard,m_cbDispatchCardCount);
					}
					else
					{
						m_GameClientView.m_HandCardControl[i].SetCardData(cbUserCard,m_cbDispatchCardCount);
					}
				}
				else
					m_GameClientView.m_HandCardControl[i].SetCardData(m_cbDispatchCardData,m_cbDispatchCardCount); 

#endif 
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
				m_GameClientView.SetCardCount(SwitchViewChairID(i),m_cbHandCardCount[i]);
			}

			//��ǰ���
			if ((IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID()))
			{
				ActiveGameFrame();
				m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
				m_GameClientView.m_btOutCard.EnableWindow(FALSE);
				m_GameClientView.m_btPassCard.EnableWindow(FALSE);
				m_GameClientView.m_btPassCard.ShowWindow(SW_SHOW);
				//m_GameClientView.m_btOutPrompt.ShowWindow(SW_SHOW);
			}

			if(IsLookonMode()==false)
			{
				m_GameClientView.m_HandCardControl[2].SetPositively(IsLookonMode()==false);
				m_GameLogic.SortCardList(m_cbPartnerCardData,m_cbHandCardCount[(GetMeChairID()+2)%GAME_PLAYER]);
				m_GameClientView.m_HandCardControl[0].SetCardData(m_cbPartnerCardData,m_cbHandCardCount[(GetMeChairID()+2)%GAME_PLAYER]);
			}

			//����ʣ��
			PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));

			//����ʱ��
			if (m_wCurrentUser!=INVALID_CHAIR)
				SetGameTimer(m_wCurrentUser,IDI_OUT_CARD,30);
		}

		return;
	}
	__super::OnTimer(nIDEvent);
}

//��ʼ��ť
LRESULT CGameClientDlg::OnStart(WPARAM wParam, LPARAM lParam)
{   

	//ʤ����Ϣ
	m_wWinCount=0;
	ZeroMemory(m_wWinOrder,sizeof(m_wWinOrder));

	//���ñ���
	m_bDeasilOrder=false;
	m_dwCardHSpace=DEFAULT_PELS;

	//�˿˱���
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
	ZeroMemory(m_cbHandCardCount,sizeof(m_cbHandCardCount));

	//���Ʊ���
	m_cbTurnCardCount=0;
	ZeroMemory(m_cbTurnCardData,sizeof(m_cbTurnCardData));

	//���ý���
	KillGameTimer(IDI_START_GAME);
	m_GameClientView.SetCardCount(INVALID_CHAIR,0);
	m_GameClientView.SetPassFlag(INVALID_CHAIR,false);
	m_GameClientView.SetUserWinOrder(INVALID_CHAIR,0);

	m_GameClientView.m_btStart.ShowWindow(SW_HIDE);

	//���ؿؼ�
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
	//m_GameClientView.m_btOutPrompt.ShowWindow(SW_HIDE);
	
	//��ȡ�˿�
	BYTE cbCardData[MAX_COUNT];
	BYTE cbCardCount=(BYTE)m_GameClientView.m_HandCardControl[2].GetShootCard(cbCardData,CountArray(cbCardData));

	//�����˿�
	m_GameLogic.SortCardList(cbCardData,cbCardCount,ST_ORDER);

	//��������
	WORD wMeChairID=GetMeChairID();
	BYTE cbSourceCount=m_cbHandCardCount[wMeChairID];
	
	BYTE cbStarLevel;
	//ը���ж�
	BYTE cbTurnCardType=m_GameLogic.GetCardType(cbCardData,cbCardCount,cbStarLevel);
	if (cbTurnCardType>=CT_BOMB)
	{
		PlayBombSound(GetMeChairID(),cbStarLevel);

	}
	else  
		PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));
	m_GameClientView.m_UserCardControl[2].SetCardData(cbCardData,cbCardCount);


	//ɾ���˿�
	m_cbHandCardCount[wMeChairID]-=cbCardCount;
	m_GameLogic.RemoveCard(cbCardData,cbCardCount,m_cbHandCardData,cbSourceCount);
	m_GameClientView.m_HandCardControl[2].SetCardData(m_cbHandCardData,m_cbHandCardCount[wMeChairID]);

	//��������
	CMD_C_OutCard OutCard;
	OutCard.cbCardCount=cbCardCount;
	CopyMemory(OutCard.cbCardData,cbCardData,cbCardCount);
	SendData(SUB_C_OUT_CARD,&OutCard,sizeof(OutCard)-sizeof(OutCard.cbCardData)+OutCard.cbCardCount*sizeof(BYTE));

	//���½���
	m_GameClientView.UpdateGameView(NULL);
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
	//m_GameClientView.m_btOutPrompt.ShowWindow(SW_HIDE);

	//��������
	SendData(SUB_C_PASS_CARD);

	//Ԥ����ʾ
	m_GameClientView.m_UserCardControl[2].SetCardData(NULL,0);
	m_GameClientView.SetPassFlag(2,true);
	PlayGameSound(AfxGetInstanceHandle(),TEXT("PASS_CARD"));

	return 0;
}

//������ʾ
LRESULT CGameClientDlg::OnOutPrompt(WPARAM wParam, LPARAM lParam)
{
	//�������
	tagOutCardResult OutCardResult;
	m_GameLogic.SearchOutCard(m_cbHandCardData,m_cbHandCardCount[GetMeChairID()],m_cbTurnCardData,m_cbTurnCardCount,OutCardResult);

	//���ý���
	if (OutCardResult.cbCardCount>0)
	{
		//���ý���
		m_GameClientView.m_HandCardControl[2].SetShootCard(OutCardResult.cbResultCard,OutCardResult.cbCardCount);

		//���ÿؼ�
		bool bOutCard=VerdictOutCard();
		m_GameClientView.m_btOutCard.EnableWindow((bOutCard==true)?TRUE:FALSE);

		//���½���
		m_GameClientView.UpdateGameView(NULL);
	}
	else 
	{
		//��������
		OnPassCard(0,0);
	}
	return 0;
}

//��ɫ����
LRESULT CGameClientDlg::OnSortByColor(WPARAM wParam, LPARAM lParam)
{
	return 0;
}
//��С����
LRESULT CGameClientDlg::OnSortByValue(WPARAM wParam, LPARAM lParam)
{
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
//�Ϲܿ���
LRESULT CGameClientDlg::OnStusteeControl(WPARAM wParam, LPARAM lParam)
{
	//���ñ���
	m_wTimeOutCount=0;

	//����״̬
	if (m_bStustee==true)
	{
		m_bStustee=false;
		m_GameClientView.m_btStusteeControl.SetButtonImage(IDB_BT_START_TRUSTEE,AfxGetInstanceHandle(),false);
		InsertSystemString(_T("��ȡ�����йܹ���."));
		CMD_C_Trustee Trustee;
		Trustee.bTrustee = false;
		SendData(SUB_C_TRUSTEE,&Trustee,sizeof(Trustee));

	}
	else
	{
		m_bStustee=true;
		m_GameClientView.m_btStusteeControl.SetButtonImage(IDB_BT_STOP_TRUSTEE,AfxGetInstanceHandle(),false);
		InsertSystemString(_T("��ѡ�����йܹ���."));
		CMD_C_Trustee Trustee;
		Trustee.bTrustee = true;
		SendData(SUB_C_TRUSTEE,&Trustee,sizeof(Trustee));
	}

	return 0;
}


//���ų�������
void CGameClientDlg::PlayBombSound(WORD wChairID, BYTE cbStarLevel)
{
	if(wChairID < 0 || wChairID > 3)
	{
		return;
	}
	if(cbStarLevel<4||cbStarLevel>12)
	{
		return;
	}

	//�ж��Ա�
	const tagUserData* pUserData = GetUserData(wChairID);
	if(pUserData == 0)
	{
		assert(0 && "�ò��������Ϣ");
		return;
	}
	bool bBoy = (pUserData->cbGender == 1 ? false : true);
	CString strSoundName;
	strSoundName.Format(_T("%d"),cbStarLevel);
	if(bBoy)
	{
		strSoundName = _T("BOY_") +strSoundName;
	}
	else
	{
		strSoundName = _T("GIRL_") + strSoundName;
	}
	PlayGameSound(AfxGetInstanceHandle(), strSoundName);
}

//�û��й�
bool CGameClientDlg::OnSubTrustee(const void * pBuffer,WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_Trustee));
	if (wDataSize!=sizeof(CMD_S_Trustee)) return false;

	//��Ϣ����
	CMD_S_Trustee * pTrustee=(CMD_S_Trustee *)pBuffer;
	m_GameClientView.SetTrustee(SwitchViewChairID(pTrustee->wChairID),pTrustee->bTrustee);
	if ((IsLookonMode()==true)||(pTrustee->wChairID!=GetMeChairID()))
	{
		const tagUserData * pUserData=GetUserData(pTrustee->wChairID);
		TCHAR szBuffer[256];
		if(pTrustee->bTrustee==true)
			_snprintf(szBuffer,sizeof(szBuffer),TEXT("���[%s]ѡ�����йܹ���."),pUserData->szName);
		else
			_snprintf(szBuffer,sizeof(szBuffer),TEXT("���[%s]ȡ�����йܹ���."),pUserData->szName);
		InsertSystemString(szBuffer);
	}
	return true;
}
//////////////////////////////////////////////////////////////////////////
