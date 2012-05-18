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
#define IDI_CHA_CARD					205								//���ƶ�ʱ��
#define IDI_DIAN_CARD					206								//���ƶ�ʱ��

//////////////////////////////////////��ӵĶ�ʱ��/////////////////////////////////////////////
#define IDI_LI_GUN                      207
#define IDI_JUE_GUN                     208
#define IDI_LI_GUN_AGREE                209                             //ͬ��������
//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientDlg, CGameFrameDlg)
	ON_WM_TIMER()
	ON_MESSAGE(IDM_START,OnStart)
	ON_MESSAGE(IDM_OUT_CARD,OnOutCard)
	ON_MESSAGE(IDM_PASS_CARD,OnPassCard)
	ON_MESSAGE(IDM_AUTO_OUTCARD,OnAutoOutCard)
	ON_MESSAGE(IDM_LEFT_HIT_CARD,OnLeftHitCard)
	ON_MESSAGE(IDM_RIGHT_HIT_CARD,OnRightHitCard)
	ON_MESSAGE(IDM_LAND_AUTOPLAY,OnAutoPlay)
	ON_MESSAGE(IDM_SORT_HAND_CARD,OnMessageSortCard)
	/////////////////////////////////////////////////
	ON_MESSAGE(IDM_SORT_HAND_CHA,OnCha)
	ON_MESSAGE(IDM_SORT_HAND_NOCHA,OnNoCha)
	ON_MESSAGE(IDM_SORT_HAND_DIAN,OnDian)
	ON_MESSAGE(IDM_SORT_HAND_NODIAN,OnNoDian)

	ON_MESSAGE(IDM_LI_GUN, OnHitLiGun)
	ON_MESSAGE(IDM_JUE_GUN, OnHitJueGun)
	ON_MESSAGE(IDM_AGREE, OnHitAgree)
	ON_MESSAGE(IDM_NOT_LI_GUN, OnHitNotLiGun)
	ON_MESSAGE(IDM_NOT_JUE_GUN, OnHitNotJueGun)
	/////////////////////////////////////////////////
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientDlg::CGameClientDlg() : CGameFrameDlg(&m_GameClientView)
{
	//��Ϸ����
	m_wBombTime=1;
	m_bHandCardCount=0;
	m_wLandUser=INVALID_CHAIR;
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

////////////////////////////////////////////////////////
	m_cbLiOrJue = 0;
////////////////////////////////////////////////////////

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
	m_bAllowLookon = AfxGetApp()->GetProfileInt(TEXT("GameOption"),TEXT("AllowLookon"),FALSE)?true:false;

	//��������
	if ((m_dwCardHSpace>MAX_PELS)||(m_dwCardHSpace<LESS_PELS)) m_dwCardHSpace=DEFAULT_PELS;

	//���ÿؼ�
	m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].SetCardSpace(m_dwCardHSpace,0,20);

	return true;
}

//���ÿ��
void CGameClientDlg::ResetGameFrame()
{
	//��Ϸ����
	m_wBombTime=1;
	m_bHandCardCount=0;
	m_wLandUser=INVALID_CHAIR;
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
		m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].SetCardSpace(m_dwCardHSpace,0,20);
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
		////////////////////////////////////////////��ӵĶ�ʱ������////////////////////////////
	case IDI_LI_GUN:
		{
			//��ʱ��ʱ��ȫ����������������
			if (nElapse == 0)
			{
				SendData(SUB_C_LI_GUN_SUPERTIME);
			}
			return true;
		}
	//�������ʱ��ʱ�򣬵������������
	case IDI_JUE_GUN:
		{
			//��ʱ��ʱ��ȫ����������������
			if (nElapse == 0)
			{
				SendData(SUB_C_LI_GUN_SUPERTIME);
			}
			return true;
		}
	//��ʱ��ʱ����Ϊ��ͬ�⴦��
	case IDI_LI_GUN_AGREE:
		{
			if (nElapse == 0)
			{
				OnHitAgree(0,0);
			}
			return true;
		}
///////////////////////////////////////////////////////////////////////////////////////////////////
	case IDI_OUT_CARD:			//�û�����
		{
			//�Զ�����
			if ((((nElapse==0)||((m_bAutoPlay==true)&&(nElapse<28))))&&(wChairID==GetMeChairID()))
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
	case IDI_CHA_CARD:			//����
		{
			if (nElapse==0)
				if ( m_GameClientView.m_btTwoScore.IsWindowVisible() ) 
					OnNoCha( 0, 0 );

			return true;
		}
	case IDI_DIAN_CARD:			//����
		{
			if (nElapse==0)
				if ( m_GameClientView.m_btGiveUpScore.IsWindowVisible() ) 
					OnNoDian( 0, 0 );

			return true;
		}
	}

	return false;
}

//�Թ�״̬
void CGameClientDlg::OnLookonChanged(bool bLookonUser, const void * pBuffer, WORD wDataSize)
{
	//�Թ�����
	m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].SetDisplayFlag( ( IsLookonMode() == true ) && ( IsAllowLookon() == true ) );

	return;
}

