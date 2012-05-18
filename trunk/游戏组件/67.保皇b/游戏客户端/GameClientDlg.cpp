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

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientDlg, CGameFrameDlg)
	ON_WM_TIMER()
	ON_MESSAGE(IDM_START,OnStart)
	ON_MESSAGE(IDM_HOUSECARL,OnHousecarl)
	ON_MESSAGE(IDM_EMPEROR,OnEmperor)
	ON_MESSAGE(IDM_OUT_CARD,OnOutCard)
	ON_MESSAGE(IDM_PASS_CARD,OnPassCard)
	ON_MESSAGE(IDM_AUTO_OUTCARD,OnAutoOutCard)
	ON_MESSAGE(IDM_LEFT_HIT_CARD,OnLeftHitCard)
	ON_MESSAGE(IDM_RIGHT_HIT_CARD,OnRightHitCard)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientDlg::CGameClientDlg() : CGameFrameDlg(&m_GameClientView)
{
	//��Ϸ����
	
	m_bHandCardCount=0;
	m_bHintCardCount=0;
	memset(m_bCardCount,0,sizeof(m_bCardCount));
	memset(m_bHandCardData,0,sizeof(m_bHandCardData));

	//���ñ���
	m_bDeasilOrder=false;
	m_dwCardHSpace=DEFAULT_PELS;

	//���Ʊ���
	m_bTurnCardCount=0;
	m_bTurnOutType=CT_INVALID;
	memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));
	memset(m_bHintCardData,0,sizeof(m_bHintCardData));

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
	SetWindowText(TEXT("���� ���ٳ�(��3,4,5)"));

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
	m_GameClientView.m_HandCardControl.SetCardSpace(m_dwCardHSpace,0,20);

#ifdef DEBUG_GAME

	BYTE bData[20]={67,1,2,3,4,20,7,27,39,8,24,40,9,25,10,26,11,27,43,29};

	m_bHandCardCount=20;

	m_GameLogic.SortCardList(bData,m_bHandCardCount);
	CopyMemory(m_bHandCardData,bData,m_bHandCardCount);
	m_GameClientView.m_HandCardControl.SetCardData(m_bHandCardData,m_bHandCardCount);
	BYTE bOutData[2]={6,22};
	m_bTurnCardCount=sizeof(bOutData);
	CopyMemory(m_bTurnCardData,bOutData,m_bTurnCardCount);
	m_bTurnOutType=m_GameLogic.GetCardType(m_bTurnCardData,m_bTurnCardCount);
	//char *str=new char[100];
	//sprintf(str,"%d",m_bTurnOutType);
	//MessageBox(str);
	//delete []str;
	//m_bTurn
	ActiveGameFrame();
	m_GameClientView.m_btOutCard.EnableWindow(FALSE);
	m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
	m_GameClientView.m_btPassCard.EnableWindow(FALSE);
	m_GameClientView.m_btPassCard.ShowWindow(SW_SHOW);
	m_GameClientView.m_btAutoOutCard.ShowWindow(SW_SHOW);
	m_GameClientView.m_btAutoOutCard.EnableWindow(TRUE);	
	m_GameClientView.m_HandCardControl.SetPositively(true);
	m_GameClientView.m_HandCardControl.SetDisplayFlag(true);
#endif
	return true;
}

