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
	//ON_MESSAGE(IDM_LAND_SCORE,OnLandScore)
	ON_MESSAGE(IDM_AUTO_OUTCARD,OnAutoOutCard)
	ON_MESSAGE(IDM_LEFT_HIT_CARD,OnLeftHitCard)
	ON_MESSAGE(IDM_RIGHT_HIT_CARD,OnRightHitCard)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientDlg::CGameClientDlg() : CGameFrameDlg(&m_GameClientView)
{
	//��Ϸ����
//	m_wBombTime=1;
	m_bHandCardCount=0;
	m_bHintCardCount=0;
//	m_wLandUser=INVALID_CHAIR;
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
	SetWindowText(TEXT("�ذ�һ��Ϸ  --  Ver��6.0.1.0"));

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
//	m_GameClientView.m_btStart.ShowWindow( SW_SHOW);

	//test
		//m_GameClientView.m_LeaveCardControl[0].SetCardData(m_GameClientView.m_byZerodata,18);
		//m_GameClientView.m_LeaveCardControl[1].SetCardData(m_GameClientView.m_byZerodata,5);
		//BYTE data[10];
		//for(int i=0;i<10;i++)
		//	data[i]=6;
		//for(int j=0;j<3;j++)
		//m_GameClientView.m_UserCardControl[j].SetCardData(data,6);
	return true;
}

//���ÿ��
void CGameClientDlg::ResetGameFrame()
{
	//��Ϸ����
//	m_wBombTime=1;
	m_bHandCardCount=0;
	m_bHintCardCount=0;
//	m_wLandUser=INVALID_CHAIR;
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
	//case IDI_LAND_SCORE:		//�ڿӽз�
	//	{
	//		if (nElapse==0)
	//		{
	//			if ((IsLookonMode()==false)&&(wChairID==GetMeChairID())) OnLandScore(255,255);
	//			return false;
	//		}
	//		if ((nElapse<=10)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false)) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));

	//		return true;
	//	}
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
	//case SUB_S_SEND_CARD:		//�����˿�
	//	{
	//		return OnSubSendCard(pBuffer,wDataSize);
	//	}
	//case SUB_S_LAND_SCORE:	//�û��з�
	//	{
	//		return OnSubLandScore(pBuffer,wDataSize);
	//	}
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
	case SUB_S_EIGHT_ONE:
		{
			return OnSubEightOne(pBuffer,wDataSize);
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
//			m_GameClientView.SetBombTime(pStatusPlay->wBombTime);
			m_GameClientView.SetBaseScore(pStatusPlay->lBaseScore);
//			m_GameClientView.m_BackCardControl.SetCardData(pStatusPlay->bBackCard,3);
			m_GameClientView.m_HandCardControl.SetCardData(m_bHandCardData,m_bHandCardCount);
//			m_GameClientView.SetLandUser(SwitchViewChairID(pStatusPlay->wLandUser),pStatusPlay->bLandScore);

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

//��Ϸ��ʼ
bool CGameClientDlg::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;

	//��Ϣ����
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

	//��������
	m_bHandCardCount=16;
	m_bTurnCardCount=0;
	m_bTurnOutType=CT_INVALID;
	ZeroMemory(m_bTurnCardData,sizeof(m_bTurnCardData));

	for(int i=0;i<GAME_PLAYER;i++)
	{
		m_bCardCount[i]=16;
	}
	CopyMemory(m_bHandCardData,pGameStart->bCardData,sizeof(pGameStart->bCardData));
	
	//���ý���
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.SetPassFlag(i,false);
		m_GameClientView.SetCardCount(i,m_bCardCount[i]);
		if(i!=2)
			m_GameClientView.m_UserCardControl[i].SetCardData(NULL,0);
	}
	m_GameClientView.m_HandCardControl.SetCardData(pGameStart->bCardData,16);
	if (IsLookonMode()==true)
	{
		//		m_GameClientView.SetLandUser(INVALID_CHAIR,0);
		m_GameClientView.m_ScoreView.ShowWindow(SW_HIDE);
		m_GameClientView.m_LeaveCardControl[0].SetCardData(NULL,0);
		m_GameClientView.m_LeaveCardControl[1].SetCardData(NULL,0);
	}

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
	m_GameClientView.m_LeaveCardControl[0].SetBackCard(m_bCardCount[0]);
	m_GameClientView.m_LeaveCardControl[1].SetBackCard(m_bCardCount[1]);

	//��������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));


	//����ʱ��
	SetGameTimer(pGameStart->wCurrentUser,IDI_OUT_CARD,30);

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
	if( GetMeChairID()!=pOutCard->wOutCardUser)
	{
		if(wOutViewChairID==0)
			m_GameClientView.m_LeaveCardControl[0].SetBackCard(m_bCardCount[pOutCard->wOutCardUser]);
		if(wOutViewChairID==2)
			m_GameClientView.m_LeaveCardControl[1].SetBackCard(m_bCardCount[pOutCard->wOutCardUser]);
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

//	//ը���ж�
//	if ((m_bTurnOutType==CT_BOMB_CARD)/*||(m_bTurnOutType==CT_MISSILE_CARD)*/)
//	{
////		m_wBombTime*=2;
////		m_GameClientView.SetBombTime(m_wBombTime);
//	}

	//�Լ��˿�
	if ((IsLookonMode()==true)&&(pOutCard->wOutCardUser==GetMeChairID()))
	{
		//ɾ���˿� 
		BYTE bSourceCount=m_bHandCardCount;
		m_bHandCardCount-=pOutCard->bCardCount;
		m_GameLogic.RemoveCard(pOutCard->bCardData,pOutCard->bCardCount,m_bHandCardData,bSourceCount);

		//���ý���
		m_GameClientView.m_HandCardControl.SetCardData(m_bHandCardData,m_bHandCardCount);
	}

	////����ж�
	//if (pOutCard->wCurrentUser==pOutCard->wOutCardUser)
	//{
	//	//���ñ���
	//	m_bTurnCardCount=0;
	//	m_bTurnOutType=CT_INVALID;
	//	m_wMostUser=pOutCard->wCurrentUser;
	//	memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));

	//	//���ý���
	//	for (WORD i=0;i<GAME_PLAYER;i++)
	//	{
	//		if (i!=pOutCard->wOutCardUser)
	//		{
	//			WORD wViewChairID=SwitchViewChairID(i);
	//			m_GameClientView.SetPassFlag(wViewChairID,true);
	//			m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(NULL,0);
	//		}
	//	}

	//	//��������
	//	PlayGameSound(AfxGetInstanceHandle(),TEXT("MOST_CARD"));

	//	//���ö�ʱ��
	//	SetTimer(IDI_MOST_CARD,3000,NULL);

	//	return true;
	//}

	//��������
	if ((IsLookonMode()==true)||(GetMeChairID()!=pOutCard->wOutCardUser)) PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));

	//�������
	if ((IsLookonMode()==false)&&(pOutCard->wCurrentUser==GetMeChairID()))
	{
		m_GameClientView.m_UserCardControl[1].SetCardData(NULL,0);
		//���������Զ�Pass
		if((pOutCard->bCardCount>1)&&(m_bHandCardCount<=pOutCard->bCardCount))
		{
			m_GameClientView.m_btPassCard.EnableWindow(TRUE);
			OnPassCard(0,0);
			return true;
		}
		////�������
		//if(	(m_bTurnOutType==CT_SINGLE) && (m_GameLogic.GetCardLogicValue(m_bHandCardData[0])<=m_GameLogic.GetCardLogicValue(pOutCard->bCardData[0])) )
		//{
		//	m_GameClientView.m_btPassCard.EnableWindow(TRUE);
		//	Sleep(2000);
		//	OnPassCard(0,0);
		//	return true;
		//}
		ActiveGameFrame();	
		m_GameClientView.m_btPassCard.EnableWindow(TRUE);
		m_GameClientView.m_btPassCard.ShowWindow(SW_SHOW);
		//�¼��ذ�һ�ҳ����ǵ���
		if( (m_GameClientView.m_bEightOne[(GetMeChairID()+1)%3]) && (m_bTurnOutType==CT_SINGLE) )
		{
			//�д���ʱ���ɷ���
			if(m_GameLogic.GetCardLogicValue(m_bHandCardData[0])>m_GameLogic.GetCardLogicValue(pOutCard->bCardData[0]))
			{
				m_GameClientView.m_btPassCard.ShowWindow(SW_HIDE);
			}
		}	
		m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);

		m_GameClientView.m_btAutoOutCard.EnableWindow(TRUE);
		m_GameClientView.m_btAutoOutCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btOutCard.EnableWindow((VerdictOutCard()==true)?TRUE:FALSE);
	}

	//����ʱ��
	if (pOutCard->wCurrentUser!=INVALID_CHAIR)
	{
		BYTE bCardCount=m_bCardCount[pOutCard->wCurrentUser];
		SetGameTimer(pOutCard->wCurrentUser,IDI_OUT_CARD,/*(bCardCount<m_bTurnCardCount)?3:*/30);
	}

	return true;
}

