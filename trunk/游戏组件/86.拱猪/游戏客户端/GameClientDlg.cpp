#include "Stdafx.h"
#include "GameClient.h"
#include "GameOption.h"
#include "GameClientDlg.h"
//////////////////////////////////////////////////////////////////////////
//�궨��

//��Ϸ��ʱ��
#define IDI_OUT_CARD					200								//���ƶ�ʱ��
#define IDI_START_GAME					201								//��ʼ��ʱ��
#define IDI_WAIT_START					202								//���ֶ�ʱ��
#define IDI_SHOW_CARD                   203                             //���ƶ�ʱ��
#define IDI_LAST_CARD_STOP              204                             //ͣ����ʱ��

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientDlg, CGameFrameDlg)
	ON_MESSAGE(IDM_START,OnStart)
	ON_MESSAGE(IDM_OUT_CARD,OnOutCard)
	ON_MESSAGE(IDM_LEFT_HIT_CARD,OnLeftHitCard)
	ON_MESSAGE(IDM_RIGHT_HIT_CARD,OnRightHitCard)
	ON_MESSAGE(IDM_SHOW_CARD,OnShowCard)	
	ON_MESSAGE(IDM_NOT_SHOW_CARD,OnNotShowCard)
    ON_MESSAGE(IDM_AUTO_OUT_CARD,OnAutoOutCard)
	ON_MESSAGE(IDM_CANCEL_AUTO_OUT_CARD,OnCancelAutoOutCard)
	ON_WM_TIMER()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientDlg::CGameClientDlg() : CGameFrameDlg(&m_GameClientView)
{

	//��Ϸ����
	ZeroMemory(m_bCardCount , sizeof(m_bCardCount)) ;
	m_bHandCardCount               = 0;	
	ZeroMemory(m_bHandCardData , sizeof(m_bHandCardData)) ;
	m_bFirstGame = true ;
	m_bAutoOutCard = false ;
	
	//���Ʊ���
	memset(m_bTurnCardData , 0 , sizeof(m_bTurnCardData)) ;					//�����б�
	m_bTurnCardCount = 0 ;
	m_wLastOutUser   = INVALID_CHAIR ;
	m_wFirstOutUser  = INVALID_CHAIR ;
	m_bFirstOutSpade = true;
	m_bFirstHearts   = true ;  
	m_bFirstClubs    = true ;   
	m_bFirstDiamonds = true ;
	m_bFirstOutCard  = false ;


	//���ñ���
	m_bDeasilOrder                 = false ;			        			//����˳��
	m_dwCardHSpace                 = DEFAULT_PELS ;	    					//�˿�����
	m_bTimerElapse                 = 30 ;
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
	SetWindowText(TEXT("������Ϸ"));

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


	return true;
}