//���ÿ��
void CGameClientDlg::ResetGameFrame()
{
	//��Ϸ����
	
	m_bHandCardCount=0;
	m_bHintCardCount=0;
	memset(m_bCardCount,0,sizeof(m_bCardCount));
	memset(m_bHandCardData,0,sizeof(m_bHandCardData));

	//���Ʊ���
	m_bTurnCardCount=0;
	m_bTurnOutType=CT_INVALID;
	memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));
	memset(m_bHandCardData,0,sizeof(m_bHintCardData));

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
		m_GameClientView.m_HandCardControl.SetCardSpace(m_dwCardHSpace,0,20);

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
			//��ʱ�ж�
			if (nElapse==0)
			{
				if ((IsLookonMode()==false)&&(wChairID==GetMeChairID())) AutomatismOutCard();
				return false;
			}

			//��������
			if (m_bHandCardCount<m_bTurnCardCount) return true;
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
			m_GameClientView.m_btStart.ShowWindow( SW_SHOW);

			//���ÿؼ�
			if (IsLookonMode()==false)
			{
				m_GameClientView.m_btStart.ShowWindow(TRUE);
				m_GameClientView.m_btStart.SetFocus();
			}

			//�����˿�
			if (IsLookonMode()==false) m_GameClientView.m_HandCardControl.SetDisplayFlag(true);

			return true;
		}
	case GS_WK_PLAYING:		//��Ϸ״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pBuffer;

			//���ñ���
			m_bTurnCardCount=pStatusPlay->bTurnCardCount;
			m_bHandCardCount=pStatusPlay->bCardCount[GetMeChairID()];
			m_bTurnOutType=m_GameLogic.GetCardType(pStatusPlay->bTurnCardData,m_bTurnCardCount);
			CopyMemory(m_bHandCardData,pStatusPlay->bCardData,m_bHandCardCount);
			CopyMemory(m_bTurnCardData,pStatusPlay->bTurnCardData,pStatusPlay->bTurnCardCount);

			//���ý���
			for (BYTE i=0;i<GAME_PLAYER;i++)
			{
				WORD wViewChairID=SwitchViewChairID(i);
				m_bCardCount[i]=pStatusPlay->bCardCount[i];
				m_GameClientView.SetCardCount(wViewChairID,pStatusPlay->bCardCount[i]);
			}
			m_GameClientView.m_HandCardControl.SetCardData(m_bHandCardData,m_bHandCardCount);
			
			//�������
			if ((IsLookonMode()==false)&&(pStatusPlay->wCurrentUser==GetMeChairID()))
			{
				m_GameClientView.m_btOutCard.EnableWindow(FALSE);
				m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
				m_GameClientView.m_btPassCard.ShowWindow(SW_SHOW);
				m_GameClientView.m_btAutoOutCard.ShowWindow(SW_SHOW);
				m_GameClientView.m_btPassCard.EnableWindow((m_bTurnCardCount!=0)?TRUE:FALSE);
				m_GameClientView.m_btAutoOutCard.EnableWindow((m_bTurnCardCount!=0)?TRUE:FALSE);
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
			if (IsLookonMode()==false) 
			{
				m_GameClientView.m_HandCardControl.SetPositively(true);
				m_GameClientView.m_HandCardControl.SetDisplayFlag(true);
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

	//��������
	m_bHandCardCount=CountArray(pSendCard->bCardData);
	CopyMemory(m_bHandCardData,pSendCard->bCardData,sizeof(pSendCard->bCardData));
	for (WORD i=0;i<GAME_PLAYER;i++) m_bCardCount[i]=CountArray(pSendCard->bCardData);

	//���ý���
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.SetPassFlag(i,false);
		m_GameClientView.SetCardCount(i,m_bCardCount[i]);
		m_GameClientView.m_UserCardControl[i].SetCardData(NULL,0);
	}
	if (IsLookonMode()==true)
	{
		m_GameClientView.m_ScoreView.ShowWindow(SW_HIDE);
	
	}
	if (IsLookonMode()==true) m_GameClientView.m_HandCardControl.SetDisplayFlag(false);

	//��ǰ���
	if ((IsLookonMode()==false))//&&(pSendCard->wCurrentUser==GetMeChairID()))
	{
		ActiveGameFrame();
		m_GameClientView.m_HandCardControl.SetCardData(m_bHandCardData,m_bHandCardCount);
		if (pSendCard->wCurrentEnthronement == GetMeChairID())//�ʵ�
		{
			if (pSendCard->wCurrentHousecarl  == GetMeChairID())//����
			{
				CMD_C_ChairID ChairID;
				ChairID.IsChair = pSendCard->wCurrentUser;
				ChairID.IsGet = 0;
				SendData(SUB_C_CHAIRID,&ChairID,sizeof(ChairID));
				m_GameClientView.m_wCurrentHousecarl = pSendCard->wCurrentHousecarl;				
			}
			else
			{
				CMD_C_ChairID ChairID;
				ChairID.IsChair = pSendCard->wCurrentUser;
				ChairID.IsGet = 1;
				SendData(SUB_C_CHAIRID,&ChairID,sizeof(ChairID));
				m_GameClientView.m_wCurrentEnthronement = pSendCard->wCurrentEnthronement;		
			}
						
		}
		else if (pSendCard->wCurrentHousecarl == GetMeChairID())//����
		{
			CMD_C_ChairID ChairID;
			ChairID.IsChair = pSendCard->wCurrentUser;
			ChairID.IsGet = 0;
			SendData(SUB_C_CHAIRID,&ChairID,sizeof(ChairID));
			m_GameClientView.m_wCurrentHousecarl = pSendCard->wCurrentHousecarl;		
			
		}
		
	}
	m_GameClientView.m_wCurrentEnthronement = SwitchViewChairID(pSendCard->wCurrentEnthronement);	
	m_GameClientView.m_wCurrentHousecarl = SwitchViewChairID(pSendCard->wCurrentHousecarl);		
	//��������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));

	return OnSubGameStart(pBuffer,wDataSize);

	return true;
}
//��Ϸ��ʼ
bool CGameClientDlg::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_SendCard));
	if (wDataSize!=sizeof(CMD_S_SendCard)) return false;

	//��Ϣ����
	CMD_S_SendCard * pGameStart=(CMD_S_SendCard *)pBuffer;

	//���ñ���

	m_bTurnCardCount=0;
	m_bTurnOutType=CT_INVALID;
	ZeroMemory(m_bTurnCardData,sizeof(m_bTurnCardData));

	//���ý���
	m_GameClientView.SetCardCount(SwitchViewChairID(pGameStart->wCurrentEnthronement),m_bCardCount[pGameStart->wCurrentEnthronement]);

	////��������
	//if (pGameStart->wCurrentEnthronement==GetMeChairID())
	//{
	//	m_GameClientView.m_HandCardControl.SetCardData(m_bHandCardData,m_bHandCardCount);
	//}

	//�������
	if (IsLookonMode()==false) m_GameClientView.m_HandCardControl.SetPositively(true);

	//��ǰ���
	if ((IsLookonMode()==false)&&(pGameStart->wCurrentUser==GetMeChairID()))
	{
		ActiveGameFrame();
		m_GameClientView.m_btOutCard.EnableWindow(FALSE);
		m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btPassCard.EnableWindow(FALSE);
		m_GameClientView.m_btPassCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btAutoOutCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btAutoOutCard.EnableWindow(FALSE);
	}	
