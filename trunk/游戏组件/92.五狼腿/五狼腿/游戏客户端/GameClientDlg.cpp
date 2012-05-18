#include "Stdafx.h"
#include "GameOption.h"
#include "GameClient.h"
#include "GameClientDlg.h"
#include ".\gameclientdlg.h"

//////////////////////////////////////////////////////////////////////////
//�궨��

//��Ϸ��ʱ��
#define IDI_OUT_CARD					200								//���ƶ�ʱ��
#define IDI_MOST_CARD					201								//���ʱ��
#define IDI_LAST_TURN					302								//���ֶ�ʱ��
#define IDI_START_GAME					203								//��ʼ��ʱ��

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientDlg, CGameFrameDlg)
	ON_WM_TIMER()
	ON_MESSAGE(IDM_START,OnStart)
	ON_MESSAGE(IDM_OUT_CARD,OnOutCard)
	ON_MESSAGE(IDM_PASS_CARD,OnPassCard)
	ON_MESSAGE(IDM_OUT_PROMPT,OnOutPrompt)
	ON_MESSAGE(IDM_LEFT_HIT_CARD,OnLeftHitCard)
	ON_MESSAGE(IDM_RIGHT_HIT_CARD,OnRightHitCard)
	ON_MESSAGE(IDM_TRUSTEE_CONTROL,OnStusteeControl)
	ON_MESSAGE(IDM_LAST_TURN_CARD,OnMessageLastTurn)
	ON_MESSAGE(IDM_SORT_CARD,OnSortCard)
	ON_MESSAGE(WM_ENABLE_HISTORY,OnMessageEnableHistory)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientDlg::CGameClientDlg() : CGameFrameDlg(&m_GameClientView)
{
	//��Ϸ����
	memset(m_bCardCount,0,sizeof(m_bCardCount));
	ZeroMemory(m_lGameScore,sizeof(m_lGameScore));
	memset(m_bHandCardData,0,sizeof(m_bHandCardData));
	memset(m_bHandCardCount,0,sizeof(m_bHandCardCount));

	//���ñ���
	m_cbSortType=ST_VIEW;
	m_bLastTurn=false;
	m_bDeasilOrder=false;
	m_dwCardHSpace=DEFAULT_PELS;

	//���Ʊ���
	m_bTurnCardCount=0;
	m_bTurnOutType=CT_ERROR;
	memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));

	//��������
	m_bStustee=false;
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
	SetWindowText(TEXT("��������Ϸ  --  Ver��6.0.1.0"));

	//����ͼ��
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);

	//��ȡ����
	m_dwCardHSpace=DEFAULT_PELS;
	m_bDeasilOrder=AfxGetApp()->GetProfileInt(TEXT("GameOption"),TEXT("DeasilOrder"),FALSE)?true:false;

	//��������
	if ((m_dwCardHSpace>MAX_PELS)||(m_dwCardHSpace<LESS_PELS)) m_dwCardHSpace=DEFAULT_PELS;

	//���ÿؼ�
	m_GameClientView.SetUserOrder(m_bDeasilOrder);
	m_GameClientView.m_HandCardControl[0].SetCardSpace(m_dwCardHSpace,0,0);
	m_GameClientView.m_HandCardControl[1].SetCardSpace(m_dwCardHSpace,0,20);

	/*
	BYTE cbTurnCard[]={0x11,0x01, 0x01, 0x21};
	BYTE cbHandCard[]={0x34,0x04,0x17, 0x19,0x19, 0x29, 0x0a, 0x3b,0x3c, 0x2c,\
		0x2c, 0x1c, 0x21, 0x21, 0x01, 0x35, 0x3a, 0x3d
				};
	m_GameLogic.SortCardList(cbTurnCard,CountArray(cbTurnCard),ST_ORDER);
	m_GameLogic.SortCardList(cbHandCard,CountArray(cbHandCard),ST_ORDER);
	m_GameClientView.m_UserCardControl[0].SetCardData(cbTurnCard,CountArray(cbTurnCard));
	m_GameClientView.m_HandCardControl[1].SetCardData(cbHandCard,CountArray(cbHandCard));
	m_GameClientView.m_HandCardControl[1].SetDisplayFlag(true);

	tagOutCardResult OutCardResult;
	ZeroMemory(&OutCardResult,sizeof(OutCardResult));
	BYTE cbCardType=m_GameLogic.GetCardType(cbTurnCard,CountArray(cbTurnCard),true);
	//cbCardType = CT_ERROR;
	bool bSuccess=m_GameLogic.SearchOutCard(cbHandCard,CountArray(cbHandCard),cbTurnCard,CountArray(cbTurnCard),cbCardType,OutCardResult);
	m_GameClientView.m_UserCardControl[2].SetCardData(OutCardResult.cbResultCard,OutCardResult.cbCardCount);
	*/
	return true;
}