//���ÿ��
void CGameClientDlg::ResetGameFrame()
{
	//��Ϸ����
	m_bHandCardCount=0;
	memset(m_bCardCount,0,sizeof(m_bCardCount));
	memset(m_bHandCardData,0,sizeof(m_bHandCardData));
	m_bFirstGame = true ;
	m_bAutoOutCard = false ;

	//���Ʊ���
	memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));
	m_bTurnCardCount = 0;
	m_wLastOutUser   = INVALID_CHAIR ;
	m_wFirstOutUser  = INVALID_CHAIR ;
	m_bFirstOutSpade = true;
	m_bFirstHearts   = true ;  
	m_bFirstClubs    = true ;   
	m_bFirstDiamonds = true ;
	m_bFirstOutCard  = false ;

	//���ñ���
	m_bDeasilOrder                 = false ;			        			//����˳��
	m_dwCardHSpace                 = DEFAULT_PELS ;	    					//�˿�����
	m_bTimerElapse                 = 30 ;

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
	case IDI_SHOW_CARD:        //�������
		{
			if (nElapse==0)
			{
				if ((IsLookonMode()==false)&&(wChairID==GetMeChairID())) OnNotShowCard(0,0);
				 m_GameClientView.m_HandCardControl.ShootAllCard(false) ;
				return false;
			}
			if ((nElapse<=10)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false)) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));

		}
	case IDI_LAST_CARD_STOP:   //�˿�ͣ��
		{
			if(nElapse==0)
			{
				//�������
				for (WORD i=0;i<GAME_PLAYER;i++) 
				{	
					m_GameClientView.m_UserCardControl[i].SetCardData(NULL,0);
					memset(m_bTurnCardData , 0 , sizeof(m_bTurnCardData)) ;
				}	
			}
			return true ;
		}
	case IDI_WAIT_START:
		{
			if (nElapse==0)
			{
				if (IsLookonMode()==false) this->PostMessage(WM_CLOSE,0,0);
				return false;
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
	case SUB_S_SEND_CARD:		//�����˿�
		{
			return OnSubSendCard(pBuffer,wDataSize);
		}
	case SUB_S_GAME_START:		//��Ϸ��ʼ
		{
			return OnSubGameStart(pBuffer,wDataSize);
		}
	case SUB_S_OUT_CARD:		//�û�����
		{		
			return OnSubOutCard(pBuffer,wDataSize);
		}
	case SUB_S_SHOW_CARD:      //�������
		{
			return OnSubShowCard(pBuffer , wDataSize) ;
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
	case GS_WK_FREE:	//����״̬
		{
			//���ý���
		
			//���ÿؼ�
			if (IsLookonMode()==false)
			{
				
				if(FALSE==m_GameClientView.m_btStart.IsWindowVisible())
				{
					m_GameClientView.m_btStart.ShowWindow(TRUE);
					m_GameClientView.m_btStart.SetFocus();
				}
			}
			//�����˿�
			if (IsLookonMode()==false) m_GameClientView.m_HandCardControl.SetDisplayFlag(true);

			return true;
		}
	case GS_WK_SHOWCARD:	//�������
		{
			//Ч������
			if(wDataSize!=sizeof(CMD_S_StatusShowCard)) return false ;
			CMD_S_StatusShowCard *pSatusShowCard = (CMD_S_StatusShowCard*)pBuffer ;

			
			//���ñ���
			m_bHandCardCount=HAND_CARD_COUNT;
			WORD wViewChairID=SwitchViewChairID(pSatusShowCard->wCurrentUser);
			for (WORD i=0;i<GAME_PLAYER;i++) m_bCardCount[i]=HAND_CARD_COUNT;
			CopyMemory(m_bHandCardData,pSatusShowCard->bCardData,m_bHandCardCount);
			m_GameClientView.SetCardCount(wViewChairID , m_bHandCardCount) ;
			m_GameClientView.m_HandCardControl.SetCardData(m_bHandCardData,m_bHandCardCount); 

			if (IsLookonMode()==false) 
			{
				m_GameClientView.m_HandCardControl.SetPositively(true);  
				m_GameClientView.m_HandCardControl.SetDisplayFlag(true);
			}

			for(BYTE i=0 ; i<GAME_PLAYER ; ++i)
			{
				m_GameClientView.SetCardCount(i , HAND_CARD_COUNT) ;
				wViewChairID = SwitchViewChairID(i) ;
				if(0!=pSatusShowCard->ShowCardCount[i])	m_GameClientView.SetShowCard(wViewChairID , pSatusShowCard->ShowCard[i] , pSatusShowCard->ShowCardCount[i]) ;
			}
	
					//��ť����
			if ((IsLookonMode()==false)&&(pSatusShowCard->wCurrentUser==GetMeChairID()) && 0==pSatusShowCard->ShowCardCount[pSatusShowCard->wCurrentUser])
			{
				//�����˿�
				m_GameClientView.m_HandCardControl.SetPositively(true) ;

				bool bCanShowCard=false ;
				BYTE bCardColor ;
				BYTE bCardValue ;
				BYTE bCardDataIndex[4] ;
				BYTE dwCardCount=0 ; 
				for(BYTE i=0 ; i<HAND_CARD_COUNT ; ++i)
				{
					bCardColor = m_GameLogic.GetCardColor(m_bHandCardData[i]) ;
					bCardColor = bCardColor>>4 ;
					bCardValue = m_GameLogic.GetCardValue(m_bHandCardData[i]) ;
					//�ж��˿�
					if((3==bCardColor && 12==bCardValue) || //����Q
						(2==bCardColor && 1==bCardValue)  || //����A
						(0==bCardColor && 11==bCardValue) || //����J
						(1==bCardColor && 10==bCardValue))   //÷��10
					{
						m_GameClientView.m_btShowCard.ShowWindow(SW_SHOW);
						m_GameClientView.m_btShowCard.EnableWindow(TRUE);

						m_GameClientView.m_btNotShowCard.ShowWindow(SW_SHOW);
						m_GameClientView.m_btNotShowCard.EnableWindow(TRUE);

						m_GameClientView.m_bShowCardHint = true ;

						//����ʱ��
						SetGameTimer(pSatusShowCard->wCurrentUser,IDI_SHOW_CARD,m_bTimerElapse);

						bCanShowCard = true ;

						//��������
						bCardDataIndex[dwCardCount++] = i ;
					}
				}

				//��������
				if(false==bCanShowCard)
				{
					CMD_C_ShowCard  ShowCard ;
					memset(&ShowCard , 0 , sizeof(ShowCard)) ;

					SendData(SUB_C_SHOW_CART , &ShowCard , sizeof(ShowCard)) ;
				}
				else    //�������� 
					m_GameClientView.m_HandCardControl.SetShootCard(bCardDataIndex , dwCardCount) ;
			}

			return true ;
		}
	case GS_WK_PLAYING:		//��Ϸ״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pBuffer;
		
			//���ñ���
			m_bHandCardCount=pStatusPlay->bCardCount[pStatusPlay->wCurrentUser];

			CopyMemory(m_bHandCardData,pStatusPlay->bCardData,m_bHandCardCount);			
			//���ý���
	
			for(BYTE i=0 ; i<GAME_PLAYER ; ++i)
			{
				WORD wViewChairID = SwitchViewChairID(i);
				m_GameClientView.SetCardCount(wViewChairID , pStatusPlay->bCardCount[i]) ;
				m_bCardCount[i]= pStatusPlay->bCardCount[i] ;
			}
			if(0!=m_bHandCardCount)	
				m_GameClientView.m_HandCardControl.SetCardData(m_bHandCardData,m_bHandCardCount); 
			m_wLastOutUser = pStatusPlay->wLastOutUser ;
			m_wFirstOutUser= pStatusPlay->wFirstOutUser ;

			for(BYTE i=0 ; i<GAME_PLAYER ; ++i)
			{
				WORD wViewChairID = SwitchViewChairID(i);
				if(0!=pStatusPlay->bScoreCardCount[i])  m_GameClientView.AddScoreCard(wViewChairID , pStatusPlay->ScoreCard[i] , pStatusPlay->bScoreCardCount[i]) ;
				if(0!=pStatusPlay->bShowCardCount[i])   m_GameClientView.SetShowCard(wViewChairID , pStatusPlay->ShowCard[i] , pStatusPlay->bShowCardCount[i]) ;
			}
			CopyMemory(m_bTurnCardData , pStatusPlay->bTurnCardData , sizeof(pStatusPlay->bTurnCardData)) ;
			    
			
			//�������
			if ((IsLookonMode()==false)&&(pStatusPlay->wCurrentUser==GetMeChairID()))
			{
				m_GameClientView.m_btOutCard.EnableWindow(FALSE);  
				m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
			}

			//���ö�ʱ��
			SetGameTimer(pStatusPlay->wCurrentUser,IDI_OUT_CARD,m_bTimerElapse);

			//�����˿�
			if (IsLookonMode()==false) 
			{
				m_GameClientView.m_HandCardControl.SetPositively(true);  
				m_GameClientView.m_HandCardControl.SetDisplayFlag(true);
			}

			if(false==IsLookonMode())
			{
				//���ý���
				m_GameClientView.m_btAutoOutCard.ShowWindow(SW_SHOW) ;
				m_GameClientView.m_btAutoOutCard.EnableWindow(TRUE) ;
			}
			return true;
		}
	}	

	return true;
}

