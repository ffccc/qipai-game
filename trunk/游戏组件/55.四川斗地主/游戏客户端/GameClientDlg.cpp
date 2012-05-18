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
#define IDI_DISPATCH_CARD				204								//���ƶ�ʱ��
#define IDI_LOOK_CATCH					205								//��ׯ��ʱ��

#ifdef _DEBUG
#define TIME_OUT_CARD					99								//���ƶ�ʱ��
#define TIME_START_GAME					99								//��ʼ��ʱ��
#define TIME_LAND_SCORE					99								//�зֶ�ʱ��
#define TIME_LOOK_CATCH					99								//��ׯ��ʱ��
#else
#define TIME_OUT_CARD					30								//���ƶ�ʱ��
#define TIME_START_GAME					30								//��ʼ��ʱ��
#define TIME_LAND_SCORE					10								//�зֶ�ʱ��
#define TIME_LOOK_CATCH					10								//��ׯ��ʱ��
#endif

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientDlg, CGameFrameDlg)
	ON_WM_TIMER()
	ON_MESSAGE(IDM_START,OnStart)
	ON_MESSAGE(IDM_OUT_CARD,OnOutCard)
	ON_MESSAGE(IDM_PASS_CARD,OnPassCard)
	ON_MESSAGE(IDM_LAND_SCORE,OnLandScore)
	ON_MESSAGE(IDM_AUTO_OUTCARD,OnAutoOutCard)
	ON_MESSAGE(IDM_LEFT_HIT_CARD,OnLeftHitCard)
	ON_MESSAGE(IDM_RIGHT_HIT_CARD,OnRightHitCard)
	ON_MESSAGE(IDM_LAND_AUTOPLAY,OnAutoPlay)
	ON_MESSAGE(IDM_SORT_HAND_CARD,OnMessageSortCard)

	ON_MESSAGE(IDM_LOOK_OR_CATCH,OnLookOrCatch)
	ON_MESSAGE(IDM_CALL_BANKER,OnCallBanker)
	ON_MESSAGE(IDM_ADD_SCORE,OnAddScore)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientDlg::CGameClientDlg() : CGameFrameDlg(&m_GameClientView)
{
	//��Ϸ����
	m_wBombTime=1;
	m_bHandCardCount=0;
	m_wLandUser=INVALID_CHAIR;
	memset(m_bBackCard,0,sizeof(m_bBackCard));
	memset(m_bCardCount,0,sizeof(m_bCardCount));
	memset(m_bHandCardData,0,sizeof(m_bHandCardData));
	m_bAutoPlay = false ;
	m_cbSortType=ST_COUNT;

	//���ñ���
	m_dwCardHSpace=DEFAULT_PELS;

	//���Ʊ���
	m_bTurnCardCount=0;
	m_bTurnOutType=CT_ERROR;
	memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));

	//��������
	m_wTimeOutCount=0;
	m_wMostUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_wTimeOutCount=0;
    
	//��������
	m_cbRemnantCardCount=0;
	m_cbDispatchCardCount=0;
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
	//���ñ���
	//SetWindowText(TEXT("�Ĵ���������Ϸ  --  Ver��6.5.1.0"));

	//����ͼ��
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);

	//��ȡ����
	m_dwCardHSpace=AfxGetApp()->GetProfileInt(TEXT("GameOption"),TEXT("CardSpace"),DEFAULT_PELS);
	m_bDeasilOrder=AfxGetApp()->GetProfileInt(TEXT("GameOption"),TEXT("DeasilOrder"),FALSE)?true:false;
	m_bAllowLookon = AfxGetApp()->GetProfileInt(TEXT("GameOption"),TEXT("AllowLookon"),FALSE)?true:false;

	//��������
	if ((m_dwCardHSpace>MAX_PELS)||(m_dwCardHSpace<LESS_PELS)) m_dwCardHSpace=DEFAULT_PELS;

	//���ÿؼ�
	m_GameClientView.m_HandCardControl[1].SetCardSpace(m_dwCardHSpace,0,20);

#ifdef _DEBUG
	//HINSTANCE hInstance=AfxGetInstanceHandle();

	//m_GameClientView.m_btAddScore.SetButtonImage(IDB_BT_BANKER_ADD1,hInstance,false);	
	//m_GameClientView.m_btNoAddScore.SetButtonImage(IDB_BT_BANKER_NO_ADD1,hInstance,false);

	//m_GameClientView.m_btAddScore.SetButtonImage(IDB_BT_BANKER_ADD2,hInstance,false);	
	//m_GameClientView.m_btNoAddScore.SetButtonImage(IDB_BT_BANKER_NO_ADD2,hInstance,false);

	//m_GameClientView.m_btAddScore.SetButtonImage(IDB_BT_ADD2,hInstance,false);	
	//m_GameClientView.m_btNoAddScore.SetButtonImage(IDB_BT_NO_ADD2,hInstance,false);

	//BYTE cbCardData[20];
	//ZeroMemory(cbCardData,sizeof(cbCardData));
	//		m_GameClientView.m_HandCardControl[0].SetCardData(cbCardData,20);
	//		m_GameClientView.m_HandCardControl[1].SetCardData(cbCardData,20);
	//		m_GameClientView.m_HandCardControl[2].SetCardData(cbCardData,20);
	//m_GameClientView. m_btAddScore.ShowWindow(5);
	//m_GameClientView. m_btNoAddScore.ShowWindow(5);

	//if (m_GameClientView.m_ScoreView.m_hWnd==NULL)
	//{
	//	m_GameClientView.m_ScoreView.Create(IDD_GAME_SCORE,&m_GameClientView);
	//}
	//m_GameClientView.m_ScoreView.ShowWindow(5);
	//m_GameClientView.m_ScoreView.SetGameTax(1,1);
#endif

	return true;
}