//���ÿ��
void CGameClientDlg::ResetGameFrame()
{
	//��Ϸ����
	memset(m_bCardCount,0,sizeof(m_bCardCount));
	ZeroMemory(m_lGameScore,sizeof(m_lGameScore));
	memset(m_bHandCardData,0,sizeof(m_bHandCardData));
	memset(m_bHandCardCount,0,sizeof(m_bHandCardCount));

	//���Ʊ���
	m_bTurnCardCount=0;
	m_bTurnOutType=CT_ERROR;
	memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));

	//��������
	m_cbSortType=ST_VIEW;
	m_bStustee=false;
	m_bLastTurn=false;
	m_wTimeOutCount=0;
	m_wMostUser=INVALID_CHAIR;

	//ɾ����ʱ
	KillGameTimer(0);
	KillTimer(IDI_LAST_TURN);
	KillTimer(IDI_MOST_CARD);

	//��ʷ����
	m_HistoryCard.ResetData();

	m_GameClientView.m_HandCardControl[0].SetPositively(false);
	m_GameClientView.m_HandCardControl[1].SetPositively(false);

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
		m_GameClientView.m_HandCardControl[0].SetCardSpace(m_dwCardHSpace,0,0);
		m_GameClientView.m_HandCardControl[1].SetCardSpace(m_dwCardHSpace,0,20);

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
			//��ʱ�����Զ��й�
			if ((m_wTimeOutCount>=3)&&(m_bStustee==false))OnStusteeControl(0,0);
			//��ʱ�ж�
			if ((nElapse==0)||((m_bStustee==true)&&(nElapse<(30))))
			{
				if ((IsLookonMode()==false)&&(wChairID==GetMeChairID())) 
				{
					if (m_bStustee==false) m_wTimeOutCount++;
					if (!m_bStustee)AutomatismOutCard();
					else 
					{
						if(m_bTurnCardCount==0)AutomatismOutCard();
						OnOutPrompt(0,0);
					}
				}
			}

			//��������
			if (m_bHandCardCount[0]<m_bTurnCardCount) return true;
			if ((nElapse<=5)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false)) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));

			return true;
		}
	case IDI_START_GAME:		//��ʼ��Ϸ
		{
			//��ʱ�����Զ��й�
			if ((m_wTimeOutCount>=3)&&(m_bStustee==false))OnStusteeControl(0,0);
			//��ʱ�ж�
			if ((nElapse==0)||((m_bStustee==true)&&(nElapse<(30))))
			{
				if ((IsLookonMode()==false)&&(wChairID==GetMeChairID()))
				{
					if (m_bStustee==false) m_wTimeOutCount++;
					OnStart(0,0);
				}
				return false;
			}
			if ((nElapse<=5)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false)) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));

			return true;
		}
	}

	return false;
}

