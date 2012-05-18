#include "Stdafx.h"
#include "GameClient.h"
#include "GameClientDlg.h"
#include ".\gameclientdlg.h"
#include "SettingDlg.h"



//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientDlg, CGameFrameDlg)
	ON_MESSAGE(IDM_BEGIN,OnHitBegin)
	ON_MESSAGE(IDM_PUBLIC_CARD,OnHitPublicCard)
	ON_MESSAGE(IDM_LEFT_HITCARD,OnHitCard)
	ON_MESSAGE(IDM_RIGHT_HITCARD,OnRightHitCard)
	//////////
	ON_MESSAGE(IDM_REVERT_CARD,OnRevertCard)

	ON_MESSAGE(IDM_OUT_CARD,OnHitOutCard)
	ON_MESSAGE(IDM_HINT,OnHint)

	ON_MESSAGE(IDM_LOOK_LAST,OnHitLastTurn)
	ON_MESSAGE(IDM_BACK_CARD,OnHitLookBack)
	ON_MESSAGE(IDM_AI,OnAI)
	ON_WM_TIMER()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientDlg::CGameClientDlg() : CGameFrameDlg(&m_GameClientView)
{
	m_bIsCanLook =FALSE;
	InitVariable();
	return;
}

//��������
CGameClientDlg::~CGameClientDlg()
{
}


void  CGameClientDlg::InitVariable()
{
	m_bFirstOutCnt=0;								
	memset(m_bFirstOutCards,0,sizeof(m_bFirstOutCards));

	m_bMeCardCnt=0;
	memset(m_bMeCard,0,sizeof(m_bMeCard));
	memset(m_bOutCardCnt,0,sizeof(m_bOutCardCnt));
	memset(m_bOutCard,0,sizeof(m_bOutCard));
	memset(m_bLastOutCardCnt,0,sizeof(m_bLastOutCardCnt));
	memset(m_bLastOutCard,0,sizeof(m_bLastOutCard));

	m_bPublicCardCnt=0;
	memset(m_bPublicCard,0,sizeof(m_bPublicCard));


	m_wCurSeat =INVALID_CHAIR;
	m_bIsAI=FALSE;
	m_bTimeCnt =0;
	m_bOverTimeCnt =0;

	////////////////////
	m_bAllMeCardCnt=0;
	memset(m_bAllMeCard,0,sizeof(m_bAllMeCard));

	m_bIsLookLast =FALSE;
	m_wNoReadyer =INVALID_CHAIR;
	m_wThrowSeat =INVALID_CHAIR;
	m_bCurSendCardPos=0;
	///////////
	m_bIsHaveScore=FALSE;
	m_bMaxCardCnt=0;
	memset(m_bMaxCard,0,sizeof(m_bMaxCard));

	m_bCurRound=0;
	m_bCurLookRound=255;
}



void	CGameClientDlg::KillAllTimer()
{
	KillGameTimer(TIME_ID_WAIT_ADHIBIT);
	KillGameTimer(TIME_ID_OUT_CARD);

	KillTimer(TIME_ID_SEND_CARD);
	KillTimer(TIME_ID_WASH_DESK);
//	KillTimer(TIME_ID_LAST_AROUND);
	KillTimer(TIME_ID_LOOK_PUBLIC);
	KillTimer(TIME_ID_SHOW_CARD);
}

//��ʼ����
bool CGameClientDlg::InitGameFrame()
{
	//���ñ���
	SetWindowText(TEXT("�����Ϸ����--240(����)"));

	//����ͼ��
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);

	m_GameClientView.m_bSunShi=AfxGetApp()->GetProfileInt(TEXT("GameOption"),TEXT("SunShiZhen"),FALSE)?true:false;







	/*
	m_Logic.SetNTHuaKind(0x00);

	BYTE bFirstCard[]={0x36};
	BYTE bMaxCard[]={0x36};
	BYTE bMeCards[] ={0x4e,0x4e,0x09,0x36,0x3c,0x3d,0x32};//0x06,0x06,0x02,0x07,0x08};//,0x04,0x06,0x07,0x07,0x01,0x0a};
	BYTE bOutCard[]={0x3c};
	int bOutCardCnt=sizeof(bOutCard);
	m_Logic.SortCard(bMeCards,NULL,sizeof(bMeCards));
	m_Logic.SortCard(bFirstCard,NULL,sizeof(bFirstCard));
	m_Logic.SortCard(bMaxCard,NULL,sizeof(bMaxCard));

	m_Logic.SortCard(bOutCard,NULL,sizeof(bOutCard));
	m_GameClientView.m_MeCard.SetCard(bMeCards,NULL,sizeof(bMeCards));

	bool bIsHaveScore=false;
	CString  str;
	bool bIsCanOut=m_Logic.CanOutCardEx(bOutCard,sizeof(bOutCard),bFirstCard,sizeof(bFirstCard),bMeCards,sizeof(bMeCards),\
		bMaxCard,sizeof(bMaxCard),bIsHaveScore);

//	bIsCanOut=m_Logic.CompareCardEx(bMaxCard,sizeof(bMaxCard),bFirstCard,sizeof(bFirstCard));
	str=bIsCanOut?"�ܳ�":"���ܳ�";
	InsertGeneralString(str,RGB(255,0,0),true);

	//m_Logic.AutoOutCardEx(bMeCards,sizeof(bMeCards),bFirstCard,sizeof(bFirstCard),bOutCard,bOutCardCnt,bMaxCard,sizeof(bMaxCard));
//	bIsCanOut=m_Logic.CompareCardEx(bMaxCard,sizeof(bMaxCard),bFirstCard,sizeof(bFirstCard));
//	str=bIsCanOut?"��":"С";
//	InsertGeneralString(str,RGB(255,0,0),true);

	m_GameClientView.m_OutCard[0].SetCard(bFirstCard,NULL,sizeof(bFirstCard));
	m_GameClientView.m_OutCard[1].SetCard(bMaxCard,NULL,sizeof(bMaxCard));
	m_GameClientView.m_OutCard[2].SetCard(bOutCard,NULL,bOutCardCnt);

	*/

	
	
	

	
	return true;
}

//���ÿ��
void CGameClientDlg::ResetGameFrame()
{
	InitVariable();
	//��ͼ��ԭ
	m_GameClientView.ResetGameView();
	KillAllTimer();
	return;
}

//��Ϸ����
void CGameClientDlg::OnGameOptionSet()
{
	CSettingDlg dlg;
	dlg.m_IsSound=IsEnableSound();
	dlg.m_bShunShiZhen=m_GameClientView.m_bSunShi;
	dlg.m_bIsCanLook =m_bIsCanLook?true:false;
	dlg.m_bIsCanChoose = !IsLookonMode();
	if(dlg.DoModal() == IDOK)
	{
		EnableSound(dlg.m_IsSound);
		m_GameClientView.m_bSunShi=dlg.m_bShunShiZhen;
		m_bIsCanLook = dlg.m_bIsCanLook;
		
		AfxGetApp()->WriteProfileInt(TEXT("GameOption"),TEXT("SunShiZhen"),dlg.m_bShunShiZhen?TRUE:FALSE);

		CRect rcCiew;
		m_GameClientView.GetClientRect(&rcCiew);
		m_GameClientView.RectifyGameView(rcCiew.Width(),rcCiew.Height());
		m_GameClientView.UpdateGameView(NULL);

		if(!IsLookonMode())
		{
			ClientLookInfo info;
			info.bIsCanLook = dlg.m_bIsCanLook;
			SendData(SUB_CS_LOOK_INFO,&info,sizeof(info));
		}
	}
	return;
}