//�����˿�
bool CGameClientDlg::OnSubSendCard(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_SendCard));
	if (wDataSize!=sizeof(CMD_S_SendCard)) return false;

	//����״̬
	SetGameStatus(GS_WK_SHOWCARD);

	//��������
	CMD_S_SendCard * pSendCard=(CMD_S_SendCard *)pBuffer;
	//��������
	m_bHandCardCount=CountArray(pSendCard->bCardData);  
	CopyMemory(m_bHandCardData,pSendCard->bCardData,sizeof(pSendCard->bCardData));

	//���ý���
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.SetCardCount(i,HAND_CARD_COUNT);
		m_bCardCount[i] = HAND_CARD_COUNT ;
		m_GameClientView.m_UserCardControl[i].SetCardData(NULL,0);
	}
	
	//�����˿�
	m_GameClientView.m_HandCardControl.SetCardData(pSendCard->bCardData,HAND_CARD_COUNT);

	if (IsLookonMode()==true) 
	{
		m_GameClientView.m_ScoreView.ShowWindow(SW_HIDE);
		m_GameClientView.m_HandCardControl.SetDisplayFlag(false);
	}

	//���ð�ť
	if ((IsLookonMode()==false)&&(pSendCard->wCurrentUser==GetMeChairID()))
	{
		//�����˿�
		m_GameClientView.m_HandCardControl.SetPositively(true) ;

		bool bCanShowCard=false ;
		BYTE bCardColor ;
		BYTE bCardValue ;
		BYTE bCardDataIndex[4] ;
		BYTE dwCardCount=0 ; 
		for(BYTE i=0 ; i<HAND_CARD_COUNT ; ++i)
		{
            bCardColor = m_GameLogic.GetCardColor(pSendCard->bCardData[i]) ;
			bCardColor = bCardColor>>4 ;
			bCardValue = m_GameLogic.GetCardValue(pSendCard->bCardData[i]) ;
			//�ж��˿�
			if((3==bCardColor && 12==bCardValue) || //����Q
			   (2==bCardColor && 1==bCardValue)  || //����A
			   (0==bCardColor && 11==bCardValue) || //����J
			   (1==bCardColor && 10==bCardValue))   //÷��10
			{
               m_GameClientView.m_btShowCard.ShowWindow(SW_SHOW);
			   m_GameClientView.m_btShowCard.EnableWindow(TRUE);

			   m_GameClientView.m_btNotShowCard.ShowWindow(SW_SHOW);
               m_GameClientView.m_btNotShowCard.EnableWindow(TRUE);

			   m_GameClientView.m_bShowCardHint = true ;

			   //����ʱ��
			   SetGameTimer(pSendCard->wCurrentUser,IDI_SHOW_CARD,m_bTimerElapse);

			   bCanShowCard = true ;

			   //��������
			   bCardDataIndex[dwCardCount++] = i ;
			}
		}

		//��������
		if(false==bCanShowCard)
		{
			CMD_C_ShowCard  ShowCard ;
			memset(&ShowCard , 0 , sizeof(ShowCard)) ;

			SendData(SUB_C_SHOW_CART , &ShowCard , sizeof(ShowCard)) ;
		}
		else    //�������� 
			m_GameClientView.m_HandCardControl.SetShootCard(bCardDataIndex , dwCardCount) ;
	}

	//��������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));

	return true;
}
bool CGameClientDlg::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	//��Ϸ����
	memset(m_bCardCount , HAND_CARD_COUNT , GAME_PLAYER) ;       	    	
	m_bHandCardCount               = HAND_CARD_COUNT;					                    

	//���Ʊ���
	memset(m_bTurnCardData , 0 , sizeof(m_bTurnCardData)) ;					
	m_bTurnCardCount = 0 ;
	m_wLastOutUser   = INVALID_CHAIR ;
	m_wFirstOutUser  = INVALID_CHAIR ;

	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;
	SetGameStatus(GS_WK_PLAYING);
	//��Ϣ����
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;
	m_bFirstGame   = pGameStart->bFirstGame ;

	//�������
	if (IsLookonMode()==false)
	{
		m_GameClientView.m_HandCardControl.SetPositively(true);
	}

	//�������
	if(false==IsLookonMode() && pGameStart->wCurrentUser==GetMeChairID())
	{
		ActiveGameFrame();
		m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btOutCard.EnableWindow(FALSE);
	}

	//��������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));

	//���ö�ʱ��
	SetGameTimer(pGameStart->wCurrentUser,IDI_OUT_CARD,m_bTimerElapse);

	if(false==IsLookonMode())
	{
		//���ý���
		m_GameClientView.m_btAutoOutCard.ShowWindow(SW_SHOW) ;
		m_GameClientView.m_btAutoOutCard.EnableWindow(TRUE) ;
	}
	return true;
}