//�Թ�״̬
void CGameClientDlg::OnLookonChanged(bool bLookonUser, const void * pBuffer, WORD wDataSize)
{
	//��ʾ����
	bool bDisplay=(IsLookonMode()==false)||(IsAllowLookon()==true);
	m_GameClientView.m_HandCardControl[0].SetDisplayFlag(bDisplay);
	m_GameClientView.m_HandCardControl[1].SetDisplayFlag(bDisplay);

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
			if (IsLookonMode()==false) 
			{
				m_GameClientView.m_btStusteeControl.EnableWindow(TRUE);
				/*m_GameClientView.m_HandCardControl[0].SetDisplayFlag(true);
				m_GameClientView.m_HandCardControl[1].SetDisplayFlag(true);*/
			}

			//��ʾ����
			bool bDisplay=(IsLookonMode()==false)||(IsAllowLookon()==true);
			m_GameClientView.m_HandCardControl[0].SetDisplayFlag(bDisplay);
			m_GameClientView.m_HandCardControl[1].SetDisplayFlag(bDisplay);

			//����ʱ��
			if (IsLookonMode()==false) SetGameTimer(GetMeChairID(),IDI_START_GAME,20);

			return true;
		}
	case GS_RF_PLAYING:		//��Ϸ״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pBuffer;

			//���ñ���
			m_bTurnCardCount=pStatusPlay->bTurnCardCount;
			m_bTurnOutType=m_GameLogic.GetCardType(pStatusPlay->bTurnCardData,m_bTurnCardCount,true);
			CopyMemory(m_bTurnCardData,pStatusPlay->bTurnCardData,pStatusPlay->bTurnCardCount);

			//�����˿�
			WORD wMeChairID=GetMeChairID();
			m_bHandCardCount[0]=pStatusPlay->bCardCount[wMeChairID];
			m_bHandCardCount[1]=pStatusPlay->bCardCount[(wMeChairID+2)%GAME_PLAYER];
			CopyMemory(m_bHandCardData[0],pStatusPlay->cbCardData[0],sizeof(pStatusPlay->cbCardData[0]));
			CopyMemory(m_bHandCardData[1],pStatusPlay->cbCardData[1],sizeof(pStatusPlay->cbCardData[1]));
			m_GameLogic.SortCardList(m_bHandCardData[0],m_bHandCardCount[0],m_cbSortType);
			m_GameLogic.SortCardList(m_bHandCardData[1],m_bHandCardCount[1],m_cbSortType);
			m_GameClientView.m_HandCardControl[1].SetCardData(m_bHandCardData[0],m_bHandCardCount[0]);
			m_GameClientView.m_HandCardControl[0].SetCardData(m_bHandCardData[1],m_bHandCardCount[1]);

			//���ý���
			for (BYTE i=0;i<GAME_PLAYER;i++)
			{
				WORD wViewChairID=SwitchViewChairID(i);
				m_bCardCount[i]=pStatusPlay->bCardCount[i];
				m_GameClientView.SetCardCount(wViewChairID,pStatusPlay->bCardCount[i]);
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

			//���ö�ʱ��
			WORD wTimeCount=30;
			SetGameTimer(pStatusPlay->wCurrentUser,IDI_OUT_CARD,wTimeCount);

			//�����˿�
			if (IsLookonMode()==false) 
			{
				m_GameClientView.m_btStusteeControl.EnableWindow(TRUE);
				m_GameClientView.m_HandCardControl[0].SetPositively(true);
				m_GameClientView.m_HandCardControl[1].SetPositively(true);
			}

			//��ʾ����
			bool bDisplay=(IsLookonMode()==false)||(IsAllowLookon()==true);
			m_GameClientView.m_HandCardControl[0].SetDisplayFlag(bDisplay);
			m_GameClientView.m_HandCardControl[1].SetDisplayFlag(bDisplay);

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
	ZeroMemory(m_lGameScore,sizeof(m_lGameScore));
	memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));

	//��ʷ��¼
	m_HistoryCard.ResetData();

	//���ý���
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.SetPassFlag(i,false);
		m_GameClientView.m_UserCardControl[i].SetCardData(NULL,0);
	}
	for (WORD i=0;i<2;i++)
	{
		m_GameClientView.m_OtherCardControl[i].SetCardData(NULL,0);
	}
	if (IsLookonMode()==true)
	{
		m_GameClientView.m_ScoreView.ShowWindow(SW_HIDE);
		m_GameClientView.SetUserTaxis(INVALID_CHAIR,0);
		m_GameClientView.SetUserScore(CountArray(m_lGameScore),0);
	}

	//�����˿�
	m_bHandCardCount[0]=CountArray(pGameStart->cbCardData[0]);
	m_bHandCardCount[1]=CountArray(pGameStart->cbCardData[1]);
	CopyMemory(m_bHandCardData[0],pGameStart->cbCardData[0],sizeof(pGameStart->cbCardData[0]));
	CopyMemory(m_bHandCardData[1],pGameStart->cbCardData[1],sizeof(pGameStart->cbCardData[1]));
	m_GameLogic.SortCardList(m_bHandCardData[0],m_bHandCardCount[0],m_cbSortType);
	m_GameLogic.SortCardList(m_bHandCardData[1],m_bHandCardCount[1],m_cbSortType);
	m_GameClientView.m_HandCardControl[1].SetCardData(m_bHandCardData[0],m_bHandCardCount[0]);
	m_GameClientView.m_HandCardControl[0].SetCardData(m_bHandCardData[1],m_bHandCardCount[1]);

	//�˿���Ŀ
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_bCardCount[i]=CountArray(pGameStart->cbCardData[0]);
		m_GameClientView.SetCardCount(SwitchViewChairID(i),m_bCardCount[i]);
	}

	//��������
	bool bPlayerMode=(IsLookonMode()==false);
	m_GameClientView.m_HandCardControl[1].SetPositively(bPlayerMode);
	m_GameClientView.m_HandCardControl[1].SetDisplayFlag(bPlayerMode);
	m_GameClientView.m_HandCardControl[0].SetDisplayFlag(bPlayerMode);

	//��ǰ���
	if ((IsLookonMode()==false)&&(pGameStart->wCurrentUser==GetMeChairID()))
	{
		if (m_bStustee==false) ActiveGameFrame();
		m_GameClientView.m_btOutCard.EnableWindow(FALSE);
		m_GameClientView.m_btPassCard.EnableWindow(FALSE);
		m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btPassCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btOutPrompt.ShowWindow(SW_SHOW);
	}

	//��������
	SetGameTimer(pGameStart->wCurrentUser,IDI_OUT_CARD,30);
	PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));

	return true;
}