//���ÿ��
void CGameClientDlg::ResetGameFrame()
{
	//��Ϸ����
	m_wBombTime=1;
	m_bHandCardCount=0;
	m_wLandUser=INVALID_CHAIR;
	memset(m_bBackCard,0,sizeof(m_bBackCard));
	memset(m_bCardCount,0,sizeof(m_bCardCount));
	memset(m_bHandCardData,0,sizeof(m_bHandCardData));
	m_bAutoPlay = false ;
	m_cbSortType=ST_COUNT;

	//���Ʊ���
	m_bTurnCardCount=0;
	m_bTurnOutType=CT_ERROR;
	memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));

	//��������
	m_wTimeOutCount=0;
	m_wMostUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_wTimeOutCount=0;
    
	//��������
	m_cbRemnantCardCount=0;
	m_cbDispatchCardCount=0;
	ZeroMemory(m_cbDispatchCardData,sizeof(m_cbDispatchCardData));

	//ɾ����ʱ
	KillGameTimer(0);
	KillTimer(IDI_MOST_CARD);
	KillTimer(IDI_DISPATCH_CARD);

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
	GameOption.m_bAllowLookOn=IsAllowUserLookon();

	//��������
	if (GameOption.DoModal()==IDOK)
	{
		//��ȡ����
		m_bDeasilOrder=GameOption.m_bDeasilOrder;
		m_dwCardHSpace=GameOption.m_dwCardHSpace;

		//���ÿؼ�
		EnableSound(GameOption.m_bEnableSound);
		m_GameClientView.SetUserOrder(GameOption.m_bDeasilOrder);
		m_GameClientView.m_HandCardControl[1].SetCardSpace(m_dwCardHSpace,0,20);
		AllowUserLookon(0,GameOption.m_bAllowLookOn);

		//��������
		AfxGetApp()->WriteProfileInt(TEXT("GameOption"),TEXT("CardSpace"),m_dwCardHSpace);
		AfxGetApp()->WriteProfileInt(TEXT("GameOption"),TEXT("DeasilOrder"),m_bDeasilOrder?TRUE:FALSE);
		AfxGetApp()->WriteProfileInt(TEXT("GameOption"),TEXT("AllowLookon"),m_bDeasilOrder?TRUE:FALSE);
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
			//�Զ�����
			if ((((nElapse==0)||((m_bAutoPlay==true)&&(nElapse<TIME_OUT_CARD))))&&(wChairID==GetMeChairID()))
			{
				//��ʱ�ж�
				if ((IsLookonMode()==false)&&(m_bAutoPlay==false)&&((++m_wTimeOutCount)>=3)) 
				{
					//״̬�л�
					m_wTimeOutCount=0;

					//�йܹ���
					m_GameClientView.m_btAutoPlayOn.ShowWindow(SW_HIDE);
					m_GameClientView.m_btAutoPlayOff.ShowWindow(SW_SHOW);
					InsertSystemString( TEXT( "��������γ�ʱ���л�Ϊ��ϵͳ�йܡ�ģʽ" ) );
					m_bAutoPlay = true;

					CMD_C_UserTrustee UserTrustee;
					ZeroMemory( &UserTrustee, sizeof( UserTrustee ) );
					UserTrustee.bTrustee = true;
					UserTrustee.wUserChairID = GetMeChairID();

					SendData( SUB_C_TRUSTEE, &UserTrustee, sizeof( UserTrustee ) );
				}

				//�Զ���ʼ
				if ( m_GameClientView.m_btStart.IsWindowVisible() ) {
					OnStart( 0, 0 );
				}
				else {
					//�Զ�����
					OnAutoOutCard( 0, 0 ) ;
					OnOutCard( 0, 0 ) ;
				}
				return true;
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
				if ((IsLookonMode()==false)&&(wChairID==GetMeChairID())) 
				{
					PostMessage(WM_CLOSE,0,0);
				}
				return false;
			}
			if ((nElapse<=10)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false)) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));

			return true;
		}
	case IDI_LAND_SCORE:		//�ڿӽз�
		{
			if ( nElapse==0 || ( nElapse <= TIME_OUT_CARD && m_bAutoPlay ) )
			{
				if ((IsLookonMode()==false)&&(wChairID==GetMeChairID())) 
				{
					if(m_GameClientView.m_btCatch.IsWindowVisible()==TRUE)OnCallBanker(0,0);
					else OnAddScore(0,0);
				}
				return false;
			}
			if ((nElapse<=10)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false)) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));

			return true;
		}
	case IDI_LOOK_CATCH:		//�û���ץ
		{
			if ( nElapse==0 || ( nElapse <= TIME_OUT_CARD && m_bAutoPlay ) )
			{
				if ((IsLookonMode()==false)&&(wChairID==GetMeChairID())) OnLookOrCatch(0,0);
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
	//�Թ�����
	m_GameClientView.m_HandCardControl[1].SetDisplayFlag( ( IsLookonMode() == true ) && ( IsAllowLookon() == true ) );

	return;
}

//������Ϣ
bool CGameClientDlg::OnGameMessage(WORD wSubCmdID, const void * pBuffer, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_CHOICE_LOOK:		//�û���ׯ
		{
			return OnSubCallBanker(pBuffer,wDataSize);
		}
	case SUB_S_SEND_CARD:		//�����˿�
		{
			return OnSubSendCard(pBuffer,wDataSize);
		}
	case SUB_S_LAND_SCORE:		//�û��з�
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
	case SUB_C_TRUSTEE:			//����й�
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_UserTrustee));
			if (wDataSize != sizeof(CMD_C_UserTrustee)) return false;
			
			CMD_C_UserTrustee *pUserTrustee = ( CMD_C_UserTrustee * )pBuffer;

			m_GameClientView.SetUserTrustee( SwitchViewChairID( pUserTrustee->wUserChairID ), pUserTrustee->bTrustee );

			//��ʾ�й�
			if ( pUserTrustee->wUserChairID != GetMeChairID() || IsLookonMode() ) {
				
				CString strMsg;
				if ( pUserTrustee->bTrustee )
					strMsg.Format( TEXT( "[ %s ] ���ѡ�����й�" ), GetUserData( pUserTrustee->wUserChairID )->szName );
				else
					strMsg.Format( TEXT( "[ %s ] ���ȡ�����й�" ), GetUserData( pUserTrustee->wUserChairID )->szName );
				InsertSystemString( strMsg );

			}
			
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
			else
			{
				m_GameClientView.m_btAutoPlayOff.ShowWindow( SW_HIDE ) ;
				m_GameClientView.m_btAutoPlayOn.ShowWindow( SW_HIDE ) ;
				m_GameClientView.m_btAutoPlayOff.EnableWindow( FALSE ) ;
				m_GameClientView.m_btAutoPlayOn.EnableWindow( FALSE ) ;
				m_GameClientView.m_btSortCard.ShowWindow( SW_HIDE );
			}

			//�����˿�
			m_GameClientView.m_HandCardControl[1].SetDisplayFlag((IsLookonMode()==false)||(IsAllowLookon()==true));

			//����ʱ��
			SetGameTimer(GetMeChairID(),IDI_START_GAME,TIME_START_GAME);

			return true;
		}
	case GS_WK_SCORE:	//�з�״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusScore)) return false;
			CMD_S_StatusScore * pStatusScore=(CMD_S_StatusScore *)pBuffer;

			//���ý���
			m_GameClientView.SetBaseScore(pStatusScore->lBaseScore);
			m_GameClientView.SetSingleRandCard(SwitchViewChairID(pStatusScore->wFirstUser),pStatusScore->bMingCard);

			//���õ���
			if(pStatusScore->wBankerUser<GAME_PLAYER)
			{
				m_wLandUser=pStatusScore->wBankerUser;
				m_GameClientView.SetLandUser(SwitchViewChairID(m_wLandUser));

				//�з���Ϣ
				for(WORD i=0;i<GAME_PLAYER;i++)
				{
					if(pStatusScore->bCallScoreTimes[i]>0)
						m_GameClientView.SetUserScroeTag(SwitchViewChairID(i),pStatusScore->bCallScoreTimes[i]);
				}
			}

			//���ƿؼ�
			CopyMemory(m_bBackCard,pStatusScore->bBackCard,sizeof(m_bBackCard));
			if(m_bBackCard[0]!=0 && m_wLandUser<GAME_PLAYER)
			{
				m_GameClientView.m_BackCardControl.SetDisplayFlag(true);
				m_GameClientView.m_BackCardControl.SetCardData(m_bBackCard,CountArray(m_bBackCard));
			}
			else
			{
				BYTE bBackCard[3]={0,0,0};
				m_GameClientView.m_BackCardControl.SetCardData(bBackCard,CountArray(bBackCard));
			}

			//�˿˿ؼ�
			BYTE bCurrentState = pStatusScore->bCurrentState;
			WORD wMeChairID = GetMeChairID();
			m_bHandCardCount=17;
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//�˿���Ŀ
				if(i!=m_wLandUser || pStatusScore->bBackCatchTag==TRUE)m_bCardCount[i] = 17;
				else m_bCardCount[i] = 20;
				WORD wViewChairID = SwitchViewChairID(i);
				m_GameClientView.SetCardCount(wViewChairID,m_bCardCount[i]);

				//���ÿؼ�
				BYTE cbCardData[20] ;
				ZeroMemory(cbCardData, sizeof(cbCardData)) ;
				m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(cbCardData,m_bCardCount[i]);
			}

			//�����˿�
			if(pStatusScore->bCardData[0]!=0 && pStatusScore->bBackCatchTag==FALSE)
			{
				BYTE bTempCount = ((m_bBackCard[0]!=0 && wMeChairID==m_wLandUser)?20:17);
				m_bHandCardCount = bTempCount;
				CopyMemory(m_bHandCardData,pStatusScore->bCardData,sizeof(m_bHandCardData));
				m_GameClientView.m_HandCardControl[1].SetCardData(m_bHandCardData,bTempCount);
			}
			m_GameClientView.m_HandCardControl[1].SetDisplayFlag((IsLookonMode()==false)||(IsAllowLookon()==true));

			//�ȴ���Ϣ
			BYTE bTemp = 0;
			switch((bCurrentState&0x0F))
			{
			case 0:				
				{
					m_GameClientView.SetUserWaitType(1);
					break;
				}
			case CALL_BANKER:
				{
					m_GameClientView.SetUserWaitType(2);
					break;
				}
			case CALL_SCORE:
				{
					if(m_wLandUser==pStatusScore->wCurrentUser)m_GameClientView.SetUserWaitType(4);
					else m_GameClientView.SetUserWaitType(3);
					break;
				}
			case CALL_TWO_SCORE:
				{
					if(m_wLandUser==pStatusScore->wCurrentUser)m_GameClientView.SetUserWaitType(6);
					else m_GameClientView.SetUserWaitType(5);
					break;
				}
			}

			//���ÿؼ�
			if ((IsLookonMode()==false)&&(pStatusScore->wCurrentUser==wMeChairID))
			{
				ActiveGameFrame();

				//��ʾ�ؼ�
				switch((bCurrentState&0x0F))
				{
				case 0:
					{
						m_GameClientView.m_btBackCatch.ShowWindow(SW_SHOW);
						m_GameClientView.m_btLookCard.ShowWindow(SW_SHOW);

						break;
					}
				case CALL_BANKER:
					{
						m_GameClientView.m_btCatch.ShowWindow(SW_SHOW);
						m_GameClientView.m_btNoCatch.ShowWindow(SW_SHOW);

						break;
					}
				case CALL_SCORE:
					{
						ASSERT(m_wLandUser<GAME_PLAYER);
						HINSTANCE hInstance=AfxGetInstanceHandle();
						if(m_wLandUser==wMeChairID)
						{
							m_GameClientView.m_btAddScore.SetButtonImage(IDB_BT_BANKER_ADD1,hInstance,false);	
							m_GameClientView.m_btNoAddScore.SetButtonImage(IDB_BT_BANKER_NO_ADD1,hInstance,false);
						}
						else
						{
							m_GameClientView.m_btAddScore.SetButtonImage(IDB_BT_ADD1,hInstance,false);	
							m_GameClientView.m_btNoAddScore.SetButtonImage(IDB_BT_NO_ADD1,hInstance,false);
						}
						m_GameClientView.m_btAddScore.ShowWindow(SW_SHOW);
						m_GameClientView.m_btNoAddScore.ShowWindow(SW_SHOW);

						break;
					}
				case CALL_TWO_SCORE:
					{
						ASSERT(m_wLandUser<GAME_PLAYER);
						HINSTANCE hInstance=AfxGetInstanceHandle();
						if(m_wLandUser==wMeChairID)
						{
							m_GameClientView.m_btAddScore.SetButtonImage(IDB_BT_BANKER_ADD2,hInstance,false);	
							m_GameClientView.m_btNoAddScore.SetButtonImage(IDB_BT_BANKER_NO_ADD2,hInstance,false);
						}
						else
						{
							m_GameClientView.m_btAddScore.SetButtonImage(IDB_BT_ADD2,hInstance,false);	
							m_GameClientView.m_btNoAddScore.SetButtonImage(IDB_BT_NO_ADD2,hInstance,false);
						}
						m_GameClientView.m_btAddScore.ShowWindow(SW_SHOW);
						m_GameClientView.m_btNoAddScore.ShowWindow(SW_SHOW);

						break;
					}
				}
			}

			//������ť
			if (IsLookonMode())
			{
				m_GameClientView.m_btAutoPlayOff.ShowWindow( SW_HIDE ) ;
				m_GameClientView.m_btAutoPlayOn.ShowWindow( SW_HIDE ) ;
				m_GameClientView.m_btAutoPlayOff.EnableWindow( FALSE ) ;
				m_GameClientView.m_btAutoPlayOn.EnableWindow( FALSE ) ;
				m_GameClientView.m_btSortCard.ShowWindow( SW_HIDE );
			}
			else if(m_bHandCardData[0]!=0)
			{
				m_GameClientView.m_btSortCard.EnableWindow(TRUE);
			}

			//�����й�
			for ( WORD wChairID = 0; wChairID < GAME_PLAYER; ++wChairID )
			{
				m_GameClientView.SetUserTrustee( SwitchViewChairID( wChairID ), pStatusScore->bUserTrustee[ wChairID ] );

				//�Լ��ж�
				if ( ! IsLookonMode() && wChairID == GetMeChairID() && pStatusScore->bUserTrustee[ wChairID ] )
				{
					//�йܹ���
					m_GameClientView.m_btAutoPlayOn.ShowWindow(SW_HIDE);
					m_GameClientView.m_btAutoPlayOff.ShowWindow(SW_SHOW);
					OnAutoPlay( true, 0 );
				}
			}

			//����ʱ��
			if((bCurrentState&0x0F)==0)
				SetGameTimer(pStatusScore->wCurrentUser,IDI_LOOK_CATCH,TIME_LOOK_CATCH);
			else 
				SetGameTimer(pStatusScore->wCurrentUser,IDI_LAND_SCORE,TIME_LAND_SCORE);

			return true;
		}
	case GS_WK_PLAYING:		//��Ϸ״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pBuffer;

			//���ý���
			m_GameClientView.SetBaseScore(pStatusPlay->lBaseScore);
			m_GameClientView.SetSingleRandCard(SwitchViewChairID(pStatusPlay->wFirstUser),pStatusPlay->bMingCard);

			//���õ���
			m_wLandUser=pStatusPlay->wBankerUser;
			m_GameClientView.SetLandUser(SwitchViewChairID(m_wLandUser));

			//�з���Ϣ
			for(WORD i=0;i<GAME_PLAYER;i++)
			{
				if(pStatusPlay->bCallScoreTimes[i]>0)
					m_GameClientView.SetUserScroeTag(SwitchViewChairID(i),pStatusPlay->bCallScoreTimes[i]);
			}

			//���ƿؼ�
			CopyMemory(m_bBackCard,pStatusPlay->bBackCard,sizeof(m_bBackCard));
			if(m_bBackCard[0]!=0)
			{
				m_GameClientView.m_BackCardControl.SetDisplayFlag(true);
				m_GameClientView.m_BackCardControl.SetCardData(m_bBackCard,CountArray(m_bBackCard));
			}
			else
			{
				BYTE bBackCard[3]={0,0,0};
				m_GameClientView.m_BackCardControl.SetCardData(bBackCard,CountArray(bBackCard));
			}

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
			m_GameClientView.m_HandCardControl[1].SetCardData(m_bHandCardData,m_bHandCardCount);

			//���ñ���
			m_GameClientView.SetBombTime((pStatusPlay->wOutBombCount+1));
			WORD wUserMultiple[GAME_PLAYER];
			WORD wTempCount = (INT)pow(2,pStatusPlay->wOutBombCount);
			for(WORD i=0;i<GAME_PLAYER;i++)wUserMultiple[i]=(WORD)pStatusPlay->bScoreTimes[i]/wTempCount;
			m_GameClientView.SetUserScroeTimes(wUserMultiple);

			//�����˿�
			BYTE cbCardData[20];
			ZeroMemory(cbCardData,sizeof(cbCardData));
			for (BYTE i=0;i<GAME_PLAYER;i++)
			{
				WORD wViewChairID=SwitchViewChairID(i);
				if (wViewChairID!=1) m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(cbCardData,pStatusPlay->bCardCount[i]);
			}

			//�������
			if ((IsLookonMode()==false)&&(pStatusPlay->wCurrentUser==GetMeChairID()))
			{
				m_GameClientView.m_btOutCard.EnableWindow(FALSE);
				m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
				m_GameClientView.m_btPassCard.ShowWindow(SW_SHOW);
				m_GameClientView.m_btAutoOutCard.ShowWindow(SW_SHOW);
				m_GameClientView.m_btPassCard.EnableWindow((m_bTurnCardCount!=0)?TRUE:FALSE);
				m_GameClientView.m_btAutoOutCard.EnableWindow(TRUE);
			}

			//��������
			if (m_bTurnCardCount!=0)
			{
				WORD wViewChairID=SwitchViewChairID(pStatusPlay->wLastOutUser);
				m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(m_bTurnCardData,m_bTurnCardCount);
			}

			if ( IsLookonMode() )
			{
				m_GameClientView.m_btAutoPlayOff.ShowWindow( SW_HIDE ) ;
				m_GameClientView.m_btAutoPlayOn.ShowWindow( SW_HIDE ) ;
				m_GameClientView.m_btAutoPlayOff.EnableWindow( FALSE ) ;
				m_GameClientView.m_btAutoPlayOn.EnableWindow( FALSE ) ;
				m_GameClientView.m_btSortCard.ShowWindow( SW_HIDE );
			}
			else
			{
				m_GameClientView.m_btSortCard.EnableWindow(TRUE);
			}

			for ( WORD wChairID = 0; wChairID < GAME_PLAYER; ++wChairID )
			{
				m_GameClientView.SetUserTrustee( SwitchViewChairID( wChairID ), pStatusPlay->bUserTrustee[ wChairID ] );
				
				//�Լ��ж�
				if ( ! IsLookonMode() && wChairID == GetMeChairID() &&pStatusPlay->bUserTrustee[ wChairID ] )
				{
					//�йܹ���
					m_GameClientView.m_btAutoPlayOn.ShowWindow(SW_HIDE);
					m_GameClientView.m_btAutoPlayOff.ShowWindow(SW_SHOW);
					OnAutoPlay( true, 0 );
				}
			}

			//���ö�ʱ��
			SetGameTimer(pStatusPlay->wCurrentUser,IDI_OUT_CARD,TIME_OUT_CARD);

			//�����˿�
			m_GameClientView.m_HandCardControl[1].SetPositively(IsLookonMode()==false);
			m_GameClientView.m_HandCardControl[1].SetDisplayFlag((IsLookonMode()==false)||(IsAllowLookon()==true));

			return true;
		}
	}

	return false;
}