//�û�����
bool CGameClientDlg::OnSubShowCard(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_ShowCard));
	if (wDataSize!=sizeof(CMD_S_ShowCard)) return false;

	CMD_S_ShowCard *pShowCard = (CMD_S_ShowCard*)pBuffer ;

	WORD wChairID = SwitchViewChairID(pShowCard->wShowCardUser) ;

	//����У��
	ASSERT(wChairID<GAME_PLAYER) ;
	if(wChairID>=GAME_PLAYER) return false ;

	BYTE bCardColor ;
	BYTE bCardValue ;
	for(BYTE i=0 ; i<pShowCard->bShowCardCount ; ++i)
	{
		bCardColor = m_GameLogic.GetCardColor(pShowCard->bShowCard[i]) ;
		bCardColor = bCardColor>>4 ;
		bCardValue = m_GameLogic.GetCardValue(pShowCard->bShowCard[i]) ;

		//����У��
		ASSERT(0!=pShowCard->bShowCard[i]) ;
		if(0==pShowCard->bShowCard[i]) return false ;
		ASSERT((3==bCardColor && 12==bCardValue) || //����Q
			   (2==bCardColor && 1==bCardValue)  || //����A
			   (0==bCardColor && 11==bCardValue) || //����J
			   (1==bCardColor && 10==bCardValue));  //÷��10

	}
	//��������
	if(0!=pShowCard->bShowCardCount) m_GameClientView.SetShowCard(wChairID , pShowCard->bShowCard , pShowCard->bShowCardCount) ;

	return true ;
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
	ASSERT(wDataSize==(wHeadSize+pOutCard->bCardCount*sizeof(pOutCard->bCardData)));
	if (wDataSize!=(wHeadSize+pOutCard->bCardCount*sizeof(pOutCard->bCardData))) return false;

	//ɾ����ʱ��
	KillGameTimer(IDI_OUT_CARD);
	KillGameTimer(IDI_LAST_CARD_STOP) ;

	//�������
	if(0==m_bTurnCardCount)
	{
		m_wFirstOutUser = pOutCard->wOutCardUser ;

		for (WORD i=0;i<GAME_PLAYER;i++) 
		{	
			m_GameClientView.m_UserCardControl[i].SetCardData(NULL,0);
			memset(m_bTurnCardData , 0 , sizeof(m_bTurnCardData)) ;
		}	
		if(GetMeChairID()==pOutCard->wOutCardUser) m_GameClientView.m_UserCardControl[2].SetCardData(&pOutCard->bCardData,pOutCard->bCardCount);
	}

	//��������
	WORD wOutViewChairID=SwitchViewChairID(pOutCard->wOutCardUser);

	m_bCardCount[pOutCard->wOutCardUser] -= pOutCard->bCardCount;
	m_GameClientView.SetCardCount(wOutViewChairID,m_bCardCount[pOutCard->wOutCardUser]);

	//��������
	if ((IsLookonMode()==true)||(GetMeChairID()!=pOutCard->wOutCardUser))
	{
		m_GameClientView.m_UserCardControl[wOutViewChairID].SetCardData(&pOutCard->bCardData,pOutCard->bCardCount);
	}

	//�������
	CopyMemory(&m_bTurnCardData[pOutCard->wOutCardUser],&pOutCard->bCardData,sizeof(BYTE)*pOutCard->bCardCount);
	m_wLastOutUser = pOutCard->wOutCardUser ;
	m_bTurnCardCount++ ;

	//���Ʊ���
	if(GAME_PLAYER==m_bTurnCardCount)
	{	//������
		WORD wMostUser = m_wFirstOutUser ;
		BYTE bFirstCardColor = m_GameLogic.GetCardColor(m_bTurnCardData[m_wFirstOutUser]) ;
		bFirstCardColor = bFirstCardColor >> 4 ;
		BYTE bMostCardValue  = m_GameLogic.GetCardValue(m_bTurnCardData[m_wFirstOutUser]) ;

		if(1==bMostCardValue) bMostCardValue += 13 ;
        //�÷����
		for(WORD i=0 ; i<GAME_PLAYER ; ++i)
		{
			BYTE bCurrentCarColor  = m_GameLogic.GetCardColor(m_bTurnCardData[i]) ;
			bCurrentCarColor = bCurrentCarColor >> 4 ;
			BYTE bCurrentCardValue = m_GameLogic.GetCardValue(m_bTurnCardData[i]) ;
			if(1==bCurrentCardValue) bCurrentCardValue += 13 ;
			if(bCurrentCarColor == bFirstCardColor)
				if(bMostCardValue < bCurrentCardValue)
				{
					bMostCardValue = bCurrentCardValue ;
					wMostUser = i ;
				}
		}

		//��ȡ����
		BYTE bScoreCard[GAME_PLAYER] ;
		BYTE bScoreCount=0 ;
		if(true==m_GameLogic.GetScoreCard(m_bTurnCardData , bScoreCard , GAME_PLAYER , bScoreCount))
		{
			m_GameClientView.AddScoreCard(SwitchViewChairID(wMostUser) , bScoreCard , bScoreCount) ;
		}

		//�����б�		
		memset(m_bTurnCardData , 0 , sizeof(m_bTurnCardData)) ;					
		m_bTurnCardCount = 0 ;
		m_wLastOutUser   = INVALID_CHAIR ;
		m_wFirstOutUser  = INVALID_CHAIR ;

		SetGameTimer(pOutCard->wCurrentUser,IDI_LAST_CARD_STOP,2);
	}
	    
	//�Լ��˿�
	if ((IsLookonMode()==true)&&(pOutCard->wOutCardUser==GetMeChairID()))
	{
		//ɾ���˿� 
		BYTE bSourceCount=m_bHandCardCount;
		m_bHandCardCount-=pOutCard->bCardCount;
		m_GameLogic.RemoveCard(&pOutCard->bCardData,pOutCard->bCardCount,m_bHandCardData,bSourceCount);
		
		//���ý���
		m_GameClientView.m_HandCardControl.SetCardData(m_bHandCardData,m_bHandCardCount);
	}

	//��������

	BYTE bCardColor  = m_GameLogic.GetCardColor(pOutCard->bCardData) ;
	bCardColor = bCardColor >> 4 ;
	BYTE bCardValue = m_GameLogic.GetCardValue(pOutCard->bCardData) ;
	if(3==bCardColor && 12==bCardValue) PlayGameSound(AfxGetInstanceHandle(),TEXT("PIG_CARD"));
	else if (0==bCardColor && 11==bCardValue) PlayGameSound(AfxGetInstanceHandle(),TEXT("SHEEP_CARD"));
	else PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));
	
	//�������
	if (pOutCard->wCurrentUser==GetMeChairID()&&IsLookonMode()==false)
	{		
		ActiveGameFrame();
		m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btOutCard.EnableWindow(FALSE);
		m_GameClientView.m_HandCardControl.ShootAllCard(false) ;

		//���ö�ʱ��
		SetGameTimer(pOutCard->wCurrentUser,IDI_OUT_CARD,m_bTimerElapse);
	}

	return true;
}
//��Ϸ����
bool CGameClientDlg::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
//	InsertSystemString("CGameClientDlg::OnSubGameEnd");
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;

	//��Ϣ����
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;

	//ɾ����ʱ��
	KillGameTimer(IDI_OUT_CARD);
	KillGameTimer(IDI_SHOW_CARD);

	//���ؿؼ�
	m_GameClientView.m_btOutCard.ShowWindow(SW_HIDE);
	//���ÿؼ�
	m_GameClientView.m_btOutCard.EnableWindow(FALSE);
	//���ý���
	m_GameClientView.m_btAutoOutCard.ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btAutoOutCard.EnableWindow(FALSE) ;
	m_GameClientView.m_btCancelAutoOutCard.ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btCancelAutoOutCard.EnableWindow(FALSE) ;
	SetGameStatus(GS_WK_FREE);

	//�����˿�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.m_UserCardControl[i].SetCardData(NULL,0);
	}

	//��ʾ�˿�
	if (IsLookonMode()==true) m_GameClientView.m_HandCardControl.SetDisplayFlag(true);

	//���û���
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		const tagUserData * pUserData=GetUserData(i);
		m_GameClientView.m_ScoreView.SetGameScore(i,pUserData->szName,pGameEnd->lGameScore[i]);
	}
	m_GameClientView.m_ScoreView.SetGameTax(pGameEnd->lGameTax);
	m_GameClientView.m_ScoreView.ShowWindow(SW_SHOW);

	//���ý���
	if (IsLookonMode()==false)
	{
		if(FALSE==m_GameClientView.m_btStart.IsWindowVisible())
		{
			m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
	    	m_GameClientView.m_btStart.UpdateWindow();
		}
		m_GameClientView.m_bSendWaiMsg=true;
		//SetGameTimer(GetMeChairID(),IDI_START_GAME,90);
	}

	//���ñ���
	m_GameClientView.AddScoreCard(INVALID_CHAIR , NULL , 0) ;
	m_GameClientView.SetShowCard(INVALID_CHAIR , NULL , 0) ;

	return true;
}
//�����ж�
bool CGameClientDlg::VerdictOutCard(bool bAutoOutCard)
{
	//״̬�ж�
	if (m_GameClientView.m_btOutCard.IsWindowVisible()==FALSE) return false;

	//��ȡ�˿�
	BYTE bCardData[HAND_CARD_COUNT];
	BYTE bShootCount=(BYTE)m_GameClientView.m_HandCardControl.GetShootCard(bCardData,HAND_CARD_COUNT);

	//�����ж�
	if (bShootCount==1)
	{
		if(true==m_bFirstGame)
		{
			//�����ж�
			bool bFisrCard=true ;
			for(WORD i=0 ; i<GAME_PLAYER ; ++i)
				if(HAND_CARD_COUNT!=m_bCardCount[i]) bFisrCard = false ;

			if(true==bFisrCard)
			{
				BYTE bCardColor = m_GameLogic.GetCardColor(bCardData[0]) ;
				bCardColor = bCardColor>>4 ;
				BYTE bCardValue = m_GameLogic.GetCardValue(bCardData[0]) ;
				if(1==bCardColor && 2==bCardValue) 
				{
					m_bFirstClubs = false ;
					return true ;
				}
				if(false==bAutoOutCard)	MessageBox("��һ�ֵĵ�һ���Ʊ����ȳ�÷��2��" , "������ʾ" , MB_OK|MB_ICONINFORMATION) ;
				m_GameClientView.m_HandCardControl.ShootAllCard(false) ;
				return false ;
			}
		}

		if(INVALID_CHAIR==m_wFirstOutUser) 
		{
			BYTE bCardColor = m_GameLogic.GetCardColor(bCardData[0]) ;
			bCardColor      = bCardColor>>4 ; 
			return VerdictFirstGameColor(bCardColor , bCardData[0] , true) ;
		}
		BYTE bFirstOutColor = m_GameLogic.GetCardColor(m_bTurnCardData[m_wFirstOutUser]) ;
		bFirstOutColor = bFirstOutColor>>4 ;
		BYTE bCurrentColor = m_GameLogic.GetCardColor(bCardData[0]) ;
		bCurrentColor = bCurrentColor>>4 ;

		//�Ƿ����
		if(bFirstOutColor!=bCurrentColor) 
		{
			for(BYTE i=0 ; i<m_bHandCardCount ; ++i)
			{
				bCurrentColor = m_GameLogic.GetCardColor(m_bHandCardData[i]) ;
				bCurrentColor = bCurrentColor >> 4 ;
				if(bFirstOutColor==bCurrentColor)
				{
					if(false==bAutoOutCard)	MessageBox("��������͵�һ������ͬ��ɫ���ƣ�" , "������ʾ" , MB_OK|MB_ICONINFORMATION) ;
					m_GameClientView.m_HandCardControl.ShootAllCard(false) ;
					return false ;
				}
			}
			return true ;
		}
		else
			return VerdictFirstGameColor(bFirstOutColor , bCardData[0] ,true) ;
		
		
	}

	return false;
}
//��һ�ֻ�ɫ
bool  CGameClientDlg::VerdictFirstGameColor(BYTE bFirstOutColor , BYTE bCardData , bool bAutoOutCard)
{
	BYTE bCurrentColor = m_GameLogic.GetCardColor(bCardData) ;
	bCurrentColor = bCurrentColor>>4 ;

	//��һ�ֻ�ɫ
	switch(bFirstOutColor)
	{
	case 0:      //����
		{
			//�ָ�����
			if(true==m_bFirstOutCard) m_bFirstDiamonds = true ;

			//��һ���ж�
			if(true==m_bFirstDiamonds)
			{
				m_bFirstDiamonds = false ;
				bool bIsShowCard = false ;

				//�Ƿ�����
				for(BYTE i=0 ; i<m_GameClientView.m_bShowCardCount[2] ; ++i)
					if(bCardData==m_GameClientView.m_bShowCard[2][i]) 
					{
						bIsShowCard = true ;
						break;
					}

					if(true==bIsShowCard)
					{
						for(BYTE i=0 ; i<m_bHandCardCount ; ++i)
						{
							if(bCardData==m_bHandCardData[i]) continue ;
							BYTE bCardColor = m_GameLogic.GetCardColor(m_bHandCardData[i]) ;
							bCardColor = bCardColor>>4 ;
							if(bCardColor==bCurrentColor) 
							{
								if(false==bAutoOutCard) MessageBox("�������Ʋ����ڸû�ɫ���ֳ���ʱ����" , "������ʾ" , MB_OK|MB_ICONINFORMATION) ;
								m_GameClientView.m_HandCardControl.ShootAllCard(false) ;
								//�ָ�����
								m_bFirstDiamonds = true ;
								return false ;
							}
						}

						return true ;
					}
					else
					{
						//�ָ�����
						m_bFirstOutCard = true ;
						return true ;
					}
			}		
			else return true ;

			return true ;
		}
	case 1:     //÷��
		{
			//�ָ�����
			if(true==m_bFirstOutCard) m_bFirstClubs = true ;

			//��һ���ж�
			if(true==m_bFirstClubs)
			{
				m_bFirstClubs = false ;
				bool bIsShowCard = false ;

				//�Ƿ�����
				for(BYTE i=0 ; i<m_GameClientView.m_bShowCardCount[2] ; ++i)
					if(bCardData==m_GameClientView.m_bShowCard[2][i]) 
					{
						bIsShowCard = true ;
						break;
					}

					if(true==bIsShowCard)
					{
						for(BYTE i=0 ; i<m_bHandCardCount ; ++i)
						{
							if(bCardData==m_bHandCardData[i]) continue ;
							BYTE bCardColor = m_GameLogic.GetCardColor(m_bHandCardData[i]) ;
							bCardColor = bCardColor>>4 ;
							if(bCardColor==bCurrentColor) 
							{
								if(false==bAutoOutCard) MessageBox("�������Ʋ����ڸû�ɫ���ֳ���ʱ����" , "������ʾ" , MB_OK|MB_ICONINFORMATION) ;
								m_GameClientView.m_HandCardControl.ShootAllCard(false) ;
								//�ָ�����
								m_bFirstClubs = true ;
								return false ;
							}
						}

						return true ;
					}
					else
					{
						//�ָ�����
						m_bFirstOutCard = true ;
						return true ;
					}
			}		
			else return true ;

			return true ;
		}
	case 2:     //����
		{
			//�ָ�����
			if(true==m_bFirstOutCard) m_bFirstHearts = true ;

			if(true==m_bFirstHearts)
			{
				m_bFirstHearts = false ;
				bool bIsShowCard = false ;

				//�Ƿ�����
				for(BYTE i=0 ; i<m_GameClientView.m_bShowCardCount[2] ; ++i)
					if(bCardData==m_GameClientView.m_bShowCard[2][i]) 
					{
						bIsShowCard = true ;
						break;
					}

					if(true==bIsShowCard)
					{
						for(BYTE i=0 ; i<m_bHandCardCount ; ++i)
						{
							if(bCardData==m_bHandCardData[i]) continue ;
							BYTE bCardColor = m_GameLogic.GetCardColor(m_bHandCardData[i]) ;
							bCardColor = bCardColor>>4 ;
							if(bCardColor==bCurrentColor) 
							{
								if(false==bAutoOutCard) MessageBox("�������Ʋ����ڸû�ɫ���ֳ���ʱ����" , "������ʾ" , MB_OK|MB_ICONINFORMATION) ;
								m_GameClientView.m_HandCardControl.ShootAllCard(false) ;
								//�ָ�����
								m_bFirstHearts = true ;
								return false ;
							}
						}
						return true ;
					}
					else
					{
						//�ָ�����
						m_bFirstOutCard = true ;
						return true ;
					}
			}		
			else return true ;

			return true ;
		}
	case 3:     //����
		{
			//�ָ�����
			if(true==m_bFirstOutCard) m_bFirstHearts = true ;

			if(true==m_bFirstOutSpade)
			{
				m_bFirstOutSpade = false ;
				bool bIsShowCard = false ;

				//�Ƿ�����
				for(BYTE i=0 ; i<m_GameClientView.m_bShowCardCount[2] ; ++i)
					if(bCardData==m_GameClientView.m_bShowCard[2][i]) 
					{
						bIsShowCard = true ;
						break;
					}

					if(true==bIsShowCard)
					{
						for(BYTE i=0 ; i<m_bHandCardCount ; ++i)
						{
							if(bCardData==m_bHandCardData[i]) continue ;
							BYTE bCardColor = m_GameLogic.GetCardColor(m_bHandCardData[i]) ;
							bCardColor = bCardColor>>4 ;
							if(bCardColor==bCurrentColor) 
							{
								if(false==bAutoOutCard) MessageBox("�������Ʋ����ڸû�ɫ���ֳ���ʱ����" , "������ʾ" , MB_OK|MB_ICONINFORMATION) ;
								m_GameClientView.m_HandCardControl.ShootAllCard(false) ;
								//�ָ�����
								m_bFirstOutSpade = true ;
								return false ;
							}
						}
						return true ;
					}
					else 
					{
						//�ָ�����
						m_bFirstOutCard = true ;
						return true ;
					}
			}		
			else return true ;

			return true ;
		}
	default:    //����
		return false ;
	}
	return true ;
}