//������Ϣ
bool CGameClientDlg::OnGameMessage(WORD wSubCmdID, const void * pBuffer, WORD wDataSize)
{
	switch (wSubCmdID)
	{
/////////////////////////////////////////////////////��ӵ���Ϣ����//////////////////////////////////////////
	case SUB_S_LI_GUN:
		{
			//AfxMessageBox("y");
			CMD_S_CanJueGun *pCanLiGun;
			pCanLiGun = ( CMD_S_CanJueGun * )pBuffer;
			//KillGameTimer()

			//m_GameClientView.m_btLiGun.ShowWindow(SW_HIDE);
			//m_GameClientView.m_btNotLiGun.ShowWindow(SW_HIDE);
			m_GameClientView.m_btJueGun.ShowWindow(SW_HIDE);
			m_GameClientView.m_btNotJueGun.ShowWindow(SW_HIDE);
			m_GameClientView.m_btAgree.ShowWindow(SW_HIDE);
			m_GameClientView.m_btDisagree.ShowWindow(SW_HIDE);
			//CString s;
			//s.Format("%d, %d", pCanLiGun->wCanJueChair[0], pCanLiGun->wCanJueChair[1]);
			//AfxMessageBox(s);

			

			if ( GetMeChairID() == pCanLiGun->wCanJueChair[0] || GetMeChairID() == pCanLiGun->wCanJueChair[1] )
			{
				m_GameClientView.m_btLiGun.ShowWindow(SW_SHOW);
				m_GameClientView.m_btNotLiGun.ShowWindow(SW_SHOW);
				m_GameClientView.m_btLiGun.EnableWindow(TRUE);
				m_GameClientView.m_btNotLiGun.EnableWindow(TRUE);
			}

			SetGameTimer(GetMeChairID(), IDI_LI_GUN, 10);

			return true;
		}
	case SUB_S_JUE_GUN_REQUEST:
		{
			//AfxMessageBox("a");
			CMD_S_LiGunRequest *pJueGunRequest;
			pJueGunRequest = (CMD_S_LiGunRequest *)pBuffer;


			m_GameClientView.m_btLiGun.ShowWindow(SW_HIDE);
			m_GameClientView.m_btNotLiGun.ShowWindow(SW_HIDE);
			m_GameClientView.m_btJueGun.ShowWindow(SW_HIDE);
			m_GameClientView.m_btNotJueGun.ShowWindow(SW_HIDE);

			if ( GetMeChairID() == pJueGunRequest->wRequest)
			{
				m_GameClientView.m_btAgree.ShowWindow(SW_SHOW);
				m_GameClientView.m_btDisagree.ShowWindow(SW_SHOW);
				m_GameClientView.m_btAgree.EnableWindow(TRUE);
				m_GameClientView.m_btDisagree.EnableWindow(TRUE);
			}

			m_cbLiOrJue = 2;

			SetGameTimer(GetMeChairID(), IDI_LI_GUN_AGREE, 5);

			return true;
		}
	case SUB_S_JUE_GUN:
		{
			CMD_S_CanJueGun *pCanJueGun;
			pCanJueGun = ( CMD_S_CanJueGun *)pBuffer;

			if (GetMeChairID()==pCanJueGun->wCanJueChair[0] || GetMeChairID()==pCanJueGun->wCanJueChair[1])
			{
				m_GameClientView.m_btJueGun.ShowWindow(SW_SHOW);
				m_GameClientView.m_btJueGun.EnableWindow(TRUE);
				m_GameClientView.m_btNotJueGun.ShowWindow(SW_SHOW);
				m_GameClientView.m_btNotJueGun.EnableWindow(TRUE);
			}
			m_cbLiOrJue = 2;

			SetGameTimer(GetMeChairID(), IDI_JUE_GUN, 10);
			return true;
		}
	case SUB_S_LI_GUN_REQUEST:
		{
			//AfxMessageBox("b");
			CMD_S_LiGunRequest *pLiGunRequest;
			pLiGunRequest = (CMD_S_LiGunRequest *)pBuffer;

			m_GameClientView.m_btLiGun.ShowWindow(SW_HIDE);
			m_GameClientView.m_btNotLiGun.ShowWindow(SW_HIDE);

			if ( GetMeChairID() == pLiGunRequest->wRequest)
			{
				m_GameClientView.m_btAgree.ShowWindow(SW_SHOW);
				m_GameClientView.m_btDisagree.ShowWindow(SW_SHOW);
				m_GameClientView.m_btAgree.EnableWindow(TRUE);
				m_GameClientView.m_btDisagree.EnableWindow(TRUE);
			}

			m_cbLiOrJue = 1;

			SetGameTimer(GetMeChairID(), IDI_LI_GUN_AGREE, 5);

			return true;
		}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
	case SUB_S_GAME_CHA:		//�û�����
		{
			return OnSubChaCard(pBuffer,wDataSize);
		}
	case SUB_S_GAME_NOCHA:		//��������
		{
			return OnSubPassCard(pBuffer,wDataSize);
		}
	case SUB_S_GAME_DIAN:		//�û�����
		{
			return OnSubDianCard(pBuffer,wDataSize);
		}
	case SUB_S_GAME_NODIAN:		//��������
		{
			return OnSubPassCard(pBuffer,wDataSize);
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
			m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].SetDisplayFlag((IsLookonMode()==false)||(IsAllowLookon()==true));

			//����ʱ��
			SetGameTimer(GetMeChairID(),IDI_START_GAME,30);

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
			m_bTurnOutType=m_GameLogic.GetCardType(pStatusPlay->bTurnCardData,m_bTurnCardCount);//��������
			CopyMemory(m_bHandCardData,pStatusPlay->bCardData[GetMeChairID()],m_bHandCardCount);
			CopyMemory(m_bTurnCardData,pStatusPlay->bTurnCardData,pStatusPlay->bTurnCardCount);

			//���ý���
			for (BYTE i=0;i<GAME_PLAYER;i++)
			{
				WORD wViewChairID=SwitchViewChairID(i);
				m_bCardCount[i]=pStatusPlay->bCardCount[i];
				m_GameClientView.SetCardCount(wViewChairID,pStatusPlay->bCardCount[i]);
			}
			m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].SetCardData(m_bHandCardData,m_bHandCardCount);
			
			//CString strScore;
			//strScore.Format( TEXT( " ��:%d \n---��:%ld\n---����:%ld\n---���ڳ��Ƶ���:%ld\n,�Լ���id:%d\n" ),pStatusPlay->gou,pStatusPlay->dian,IsLookonMode(),
			//	pStatusPlay->wCurrentUser,GetMeChairID());
			//InsertShtickString( strScore, RGB( 255, 0, 0 ), true );


			//�������
			if((pStatusPlay->gou)&&(IsLookonMode()==false)&&(pStatusPlay->wCurrentUser==GetMeChairID()))
			{
				m_GameClientView.m_btOneScore.ShowWindow(SW_SHOW);
				m_GameClientView.m_btOneScore.EnableWindow(FALSE);
				m_GameClientView.m_btTwoScore.ShowWindow(SW_SHOW);
				m_GameClientView.m_btTwoScore.EnableWindow(FALSE);
				SetGameTimer(pStatusPlay->wCurrentUser,IDI_CHA_CARD,10);
				//InsertShtickString( TEXT( " 11111111111111" ), RGB( 255, 0, 0 ), true );
			}
			else if((pStatusPlay->dian)&&(IsLookonMode()==false)&&(pStatusPlay->wCurrentUser==GetMeChairID()))
			{
				m_GameClientView.m_btThreeScore.ShowWindow(SW_SHOW);
				m_GameClientView.m_btThreeScore.EnableWindow(FALSE);
				m_GameClientView.m_btGiveUpScore.ShowWindow(SW_SHOW);
				m_GameClientView.m_btGiveUpScore.EnableWindow(FALSE);
				SetGameTimer(pStatusPlay->wCurrentUser,IDI_DIAN_CARD,10);
				////InsertShtickString( TEXT( " 222222222222222" ), RGB( 255, 0, 0 ), true );
			}
			else if ((IsLookonMode()==false)&&(pStatusPlay->wCurrentUser==GetMeChairID()))
			{
				m_GameClientView.m_btOutCard.EnableWindow(FALSE);
				m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
				m_GameClientView.m_btPassCard.ShowWindow(SW_SHOW);
				m_GameClientView.m_btAutoOutCard.ShowWindow(SW_SHOW);
				m_GameClientView.m_btPassCard.EnableWindow((m_bTurnCardCount!=0)?TRUE:FALSE);
				m_GameClientView.m_btAutoOutCard.EnableWindow(TRUE);
				//InsertShtickString( TEXT( " 3333333333333333" ), RGB( 255, 0, 0 ), true );
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

			//�����˿�
			BYTE cbCardData[20];
			ZeroMemory(cbCardData,sizeof(cbCardData));
			for (BYTE i=0;i<GAME_PLAYER;i++)
			{
				WORD wViewChairID=SwitchViewChairID(i);
				if (wViewChairID!=ME_VIEW_CHAIR) m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(cbCardData,pStatusPlay->bCardCount[i]);
			}

			//���ö�ʱ��
			SetGameTimer(pStatusPlay->wCurrentUser,IDI_OUT_CARD,30);

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

			//�����˿�
			m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].SetPositively(IsLookonMode()==false);
			m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].SetDisplayFlag((IsLookonMode()==false)||(IsAllowLookon()==true));

			return true;
		}
	}

	return false;
}