//ʱ����Ϣ
bool CGameClientDlg::OnTimerMessage(WORD wChairID, UINT nElapse, UINT nTimerID)
{
	switch(nTimerID)
	{
	case	TIME_ID_WAIT_START:
		{
			//����Ƿ�������Ȼ�ڱ����趨ʱ����״̬
			WORD wReadyCnt=0;
			WORD wCurPlayerCnt=0;
			//�����Ƿ���Ҫ���õ���ʱ ��ʱ��
			for(int i=0;i<GAME_PLAYER;i++)
			{
				if(GetUserData(i))
				{
					wCurPlayerCnt++;
					if(GetUserData(i)->cbUserStatus ==US_READY)
					{
						wReadyCnt++;
					}
					else
					{
						m_wNoReadyer =i;
					}
				}
			}
			//ֻʣ��һ����û��׼��
			if(!(wReadyCnt == GAME_PLAYER-1 && wCurPlayerCnt ==GAME_PLAYER))
			{
				m_wNoReadyer =INVALID_CHAIR;
				SetTimer(TIME_ID_CHECK_READY,TIME_CNT_CHECK_READY,NULL);
				return false;
			}

			if(nElapse<1)
			{
				if(GetMeChairID() == wChairID)
				{
					PostMessage(WM_QUIT);
				}
				else
				{
					if(GetUserData(m_wNoReadyer))
					{
						CString hintInfo;
						hintInfo.Format(" [ %s ]��Ϊ̫��ʱ��û��ʼ���ѱ��߳���Ϸ",GetUserData(m_wNoReadyer)->szName);
						InsertGeneralString(hintInfo,RGB(255,0,0),true);
					}
				}

			}
			else if(nElapse < 10&&GetMeChairID() == wChairID)
			{
				if(GetMeChairID() == wChairID &&!IsLookonMode())
				{
				PlayGameSound(::AfxGetInstanceHandle(), 
					_T("GAMEWARNING"));
				}
			}
			return true;
		}
	case TIME_ID_WAIT_ADHIBIT:
		{
			m_GameClientView.m_Timer_Scale=nElapse/(float)(TIME_CNT_WAIT_ADHIBIT);
			m_GameClientView.UpdateGameView(&m_GameClientView.m_Rct_DymTimer);
			if(nElapse<1)
			{
				m_GameClientView.m_NtList.ShowWindow(SW_HIDE);
				if(!IsLookonMode())
				{
					SendData(SUB_CS_FINISH_ASHIBIT);
				}
			}
			return true;
		}

	case	TIME_ID_OUT_CARD:
		{
			m_GameClientView.m_Timer_Scale=nElapse/(float)(TIME_CNT_OUT_CARD);
			m_GameClientView.UpdateGameView(&m_GameClientView.m_Rct_DymTimer);
			bool	IsWatch	=   IsLookonMode();
			WORD    MeSeat=	GetMeChairID();
			//��ʱ�Զ�����
			if((!IsWatch) && (wChairID==MeSeat))
			{
				if(m_bIsAI)
				{
					m_bTimeCnt++;
				}
				if(m_bTimeCnt ==START_AI_CNT ||nElapse < 1)
				{
					//������ԭ
					m_bTimeCnt =0;
					if(nElapse <1)
					{
						m_bOverTimeCnt++;
						CString str;
						int cnt=MAX_OVER_TIME_CNT;
						str.Format("ϵͳ��ʾ:��������%d�γ�ʱ�������ʱ%d�Σ�ϵͳ����������Ϊ�й�״̬!",m_bOverTimeCnt,cnt);
						InsertGeneralString(str,RGB(255,0,0),true);
					}
					//��ʱ����̫�࣬���Զ��˳�
					if(m_bOverTimeCnt ==MAX_OVER_TIME_CNT)
					{
						m_bOverTimeCnt=0;
						if(!m_bIsAI)
						{
							OnAI(0,0);
						}
					}
					AutoOut();
					return false;
				}
				else if(nElapse < 10)
				{
					if(GetMeChairID() == wChairID &&!IsLookonMode())
					{
						PlayGameSound(::AfxGetInstanceHandle(), 
							_T("GAMEWARNING"));
					}
				}
			}
			return true;	
		}
	}
	return false;
}

//�Թ�״̬
void CGameClientDlg::OnLookonChanged(bool bLookonUser, const void * pBuffer, WORD wDataSize)
{
	return ;
}

void		 CGameClientDlg::UpdateNTList()
{
	if(IsLookonMode())  return;
	BOOL bIsNT[6]={0};
	BYTE bNTCnt[6] ={0};
	if(GetMeChairID()==m_GameClientView.m_wPublicChair)
		goto loop;
	BYTE NTNum =m_Logic.GetNTCardNum();
	for(int i=0;i<m_bMeCardCnt;i++)
	{
		if(m_Logic.GetCardNum(m_bMeCard[i]) ==NTNum)
		{
			bNTCnt[(m_Logic.GetCardHuaKind(m_bMeCard[i],true))>>4]++;
		}
		else if(m_bMeCard[i] ==0x4e)
		{
			bNTCnt[4] ++;
		}
		else if(m_bMeCard[i] ==0x4f)
		{
			bNTCnt[5] ++;
		}
	}
	if(m_bPublicCardCnt ==0)
	{
		for(int i=0;i<6;i++)
		{
			if((bNTCnt[i] >0&&i<4)||(bNTCnt[i]==2&&i>=4))
			{
				bIsNT[i] =TRUE;
			}
		}
		
	}
	else if(m_bPublicCardCnt ==1)
	{
		for(int i=0;i<6;i++)
		{
			if(bNTCnt[i] ==2)
			{
				bIsNT[i] =TRUE;
			}
		}	
	}
	//�����ƣ�ֻ��������
	else if(m_bPublicCardCnt ==2)
	{
		//m_bPublicCard[3] ==0��ʾ�Ƕ���
		BYTE bFrom=m_bPublicCard[0]>>4;
		if(m_bPublicCard[0] ==0x4f)
		{
			bFrom=5;
		}
		for(int i=bFrom+1;i<6;i++)
		{
			if(bNTCnt[i] ==2)
			{
				bIsNT[i] =TRUE;
			}
		}
	}
loop:
	m_GameClientView.m_NtList.UpdateNtList(bIsNT);
}

//������Ϣ
bool CGameClientDlg::OnGameMessage(WORD wSubCmdID, const void * pBuffer, WORD wDataSize)
{
	switch(wSubCmdID)
	{
		//���¿�ʼ����
	case	SUB_SC_BEGIN_SEND:
		{
			if(wDataSize !=sizeof(ServerBeginSend)) return false;
			PlayGameSound(::AfxGetInstanceHandle(),TEXT("GAMESTART"));
			ServerBeginSend *pCard=(ServerBeginSend *)pBuffer;
			return SubBeginSend(pCard);
		}
	case SUB_SC_SEND_FINISH:
		{	
			CString str =m_bPublicCardCnt!=0?TEXT("�ȴ���ҷ���..."):TEXT("�ȴ���ҽ���...") ;
			m_GameClientView.m_dlgHintInfo.SetShowInfo(str);
			//�ж��Ƿ��ܹ�������
			HideNTList();
			//�Թ���һ����ʾȫ���˿�
			if(IsLookonMode())
			{
				KillTimer(TIME_ID_SEND_CARD);
				memcpy(m_bMeCard,m_bAllMeCard,m_bAllMeCardCnt);
				m_bMeCardCnt = m_bAllMeCardCnt;
				SetSpaceCard();
			}
			//�ȴ��Ƿ������ճ��
			SetGameTimer(GetMeChairID(),TIME_ID_WAIT_ADHIBIT,TIME_CNT_WAIT_ADHIBIT);
			SetGameStatus(GS_WAIT_PUBLIC);
			//����δ�����Ƶ��Թ���һ�ΰ�����ʾ��
			m_GameClientView.UpdateGameView(NULL);
			return true;

		}
	case	SUB_NT_CARD:
		{
			if(wDataSize !=sizeof(ServerNtStruct)) return false;
			ServerNtStruct *pCards=(ServerNtStruct *)pBuffer;
			return SubNTCard(pCards);

		}

	case SUB_SC_BEGIN_OUT:
		{
			if(wDataSize !=sizeof(ServerBeginOut)) return false;
			ServerBeginOut *pSeat =(ServerBeginOut *)pBuffer;
			return SubBeginOut(pSeat);
		}
	case SUB_SC_THROW_CARD:	//�û�˦��ʧ��
		{
			ServerThrowCard * pShowCard=(ServerThrowCard *)pBuffer;
			//�Թ���������Ѿ������� 
			m_wThrowSeat=pShowCard->bDeskStation;
			if(GetMeChairID() == m_wThrowSeat &&IsLookonMode())
			{
				m_Logic.RemoveCard(pShowCard->iCardList,pShowCard->iCardCount,m_bMeCard,m_bMeCardCnt);
				m_bMeCardCnt -= pShowCard->iCardCount;
				SetSpaceCard();
			}
			//����
			if(m_bFirstOutCnt==0)
			{
				m_bFirstOutCnt =pShowCard->iResultCount;
				memcpy(m_bFirstOutCards,&pShowCard->iCardList[pShowCard->iCardCount],m_bFirstOutCnt);
				//������һ�ֵ��˿�
				if(m_bOutCardCnt[0]!=0)
				{
					memcpy(m_bLastOutCard[m_bCurRound],m_bOutCard,sizeof(m_bOutCard));
					memcpy(m_bLastOutCardCnt[m_bCurRound],m_bOutCardCnt,sizeof(m_bOutCardCnt));
					m_bCurRound++;
				}
				memset(m_bOutCard,0,sizeof(m_bOutCard));
				memset(m_bOutCardCnt,0,sizeof(m_bOutCardCnt));
				//�����˿�
				for(int i=0;i<GAME_PLAYER;i++)
				{
					m_GameClientView.m_OutCard[i].SetCard(NULL,NULL,0);
				}
				if(!m_bIsHaveScore)
				{
					m_bIsHaveScore=m_Logic.IsHaveScore(m_bFirstOutCards,m_bFirstOutCnt);
				}
				m_bMaxCardCnt=m_bFirstOutCnt;
				memcpy(m_bMaxCard,m_bFirstOutCards,m_bFirstOutCnt);
			}
			m_bOutCardCnt[m_wThrowSeat] = pShowCard->iCardCount;
			memcpy(m_bOutCard[m_wThrowSeat],pShowCard->iCardList,pShowCard->iCardCount);
			WORD wViewSeat =SwitchViewChairID(m_wThrowSeat);
			m_GameClientView.m_OutCard[wViewSeat].SetCard(pShowCard->iCardList,NULL,pShowCard->iCardCount);
			m_wCurSeat = pShowCard->wNextSeat;
			//��¼��ʵ�����˿�		
			if(GetUserData(pShowCard->bDeskStation))
			{
				CString str;
				str.Format("------ %s ˦��ʧ��,ǿ�Ƴ�С",GetUserData(pShowCard->bDeskStation)->szName);
				InsertGeneralString(str,RGB(255,0,0),true);
			}
			SetTimer(TIME_ID_SHOW_CARD,TIME_CNT_SHOW_CARD,NULL);
			return true;
		}
	case SUB_OUT_CARD:
		{
			ServerOutCard *pCard =(ServerOutCard *)pBuffer;
			return SubOutCard(pCard);
		}
	case SUB_SC_BATCH_OUT:
		{
			if(wDataSize !=sizeof(ServerBatchOut)) return false;
			ServerBatchOut *pCard =(ServerBatchOut *)pBuffer;
			return SubBatchOut(pCard);
		}
	case SUB_SC_GAME_POINT:
		{
			if(wDataSize !=sizeof(ServerGamePoint)) return false;
			ServerGamePoint *pCard =(ServerGamePoint *)pBuffer;
			return SubGamePoint(pCard);
		}
	case SUB_SC_GAME_END:
		{
			if(wDataSize !=sizeof(ServerGameEnd)) return false;
			ServerGameEnd *pEnd =(ServerGameEnd *)pBuffer;
			return SubGameEnd(pEnd);
		}
	default:
		{
			return false;
		}
	}
	return false;
}