//�û���ץ
bool CGameClientDlg::OnSubCallBanker(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_CallBanker));
	if (wDataSize!=sizeof(CMD_S_CallBanker)) return false;

	//�رճɼ�
	if (m_GameClientView.m_ScoreView.m_hWnd!=NULL)
	{
		m_GameClientView.m_ScoreView.ShowWindow(SW_HIDE) ;
		m_GameClientView.m_ScoreView.DestroyWindow();
	}

	//�������
	m_wBombTime = 1;
	m_GameClientView.SetBombTime(m_wBombTime);
	m_GameClientView.SetLandUser(INVALID_CHAIR);
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.SetLandAction(i,0);
		m_GameClientView.SetPassFlag(i,false);
		m_GameClientView.m_UserCardControl[i].SetCardData(NULL,0);
	}

	//��������
	CMD_S_CallBanker * pCallBanker=(CMD_S_CallBanker *)pBuffer;

	//�ؼ��ж�
	WORD wMeChairID = GetMeChairID();
	if(wMeChairID==pCallBanker->wCurrentUser && !IsLookonMode())
	{
		//��ʾ�ؼ�
		ActiveGameFrame();
		m_GameClientView.m_btBackCatch.ShowWindow(SW_SHOW);
		m_GameClientView.m_btLookCard.ShowWindow(SW_SHOW);
	}

	//���ý���
	m_bHandCardCount=17;
	m_GameClientView.SetUserWaitType(1);
	m_GameClientView.SetSingleRandCard(SwitchViewChairID(pCallBanker->wCurrentUser),pCallBanker->bMingCard);

	//�˿˿ؼ�
	BYTE bBackCard[3]={0,0,0};
	m_GameClientView.m_BackCardControl.SetCardData(bBackCard,CountArray(bBackCard));
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_bCardCount[i] = 17;
		m_GameClientView.SetCardCount(i,m_bCardCount[i]);
		BYTE cbCardData[20] ;
		ZeroMemory(cbCardData, sizeof(cbCardData)) ;
		m_GameClientView.m_HandCardControl[i].SetCardData(cbCardData,17);
	}

	//����ʱ��
	SetGameTimer(pCallBanker->wCurrentUser,IDI_LOOK_CATCH,TIME_LOOK_CATCH);

	//��������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));

	return true;
}