//�����˿�
bool CGameClientDlg::OnSubSendCard(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_SendAllCard));
	if (wDataSize!=sizeof(CMD_S_SendAllCard)) return false;

	//��������
	CMD_S_SendAllCard * pSendCard=(CMD_S_SendAllCard *)pBuffer;

	//�رճɼ�
	if (m_GameClientView.m_ScoreView.m_hWnd!=NULL)
	{
		m_GameClientView.m_ScoreView.ShowWindow(SW_HIDE) ;
		m_GameClientView.m_ScoreView.DestroyWindow();
	}

	//��������
	m_wCurrentUser=pSendCard->wCurrentUser;//��ǰ�û�

	CopyMemory(m_bHandCardData,pSendCard->bCardData[GetMeChairID()],sizeof(pSendCard->bCardData[GetMeChairID()]));//�����˿�
	
	int numtemp =0;
	for (WORD i=0;i<15;i++)
	{
		if(pSendCard->bCardData[GetMeChairID()][i] != 0) 	numtemp++;
		else break;
	}
	m_bHandCardCount=numtemp;//NORMAL_COUNT;//���е��˿���Ŀ	
	
	for (WORD i=0;i<GAME_PLAYER;i++) 
		m_bCardCount[i]=pSendCard->cbCardNum[i];

	//���ý���
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.SetLandScore(i,0);
		m_GameClientView.SetPassFlag(i,false);
		m_GameClientView.m_UserCardControl[i].SetCardData(NULL,0);
		m_GameClientView.m_HandCardControl[i].SetCardData(NULL,0);
	}

	m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].SetDisplayFlag(((IsLookonMode()==false)||(IsAllowLookon()==true)));

	//�����˿�
	for ( BYTE i = 0; i < GAME_PLAYER; ++i )
		if (i!=ME_VIEW_CHAIR)
			m_GameClientView.m_HandCardControl[i].SetCardSpace(8,8,0);

	//�˿�
	
	DispatchUserCard(m_bHandCardData,m_bHandCardCount);
	m_GameLogic.SortCardList(m_bHandCardData,m_bHandCardCount,ST_ORDER);//�����˿�

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
	m_bTurnCardCount=0;//������Ŀ
	m_bTurnOutType=CT_ERROR;//��������//��������
	ZeroMemory(m_bTurnCardData,sizeof(m_bTurnCardData));//�����б�

	//�رճɼ�
	if (m_GameClientView.m_ScoreView.m_hWnd!=NULL)
	{
		m_GameClientView.m_ScoreView.ShowWindow(SW_HIDE) ;
		m_GameClientView.m_ScoreView.DestroyWindow();
	}

	//���ÿؼ�
	m_GameClientView.ShowLandTitle(false);//��ʾ��ʾ

	//�ص���ť
	m_GameClientView.m_btLiGun.ShowWindow(SW_HIDE);
	m_GameClientView.m_btNotLiGun.ShowWindow(SW_HIDE);
	m_GameClientView.m_btJueGun.ShowWindow(SW_HIDE);
	m_GameClientView.m_btNotJueGun.ShowWindow(SW_HIDE);
	m_GameClientView.m_btAgree.ShowWindow(SW_HIDE);
	m_GameClientView.m_btDisagree.ShowWindow(SW_HIDE);

	//���ý���
	m_GameClientView.SetLandScore(INVALID_CHAIR,0);//�ڿӷ���

	m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].SetPositively( IsLookonMode()==false || IsAllowLookon() );

	//��ǰ���
	if ((IsLookonMode()==false)&&(pGameStart->wCurrentUser==GetMeChairID()))
	{
		ActiveGameFrame();
		m_GameClientView.m_btOutCard.EnableWindow(FALSE);
		m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btPassCard.EnableWindow(FALSE);
		m_GameClientView.m_btPassCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btAutoOutCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btAutoOutCard.EnableWindow(TRUE);

	}

	//�����˿�
	for ( BYTE i = 0; i < GAME_PLAYER; ++i )
		if (i!=ME_VIEW_CHAIR)
			m_GameClientView.m_HandCardControl[i].SetCardSpace(8,8,0);//���ü��

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

	m_GameClientView.m_btOneScore.ShowWindow(SW_HIDE);
	m_GameClientView.m_btOneScore.EnableWindow(FALSE);
	m_GameClientView.m_btTwoScore.ShowWindow(SW_HIDE);
	m_GameClientView.m_btTwoScore.EnableWindow(FALSE);
	m_GameClientView.m_btThreeScore.ShowWindow(SW_HIDE);
	m_GameClientView.m_btThreeScore.EnableWindow(FALSE);
	m_GameClientView.m_btGiveUpScore.ShowWindow(SW_HIDE);
	m_GameClientView.m_btGiveUpScore.EnableWindow(FALSE);
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
	m_bTurnOutType=m_GameLogic.GetCardType(pOutCard->bCardData,pOutCard->bCardCount);//��������
	CopyMemory(m_bTurnCardData,pOutCard->bCardData,sizeof(BYTE)*pOutCard->bCardCount);

	//ը���ж�
	if (m_bTurnOutType>=4)
	{
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
		m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].SetCardData(m_bHandCardData,m_bHandCardCount);
	}

	//��������	
	m_bCardCount[pOutCard->wOutCardUser]-=pOutCard->bCardCount;
	m_GameClientView.SetCardCount(wOutViewChairID,m_bCardCount[pOutCard->wOutCardUser]);
	for (WORD wChairID=0; wChairID<GAME_PLAYER; ++wChairID)
	{
		WORD wViewID = SwitchViewChairID(wChairID) ;
		if(ME_VIEW_CHAIR!=wViewID)
		{
			BYTE cbCardData[20] ;
			ZeroMemory(cbCardData, sizeof(cbCardData)) ;
			m_GameClientView.m_HandCardControl[wViewID].SetCardData(cbCardData,m_bCardCount[wChairID]);
		}
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

	if ((IsLookonMode()==true)||(GetMeChairID()!=pOutCard->wOutCardUser))
	{
		//ը���ж�
		if (/*(m_bTurnOutType==CT_BOMB_CARD)||*/(m_bTurnOutType==CT_MISSILE_CARD))
			PlayGameSound(AfxGetInstanceHandle(),TEXT("BOMB_CARD"));
		else
			PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));
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

	/////////////////////////////////////////////////
	m_GameClientView.m_btOneScore.ShowWindow(SW_HIDE);
	m_GameClientView.m_btOneScore.EnableWindow(FALSE);
	m_GameClientView.m_btTwoScore.ShowWindow(SW_HIDE);
	m_GameClientView.m_btTwoScore.EnableWindow(FALSE);
	m_GameClientView.m_btThreeScore.ShowWindow(SW_HIDE);
	m_GameClientView.m_btThreeScore.EnableWindow(FALSE);
	m_GameClientView.m_btGiveUpScore.ShowWindow(SW_HIDE);
	m_GameClientView.m_btGiveUpScore.EnableWindow(FALSE);
	/////////////////////////////////////////////////
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
	KillTimer(IDI_DISPATCH_CARD);

	//ֹͣ����
	DoncludeDispatchCard();

	//���ؿؼ�
	m_GameClientView.m_btOutCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btPassCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btOneScore.ShowWindow(SW_HIDE);
	m_GameClientView.m_btTwoScore.ShowWindow(SW_HIDE);
	m_GameClientView.m_btGiveUpScore.ShowWindow(SW_HIDE);
	m_GameClientView.m_btAutoOutCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btThreeScore.ShowWindow(SW_HIDE);
	m_GameClientView.m_btSortCard.EnableWindow(FALSE);

	////////////////////��ӵĽ��ð�ť�ؼ�/////////////////
	m_GameClientView.m_btLiGun.ShowWindow(SW_HIDE);
	m_GameClientView.m_btNotLiGun.ShowWindow(SW_HIDE);
	m_GameClientView.m_btJueGun.ShowWindow(SW_HIDE);
	m_GameClientView.m_btNotJueGun.ShowWindow(SW_HIDE);
	m_GameClientView.m_btAgree.ShowWindow(SW_HIDE);
	m_GameClientView.m_btDisagree.ShowWindow(SW_HIDE);

	//���ÿؼ�
	m_GameClientView.m_btOutCard.EnableWindow(FALSE);
	m_GameClientView.m_btPassCard.EnableWindow(FALSE);

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

	m_GameClientView.m_ScoreView.SetGameTax(pGameEnd->lGameTax);

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

		if (i!=ME_VIEW_CHAIR)
			m_GameClientView.m_HandCardControl[i].SetCardSpace(16,16,0);
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
		SetGameTimer(GetMeChairID(),IDI_START_GAME,30);

		//�йܹ���
		 m_GameClientView.m_btAutoPlayOn.ShowWindow(SW_SHOW);
		m_GameClientView.m_btAutoPlayOff.ShowWindow(SW_HIDE);
		OnAutoPlay( false, 0 );
	}
	m_GameClientView.ShowLandTitle(false);