bool	CGameClientDlg::SubBeginOut(ServerBeginOut *pSeat)
{
	m_Logic.SetNTHuaKind(m_GameClientView.m_bPublicCard);
	m_Logic.SortCard(m_bMeCard,NULL,m_bMeCardCnt);
	SetSpaceCard();
	m_GameClientView.m_btLastTurn.ShowWindow(SW_SHOW);

	m_wCurSeat =pSeat->wFirstOutSeat;
	SetGameStatus(GS_OUT_CARD);
	PlayGameSound(::AfxGetInstanceHandle(),TEXT("BEGINOUT"));
	m_GameClientView.m_dlgHintInfo.ShowWindow(SW_HIDE);
	SetGameTimer(m_wCurSeat,TIME_ID_OUT_CARD,TIME_CNT_OUT_CARD);

	if(GetMeChairID() == m_wCurSeat &&!IsLookonMode())
	{
		SetOutCardButton();
	}
	//���½���
	for(int i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.m_OutCard[i].SetCard(NULL,NULL,0);
	}
	m_GameClientView.m_wBankerChair=pSeat->wFirstOutSeat;
	if(m_GameClientView.m_wBankerChair!=INVALID_CHAIR)
	{
		m_GameClientView.m_wViewBankerChair=SwitchViewChairID(m_GameClientView.m_wBankerChair);
	}
	m_GameClientView.m_PublicCard.SetCard(m_bPublicCard,NULL,m_bPublicCardCnt);
	m_GameClientView.UpdateGameView(NULL);

#ifdef _DEBUG
	CString str,temp;
	str.Format("��λ%d���˿��� %d:",GetMeChairID(),m_bMeCardCnt);
	for(int i=0;i<m_bMeCardCnt;i++)
	{
		temp.Format("%x-",m_bMeCard[i]);
		str += temp;
	}
	InsertGeneralString(str,RGB(255,0,0),true);
#endif
	return true;
}


bool	CGameClientDlg::SubNTCard(ServerNtStruct *pCards)
{
	///////////////////////////////////
	m_bPublicCardCnt = pCards->bCardCount;
	memcpy(m_bPublicCard,pCards->bCard,m_bPublicCardCnt);
	m_GameClientView.m_bPublicCard=pCards->bHuaKind;
	///////////
	//��¼���������Ϣ
//	m_Logic.SetNTHuaKind(m_GameClientView.m_bPublicCard);
	//��ʾ�ϴ������ߵİ�ť
	if(GetMeChairID()==m_GameClientView.m_wPublicChair&&!IsLookonMode()) 
	{
		m_GameClientView.m_NtList.ShowWindow(SW_SHOW);
	}
	m_GameClientView.m_wPublicChair = pCards->wNTSeat;
	m_GameClientView.m_wViewPublicChair =SwitchViewChairID(m_GameClientView.m_wPublicChair);
	m_GameClientView.m_OutCard[m_GameClientView.m_wViewPublicChair].SetCard(m_bPublicCard,NULL,m_bPublicCardCnt);
	UpdateNTList();
	if(GetGameStatus()==GS_WAIT_PUBLIC&&!IsLookonMode())
	{
		HideNTList();
	}
	m_GameClientView.UpdateGameView(NULL);
	return true;
}

void	CGameClientDlg::HideNTList()
{
	bool bIsCanPublic=false;
	CBitmapButton *pBT[]={&m_GameClientView.m_NtList.m_Bm1,&m_GameClientView.m_NtList.m_Bm2,&m_GameClientView.m_NtList.m_Bm3,\
		&m_GameClientView.m_NtList.m_Bm4,&m_GameClientView.m_NtList.m_Bm5,&m_GameClientView.m_NtList.m_bt20};
	for(int i=0;i<sizeof(pBT)/sizeof(pBT[0]);++i) 
	{
		if(pBT[i]->IsWindowEnabled())  
		{
			bIsCanPublic=true;
			break;
		}
	}
	//�����������ذ�ť
	if(!bIsCanPublic)  m_GameClientView.m_NtList.ShowWindow(SW_HIDE);
}
bool	CGameClientDlg::SubBeginSend(ServerBeginSend *pCard)
{
	if(IsLookonMode())
	{
		ResetGameFrame();
	}
	m_bCurSendCardPos=0;
	memset(m_bMeCard,0,sizeof(m_bMeCard));
	m_bMeCardCnt=0;
	//����ȴ���ʼ��ʱ��
	KillTimer(TIME_ID_CHECK_READY);
	KillGameTimer(TIME_ID_WAIT_START);
	m_bIsCanLook = pCard->bIsCanLook;

	m_bAllMeCardCnt =PLAYER_CARD_CNT;
	memcpy(m_bAllMeCard,pCard->bAllCard,m_bAllMeCardCnt);
	m_GameClientView.UpdateGameView(NULL);

	if(pCard->bIsResend)
	{
		InsertGeneralString(TEXT("��Ϊ���˽�����ϵͳ���·���"),RGB(255,0,0),true);
		CString str = TEXT("�ȴ���ҽ���...") ;
		//��ʾ��ʾ��Ϣ
		m_GameClientView.m_dlgHintInfo.SetShowInfo(str);
	}
	
	SetTimer(TIME_ID_SEND_CARD,TIME_CNT_SEND_CARD,NULL);
	
	return true;
}
bool	CGameClientDlg::SubGameEnd(ServerGameEnd *pEnd)
{
	PlayGameSound(::AfxGetInstanceHandle(),TEXT("GAMEEND"));
	//��ʾ����
	SetGameStatus(GS_FREE);
	//��ʾ��Ϣ
	CString str=TEXT("");
	CString temp;
	str =TEXT("������÷���� ��");
//	InsertGeneralString(str,RGB(255,0,0),true);
	if(pEnd->bIsForceExit)
	{
		for(int tempSeat=m_GameClientView.m_wBankerChair;tempSeat<m_GameClientView.m_wBankerChair+GAME_PLAYER*3;tempSeat+=3)
		{
			int i=tempSeat%GAME_PLAYER;
			if(GetUserData(i))
			{
				temp.Format(" [ %s ] �÷�Ϊ %d",GetUserData(i)->szName,pEnd->iScore[i]);
				str +=  temp;
				m_GameClientView.m_ScoreWnd.SetGameScore(i,GetUserData(i)->szName,pEnd->iScore[i]);
				if(pEnd->iScore[i]>0)
				{
				//	InsertGeneralString(temp,RGB(0,0,255),true);
				}
				else
				{
				//	InsertGeneralString(temp,RGB(255,0,0),true);
				}
			}
		}
	}
	else
	{
		for(int tempSeat=m_GameClientView.m_wBankerChair;tempSeat<m_GameClientView.m_wBankerChair+GAME_PLAYER*3;tempSeat+=3)
		{
			int i=tempSeat%GAME_PLAYER;
			if(GetUserData(i))
			{
				temp.Format(" [ %s ] �÷�Ϊ %d",GetUserData(i)->szName,pEnd->iScore[i]);
				str +=  temp;
				m_GameClientView.m_ScoreWnd.SetGameScore(i,GetUserData(i)->szName,pEnd->iScore[i]);
				if(pEnd->iScore[i]>0)
				{
			//		InsertGeneralString(temp,RGB(0,0,255),true);
				}
				else
				{
			//		InsertGeneralString(temp,RGB(255,0,0),true);
				}
			}
		}
	}
	
	//����
	KillAllTimer();
	m_GameClientView.HideAllButton();
	if(!IsLookonMode())
	{
		m_GameClientView.m_btBegin.ShowWindow(SW_SHOW);
	}
	m_GameClientView.m_ScoreWnd.ShowWindow(SW_SHOW);
	SetTimer(TIME_ID_CHECK_READY,TIME_CNT_CHECK_READY,NULL);
	m_GameClientView.UpdateGameView(NULL);
	return true;
}