//�����ж�
bool  CGameClientDlg::VerdictShowCard()
{
	//״̬�ж�
	if (m_GameClientView.m_btShowCard.IsWindowVisible()==FALSE) return false;

	//��ȡ�˿�
	BYTE bCardData[HAND_CARD_COUNT];
	BYTE bShootCount=(BYTE)m_GameClientView.m_HandCardControl.GetShootCard(bCardData,HAND_CARD_COUNT);

	//�ж�����
	if(bShootCount>=1 && bShootCount<=4)
	{
		BYTE bCardColor ;
		BYTE bCardValue ;

		BYTE bShowCardCount=0;
		for(BYTE i=0 ; i<bShootCount ; ++i)
		{
			bCardColor = m_GameLogic.GetCardColor(bCardData[i]) ;
			bCardColor = bCardColor>>4 ;
			bCardValue = m_GameLogic.GetCardValue(bCardData[i]) ;
			//�ж��˿�
			if((3==bCardColor && 12==bCardValue)  || //����Q
				(2==bCardColor && 1==bCardValue)  || //����A
				(0==bCardColor && 11==bCardValue) || //����J
				(1==bCardColor && 10==bCardValue))   //÷��10
			{
				++bShowCardCount;
			}
		}
		return bShootCount==bShowCardCount;
	}
	return false ;
}