#ifdef VIDEO_GAME
	//��ʾ�˿�
	//m_GameClientView.m_HandCardControl[ 0 ].ShowWindow( SW_SHOW );
	//m_GameClientView.m_HandCardControl[ 1 ].ShowWindow( SW_SHOW );
	//m_GameClientView.m_HandCardControl[ 3 ].ShowWindow( SW_SHOW );
#endif

	return true;
}

//�����ж�
bool CGameClientDlg::VerdictOutCard()
{
	//״̬�ж�
	if (m_GameClientView.m_btOutCard.IsWindowVisible()==FALSE) return false;

	//��ȡ�˿�
	BYTE bCardData[MAX_COUNT];
	BYTE bShootCount=(BYTE)m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].GetShootCard(bCardData,CountArray(bCardData));

	//�����ж�
	if (bShootCount>0L)
	{
		//��������
		BYTE bCardType=m_GameLogic.GetCardType(bCardData,bShootCount);//��������

		//�����ж�
		if (bCardType==CT_ERROR) return false;

		//�����ж�
		if (m_bTurnCardCount==0) return true;//������Ŀ
		//�����б�//�Լ��˿�����//������Ŀ//�Լ�������
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
		m_GameClientView.m_UserCardControl[ME_VIEW_CHAIR].SetCardData(OutCard.bCardData,OutCard.bCardCount);

		//Ԥ��ɾ��
		BYTE bSourceCount=m_bHandCardCount;
		m_bHandCardCount-=OutCard.bCardCount;
		m_GameLogic.RemoveCard(OutCard.bCardData,OutCard.bCardCount,m_bHandCardData,bSourceCount);
		m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].SetCardData(m_bHandCardData,m_bHandCardCount);
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
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			WORD wViewChairID = SwitchViewChairID(i) ;
			if (wViewChairID!=ME_VIEW_CHAIR)
			{
				BYTE cbCardData[20] ;
				ZeroMemory(cbCardData, sizeof(cbCardData)) ;
				m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(cbCardData,NORMAL_COUNT);
			}
			else m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].SetCardData(m_bHandCardData,m_bHandCardCount); 
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
	m_cbDispatchCardCount=0;//�ɷ���Ŀ
	m_cbRemnantCardCount=cbCardCount;//ʣ����Ŀ
	CopyMemory(m_cbDispatchCardData,cbCardData,sizeof(BYTE)*cbCardCount);//�ɷ��˿�

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
			BYTE cbUserCard[NORMAL_COUNT];
			ZeroMemory(cbUserCard,sizeof(cbUserCard));

			//�ɷ��˿�
			m_cbRemnantCardCount--;
			m_cbDispatchCardCount++;
			m_GameLogic.SortCardList(m_cbDispatchCardData,m_cbDispatchCardCount,ST_ORDER);

			//���ý���
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				WORD wViewID = SwitchViewChairID(i) ;
				if (wViewID!=ME_VIEW_CHAIR)
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

			}
			//���ý���
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				WORD wViewID = SwitchViewChairID(i) ;
				if (wViewID!=ME_VIEW_CHAIR)
				{
					BYTE cbCardData[20] ;
					ZeroMemory(cbCardData, sizeof(cbCardData)) ;
					m_GameClientView.m_HandCardControl[wViewID].SetCardData(cbCardData,NORMAL_COUNT);
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

			////////////////////////////////////////////////��ӵĴ���//////////////////////////////////////////
			//����������ť
			m_GameClientView.m_btLiGun.ShowWindow(SW_SHOW);
			m_GameClientView.m_btLiGun.EnableWindow(TRUE);
			m_GameClientView.m_btNotLiGun.ShowWindow(SW_SHOW);
			m_GameClientView.m_btNotLiGun.EnableWindow(TRUE);
			SetGameTimer(GetMeChairID(), IDI_LI_GUN, 10);
			////////////////////////////////////////////////////////////////////////////////////////////////////

			//����ʱ��
			//if (m_wCurrentUser!=INVALID_CHAIR) SetGameTimer(m_wCurrentUser,IDI_LAND_SCORE,30);
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
	m_GameClientView.ShowLandTitle(false);
	m_GameClientView.SetBombTime(m_wBombTime);
	m_GameClientView.SetCardCount(INVALID_CHAIR,0);
	m_GameClientView.SetLandUser(INVALID_CHAIR,0);
	m_GameClientView.SetLandScore(INVALID_CHAIR,0);
	m_GameClientView.SetPassFlag(INVALID_CHAIR,false);

	//���ؿؼ�
	m_GameClientView.m_btStart.ShowWindow(FALSE);
	if (m_GameClientView.m_ScoreView.m_hWnd!=NULL) m_GameClientView.m_ScoreView.DestroyWindow();

	//�����˿�
	//m_GameClientView.m_BackCardControl.SetCardData(NULL,0);
	m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].SetPositively(false);
	for (WORD i=0;i<GAME_PLAYER;i++) 
	{
		m_GameClientView.m_UserCardControl[i].SetCardData(NULL,0);
		m_GameClientView.m_HandCardControl[i].SetCardData(NULL,0);
		WORD wViewID = SwitchViewChairID(i) ;
		if(wViewID==ME_VIEW_CHAIR)	m_GameClientView.m_HandCardControl[wViewID].SetDisplayFlag(true);
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
	OutCard.bCardCount=(BYTE)m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].GetShootCard(OutCard.bCardData,CountArray(OutCard.bCardData));
	m_GameLogic.SortCardList( OutCard.bCardData, OutCard.bCardCount, ST_ORDER);
	SendData(SUB_C_OUT_CART,&OutCard,sizeof(OutCard)-sizeof(OutCard.bCardData)+OutCard.bCardCount*sizeof(BYTE));

	//Ԥ����ʾ
	PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));
	m_GameClientView.m_UserCardControl[ME_VIEW_CHAIR].SetCardData(OutCard.bCardData,OutCard.bCardCount);

	//Ԥ��ɾ��
	BYTE bSourceCount=m_bHandCardCount;
	m_bHandCardCount-=OutCard.bCardCount;
	m_GameLogic.RemoveCard(OutCard.bCardData,OutCard.bCardCount,m_bHandCardData,bSourceCount);
	m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].SetCardData(m_bHandCardData,m_bHandCardCount);

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
	m_GameClientView.SetPassFlag(ME_VIEW_CHAIR,true);
	PlayGameSound(AfxGetInstanceHandle(),TEXT("PASS_CARD"));
	m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].SetCardData(m_bHandCardData,m_bHandCardCount);

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
		m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].SetShootCard(OutCardResult.cbResultCard,OutCardResult.cbCardCount);

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

	bool bChaCard=VerdictChaCard();
	m_GameClientView.m_btOneScore.EnableWindow(bChaCard?TRUE:FALSE);

	bool bDianCard=VerdictDianCard();
	m_GameClientView.m_btThreeScore.EnableWindow(bDianCard?TRUE:FALSE);

	//CString ss;
	//ss.Format( TEXT( " %d" ), bChaCard);
	//InsertShtickString( ss, RGB( 255, 0, 0 ), true );
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