bool    CGameClientDlg::SubOutCard(ServerOutCard *p)
{	
/////////////////////////////////
#ifdef _DEBUG
	CString str,temp;
	str.Format("[%s]����Ϊ:",GetUserData(p->wSeat)->szName);
	for(int i=0;i<p->bOutCardCnt;i++)
	{
		temp.Format("%x--",p->bOutCard[i]);
		str +=temp;
	}
	InsertGeneralString(str,RGB(255,0,0),true);
#endif
///////////////////////////////
	PlayGameSound(::AfxGetInstanceHandle(),TEXT("OUTCARD"));
	WORD wMeChair =GetMeChairID();
	bool bWatcher =IsLookonMode();
	m_wCurSeat =p->wNextSeat;
	/////////////////////
	//������������Ϣ
	KillGameTimer(TIME_ID_OUT_CARD);
	if(wMeChair == p->wSeat)
	{
		if(IsLookonMode())
		{
			if(m_Logic.RemoveCard(p->bOutCard,p->bOutCardCnt,m_bMeCard,m_bMeCardCnt)==0)
			{
				return false;
			}
			m_bMeCardCnt -= p->bOutCardCnt;
			SetSpaceCard();
		}
		m_GameClientView.m_btOutCard.ShowWindow(SW_HIDE);
		m_GameClientView.m_btHint.ShowWindow(SW_HIDE);
	}
	//����
	if(m_bFirstOutCnt==0)
	{
		//��¼���ֳ�����
		m_bFirstOutCnt =p->bOutCardCnt;
		memcpy(m_bFirstOutCards,p->bOutCard,m_bFirstOutCnt);
		//������һ�ֵ��˿�(�����ƾͲ��ñ���)
		if(m_bOutCardCnt[0]!=0)
		{
			memcpy(m_bLastOutCard[m_bCurRound],m_bOutCard,sizeof(m_bOutCard));
			memcpy(m_bLastOutCardCnt[m_bCurRound],m_bOutCardCnt,sizeof(m_bOutCardCnt));
			m_bCurRound++;
		}
		memset(m_bOutCard,0,sizeof(m_bOutCard));
		memset(m_bOutCardCnt,0,sizeof(m_bOutCardCnt));
		//�����˿�
		for(int i=0;i<GAME_PLAYER;i++)
		{
			m_GameClientView.m_OutCard[i].SetCard(NULL,NULL,0);
		}
		m_bMaxCardCnt=m_bFirstOutCnt;
		memcpy(m_bMaxCard,m_bFirstOutCards,m_bMaxCardCnt);
	}
	else
	{
		if(m_Logic.CompareCardEx(p->bOutCard,p->bOutCardCnt,m_bMaxCard,m_bMaxCardCnt))
		{
			m_bMaxCardCnt=p->bOutCardCnt;
			memcpy(m_bMaxCard,p->bOutCard,p->bOutCardCnt);
		}
	}

	//��ʾ������Ϣ
	WORD wViewSeat = SwitchViewChairID(p->wSeat);
	m_GameClientView.m_OutCard[wViewSeat].SetCard(p->bOutCard,NULL,p->bOutCardCnt);
	//��¼��������
	m_bOutCardCnt[p->wSeat]=p->bOutCardCnt;
	memcpy(m_bOutCard[p->wSeat],p->bOutCard,p->bOutCardCnt);

	//һ�ֽ�������ԭ���ֳ�������
	if(p->bIsLastOut)
	{
		m_bFirstOutCnt=0;								
		memset(m_bFirstOutCards,0,sizeof(m_bFirstOutCards));
		m_bIsHaveScore=FALSE;
		m_bMaxCardCnt=0;
		memset(m_bMaxCard,0,sizeof(m_bMaxCard));
	}
	else
	{
		if(!m_bIsHaveScore)
		{
			m_bIsHaveScore=m_Logic.IsHaveScore(p->bOutCard,p->bOutCardCnt);
		}
		
	}
	if(wMeChair ==m_wCurSeat&&!bWatcher)
	{
		ActiveGameFrame();
		SetOutCardButton();
	}
	SetGameTimer(m_wCurSeat,TIME_ID_OUT_CARD,TIME_CNT_OUT_CARD);

	return true;
}


bool	CGameClientDlg::SubBatchOut(ServerBatchOut *p)
{
	if(m_bFirstOutCnt==0)
	{
		memcpy(m_bLastOutCard[m_bCurRound],m_bOutCard,sizeof(m_bOutCard));
		memcpy(m_bLastOutCardCnt[m_bCurRound],m_bOutCardCnt,sizeof(m_bOutCardCnt));
		m_bCurRound++;
	}
	for(int i=0;i<GAME_PLAYER;i++)
	{
		m_bOutCardCnt[i] =p->bOutCardCnt[i];
		memcpy(m_bOutCard[i],p->bOutCard[i],m_bOutCardCnt[i]);
		WORD wViewSeat=SwitchViewChairID(i);
		m_GameClientView.m_OutCard[wViewSeat].SetCard(p->bOutCard[i],NULL,p->bOutCardCnt[i]);
	}
	m_GameClientView.m_MeCard.SetCard(NULL,NULL,0);
	//�ƺ�
	KillGameTimer(TIME_ID_OUT_CARD);
	m_GameClientView.m_btOutCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btHint.ShowWindow(SW_HIDE);
	return true;
}


bool	CGameClientDlg::SubGamePoint(ServerGamePoint *pCard)
{
	WORD wSeat=SwitchViewChairID(pCard->wWiner);
	if(m_GameClientView.m_nCurScore[wSeat]==0)
	{
		m_GameClientView.m_Score_Card[wSeat].SetCard(pCard->bScoreCard,NULL,pCard->bScoreCardCnt);
	}
	else
	{
		BYTE bCards[25]={0};
		BYTE bCardsCnt=0;
		bCardsCnt =m_GameClientView.m_Score_Card[wSeat].GetCard(bCards,NULL);
		memcpy(&bCards[bCardsCnt],pCard->bScoreCard,pCard->bScoreCardCnt);
		bCardsCnt += pCard->bScoreCardCnt;
		m_GameClientView.m_Score_Card[wSeat].SetCard(bCards,NULL,bCardsCnt);
	}
	m_GameClientView.m_nCurScore[wSeat] += pCard->bScoreValue;
	m_GameClientView.UpdateGameView(NULL);
	return true;
}