//�а�һ
bool CGameClientDlg::OnSubEightOne( const void *pBuffer, WORD wDataSize )
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_EightOne)) return false;
	CMD_S_EightOne *EightOne=(CMD_S_EightOne *)pBuffer;
	m_GameClientView.m_bEightOne[EightOne->wEightOneUser]=true;

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
		////�������
		//if(	(m_bTurnOutType==CT_SINGLE) && (m_GameLogic.GetCardLogicValue(m_bHandCardData[0])<=m_GameLogic.GetCardLogicValue(m_bTurnCardData[0])) )
		//{
		//	m_GameClientView.m_btPassCard.EnableWindow(TRUE);
		//	Sleep(2000);
		//	OnPassCard(0,0);
		//	return true;
		//}
		ActiveGameFrame();
		m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btPassCard.ShowWindow(SW_SHOW);
		//�¼��ذ�һ���ǵ���
		if( (m_GameClientView.m_bEightOne[(GetMeChairID()+1)%3]) && (m_bTurnOutType==CT_SINGLE) )
		{
			//�д���ʱ���ɷ���
			if(m_GameLogic.GetCardLogicValue(m_bHandCardData[0])>m_GameLogic.GetCardLogicValue(m_bTurnCardData[0]))
			{
				m_GameClientView.m_btPassCard.ShowWindow(SW_HIDE);
			}
		}
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
	KillGameTimer(IDI_LAND_SCORE);

	//���ؿؼ�
	m_GameClientView.m_btOutCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btPassCard.ShowWindow(SW_HIDE);
	//m_GameClientView.m_btOneScore.ShowWindow(SW_HIDE);
	//m_GameClientView.m_btTwoScore.ShowWindow(SW_HIDE);
	//m_GameClientView.m_btThreeScore.ShowWindow(SW_HIDE);