//�����˿�
bool CGameClientDlg::OnSubSendCard(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_SendAllCard));
	if (wDataSize!=sizeof(CMD_S_SendAllCard)) return false;

	//��������
	CMD_S_SendAllCard * pSendCard=(CMD_S_SendAllCard *)pBuffer;

	//��������
	CopyMemory(m_bBackCard,pSendCard->bBackCardData,sizeof(m_bBackCard));
	m_wCurrentUser=pSendCard->wCurrentUser;
	CopyMemory(m_bHandCardData,pSendCard->bCardData[GetMeChairID()],sizeof(pSendCard->bCardData[GetMeChairID()]));

	//�����˿�
	m_GameClientView.m_HandCardControl[1].SetDisplayFlag(((IsLookonMode()==false)||(IsAllowLookon()==true)));

#ifdef _DEBUG
	for ( WORD wChairID = 0; wChairID < GAME_PLAYER; ++wChairID )
	{
		m_GameClientView.m_HandCardControl[SwitchViewChairID(wChairID)].SetCardData(pSendCard->bCardData[wChairID], 17);
		m_GameClientView.m_HandCardControl[SwitchViewChairID(wChairID)].SetDisplayFlag(true);
	}
#endif

	//�ŷ��˿�
	DispatchUserCard(m_bHandCardData,m_bHandCardCount);
	m_GameLogic.SortCardList(m_bHandCardData,m_bHandCardCount,ST_ORDER);
	DoncludeDispatchCard();

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

	//���õ���
	if(pLandScore->wBankerUser!=INVALID_CHAIR)
	{
		m_wLandUser=pLandScore->wBankerUser;
		m_GameClientView.SetLandUser(SwitchViewChairID(m_wLandUser));
	}

	//���ý���
	WORD wViewChairID=SwitchViewChairID(pLandScore->bLandUser);
	m_GameClientView.SetLandAction(INVALID_CHAIR,0);
	m_GameClientView.SetLandAction(wViewChairID,pLandScore->bLandScore);

	//��������
	WORD wMeChairID = GetMeChairID();
	if (IsLookonMode()||(pLandScore->bLandUser!=wMeChairID))
	{
		switch(pLandScore->bLandScore)
		{
		case PASS_ACTION:
			PlayGameSound(AfxGetInstanceHandle(),TEXT("PASS_CARD"));
			break;
		case BACK_CATCH:
		case MING_CATCH:
		case LOOK_CARD:
			PlayGameSound(AfxGetInstanceHandle(),TEXT("CALL_CARD"));
			break;
		case CALL_SCORE:
		case CALL_TWO_SCORE:
			if(pLandScore->bLandUser==m_wLandUser)
				PlayGameSound(AfxGetInstanceHandle(),TEXT("LAND_SCORE"));
			else PlayGameSound(AfxGetInstanceHandle(),TEXT("IDLE_SCORE"));
			break;
		}
	}

	//�������
	if ((IsLookonMode()==false)&&(pLandScore->wCurrentUser==wMeChairID))
	{
		ActiveGameFrame();

		//��ʾ�ؼ�
		switch((pLandScore->bCurrentState&0x0F))
		{
		case CALL_BANKER:
			{
				m_GameClientView.m_btCatch.ShowWindow(SW_SHOW);
				m_GameClientView.m_btNoCatch.ShowWindow(SW_SHOW);

				break;
			}
		case CALL_SCORE:
			{
				ASSERT(m_wLandUser<GAME_PLAYER);
				HINSTANCE hInstance=AfxGetInstanceHandle();
				if(m_wLandUser==wMeChairID)
				{
					m_GameClientView.m_btAddScore.SetButtonImage(IDB_BT_BANKER_ADD1,hInstance,false);	
					m_GameClientView.m_btNoAddScore.SetButtonImage(IDB_BT_BANKER_NO_ADD1,hInstance,false);
				}
				else
				{
					m_GameClientView.m_btAddScore.SetButtonImage(IDB_BT_ADD1,hInstance,false);	
					m_GameClientView.m_btNoAddScore.SetButtonImage(IDB_BT_NO_ADD1,hInstance,false);
				}
				m_GameClientView.m_btAddScore.ShowWindow(SW_SHOW);
				m_GameClientView.m_btNoAddScore.ShowWindow(SW_SHOW);

				break;
			}
		case CALL_TWO_SCORE:
			{
				ASSERT(m_wLandUser<GAME_PLAYER);
				HINSTANCE hInstance=AfxGetInstanceHandle();
				if(m_wLandUser==wMeChairID)
				{
					m_GameClientView.m_btAddScore.SetButtonImage(IDB_BT_BANKER_ADD2,hInstance,false);	
					m_GameClientView.m_btNoAddScore.SetButtonImage(IDB_BT_BANKER_NO_ADD2,hInstance,false);
				}
				else
				{
					m_GameClientView.m_btAddScore.SetButtonImage(IDB_BT_ADD2,hInstance,false);	
					m_GameClientView.m_btNoAddScore.SetButtonImage(IDB_BT_NO_ADD2,hInstance,false);
				}
				m_GameClientView.m_btAddScore.ShowWindow(SW_SHOW);
				m_GameClientView.m_btNoAddScore.ShowWindow(SW_SHOW);

				break;
			}
		}
	}

	//�ȴ���Ϣ
	WORD wCurrentUser = pLandScore->wCurrentUser;
	switch((pLandScore->bCurrentState&0x0F))
	{
	case CALL_BANKER:
		{
			m_GameClientView.SetUserWaitType(2);
			break;
		}
	case CALL_SCORE:
		{
			if(m_wLandUser==wCurrentUser)m_GameClientView.SetUserWaitType(4);
			else m_GameClientView.SetUserWaitType(3);
			break;
		}
	case CALL_TWO_SCORE:
		{
			if(m_wLandUser==wCurrentUser)m_GameClientView.SetUserWaitType(6);
			else m_GameClientView.SetUserWaitType(5);
			break;
		}
	}

	//�з���Ϣ
	m_GameClientView.SetUserScroeTag(SwitchViewChairID(pLandScore->bLandUser),pLandScore->bUserCallTimes);

	//�������
	if(pLandScore->bCurrentState&0xF0)
	{
		//��ǰ�û�
		WORD wChairID = m_wLandUser;
		m_bCardCount[wChairID] = 20;

		//�������
		ASSERT(m_bBackCard[0]!=0);
		m_GameClientView.m_BackCardControl.SetCardData(m_bBackCard,CountArray(m_bBackCard));
		m_GameClientView.m_BackCardControl.SetDisplayFlag(true);

		//���ý���
		m_GameClientView.SetLandAction(INVALID_CHAIR,0);
		m_GameClientView.SetCardCount(SwitchViewChairID(wChairID),m_bCardCount[wChairID]);

		//��������
		if (wChairID==GetMeChairID())
		{
			BYTE bCardCound=m_bHandCardCount;
			m_bHandCardCount+=CountArray(m_bBackCard);
			CopyMemory(&m_bHandCardData[bCardCound],m_bBackCard,sizeof(m_bBackCard));
			m_GameLogic.SortCardList(m_bHandCardData,m_bHandCardCount,ST_ORDER);
			m_GameClientView.m_HandCardControl[1].SetCardData(m_bHandCardData,m_bHandCardCount);
		}
		else
		{	
			BYTE cbCardData[20] ;
			ZeroMemory(cbCardData, sizeof(cbCardData)) ;
			m_GameClientView.m_HandCardControl[SwitchViewChairID(wChairID)].SetCardData(cbCardData,20);
		}
	}

	//����ʱ��
	if(pLandScore->wCurrentUser<GAME_PLAYER)
		SetGameTimer(pLandScore->wCurrentUser,IDI_LAND_SCORE,TIME_LAND_SCORE);

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
	WORD wMeChiarID = GetMeChairID();
	ASSERT(m_wLandUser<GAME_PLAYER);
	ZeroMemory(m_bTurnCardData,sizeof(m_bTurnCardData));
	if(pGameStart->bBackCard[0]!=0)CopyMemory(m_bBackCard,pGameStart->bBackCard,sizeof(m_bBackCard));
	if(!IsLookonMode())m_GameClientView.m_btSortCard.EnableWindow(TRUE);

	//���ÿؼ�
	if(m_bCardCount[m_wLandUser]<20)
	{
		m_bCardCount[m_wLandUser]=20;
		m_GameClientView.m_BackCardControl.SetCardData(m_bBackCard,CountArray(m_bBackCard));
		m_GameClientView.m_BackCardControl.SetDisplayFlag( true );
		m_GameClientView.SetCardCount(SwitchViewChairID(m_wLandUser),m_bCardCount[m_wLandUser]);

		//��������
		if (m_wLandUser==GetMeChairID())
		{
			ASSERT(m_bBackCard[0]!=0);
			BYTE bCardCound=m_bHandCardCount;
			m_bHandCardCount=20;
			CopyMemory(&m_bHandCardData[bCardCound],m_bBackCard,sizeof(m_bBackCard));
			m_GameLogic.SortCardList(m_bHandCardData,m_bHandCardCount,ST_ORDER);
			m_GameClientView.m_HandCardControl[1].SetCardData(m_bHandCardData,m_bHandCardCount);
		}
		else 
		{
			BYTE cbCardData[20] ;
			ZeroMemory(cbCardData, sizeof(cbCardData)) ;
			m_GameClientView.m_HandCardControl[SwitchViewChairID(m_wLandUser)].SetCardData(cbCardData,20);
		}
	}

	//���ý���
	m_GameClientView.SetUserWaitType(0);
	WORD wUserMultiple[GAME_PLAYER];
	ZeroMemory(wUserMultiple,sizeof(wUserMultiple));
	for(WORD i=0;i<GAME_PLAYER;i++)
	{
		wUserMultiple[SwitchViewChairID(i)]=pGameStart->wUserMultiple[i];
	}
	m_GameClientView.SetUserScroeTimes(wUserMultiple);
	m_GameClientView.SetLandAction(INVALID_CHAIR,0);

	//�������
	m_GameClientView.m_HandCardControl[1].SetPositively( IsLookonMode()==false || IsAllowLookon() );

	//��ǰ���
	if ((IsLookonMode()==false)&&(pGameStart->wCurrentUser==wMeChiarID))
	{
		ActiveGameFrame();
		m_GameClientView.m_btOutCard.EnableWindow(FALSE);
		m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btPassCard.EnableWindow(FALSE);
		m_GameClientView.m_btPassCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btAutoOutCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btAutoOutCard.EnableWindow(TRUE);
	}

	//����ʱ��
	SetGameTimer(pGameStart->wCurrentUser,IDI_OUT_CARD,TIME_OUT_CARD);

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
	WORD wOutViewChairID=SwitchViewChairID(pOutCard->wOutCardUser);

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

	//ը���ж�
	if ((m_bTurnOutType==CT_BOMB_CARD)||(m_bTurnOutType==CT_MISSILE_CARD))
	{
		m_wBombTime++;
		m_GameClientView.SetBombTime(m_wBombTime);

		//���Ŷ���
		m_GameClientView.SetBombEffect(true);
	}

	//�Լ��˿�
	if ((IsLookonMode()==true)&&(pOutCard->wOutCardUser==GetMeChairID()))
	{
		//ɾ���˿� 
		BYTE bSourceCount=m_bHandCardCount;
		m_bHandCardCount-=pOutCard->bCardCount;
		m_GameLogic.RemoveCard(pOutCard->bCardData,pOutCard->bCardCount,m_bHandCardData,bSourceCount);

		//���ý���
		m_GameClientView.m_HandCardControl[1].SetCardData(m_bHandCardData,m_bHandCardCount);
	}

	//��������	
	m_bCardCount[pOutCard->wOutCardUser]-=pOutCard->bCardCount;
	m_GameClientView.SetCardCount(wOutViewChairID,m_bCardCount[pOutCard->wOutCardUser]);
	for (WORD wChairID=0; wChairID<GAME_PLAYER; ++wChairID)
	{
		WORD wViewID = SwitchViewChairID(wChairID) ;
		if(1!=wViewID)
		{
			BYTE cbCardData[20] ;
			ZeroMemory(cbCardData, sizeof(cbCardData)) ;
			m_GameClientView.m_HandCardControl[wViewID].SetCardData(cbCardData,m_bCardCount[wChairID]);
		}
	}

	//��˫����
	if(m_bCardCount[pOutCard->wOutCardUser]==1)PlayGameSound(AfxGetInstanceHandle(),TEXT("ONE_SHEET"));
	//��������
	else if(m_bCardCount[pOutCard->wOutCardUser]==2)PlayGameSound(AfxGetInstanceHandle(),TEXT("TWO_SHEET"));
	//ը���ж�
	else if ((m_bTurnOutType==CT_BOMB_CARD)||(m_bTurnOutType==CT_MISSILE_CARD))
		PlayGameSound(AfxGetInstanceHandle(),TEXT("BOMB_CARD"));
	//����һ��
	else if(m_bTurnOutType==CT_THREE_LINE_TAKE_ONE && pOutCard->bCardCount==4)
		PlayGameSound(AfxGetInstanceHandle(),TEXT("THREE_LINE_TAKE_ONE"));
	//��������
	else if(pOutCard->bCardCount>=6)
		PlayGameSound(AfxGetInstanceHandle(),TEXT("SIX_SHEET"));
	//��������
	else if(pOutCard->bCardCount==1)
	{
		BYTE bCardValue = m_GameLogic.GetCardValue( pOutCard->bCardData[0]);
		BYTE bCardColor = m_GameLogic.GetCardColor( pOutCard->bCardData[0]);
		if(bCardValue==2)
		{
			//�ڶ�����
			if((bCardColor>>4)%2==1)PlayGameSound(AfxGetInstanceHandle(),TEXT("BLACK_TWO"));
			//�������
			else PlayGameSound(AfxGetInstanceHandle(),TEXT("RED_TWO"));
		}
		//С������
		else if(0x0E==bCardValue)PlayGameSound(AfxGetInstanceHandle(),TEXT("KING1"));
		//��������
		else if(0x0F==bCardValue)PlayGameSound(AfxGetInstanceHandle(),TEXT("KING2"));
		else PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));
	}
	//��������
	else if ((IsLookonMode()==true)||(GetMeChairID()!=pOutCard->wOutCardUser))
		PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));

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

		//PlayGameSound(AfxGetInstanceHandle(),TEXT("MOST_CARD"));

		//���ö�ʱ��
		SetTimer(IDI_MOST_CARD,3000,NULL);

		return true;
	}

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
		SetGameTimer(pOutCard->wCurrentUser,IDI_OUT_CARD,(bCardCount<m_bTurnCardCount)?3:TIME_OUT_CARD);
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
		m_GameClientView.m_btAutoOutCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btPassCard.EnableWindow((m_bTurnCardCount>0)?TRUE:FALSE);
		m_GameClientView.m_btOutCard.EnableWindow((VerdictOutCard()==true)?TRUE:FALSE);
		m_GameClientView.m_btAutoOutCard.EnableWindow( TRUE );
	}

	//��������
	if ((IsLookonMode()==true)||(pPassCard->wPassUser!=GetMeChairID()))	PlayGameSound(AfxGetInstanceHandle(),TEXT("PASS_CARD"));

	//����ʱ��
	if (m_bTurnCardCount!=0)
	{
		BYTE bCardCount=m_bCardCount[pPassCard->wCurrentUser];
		SetGameTimer(pPassCard->wCurrentUser,IDI_OUT_CARD,(bCardCount<m_bTurnCardCount)?3:TIME_OUT_CARD);
	}
	else SetGameTimer(pPassCard->wCurrentUser,IDI_OUT_CARD,TIME_OUT_CARD);

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
	KillTimer(IDI_DISPATCH_CARD);

	//ֹͣ����
	DoncludeDispatchCard();

	//�������
	m_GameClientView.SetLandAction(INVALID_CHAIR,0);

	//���ؿؼ�
	m_GameClientView.m_btOutCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btPassCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btGiveUpScore.ShowWindow(SW_HIDE);
	m_GameClientView.m_btAutoOutCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btCatch.ShowWindow(SW_HIDE);
	m_GameClientView.m_btNoCatch.ShowWindow(SW_HIDE);
	m_GameClientView.m_btAddScore.ShowWindow(SW_HIDE);
	m_GameClientView.m_btNoAddScore.ShowWindow(SW_HIDE);
	m_GameClientView.m_btBackCatch.ShowWindow(SW_HIDE);
	m_GameClientView.m_btLookCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btSortCard.EnableWindow(FALSE);

	//���ÿؼ�
	m_GameClientView.m_btOutCard.EnableWindow(FALSE);
	m_GameClientView.m_btPassCard.EnableWindow(FALSE);

	//��������
	m_GameClientView.m_BackCardControl.SetCardData(pGameEnd->bBackCard,3);
	m_GameClientView.m_BackCardControl.SetDisplayFlag(true);

	//�����ɼ�
	if (m_GameClientView.m_ScoreView.m_hWnd==NULL)
	{
		m_GameClientView.m_ScoreView.Create(IDD_GAME_SCORE,&m_GameClientView);
	}

	InsertShtickString( TEXT( " ���ֽ������ɼ�ͳ�ƣ�" ), RGB( 255, 0, 0 ), true );

	//���û���
	CString strScore;
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		const tagUserData * pUserData=GetUserData(i);
		m_GameClientView.m_ScoreView.SetGameScore(i,pUserData->szName,pGameEnd->lGameScore[i]);
		strScore.Format( TEXT( " %s ��%ld" ), pUserData->szName,pGameEnd->lGameScore[i]);
		InsertShtickString( strScore, RGB( 255, 0, 0 ), true );
	}

	//��Ϸ����
	IClientKernel *pIClientKernel = ( IClientKernel * )GetClientKernel( IID_IClientKernel, VER_IClientKernel );
	const tagServerAttribute *pServerAttribute  = pIClientKernel->GetServerAttribute();

	if ( pServerAttribute->wGameGenre == GAME_GENRE_GOLD ) {
		strScore.Format( TEXT( "��˰��%ld" ), pGameEnd->lGameTax);
		InsertShtickString( strScore, RGB( 255, 0, 0 ), true );
	
	}

	m_GameClientView.m_ScoreView.SetGameTax(pGameEnd->lGameTax,m_GameClientView.GetBaseScore());

	//�����ɼ�
	CRect scoreRect, gameViewRect ;
	m_GameClientView.GetWindowRect( gameViewRect ) ;
	m_GameClientView.m_ScoreView.GetWindowRect( scoreRect ) ;
	m_GameClientView.m_ScoreView.MoveWindow( gameViewRect.left + ( gameViewRect.Width() - scoreRect.Width() ) / 2, 
		gameViewRect.top + ( gameViewRect.Height() - scoreRect.Height() ) / 2, scoreRect.Width(), scoreRect.Height()) ;
	m_GameClientView.m_ScoreView.ShowWindow(SW_SHOW);

	//��ʾ�˿�
	BYTE cbCardPos=0 ;
	for(BYTE i=0; i<GAME_PLAYER; ++i)
	{
		WORD wViewID = SwitchViewChairID(i) ;
		m_GameClientView.m_HandCardControl[wViewID].SetCardData(pGameEnd->bCardData+cbCardPos, pGameEnd->bCardCount[i]) ;
		m_GameClientView.m_HandCardControl[wViewID].SetDisplayFlag(true) ;
		cbCardPos += pGameEnd->bCardCount[i] ;
	}

	//��������
	WORD wMeChairID=GetMeChairID();
	DOUBLE lMeScore=pGameEnd->lGameScore[GetMeChairID()];
	if (lMeScore>0L) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WIN"));
	else if (lMeScore<0L) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_LOST"));
	else PlayGameSound(GetModuleHandle(NULL),TEXT("GAME_END"));

	//���ý���
	if (IsLookonMode()==false)
	{
		m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
		SetGameTimer(GetMeChairID(),IDI_START_GAME,TIME_START_GAME);

		//�йܹ���
		 m_GameClientView.m_btAutoPlayOn.ShowWindow(SW_SHOW);
		m_GameClientView.m_btAutoPlayOff.ShowWindow(SW_HIDE);
		OnAutoPlay( false, 0 );
	}
	m_GameClientView.SetUserWaitType(0);