void	CGameClientDlg::SetMeUpCard(BYTE bUpCards[],BYTE bUpCardsCnt)
{
	BYTE *bIsUp =new BYTE[m_bMeCardCnt];
	memset(bIsUp,FALSE,m_bMeCardCnt);
	for(int i=0;i<bUpCardsCnt;i++)
	{
		for(int j=0;j<m_bMeCardCnt;j++)
		{
			//����ֵ��ȵ��ƣ������ų�֮
			if(bUpCards[i] ==m_bMeCard[j])
			{
				if(bIsUp[j] ==FALSE)
				{
					bIsUp[j] =TRUE;
					break;
				}
			}
		}
	}
	BYTE bCards[5]={0};
	BYTE bCardCnt=0;
	for(BYTE i=1;i<m_bMeCardCnt;++i)
	{
		if(m_Logic.GetCardHuaKind(m_bMeCard[i],FALSE)!=m_Logic.GetCardHuaKind(m_bMeCard[i-1],FALSE))
		{
			bCards[bCardCnt++]=i;
		}
	}
	m_GameClientView.m_MeCard.SetAddParameter(bCards,bCardCnt);

	m_GameClientView.m_MeCard.SetCard(m_bMeCard,bIsUp,m_bMeCardCnt);
	SafeDeleteArray(bIsUp);
}

LRESULT	CGameClientDlg::OnHitBegin(WPARAM wparam, LPARAM lparam)
{
	//���ý���
	ResetGameFrame();
	//׼����ʼ
	SendUserReady(NULL,0);
	PlayGameSound(::AfxGetInstanceHandle(),TEXT("READYGO"));
	return 0;
}

LRESULT	CGameClientDlg::OnHitPublicCard(WPARAM wparam, LPARAM lparam)
{
	
	ClientNtStruct PublicCard;
	memset(&PublicCard,0,sizeof(PublicCard));
	BYTE bPublicCard=0;
	if(wparam <UG_NT_CARD_KING)
	{
		BYTE bNTNum = m_Logic.GetNTCardNum()-1;
		bPublicCard= bNTNum | (BYTE)wparam;
	}
	else if(wparam == UG_NT_CARD_KING)
	{
		if(lparam==0)
		{
			bPublicCard=0x4e;
		}
		else
		{
			bPublicCard=0x4f;
		}
	}
	else
	{
		AfxMessageBox("������Ϣ���밴ȷ������",MB_ICONINFORMATION);
		return 0;
	}
	for(int i=0;i<m_bMeCardCnt;i++)
	{
		if(m_bMeCard[i] == bPublicCard)
		{
			PublicCard.bCard[PublicCard.bCardCount++] = m_bMeCard[i];
		}
	}
	ASSERT(PublicCard.bCardCount <3);
	m_GameClientView.m_NtList.ShowWindow(SW_HIDE);
	SendData(SUB_NT_CARD,&PublicCard,sizeof(PublicCard));
	return 0;
}

//�����˿˰�ť����
LRESULT	CGameClientDlg::OnHitCard(WPARAM wparam, LPARAM lparam)
{
	BYTE bGameStatus=GetGameStatus();

	BYTE  bCards[PLAYER_CARD_CNT]={0};
	BYTE bCardsCnt=0;
	bCardsCnt =m_GameClientView.m_MeCard.GetUpCard(bCards);

	//��������ڲ鿴
	if(m_bIsLookLast)
	{
		RevertDeskCard();
	}

	BYTE  bMeCards[2*PLAYER_CARD_CNT]={0};
	BYTE bMeCardsCnt=0;
	bMeCardsCnt=m_GameClientView.m_MeCard.GetCard(NULL,bMeCards);

	if(m_bFirstOutCnt!=1)
	{
		BYTE bEqualPos=255;
		if(((BYTE)wparam<m_bMeCardCnt-1) && (m_bMeCard[wparam] ==m_bMeCard[wparam+1]) )
		{
			bEqualPos =wparam+1;
		}
		if(wparam >0 &&m_bMeCard[wparam] ==m_bMeCard[wparam-1])
		{
			bEqualPos =wparam-1;
		}
		if(bEqualPos!=255)
		{
			if(lparam == TRUE && !bMeCards[bEqualPos]  &&!(bGameStatus == GS_OUT_CARD\
				&&m_bFirstOutCnt !=0 && bCardsCnt +1>m_bFirstOutCnt))
			{
				bCards[bCardsCnt] =m_bMeCard[wparam];
				bCardsCnt++;
				SetMeUpCard(bCards,bCardsCnt);
				m_Logic.SortCard(bCards,NULL,bCardsCnt);
			}
		}
	}
	//ͬ������Ҳһ���

	switch(bGameStatus)
	{
	case GS_OUT_CARD:
		{
			if(m_Logic.CanOutCardEx(bCards,bCardsCnt,m_bFirstOutCards,m_bFirstOutCnt,m_bMeCard,m_bMeCardCnt,\
				m_bMaxCard,m_bMaxCardCnt,m_bIsHaveScore))
			{
				m_GameClientView.m_btOutCard.EnableWindow(TRUE);
				//���������һ��
				if(m_bCurRound==0&&m_bFirstOutCnt==0&&m_bPublicCardCnt==2&&m_bMeCardCnt==PLAYER_CARD_CNT)
				{
					if(!(bCardsCnt==2&&bCards[0]==bCards[1]&&bCards[0]==m_bPublicCard[0]))
					{
						m_GameClientView.m_btOutCard.EnableWindow(FALSE);
					}
				}
				
			}
			else
			{
				m_GameClientView.m_btOutCard.EnableWindow(FALSE);
			}
			return 0;
		}
	default:
		{
			return 0;
		}
	}
	return 0;
}


//��ʾ���ư�ť
LRESULT	CGameClientDlg::OnHint(WPARAM wparam, LPARAM lparam)
{
	BYTE bOutCard[PLAYER_CARD_CNT]={0};
	int bOutCardCnt=0;
//	m_Logic.AutoOutCard(m_bMeCard,m_bMeCardCnt,m_bFirstOutCards,m_bFirstOutCnt,bOutCard,bOutCardCnt);
	m_Logic.AutoOutCardEx(m_bMeCard,m_bMeCardCnt,m_bFirstOutCards,m_bFirstOutCnt,bOutCard,bOutCardCnt,m_bMaxCard,m_bMaxCardCnt);
	//���������һ��
	if(m_bCurRound==0&&m_bFirstOutCnt==0&&m_bPublicCardCnt==2&&m_bMeCardCnt==PLAYER_CARD_CNT)
	{
		bOutCardCnt=2;
		memcpy(bOutCard,m_bPublicCard,m_bPublicCardCnt);
	}
	SetMeUpCard(bOutCard,bOutCardCnt);
	m_GameClientView.m_btOutCard.EnableWindow(TRUE);
	return 0;
}
LRESULT	CGameClientDlg::OnRevertCard(WPARAM wparam, LPARAM lparam)
{
	//��������ڲ鿴
	if(m_bIsLookLast)
	{
		RevertDeskCard();
	}
	return 0;
}
//�������ư�ť����
LRESULT	CGameClientDlg::OnHitOutCard(WPARAM wparam, LPARAM lparam)
{
	//�Լ������ƾͰѴ�����ԭ
	m_bOverTimeCnt=0;

	m_GameClientView.m_btOutCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btHint.ShowWindow(SW_HIDE);
	BYTE bCards[PLAYER_CARD_CNT]={0};
	BYTE bCardsCnt=0;
	bCardsCnt =m_GameClientView.m_MeCard.GetUpCard(bCards);

	SendOutCard(bCards,bCardsCnt);

	return 0;
	
}
void	CGameClientDlg::SendOutCard(BYTE bCards[],BYTE bCardsCnt)
{
	//��֮�����ʾ
	if(m_Logic.RemoveCard(bCards,bCardsCnt,m_bMeCard,m_bMeCardCnt)==0||bCardsCnt==0)
	{
		AfxMessageBox("ѡ�ƴ���!");
		return;
	}
	m_bMeCardCnt-=bCardsCnt;

	SetSpaceCard();
	//�����˿�
	ClientOutCard OutCard;
	memset(&OutCard,0,sizeof(OutCard));
	OutCard.wSeat =GetMeChairID();
	OutCard.bOutCardCnt=bCardsCnt;
	memcpy(OutCard.bOutCard,bCards,bCardsCnt);
	SendData(SUB_OUT_CARD,&OutCard,sizeof(OutCard)-(sizeof(OutCard.bOutCard)-bCardsCnt*sizeof(OutCard.bOutCard[0])));
#ifdef _DEBUG
	CString str,temp;
	str.Format("���ȳ����Ƹ���%d:",m_bFirstOutCnt);
	for(int i=0;i<m_bFirstOutCnt;i++)
	{
		temp.Format("%x",m_bFirstOutCards[i]);
		str +=temp;
	}
	str+="\n";
	temp.Format("����Ƹ���%d:",m_bMaxCardCnt);
	str +=temp;
	for(int i=0;i<m_bMaxCardCnt;i++)
	{
		temp.Format("%x",m_bMaxCard[i]);
		str +=temp;
	}
	str+="\n";
	temp=m_bIsHaveScore?"�з�":"û�з�";
	str +=temp;
	InsertGeneralString(str,RGB(255,0,0),true);

#endif


	//�ƺ���
	KillGameTimer(TIME_ID_OUT_CARD);
	m_GameClientView.m_btOutCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btHint.ShowWindow(SW_HIDE);
	m_GameClientView.m_OutCard[SwitchViewChairID(GetMeChairID())].SetCard(bCards,NULL,bCardsCnt);
}