//��������
void CGameClientDlg::PlaySound(BYTE Type)
{
	switch(Type)
	  {
	    case CT_BOMB:              
			            PlayGameSound(AfxGetInstanceHandle(),TEXT("WoZha"));
						break;
        case CT_DOUBLE_BOMB:           
			            PlayGameSound(AfxGetInstanceHandle(),TEXT("WoZha"));
						break;
        case CT_CRURA_FAKE:       
			            PlayGameSound(AfxGetInstanceHandle(),TEXT("DaZha")); 
						break;
        case CT_KING_LITTET:         
			            PlayGameSound(AfxGetInstanceHandle(),TEXT("DaZha"));
						break;
        case CT_KING_BIG: 
			            PlayGameSound(AfxGetInstanceHandle(),TEXT("DaZha"));
						break;
        case CT_CRURA_PURE: 
			            PlayGameSound(AfxGetInstanceHandle(),TEXT("DaZha"));
						break;
        default :
                 PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));
      }
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

	//��ǰ�˿�
	SwitchToCurrentCard();

	//��������
	WORD wOutViewChairID=SwitchViewChairID(pOutCard->wOutCardUser);
	m_bCardCount[pOutCard->wOutCardUser]-=pOutCard->bCardCount;
	m_GameClientView.SetCardCount(wOutViewChairID,m_bCardCount[pOutCard->wOutCardUser]);

	//��������
	if ((IsLookonMode()==true)||(GetMeChairID()!=pOutCard->wOutCardUser))
	{
		m_GameClientView.m_UserCardControl[wOutViewChairID].SetCardData(pOutCard->bCardData,pOutCard->bCardCount);
	}

	//ʤ���ж�
	if (m_bCardCount[pOutCard->wOutCardUser]==0)
	{
		WORD wWinCount=0;
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			if (m_bCardCount[i]==0) wWinCount++;
		}
		m_GameClientView.SetUserTaxis(wOutViewChairID,(BYTE)wWinCount);
	}

	//��������
	if (m_bTurnCardCount==0)
	{
		for (WORD i=0;i<GAME_PLAYER;i++) 
		{
			//��������
			m_HistoryCard.OnEventSaveData(i);

			if (i==pOutCard->wOutCardUser) continue;
			m_GameClientView.m_UserCardControl[SwitchViewChairID(i)].SetCardData(NULL,0);
		}
		m_GameClientView.SetPassFlag(INVALID_CHAIR,false);
	}

	//��¼����
	m_bTurnCardCount=pOutCard->bCardCount;
	m_bTurnOutType=m_GameLogic.GetCardType(pOutCard->bCardData,pOutCard->bCardCount,true);
	CopyMemory(m_bTurnCardData,pOutCard->bCardData,sizeof(BYTE)*pOutCard->bCardCount);

	//��ʷ�˿�
	m_HistoryCard.OnEventSaveData(pOutCard->wOutCardUser);
	m_HistoryCard.OnEventUserOutCard(pOutCard->wOutCardUser,pOutCard->bCardData,pOutCard->bCardCount);

	//�Լ��˿�
	if ((IsLookonMode()==true)&&(pOutCard->wOutCardUser==GetMeChairID()))
	{
		//ɾ���˿� 
		BYTE bSourceCount=m_bHandCardCount[0];
		m_bHandCardCount[0]-=pOutCard->bCardCount;
		m_GameLogic.RemoveCard(pOutCard->bCardData,pOutCard->bCardCount,m_bHandCardData[0],bSourceCount);

		//���ý���
		m_GameLogic.SortCardList(m_bHandCardData[0],m_bHandCardCount[0],m_cbSortType);
		m_GameClientView.m_HandCardControl[1].SetCardData(m_bHandCardData[0],m_bHandCardCount[0]);
	}

	//�Է��˿�
	if (pOutCard->wOutCardUser==((GetMeChairID()+2)%GAME_PLAYER))
	{
		//ɾ���˿� 
		BYTE bSourceCount=m_bHandCardCount[1];
		m_bHandCardCount[1]-=pOutCard->bCardCount;
		m_GameLogic.RemoveCard(pOutCard->bCardData,pOutCard->bCardCount,m_bHandCardData[1],bSourceCount);

		//���ý���
		m_GameLogic.SortCardList(m_bHandCardData[1],m_bHandCardCount[1],m_cbSortType);
		m_GameClientView.m_HandCardControl[0].SetCardData(m_bHandCardData[1],m_bHandCardCount[1]);
	}

	//����÷�
	for (BYTE i=0;i<m_bTurnCardCount;i++)
	{
		if ((m_bTurnCardData[i]&LOGIC_MASK_VALUE)==5) m_lGameScore[2]+=5;
		if ((m_bTurnCardData[i]&LOGIC_MASK_VALUE)==10) m_lGameScore[2]+=10;
		if ((m_bTurnCardData[i]&LOGIC_MASK_VALUE)==13) m_lGameScore[2]+=10;
	}
	m_GameClientView.SetUserScore(2,m_lGameScore[2]);

	//����ж�
	if (pOutCard->wCurrentUser==pOutCard->wOutCardUser)
	{
		//�ۼƻ���
		WORD wMeChairID=GetMeChairID();
		if ((pOutCard->wOutCardUser==wMeChairID)||(pOutCard->wOutCardUser==((wMeChairID+2)%GAME_PLAYER)))
		{
			m_lGameScore[0]+=m_lGameScore[2];
			m_lGameScore[2]=0L;
			m_GameClientView.SetUserScore(0,m_lGameScore[0]);
			m_GameClientView.SetUserScore(2,m_lGameScore[2]);
		}
		else
		{
			m_lGameScore[1]+=m_lGameScore[2];
			m_lGameScore[2]=0L;
			m_GameClientView.SetUserScore(1,m_lGameScore[1]);
			m_GameClientView.SetUserScore(2,m_lGameScore[2]);
		}

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
	if ((IsLookonMode()==true)||(GetMeChairID()!=pOutCard->wOutCardUser)) //PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));
		PlaySound(m_bTurnOutType);

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
		if (m_bStustee==false) ActiveGameFrame();
		m_GameClientView.m_btPassCard.EnableWindow(TRUE);
		m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btPassCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btOutPrompt.ShowWindow(SW_SHOW);
		m_GameClientView.m_btOutCard.EnableWindow((VerdictOutCard()==true)?TRUE:FALSE);
	}

	//���ö�ʱ��
	if (pOutCard->wCurrentUser!=INVALID_CHAIR)
	{
		WORD wTimeCount=30;
		SetGameTimer(pOutCard->wCurrentUser,IDI_OUT_CARD,wTimeCount);
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

	//��ǰ�˿�
	SwitchToCurrentCard();

	//�������
	if ((IsLookonMode()==true)||(pPassCard->wPassUser!=GetMeChairID()))
	{
		WORD wViewChairID=SwitchViewChairID(pPassCard->wPassUser);
		m_GameClientView.SetPassFlag(wViewChairID,true);
		m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(NULL,0);
	}

	//��ʷ�˿�
	m_HistoryCard.OnEventSaveData(pPassCard->wPassUser);
	m_HistoryCard.OnEventUserPass(pPassCard->wPassUser);

	//һ���ж�
	if (pPassCard->bNewTurn==TRUE)
	{
		//�ۼƻ���
		WORD wMeChairID=GetMeChairID();
		if ((pPassCard->wCurrentUser==wMeChairID)||(pPassCard->wCurrentUser==((wMeChairID+2)%GAME_PLAYER)))
		{
			m_lGameScore[0]+=m_lGameScore[2];
			m_lGameScore[2]=0L;
			m_GameClientView.SetUserScore(0,m_lGameScore[0]);
			m_GameClientView.SetUserScore(2,m_lGameScore[2]);
		}
		else
		{
			m_lGameScore[1]+=m_lGameScore[2];
			m_lGameScore[2]=0L;
			m_GameClientView.SetUserScore(1,m_lGameScore[1]);
			m_GameClientView.SetUserScore(2,m_lGameScore[2]);
		}

		//���ñ���
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
		if (m_bStustee==false) ActiveGameFrame();
		m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btPassCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btOutPrompt.ShowWindow(SW_SHOW);
		m_GameClientView.m_btPassCard.EnableWindow((m_bTurnCardCount>0)?TRUE:FALSE);
		m_GameClientView.m_btOutCard.EnableWindow((VerdictOutCard()==true)?TRUE:FALSE);
	}

	//��������
	if ((IsLookonMode()==true)||(pPassCard->wPassUser!=GetMeChairID()))	PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_NOTOUT"));

	//����ʱ��
	if (m_bTurnCardCount!=0)
	{
		WORD wTimeCount=30;
		SetGameTimer(pPassCard->wCurrentUser,IDI_OUT_CARD,wTimeCount);
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

	//����״̬
	SetGameStatus(GS_RF_FREE);

	//ɾ����ʱ��
	KillTimer(IDI_MOST_CARD);
	KillGameTimer(IDI_OUT_CARD);

	//���ؿؼ�
	m_GameClientView.m_btOutCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btPassCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btOutPrompt.ShowWindow(SW_HIDE);

	//���ÿؼ�
	m_GameClientView.m_btOutCard.EnableWindow(FALSE);
	m_GameClientView.m_btPassCard.EnableWindow(FALSE);
	m_GameClientView.m_btLastTurn.EnableWindow(FALSE);

	//���û���
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		const tagUserData * pUserData=GetUserData(i);
		m_GameClientView.m_ScoreView.SetGameScore(i,pUserData->szName,pGameEnd->lGameScore[i]);
	}
	WORD wMeChairID=GetMeChairID();
	tagScoreViewInfo ScoreViewInfo;
	if ((wMeChairID==0)||(wMeChairID==2))
	{
		ScoreViewInfo.lDrawScore[0]=pGameEnd->lDrawScore[0];
		ScoreViewInfo.lDrawScore[1]=pGameEnd->lDrawScore[1];
	}
	else
	{
		ScoreViewInfo.lDrawScore[0]=pGameEnd->lDrawScore[1];
		ScoreViewInfo.lDrawScore[1]=pGameEnd->lDrawScore[0];
	}
	m_GameClientView.m_ScoreView.SetScoreViewInfo(ScoreViewInfo);
	m_GameClientView.m_ScoreView.ShowWindow(SW_SHOW);

	//��ʾ�˿�
	if (IsLookonMode()==true) 
	{
		m_GameClientView.m_HandCardControl[0].SetDisplayFlag(true);
		m_GameClientView.m_HandCardControl[1].SetDisplayFlag(true);
	}

	//�����˿�
	BYTE bCardPos=0;
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		WORD wViewChairID=SwitchViewChairID(i);
		if (wViewChairID==0) m_GameClientView.m_HandCardControl[0].SetCardData(&pGameEnd->bCardData[bCardPos],pGameEnd->bCardCount[i]);
		else if (wViewChairID==1) m_GameClientView.m_OtherCardControl[0].SetCardData(&pGameEnd->bCardData[bCardPos],pGameEnd->bCardCount[i]);
		else if (wViewChairID==2) m_GameClientView.m_HandCardControl[1].SetCardData(&pGameEnd->bCardData[bCardPos],pGameEnd->bCardCount[i]);
		else if (wViewChairID==3) m_GameClientView.m_OtherCardControl[1].SetCardData(&pGameEnd->bCardData[bCardPos],pGameEnd->bCardCount[i]);

		bCardPos+=pGameEnd->bCardCount[i];
		if (pGameEnd->bCardCount[i]!=0)
		{
			m_GameClientView.SetPassFlag(wViewChairID,false);
			m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(NULL,0);
		}
	}

	//��������
	LONG lMeScore=pGameEnd->lGameScore[GetMeChairID()];
	if (lMeScore>0L) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WIN"));
	else if (lMeScore<0L) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_LOST"));
	else PlayGameSound(GetModuleHandle(NULL),TEXT("GAME_END"));

	//���ý���
	if (IsLookonMode()==false)
	{
		m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
		SetGameTimer(GetMeChairID(),IDI_START_GAME,20);//����һ����Ϸ������ ����ʱ��ʼʱ��
	}

	//�ɼ����
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��������
		TCHAR szBuffer[64];
		const tagUserData * pUserData=GetUserData(i);

		//�ɼ����
		if (pUserData!=NULL)
		{
			_snprintf(szBuffer,CountArray(szBuffer),TEXT("%s��%ld"),pUserData->szName,pGameEnd->lGameScore[i]);
			InsertGeneralString(szBuffer,RGB(255,0,255),true);//255,128,0
		}
	}

	return true;
}