//��ʱ����Ϣ
void CGameClientDlg::OnTimer(UINT nIDEvent)
{


	__super::OnTimer(nIDEvent);
}
//��ʼ��ť
LRESULT CGameClientDlg::OnStart(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam) ;
	UNREFERENCED_PARAMETER(lParam) ;

	//���ñ���
	m_bHandCardCount=0;
	m_bTurnCardCount=0;
	memset(m_bHandCardData,0,sizeof(m_bHandCardData));
	memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));
	m_bAutoOutCard = false ;

	//���Ʊ���
	m_wFirstOutUser  = INVALID_CHAIR ;
	m_wLastOutUser   = INVALID_CHAIR ;
	m_bFirstOutSpade = true;
	m_bFirstHearts   = true ;  
	m_bFirstClubs    = true ;   
	m_bFirstDiamonds = true ;
	m_bFirstOutCard  = false ;

	//���ñ���
	m_bDeasilOrder  = false ;			        			
	m_dwCardHSpace  = DEFAULT_PELS ;	    				
	m_bTimerElapse  = 30 ;

	//���ý���
	KillGameTimer(IDI_START_GAME);
	KillGameTimer(IDI_WAIT_START);
	m_GameClientView.SetCardCount(INVALID_CHAIR,0);

	//���ؿؼ�
	m_GameClientView.m_btStart.ShowWindow(FALSE);
	m_GameClientView.m_ScoreView.ShowWindow(SW_HIDE);
	m_GameClientView.m_btAutoOutCard.ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btAutoOutCard.EnableWindow(FALSE) ;
	m_GameClientView.m_btCancelAutoOutCard.ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btCancelAutoOutCard.EnableWindow(FALSE) ;

	//�����˿�
	m_GameClientView.m_HandCardControl.SetCardData(NULL,0);
	m_GameClientView.m_HandCardControl.SetPositively(false);
	//���ñ���
	m_GameClientView.AddScoreCard(INVALID_CHAIR , NULL , 0) ;
	m_GameClientView.SetShowCard(INVALID_CHAIR , NULL , 0) ;

	//����˿�
	for (WORD i=0;i<GAME_PLAYER;i++) m_GameClientView.m_UserCardControl[i].SetCardData(NULL,0);

	//������Ϣ
	SendUserReady(NULL,0);

	return 0;
}
//������Ϣ
LRESULT CGameClientDlg::OnOutCard(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam) ;
	UNREFERENCED_PARAMETER(lParam) ;

	//״̬�ж�
	if(m_GameClientView.m_btOutCard.IsWindowVisible()==FALSE) return 0;
	if(m_GameClientView.m_btOutCard.IsWindowEnabled()==FALSE) return 0;

	//�ָ�����
	m_bFirstOutCard = false ;

	//���ý���
	KillGameTimer(IDI_OUT_CARD);
	m_GameClientView.m_btOutCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btOutCard.EnableWindow(FALSE);

	//��������
	CMD_C_OutCard OutCard;
	memset(&OutCard , 0 , sizeof(OutCard)) ;
	OutCard.bCardCount=(BYTE)m_GameClientView.m_HandCardControl.GetShootCard(&OutCard.bCardData,1);

	SendData(SUB_C_OUT_CART,&OutCard,sizeof(OutCard)-sizeof(OutCard.bCardData)+OutCard.bCardCount*sizeof(BYTE));

	//Ԥ����ʾ
	PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));

	m_GameClientView.m_UserCardControl[2].SetCardData(&OutCard.bCardData,OutCard.bCardCount);

	//Ԥ��ɾ��
	BYTE bSourceCount=m_bHandCardCount;
	m_bHandCardCount-=OutCard.bCardCount;
	m_GameLogic.RemoveCard(&OutCard.bCardData,OutCard.bCardCount,m_bHandCardData,bSourceCount);
	m_GameClientView.m_HandCardControl.SetCardData(m_bHandCardData,m_bHandCardCount);

	return 0;
}
//����˿�
LRESULT CGameClientDlg::OnLeftHitCard(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam) ;
	UNREFERENCED_PARAMETER(lParam) ;

	//���ư�ť
	if(TRUE==m_GameClientView.m_btOutCard.IsWindowVisible())
	{
		//���ÿؼ�
		bool bResult = VerdictOutCard() ;

		m_GameClientView.m_btOutCard.EnableWindow((true==bResult)?TRUE:FALSE); 
	}

	//���ư�ť
	if(TRUE==m_GameClientView.m_btShowCard.IsWindowVisible())
	{
		//���ÿؼ�
		bool bResult = VerdictShowCard() ;

		m_GameClientView.m_btShowCard.EnableWindow((true==bResult)?TRUE:FALSE); 

	}

	return 0;
}