//	m_GameClientView.m_LeaveCardControl[0].SetBackCard(m_bCardCount[0]);//.SetCardData(m_GameClientView.m_byZerodata,m_bCardCount[0]);
//	m_GameClientView.m_LeaveCardControl[1].SetBackCard(m_bCardCount[1]);//.SetCardData(m_GameClientView.m_byZerodata,m_bCardCount[1]);
	//��������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));

	//����ʱ��
	SetGameTimer(pGameStart->wCurrentUser,IDI_OUT_CARD,30);
	m_GameClientView.m_bGamePlaying = true;
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
	WORD wOutViewChairID=SwitchViewChairID(pOutCard->wOutCardUser);
	m_bCardCount[pOutCard->wOutCardUser]-=pOutCard->bCardCount;
	m_GameClientView.SetCardCount(wOutViewChairID,m_bCardCount[pOutCard->wOutCardUser]);
	for (BYTE i = 0; i < pOutCard->bCardCount; i++)
	{
		if (pOutCard->bCardData[i] == 0x43)
			m_GameClientView.m_bOutHousecarl = true;
	}
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
	//if ((IsLookonMode()==true)&&(pOutCard->wOutCardUser==GetMeChairID()))
	//{
	//	//ɾ���˿� 
	//	BYTE bSourceCount=m_bHandCardCount;
	//	m_bHandCardCount-=pOutCard->bCardCount;
	//	m_GameLogic.RemoveCard(pOutCard->bCardData,pOutCard->bCardCount,m_bHandCardData,bSourceCount);

	//	//���ý���
	//	m_GameClientView.m_HandCardControl.SetCardData(m_bHandCardData,m_bHandCardCount);
	//}

	//����ж�
	if (pOutCard->wCurrentUser==pOutCard->wOutCardUser)
	{
		//���ñ���
		m_bTurnCardCount=0;
		m_bTurnOutType=CT_INVALID;
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
			else
			{
				if (m_bCardCount[i] <= 0)
				{
					BYTE a = (i+1)%GAME_PLAYER;
					while(1)
					{
						if (a < GAME_PLAYER && m_bCardCount[a]>0)
						{	
							m_wMostUser=pOutCard->wCurrentUser=a;
							break;
						}
						else
							a = (a++)%GAME_PLAYER;
					}
				}
					
			}
		}
		

		//��������
		//PlayGameSound(AfxGetInstanceHandle(),TEXT("MOST_CARD"));

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
		m_GameClientView.m_btAutoOutCard.EnableWindow(TRUE);
		m_GameClientView.m_btAutoOutCard.ShowWindow(SW_SHOW);
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
		m_bTurnOutType=CT_INVALID;
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
		m_GameClientView.m_btAutoOutCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btPassCard.EnableWindow((m_bTurnCardCount>0)?TRUE:FALSE);
		m_GameClientView.m_btOutCard.EnableWindow((VerdictOutCard()==true)?TRUE:FALSE);
		m_GameClientView.m_btAutoOutCard.EnableWindow((m_bTurnCardCount>0)?TRUE:FALSE);
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

	//ɾ����ʱ��
	KillTimer(IDI_MOST_CARD);
	KillGameTimer(IDI_OUT_CARD);
	
	//���ؿؼ�
	m_GameClientView.m_btOutCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btPassCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btAutoOutCard.ShowWindow(SW_HIDE);

	//���ÿؼ�
	m_GameClientView.m_btOutCard.EnableWindow(FALSE);
	m_GameClientView.m_btPassCard.EnableWindow(FALSE);

	//���û���
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		const tagUserData * pUserData=GetUserData(i);
		m_GameClientView.m_ScoreView.SetGameScore(i,pUserData->szName,pGameEnd->lGameScore[i]);
	}
	//m_GameClientView.m_LeaveCardControl[0].SetDisplayFlag(true);
	//m_GameClientView.m_LeaveCardControl[1].SetDisplayFlag(true);

	m_GameClientView.m_ScoreView.SetGameTax(pGameEnd->lGameTax);
	m_GameClientView.m_ScoreView.ShowWindow(SW_SHOW);

	//�����˿�
	BYTE bCardPos=0;
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		WORD wViewChairID=SwitchViewChairID(i);
		
		bCardPos+=pGameEnd->bCardCount[i];
		if (pGameEnd->bCardCount[i]!=0)
		{
			m_GameClientView.SetPassFlag(wViewChairID,false);
			m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(NULL,0);
		}
	}

	//��ʾ�˿�
	if (IsLookonMode()==true) m_GameClientView.m_HandCardControl.SetDisplayFlag(true);

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
	m_GameClientView.m_wCurrentEnthronement = -1;				
	m_GameClientView.m_wCurrentHousecarl = -1;		
	m_GameClientView.m_bGamePlaying = false;
	m_GameClientView.m_bOutHousecarl = false;
	return true;
}