LRESULT CGameClientDlg::OnMessageSortCard(WPARAM wParam, LPARAM lParam)
{

	//�����˿�
	WORD wMeChairID=GetMeChairID();
	m_GameLogic.SortCardList(m_bHandCardData,m_bHandCardCount,m_cbSortType);

	//��ȡ�˿�
	BYTE cbShootCount=(BYTE)m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].SetCardData(m_bHandCardData,m_bHandCardCount);

	//���ñ���
	m_cbSortType=(m_cbSortType==ST_ORDER)?ST_COUNT:ST_ORDER;

	//���ð�ť
	UINT uImageID=(m_cbSortType==ST_ORDER)?IDB_ORDER_SORT:IDB_COUNT_SORT;
	m_GameClientView.m_btSortCard.SetButtonImage(uImageID,AfxGetInstanceHandle(),false);

	//���½���
	m_GameClientView.UpdateGameView(NULL);

	return 0;
}
//�û�����
bool CGameClientDlg::OnSubChaCard(const void * pBuffer, WORD wDataSize)
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
	m_bTurnOutType=m_GameLogic.GetCardType(pOutCard->bCardData,pOutCard->bCardCount);//��������
	CopyMemory(m_bTurnCardData,pOutCard->bCardData,sizeof(BYTE)*pOutCard->bCardCount);

	//�Լ��˿�
	if ((IsLookonMode()==true)&&(pOutCard->wOutCardUser==GetMeChairID()))
	{
		//ɾ���˿� 
		BYTE bSourceCount=m_bHandCardCount;
		m_bHandCardCount-=pOutCard->bCardCount;
		m_GameLogic.RemoveCard(pOutCard->bCardData,pOutCard->bCardCount,m_bHandCardData,bSourceCount);

		//���ý���
		m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].SetCardData(m_bHandCardData,m_bHandCardCount);
	}

	//��������	
	m_bCardCount[pOutCard->wOutCardUser]-=pOutCard->bCardCount;//�˿���Ŀ
	m_GameClientView.SetCardCount(wOutViewChairID,m_bCardCount[pOutCard->wOutCardUser]);
	for (WORD wChairID=0; wChairID<GAME_PLAYER; ++wChairID)
	{
		WORD wViewID = SwitchViewChairID(wChairID) ;
		if(ME_VIEW_CHAIR!=wViewID)
		{
			BYTE cbCardData[20] ;
			ZeroMemory(cbCardData, sizeof(cbCardData)) ;
			m_GameClientView.m_HandCardControl[wViewID].SetCardData(cbCardData,m_bCardCount[wChairID]);
		}
	}

	if (pOutCard->wCurrentUser!=INVALID_CHAIR)//��ǰ���
	{
		WORD wViewChairID=SwitchViewChairID(pOutCard->wCurrentUser);
		m_GameClientView.SetPassFlag(wViewChairID,false);
		m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(NULL,0);
	}

	//�������
	if ((IsLookonMode()==false)&&(pOutCard->wCurrentUser==GetMeChairID()))
	{
		ActiveGameFrame();
		//������ʾ�Ͳ���
		m_GameClientView.m_btOneScore.ShowWindow(SW_SHOW);
		m_GameClientView.m_btOneScore.EnableWindow((VerdictChaCard()==true)?TRUE:FALSE);
		m_GameClientView.m_btTwoScore.ShowWindow(SW_SHOW);
		m_GameClientView.m_btTwoScore.EnableWindow(TRUE);
	}

	//����ʱ��
	if (pOutCard->wCurrentUser!=INVALID_CHAIR)
	{
		BYTE bCardCount=m_bCardCount[pOutCard->wCurrentUser];
		SetGameTimer(pOutCard->wCurrentUser,IDI_CHA_CARD,10);
	}

	
	return true;
}
//������Ϣ
LRESULT CGameClientDlg::OnCha(WPARAM wParam, LPARAM lParam)
{
	//״̬�ж�
	if ((m_GameClientView.m_btOneScore.IsWindowEnabled()==FALSE)||
		(m_GameClientView.m_btOneScore.IsWindowVisible()==FALSE)) return 0;

	//���ñ���
	m_wTimeOutCount=0;

	//���ý���
	KillGameTimer(IDI_CHA_CARD);
	m_GameClientView.m_btOneScore.ShowWindow(SW_HIDE);
	m_GameClientView.m_btOneScore.EnableWindow(FALSE);
	m_GameClientView.m_btTwoScore.ShowWindow(SW_HIDE);
	m_GameClientView.m_btTwoScore.EnableWindow(FALSE);

	//��������
	CMD_C_OutCard OutCard;
	OutCard.bCardCount=(BYTE)m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].GetShootCard(OutCard.bCardData,CountArray(OutCard.bCardData));
	m_GameLogic.SortCardList( OutCard.bCardData, OutCard.bCardCount, ST_ORDER);
	SendData(SUB_C_GAME_CHA,&OutCard,sizeof(OutCard)-sizeof(OutCard.bCardData)+OutCard.bCardCount*sizeof(BYTE));

	//Ԥ����ʾ
	PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));
	m_GameClientView.m_UserCardControl[ME_VIEW_CHAIR].SetCardData(OutCard.bCardData,OutCard.bCardCount);
	
	//Ԥ��ɾ��
	BYTE bSourceCount=m_bHandCardCount;
	m_bHandCardCount-=OutCard.bCardCount;
	m_GameLogic.RemoveCard(OutCard.bCardData,OutCard.bCardCount,m_bHandCardData,bSourceCount);
	m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].SetCardData(m_bHandCardData,m_bHandCardCount);

	return 0;
}