//�������ְ�ť����
LRESULT	CGameClientDlg::OnHitLastTurn(WPARAM wparam, LPARAM lparam)
{
	//�����Ƿ�����һ�ֵ���,�Ƿ����ڲ鿴��һ��
	int nLastCnt=0;
	for(int i=0;i<GAME_PLAYER;i++)
	{
		nLastCnt += m_bLastOutCardCnt[0][i];
	}
	if(nLastCnt ==0||m_bCurRound==0)//||m_bIsLookLast)
	{
		return 0;
	}
	if(!m_bIsLookLast||m_bCurLookRound==255)
	{
		m_bCurLookRound=m_bCurRound-1;
	}
	else
	{
		if(m_bCurLookRound==0)
		{
			m_bCurLookRound=m_bCurRound-1;
		}
		else
		{
			m_bCurLookRound--;
		}	
	}
	m_bIsLookLast =TRUE;
	for(int i=0;i<GAME_PLAYER;i++)
	{
		WORD wViewSeat=SwitchViewChairID(i);
		m_GameClientView.m_OutCard[wViewSeat].SetCard(m_bLastOutCard[m_bCurLookRound][i],NULL,m_bLastOutCardCnt[m_bCurLookRound][i]);
	}
//	SetTimer(TIME_ID_LAST_AROUND,TIME_CNT_LAST_AROUND,NULL);
	return 0;
}
//���������ư�ť����
LRESULT	CGameClientDlg::OnHitLookBack(WPARAM wparam, LPARAM lparam)
{
	if(!IsLookonMode() &&GetMeChairID() == m_GameClientView.m_wBankerChair)
	{
		;
	}
	return 0;
}
//�Ҽ�����
LRESULT	CGameClientDlg::OnRightHitCard(WPARAM wparam, LPARAM lparam)
{
	if(GetGameStatus() ==GS_OUT_CARD &&!IsLookonMode()&&m_wCurSeat ==GetMeChairID() &&m_GameClientView.m_btOutCard.IsWindowVisible())
	{
		BYTE	bCards[2*PLAYER_CARD_CNT]={0};
		BYTE	bCardsCnt=0;
		bCardsCnt =m_GameClientView.m_MeCard.GetUpCard(bCards);
		if(m_Logic.CanOutCardEx(bCards,bCardsCnt,m_bFirstOutCards,m_bFirstOutCnt,m_bMeCard,m_bMeCardCnt,\
				m_bMaxCard,m_bMaxCardCnt,m_bIsHaveScore))
		{
			//���������һ��
			if(m_bCurRound==0&&m_bFirstOutCnt==0&&m_bPublicCardCnt==2&&m_bMeCardCnt==PLAYER_CARD_CNT)
			{
				if(!(bCardsCnt==2&&bCards[0]==bCards[1]&&bCards[0]==m_bPublicCard[0]))
				{
					return 0;
				}
			}
			BYTE bCards[PLAYER_CARD_CNT]={0};
			BYTE bCardsCnt=0;
			bCardsCnt =m_GameClientView.m_MeCard.GetUpCard(bCards);
			//�����˿�
			SendOutCard(bCards,bCardsCnt);
		}
	}
	return 0;
}
//AI��ť
LRESULT	CGameClientDlg::OnAI(WPARAM wparam, LPARAM lparam)
{
	if(GetGameStatus() !=GS_OUT_CARD)
	{
		return 0;
	}
	m_bIsAI =!m_bIsAI;

	if(m_bIsAI)
	{
		m_GameClientView.m_btAI.SetButtonImage(IDB_BT_CANCLE_AI,::AfxGetInstanceHandle(),false);
		//�ǵ�ǰ�����߰��İ�ť
		if(m_wCurSeat ==GetMeChairID() &&!IsLookonMode())
		{
			int temp =START_AI_CNT;
			CString str;
			str.Format("--�йܽ��� %d ���������",temp);
			InsertGeneralString(str,RGB(255,0,0),true);
		}
	}
	else
	{
		m_GameClientView.m_btAI.SetButtonImage(IDB_BT_AI,::AfxGetInstanceHandle(),false);
	}




	return 0;
}

void	CGameClientDlg::SetOutCardButton()
{
	m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
	m_GameClientView.m_btHint.ShowWindow(SW_SHOW);


	BYTE  bCards[2*PLAYER_CARD_CNT]={0};
	int bCardsCnt=0;
	bCardsCnt =m_GameClientView.m_MeCard.GetUpCard(bCards);
	//����ǰ��������Ƿ�ɳ�
	if(bCardsCnt )
	{
		if(m_Logic.CanOutCardEx(bCards,bCardsCnt,m_bFirstOutCards,m_bFirstOutCnt,m_bMeCard,m_bMeCardCnt,\
				m_bMaxCard,m_bMaxCardCnt,m_bIsHaveScore))
		{
			m_GameClientView.m_btOutCard.EnableWindow(TRUE);
			//���������һ��
			if(m_bCurRound==0&&m_bFirstOutCnt==0&&m_bPublicCardCnt==2&&m_bMeCardCnt==PLAYER_CARD_CNT)
			{
				if(!(bCardsCnt==2&&bCards[0]==bCards[1]&&bCards[0]==m_bPublicCard[0]))
				{
					m_GameClientView.m_btOutCard.EnableWindow(FALSE);
				}
			}
		}
		else
		{
			m_GameClientView.m_btOutCard.EnableWindow(FALSE);
		}
	}
	else
	{
		m_GameClientView.m_btOutCard.EnableWindow(FALSE);
		/*
		if(m_GameClientView.m_btOutCard.IsWindowEnabled()==FALSE && m_bFirstOutCnt !=0)
		{
			//�Զ������ܳ�����
			m_Logic.AutoOutCard(m_bMeCard,m_bMeCardCnt,m_bFirstOutCards,m_bFirstOutCnt,bCards,bCardsCnt);
			SetMeUpCard(bCards,bCardsCnt);
			m_GameClientView.m_btOutCard.EnableWindow(TRUE);
		}
		*/
	}
}

void	CGameClientDlg::AutoOut()
{
	BYTE bOutCard[PLAYER_CARD_CNT]={0};
	int bOutCardCnt=0;
	if(!m_Logic.AutoOutCardEx(m_bMeCard,m_bMeCardCnt,m_bFirstOutCards,\
		m_bFirstOutCnt,bOutCard,bOutCardCnt,m_bMaxCard,m_bMaxCardCnt))
	{
		AfxMessageBox(TEXT("�й�ʧ�ܣ����û��Լ�ѡ�����!"),MB_ICONWARNING);
		return;
	}
	if(m_bCurRound==0&&m_bFirstOutCnt==0&&m_bPublicCardCnt==2&&m_bMeCardCnt==PLAYER_CARD_CNT)
	{
		bOutCardCnt=2;
		memcpy(bOutCard,m_bPublicCard,m_bPublicCardCnt);
	}
	SendOutCard(bOutCard,bOutCardCnt);
}