//�����ж�
bool CGameClientDlg::VerdictOutCard()
{
	//״̬�ж�
	if (m_GameClientView.m_btOutCard.IsWindowVisible()==FALSE) return false;

	//��ȡ�˿�
	BYTE bCardData[PLAYER_CARDS];
	BYTE bShootCount=(BYTE)m_GameClientView.m_HandCardControl.GetShootCard(bCardData,CountArray(bCardData));

	//�����ж�
	if (bShootCount>0L)
	{
		//��������
		BYTE bCardType=m_GameLogic.GetCardType(bCardData,bShootCount);

		//�����ж�
		if (bCardType==CT_INVALID) return false;

		//�����ж�
		if (m_bTurnCardCount==0) return true;
		return m_GameLogic.CompareCard(bCardData,m_bTurnCardData,bShootCount,m_bTurnCardCount);
	}

	return false;
}

//�Զ�����
bool CGameClientDlg::AutomatismOutCard()
{
	//�ȳ�����
	if (m_bTurnCardCount==0)
	{
		//���ƽ���
		KillGameTimer(IDI_OUT_CARD);
		m_GameClientView.m_btOutCard.ShowWindow(SW_HIDE);
		m_GameClientView.m_btOutCard.EnableWindow(FALSE);
		m_GameClientView.m_btPassCard.ShowWindow(SW_HIDE);
		m_GameClientView.m_btPassCard.EnableWindow(FALSE);
		m_GameClientView.m_btAutoOutCard.ShowWindow(SW_HIDE);
		m_GameClientView.m_btAutoOutCard.EnableWindow(FALSE);

		//��������
		CMD_C_OutCard OutCard;
		OutCard.bCardCount=1;
		OutCard.bCardData[0]=m_bHandCardData[m_bHandCardCount-1];
		SendData(SUB_C_OUT_CART,&OutCard,sizeof(OutCard)-sizeof(OutCard.bCardData)+OutCard.bCardCount*sizeof(BYTE));

		//Ԥ�ȴ���
		PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));
		m_GameClientView.m_UserCardControl[2].SetCardData(OutCard.bCardData,OutCard.bCardCount);

		//Ԥ��ɾ��
		BYTE bSourceCount=m_bHandCardCount;
		m_bHandCardCount-=OutCard.bCardCount;
		m_GameLogic.RemoveCard(OutCard.bCardData,OutCard.bCardCount,m_bHandCardData,bSourceCount);
		m_GameClientView.m_HandCardControl.SetCardData(m_bHandCardData,m_bHandCardCount);
	}
	else //OnPassCard(0,0);
		AutoOutCard111(0);

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
			m_GameClientView.m_btAutoOutCard.ShowWindow(SW_SHOW);
			m_GameClientView.m_btAutoOutCard.EnableWindow(FALSE);
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
	m_bHandCardCount=0;
	m_bTurnCardCount=0;
	m_bTurnOutType=CT_INVALID;
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
	m_GameClientView.m_HandCardControl.SetCardData(NULL,0);
	m_GameClientView.m_HandCardControl.SetPositively(false);