//����
LRESULT CGameClientDlg::OnNoCha(WPARAM wParam, LPARAM lParam)
{
	//״̬�ж�
	if (m_GameClientView.m_btTwoScore.IsWindowEnabled()==FALSE) return 0;

	//���ñ���
	m_wTimeOutCount=0;

	//���ý���
	KillGameTimer(IDI_CHA_CARD);
	KillGameTimer(IDI_OUT_CARD);
	m_GameClientView.m_btOneScore.ShowWindow(SW_HIDE);
	m_GameClientView.m_btOneScore.EnableWindow(FALSE);
	m_GameClientView.m_btTwoScore.ShowWindow(SW_HIDE);
	m_GameClientView.m_btTwoScore.EnableWindow(FALSE);

	//��������
	SendData(SUB_C_GAME_NOCHA);

	//Ԥ����ʾ
	//m_GameClientView.SetPassFlag(ME_VIEW_CHAIR,true);
	PlayGameSound(AfxGetInstanceHandle(),TEXT("PASS_CARD"));
	m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].SetCardData(m_bHandCardData,m_bHandCardCount);

	return 0;
}
//������Ϣ
LRESULT CGameClientDlg::OnDian(WPARAM wParam, LPARAM lParam)
{
	//״̬�ж�
	if ((m_GameClientView.m_btThreeScore.IsWindowEnabled()==FALSE)||
		(m_GameClientView.m_btThreeScore.IsWindowVisible()==FALSE)) return 0;

	//���ñ���
	m_wTimeOutCount=0;

	//���ý���
	KillGameTimer(IDI_DIAN_CARD);
	m_GameClientView.m_btThreeScore.ShowWindow(SW_HIDE);
	m_GameClientView.m_btThreeScore.EnableWindow(FALSE);
	m_GameClientView.m_btGiveUpScore.ShowWindow(SW_HIDE);
	m_GameClientView.m_btGiveUpScore.EnableWindow(FALSE);

	//��������
	CMD_C_OutCard OutCard;
	OutCard.bCardCount=(BYTE)m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].GetShootCard(OutCard.bCardData,CountArray(OutCard.bCardData));
	m_GameLogic.SortCardList( OutCard.bCardData, OutCard.bCardCount, ST_ORDER);
	SendData(SUB_C_GAME_DIAN,&OutCard,sizeof(OutCard)-sizeof(OutCard.bCardData)+OutCard.bCardCount*sizeof(BYTE));

	//Ԥ����ʾ
	PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));
	m_GameClientView.m_UserCardControl[ME_VIEW_CHAIR].SetCardData(OutCard.bCardData,OutCard.bCardCount);

	//Ԥ��ɾ��
	BYTE bSourceCount=m_bHandCardCount;
	m_bHandCardCount-=OutCard.bCardCount;
	m_GameLogic.RemoveCard(OutCard.bCardData,OutCard.bCardCount,m_bHandCardData,bSourceCount);
	m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].SetCardData(m_bHandCardData,m_bHandCardCount);

	return 0;
}