//��Ϸ����
bool CGameClientDlg::OnGameSceneMessage(BYTE cbGameStation, bool bLookonOther, const void * pBuffer, WORD wDataSize)
{
	bool bIsLook =IsLookonMode();
	//�Թ��߲��ܵ��˿�
	switch(cbGameStation)
	{
	case GS_FREE:
		{
			if(!IsLookonMode())
			{
				m_GameClientView.m_btBegin.ShowWindow(SW_SHOW);
			}
			SetTimer(TIME_ID_CHECK_READY,TIME_CNT_CHECK_READY,NULL);
			return true;

		}
	case GS_SEND_CARD:
		{
			if(wDataSize !=sizeof(GS_SendStruct)) return false;
			GS_SendStruct *pStatusStruct=(GS_SendStruct *)pBuffer;

			m_bIsCanLook = pStatusStruct->bIsCanLook;
			if(!m_bIsCanLook && IsLookonMode())
			{
				m_GameClientView.m_MeCard.SetShowCard(false);
			}
			

			m_GameClientView.m_wBankerChair =pStatusStruct->wBanker;
			m_GameClientView.m_bPublicCard =pStatusStruct->bPublicCardFlower;
			if(m_GameClientView.m_wBankerChair !=INVALID_CHAIR)
			{
				m_GameClientView.m_wViewBankerChair =SwitchViewChairID(m_GameClientView.m_wBankerChair);
			}
			//��ǰ������ʾ
			m_bPublicCardCnt =pStatusStruct->bPublicCardCnt;
			if(m_bPublicCardCnt )
			{
				memcpy(m_bPublicCard,pStatusStruct->bPublicCard,m_bPublicCardCnt);
			}

			m_GameClientView.m_wPublicChair =pStatusStruct->wPublic;
			if(m_GameClientView.m_wPublicChair !=INVALID_CHAIR)
			{
				m_GameClientView.m_wViewPublicChair =SwitchViewChairID(m_GameClientView.m_wPublicChair);
				m_GameClientView.m_OutCard[m_GameClientView.m_wViewPublicChair].SetCard(m_bPublicCard,NULL,m_bPublicCardCnt);
			}
			
			//��������
			m_bAllMeCardCnt =pStatusStruct->bAllMeCardCnt;
			memcpy(m_bAllMeCard,pStatusStruct->bAllMeCard,m_bAllMeCardCnt);
			//�趨ʱ��
			SetTimer(TIME_ID_SEND_CARD,TIME_CNT_SEND_CARD,NULL);
			CString str =m_bPublicCardCnt!=0?TEXT("�ȴ���ҷ���..."):TEXT("�ȴ���ҽ���...") ;
			m_GameClientView.m_dlgHintInfo.SetShowInfo(str);

			//��ʾ���ܰ�ť
	

			m_GameClientView.UpdateGameView(NULL);
			return true;

		}
	case GS_WAIT_PUBLIC:
		{
			if(wDataSize !=sizeof(GS_WaitStruct)) return false;
			GS_WaitStruct *pStatusStruct=(GS_WaitStruct *)pBuffer;

			m_bIsCanLook = pStatusStruct->bIsCanLook;
			if(!m_bIsCanLook && IsLookonMode())
			{
				m_GameClientView.m_MeCard.SetShowCard(false);
			}
			m_GameClientView.m_wBankerChair =pStatusStruct->wBanker;
			m_GameClientView.m_bPublicCard =pStatusStruct->bPublicCardFlower;
			m_Logic.SetNTHuaKind(m_GameClientView.m_bPublicCard);
			//��������
			if(m_GameClientView.m_wBankerChair !=INVALID_CHAIR)
			{
				m_GameClientView.m_wViewBankerChair =SwitchViewChairID(m_GameClientView.m_wBankerChair);
			}
			m_GameClientView.m_wPublicChair =pStatusStruct->wPublic;
			if(m_GameClientView.m_wPublicChair !=INVALID_CHAIR)
			{
				m_GameClientView.m_wViewPublicChair =SwitchViewChairID(m_GameClientView.m_wPublicChair);
				m_GameClientView.m_OutCard[m_GameClientView.m_wViewPublicChair].SetCard(m_bPublicCard,NULL,m_bPublicCardCnt);
			}

			//��ǰ������ʾ
			m_bPublicCardCnt =pStatusStruct->bPublicCardCnt;
			if(m_bPublicCardCnt )
			{
				memcpy(m_bPublicCard,pStatusStruct->bPublicCard,m_bPublicCardCnt);
			}
			m_bMeCardCnt =pStatusStruct->bMeCardCnt;
			memcpy(m_bMeCard,pStatusStruct->bMeCard,m_bMeCardCnt);
			SetSpaceCard();

			if(!IsLookonMode())
			{
				m_GameClientView.m_NtList.ShowWindow(SW_SHOW);
				UpdateNTList();
				HideNTList();
			}
			SetGameTimer(GetMeChairID(),TIME_ID_WAIT_ADHIBIT,TIME_CNT_WAIT_ADHIBIT);

			CString str =m_bPublicCardCnt!=0?TEXT("�ȴ���ҷ���..."):TEXT("�ȴ���ҽ���...") ;
			m_GameClientView.m_dlgHintInfo.SetShowInfo(str);

			m_GameClientView.UpdateGameView(NULL);

			return true;
		}
	case GS_OUT_CARD:
		{
			if(wDataSize !=sizeof(GS_OutStruct)) return false;
			GS_OutStruct *pStatusStruct=(GS_OutStruct *)pBuffer;

			m_bIsCanLook = pStatusStruct->bIsCanLook;
			if(!m_bIsCanLook && IsLookonMode())
			{
				m_GameClientView.m_MeCard.SetShowCard(false);
			}

			m_GameClientView.m_wBankerChair =pStatusStruct->wBanker;
			m_GameClientView.m_bPublicCard =pStatusStruct->bPublicCardFlower;

			m_wThrowSeat = pStatusStruct->wThrowSeat;
			//��������
			m_Logic.SetNTHuaKind(m_GameClientView.m_bPublicCard);

			if(m_GameClientView.m_wBankerChair !=INVALID_CHAIR)
			{
				m_GameClientView.m_wViewBankerChair =SwitchViewChairID(m_GameClientView.m_wBankerChair);
			}
			m_GameClientView.m_wPublicChair =pStatusStruct->wPublic;
			if(m_GameClientView.m_wPublicChair !=INVALID_CHAIR)
			{
				m_GameClientView.m_wViewPublicChair =SwitchViewChairID(m_GameClientView.m_wPublicChair);
			}

			m_bFirstOutCnt = pStatusStruct->bFirstCardCnt;
			memcpy(m_bFirstOutCards,pStatusStruct->bFirstCard,m_bFirstOutCnt);

			//��ʾ�ҵ��˿�
			m_bMeCardCnt =pStatusStruct->bMeCardCnt;
			memcpy(m_bMeCard,pStatusStruct->bMeCard,m_bMeCardCnt);
			SetSpaceCard();

			//������ʾ
			//m_GameClientView.m_nCurScore =pStatusStruct->nCurScore;
			//m_GameClientView.m_PublicCard.SetCard(pStatusStruct->bScoreCard,NULL,pStatusStruct->bScoreCardCnt);

			//������ʾ
			m_GameClientView.m_PublicCard.SetCard(pStatusStruct->bPublicCard,NULL,pStatusStruct->bPublicCardCnt);
			//������ʾ
			WORD wViewSeat=0;
			for(int i=0;i<GAME_PLAYER;i++)
			{
				wViewSeat =SwitchViewChairID(i);
				///////////////////
				m_GameClientView.m_nCurScore[wViewSeat]=pStatusStruct->nCurScore[i];
				m_GameClientView.m_Score_Card[wViewSeat].SetCard(pStatusStruct->bScoreCard[i],NULL,pStatusStruct->bScoreCardCnt[i]);
				////////////////////
				m_GameClientView.m_OutCard[wViewSeat].SetCard(pStatusStruct->bOutCard[i],NULL,pStatusStruct->bOutCardCnt[i]);
			}
			memcpy(m_bOutCard,pStatusStruct->bOutCard,sizeof(m_bOutCard));
			memcpy(m_bOutCardCnt,pStatusStruct->bOutCardCnt,sizeof(m_bOutCardCnt));

			if(pStatusStruct->bOutHandCnt!=0)
			{
				m_bCurRound =(pStatusStruct->bOutHandCnt-1)/GAME_PLAYER;
			}
			memcpy(m_bLastOutCard,pStatusStruct->bLastOutCard,sizeof(m_bLastOutCard));
			memcpy(m_bLastOutCardCnt,pStatusStruct->bLastOutCardCnt,sizeof(m_bLastOutCardCnt));
			m_bIsHaveScore=pStatusStruct->bIsHaveScore;
			m_bMaxCardCnt=pStatusStruct->bMaxCardCnt;
			memcpy(m_bMaxCard,pStatusStruct->bMaxCard,m_bMaxCardCnt);

			m_wCurSeat =pStatusStruct->wCurSeat;
			if(m_wCurSeat ==GetMeChairID() && !bIsLook)
			{
				m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
				m_GameClientView.m_btOutCard.EnableWindow(FALSE);
				m_GameClientView.m_btHint.ShowWindow(SW_SHOW);
			}
			SetGameTimer(m_wCurSeat,TIME_ID_OUT_CARD,TIME_CNT_OUT_CARD);

			return true;
		}
	default:
		{
			AfxMessageBox("��p!������Ϸ��!!!!!",MB_ICONWARNING);
			return false;
		}
	}
}