//	m_GameClientView.m_btGiveUpScore.ShowWindow(SW_HIDE);
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
	m_GameClientView.m_LeaveCardControl[0].SetDisplayFlag(true);
	m_GameClientView.m_LeaveCardControl[1].SetDisplayFlag(true);

	m_GameClientView.m_ScoreView.SetGameTax(pGameEnd->lGameTax);
	m_GameClientView.m_ScoreView.ShowWindow(SW_SHOW);

	//test
	//WORD wViewChairID=0;
	//�����˿�
	BYTE bCardPos=0;
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		WORD wViewChairID=SwitchViewChairID(i);
		if (wViewChairID==0)
		{
			m_GameClientView.m_LeaveCardControl[0].SetCardData(&pGameEnd->bCardData[bCardPos],pGameEnd->bCardCount[i]);
		}
		else if (wViewChairID==2) 
		{
			m_GameClientView.m_LeaveCardControl[1].SetCardData(&pGameEnd->bCardData[bCardPos],pGameEnd->bCardCount[i]);
		}
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
		//SetGameTimer(GetMeChairID(),IDI_START_GAME,30);
	}
//	m_GameClientView.ShowLandTitle(false);

	return true;
}

//�����ж�
bool CGameClientDlg::VerdictOutCard()
{
	//״̬�ж�
	if (m_GameClientView.m_btOutCard.IsWindowVisible()==FALSE) return false;

	//��ȡ�˿�
	BYTE bCardData[16];
	BYTE bShootCount=(BYTE)m_GameClientView.m_HandCardControl.GetShootCard(bCardData,CountArray(bCardData));

	//���ܳ�����
	if(bShootCount==3)
		return false;
	//�����ж�
	if (bShootCount>0L)
	{
		//��������
		BYTE bCardType=m_GameLogic.GetCardType(bCardData,bShootCount);

		//�����ж�
		if (bCardType==CT_INVALID) return false;

		//�ȳ���
		if (m_bTurnCardCount==0) 
		{
			//�¼��ذ�һ�ҳ����ǵ���,����
			if( m_GameClientView.m_bEightOne[(GetMeChairID()+1)%3] && bShootCount==1 )
			{
				if( m_GameLogic.GetCardLogicValue(bCardData[0])<m_GameLogic.GetCardLogicValue(m_bHandCardData[0]) )
				return false;
			}
			//�ذ�һ
			if(m_bHandCardCount>1)
			{
				if(bShootCount==m_bHandCardCount)
					return false;
				else if(m_bHandCardCount-bShootCount==2 || m_bHandCardCount-bShootCount==1)
				{
					//�����2,���һ�Ų���Ϊ2
					if(m_GameLogic.IsCardTwoOwner(m_bHandCardData,m_bHandCardCount))
					{
						if(!m_GameLogic.IsCardTwoOwner(bCardData,bShootCount))
						{
							return false;
						}
					}
				}
			}
			return true;
		} 
		//����
		else
		{
			//�¼��ذ�һ,�ҳ����ǵ���
			if( (m_GameClientView.m_bEightOne[(GetMeChairID()+1)%3]) && (bShootCount==1) )
			{
				if(m_GameLogic.GetCardLogicValue(bCardData[0])!=m_GameLogic.GetCardLogicValue(m_bHandCardData[0]))
					return false;
			}
			return m_GameLogic.CompareCard(bCardData,m_bTurnCardData,bShootCount,m_bTurnCardCount);
		}
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
		//�¼��ذ�һ,�ҳ��Ƴ�����
		if( (m_GameClientView.m_bEightOne[(GetMeChairID()+1)%3]) )
		{
			OutCard.bCardData[0]=m_bHandCardData[0];
		}
		else
		{
			OutCard.bCardData[0]=m_bHandCardData[m_bHandCardCount-1];
		}
		
		SendData(SUB_C_OUT_CART,&OutCard,sizeof(OutCard)-sizeof(OutCard.bCardData)+OutCard.bCardCount*sizeof(BYTE));

		//Ԥ�ȴ���
		PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));
		m_GameClientView.m_UserCardControl[1].SetCardData(OutCard.bCardData,OutCard.bCardCount);

		//Ԥ��ɾ��
		BYTE bSourceCount=m_bHandCardCount;
		m_bHandCardCount-=OutCard.bCardCount;
		m_GameLogic.RemoveCard(OutCard.bCardData,OutCard.bCardCount,m_bHandCardData,bSourceCount);
		m_GameClientView.m_HandCardControl.SetCardData(m_bHandCardData,m_bHandCardCount);
	}
	else
	{
		//�¼��ذ�һ,���������б������ƴ�
		if( (m_GameClientView.m_bEightOne[(GetMeChairID()+1)%3]) && (m_bTurnOutType==CT_SINGLE) )
		{
			if( (m_GameLogic.GetCardLogicValue(m_bHandCardData[0]))>(m_GameLogic.GetCardLogicValue(m_bTurnCardData[0])) )
			{
				//��������
				CMD_C_OutCard OutCard;
				OutCard.bCardCount=1;
				OutCard.bCardData[0]=m_bHandCardData[0];
				SendData(SUB_C_OUT_CART,&OutCard,sizeof(OutCard)-sizeof(OutCard.bCardData)+OutCard.bCardCount*sizeof(BYTE));
				//Ԥ�ȴ���
				PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));
				m_GameClientView.m_UserCardControl[1].SetCardData(OutCard.bCardData,OutCard.bCardCount);

				//Ԥ��ɾ��
				BYTE bSourceCount=m_bHandCardCount;
				m_bHandCardCount-=OutCard.bCardCount;
				m_GameLogic.RemoveCard(OutCard.bCardData,OutCard.bCardCount,m_bHandCardData,bSourceCount);
				m_GameClientView.m_HandCardControl.SetCardData(m_bHandCardData,m_bHandCardCount);
			}
		}
		else
			OnPassCard(0,0);
	}
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