#ifdef VIDEO_GAME
	//��ʾ�˿�
	m_GameClientView.m_HandCardControl[ 0 ].ShowWindow( SW_SHOW );
	m_GameClientView.m_HandCardControl[ 2 ].ShowWindow( SW_SHOW );
#endif

	return true;
}

//�����ж�
bool CGameClientDlg::VerdictOutCard()
{
	//״̬�ж�
	if (m_GameClientView.m_btOutCard.IsWindowVisible()==FALSE) return false;

	//��ȡ�˿�
	BYTE bCardData[20];
	BYTE bShootCount=(BYTE)m_GameClientView.m_HandCardControl[1].GetShootCard(bCardData,CountArray(bCardData));

	//�����ж�
	if (bShootCount>0L)
	{
		//��������
		BYTE bCardType=m_GameLogic.GetCardType(bCardData,bShootCount);

		//�����ж�
		if (bCardType==CT_ERROR) return false;

		//�����ж�
		if (m_bTurnCardCount==0) return true;
		return m_GameLogic.CompareCard(m_bTurnCardData,bCardData,m_bTurnCardCount,bShootCount);
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
		m_GameClientView.m_UserCardControl[1].SetCardData(OutCard.bCardData,OutCard.bCardCount);

		//Ԥ��ɾ��
		BYTE bSourceCount=m_bHandCardCount;
		m_bHandCardCount-=OutCard.bCardCount;
		m_GameLogic.RemoveCard(OutCard.bCardData,OutCard.bCardCount,m_bHandCardData,bSourceCount);
		m_GameClientView.m_HandCardControl[1].SetCardData(m_bHandCardData,m_bHandCardCount);
	}
	else OnPassCard(0,0);

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

		//���ý���
		m_GameClientView.m_HandCardControl[1].SetCardData(m_bHandCardData,m_bHandCardCount); 

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
	SetTimer(IDI_DISPATCH_CARD,120,NULL);

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
		SetGameTimer(wCurrentUser,IDI_OUT_CARD,TIME_OUT_CARD);

		return;
	}

	//�ɷ��˿�
	if (nIDEvent==IDI_DISPATCH_CARD)
	{
		//�ɷ�����
		if (m_cbRemnantCardCount>0)
		{
			//�˿�����
			BYTE cbUserCard[20];
			ZeroMemory(cbUserCard,sizeof(cbUserCard));

			//�ɷ��˿�
			m_cbRemnantCardCount--;
			m_cbDispatchCardCount++;
			m_GameLogic.SortCardList(m_cbDispatchCardData,m_cbDispatchCardCount,ST_ORDER);

			//���ý���
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				WORD wViewID = SwitchViewChairID(i) ;
				if (wViewID!=1)
				{
					m_GameClientView.m_HandCardControl[wViewID].SetCardData(cbUserCard,m_cbDispatchCardCount);
				}
				else m_GameClientView.m_HandCardControl[wViewID].SetCardData(m_cbDispatchCardData,m_cbDispatchCardCount); 
			}

			//��������
			PlayGameSound(AfxGetInstanceHandle(),TEXT("DISPATCH_CARD"));
		}

		//��ֹ�ж�
		if (m_cbRemnantCardCount==0)
		{
			//ɾ����ʱ��
			KillTimer(IDI_DISPATCH_CARD);

			//�˿���Ŀ
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				m_GameClientView.SetCardCount(SwitchViewChairID(i),m_bCardCount[i]);
			}

			//��ǰ���
			if ((IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID()))
			{
				ActiveGameFrame();
				m_GameClientView.m_btGiveUpScore.ShowWindow(SW_SHOW);
			}
			//���ý���
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				WORD wViewID = SwitchViewChairID(i) ;
				if (wViewID!=1)
				{
					BYTE cbCardData[20] ;
					ZeroMemory(cbCardData, sizeof(cbCardData)) ;
					m_GameClientView.m_HandCardControl[wViewID].SetCardData(cbCardData,17);
					m_GameClientView.m_HandCardControl[wViewID].SetDisplayFlag(false) ;
				}
				else 
				{
					m_GameClientView.m_HandCardControl[wViewID].SetCardData(m_bHandCardData,m_bHandCardCount); 
					if(false==IsLookonMode())
					{
						m_GameClientView.m_HandCardControl[wViewID].SetDisplayFlag(true) ;
						m_GameClientView.m_HandCardControl[wViewID].SetPositively(true) ;
					}
				}
			}

			if(IsLookonMode()==false) m_GameClientView.m_btSortCard.EnableWindow(TRUE);

			//��������
			PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));

			//����ʱ��
			if (m_wCurrentUser!=INVALID_CHAIR) SetGameTimer(m_wCurrentUser,IDI_LAND_SCORE,TIME_LAND_SCORE);
		}

		return;
	}

	__super::OnTimer(nIDEvent);
}