//�Ҽ��˿�
LRESULT CGameClientDlg::OnRightHitCard(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam) ;
	UNREFERENCED_PARAMETER(lParam) ;

	//�û�����
	bool bResult = VerdictOutCard() ;
	if(false==bResult) return 0 ;
	OnOutCard(0,0);

	return 0;
}

LRESULT CGameClientDlg::OnWaitStart(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam) ;
	UNREFERENCED_PARAMETER(lParam) ;

	KillGameTimer(IDI_START_GAME);
	SetGameTimer(GetMeChairID(),IDI_WAIT_START,10);
	return 0;
}
//��Ҳ���
LRESULT CGameClientDlg::OnNotShowCard(WPARAM wParam, LPARAM lParam)
{
	//���ò���
	UNREFERENCED_PARAMETER(wParam) ;
	UNREFERENCED_PARAMETER(lParam) ;
    
	//ɾ����ʱ��
	KillGameTimer(IDI_SHOW_CARD);

	CMD_C_ShowCard  ShowCard ;
	memset(&ShowCard , 0 , sizeof(ShowCard)) ;

	SendData(SUB_C_SHOW_CART , &ShowCard , sizeof(ShowCard)) ;

	m_GameClientView.m_btShowCard.ShowWindow(SW_HIDE);

	m_GameClientView.m_btNotShowCard.ShowWindow(SW_HIDE);

	m_GameClientView.m_HandCardControl.ShootAllCard(false) ;

	m_GameClientView.m_bShowCardHint = false ;
	m_GameClientView.UpdateGameView(NULL);

	return 0 ;
}
//�������
LRESULT CGameClientDlg::OnShowCard(WPARAM wParam, LPARAM lParam)
{
	//���ò���
	UNREFERENCED_PARAMETER(wParam) ;
	UNREFERENCED_PARAMETER(lParam) ;

	//ɾ����ʱ��
	KillGameTimer(IDI_SHOW_CARD);

	//��ȡ�˿�
	BYTE bCardData[4];
	BYTE bShootCount=(BYTE)m_GameClientView.m_HandCardControl.GetShootCard(bCardData,4);

	CMD_C_ShowCard  ShowCard ;
	memset(&ShowCard , 0 , sizeof(ShowCard)) ;

	CopyMemory(ShowCard.bShowCard , bCardData , bShootCount) ;
	ShowCard.bShowCardCount = bShootCount ;

    SendData(SUB_C_SHOW_CART , &ShowCard , sizeof(ShowCard)) ;

    m_GameClientView.m_HandCardControl.ShootAllCard(false) ;

	m_GameClientView.m_btShowCard.ShowWindow(SW_HIDE);

	m_GameClientView.m_btNotShowCard.ShowWindow(SW_HIDE);

	m_GameClientView.m_bShowCardHint = false ;
	m_GameClientView.UpdateGameView(NULL);

   return 0 ;
}
//�Զ�����
LRESULT CGameClientDlg::OnAutoOutCard(WPARAM wParam, LPARAM lParam)
{
	//���ò���
	UNREFERENCED_PARAMETER(wParam) ;
	UNREFERENCED_PARAMETER(lParam) ;

	m_bAutoOutCard = !m_bAutoOutCard ; 

	//����ʱ��
	if(true==m_bAutoOutCard) m_bTimerElapse = 1 ;

	AutomatismOutCard() ;

	//���ý���
	m_GameClientView.m_btAutoOutCard.ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btAutoOutCard.EnableWindow(FALSE) ;
	m_GameClientView.m_btCancelAutoOutCard.ShowWindow(SW_SHOW) ;
	m_GameClientView.m_btCancelAutoOutCard.EnableWindow(TRUE) ;
	return 0 ;
}
//ȡ������
LRESULT CGameClientDlg::OnCancelAutoOutCard(WPARAM wParam, LPARAM lParam)
{
	//���ò���
	UNREFERENCED_PARAMETER(wParam) ;
	UNREFERENCED_PARAMETER(lParam) ;

	m_bAutoOutCard = !m_bAutoOutCard ;
	if(!m_bAutoOutCard)	m_bTimerElapse = 30 ;

	//���ý���
	m_GameClientView.m_btCancelAutoOutCard.ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btCancelAutoOutCard.EnableWindow(FALSE) ;
	m_GameClientView.m_btAutoOutCard.ShowWindow(SW_SHOW) ;
	m_GameClientView.m_btAutoOutCard.EnableWindow(TRUE) ;

	return 0 ;
}
//�Զ�����
bool CGameClientDlg::AutomatismOutCard()
{
	//�����˿�
	BYTE bCardDataIndex ;
	m_GameClientView.m_HandCardControl.ShootAllCard(false) ;
	for(BYTE i=m_bHandCardCount-1 ; i!=255 ; --i)
	{
		//������֤
		ASSERT(i>=0 && i<=m_GameClientView.m_HandCardControl.GetCardCount()) ;
        if(i==255 || i>m_GameClientView.m_HandCardControl.GetCardCount()) return false ;
	
        bCardDataIndex = i ;
		m_GameClientView.m_HandCardControl.SetShootCard(&bCardDataIndex , 1) ;	
		//��Ч�ж�
		if(true==VerdictOutCard(true)) 
		{
			m_GameClientView.m_btOutCard.EnableWindow(TRUE) ;
			OnOutCard(0 , 0) ;
			return true ;
		}
		m_GameClientView.m_HandCardControl.ShootAllCard(false) ;
	}
	return false ;
}

//////////////////////////////////////////////////////////////////////////