bool CGameClientDlg::OnFrameMessage(WORD wSubCmdID, const void * pBuffer, WORD wDataSize)
{
	return false;
}

//////////////////////////////////////////////////////////////////////////

void  CGameClientDlg::RevertDeskCard()
{
	m_bIsLookLast =FALSE;
	for(int i=0;i<GAME_PLAYER;i++)
	{
		WORD wViewSeat=SwitchViewChairID(i);
		m_GameClientView.m_OutCard[wViewSeat].SetCard(m_bOutCard[i],NULL,m_bOutCardCnt[i]);
	}
//	KillTimer(TIME_ID_LAST_AROUND);

}
void CGameClientDlg::OnTimer(UINT nIDEvent)
{
	switch(nIDEvent)
	{
	case TIME_ID_SHOW_CARD:
		{
			//����ʵ�ļ���
			if(m_wThrowSeat !=INVALID_CHAIR)
			{
				WORD  wViewSeat =SwitchViewChairID(m_wThrowSeat);
				m_GameClientView.m_OutCard[wViewSeat].SetCard(m_bFirstOutCards,NULL,m_bFirstOutCnt);

				if(!m_Logic.RemoveCard(m_bFirstOutCards,m_bFirstOutCnt,m_bOutCard[m_wThrowSeat],m_bOutCardCnt[m_wThrowSeat]))
				{
					InsertGeneralString("˦���������",RGB(255,0,0),true);
				}
				else
				{
					// �õ�Ӧ���ջ�������
					m_bOutCardCnt[m_wThrowSeat] -=m_bFirstOutCnt;
					if(GetMeChairID() == m_wThrowSeat)
					{
						memcpy(&m_bMeCard[m_bMeCardCnt],m_bOutCard[m_wThrowSeat],m_bOutCardCnt[m_wThrowSeat]);
						m_bMeCardCnt +=m_bOutCardCnt[m_wThrowSeat];
						m_Logic.SortCard(m_bMeCard,NULL,m_bMeCardCnt);
						SetSpaceCard();
					}
					//

				}
				//��ԭ��ʵ�ĳ���
				m_bOutCardCnt[m_wThrowSeat] =m_bFirstOutCnt;
				memcpy(m_bOutCard[m_wThrowSeat],m_bFirstOutCards,m_bFirstOutCnt);
			}
			else
			{
				InsertGeneralString("˦�ƴ���",RGB(255,0,0),true);
			}
			//���ó���
			SetGameTimer(m_wCurSeat,TIME_ID_OUT_CARD,TIME_CNT_OUT_CARD);
			if(GetMeChairID() == m_wCurSeat &&!IsLookonMode())
			{
				SetOutCardButton();
			}
			KillTimer(TIME_ID_SHOW_CARD);

			break;
		}
	case TIME_ID_CHECK_READY:
		{
			WORD wReadyCnt=0;
			WORD wCurPlayerCnt=0;
			//�����Ƿ���Ҫ���õ���ʱ ��ʱ��
			for(int i=0;i<GAME_PLAYER;i++)
			{
				if(GetUserData(i))
				{
					wCurPlayerCnt++;
					if(GetUserData(i)->cbUserStatus ==US_READY)
					{
						wReadyCnt++;
					}
					else
					{
						m_wNoReadyer =i;
					}
				}
			}

			//ֻʣ��һ����û��׼��
			if(wReadyCnt == GAME_PLAYER-1 && wCurPlayerCnt ==GAME_PLAYER)
			{
				SetGameTimer(m_wNoReadyer,TIME_ID_WAIT_START,TIME_CNT_WAIT_START);
				KillTimer(TIME_ID_CHECK_READY);
			}
			break;
		}
	case TIME_ID_SEND_CARD:
		{
			if(m_bMeCardCnt >1)
			{
				PlayGameSound(::AfxGetInstanceHandle(),TEXT("SENDCARD"));
			}
			//�����ҵ��˿�
			m_bMeCard[m_bMeCardCnt++]=m_bAllMeCard[m_bCurSendCardPos++];
			//���·��ư�ť
			m_Logic.SortCard(m_bMeCard,NULL,m_bMeCardCnt);
			UpdateNTList();
			BYTE cards[PLAYER_CARD_CNT]={0};
			BYTE cardsCnt=m_GameClientView.m_MeCard.GetUpCard(cards);
			if(cardsCnt ==0)
			{
				SetSpaceCard();
			}
			else
			{
				SetMeUpCard(cards,cardsCnt);
			}

			if(m_bMeCardCnt ==1)
			{
				SetGameStatus(GS_SEND_CARD);
				CString str = TEXT("�ȴ���ҽ���...") ;
				//��ʾ��ʾ��Ϣ
				m_GameClientView.m_dlgHintInfo.SetShowInfo(str);
				if(!IsLookonMode())
				{
					m_GameClientView.m_NtList.ShowWindow(SW_SHOW);
				}
			}
			if(m_bCurSendCardPos==m_bAllMeCardCnt)
			{
				
				SetGameStatus(GS_WAIT_PUBLIC);
				CString str =TEXT("�ȴ�������ҷ��˿�...");
				//��ʾ��ʾ��Ϣ
				m_GameClientView.m_dlgHintInfo.SetShowInfo(str);
				m_GameClientView.UpdateGameView(NULL);
				if(!IsLookonMode())
				{
					//����������� ������ɵ���Ϣ
					SendData(SUB_CS_SEND_FINISH);
				}
				KillTimer(TIME_ID_SEND_CARD);
			}
			break;
		}
	case TIME_ID_LOOK_BACK:
		{
			KillTimer(TIME_ID_LOOK_BACK);
			break;

		}
	case TIME_ID_WASH_DESK:
		{
			//������һ�ֵ��˿�
			memcpy(m_bLastOutCard,m_bOutCard,sizeof(m_bOutCard));
			memcpy(m_bLastOutCardCnt,m_bOutCardCnt,sizeof(m_bOutCardCnt));
			memset(m_bOutCard,0,sizeof(m_bOutCard));
			memset(m_bOutCardCnt,0,sizeof(m_bOutCardCnt));

			//�����˿�
			for(int i=0;i<GAME_PLAYER;i++)
			{
				m_GameClientView.m_OutCard[i].SetCard(NULL,NULL,0);
			}
			if(m_wCurSeat ==GetMeChairID()&&!IsLookonMode())
			{
				ActiveGameFrame();
				SetOutCardButton();
			}
			KillTimer(TIME_ID_WASH_DESK);
			SetGameTimer(m_wCurSeat,TIME_ID_OUT_CARD,TIME_CNT_OUT_CARD);
			break;
		}
	case TIME_ID_LAST_AROUND:
		{
			RevertDeskCard();
			break;
		}
	case TIME_ID_LOOK_PUBLIC:
		{
			KillTimer(TIME_ID_LOOK_PUBLIC);
			if(GetMeChairID() == m_wCurSeat && !IsLookonMode())
			{
				ActiveGameFrame();
				SetOutCardButton();
			}

			SetGameTimer(m_wCurSeat,TIME_ID_OUT_CARD,TIME_CNT_OUT_CARD);
			m_GameClientView.UpdateGameView(NULL);

			break;
		}
	}
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CGameFrameDlg::OnTimer(nIDEvent);
}


void CGameClientDlg::SetSpaceCard()
{
	if(IsLookonMode()) goto  flag;
	BYTE bCards[5]={0};
	BYTE bCardCnt=0;
	for(BYTE i=1;i<m_bMeCardCnt;++i)
	{
		if(m_Logic.GetCardHuaKind(m_bMeCard[i],FALSE)!=m_Logic.GetCardHuaKind(m_bMeCard[i-1],FALSE))
		{
			bCards[bCardCnt++]=i;
		}
	}
	m_GameClientView.m_MeCard.SetAddParameter(bCards,bCardCnt);
flag:
	m_GameClientView.m_MeCard.SetCard(m_bMeCard,NULL,m_bMeCardCnt);
}