//�����ж�
bool CGameClientDlg::VerdictOutCard()
{
	//״̬�ж�
	if (m_GameClientView.m_btOutCard.IsWindowVisible()==FALSE) return false;

	//��ȡ�˿�
	BYTE bCardData[MAX_COUNT];
	BYTE bShootCount=(BYTE)m_GameClientView.m_HandCardControl[1].GetShootCard(bCardData,CountArray(bCardData));

	//�����ж�
	if (bShootCount>0L)
	{
		//�����˿�
		m_GameLogic.SortCardList(bCardData,bShootCount,ST_ORDER);

		//��������
		BYTE bLastType=CT_ERROR;
		if (m_bTurnCardCount>0) bLastType=m_GameLogic.GetCardType(m_bTurnCardData,m_bTurnCardCount,true);
		BYTE bCardType=m_GameLogic.GetCardType(bCardData,bShootCount,((bLastType==CT_DOUBLE_BOMB)||(bLastType==CT_DOUBLE_LINE)));

		//�����ж�
		if (bCardType==CT_ERROR) return false;

		//�����ж�
		if (m_bTurnCardCount==0) return true;

		//�Ա��˿�
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
		m_GameClientView.m_btOutPrompt.ShowWindow(SW_HIDE);

		//��������
		CMD_C_OutCard OutCard;
		OutCard.bCardCount=1;
		OutCard.bCardData[0]=m_bHandCardData[0][m_bHandCardCount[0]-1];
		SendData(SUB_C_OUT_CARD,&OutCard,sizeof(OutCard)-sizeof(OutCard.bCardData)+OutCard.bCardCount*sizeof(BYTE));

		//Ԥ�ȴ���
		PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));
		m_GameClientView.m_UserCardControl[2].SetCardData(OutCard.bCardData,OutCard.bCardCount);

		//Ԥ��ɾ��
		BYTE bSourceCount=m_bHandCardCount[0];
		m_bHandCardCount[0]-=OutCard.bCardCount;
		m_GameLogic.RemoveCard(OutCard.bCardData,OutCard.bCardCount,m_bHandCardData[0],bSourceCount);
		m_GameClientView.m_HandCardControl[1].SetCardData(m_bHandCardData[0],m_bHandCardCount[0]);
	}
	else OnPassCard(0,0);

	return true;
}