//Doctor 	__super::OnTimer(nIDEvent);
}

//��ʼ��ť
LRESULT CGameClientDlg::OnStart(WPARAM wParam, LPARAM lParam)
{
	//���ñ���
//	m_wBombTime=1;
	m_wTimeOutCount=0;
	m_bHandCardCount=0;
	m_bTurnCardCount=0;
	m_bTurnOutType=CT_INVALID;
	m_wMostUser=INVALID_CHAIR;
	memset(m_bHandCardData,0,sizeof(m_bHandCardData));
	memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));

	//���ý���
	KillGameTimer(IDI_START_GAME);
	m_GameClientView.SetBaseScore(0L);
	memset(m_GameClientView.m_bEightOne,0,sizeof(m_GameClientView.m_bEightOne));
//	m_GameClientView.ShowLandTitle(false);
//	m_GameClientView.SetBombTime(m_wBombTime);
	m_GameClientView.SetCardCount(INVALID_CHAIR,0);
	
	//m_GameClientView.SetLandUser(INVALID_CHAIR,0);
	//m_GameClientView.SetLandScore(INVALID_CHAIR,0);
	m_GameClientView.SetPassFlag(INVALID_CHAIR,false);

	//���ؿؼ�
	m_GameClientView.m_btStart.ShowWindow(FALSE);
	m_GameClientView.m_ScoreView.ShowWindow(SW_HIDE);

	//�����˿�
//	m_GameClientView.m_BackCardControl.SetCardData(NULL,0);
	m_GameClientView.m_HandCardControl.SetCardData(NULL,0);
	m_GameClientView.m_HandCardControl.SetPositively(false);
	m_GameClientView.m_LeaveCardControl[0].SetCardData(NULL,0);
	m_GameClientView.m_LeaveCardControl[1].SetCardData(NULL,0);
	for (WORD i=0;i<GAME_PLAYER;i++) m_GameClientView.m_UserCardControl[i].SetCardData(NULL,0);

	//������Ϣ
	SendUserReady(NULL,0);
	PlayGameSound(AfxGetInstanceHandle(),TEXT("HIT_BUTTON"));
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
	m_GameClientView.m_UserCardControl[1].SetCardData(OutCard.bCardData,OutCard.bCardCount);

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
	m_GameClientView.UpdateGameView(NULL);

	return 0;
}

////�з���Ϣ
//LRESULT CGameClientDlg::OnLandScore(WPARAM wParam, LPARAM lParam)
//{
//	//���ý���
//	KillGameTimer(IDI_LAND_SCORE);
//	m_GameClientView.m_btOneScore.ShowWindow(SW_HIDE);
//	m_GameClientView.m_btTwoScore.ShowWindow(SW_HIDE);
//	m_GameClientView.m_btThreeScore.ShowWindow(SW_HIDE);
//	m_GameClientView.m_btGiveUpScore.ShowWindow(SW_HIDE);
//
//	//��������
//	CMD_C_LandScore LandScore;
//	LandScore.bLandScore=(BYTE)wParam;
//	SendData(SUB_C_LAND_SCORE,&LandScore,sizeof(LandScore));
//
//	return 0;
//}

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

//����˿�
LRESULT CGameClientDlg::OnRightHitCard(WPARAM wParam, LPARAM lParam)
{
	//�û�����
	OnOutCard(0,0);

	return 0;
}