//����
LRESULT CGameClientDlg::OnNoDian(WPARAM wParam, LPARAM lParam)
{
	//״̬�ж�
	if (m_GameClientView.m_btGiveUpScore.IsWindowEnabled()==FALSE) return 0;

	//���ñ���
	m_wTimeOutCount=0;

	//���ý���
	KillGameTimer(IDI_DIAN_CARD);
	KillGameTimer(IDI_OUT_CARD);
	m_GameClientView.m_btThreeScore.ShowWindow(SW_HIDE);
	m_GameClientView.m_btThreeScore.EnableWindow(FALSE);
	m_GameClientView.m_btGiveUpScore.ShowWindow(SW_HIDE);
	m_GameClientView.m_btGiveUpScore.EnableWindow(FALSE);


	//��������
	SendData(SUB_C_GAME_NODIAN);

	//Ԥ����ʾ
	//m_GameClientView.SetPassFlag(ME_VIEW_CHAIR,true);
	PlayGameSound(AfxGetInstanceHandle(),TEXT("PASS_CARD"));
	m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].SetCardData(m_bHandCardData,m_bHandCardCount);

	return 0;
}
//�û�����
bool CGameClientDlg::OnSubDianCard(const void * pBuffer, WORD wDataSize)
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
	m_bTurnOutType=m_GameLogic.GetCardType(pOutCard->bCardData,pOutCard->bCardCount);//��������
	CopyMemory(m_bTurnCardData,pOutCard->bCardData,sizeof(BYTE)*pOutCard->bCardCount);

	//�Լ��˿�
	if ((IsLookonMode()==true)&&(pOutCard->wOutCardUser==GetMeChairID()))
	{
		//ɾ���˿� 
		BYTE bSourceCount=m_bHandCardCount;
		m_bHandCardCount-=pOutCard->bCardCount;
		m_GameLogic.RemoveCard(pOutCard->bCardData,pOutCard->bCardCount,m_bHandCardData,bSourceCount);

		//���ý���
		m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].SetCardData(m_bHandCardData,m_bHandCardCount);
	}

	//��������	
	m_bCardCount[pOutCard->wOutCardUser]-=pOutCard->bCardCount;//�˿���Ŀ
	m_GameClientView.SetCardCount(wOutViewChairID,m_bCardCount[pOutCard->wOutCardUser]);
	for (WORD wChairID=0; wChairID<GAME_PLAYER; ++wChairID)
	{
		WORD wViewID = SwitchViewChairID(wChairID) ;
		if(ME_VIEW_CHAIR!=wViewID)
		{
			BYTE cbCardData[20] ;
			ZeroMemory(cbCardData, sizeof(cbCardData)) ;
			m_GameClientView.m_HandCardControl[wViewID].SetCardData(cbCardData,m_bCardCount[wChairID]);
		}
	}

	if (pOutCard->wCurrentUser!=INVALID_CHAIR)//��ǰ���
	{
		WORD wViewChairID=SwitchViewChairID(pOutCard->wCurrentUser);
		m_GameClientView.SetPassFlag(wViewChairID,false);
		m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(NULL,0);
	}

	//�������
	if ((IsLookonMode()==false)&&(pOutCard->wCurrentUser==GetMeChairID()))
	{
		ActiveGameFrame();
		//������ʾ�Ͳ���
		m_GameClientView.m_btThreeScore.ShowWindow(SW_SHOW);
		m_GameClientView.m_btThreeScore.EnableWindow((VerdictChaCard()==true)?TRUE:FALSE);
		m_GameClientView.m_btGiveUpScore.ShowWindow(SW_SHOW);
		m_GameClientView.m_btGiveUpScore.EnableWindow(TRUE);
	}

	//����ʱ��
	if (pOutCard->wCurrentUser!=INVALID_CHAIR)
	{
		BYTE bCardCount=m_bCardCount[pOutCard->wCurrentUser];
		SetGameTimer(pOutCard->wCurrentUser,IDI_DIAN_CARD,10);
	}

	return true;
}
//�����ж�
bool CGameClientDlg::VerdictChaCard()
{
	//״̬�ж�
	if (m_GameClientView.m_btOneScore.IsWindowVisible()==FALSE) return false;

	//��ȡ�˿�
	BYTE bCardData[MAX_COUNT];
	BYTE bShootCount=(BYTE)m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].GetShootCard(bCardData,CountArray(bCardData));
	//�����ж�
	if (bShootCount>0L)
	{
		//��������
		BYTE bCardType=m_GameLogic.GetCardType(bCardData,bShootCount);//��������

		//�����ж�
		if (bCardType==CT_ERROR) return false;

		//�����ж�
		if (m_bTurnCardCount==0) return true;//������Ŀ

		//�����б�//�Լ��˿�����//������Ŀ//�Լ�������
		return m_GameLogic.CompareChaCard(m_bTurnCardData,bCardData,m_bTurnCardCount,bShootCount);
	}
	return false;
}
//�����ж�
bool CGameClientDlg::VerdictDianCard()
{
	//״̬�ж�
	if (m_GameClientView.m_btThreeScore.IsWindowVisible()==FALSE) return false;

	//��ȡ�˿�
	BYTE bCardData[MAX_COUNT];
	BYTE bShootCount=(BYTE)m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].GetShootCard(bCardData,CountArray(bCardData));
	//�����ж�
	if (bShootCount>0L)
	{
		//��������
		BYTE bCardType=m_GameLogic.GetCardType(bCardData,bShootCount);//��������

		//�����ж�
		if (bCardType==CT_ERROR) return false;

		//�����ж�
		if (m_bTurnCardCount==0) return true;//������Ŀ

		//�����б�//�Լ��˿�����//������Ŀ//�Լ�������
		return m_GameLogic.CompareDianCard(m_bTurnCardData,bCardData,m_bTurnCardCount,bShootCount);
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////////////
LRESULT CGameClientDlg::OnHitLiGun(WPARAM wParam, LPARAM lParam)
{
	m_GameClientView.m_btLiGun.ShowWindow(SW_HIDE);

	CMD_C_LiGun LiGun;
	ZeroMemory(&LiGun, sizeof(LiGun));
	LiGun.wLiGunUserChairID = GetMeChairID();
	SendData(SUB_C_LI_GUN, &LiGun, sizeof(LiGun));

	return 0;
}

LRESULT CGameClientDlg::OnHitJueGun(WPARAM wParam, LPARAM lParam)
{
	m_GameClientView.m_btJueGun.ShowWindow(SW_HIDE);
	m_GameClientView.m_btNotJueGun.ShowWindow(SW_HIDE);

	CMD_C_LiGun JueGun;
	ZeroMemory(&JueGun, sizeof(JueGun));
	JueGun.wLiGunUserChairID = GetMeChairID();

	SendData(SUB_C_JUE_GUN, &JueGun, sizeof(JueGun));

	return 0;
}


LRESULT CGameClientDlg::OnHitAgree(WPARAM wParam, LPARAM lParam)
{
	m_GameClientView.m_btAgree.ShowWindow(SW_HIDE);
	m_GameClientView.m_btDisagree.ShowWindow(SW_HIDE);
	BYTE cbAgree = (BYTE)wParam;
	CMD_C_LiGunAnswer LiGunAnswer;
	ZeroMemory(&LiGunAnswer, sizeof(LiGunAnswer));
	LiGunAnswer.cbAnswer = (BYTE)wParam;
	LiGunAnswer.wAgreeChairID = GetMeChairID();
	LiGunAnswer.cbLiOrJue = m_cbLiOrJue;

	SendData(SUB_C_LI_GUN_ANSWER, &LiGunAnswer, sizeof(LiGunAnswer));

	//ͬ���������߾������ô��ǰ��Ҳ�������Ϸ�����е���Ԥ��ʾΪ����
	if (cbAgree == 1)
	{
		memset(m_cbLiOrJueCardData, 0, sizeof(m_cbLiOrJueCardData));
		m_GameClientView.m_HandCardControl[2].SetCardData(m_cbLiOrJueCardData, m_bHandCardCount);
	}

	return 0;
}

LRESULT CGameClientDlg::OnHitNotLiGun(WPARAM wParam, LPARAM lParam)
{
	m_GameClientView.m_btLiGun.ShowWindow(SW_HIDE);
	m_GameClientView.m_btJueGun.ShowWindow(SW_HIDE);
	m_GameClientView.m_btNotLiGun.ShowWindow(SW_HIDE);
	m_GameClientView.m_btNotJueGun.ShowWindow(SW_HIDE);
	m_GameClientView.m_btAgree.ShowWindow(SW_HIDE);
	m_GameClientView.m_btDisagree.ShowWindow(SW_HIDE);

	CMD_C_NotLiOrJue NotLiOrJue;
	ZeroMemory(&NotLiOrJue, sizeof(NotLiOrJue));
	NotLiOrJue.wNotLiOrJueChair = GetMeChairID();

	SendData(SUB_C_NOT_LI_GUN, &NotLiOrJue, sizeof(NotLiOrJue));

	return 0;
}

LRESULT CGameClientDlg::OnHitNotJueGun(WPARAM wParam, LPARAM lParam)
{
	m_GameClientView.m_btLiGun.ShowWindow(SW_HIDE);
	m_GameClientView.m_btJueGun.ShowWindow(SW_HIDE);
	m_GameClientView.m_btNotLiGun.ShowWindow(SW_HIDE);
	m_GameClientView.m_btNotJueGun.ShowWindow(SW_HIDE);
	m_GameClientView.m_btAgree.ShowWindow(SW_HIDE);
	m_GameClientView.m_btDisagree.ShowWindow(SW_HIDE);

	CMD_C_NotLiOrJue NotLiOrJue;
	ZeroMemory(&NotLiOrJue, sizeof(NotLiOrJue));
	NotLiOrJue.wNotLiOrJueChair = GetMeChairID();

	SendData(SUB_C_NOT_JUE_GUN, &NotLiOrJue, sizeof(NotLiOrJue));

	return 0;
}