//��ʱ����Ϣ
void CGameClientDlg::OnTimer(UINT nIDEvent)
{
	//����
	if (nIDEvent==IDI_LAST_TURN)
	{
		SwitchToCurrentCard();
		return;
	}

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
			if (m_bStustee==false)ActiveGameFrame();
			m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
			m_GameClientView.m_btPassCard.ShowWindow(SW_SHOW);
			m_GameClientView.m_btOutPrompt.ShowWindow(SW_SHOW);
			m_GameClientView.m_btPassCard.EnableWindow(FALSE);
			m_GameClientView.m_btOutCard.EnableWindow((VerdictOutCard()==true)?TRUE:FALSE);
		}

		//����ʱ��
		WORD wTimeCount=20;
		SetGameTimer(wCurrentUser,IDI_OUT_CARD,wTimeCount);

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
	m_bTurnOutType=CT_ERROR;
	m_wMostUser=INVALID_CHAIR;
	memset(m_bHandCardData,0,sizeof(m_bHandCardData));
	memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));
	memset(m_bHandCardCount,0,sizeof(m_bHandCardCount));

	//���ý���
	KillGameTimer(IDI_START_GAME);
	m_GameClientView.SetCardCount(INVALID_CHAIR,0);
	m_GameClientView.SetUserTaxis(INVALID_CHAIR,0);
	m_GameClientView.SetPassFlag(INVALID_CHAIR,false);
	m_GameClientView.SetUserScore(CountArray(m_lGameScore),0);

	//���ؿؼ�
	m_GameClientView.m_btStart.ShowWindow(FALSE);
	m_GameClientView.m_ScoreView.ShowWindow(SW_HIDE);//SW_HIDE    SW_SHOW

	//�����˿�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.m_UserCardControl[i].SetCardData(NULL,0);
	}
	for (WORD i=0;i<2;i++)
	{
		m_GameClientView.m_HandCardControl[i].SetCardData(NULL,0);
		m_GameClientView.m_OtherCardControl[i].SetCardData(NULL,0);
		m_GameClientView.m_HandCardControl[i].SetPositively(false);
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
	m_GameClientView.m_btOutPrompt.ShowWindow(SW_HIDE);
	
	//��������
	CMD_C_OutCard OutCard;
	OutCard.bCardCount=(BYTE)m_GameClientView.m_HandCardControl[1].GetShootCard(OutCard.bCardData,CountArray(OutCard.bCardData));
	m_GameLogic.SortCardList(OutCard.bCardData,OutCard.bCardCount,ST_ORDER);
	SendData(SUB_C_OUT_CARD,&OutCard,sizeof(OutCard)-sizeof(OutCard.bCardData)+OutCard.bCardCount*sizeof(BYTE));

	//Ԥ����ʾ
	//PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));
	BYTE OutCardType=m_GameLogic.GetCardType(OutCard.bCardData , OutCard.bCardCount ,true);
	PlaySound(OutCardType);
	m_GameClientView.m_UserCardControl[2].SetCardData(OutCard.bCardData,OutCard.bCardCount);

	//Ԥ��ɾ��
	BYTE bSourceCount=m_bHandCardCount[0];
	m_bHandCardCount[0]-=OutCard.bCardCount;
	m_GameLogic.RemoveCard(OutCard.bCardData,OutCard.bCardCount,m_bHandCardData[0],bSourceCount);
	m_GameLogic.SortCardList(m_bHandCardData[0],m_bHandCardCount[0],m_cbSortType);
	m_GameClientView.m_HandCardControl[1].SetCardData(m_bHandCardData[0],m_bHandCardCount[0]);

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
	m_GameClientView.m_btOutPrompt.ShowWindow(SW_HIDE);

	//��������
	SendData(SUB_C_PASS_CARD);

	//Ԥ����ʾ
	m_GameClientView.SetPassFlag(2,true);
	m_GameClientView.m_HandCardControl[1].ShootAllCard(false);
	m_GameClientView.m_UserCardControl[2].SetCardData(NULL,0);

	//��������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_NOTOUT"));

	return 0;
}