void CGameClientDlg::AutoOutCard(BYTE m_WhichOnsKindCard)
{
	//�¼��ذ�һ���ǵ���
	if( (m_GameClientView.m_bEightOne[(GetMeChairID()+1)%3]) && (m_bTurnOutType==CT_SINGLE) )
	{
		BYTE bOutCard=0;
		//�д���ʱ��ʾ������
		if(m_GameLogic.GetCardLogicValue(m_bHandCardData[0])>m_GameLogic.GetCardLogicValue(m_bTurnCardData[0]))
		{	
			m_GameClientView.m_HandCardControl.SetShootCard(&bOutCard,1);
			m_GameClientView.m_btOutCard.EnableWindow(TRUE);
			return;
		}
		else
		{
			OnPassCard(0,0);
			return;
		}

	}
	if(m_bHintCardData[0]==0)
	{
		m_bHintCardCount=m_bTurnCardCount;
		CopyMemory(m_bHintCardData,m_bTurnCardData,m_bTurnCardCount);
	}
	int i=0;
	BYTE m_bWhichKindSel=0;
	BYTE							m_bTempSCardCount=0;				//�˿���Ŀ
	BYTE							m_bTempSCardData[16];				//�����˿�
	BYTE							m_bTempDCardCount=0;				//�˿���Ŀ
	BYTE							m_bTempDCardData[16];				//�����˿�
	BYTE							m_bTempTCardCount=0;				//�˿���Ŀ
	BYTE							m_bTempTCardData[16];				//�����˿�
	BYTE							m_bTempFCardCount=0;				//�˿���Ŀ
	BYTE							m_bTempFCardData[16];				//�����˿�
	BYTE							m_bTempGetCardCount=0;				//�˿���Ŀ
	BYTE							m_bTempGetCardData[16];				//�����˿�
	BYTE							m_bTempGetCardIndex[16];			//�����˿�
	BYTE m_TempCard=0;
	//���û���˳��ƣ�����ʾ
	if(m_bTurnCardCount==0)
		return;
	m_GameClientView.m_HandCardControl.ShootAllCard(false);
	for(i=0;i<m_bHandCardCount;i++)
	{	
		BYTE m_GetCard=m_GameLogic.GetCardLogicValue(m_bHandCardData[i]);
		if(m_TempCard!=m_GetCard)
		{
			m_bTempSCardData[m_bTempSCardCount++]=m_bHandCardData[i];
			m_TempCard=m_GetCard;
		}
	}
	//��ȡ�����б�
	m_TempCard=0;
	for(i=0;i<m_bHandCardCount-1;i++)
	{	
		BYTE m_GetCard1=m_GameLogic.GetCardLogicValue(m_bHandCardData[i]);
		BYTE m_GetCard2=m_GameLogic.GetCardLogicValue(m_bHandCardData[i+1]);
		if(m_TempCard!=m_GetCard1&&m_GetCard1==m_GetCard2&&m_GetCard1<16)
		{
			m_bTempDCardData[m_bTempDCardCount++]=m_bHandCardData[i];
			m_bTempDCardData[m_bTempDCardCount++]=m_bHandCardData[i+1];
			m_TempCard=m_GetCard1;
		}
	}
	//��ȡ�������б�
	m_TempCard=0;
	for(i=0;i<m_bHandCardCount-2;i++)
	{	
		BYTE m_GetCard1=m_GameLogic.GetCardLogicValue(m_bHandCardData[i]);
		BYTE m_GetCard2=m_GameLogic.GetCardLogicValue(m_bHandCardData[i+1]);
		BYTE m_GetCard3=m_GameLogic.GetCardLogicValue(m_bHandCardData[i+2]);
		if(m_TempCard!=m_GetCard1&&m_GetCard1==m_GetCard2&&m_GetCard1==m_GetCard3)
		{
			m_bTempTCardData[m_bTempTCardCount++]=m_bHandCardData[i];
			m_bTempTCardData[m_bTempTCardCount++]=m_bHandCardData[i+1];
			m_bTempTCardData[m_bTempTCardCount++]=m_bHandCardData[i+2];
			m_TempCard=m_GetCard1;
		}
	}
	//��ȡ�������б�
	m_TempCard=0;
	for(i=0;i<m_bHandCardCount-3;i++)
	{	
		BYTE m_GetCard1=m_GameLogic.GetCardLogicValue(m_bHandCardData[i]);
		BYTE m_GetCard2=m_GameLogic.GetCardLogicValue(m_bHandCardData[i+1]);
		BYTE m_GetCard3=m_GameLogic.GetCardLogicValue(m_bHandCardData[i+2]);
		BYTE m_GetCard4=m_GameLogic.GetCardLogicValue(m_bHandCardData[i+3]);
		if(m_TempCard!=m_GetCard1&&m_GetCard1==m_GetCard2&&m_GetCard1==m_GetCard3&&m_GetCard1==m_GetCard4)
		{
			m_bTempFCardData[m_bTempFCardCount++]=m_bHandCardData[i];
			m_bTempFCardData[m_bTempFCardCount++]=m_bHandCardData[i+1];
			m_bTempFCardData[m_bTempFCardCount++]=m_bHandCardData[i+2];
			m_bTempFCardData[m_bTempFCardCount++]=m_bHandCardData[i+3];
			m_TempCard=m_GetCard1;
		}
	}
	//���������������ж�
	i=0;
	BYTE bTurnOutType=m_GameLogic.GetCardType(m_bHintCardData,m_bHintCardCount);
	switch(bTurnOutType)
	{
	case CT_SINGLE:
	case CT_ONE_LINE:
		if(m_WhichOnsKindCard==1)   //�ж��ǲ��Ǿ���Ψһ��
		{
			for(i=m_bTempSCardCount;i>0;i--)
			{
				if(i-m_bHintCardCount>=0&&m_GameLogic.CompareCard(&m_bTempSCardData[i-m_bHintCardCount],m_bHintCardData,m_bHintCardCount,m_bHintCardCount))
				{
					if((m_bWhichKindSel++)>1)
						i=0;
				}
			}
		}
		for(i=m_bTempSCardCount;i>0;i--)
		{
			if(i-m_bHintCardCount>=0&&m_GameLogic.CompareCard(&m_bTempSCardData[i-m_bHintCardCount],m_bHintCardData,m_bHintCardCount,m_bHintCardCount))
			{
				//�ж��ǲ���������
				bool m_bIsHaveCard=false;
				for(int j=0;j<m_bTempDCardCount;j++)
				{
					for(int n=0;n<m_bHintCardCount;n++)
					{
						if(m_GameLogic.GetCardLogicValue(m_bTempSCardData[i-m_bHintCardCount+n])==m_GameLogic.GetCardLogicValue(m_bTempDCardData[j]))
							m_bIsHaveCard=true;
					}
				}
				//�������������������
				if(m_bTempGetCardCount==0||!m_bIsHaveCard)
				{
					CopyMemory(m_bTempGetCardData,&m_bTempSCardData[i-m_bHintCardCount],m_bHintCardCount);
					m_bTempGetCardCount=m_bHintCardCount;
				}
				if(!m_bIsHaveCard)
					break;
			}
		}
		break;
	case CT_DOUBLE:
	case CT_DOUBLE_LINE:
		if(m_WhichOnsKindCard==1)     //�ж��ǲ��Ǿ���Ψһ��
		{
			for(i=m_bTempDCardCount;i>0;i--)
			{
				if(i-m_bHintCardCount>=0&&m_GameLogic.CompareCard(&m_bTempDCardData[i-m_bHintCardCount],m_bHintCardData,m_bHintCardCount,m_bHintCardCount))
				{
					if((m_bWhichKindSel++)>1)
						i=0;

				}
			}
		}
		for(i=m_bTempDCardCount;i>0;i--)
		{
			if(i-m_bHintCardCount>=0&&m_GameLogic.CompareCard(&m_bTempDCardData[i-m_bHintCardCount],m_bHintCardData,m_bHintCardCount,m_bHintCardCount))
			{
				//�ж��ǲ���������
				bool m_bIsHaveCard=false;
				for(int j=0;j<m_bTempTCardCount;j++)
				{
					for(int n=0;n<m_bHintCardCount;n++)
					{
						if(m_GameLogic.GetCardLogicValue(m_bTempDCardData[i-m_bHintCardCount+n])==m_GameLogic.GetCardLogicValue(m_bTempTCardData[j]))
							m_bIsHaveCard=true;
					}
				}
				if(m_bTempGetCardCount==0||!m_bIsHaveCard)
				{
					CopyMemory(m_bTempGetCardData,&m_bTempDCardData[i-m_bHintCardCount],m_bHintCardCount);
					m_bTempGetCardCount=m_bHintCardCount;
				}
				if(!m_bIsHaveCard)
					break;
			}
		}
		break;
//	case CT_THREE:
	case CT_THREE_LINE:
		if(m_WhichOnsKindCard==1)           //�ж��ǲ��Ǿ���Ψһ��
		{
			for(i=m_bTempTCardCount;i>0;i--)
			{
				if(i-m_bHintCardCount>=0&&m_GameLogic.CompareCard(&m_bTempTCardData[i-m_bHintCardCount],m_bHintCardData,m_bHintCardCount,m_bHintCardCount))
				{
					if((m_bWhichKindSel++)>1)
						i=0;
				}
			}
		}
		for(i=m_bTempTCardCount;i>0;i--)
		{
			if(i-m_bHintCardCount>=0&&m_GameLogic.CompareCard(&m_bTempTCardData[i-m_bHintCardCount],m_bHintCardData,m_bHintCardCount,m_bHintCardCount))
			{
				//�ж��ǲ���������
				bool m_bIsHaveCard=false;
				for(int j=0;j<m_bTempFCardCount;j++)
				{
					for(int n=0;n<m_bHintCardCount;n++)
					{
						if(m_GameLogic.GetCardLogicValue(m_bTempTCardData[i-m_bHintCardCount+n])==m_GameLogic.GetCardLogicValue(m_bTempFCardData[j]))
							m_bIsHaveCard=true;
					}
				}
				if(m_bTempGetCardCount==0||!m_bIsHaveCard)
				{
					CopyMemory(m_bTempGetCardData,&m_bTempTCardData[i-m_bHintCardCount],m_bHintCardCount);
					m_bTempGetCardCount=m_bHintCardCount;
				}
				if(!m_bIsHaveCard&&m_bTempGetCardCount!=0)
					break;
			}
		}
		break;
	//	case CT_THREE_LINE_TAKE_DOUBLE:
	case CT_THREE_LINE_TAKE_ONE:
		{
			//�����˿�
			tagAnalyseResult AnalyseResult;
			m_GameLogic.AnalysebCardData(m_bHintCardData,m_bHintCardCount,AnalyseResult);      
			if(m_WhichOnsKindCard==1)               //�ж��ǲ��Ǿ���Ψһ��
			{
				for(i=m_bTempTCardCount;i>0;i--)
				{
					if(i-AnalyseResult.bThreeCount*3>=0&&m_GameLogic.CompareCard(&m_bTempTCardData[i-AnalyseResult.bThreeCount*3],m_bHintCardData,AnalyseResult.bThreeCount*3,AnalyseResult.bThreeCount*3))
					{
						if((m_bWhichKindSel++)>1)
							i=0;
					}
				}
			}
			for(i=m_bTempTCardCount;i>0;i--)
			{
				if(i-AnalyseResult.bThreeCount*3>=0&&m_GameLogic.CompareCard(&m_bTempTCardData[i-AnalyseResult.bThreeCount*3],AnalyseResult.m_bTCardData,AnalyseResult.bThreeCount*3,AnalyseResult.bThreeCount*3))
				{
					//�ж��ǲ���������
					bool m_bIsHaveCard=false;
					for(int j=0;j<m_bTempFCardCount;j++)
					{
						for(int n=0;n<AnalyseResult.bThreeCount*3;n++)
						{
							if(m_GameLogic.GetCardLogicValue(m_bTempTCardData[i-AnalyseResult.bThreeCount*3+n])==m_GameLogic.GetCardLogicValue(m_bTempFCardData[j]))
								m_bIsHaveCard=true;
						}
					}
					if(m_bTempGetCardCount==0||!m_bIsHaveCard)
					{
						CopyMemory(m_bTempGetCardData,&m_bTempTCardData[i-AnalyseResult.bThreeCount*3],AnalyseResult.bThreeCount*3);
						m_bTempGetCardCount=AnalyseResult.bThreeCount*3;
					}
					if(!m_bIsHaveCard&&m_bTempGetCardCount!=0)
						i=0;
				}
			}
			if(m_bTempGetCardCount>0)
			{
				bool m_bIsHaveSame;
				for(int m=0;m<AnalyseResult.bDoubleCount;m++)
				{
					for(int j=0;j<m_bTempDCardCount/2;j++)
					{
						//�ж��ǲ���������
						m_bIsHaveSame=false;
						for(int n=0;n<m_bTempGetCardCount;n++)
						{
							if(m_GameLogic.GetCardLogicValue(m_bTempDCardData[m_bTempDCardCount-j*2-1])==m_GameLogic.GetCardLogicValue(m_bTempGetCardData[n]))
							{
								m_bIsHaveSame=true;
							}
						}
						if(!m_bIsHaveSame)
						{
							bool m_bIsHaveCard=false;
							for(int s=0;s<m_bTempTCardCount;s++)
							{
								for(int n=0;n<m_bTempGetCardCount;n++)
								{
									if(m_GameLogic.GetCardLogicValue(m_bTempDCardData[m_bTempDCardCount-j*2-1])==m_GameLogic.GetCardLogicValue(m_bTempTCardData[s]))
										m_bIsHaveCard=true;
								}
							}
							if(m_bTempGetCardCount==AnalyseResult.bThreeCount*3||!m_bIsHaveCard)
							{
								m_bTempGetCardData[AnalyseResult.bThreeCount*3+m*2]=m_bTempDCardData[m_bTempDCardCount-j*2-1];
								m_bTempGetCardData[AnalyseResult.bThreeCount*3+m*2+1]=m_bTempDCardData[m_bTempDCardCount-j*2-2];
								m_bTempGetCardCount=AnalyseResult.bThreeCount*3+(m+1)*2;
							}
							if(!m_bIsHaveCard)
							{
								n=m_bTempGetCardCount;
								j=m_bTempDCardCount/2;
							}
						}
					}
				}
				for(int m=0;m<AnalyseResult.bSignedCount;m++)
				{
					for(int j=0;j<m_bTempSCardCount;j++)
					{
						//�ж��ǲ���������
						m_bIsHaveSame=false;
						for(int n=0;n<m_bTempGetCardCount;n++)
						{
							if(m_GameLogic.GetCardLogicValue(m_bTempSCardData[m_bTempSCardCount-j-1])==m_GameLogic.GetCardLogicValue(m_bTempGetCardData[n]))
							{
								m_bIsHaveSame=true;
							}
						}
						if(!m_bIsHaveSame)
						{
							bool m_bIsHaveCard=false;
							for(int s=0;s<m_bTempDCardCount;s++)
							{
								for(n=0;n<m_bTempGetCardCount;n++)
								{
									if(m_GameLogic.GetCardLogicValue(m_bTempSCardData[m_bTempSCardCount-j-1])==m_GameLogic.GetCardLogicValue(m_bTempDCardData[s]))
										m_bIsHaveCard=true;
								}
							}
							if(m_bTempGetCardCount==AnalyseResult.bThreeCount*3||!m_bIsHaveCard)
							{
								m_bTempGetCardData[AnalyseResult.bThreeCount*3+m]=m_bTempSCardData[m_bTempSCardCount-j-1];
								m_bTempGetCardCount=AnalyseResult.bThreeCount*3+m+1;
							}
							if(!m_bIsHaveCard)
							{
								n=m_bTempGetCardCount;
								j=m_bTempSCardCount;
							}
						}
					}
				}
			}
		}
		break;
//	case CT_FOUR_LINE_TAKE_ONE:
	case CT_FOUR_LINE_TAKE_DOUBLE:
		{
			//�����˿�
			tagAnalyseResult AnalyseResult;
			m_GameLogic.AnalysebCardData(m_bHintCardData,m_bHintCardCount,AnalyseResult);
			if(m_WhichOnsKindCard==1)       //�ж��ǲ��Ǿ���Ψһ��
			{
				for(i=m_bTempFCardCount;i>0;i--)
				{
					if(i-AnalyseResult.bFourCount*4>=0&&m_GameLogic.CompareCard(&m_bTempFCardData[i-AnalyseResult.bFourCount*4],m_bHintCardData,AnalyseResult.bFourCount*4,AnalyseResult.bFourCount*4))
					{
						if((m_bWhichKindSel++)>1)
							i=0;
					}
				}
			}
			for(i=m_bTempFCardCount;i>0;i--)
			{
				if(i-AnalyseResult.bFourCount*4>=0&&m_GameLogic.CompareCard(&m_bTempFCardData[i-AnalyseResult.bFourCount*4],m_bHintCardData,AnalyseResult.bFourCount*4,AnalyseResult.bFourCount*4))
				{
					CopyMemory(m_bTempGetCardData,&m_bTempFCardData[i-AnalyseResult.bFourCount*4],AnalyseResult.bFourCount*4);
					m_bTempGetCardCount=AnalyseResult.bFourCount*4;
					i=0;
				}
			}
			if(m_bTempGetCardCount>0)
			{
				bool m_bIsHaveSame;
				for(int m=0;m<AnalyseResult.bDoubleCount;m++)
				{
					for(int j=0;j<m_bTempDCardCount/2;j++)
					{
						//�ж��ǲ���������
						m_bIsHaveSame=false;
						for(int n=0;n<m_bTempGetCardCount;n++)
						{
							if(m_GameLogic.GetCardLogicValue(m_bTempDCardData[m_bTempDCardCount-j*2-1])==m_GameLogic.GetCardLogicValue(m_bTempGetCardData[n]))
							{
								m_bIsHaveSame=true;
							}
						}
						if(!m_bIsHaveSame)
						{
							bool m_bIsHaveCard=false;
							for(int s=0;s<m_bTempTCardCount;s++)
							{
								for(int n=0;n<m_bTempGetCardCount;n++)
								{
									if(m_GameLogic.GetCardLogicValue(m_bTempDCardData[m_bTempDCardCount-j*2-1])==m_GameLogic.GetCardLogicValue(m_bTempTCardData[s]))
										m_bIsHaveCard=true;
								}
							}
							if(m_bTempGetCardCount==AnalyseResult.bFourCount*4||!m_bIsHaveCard)
							{
								m_bTempGetCardData[AnalyseResult.bFourCount*4+m*2]=m_bTempDCardData[m_bTempDCardCount-j*2-1];
								m_bTempGetCardData[AnalyseResult.bFourCount*4+m*2+1]=m_bTempDCardData[m_bTempDCardCount-j*2-2];
								m_bTempGetCardCount=AnalyseResult.bFourCount*4+(m+1)*2;
							}
							if(!m_bIsHaveCard)
							{
								n=m_bTempGetCardCount;
								j=m_bTempDCardCount/2;
							}
						}
					}
				}
				for(int m=0;m<AnalyseResult.bSignedCount;m++)
				{
					for(int j=0;j<m_bTempSCardCount;j++)
					{
						//�ж��ǲ���������
						m_bIsHaveSame=false;
						for(int n=0;n<m_bTempGetCardCount;n++)
						{
							if(m_GameLogic.GetCardLogicValue(m_bTempSCardData[m_bTempSCardCount-j-1])==m_GameLogic.GetCardLogicValue(m_bTempGetCardData[n]))
							{
								m_bIsHaveSame=true;
							}
						}
						if(!m_bIsHaveSame)
						{
							bool m_bIsHaveCard=false;
							for(int s=0;s<m_bTempDCardCount;s++)
							{
								for(int n=0;n<m_bTempGetCardCount;n++)
								{
									if(m_GameLogic.GetCardLogicValue(m_bTempSCardData[m_bTempSCardCount-j-1])==m_GameLogic.GetCardLogicValue(m_bTempDCardData[j]))
										m_bIsHaveCard=true;
								}
							}
							if(m_bTempGetCardCount==AnalyseResult.bFourCount*4||!m_bIsHaveCard)
							{
								m_bTempGetCardData[AnalyseResult.bFourCount*4+m]=m_bTempSCardData[m_bTempSCardCount-j-1];
								m_bTempGetCardCount=AnalyseResult.bFourCount*4+m+1;
							}
							if(!m_bIsHaveCard)
							{
								n=m_bTempGetCardCount;
								j=m_bTempSCardCount;
							}
						}
					}
				}
			}
		}
		break;
	}
	bool bBomb=false;
	if(m_bTempGetCardCount==0)
	{
		m_bWhichKindSel=0;
		//�ж�ը���Ŀ�����
		if(m_bTempFCardCount>3)
		{
			for(i=m_bTempFCardCount-4;i>=0;i--)
			{
				if(m_GameLogic.CompareCard(&m_bTempFCardData[i],m_bHintCardData,4,m_bHintCardCount))
				{
					if((m_bWhichKindSel++)==0)
					{
						CopyMemory(m_bTempGetCardData,&m_bTempFCardData[i],4);
						bBomb=true;
						m_bTempGetCardCount=4;
					}
				}
			}
		}
		if(m_bTempGetCardCount==0)
		{
			if(m_bHandCardCount>1)
			{
				if(m_GameLogic.GetCardLogicValue(m_bHandCardData[0])>15&&m_GameLogic.GetCardLogicValue(m_bHandCardData[1])>15)
				{
					CopyMemory(m_bTempGetCardData,m_bHandCardData,2);
					m_bTempGetCardCount=2;
					if(m_WhichOnsKindCard==1)
						m_bWhichKindSel=1;
				}
			}
		}
	}
	BYTE m_GetIndex=0;
	if(m_bTempGetCardCount==0)
	{
		if(m_WhichOnsKindCard!=1)
		{
			if(m_bHintCardData[0]!=m_bTurnCardData[0])
			{
				m_bHintCardData[0]=0;
				AutoOutCard(0);
			}
			else
				OnPassCard(0,0);
		}
	}
	else
	{
		for(int j=0;j<m_bTempGetCardCount;j++)
		{
			for(i=0;i<m_bHandCardCount;i++)
			{
				if(m_bHandCardData[i]==m_bTempGetCardData[j])
				{
					m_bTempGetCardIndex[m_GetIndex++]=i;
				}
			}
		}

	}
	if(m_GameLogic.CompareCard(m_bTempGetCardData,m_bHintCardData,m_bTempGetCardCount,m_bHintCardCount))
	{
		if(m_WhichOnsKindCard==1&&m_bWhichKindSel==1||m_WhichOnsKindCard!=1)
		{
			//������ʾ����
			if(bBomb)
				m_bHintCardCount=4;
			CopyMemory(m_bHintCardData,m_bTempGetCardData,m_bHintCardCount);
			m_GameClientView.m_HandCardControl.SetShootCard(m_bTempGetCardIndex,m_GetIndex);
			m_GameClientView.m_btOutCard.EnableWindow(TRUE);
		}
	}
	else
	{
		if(m_WhichOnsKindCard!=1)
		{	
			if(m_bHintCardData[0]!=m_bTurnCardData[0])
			{
				memset(m_bHintCardData,0,sizeof(m_bHintCardData));
				AutoOutCard(0);
			}
			else
			{
				OnPassCard(0,0);
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////