//	m_GameClientView.m_LeaveCardControl[0].SetCardData(NULL,0);
//	m_GameClientView.m_LeaveCardControl[1].SetCardData(NULL,0);
	for (WORD i=0;i<GAME_PLAYER;i++) m_GameClientView.m_UserCardControl[i].SetCardData(NULL,0);

	//������Ϣ
	SendUserReady(NULL,0);
	PlayGameSound(AfxGetInstanceHandle(),TEXT("Ready"));
	return 0;
}

//������Ϣ
LRESULT CGameClientDlg::OnOutCard(WPARAM wParam, LPARAM lParam)
{
	//״̬�ж�
	if ((m_GameClientView.m_btOutCard.IsWindowEnabled()==FALSE)||
		(m_GameClientView.m_btOutCard.IsWindowVisible()==FALSE)) return 0;

	memset(m_bHintCardData,0,sizeof(m_bHintCardData));
	//���ý���
	KillGameTimer(IDI_OUT_CARD);
	m_GameClientView.m_btOutCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btOutCard.EnableWindow(FALSE);
	m_GameClientView.m_btPassCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btPassCard.EnableWindow(FALSE);
	m_GameClientView.m_btAutoOutCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btAutoOutCard.EnableWindow(FALSE);

	//��������
	CMD_C_OutCard OutCard;
	OutCard.bCardCount=(BYTE)m_GameClientView.m_HandCardControl.GetShootCard(OutCard.bCardData,CountArray(OutCard.bCardData));
	SendData(SUB_C_OUT_CART,&OutCard,sizeof(OutCard)-sizeof(OutCard.bCardData)+OutCard.bCardCount*sizeof(BYTE));

	//Ԥ����ʾ
	PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));
	m_GameClientView.m_UserCardControl[2].SetCardData(OutCard.bCardData,OutCard.bCardCount);

	//Ԥ��ɾ��
	BYTE bSourceCount=m_bHandCardCount;
	m_bHandCardCount-=OutCard.bCardCount;
	m_GameLogic.RemoveCard(OutCard.bCardData,OutCard.bCardCount,m_bHandCardData,bSourceCount);
	m_GameClientView.m_HandCardControl.SetCardData(m_bHandCardData,m_bHandCardCount);

	return 0;
}

//��������
LRESULT CGameClientDlg::OnPassCard(WPARAM wParam, LPARAM lParam)
{
	//״̬�ж�
	if (m_GameClientView.m_btPassCard.IsWindowEnabled()==FALSE) return 0;

	memset(m_bHintCardData,0,sizeof(m_bHintCardData));
	//���ý���
	KillGameTimer(IDI_OUT_CARD);
	m_GameClientView.m_btOutCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btOutCard.EnableWindow(FALSE);
	m_GameClientView.m_btPassCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btPassCard.EnableWindow(FALSE);
	m_GameClientView.m_btAutoOutCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btAutoOutCard.EnableWindow(FALSE);

	//��������
	SendData(SUB_C_PASS_CARD);

	//Ԥ����ʾ
	m_GameClientView.SetPassFlag(1,true);
	PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));
	m_GameClientView.m_HandCardControl.SetCardData(m_bHandCardData,m_bHandCardCount);

	return 0;
}