//������ʾ
LRESULT CGameClientDlg::OnOutPrompt(WPARAM wParam, LPARAM lParam)
{
	//���Ʒ���
	tagOutCardResult OutCardResult;
	ZeroMemory(&OutCardResult,sizeof(OutCardResult));

	BYTE cbCardData[27];
	CopyMemory(cbCardData,m_bHandCardData[0],sizeof(BYTE)*m_bHandCardCount[0]);
	m_GameLogic.SortCardList(cbCardData,m_bHandCardCount[0],ST_ORDER);
	bool bSuccess=m_GameLogic.SearchOutCard(cbCardData,m_bHandCardCount[0],m_bTurnCardData,m_bTurnCardCount,m_bTurnOutType,OutCardResult);

	//���ƴ���
	if ((bSuccess==true)&&(OutCardResult.cbCardCount>0))
	{
		//�����˿�
		m_GameClientView.m_HandCardControl[1].SetShootCard(OutCardResult.cbResultCard,OutCardResult.cbCardCount);

		//�ؼ�����
		OnLeftHitCard(0,0);
		if(m_bStustee)OnOutCard(0,0);
	}
	else OnPassCard(0,0);

	return 0;
}

//�Ҽ��˿�
LRESULT CGameClientDlg::OnLeftHitCard(WPARAM wParam, LPARAM lParam)
{
	//����״̬
	if (m_GameClientView.m_btOutCard.IsWindowVisible())
	{
		bool bOutCard=VerdictOutCard();
		m_GameClientView.m_btOutCard.EnableWindow((bOutCard==true)?TRUE:FALSE);
	}

	return 0;
}