//��ʼ��ť
LRESULT CGameClientDlg::OnStart(WPARAM wParam, LPARAM lParam)
{
	//���ñ���
	m_wBombTime=1;
	m_wTimeOutCount=0;
	m_wTimeOutCount=0;
	m_bHandCardCount=0;
	m_bTurnCardCount=0;
	m_bTurnOutType=CT_ERROR;
	m_wMostUser=INVALID_CHAIR;
	memset(m_bHandCardData,0,sizeof(m_bHandCardData));
	memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));

	//���ý���
	KillGameTimer(IDI_START_GAME);
	m_GameClientView.SetUserScroeTimes(NULL);
	m_GameClientView.SetUserWaitType(0);
	m_GameClientView.SetBombTime(m_wBombTime);
	m_GameClientView.SetCardCount(INVALID_CHAIR,0);
	m_GameClientView.SetLandUser(INVALID_CHAIR);
	m_GameClientView.SetLandAction(INVALID_CHAIR,0);
	m_GameClientView.SetPassFlag(INVALID_CHAIR,false);

	//���ؿؼ�
	m_GameClientView.m_btStart.ShowWindow(FALSE);
	if (m_GameClientView.m_ScoreView.m_hWnd!=NULL) m_GameClientView.m_ScoreView.DestroyWindow();

	//�����˿�
	m_GameClientView.m_BackCardControl.SetCardData(NULL,0);
	m_GameClientView.m_HandCardControl[1].SetPositively(false);
	for (WORD i=0;i<GAME_PLAYER;i++) 
	{
		m_GameClientView.SetUserScroeTag(i,0);
		m_GameClientView.SetSingleRandCard(i,0);
		m_GameClientView.m_UserCardControl[i].SetCardData(NULL,0);
		m_GameClientView.m_HandCardControl[i].SetCardData(NULL,0);
		WORD wViewID = SwitchViewChairID(i) ;
		if(wViewID==1)	m_GameClientView.m_HandCardControl[wViewID].SetDisplayFlag(true);
		else m_GameClientView.m_HandCardControl[wViewID].SetDisplayFlag(false);
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

	//���ñ���
	m_wTimeOutCount=0;

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
	OutCard.bCardCount=(BYTE)m_GameClientView.m_HandCardControl[1].GetShootCard(OutCard.bCardData,CountArray(OutCard.bCardData));
	SendData(SUB_C_OUT_CART,&OutCard,sizeof(OutCard)-sizeof(OutCard.bCardData)+OutCard.bCardCount*sizeof(BYTE));

	//Ԥ����ʾ
	PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));
	m_GameClientView.m_UserCardControl[1].SetCardData(OutCard.bCardData,OutCard.bCardCount);

	//Ԥ��ɾ��
	BYTE bSourceCount=m_bHandCardCount;
	m_bHandCardCount-=OutCard.bCardCount;
	m_GameLogic.RemoveCard(OutCard.bCardData,OutCard.bCardCount,m_bHandCardData,bSourceCount);
	m_GameClientView.m_HandCardControl[1].SetCardData(m_bHandCardData,m_bHandCardCount);

	return 0;
}