//������ʾ
LRESULT CGameClientDlg::OnAutoOutCard(WPARAM wParam, LPARAM lParam)
{
	AutoOutCard(0);
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

//��λ
LRESULT CGameClientDlg::OnEmperor(WPARAM wParam, LPARAM lParam)
{
	CMD_C_ChairID ChairID;
	ChairID.IsChair = (BYTE)wParam;
	ChairID.IsGet = (BYTE)lParam;
	SendData(SUB_C_CHAIRID,&ChairID,sizeof(ChairID));
	return 0;
}
//����
LRESULT CGameClientDlg::OnHousecarl(WPARAM wParam, LPARAM lParam)
{
	CMD_C_ChairID ChairID;
	ChairID.IsChair = (BYTE)wParam;
	ChairID.IsGet = (BYTE)lParam;
	SendData(SUB_C_CHAIRID,&ChairID,sizeof(ChairID));
	return 0;
}
//����˿�
LRESULT CGameClientDlg::OnRightHitCard(WPARAM wParam, LPARAM lParam)
{
	//�û�����
	OnOutCard(0,0);

	return 0;
}

void CGameClientDlg::AutoOutCard(BYTE WhichOnsKindCard)
{
	if(m_bHintCardData[0]==0)
	{
		m_bHintCardCount=m_bTurnCardCount;
		CopyMemory(m_bHintCardData,m_bTurnCardData,m_bTurnCardCount);
	}
	//���û���˳��ƣ�����ʾ
	if(m_bTurnCardCount==0)
		return;

	BYTE							bTempSCardCount = 0;						//�˿���Ŀ
	BYTE							bTempSCardData[PLAYER_CARDS];				//�����˿�
	BYTE							bTempCCardData[PLAYER_CARDS];				//�����˿�
	BYTE							TurnCard = 0;
	BYTE							TempCard = 0;
	BYTE							DXCard = 0;
	memset(bTempSCardData,0,sizeof(bTempSCardData));
	memset(bTempCCardData,0,sizeof(bTempCCardData));
	TurnCard = m_GameLogic.GetCardLogicValue(m_bTurnCardData[0]);
	TempCard = m_GameLogic.GetCardLogicValue(m_bHandCardData[m_bHandCardCount-1]);
	m_GameClientView.m_HandCardControl.ShootAllCard(false);

	for(BYTE i=m_bHandCardCount-1;i>0;i--)
	{	
		BYTE GetCard=m_GameLogic.GetCardLogicValue(m_bHandCardData[i]);
		BYTE GetCardCount = 0;
		
		if(TurnCard < GetCard)
		{
			if (TempCard == GetCard)
			{
				bTempSCardData[bTempSCardCount]=m_bHandCardData[i];
				bTempCCardData[bTempSCardCount]=i;
				bTempSCardCount++;
				if (bTempSCardCount>0 && bTempSCardCount == m_bTurnCardCount)
				{
					m_bHintCardCount = bTempSCardCount;
					memset(m_bHintCardData,0,sizeof(m_bHintCardData));
					CopyMemory(m_bHintCardData,bTempSCardData,m_bHintCardCount);
					m_GameClientView.m_HandCardControl.SetShootCard(bTempCCardData,m_bHintCardCount);
					m_GameClientView.m_btOutCard.EnableWindow(TRUE);
					return;
				}
			}
			else
			{
				if (i>0) i++;
				TempCard=GetCard;
				memset(bTempSCardData,0,sizeof(bTempSCardData));
				memset(bTempCCardData,0,sizeof(bTempCCardData));
				bTempSCardCount = 0;
			}
		}
	}
	if (m_bTurnCardCount == 1)
	{
		TurnCard = m_GameLogic.GetCardLogicValue(m_bTurnCardData[0]);
		TempCard = m_GameLogic.GetCardLogicValue(m_bHandCardData[0]);
		if(TurnCard < TempCard)
		{
			bTempSCardData[0]=m_bHandCardData[0];
			bTempCCardData[0]=0;
			m_bHintCardCount = 1;
			memset(m_bHintCardData,0,sizeof(m_bHintCardData));
			CopyMemory(m_bHintCardData,bTempSCardData,m_bHintCardCount);
			m_GameClientView.m_HandCardControl.SetShootCard(bTempCCardData,m_bHintCardCount);
			m_GameClientView.m_btOutCard.EnableWindow(TRUE);
			return;
		}
	}
	//return;
	memset(bTempSCardData,0,sizeof(bTempSCardData));
	memset(bTempCCardData,0,sizeof(bTempCCardData));
	TurnCard = m_GameLogic.GetCardLogicValue(m_bTurnCardData[0]);
	TempCard = m_GameLogic.GetCardLogicValue(m_bHandCardData[m_bHandCardCount-1]);
	m_GameClientView.m_HandCardControl.ShootAllCard(false);

	for (BYTE i = 0; i < 8; i++)
	{
		if (m_GameLogic.GetCardLogicValue(m_bHandCardData[i]) > 15)
			DXCard++;
		else
			break;
	}
	if (DXCard <= 0) return;
	for(BYTE i=m_bHandCardCount-1;i>0;i--)
	{	
		BYTE GetCard=m_GameLogic.GetCardLogicValue(m_bHandCardData[i]);
		BYTE GetCardCount = 0;
		
		if(TurnCard < GetCard)
		{
			if (TempCard == GetCard)
			{
				bTempSCardData[bTempSCardCount]=m_bHandCardData[i];
				bTempCCardData[bTempSCardCount]=i;
				bTempSCardCount++;
				
			}
			else
			{
				if (bTempSCardCount > 0)
				{
					BYTE b = m_bTurnCardCount - bTempSCardCount;
					if (b > DXCard) break;
					if (DXCard > 0)
					{
						for (BYTE c = DXCard-1; c > 0; c--)
						{
							BYTE CardValue=m_GameLogic.GetCardLogicValue(m_bHandCardData[c]);
							if (CardValue > 15)
							{
								bTempSCardData[bTempSCardCount]=m_bHandCardData[c];
								bTempCCardData[bTempSCardCount]=c;
								bTempSCardCount++;
								if (bTempSCardCount>0 && bTempSCardCount == m_bTurnCardCount)
								{
									m_bHintCardCount = bTempSCardCount;
									m_GameLogic.SortCardList(bTempSCardData,bTempSCardCount);
									CopyMemory(m_bHintCardData,bTempSCardData,m_bHintCardCount);
									m_GameClientView.m_HandCardControl.SetShootCard(bTempCCardData,m_bHintCardCount);
									m_GameClientView.m_btOutCard.EnableWindow(TRUE);
									return;
								}
							}
						}
					}
				}
				//i++;
				TempCard=GetCard;
				memset(bTempSCardData,0,sizeof(bTempSCardData));
				memset(bTempCCardData,0,sizeof(bTempCCardData));
				bTempSCardCount = 0;
			}
		}
	}

}

void CGameClientDlg::AutoOutCard111(BYTE WhichOnsKindCard)
{
	if(m_bHintCardData[0]==0)
	{
		m_bHintCardCount=m_bTurnCardCount;
		CopyMemory(m_bHintCardData,m_bTurnCardData,m_bTurnCardCount);
	}
	//���û���˳��ƣ�����ʾ
	if(m_bTurnCardCount==0)
		return;

	BYTE							bTempSCardCount = 0;							//�˿���Ŀ
	BYTE							bTempSCardData[PLAYER_CARDS];				//�����˿�
	BYTE							bTempCCardData[PLAYER_CARDS];				//�����˿�
	BYTE							TurnCard = 0;
	BYTE							TempCard = 0;
	BYTE							DXCard = 0;
	memset(bTempSCardData,0,sizeof(bTempSCardData));
	memset(bTempCCardData,0,sizeof(bTempCCardData));
	TurnCard = m_GameLogic.GetCardLogicValue(m_bTurnCardData[0]);
	TempCard = m_GameLogic.GetCardLogicValue(m_bHandCardData[m_bHandCardCount-1]);
	m_GameClientView.m_HandCardControl.ShootAllCard(false);

	for(BYTE i=m_bHandCardCount-1;i>0;i--)
	{	
		BYTE GetCard=m_GameLogic.GetCardLogicValue(m_bHandCardData[i]);
		BYTE GetCardCount = 0;
		
		if(TurnCard < GetCard)
		{
			if (TempCard == GetCard)
			{
				bTempSCardData[bTempSCardCount]=m_bHandCardData[i];
				bTempCCardData[bTempSCardCount]=i;
				bTempSCardCount++;
				if (bTempSCardCount  > 0 && bTempSCardCount == m_bTurnCardCount)
				{
					m_bHintCardCount = bTempSCardCount;
					memset(m_bHintCardData,0,sizeof(m_bHintCardData));
					CopyMemory(m_bHintCardData,bTempSCardData,m_bHintCardCount);
					//��������
					CMD_C_OutCard OutCard;
					OutCard.bCardCount=bTempSCardCount;
					CopyMemory(OutCard.bCardData,bTempSCardData,bTempSCardCount);
					SendData(SUB_C_OUT_CART,&OutCard,sizeof(OutCard)-sizeof(OutCard.bCardData)+OutCard.bCardCount*sizeof(BYTE));

					//Ԥ�ȴ���
					PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));
					m_GameClientView.m_UserCardControl[2].SetCardData(OutCard.bCardData,OutCard.bCardCount);

					//Ԥ��ɾ��
					BYTE bSourceCount=m_bHandCardCount;
					m_bHandCardCount-=OutCard.bCardCount;
					m_GameLogic.RemoveCard(OutCard.bCardData,OutCard.bCardCount,m_bHandCardData,bSourceCount);
					m_GameClientView.m_HandCardControl.SetCardData(m_bHandCardData,m_bHandCardCount);

					m_GameClientView.m_btOutCard.ShowWindow(SW_HIDE);
					m_GameClientView.m_btPassCard.ShowWindow(SW_HIDE);
					m_GameClientView.m_btAutoOutCard.ShowWindow(SW_HIDE);
					////////
					//m_GameClientView.m_HandCardControl.SetShootCard(bTempCCardData,m_bHintCardCount);
					//m_GameClientView.m_btOutCard.EnableWindow(TRUE);
					return;
				}
			}
			else
			{
				if (i>0) i++;
				TempCard=GetCard;
				memset(bTempSCardData,0,sizeof(bTempSCardData));
				memset(bTempCCardData,0,sizeof(bTempCCardData));
				bTempSCardCount = 0;
			}
		}
	}
	if (m_bTurnCardCount == 1)
	{
		TurnCard = m_GameLogic.GetCardLogicValue(m_bTurnCardData[0]);
		TempCard = m_GameLogic.GetCardLogicValue(m_bHandCardData[0]);
		if(TurnCard < TempCard)
		{
			bTempSCardData[0]=m_bHandCardData[0];
			bTempCCardData[0]=0;
			m_bHintCardCount = 1;
			memset(m_bHintCardData,0,sizeof(m_bHintCardData));
			CopyMemory(m_bHintCardData,bTempSCardData,m_bHintCardCount);
			m_GameClientView.m_HandCardControl.SetShootCard(bTempCCardData,m_bHintCardCount);
			m_GameClientView.m_btOutCard.EnableWindow(TRUE);
			return;
		}
	}
	// OnPassCard(0,0);
	// return;
	memset(bTempSCardData,0,sizeof(bTempSCardData));
	memset(bTempCCardData,0,sizeof(bTempCCardData));
	TurnCard = m_GameLogic.GetCardLogicValue(m_bTurnCardData[0]);
	TempCard = m_GameLogic.GetCardLogicValue(m_bHandCardData[m_bHandCardCount-1]);
	m_GameClientView.m_HandCardControl.ShootAllCard(false);

	for (BYTE i = 0; i < 8; i++)
	{
		if (m_GameLogic.GetCardLogicValue(m_bHandCardData[i]) > 15)
			DXCard++;
		else
			break;
	}
	if (DXCard <= 0) OnPassCard(0,0);
	for(BYTE i=m_bHandCardCount-1;i>0;i--)
	{	
		BYTE GetCard=m_GameLogic.GetCardLogicValue(m_bHandCardData[i]);
		BYTE GetCardCount = 0;
		
		if(TurnCard < GetCard)
		{
			if (TempCard == GetCard)
			{
				bTempSCardData[bTempSCardCount]=m_bHandCardData[i];
				bTempCCardData[bTempSCardCount]=i;
				bTempSCardCount++;
				
			}
			else
			{
				if (bTempSCardCount > 0)
				{
					BYTE b = m_bTurnCardCount - bTempSCardCount;
					if (b > DXCard) break;
					if (DXCard > 0)
					{
						for (BYTE c = DXCard-1; c > 0; c--)
						{
							BYTE CardValue=m_GameLogic.GetCardLogicValue(m_bHandCardData[c]);
							if (CardValue > 15)
							{
								bTempSCardData[bTempSCardCount]=m_bHandCardData[c];
								bTempCCardData[bTempSCardCount]=c;
								bTempSCardCount++;
								if (bTempSCardCount  > 0 && bTempSCardCount == m_bTurnCardCount)
								{
									m_bHintCardCount = bTempSCardCount;
									CopyMemory(m_bHintCardData,bTempSCardData,m_bHintCardCount);
									//��������
									CMD_C_OutCard OutCard;
									OutCard.bCardCount=bTempSCardCount;
									CopyMemory(OutCard.bCardData,bTempSCardData,bTempSCardCount);
									SendData(SUB_C_OUT_CART,&OutCard,sizeof(OutCard)-sizeof(OutCard.bCardData)+OutCard.bCardCount*sizeof(BYTE));

									//Ԥ�ȴ���
									PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));
									m_GameClientView.m_UserCardControl[2].SetCardData(OutCard.bCardData,OutCard.bCardCount);

									//Ԥ��ɾ��
									BYTE bSourceCount=m_bHandCardCount;
									m_bHandCardCount-=OutCard.bCardCount;
									m_GameLogic.RemoveCard(OutCard.bCardData,OutCard.bCardCount,m_bHandCardData,bSourceCount);
									m_GameClientView.m_HandCardControl.SetCardData(m_bHandCardData,m_bHandCardCount);
									
									m_GameClientView.m_btOutCard.ShowWindow(SW_HIDE);
									m_GameClientView.m_btPassCard.ShowWindow(SW_HIDE);
									m_GameClientView.m_btAutoOutCard.ShowWindow(SW_HIDE);
									return;
								}
							}
						}
					}
				}
				//i++;
				TempCard=GetCard;
				memset(bTempSCardData,0,sizeof(bTempSCardData));
				memset(bTempCCardData,0,sizeof(bTempCCardData));
				bTempSCardCount = 0;
			}
		}
	}
	OnPassCard(0,0);
}

//////////////////////////////////////////////////////////////////////////