//����˿�
LRESULT CGameClientDlg::OnRightHitCard(WPARAM wParam, LPARAM lParam)
{
	//�û�����
	OnOutCard(0,0);

	return 0;
}

//�����˿�
LRESULT CGameClientDlg::OnSortCard(WPARAM wParam, LPARAM lParam)
{
	//���ñ���
	m_cbSortType=(m_cbSortType==ST_VIEW)?ST_BOMB:ST_VIEW;

	//���ý���
	m_GameLogic.SortCardList(m_bHandCardData[0],m_bHandCardCount[0],m_cbSortType);
	m_GameLogic.SortCardList(m_bHandCardData[1],m_bHandCardCount[1],m_cbSortType);
	m_GameClientView.m_HandCardControl[1].SetCardData(m_bHandCardData[0],m_bHandCardCount[0]);
	m_GameClientView.m_HandCardControl[0].SetCardData(m_bHandCardData[1],m_bHandCardCount[1]);

	return 0;
}

//�Ϲܿ���
LRESULT CGameClientDlg::OnStusteeControl(WPARAM wParam, LPARAM lParam)
{
	m_wTimeOutCount=0;
	//����״̬
	if (m_bStustee==true)
	{
		m_bStustee=false;
		m_GameClientView.m_btStusteeControl.SetButtonImage(IDB_BT_START_TRUSTEE,AfxGetInstanceHandle(),false);
	}
	else
	{
		m_bStustee=true;
		m_GameClientView.m_btStusteeControl.SetButtonImage(IDB_BT_STOP_TRUSTEE,AfxGetInstanceHandle(),false);
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
			m_GameClientView.SetPassFlag(wViewChairID,cbEventFlag==AF_PASS);
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

//��������
LRESULT CGameClientDlg::OnMessageEnableHistory(WPARAM wParam, LPARAM lParam)
{
	//��������
	m_GameClientView.m_btLastTurn.EnableWindow(TRUE);

	return 0;
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
		m_GameClientView.SetPassFlag(wViewChairID,cbEventFlag==AF_PASS);
		m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(cbCardData,cbCardCount);
	}

	//���½���
	m_GameClientView.UpdateGameView(NULL);

	return true;
}


//////////////////////////////////////////////////////////////////////////