//��������
LRESULT CGameClientDlg::OnPassCard(WPARAM wParam, LPARAM lParam)
{
	//״̬�ж�
	if (m_GameClientView.m_btPassCard.IsWindowEnabled()==FALSE) return 0;

	//���ñ���
	m_wTimeOutCount=0;

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
	PlayGameSound(AfxGetInstanceHandle(),TEXT("PASS_CARD"));
	m_GameClientView.m_HandCardControl[1].SetCardData(m_bHandCardData,m_bHandCardCount);

	return 0;
}

//�з���Ϣ
LRESULT CGameClientDlg::OnLandScore(WPARAM wParam, LPARAM lParam)
{
	//���ý���
	KillGameTimer(IDI_LAND_SCORE);
	m_GameClientView.m_btGiveUpScore.ShowWindow(SW_HIDE);

	//��������
	CMD_C_LandScore LandScore;
	LandScore.bLandScore=(BYTE)wParam;
	SendData(SUB_C_LAND_SCORE,&LandScore,sizeof(LandScore));

	return 0;
}

//������ʾ
LRESULT CGameClientDlg::OnAutoOutCard(WPARAM wParam, LPARAM lParam)
{
	//�������
	tagOutCardResult OutCardResult;
	m_GameLogic.SearchOutCard(m_bHandCardData,m_bHandCardCount,m_bTurnCardData,m_bTurnCardCount,OutCardResult);

	//���ý���
	if (OutCardResult.cbCardCount>0)
	{
		//���ý���
		m_GameClientView.m_HandCardControl[1].SetShootCard(OutCardResult.cbResultCard,OutCardResult.cbCardCount);

		//���ÿؼ�
		bool bOutCard=VerdictOutCard();
		m_GameClientView.m_btOutCard.EnableWindow((bOutCard==true)?TRUE:FALSE);
	}
	else 
	{
		//��������
		OnPassCard(0,0);
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

//////////////////////////////////////////////////////////////////////////

LRESULT CGameClientDlg::OnAutoPlay(WPARAM wParam, LPARAM lParam)
{
    bool bAutoPlay = (wParam == 1);
    if (bAutoPlay == m_bAutoPlay)
        return 0;

    m_bAutoPlay = bAutoPlay;

    if (m_bAutoPlay)
		InsertSystemString("ʹ���йܹ���, ��������Զ�����״̬!");
    else
		InsertSystemString("ȡ���йܹ���. ");

	//������Ϣ
	CMD_C_UserTrustee UserTrustee;
	ZeroMemory( &UserTrustee, sizeof( UserTrustee ) );
	UserTrustee.bTrustee = m_bAutoPlay;
	UserTrustee.wUserChairID = GetMeChairID();

	SendData( SUB_C_TRUSTEE, &UserTrustee, sizeof( UserTrustee ) );

    return 0;
}

//�����˿�
LRESULT CGameClientDlg::OnMessageSortCard(WPARAM wParam, LPARAM lParam)
{
	//�����˿�
	WORD wMeChairID=GetMeChairID();
	m_GameLogic.SortCardList(m_bHandCardData,m_bHandCardCount,m_cbSortType);

	//��ȡ�˿�
	BYTE cbShootCount=(BYTE)m_GameClientView.m_HandCardControl[1].SetCardData(m_bHandCardData,m_bHandCardCount);

	//���ñ���
	m_cbSortType=(m_cbSortType==ST_ORDER)?ST_COUNT:ST_ORDER;

	//���ð�ť
	UINT uImageID=(m_cbSortType==ST_ORDER)?IDB_ORDER_SORT:IDB_COUNT_SORT;
	m_GameClientView.m_btSortCard.SetButtonImage(uImageID,AfxGetInstanceHandle(),false);

	//���½���
	m_GameClientView.UpdateGameView(NULL);

	return 0;
}

//��ץ��Ϣ
LRESULT CGameClientDlg::OnLookOrCatch(WPARAM wParam, LPARAM lParam)
{
	//���ý���
	KillGameTimer(IDI_LOOK_CATCH);
	m_GameClientView.m_btLookCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btBackCatch.ShowWindow(SW_HIDE);

	//��������
	CMD_C_LandScore LandScore;
	LandScore.bLandScore=((BYTE(wParam)==TRUE)?BACK_CATCH:LOOK_CARD);
	SendData(SUB_C_LAND_SCORE,&LandScore,sizeof(LandScore));

	//��������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("CALL_CARD"));

	return 0;
}

//��ׯ��Ϣ
LRESULT CGameClientDlg::OnCallBanker(WPARAM wParam, LPARAM lParam)
{
	//���ý���
	KillGameTimer(IDI_LAND_SCORE);
	m_GameClientView.m_btCatch.ShowWindow(SW_HIDE);
	m_GameClientView.m_btNoCatch.ShowWindow(SW_HIDE);

	//��������
	CMD_C_LandScore LandScore;
	LandScore.bLandScore=((BYTE(wParam)==TRUE)?MING_CATCH:0);
	SendData(SUB_C_LAND_SCORE,&LandScore,sizeof(LandScore));

	//��������
	if(BYTE(wParam)==TRUE)PlayGameSound(AfxGetInstanceHandle(),TEXT("CALL_CARD"));
	else PlayGameSound(AfxGetInstanceHandle(),TEXT("PASS_CARD"));

	return 0;
}

//�з���Ϣ
LRESULT CGameClientDlg::OnAddScore(WPARAM wParam, LPARAM lParam)
{
	//���ý���
	KillGameTimer(IDI_LAND_SCORE);
	m_GameClientView.m_btAddScore.ShowWindow(SW_HIDE);
	m_GameClientView.m_btNoAddScore.ShowWindow(SW_HIDE);

	//��������
	CMD_C_LandScore LandScore;
	LandScore.bLandScore=((BYTE(wParam)==TRUE)?DOUBLE_SCORE:0);
	SendData(SUB_C_LAND_SCORE,&LandScore,sizeof(LandScore));

	//��������
	if(BYTE(wParam)==TRUE)
	{
		if(GetMeChairID()==m_wLandUser)
			PlayGameSound(AfxGetInstanceHandle(),TEXT("LAND_SCORE"));
		else PlayGameSound(AfxGetInstanceHandle(),TEXT("IDLE_SCORE"));
	}
	else PlayGameSound(AfxGetInstanceHandle(),TEXT("PASS_CARD"));

	return 0;
}
