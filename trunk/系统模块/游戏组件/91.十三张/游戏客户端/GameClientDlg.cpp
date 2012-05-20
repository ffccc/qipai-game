#include "Stdafx.h"
#include "GameClient.h"
#include "GameClientDlg.h"
#include "GameOption.h"
#include "SetChip.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientDlg, CGameFrameDlg)
		ON_MESSAGE(IDM_START,OnStart)
		ON_MESSAGE(IDM_SETCHIP,OnSetChip)
		ON_MESSAGE(IDM_LEFT_HIT_CARD,OnLeftHitCard)
		ON_MESSAGE(IDM_FRONTCARD , OnFrontCard)
		ON_MESSAGE(IDM_BACKCARD , OnBackCard)
		ON_MESSAGE(IDM_MIDCARD , OnMidCard)
		ON_MESSAGE(IDM_SHOWCARD , OnShowCard)
		ON_MESSAGE(IDM_DESCEND_SORT , OnDescendSort)
		ON_MESSAGE(IDM_ASCEND_SORT , OnAscendSort)
		ON_MESSAGE(IDM_COLOR_SORT , OnColorSort)
		ON_MESSAGE(IDM_AUTO_OUT_CARD,OnAutoOutCard)
		ON_MESSAGE( IDM_NO_CALL_BANKER, OnNoCallBanker )
		ON_MESSAGE( IDM_CALL_BANKER, OnCallBanker )
        ON_WM_TIMER()
		ON_MESSAGE(IDM_RESTORE_CARD, OnRestoreCard)
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////
int						CGameClientDlg::m_bCompareTime=2000;						//比较时间
//int					CGameClientDlg::m_bCompareTime=1;							//比较时间

#ifdef _DEBUG
const int				g_nGameTime = 90 ;
bool					g_bNewStart = true ;
#endif
/////////////////////////////////////////////////////////////////////////////////
//宏定义

//结束原因
#define GER_NORMAL						0x00							//常规结束
#define GER_DISMISS						0x01							//游戏解散
#define GER_USER_LEFT					0x02							//用户离开

//游戏定时器
#define IDI_START_GAME					200								//开始定时器
#define IDI_SHOW_CARD					201								//摊牌定时器
#define IDI_COMPARE_FRONT				202								//比较定时器
#define IDI_COMPARE_MID					203								//比较定时器
#define IDI_COMPARE_BACK				204								//比较定时器
#define IDI_COMPLETE_COMPARE			205								//完成定时器
#define IDI_SETCHIP						206								//下注定时器
#define IDI_CALL_BANKER					207								//叫庄定时器

#define IDT_START_TIME					30								//开始时间
#define IDT_CALL_BANKER_TIME			25								//叫庄时间
////////////////////////////////////////////////////////////////////////////////////

//构造函数
CGameClientDlg::CGameClientDlg() : CGameFrameDlg(&m_GameClientView)
{
	//游戏变量
	m_wBanker =	INVALID_CHAIR ; 
	for(WORD i=0 ; i<GAME_PLAYER ; ++i) m_nChip[i] = -1 ;
	m_dwCardHSpace=20 ;
	m_bHandCardCount=0 ;
	ZeroMemory(m_bSegmentCard , sizeof(m_bSegmentCard)) ;
	m_wMaxChip=10 ;
	m_bFinishSegment=false ;
	m_bShowCardTime=90 ;
	m_bReady = false ;
	m_bCanSetChip = true ;

	return;
}

//析构函数
CGameClientDlg::~CGameClientDlg()
{
}

//初始函数
bool CGameClientDlg::InitGameFrame()
{
	//设置标题
	SetWindowText(TEXT("十三张  --  乾元网络开发"));

	//设置图标
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);

	//配置控件
	m_GameClientView.m_UserCardControl[2].SetCardSpace(m_dwCardHSpace,20,20);

	return true;
}

//重置框架
void CGameClientDlg::ResetGameFrame()
{
	//游戏变量
	m_wBanker =	INVALID_CHAIR ;
	for(WORD i=0 ; i<GAME_PLAYER ; ++i) m_nChip[i] = -1 ;
	m_bHandCardCount=0 ;
	ZeroMemory(m_bSegmentCard , sizeof(m_bSegmentCard)) ;
	ZeroMemory(m_bHandCardData , sizeof(m_bHandCardData)) ;
	ZeroMemory(m_bTmpHandCardData , sizeof(m_bTmpHandCardData)) ;
	m_wMaxChip=10 ;
	m_bShowCardTime=90 ;
	m_bReady = false ;

	//设置扑克
	m_GameClientView.SetCardData(INVALID_CHAIR , NULL , 0 , enHandCard) ;
	m_GameClientView.SetCardData(INVALID_CHAIR , NULL , 0 , enSegmentCard) ;
	m_GameClientView.SetCardData(INVALID_CHAIR , NULL , 0 , enAllSegmentCard) ;
	m_GameClientView.SetCardData(INVALID_CHAIR , NULL , 0 , enCompareCard) ;
	m_GameClientView.SetCardData(INVALID_CHAIR , NULL , 0 , enBankCard) ;

	//清空扑克
	for (WORD i=0;i<GAME_PLAYER;i++) m_GameClientView.m_UserCardControl[i].SetCardData(NULL,0) ;

	m_GameClientView.SetBanker(INVALID_CHAIR) ;

	m_GameClientView.SetChip(INVALID_CHAIR , 0) ;

	return;
}

//游戏设置
void CGameClientDlg::OnGameOptionSet()
{	//构造数据

	if(false==IsLookonMode())
	{
		CGameOption GameOption;
		GameOption.m_dwCardHSpace=m_dwCardHSpace;
		GameOption.m_bEnableSound=IsEnableSound();
		GameOption.m_bCompareTime=m_bCompareTime/1000 ;

		//配置数据
		if (GameOption.DoModal()==IDOK)
		{
			//获取参数
			m_dwCardHSpace=GameOption.m_dwCardHSpace;

			//设置控件
			EnableSound(GameOption.m_bEnableSound);
			m_GameClientView.m_UserCardControl[2].SetCardSpace(m_dwCardHSpace,0,20);

			m_bCompareTime=GameOption.m_bCompareTime*1000 ;

			if(GameOption.m_bCompareTime<1 || GameOption.m_bCompareTime>10) 
				m_bCompareTime=0 ;
	
			if(GetMeChairID()==m_wBanker && GetGameStatus()!=GS_WK_PLAYING && false==IsLookonMode() && false==m_bReady && true==m_bCanSetChip)
			{
				CMD_C_ShowCardTime ShowCardTime ;
				memset(&ShowCardTime , 0 , sizeof(ShowCardTime)) ;
				SendData(SEB_C_SHOW_CARD_TIME , &ShowCardTime , sizeof(ShowCardTime)) ;
			}


			//保存配置
			AfxGetApp()->WriteProfileInt(TEXT("GameOption"),TEXT("CardSpace"),m_dwCardHSpace);
		}
	}

	return;
}

//时间消息
bool CGameClientDlg::OnTimerMessage(WORD wChairID, UINT nElapse, UINT nTimerID)
{
	switch (nTimerID)
	{
	case IDI_SETCHIP:			//设置下注
		{
			if (nElapse==0)
			{
				if ((IsLookonMode()==false)&&(wChairID==GetMeChairID()))
				{
					//最大下注
					LONG lChipsArray[SET_CHIP_COUNT] ;

					GetChips( lChipsArray ) ;

					OnSetChip( lChipsArray[0], 0 ) ;
				}

				CSetChip *pSetChip = (CSetChip *)FindWindow(NULL , TEXT("压注")) ;

				if( pSetChip )
				{
					pSetChip->OnBnClickedCancel();
				}

				return false;
			}

			if ((nElapse<=10)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false)) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));

			return true;
		}
	case IDI_START_GAME:		//开始游戏
		{
			if (nElapse==0)
			{
				if ((IsLookonMode()==false)&&(wChairID==GetMeChairID())) PostMessage( WM_CLOSE, 0, 0 ) ;
				return false;
			}
			if ((nElapse<=10)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false)) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));

			return true;
		}
	case IDI_SHOW_CARD:        //玩家亮牌
		{
			if (nElapse==0)
			{
#ifdef _DEBUG
				CFile file ;
				CString name ;
				name.Format("%ld.txt" , GetMeChairID()) ;
				file.Open(name , CFile::modeWrite | CFile::modeNoTruncate | CFile::modeCreate) ;
				file.SeekToEnd() ;
				file.Write("\r\n\r\n" , (UINT)strlen("\r\n\r\n")) ;
				file.Write("case IDI_SHOW_CARD" , (UINT)(strlen("case IDI_SHOW_CARD"))) ;
				file.Close() ;
#endif
				if ((IsLookonMode()==false)&&(wChairID==GetMeChairID())) 
				{

#ifdef _DEBUG
					CFile file ;
					CString name ;
					name.Format("%ld.txt" , GetMeChairID()) ;
					file.Open(name , CFile::modeWrite | CFile::modeNoTruncate | CFile::modeCreate) ;
					file.SeekToEnd() ;
					file.Write("\r\n\r\n" , (UINT)strlen("\r\n\r\n")) ;
					file.Write("if ((IsLookonMode()==false)&&(wChairID==GetMeChairID())) " , (UINT)(strlen("if ((IsLookonMode()==false)&&(wChairID==GetMeChairID())) "))) ;
					file.Close() ;
#endif
					m_GameClientView.m_btShowCard.ShowWindow(SW_HIDE) ;
					m_GameClientView.m_btShowCard.EnableWindow(FALSE) ;

					m_GameClientView.m_btAutoOutCard.ShowWindow(SW_HIDE) ;
					m_GameClientView.m_btAutoOutCard.EnableWindow(FALSE) ;

					m_GameClientView.m_btSetChip[0].ShowWindow(SW_HIDE) ;
					m_GameClientView.m_btSetChip[0].EnableWindow(FALSE) ;
					m_GameClientView.m_btSetChip[1].ShowWindow(SW_HIDE) ;
					m_GameClientView.m_btSetChip[1].EnableWindow(FALSE) ;
					m_GameClientView.m_btSetChip[2].ShowWindow(SW_HIDE) ;
					m_GameClientView.m_btSetChip[2].EnableWindow(FALSE) ;
					m_GameClientView.m_btSetChip[3].ShowWindow(SW_HIDE) ;
					m_GameClientView.m_btSetChip[3].EnableWindow(FALSE) ;


					m_GameClientView.m_btFrontCard.ShowWindow(SW_HIDE) ;
					m_GameClientView.m_btFrontCard.EnableWindow(FALSE) ;
					m_GameClientView.m_btMidCard.ShowWindow(SW_HIDE) ;
					m_GameClientView.m_btMidCard.EnableWindow(FALSE) ;
					m_GameClientView.m_btBackCard.ShowWindow(SW_HIDE) ;
					m_GameClientView.m_btBackCard.EnableWindow(FALSE) ;

#ifdef _DEBUG
					name.Format("%ld.txt" , GetMeChairID()) ;
					file.Open(name , CFile::modeWrite | CFile::modeNoTruncate | CFile::modeCreate) ;
					file.SeekToEnd() ;
					file.Write("\r\n\r\n" , (UINT)strlen("\r\n\r\n")) ;
					file.Write("before AutomaticOutCard" , (UINT)(strlen("before AutomaticOutCard"))) ;
					file.Close() ;
#endif

					AutomaticOutCard() ;			
					//摊牌
#ifdef _DEBUG
					name.Format("%ld.txt" , GetMeChairID()) ;
					file.Open(name , CFile::modeWrite | CFile::modeNoTruncate | CFile::modeCreate) ;
					file.SeekToEnd() ;
					file.Write("\r\n\r\n" , (UINT)strlen("\r\n\r\n")) ;
					file.Write("fater AutomaticOutCard" , (UINT)(strlen("fater AutomaticOutCard"))) ;
					file.Close() ;
#endif
					OnShowCard(0 , 0) ;
				}

                return false;
			}
			if ((nElapse<=10)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false)) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));

			return true ;
		}
	case IDI_CALL_BANKER :	//玩家叫庄
		{
			if ( nElapse == 0 && !IsLookonMode() && wChairID == GetMeChairID() )
				OnNoCallBanker( 0, 0 ) ;				

			if ( ( nElapse <= 10 ) && ( wChairID == GetMeChairID() ) && ( IsLookonMode() == false ) )
				PlayGameSound( AfxGetInstanceHandle(), TEXT( "GAME_WARN" ) ) ;

			return true ;
		}
		
	}

	return false;
}

//旁观状态
void CGameClientDlg::OnLookonChanged(bool bLookonUser, const void * pBuffer, WORD wDataSize)
{
}

//网络消息
bool CGameClientDlg::OnGameMessage(WORD wSubCmdID, const void * pBuffer, WORD wDataSize)
{	
	switch (wSubCmdID)
	{
	case SUB_S_START_CALL_BANKER:		//开始叫庄
		{
			//数据验证
			ASSERT( sizeof( CMD_S_StartCallBanker ) == wDataSize ) ;
			if ( sizeof( CMD_S_StartCallBanker ) != wDataSize ) return false ;

			CMD_S_StartCallBanker  *pStartCallBanker = ( CMD_S_StartCallBanker* )pBuffer ;

			//叫庄时间
			SetGameTimer( pStartCallBanker->wCurrentCaller, IDI_CALL_BANKER, IDT_CALL_BANKER_TIME ) ;
			
			//设置按钮
			if ( GetMeChairID() == pStartCallBanker->wCurrentCaller && !IsLookonMode() )
			{
				m_GameClientView.m_btCallBanker.ShowWindow( SW_SHOW ) ;
				m_GameClientView.m_btNoCallBanker.ShowWindow( SW_SHOW ) ;
			}

			//闪动玩家
			if ( IsLookonMode() == false && GetMeChairID() == pStartCallBanker->wCurrentCaller ) ActiveGameFrame();

			//设置状态
			SetGameStatus( GS_WK_CALL_BANKER ) ;

			//设置庄家
			m_GameClientView.SetBanker( INVALID_CHAIR ) ;

			return true ;
		}
	case SUB_S_SHOW_CARD_TIME:	//摊牌时间
		{
			return OnSubShowCardTime(pBuffer,wDataSize);
		}
	case SUB_S_SEND_CARD:		//发送扑克
		{
			return OnSubSendCard(pBuffer,wDataSize);
		}
	case SUB_S_SETCHIP:			//设置压注
		{
			return OnSubSetChip(pBuffer , wDataSize) ;
		}
	case SUB_S_SETSEGMENT:		//设置分段
		{
            return OnSubSetSegment(pBuffer , wDataSize) ;
		}
	case SUB_S_SHOW_CARD:      //玩家摊牌
		{
			return OnSubShowCard(pBuffer , wDataSize) ;
		}
	case SUB_S_COMPARE_CARD:	//比较扑克
		{
			return OnSubCompareCard(pBuffer , wDataSize) ;
		}
	case SUB_S_GAME_END:		//游戏结束
		{
			return OnSubGameEnd(pBuffer,wDataSize);
		}	
	case SUB_S_CALL_BANKER :		//玩家叫庄
		{
			//数据验证
			ASSERT( sizeof( CMD_S_CallBanker ) == wDataSize ) ;
			if ( sizeof( CMD_S_CallBanker ) != wDataSize ) return false ;

			CMD_S_CallBanker *pCallBanker = ( CMD_S_CallBanker* )pBuffer ;

			//玩家叫庄
			if ( pCallBanker->bCallBanker )
			{
				//设置变量
				m_wBanker = pCallBanker->wLastCaller ;

				//删除定时器
				KillGameTimer( IDI_CALL_BANKER ) ;

				//设置界面
				m_GameClientView.SetBanker( SwitchViewChairID( pCallBanker->wLastCaller ) ) ;
				m_GameClientView.SetCallBankerInfo( INVALID_CHAIR, false ) ;

				//设置按钮
				if ( pCallBanker->wLastCaller != GetMeChairID() && !IsLookonMode() )
				{
					//下注大小
					LONG lChipsArray[SET_CHIP_COUNT] ;
					GetChips( lChipsArray ) ;

					m_GameClientView.SetChips( lChipsArray ) ;

					for ( BYTE cbChipIdx = 0; cbChipIdx < SET_CHIP_COUNT; cbChipIdx++ )
					{
						CString strChip ;
						strChip.Format( TEXT( "%ld" ), lChipsArray[cbChipIdx] ) ;
						m_GameClientView.m_btSetChip[cbChipIdx].SetWindowText( strChip ) ;
					}

					m_GameClientView.m_btSetChip[0].ShowWindow( SW_SHOW ) ;
					m_GameClientView.m_btSetChip[0].EnableWindow( TRUE ) ;
					m_GameClientView.m_btSetChip[1].ShowWindow( SW_SHOW ) ;
					m_GameClientView.m_btSetChip[1].EnableWindow( TRUE ) ;
					m_GameClientView.m_btSetChip[2].ShowWindow( SW_SHOW ) ;
					m_GameClientView.m_btSetChip[2].EnableWindow( TRUE ) ;
					m_GameClientView.m_btSetChip[3].ShowWindow( SW_SHOW ) ;
					m_GameClientView.m_btSetChip[3].EnableWindow( TRUE ) ;
					SetGameTimer( GetMeChairID(), IDI_SETCHIP, 30 ) ;
				}
				
				//设置状态
				SetGameStatus( GS_WK_SETCHIP ) ;
			}
			//玩家不叫
			else
			{
				//设置定时器
				SetGameTimer( pCallBanker->wCurrentCaller, IDI_CALL_BANKER, IDT_CALL_BANKER_TIME ) ;				

				//设置按钮
				if ( pCallBanker->wCurrentCaller == GetMeChairID() && !IsLookonMode())
				{
					m_GameClientView.m_btCallBanker.ShowWindow( SW_SHOW ) ;
					m_GameClientView.m_btNoCallBanker.ShowWindow( SW_SHOW ) ;
				}

				//设置界面
				m_GameClientView.SetCallBankerInfo( SwitchViewChairID( pCallBanker->wLastCaller ), true ) ;
			}

			return true ;
		}
	case SUB_S_END_ALL_NO_CALL_BANKER:	//都不叫庄
		{
			//消息提示
			InsertShtickString( TEXT( "所有人都不叫庄，游戏结束" ), RGB( 255, 0, 0 ) , true ) ;

			//显示成绩框
			//设置积分
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				const tagUserData * pUserData=GetUserData(i);
				if(NULL!=pUserData)
					m_GameClientView.m_ScoreView.SetGameScore( i, pUserData->szName, 0 );
				else
					m_GameClientView.m_ScoreView.SetGameScore( i, "" ,0 );
			}
			m_GameClientView.m_ScoreView.SetGameTax( 0 );
			m_GameClientView.m_ScoreView.ShowWindow( SW_SHOW );
			m_GameClientView.m_ScoreView.CenterWindow( &m_GameClientView ) ;

			m_GameClientView.SetCallBankerInfo( INVALID_CHAIR, false ) ;

			//设置按钮
			if ( !IsLookonMode() ) 
			{
				m_GameClientView.m_btStart.ShowWindow( SW_SHOW ) ;
				m_GameClientView.m_btStart.EnableWindow( TRUE ) ;

				//设置定时器
				SetGameTimer( GetMeChairID(), IDI_START_GAME, IDT_START_TIME ) ;
			}

			return true ;
		}
	}
	
	return false;
}

//游戏场景
bool CGameClientDlg::OnGameSceneMessage(BYTE cbGameStation, bool bLookonOther, const void * pBuffer, WORD wDataSize)
{
	switch (cbGameStation)
	{
	case GS_WK_FREE:	//空闲状态
		{
			//设置控件
			if ( IsLookonMode() == false )
			{
				if(FALSE==m_GameClientView.m_btStart.IsWindowVisible())
				{
					m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
					m_GameClientView.m_btStart.EnableWindow(TRUE) ;
					m_GameClientView.m_btStart.SetFocus();
				}
				SetGameTimer(GetMeChairID(),IDI_START_GAME,IDT_START_TIME);
			}
			//设置扑克
			if (IsLookonMode()==false) m_GameClientView.m_UserCardControl[2].SetDisplayFlag(true);


			return true;
		}

	case GS_WK_CALL_BANKER :	//叫庄状态
		{
			//验证参数
			ASSERT( sizeof( CMD_S_StatusCallBanker ) == wDataSize ) ;
			if ( sizeof( CMD_S_StatusCallBanker ) != wDataSize ) return false ;

			CMD_S_StatusCallBanker  *pStatusCallBanker = ( CMD_S_StatusCallBanker * ) pBuffer ;
			
			//设置定时器
			SetGameTimer( pStatusCallBanker->wCurrentCaller, IDI_CALL_BANKER, IDT_CALL_BANKER_TIME ) ;

			if ( GetMeChairID() == pStatusCallBanker->wCurrentCaller  && !IsLookonMode() )
			{
				//设置按钮
				m_GameClientView.m_btCallBanker.ShowWindow( SW_SHOW ) ;
				m_GameClientView.m_btNoCallBanker.ShowWindow( SW_SHOW ) ;
			}
			
			return true ;
		}

	case GS_WK_SETCHIP:		//下注状态
		{
			CMD_S_StatusSetChip *pStatusSetChip=(CMD_S_StatusSetChip *)pBuffer ;

#ifdef _DEBUG
		//	AfxMessageBox("case GS_WK_SETCHIP") ;
#endif
			
			m_wBanker = pStatusSetChip->wBanker ;
			m_GameClientView.SetBanker(SwitchViewChairID(m_wBanker)) ;
			m_bCanSetChip = false ;

			//最高下注
			m_GameClientView.SetMaxChip(m_wMaxChip) ;


			m_GameClientView.m_btSetChip[0].ShowWindow(SW_HIDE) ;
			m_GameClientView.m_btSetChip[0].EnableWindow(FALSE) ;
			m_GameClientView.m_btSetChip[1].ShowWindow(SW_HIDE) ;
			m_GameClientView.m_btSetChip[1].EnableWindow(FALSE) ;
			m_GameClientView.m_btSetChip[2].ShowWindow(SW_HIDE) ;
			m_GameClientView.m_btSetChip[2].EnableWindow(FALSE) ;
			m_GameClientView.m_btSetChip[3].ShowWindow(SW_HIDE) ;
			m_GameClientView.m_btSetChip[3].EnableWindow(FALSE) ;
			m_GameClientView.m_btFrontCard.ShowWindow(SW_HIDE) ;
			m_GameClientView.m_btFrontCard.EnableWindow(FALSE) ;
			m_GameClientView.m_btMidCard.ShowWindow(SW_HIDE) ;
			m_GameClientView.m_btMidCard.EnableWindow(FALSE) ;
			m_GameClientView.m_btBackCard.ShowWindow(SW_HIDE) ;
			m_GameClientView.m_btBackCard.EnableWindow(FALSE) ;
	

			CopyMemory(m_nChip , pStatusSetChip->nChip , sizeof(m_nChip)) ;
			for(WORD i=0 ; i<GAME_PLAYER ; ++i) m_GameClientView.SetChip(SwitchViewChairID(i) , m_nChip[i]) ;

			//设置按钮
			if(false==IsLookonMode())
			{
				if(-1==m_nChip[GetMeChairID()] && GetMeChairID()!=m_wBanker)
				{
#ifdef _DEBUG
					AfxMessageBox("GS_WK_SETCHIP:if(-1==m_nChip[GetMeChairID()])") ;
#endif
					//下注大小
					LONG lChipsArray[SET_CHIP_COUNT] ;
					GetChips( lChipsArray ) ;
					m_GameClientView.SetChips( lChipsArray ) ;
                    
					for ( BYTE cbChipIdx = 0; cbChipIdx < SET_CHIP_COUNT; cbChipIdx++ )
					{
						CString strChip ;
						strChip.Format( TEXT( "%ld" ), lChipsArray[cbChipIdx] ) ;
						m_GameClientView.m_btSetChip[cbChipIdx].SetWindowText( strChip ) ;
					}

					m_GameClientView.m_btSetChip[0].ShowWindow( SW_SHOW ) ;
					m_GameClientView.m_btSetChip[0].EnableWindow( TRUE ) ;
					m_GameClientView.m_btSetChip[1].ShowWindow( SW_SHOW ) ;
					m_GameClientView.m_btSetChip[1].EnableWindow( TRUE ) ;
					m_GameClientView.m_btSetChip[2].ShowWindow( SW_SHOW ) ;
					m_GameClientView.m_btSetChip[2].EnableWindow( TRUE ) ;
					m_GameClientView.m_btSetChip[3].ShowWindow( SW_SHOW ) ;
					m_GameClientView.m_btSetChip[3].EnableWindow( TRUE ) ;
	
					//设置时间
					bool bRet = SetGameTimer(GetMeChairID() , IDI_SETCHIP , 30) ;
#ifdef _DEBUG	
					KillTimer(IDI_SETCHIP) ;
					bRet = SetGameTimer(GetMeChairID() , IDI_SETCHIP , g_nGameTime) ;
#endif
				}
			}

			return true ;
		}
	case GS_WK_PLAYING:		//游戏状态
		{
#ifdef _DEBUG
	//		AfxMessageBox("case GS_WK_PLAYING") ;
#endif

			CMD_S_StatusPlay *pStatusPlay = (CMD_S_StatusPlay *)pBuffer;

			m_wBanker = pStatusPlay->wBanker ;
			m_GameClientView.SetBanker(SwitchViewChairID(m_wBanker)) ;

			//最高下注
			m_GameClientView.SetMaxChip(m_wMaxChip) ;

			bool bFinishiShowCard = true ;
			//完成摊牌
			for(WORD wID = 0 ; wID<GAME_PLAYER ; ++wID)
			{
				const tagUserData * pUserData=GetUserData(wID);
				if(pUserData) 
					if(false==pStatusPlay->bFinishSegment[wID])
					{
						bFinishiShowCard = false ;
						break ;
					}
			}

			CopyMemory(m_bHandCardData , pStatusPlay->bHandCardData , sizeof(m_bHandCardData)) ;
			CopyMemory(m_bTmpHandCardData , m_bHandCardData , sizeof(m_bHandCardData)) ;
			m_bHandCardCount = HAND_CARD_COUNT ;
			if(false==pStatusPlay->bFinishSegment[GetMeChairID()])	m_GameClientView.SetCardData(2 , m_bHandCardData , m_bHandCardCount , enHandCard) ;

			if(false==IsLookonMode())
			{
				m_GameClientView.m_UserCardControl[2].SetDisplayFlag(true) ;
				m_GameClientView.m_UserCardControl[2].SetPositively(true) ;
				m_GameClientView.m_btSetChip[0].ShowWindow(SW_HIDE) ;
				m_GameClientView.m_btSetChip[0].EnableWindow(FALSE) ;
				m_GameClientView.m_btSetChip[1].ShowWindow(SW_HIDE) ;
				m_GameClientView.m_btSetChip[1].EnableWindow(FALSE) ;
				m_GameClientView.m_btSetChip[2].ShowWindow(SW_HIDE) ;
				m_GameClientView.m_btSetChip[2].EnableWindow(FALSE) ;
				m_GameClientView.m_btSetChip[3].ShowWindow(SW_HIDE) ;
				m_GameClientView.m_btSetChip[3].EnableWindow(FALSE) ;

				if(true==pStatusPlay->bFinishSegment[GetMeChairID()])
				{
					m_GameClientView.m_btFrontCard.ShowWindow(SW_HIDE) ;
					m_GameClientView.m_btFrontCard.EnableWindow(FALSE) ;
					m_GameClientView.m_btMidCard.ShowWindow(SW_HIDE) ;
					m_GameClientView.m_btMidCard.EnableWindow(FALSE) ;
					m_GameClientView.m_btBackCard.ShowWindow(SW_HIDE) ;
					m_GameClientView.m_btBackCard.EnableWindow(FALSE) ;

					m_GameClientView.m_btAutoOutCard.ShowWindow(SW_HIDE) ;
					m_GameClientView.m_btAutoOutCard.EnableWindow(FALSE) ;
					m_GameClientView.m_btDescend.ShowWindow(SW_HIDE) ;
					m_GameClientView.m_btDescend.EnableWindow(FALSE) ;
					m_GameClientView.m_btAscend.ShowWindow(SW_HIDE) ;
					m_GameClientView.m_btAscend.EnableWindow(FALSE) ;
					m_GameClientView.m_btColor.ShowWindow(SW_HIDE) ;
					m_GameClientView.m_btColor.EnableWindow(FALSE) ;
				}
				else
				{
					m_GameClientView.m_btFrontCard.ShowWindow(SW_SHOW) ;
					m_GameClientView.m_btFrontCard.EnableWindow(FALSE) ;
					m_GameClientView.m_btMidCard.ShowWindow(SW_SHOW) ;
					m_GameClientView.m_btMidCard.EnableWindow(FALSE) ;
					m_GameClientView.m_btBackCard.ShowWindow(SW_SHOW) ;
					m_GameClientView.m_btBackCard.EnableWindow(FALSE) ;

					m_GameClientView.m_btAutoOutCard.ShowWindow(SW_SHOW) ;
					m_GameClientView.m_btAutoOutCard.EnableWindow(TRUE) ;
					m_GameClientView.m_btDescend.ShowWindow(SW_SHOW) ;
					m_GameClientView.m_btDescend.EnableWindow(TRUE) ;
					m_GameClientView.m_btAscend.ShowWindow(SW_SHOW) ;
					m_GameClientView.m_btAscend.EnableWindow(TRUE) ;
					m_GameClientView.m_btColor.ShowWindow(SW_SHOW) ;
					m_GameClientView.m_btColor.EnableWindow(TRUE) ;
				}
			}
			else	
			{
				m_GameClientView.m_UserCardControl[2].SetDisplayFlag(false) ;
				m_GameClientView.m_UserCardControl[2].SetPositively(false) ;
				m_GameClientView.m_btSetChip[0].ShowWindow(SW_HIDE) ;
				m_GameClientView.m_btSetChip[0].EnableWindow(FALSE) ;
				m_GameClientView.m_btSetChip[1].ShowWindow(SW_HIDE) ;
				m_GameClientView.m_btSetChip[1].EnableWindow(FALSE) ;
				m_GameClientView.m_btSetChip[2].ShowWindow(SW_HIDE) ;
				m_GameClientView.m_btSetChip[2].EnableWindow(FALSE) ;
				m_GameClientView.m_btSetChip[3].ShowWindow(SW_HIDE) ;
				m_GameClientView.m_btSetChip[3].EnableWindow(FALSE) ;
				m_GameClientView.m_btFrontCard.ShowWindow(SW_HIDE) ;
				m_GameClientView.m_btFrontCard.EnableWindow(FALSE) ;
				m_GameClientView.m_btMidCard.ShowWindow(SW_HIDE) ;
				m_GameClientView.m_btMidCard.EnableWindow(FALSE) ;
				m_GameClientView.m_btBackCard.ShowWindow(SW_HIDE) ;
				m_GameClientView.m_btBackCard.EnableWindow(FALSE) ;
				m_GameClientView.m_btAutoOutCard.ShowWindow(SW_HIDE) ;
				m_GameClientView.m_btAutoOutCard.EnableWindow(FALSE) ;
				m_GameClientView.m_btDescend.ShowWindow(SW_HIDE) ;
				m_GameClientView.m_btDescend.EnableWindow(FALSE) ;
				m_GameClientView.m_btAscend.ShowWindow(SW_HIDE) ;
				m_GameClientView.m_btAscend.EnableWindow(FALSE) ;
				m_GameClientView.m_btColor.ShowWindow(SW_HIDE) ;
				m_GameClientView.m_btColor.EnableWindow(FALSE) ;
			}

			CopyMemory(m_bSegmentCard , pStatusPlay->bSegmentCard , sizeof(m_bSegmentCard)) ;

			//没有摊牌

			if(true==pStatusPlay->bFinishSegment[GetMeChairID()])
			{
				for(WORD wID = 0 ; wID<GAME_PLAYER ; ++wID)
				{
					WORD wChairID = SwitchViewChairID(wID) ;
					const tagUserData * pUserData=GetUserData(wID);

					if(NULL!=pUserData)
					{
						//摊牌扑克
						if(true==pStatusPlay->bFinishSegment[wID])
						{

							for(BYTE j=0 ; j<3 ; ++j)
							{
								if(0==j)
								{
									m_GameClientView.SetCardData(wChairID , m_bSegmentCard[wID][j] , 3 , enAllSegmentCard , j) ;
									if(true==IsLookonMode())
										m_GameClientView.m_AllSegCardControl[wChairID][j].SetDisplayFlag(false) ;
								}
								else
								{
									m_GameClientView.SetCardData(wChairID , m_bSegmentCard[wID][j] , 5 , enAllSegmentCard , j) ;
									if(true==IsLookonMode())
										m_GameClientView.m_AllSegCardControl[wChairID][j].SetDisplayFlag(false) ;
								}
							}
						}
						//手上扑克
						else
						{
							BYTE bCardData[13] ;
							memset(bCardData , 0 , 13) ;
							m_GameClientView.SetCardData(wChairID , bCardData , 13 , enHandCard) ;
						}
					}
				}
			}
			else
			{
				for(WORD wID = 0 ; wID<GAME_PLAYER ; ++wID)
				{
					const tagUserData * pUserData=GetUserData(wID);

					if(NULL!=pUserData)
					{
						if(wID!=GetMeChairID())
						{
							BYTE bCardData[13] ;
							memset(bCardData , 0 , 13) ;
							m_GameClientView.SetCardData(SwitchViewChairID(wID) , bCardData , 13 , enHandCard) ;
						}
					}
				}
			}

			if(true==bFinishiShowCard)
			{
				KillTimer(m_nTimer) ;
			//	m_nTimer=SetTimer(IDI_COMPARE_FRONT , m_bCompareTime , NULL) ;
				OnSubCompareCard(NULL , 0) ;
			}


			CopyMemory(m_nChip , pStatusPlay->nChip , sizeof(m_nChip)) ;
			for(WORD i=0 ; i<GAME_PLAYER ; ++i) m_GameClientView.SetChip(SwitchViewChairID(i) , m_nChip[i]) ;

			//设置按钮
			if(false==IsLookonMode())
			{
				if(-1==m_nChip[GetMeChairID()] && GetMeChairID()!=m_wBanker)
				{
#ifdef _DEBUG
					AfxMessageBox("GS_WK_PLAYING:if(-1==m_nChip[GetMeChairID()])") ;
#endif
					//下注大小
					LONG lChipsArray[SET_CHIP_COUNT] ;
					GetChips( lChipsArray ) ;
					m_GameClientView.SetChips( lChipsArray ) ;
                    
					for ( BYTE cbChipIdx = 0; cbChipIdx < SET_CHIP_COUNT; cbChipIdx++ )
					{
						CString strChip ;
						strChip.Format( TEXT( "%ld" ), lChipsArray[cbChipIdx] ) ;
						m_GameClientView.m_btSetChip[cbChipIdx].SetWindowText( strChip ) ;
					}

					m_GameClientView.m_btSetChip[0].ShowWindow( SW_SHOW ) ;
					m_GameClientView.m_btSetChip[0].EnableWindow( TRUE ) ;
					m_GameClientView.m_btSetChip[1].ShowWindow( SW_SHOW ) ;
					m_GameClientView.m_btSetChip[1].EnableWindow( TRUE ) ;
					m_GameClientView.m_btSetChip[2].ShowWindow( SW_SHOW ) ;
					m_GameClientView.m_btSetChip[2].EnableWindow( TRUE ) ;
					m_GameClientView.m_btSetChip[3].ShowWindow( SW_SHOW ) ;
					m_GameClientView.m_btSetChip[3].EnableWindow( TRUE ) ;
					m_GameClientView.m_btAutoOutCard.ShowWindow(SW_SHOW) ;
					m_GameClientView.m_btAutoOutCard.EnableWindow(TRUE) ;

					//设置时间
					bool bRet = SetGameTimer(GetMeChairID() , IDI_SHOW_CARD , m_bShowCardTime) ;
#ifdef _DEBUG	
					KillTimer(IDI_SHOW_CARD) ;
					bRet = SetGameTimer(GetMeChairID() , IDI_SHOW_CARD , g_nGameTime) ;
#endif
					//	SetGameTimer(GetMeChairID() , IDI_SHOW_CARD , 5) ;
				}
				else if(false==pStatusPlay->bFinishSegment[GetMeChairID()])
				{
					m_GameClientView.m_btFrontCard.ShowWindow(SW_SHOW) ;
					m_GameClientView.m_btFrontCard.EnableWindow(FALSE) ;
					m_GameClientView.m_btMidCard.ShowWindow(SW_SHOW) ;
					m_GameClientView.m_btMidCard.EnableWindow(FALSE) ;
					m_GameClientView.m_btBackCard.ShowWindow(SW_SHOW) ;
					m_GameClientView.m_btBackCard.EnableWindow(FALSE) ;

					m_GameClientView.m_btAutoOutCard.ShowWindow(SW_SHOW) ;
					m_GameClientView.m_btAutoOutCard.EnableWindow(TRUE) ;

					//设置时间
					bool bRet = SetGameTimer(GetMeChairID() , IDI_SHOW_CARD , m_bShowCardTime) ;
#ifdef _DEBUG	
					KillTimer(IDI_SHOW_CARD) ;
					bRet = SetGameTimer(GetMeChairID() , IDI_SHOW_CARD , g_nGameTime) ;
#endif
					//		SetGameTimer(GetMeChairID() , IDI_SHOW_CARD , 5) ;
				}
			}

			return true ;
			////////////
		}
	}	

	return true;
}

//开始按钮
LRESULT CGameClientDlg::OnStart(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam) ;
	UNREFERENCED_PARAMETER(lParam) ;

	m_bReady = true ;

	
	//设置界面
	KillGameTimer(IDI_START_GAME);

	m_GameClientView.m_ScoreView.ShowWindow(SW_HIDE);
	m_GameClientView.SetChip(INVALID_CHAIR , 0) ;
	for(WORD i=0 ; i<GAME_PLAYER ; ++i) m_nChip[i] = -1 ;
	m_GameClientView.ShowBankerName(false) ;
	m_GameClientView.SetCallBankerInfo( INVALID_CHAIR, false ) ;

	//设置按钮
	m_GameClientView.m_btStart.ShowWindow(SW_HIDE);
	m_GameClientView.m_btStart.EnableWindow(FALSE) ;
	m_GameClientView.m_btFrontCard.ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btFrontCard.EnableWindow(FALSE) ;
	m_GameClientView.m_btMidCard.ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btMidCard.EnableWindow(FALSE) ;
	m_GameClientView.m_btBackCard.ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btBackCard.EnableWindow(FALSE) ;
	m_GameClientView.m_btShowCard.ShowWindow( SW_HIDE ) ;

	//设置扑克
	m_GameClientView.SetCardData(INVALID_CHAIR , NULL , 0 , enHandCard) ;
	m_GameClientView.SetCardData(INVALID_CHAIR , NULL , 0 , enSegmentCard) ;
	m_GameClientView.SetCardData(INVALID_CHAIR , NULL , 0 , enAllSegmentCard) ;
	m_GameClientView.SetCardData(INVALID_CHAIR , NULL , 0 , enCompareCard) ;
	m_GameClientView.SetCardData(INVALID_CHAIR , NULL , 0 , enBankCard) ;
	for ( BYTE cbSegCardIdx = 0; cbSegCardIdx < 3; cbSegCardIdx++ )
		m_GameClientView.m_HandSegCardControl[cbSegCardIdx].SetCardData( NULL, 0 ) ;

	//清空扑克
	for (WORD i=0;i<GAME_PLAYER;i++) m_GameClientView.m_UserCardControl[i].SetCardData(NULL,0);

	//发送消息
	SendUserReady(NULL,0);

#ifdef _DEBUG
g_bNewStart = true ;
#endif

	return 0;
}

//发送扑克
bool CGameClientDlg::OnSubSendCard(const void * pBuffer, WORD wDataSize)
{

	//初始化
	//设置界面
	KillGameTimer(IDI_START_GAME);
	KillGameTimer(IDI_SETCHIP) ;

	m_GameClientView.m_ScoreView.ShowWindow(SW_HIDE);
	m_GameClientView.m_ScoreView.ResetScore() ;
	m_GameClientView.SetChip(INVALID_CHAIR , 0) ;

	//设置按钮
	m_GameClientView.m_btStart.ShowWindow(SW_HIDE);
	m_GameClientView.m_btStart.EnableWindow(FALSE) ;
	m_GameClientView.m_btSetChip[0].ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btSetChip[0].EnableWindow(FALSE) ;
	m_GameClientView.m_btSetChip[1].ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btSetChip[1].EnableWindow(FALSE) ;
	m_GameClientView.m_btSetChip[2].ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btSetChip[2].EnableWindow(FALSE) ;
	m_GameClientView.m_btSetChip[3].ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btSetChip[3].EnableWindow(FALSE) ;

	//设置扑克
	m_GameClientView.SetCardData(INVALID_CHAIR , NULL , 0 , enHandCard) ;
	m_GameClientView.SetCardData(INVALID_CHAIR , NULL , 0 , enSegmentCard) ;
	m_GameClientView.SetCardData(INVALID_CHAIR , NULL , 0 , enAllSegmentCard) ;
	m_GameClientView.SetCardData(INVALID_CHAIR , NULL , 0 , enCompareCard) ;
	m_GameClientView.SetCardData(INVALID_CHAIR , NULL , 0 , enBankCard) ;

	//清空扑克
	for (WORD i=0;i<GAME_PLAYER;i++) m_GameClientView.m_UserCardControl[i].SetCardData(NULL,0);



	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_SendCard));
	if (wDataSize!=sizeof(CMD_S_SendCard)) return false;

	//设置状态
	SetGameStatus(GS_WK_PLAYING);

	//变量定义
	CMD_S_SendCard * pSendCard=(CMD_S_SendCard *)pBuffer;

	//设置数据
	CopyMemory(m_bHandCardData,pSendCard->bCardData,sizeof(pSendCard->bCardData));
	CopyMemory(m_bTmpHandCardData , m_bHandCardData , sizeof(m_bHandCardData)) ;
	m_wBanker = pSendCard->wBanker ;
	m_bHandCardCount=13 ;
	m_nChip[m_wBanker] = 0 ;
	m_bFinishSegment=false ; 
	
	//设置界面
	m_GameClientView.SetBanker(SwitchViewChairID( pSendCard->wBanker)) ;
	for(WORD wID=0 ; wID<GAME_PLAYER ; ++wID)
	{
		const tagUserData * pUserData=GetUserData(wID);
		if(pUserData && wID!=m_wBanker)
            m_GameClientView.SetChip(SwitchViewChairID(wID) , m_nChip[wID]) ;
	}

	BYTE bCardData[13] ;
	memset(bCardData , 0 , 13) ;

	//手上扑克
	m_GameClientView.SetCardData(2, m_bHandCardData , HAND_CARD_COUNT , enHandCard);

	//其他玩家
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		const tagUserData * pUserData=GetUserData(i);
		WORD wChairID = SwitchViewChairID(i) ;
		
		//设置背景
		if(NULL!=pUserData && GetMeChairID()!=i)
		{
			m_GameClientView.SetCardData(wChairID , bCardData , HAND_CARD_COUNT , enHandCard);
		}
	}

	if (IsLookonMode()==true) 
	{
		m_GameClientView.m_UserCardControl[2].SetDisplayFlag(false);
	}

	//设置按钮
	if ((IsLookonMode()==false)&&(pSendCard->wCurrentUser==GetMeChairID()))
	{
		m_GameClientView.m_UserCardControl[2].SetPositively(true) ;
		m_GameClientView.m_UserCardControl[2].ShootAllCard(false) ;

		m_GameClientView.m_btFrontCard.ShowWindow(SW_SHOW) ;
		m_GameClientView.m_btFrontCard.EnableWindow(FALSE) ;
		m_GameClientView.m_btMidCard.ShowWindow(SW_SHOW) ;
		m_GameClientView.m_btMidCard.EnableWindow(FALSE) ;
		m_GameClientView.m_btBackCard.ShowWindow(SW_SHOW) ;
		m_GameClientView.m_btBackCard.EnableWindow(FALSE) ;

		m_GameClientView.m_btDescend.ShowWindow(SW_SHOW) ;
		m_GameClientView.m_btDescend.EnableWindow(TRUE) ;
		m_GameClientView.m_btAscend.ShowWindow(SW_SHOW) ;
		m_GameClientView.m_btAscend.EnableWindow(TRUE) ;
		m_GameClientView.m_btColor.ShowWindow(SW_SHOW) ;
		m_GameClientView.m_btColor.EnableWindow(TRUE) ;

		m_GameClientView.m_btAutoOutCard.ShowWindow(SW_SHOW) ;
		m_GameClientView.m_btAutoOutCard.EnableWindow(TRUE) ;
	}

	//播放声音
	PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));

	//设置时间
	bool bRet = SetGameTimer(GetMeChairID() , IDI_SHOW_CARD , m_bShowCardTime) ;
//	SetGameTimer(GetMeChairID() , IDI_SHOW_CARD , 10) ;
#ifdef _DEBUG	
	KillTimer(IDI_SHOW_CARD) ;
	bRet = SetGameTimer(GetMeChairID() , IDI_SHOW_CARD , g_nGameTime) ;
#endif

	return true;
}


//游戏结束
bool CGameClientDlg::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//	InsertSystemString("CGameClientDlg::OnSubGameEnd");
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;

	//消息处理
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;

	//设置庄家
	m_GameClientView.SetBanker(SwitchViewChairID(m_wBanker)) ;

	m_bReady = false ; 

	//游戏状态
	SetGameStatus(GS_WK_FREE) ;

	//删除定时器
	KillGameTimer(IDI_SHOW_CARD);
	KillTimer(m_nTimer) ;

	//设置按钮

	m_GameClientView.m_btSetChip[0].ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btSetChip[0].EnableWindow(FALSE) ;
	m_GameClientView.m_btSetChip[1].ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btSetChip[1].EnableWindow(FALSE) ;
	m_GameClientView.m_btSetChip[2].ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btSetChip[2].EnableWindow(FALSE) ;
	m_GameClientView.m_btSetChip[3].ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btSetChip[3].EnableWindow(FALSE) ;
	
	m_GameClientView.m_btFrontCard.ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btFrontCard.EnableWindow(FALSE) ;
	m_GameClientView.m_btMidCard.ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btMidCard.EnableWindow(FALSE) ;
	m_GameClientView.m_btBackCard.ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btBackCard.EnableWindow(FALSE) ;

	m_GameClientView.m_btAutoOutCard.ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btAutoOutCard.EnableWindow(FALSE) ;
	m_GameClientView.m_btDescend.ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btDescend.EnableWindow(FALSE) ;
	m_GameClientView.m_btAscend.ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btAscend.EnableWindow(FALSE) ;
	m_GameClientView.m_btColor.ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btColor.EnableWindow(FALSE) ;
	m_GameClientView.m_btRestoreCard.ShowWindow(SW_HIDE) ;

	m_GameClientView.m_btNoCallBanker.ShowWindow( SW_HIDE ) ;
	m_GameClientView.m_btCallBanker.ShowWindow( SW_HIDE ) ;
    

	//设置状态
	SetGameStatus(GS_WK_FREE);

	//设置扑克

	m_GameClientView.SetCardData(INVALID_CHAIR , NULL , 0 , enHandCard);
	m_GameClientView.SetCardData(INVALID_CHAIR , NULL , 0 , enSegmentCard) ;
	m_GameClientView.SetCardData(INVALID_CHAIR , NULL , 0 , enBankCard) ;

	InsertShtickString( TEXT( "" ), RGB( 255, 0, 0 ) , true ) ;
	InsertShtickString( TEXT( "" ), RGB( 255, 0, 0 ) , true ) ;
	InsertShtickString( TEXT( "本局得分：" ), RGB( 255, 0, 0 ) , true ) ;

	//设置积分
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		const tagUserData * pUserData=GetUserData(i);
		if(NULL!=pUserData)
		{
            m_GameClientView.m_ScoreView.SetGameScore(i,pUserData->szName,pGameEnd->lGameScore[i]);
			CString strScoreRes ;
			strScoreRes.Format( TEXT( "%s：%ld" ) , pUserData->szName,pGameEnd->lGameScore[i] ) ;
			InsertShtickString( strScoreRes, RGB( 255, 0, 0 ) , true ) ;
		}
		else
			m_GameClientView.m_ScoreView.SetGameScore(i,"",0);
	}
    m_GameClientView.m_ScoreView.SetGameTax(pGameEnd->lGameTax);
	m_GameClientView.m_ScoreView.ShowWindow(SW_SHOW);
	m_GameClientView.m_ScoreView.CenterWindow(&m_GameClientView) ;

	CString strScoreRes ;
	strScoreRes.Format( TEXT( "税收：%ld" ) , pGameEnd->lGameTax ) ;
	InsertShtickString( strScoreRes, RGB( 255, 0, 0 ) , true ) ;

	//显示信息	
	InsertShtickString( TEXT( "牌比结果：" ), RGB( 255, 0, 0 ) , true ) ;
	

	if(GER_NORMAL==pGameEnd->bEndMode)
	{
		//庄稼信息
		const tagUserData * pBankerData=GetUserData(m_wBanker);
		CString strWindMsg , strChip ;


		//计算结果
//		InsertShtickString("三墩比较结果:" , RGB(255,0,0) , true);
		int nBankerWinChip = 0 ; 

		bool bBankerDragon = false , //庄家乌龙
			bUserDragon   = false ; //闲家乌龙

		if((true==m_GameLogic.CompareCard(m_bSegmentCard[m_wBanker][0] , m_bSegmentCard[m_wBanker][1] , 3 , 5 , false)) &&
			(true==m_GameLogic.CompareCard(m_bSegmentCard[m_wBanker][1] , m_bSegmentCard[m_wBanker][2] , 5 , 5 , false)))
			bBankerDragon = false ;
		else
			bBankerDragon = true ;

		const tagUserData *pBanker = GetUserData(m_wBanker) ;
		for(WORD i=0 ; i<GAME_PLAYER ; ++i)
		{
			BYTE bWindCount=0 ;
			strWindMsg="" ;

			const tagUserData * pUserData=GetUserData(i);

			if(NULL!=pUserData)
			{
				if(i!=m_wBanker)
				{
					const tagUserData * pUserData=GetUserData(i);

					if(true==m_GameLogic.CompareCard(m_bSegmentCard[m_wBanker][0] , m_bSegmentCard[i][0] , 3 , 3 , true)) ++bWindCount ;
					if(true==m_GameLogic.CompareCard(m_bSegmentCard[m_wBanker][1] , m_bSegmentCard[i][1] , 5 , 5 , true)) ++bWindCount ;
					if(true==m_GameLogic.CompareCard(m_bSegmentCard[m_wBanker][2] , m_bSegmentCard[i][2] , 5 , 5 , true)) ++bWindCount ;

					if((true==m_GameLogic.CompareCard(m_bSegmentCard[i][0] , m_bSegmentCard[i][1] , 3 , 5 , false)) &&
						(true==m_GameLogic.CompareCard(m_bSegmentCard[i][1] , m_bSegmentCard[i][2] , 5 , 5 , false)))
						bUserDragon = false ;
					else
						bUserDragon = true ;

					//摆乌龙
					if(true==bUserDragon && true==bBankerDragon)
					{
						strWindMsg = CString("庄家[ ") + CString(pBanker->szName) + CString(" ]和[ ") +  CString(pUserData->szName) + CString(" ]倒配，和局！") ; 
					}
					else if(true==bBankerDragon)
					{
						strChip.Format(": %d 注" , m_nChip[i]) ;
					//	strWindMsg = (CString("庄家") + CString(pBanker->szName) + CString("倒配，")+CString(pUserData->szName) + CString(" 赢 庄家") + CString(pBanker->szName) + strChip) ;
						strWindMsg = (CString("庄家[ ") + CString(pBanker->szName) + CString(" ]倒配，[ ")+CString(pUserData->szName) + CString(" ] 赢 庄家[ ") + CString(pBanker->szName) +" ]") ;
						nBankerWinChip-=m_nChip[i] ;
					}
					else if(true==bUserDragon)
					{
						strChip.Format(": %d 注" , m_nChip[i]) ;
					//	strWindMsg = (CString(pUserData->szName) + CString("摆乌龙，") + CString("庄家") + CString(pBanker->szName) + CString(" 赢 ") + CString(pUserData->szName) + strChip) ;
						strWindMsg = "[ " + (CString(pUserData->szName) + CString(" ]倒配， ") + CString("庄家[ ") + CString(pBanker->szName) + CString(" ] 赢 [ ") + CString(pUserData->szName) + " ]") ;
						nBankerWinChip+=m_nChip[i] ;
					}
					//闲家赢
					else if(bWindCount>=2)
					{
						strChip.Format(": %d 注" , m_nChip[i]) ;
					//	strWindMsg = (CString(pUserData->szName) + CString(" 赢 庄家") + CString(pBanker->szName) + strChip) ;
						strWindMsg = "[ "+(CString(pUserData->szName) + CString(" ] 赢 庄家[ ") + CString(pBanker->szName) + " ]") ;
						nBankerWinChip-=m_nChip[i] ;
					}
					//庄家赢
					else
					{
						strChip.Format(": %d 注" , m_nChip[i]) ;
					//	strWindMsg = (CString("庄家") + CString(pBanker->szName) + CString(" 赢 ") + CString(pUserData->szName) + strChip) ;
						strWindMsg = (CString("庄家[ ") + CString(pBanker->szName) + CString(" ] 赢 [ ") + CString(pUserData->szName) + " ]") ;
						nBankerWinChip+=m_nChip[i] ;
					}

					InsertShtickString(strWindMsg , RGB( 255, 0, 0) , true) ;

					//播放声音
					if(i==GetMeChairID())
					{
						if(bWindCount>=2) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WIN"));
						else			  PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_LOST"));
					}
				}
			}
		}

		//播放声音
		if(m_wBanker==GetMeChairID())
		{
			if(nBankerWinChip>0) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WIN"));
			else			  PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_LOST"));
		}

		//显示信息
		//InsertShtickString(TEXT("翻倍统计：") , RGB(255,0,0) , true) ;
		//for(WORD wChairID=0; wChairID<GAME_PLAYER; ++wChairID)
		//{
		//	if(wChairID==m_wBanker) continue ;

		//	const tagUserData * pBankerData = GetUserData(m_wBanker) ;
		//	const tagUserData * pUserData=GetUserData(wChairID);
		//	if(pUserData)
		//	{
		//		CString strMsg ;
		//		strMsg.Format(TEXT("头墩：%s<--->%s %ld:%ld"), pBanker->szName, pUserData->szName, pGameEnd->cbResult[wChairID][0][0], pGameEnd->cbResult[wChairID][1][0]) ;
		//		InsertShtickString(strMsg , RGB(255,0,0) , true) ;
		//		
		//		strMsg.Format(TEXT("中墩：%s<--->%s %ld:%ld"), pBanker->szName, pUserData->szName, pGameEnd->cbResult[wChairID][0][1], pGameEnd->cbResult[wChairID][1][1]) ;
		//		InsertShtickString(strMsg , RGB(255,0,0) , true) ;
		//		
		//		strMsg.Format(TEXT("后墩：%s<--->%s %ld:%ld"), pBanker->szName, pUserData->szName, pGameEnd->cbResult[wChairID][0][2], pGameEnd->cbResult[wChairID][1][2]) ;
		//		InsertShtickString(strMsg , RGB(255,0,0) , true) ;
		//		
		//		strMsg.Format(TEXT("总共倍数：%s<--->%s %ld:%ld"), pBanker->szName, pUserData->szName, pGameEnd->cbResult[wChairID][0][3], pGameEnd->cbResult[wChairID][1][3]) ;
		//		InsertShtickString(strMsg , RGB(255,0,0) , true) ;
		//	}
		//}
	}

	//设置界面
	if (IsLookonMode()==false)
	{
		if(FALSE==m_GameClientView.m_btStart.IsWindowVisible())
		{
			m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
			m_GameClientView.m_btStart.EnableWindow(TRUE) ;
		}
		bool bRet = SetGameTimer(GetMeChairID(),IDI_START_GAME,IDT_START_TIME);
#ifdef _DEBUG	
		KillTimer(IDI_START_GAME) ;
		bRet = SetGameTimer(GetMeChairID() , IDI_START_GAME , g_nGameTime) ;
#endif
	}

//	m_GameClientView.SetBanker(INVALID_CHAIR) ;

	//设置变量

	return true;
}

//左键扑克
LRESULT CGameClientDlg::OnLeftHitCard(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam) ;
	UNREFERENCED_PARAMETER(lParam) ;

	if(false==m_bFinishSegment)
	{

		//获取扑克
		BYTE bCardData[HAND_CARD_COUNT];
		BYTE bShootCount=(BYTE)m_GameClientView.m_UserCardControl[2].GetShootCard(bCardData,HAND_CARD_COUNT);

		//设置按钮
		if(5==bShootCount)
		{
			if(TRUE==m_GameClientView.m_btMidCard.IsWindowVisible())
				m_GameClientView.m_btMidCard.EnableWindow(TRUE) ;
			if(TRUE==m_GameClientView.m_btBackCard.IsWindowVisible())
				m_GameClientView.m_btBackCard.EnableWindow(TRUE) ;
		}
		else if(3==bShootCount)
		{
			if(TRUE==m_GameClientView.m_btFrontCard.IsWindowVisible())
				m_GameClientView.m_btFrontCard.EnableWindow(TRUE) ;
		}
		else
		{
			if(TRUE==m_GameClientView.m_btMidCard.IsWindowVisible())
				m_GameClientView.m_btMidCard.EnableWindow(FALSE) ;
			if(TRUE==m_GameClientView.m_btBackCard.IsWindowVisible())
				m_GameClientView.m_btBackCard.EnableWindow(FALSE) ;
			if(TRUE==m_GameClientView.m_btFrontCard.IsWindowVisible())
				m_GameClientView.m_btFrontCard.EnableWindow(FALSE) ;
		}
	}
	if(true==m_bFinishSegment)
	{
		BYTE bSegCardData[3][HAND_CARD_COUNT]={0} ;
		BYTE bSegShootCount[3]={0} ;
		for(BYTE i=0 ; i<3 ; ++i)
		{
			bSegShootCount[i]=(BYTE)m_GameClientView.m_HandSegCardControl[i].GetShootCard(bSegCardData[i],HAND_CARD_COUNT);
		}

		if(bSegShootCount[0]!=bSegShootCount[1] && bSegShootCount[0]!=bSegShootCount[2] && bSegShootCount[1]!=bSegShootCount[2]) return 0 ;
		if(bSegShootCount[0]==bSegShootCount[1] && bSegShootCount[0]==bSegShootCount[2] && bSegShootCount[1]==bSegShootCount[2]) return 0 ;

		//交换扑克
		if(bSegShootCount[0]==bSegShootCount[1])
		{
			for(BYTE i=0 ; i<bSegShootCount[0] ; ++i)
			{
				DWORD wIndex=m_GameClientView.m_HandSegCardControl[0].GetCardData(bSegCardData[0][i]) ;
				ASSERT(-1!=wIndex) ;
				if(-1==wIndex) return 0 ;
				m_GameClientView.m_HandSegCardControl[0].SetCardItem(bSegCardData[1][i] , wIndex) ;
				m_GameClientView.m_AllSegCardControl[2][0].SetCardItem(bSegCardData[1][i] , wIndex) ;

				wIndex=m_GameClientView.m_HandSegCardControl[1].GetCardData(bSegCardData[1][i]) ;
				ASSERT(-1!=wIndex) ;
				if(-1==wIndex) return 0 ;
				m_GameClientView.m_HandSegCardControl[1].SetCardItem(bSegCardData[0][i] , wIndex) ;
				m_GameClientView.m_AllSegCardControl[2][1].SetCardItem(bSegCardData[0][i] , wIndex) ;
			}
		}
		//交换扑克
		if(bSegShootCount[0]==bSegShootCount[2])
		{
			for(BYTE i=0 ; i<bSegShootCount[0] ; ++i)
			{
				DWORD wIndex=m_GameClientView.m_HandSegCardControl[0].GetCardData(bSegCardData[0][i]) ;
				ASSERT(-1!=wIndex) ;
				if(-1==wIndex) return 0 ;
				m_GameClientView.m_HandSegCardControl[0].SetCardItem(bSegCardData[2][i] , wIndex) ;
				m_GameClientView.m_AllSegCardControl[2][0].SetCardItem(bSegCardData[2][i] , wIndex) ;

				wIndex=m_GameClientView.m_AllSegCardControl[2][2].GetCardData(bSegCardData[2][i]) ;
				ASSERT(-1!=wIndex) ;
				if(-1==wIndex) return 0 ;
				m_GameClientView.m_HandSegCardControl[2].SetCardItem(bSegCardData[0][i] , wIndex) ;
				m_GameClientView.m_AllSegCardControl[2][2].SetCardItem(bSegCardData[0][i] , wIndex) ;
			}
		}
		//交换扑克
		if(bSegShootCount[1]==bSegShootCount[2])
		{
			for(BYTE i=0 ; i<bSegShootCount[1] ; ++i)
			{
				DWORD wIndex=m_GameClientView.m_HandSegCardControl[1].GetCardData(bSegCardData[1][i]) ;
				ASSERT(-1!=wIndex) ;
				if(-1==wIndex) return 0 ;
				m_GameClientView.m_HandSegCardControl[1].SetCardItem(bSegCardData[2][i] , wIndex) ;
				m_GameClientView.m_AllSegCardControl[2][1].SetCardItem(bSegCardData[2][i] , wIndex) ;

				wIndex=m_GameClientView.m_AllSegCardControl[2][2].GetCardData(bSegCardData[2][i]) ;
				ASSERT(-1!=wIndex) ;
				if(-1==wIndex) return 0 ;
				m_GameClientView.m_HandSegCardControl[2].SetCardItem(bSegCardData[1][i] , wIndex) ;
				m_GameClientView.m_AllSegCardControl[2][2].SetCardItem(bSegCardData[1][i] , wIndex) ;
			}
		}

		//包牌按钮
	//	if(VerifyBaoPai() && IsLookonMode()==false) m_GameClientView.m_btBaoPai.ShowWindow(SW_SHOW) ;
	}
	
	return 0;
}
//压注消息
LRESULT CGameClientDlg::OnSetChip(WPARAM wParam, LPARAM lParam)
{
	//无用参数
	UNREFERENCED_PARAMETER(lParam) ;

	KillGameTimer(IDI_SETCHIP) ;
	

    CMD_C_SetChip  SetChip ;
	memset(&SetChip , 0 , sizeof(SetChip)) ;


    SetChip.nChip = (LONG)wParam ;

#ifdef _DEBUG
	if(-1==SetChip.nChip)
		AfxMessageBox("if(-1==SetChip.nChip)") ;
#endif

	SendData(SUB_C_SETCHIP , &SetChip , sizeof(SetChip)) ;

	m_GameClientView.m_btSetChip[0].ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btSetChip[0].EnableWindow(FALSE) ;
	m_GameClientView.m_btSetChip[1].ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btSetChip[1].EnableWindow(FALSE) ;
	m_GameClientView.m_btSetChip[2].ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btSetChip[2].EnableWindow(FALSE) ;
	m_GameClientView.m_btSetChip[3].ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btSetChip[3].EnableWindow(FALSE) ;

	return true ;
}
//设置前墩
LRESULT CGameClientDlg::OnFrontCard(WPARAM wParam, LPARAM lParam) 
{
	//无用参数
	UNREFERENCED_PARAMETER(wParam) ;
	UNREFERENCED_PARAMETER(lParam) ;

	CMD_C_SegCard  SegCard ;
	memset(&SegCard , 0 , sizeof(SegCard)) ;

	//获取扑克
	BYTE bCardData[HAND_CARD_COUNT];
	BYTE bShootCount=(BYTE)m_GameClientView.m_UserCardControl[2].GetShootCard(bCardData,HAND_CARD_COUNT);
	//排列扑克
	m_GameLogic.SortCardList(bCardData , bShootCount) ;

	ASSERT(3==bShootCount) ;
	if(3!=bShootCount) 
	{
		AfxMessageBox("错误扑克,请重新选择!") ;
		m_GameClientView.m_UserCardControl[2].ShootAllCard(false) ;
		return true ;
	}

	//删除扑克
	m_GameLogic.RemoveCard(bCardData , bShootCount , m_bHandCardData , 	m_bHandCardCount) ;
	m_bHandCardCount -= bShootCount ;
	//排列扑克
	m_GameLogic.SortCardList(m_bHandCardData , m_bHandCardCount) ;

	//设置界面
	m_GameClientView.SetCardData(2 , bCardData , bShootCount , enSegmentCard) ;
	m_GameClientView.SetCardData(2 , m_bHandCardData , m_bHandCardCount , enHandCard) ;
	m_GameClientView.m_SegCardControl[2].ShowWindow(SW_SHOW) ;

	//隐藏扑克
	m_GameClientView.m_AllSegCardControl[2][0].ShowWindow(SW_HIDE) ;
	//保存扑克
	m_GameClientView.m_AllSegCardControl[2][0].SetCardData(bCardData , bShootCount) ;

	SegCard.SegmentType=enFront ;

	SendData(SUB_C_SEGCARD , &SegCard , sizeof(SegCard)) ;

	//设置按钮
	m_GameClientView.m_btFrontCard.EnableWindow(FALSE) ;
	m_GameClientView.m_btFrontCard.ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btAutoOutCard.ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btAutoOutCard.EnableWindow(FALSE) ;
	
	if(3==m_bHandCardCount || 5==m_bHandCardCount)
	{
		//显示扑克
//		m_GameClientView.m_AllSegCardControl[2][0].ShowWindow(SW_SHOW) ;

		m_bFinishSegment=true ; 

		//设置扑克
		if(0!=m_GameClientView.m_AllSegCardControl[2][1].GetCardCount())
		{
	//		m_GameClientView.m_AllSegCardControl[2][1].ShowWindow(SW_SHOW) ;
			m_GameClientView.m_AllSegCardControl[2][2].SetCardData(m_bHandCardData , m_bHandCardCount) ;
			m_GameClientView.m_AllSegCardControl[2][2].ShowWindow(SW_HIDE) ;
		}
		else if(0!=m_GameClientView.m_AllSegCardControl[2][2].GetCardCount())
		{
	//		m_GameClientView.m_AllSegCardControl[2][2].ShowWindow(SW_SHOW) ;
			m_GameClientView.m_AllSegCardControl[2][1].SetCardData(m_bHandCardData , m_bHandCardCount) ;
			m_GameClientView.m_AllSegCardControl[2][1].ShowWindow(SW_HIDE) ;
		}
		else AfxMessageBox("OnFrontCard：错误扑克！") ;

		//设置扑克
		m_GameClientView.SetCardData(2 , NULL , 0 , enHandCard) ;
		m_GameClientView.SetCardData(2 , NULL , 0 , enSegmentCard) ;
		m_GameClientView.m_SegCardControl[2].ShowWindow(SW_HIDE) ;

		m_GameClientView.m_AllSegCardControl[2][0].GetCardData(bCardData , 3) ;
		m_GameClientView.m_HandSegCardControl[0].SetCardData(bCardData , 3) ;

		m_GameClientView.m_AllSegCardControl[2][1].GetCardData(bCardData , 5) ;
		m_GameClientView.m_HandSegCardControl[1].SetCardData(bCardData , 5) ;

		m_GameClientView.m_AllSegCardControl[2][2].GetCardData(bCardData , 5) ;
		m_GameClientView.m_HandSegCardControl[2].SetCardData(bCardData , 5) ;

		//包牌按钮
//		if(VerifyBaoPai() && IsLookonMode()==false) m_GameClientView.m_btBaoPai.ShowWindow(SW_SHOW) ;

		for(BYTE i=0 ; i<3 ; ++i)
		{
			m_GameClientView.m_HandSegCardControl[i].ShowWindow(SW_SHOW) ;
		}

		if(true==IsLookonMode())
            for(BYTE i=0 ; i<3 ; ++i)
			{
                m_GameClientView.m_HandSegCardControl[i].SetPositively(false) ;
				m_GameClientView.m_HandSegCardControl[i].SetDisplayFlag(false) ;
			}

		//设置按钮
		if(TRUE==m_GameClientView.m_btBackCard.IsWindowVisible())
			m_GameClientView.m_btBackCard.ShowWindow(SW_HIDE) ;
		if(TRUE==m_GameClientView.m_btMidCard.IsWindowVisible())
			m_GameClientView.m_btMidCard.ShowWindow(SW_HIDE) ;
		
		m_GameClientView.m_btShowCard.ShowWindow(TRUE) ;
		m_GameClientView.m_btShowCard.EnableWindow(TRUE) ;

		m_GameClientView.m_btDescend.ShowWindow(SW_HIDE) ;
		m_GameClientView.m_btDescend.EnableWindow(FALSE) ;
		m_GameClientView.m_btAscend.ShowWindow(SW_HIDE) ;
		m_GameClientView.m_btAscend.EnableWindow(FALSE) ;
		m_GameClientView.m_btColor.ShowWindow(SW_HIDE) ;
		m_GameClientView.m_btColor.EnableWindow(FALSE) ;
	}

	m_GameClientView.m_btRestoreCard.ShowWindow(SW_SHOW) ;

	//播放声音
	PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));

	return true ;
}
//设置中墩
LRESULT CGameClientDlg::OnMidCard(WPARAM wParam, LPARAM lParam)
{
	//无用参数
	UNREFERENCED_PARAMETER(wParam) ;
	UNREFERENCED_PARAMETER(lParam) ;

	CMD_C_SegCard  SegCard ;
	memset(&SegCard , 0 , sizeof(SegCard)) ;

	//获取扑克
	BYTE bCardData[HAND_CARD_COUNT];
	BYTE bShootCount=(BYTE)m_GameClientView.m_UserCardControl[2].GetShootCard(bCardData,HAND_CARD_COUNT);
	//排列扑克
	m_GameLogic.SortCardList(bCardData , bShootCount) ;

	ASSERT(5==bShootCount) ;
	if(5!=bShootCount) 
	{
		AfxMessageBox("错误扑克,请重新选择!") ;
		m_GameClientView.m_UserCardControl[2].ShootAllCard(false) ;
		return true ;
	}

	//删除扑克
	m_GameLogic.RemoveCard(bCardData , bShootCount , m_bHandCardData , 	m_bHandCardCount) ;
	m_bHandCardCount -= bShootCount ;
	//排列扑克
	m_GameLogic.SortCardList(m_bHandCardData , m_bHandCardCount) ;

	//隐藏扑克
	m_GameClientView.m_AllSegCardControl[2][1].ShowWindow(SW_HIDE) ;
	//保存扑克
	m_GameClientView.m_AllSegCardControl[2][1].SetCardData(bCardData , bShootCount) ;

	//设置界面
	m_GameClientView.SetCardData(2 , bCardData , bShootCount , enSegmentCard) ;
	m_GameClientView.SetCardData(2 , m_bHandCardData , m_bHandCardCount , enHandCard) ;
	m_GameClientView.m_SegCardControl[2].ShowWindow(SW_SHOW) ;

	SegCard.SegmentType=enMid ;

	SendData(SUB_C_SEGCARD , &SegCard , sizeof(SegCard)) ;

	//设置按钮
	m_GameClientView.m_btMidCard.EnableWindow(FALSE) ;
	m_GameClientView.m_btMidCard.ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btBackCard.EnableWindow(FALSE) ;

	m_GameClientView.m_btAutoOutCard.ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btAutoOutCard.EnableWindow(FALSE) ;

	if(3==m_bHandCardCount || 5==m_bHandCardCount)
	{
		//显示扑克
//		m_GameClientView.m_AllSegCardControl[2][1].ShowWindow(SW_SHOW) ;

		m_bFinishSegment=true ; 

		//设置扑克
		if(0!=m_GameClientView.m_AllSegCardControl[2][0].GetCardCount())
		{
	//		m_GameClientView.m_AllSegCardControl[2][0].ShowWindow(SW_SHOW) ;
			m_GameClientView.m_AllSegCardControl[2][2].SetCardData(m_bHandCardData , m_bHandCardCount) ;
			m_GameClientView.m_AllSegCardControl[2][2].ShowWindow(SW_HIDE) ;
		}
		else if(0!=m_GameClientView.m_AllSegCardControl[2][2].GetCardCount())
		{
	//		m_GameClientView.m_AllSegCardControl[2][2].ShowWindow(SW_SHOW) ;
			m_GameClientView.m_AllSegCardControl[2][0].SetCardData(m_bHandCardData , m_bHandCardCount) ;
			m_GameClientView.m_AllSegCardControl[2][0].ShowWindow(SW_HIDE) ;
		}
		else AfxMessageBox("OnMidCard：错误扑克！") ;

		//设置扑克
		m_GameClientView.SetCardData(2 , NULL , 0 , enHandCard) ;
		m_GameClientView.SetCardData(2 , NULL , 0 , enSegmentCard) ;

		m_GameClientView.m_SegCardControl[2].ShowWindow(SW_HIDE) ;

		m_GameClientView.m_AllSegCardControl[2][0].GetCardData(bCardData , 3) ;
		m_GameClientView.m_HandSegCardControl[0].SetCardData(bCardData , 3) ;

		m_GameClientView.m_AllSegCardControl[2][1].GetCardData(bCardData , 5) ;
		m_GameClientView.m_HandSegCardControl[1].SetCardData(bCardData , 5) ;

		m_GameClientView.m_AllSegCardControl[2][2].GetCardData(bCardData , 5) ;
		m_GameClientView.m_HandSegCardControl[2].SetCardData(bCardData , 5) ;

		//包牌按钮
//		if(VerifyBaoPai() && IsLookonMode()==false) m_GameClientView.m_btBaoPai.ShowWindow(SW_SHOW) ;

		for(BYTE i=0 ; i<3 ; ++i)
		{
			m_GameClientView.m_HandSegCardControl[i].ShowWindow(SW_SHOW) ;
		}

		if(true==IsLookonMode())
			for(BYTE i=0 ; i<3 ; ++i)
			{
				m_GameClientView.m_HandSegCardControl[i].SetPositively(false) ;
				m_GameClientView.m_HandSegCardControl[i].SetDisplayFlag(false) ;
			}


		//设置按钮
		if(TRUE==m_GameClientView.m_btFrontCard.IsWindowVisible())
			m_GameClientView.m_btFrontCard.ShowWindow(SW_HIDE) ;
		if(TRUE==m_GameClientView.m_btBackCard.IsWindowVisible())
			m_GameClientView.m_btBackCard.ShowWindow(SW_HIDE) ;

		m_GameClientView.m_btShowCard.ShowWindow(TRUE) ;
		m_GameClientView.m_btShowCard.EnableWindow(TRUE) ;

		m_GameClientView.m_btDescend.ShowWindow(SW_HIDE) ;
		m_GameClientView.m_btDescend.EnableWindow(FALSE) ;
		m_GameClientView.m_btAscend.ShowWindow(SW_HIDE) ;
		m_GameClientView.m_btAscend.EnableWindow(FALSE) ;
		m_GameClientView.m_btColor.ShowWindow(SW_HIDE) ;
		m_GameClientView.m_btColor.EnableWindow(FALSE) ;
	}

	m_GameClientView.m_btRestoreCard.ShowWindow(SW_SHOW) ;

	//播放声音
	PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));

	return true ;
}
//玩家摊牌
LRESULT CGameClientDlg::OnShowCard(WPARAM wParam, LPARAM lParam) 
{
	//无用参数
	UNREFERENCED_PARAMETER(wParam) ;
	UNREFERENCED_PARAMETER(lParam) ;
#ifdef _DEBUG
	CFile file ;
	CString name ;
	name.Format("%ld.txt" , GetMeChairID()) ;
	file.Open(name , CFile::modeWrite | CFile::modeNoTruncate | CFile::modeCreate) ;
	file.SeekToEnd() ;
	file.Write("\r\n\r\n" , (UINT)strlen("\r\n\r\n")) ;
	file.Write("OnShowCard" , (UINT)(strlen("OnShowCard"))) ;
	file.Close() ;
#endif

    CMD_C_ShowCard AllSegmentCard ;
	memset(&AllSegmentCard , 0 , sizeof(AllSegmentCard)) ;

	BYTE bCardCount ;

	bCardCount = (BYTE)(m_GameClientView.m_AllSegCardControl[2][0].GetCardData(AllSegmentCard.bFrontCard , 3)) ;
	ASSERT(bCardCount==3) ;
	if(3!=bCardCount) return false ;
	m_GameLogic.SortCardList(AllSegmentCard.bFrontCard , bCardCount) ;

	bCardCount = (BYTE)(m_GameClientView.m_AllSegCardControl[2][1].GetCardData(AllSegmentCard.bMidCard , 5)) ;
	ASSERT(bCardCount==5) ;
	if(5!=bCardCount) return false ;
	m_GameLogic.SortCardList(AllSegmentCard.bMidCard , bCardCount) ;

	bCardCount = (BYTE)(m_GameClientView.m_AllSegCardControl[2][2].GetCardData(AllSegmentCard.bBackCard , 5)) ;
	ASSERT(bCardCount==5) ;
	if(5!=bCardCount) return false ;
	m_GameLogic.SortCardList(AllSegmentCard.bBackCard , bCardCount) ;

	if(false==IsLookonMode())
        SendData(SUB_C_SHOWCARD , &AllSegmentCard , sizeof(AllSegmentCard)) ;

	for(BYTE i=0 ; i<3 ; ++i)
	{
		m_GameClientView.m_AllSegCardControl[2][i].ShowWindow(SW_SHOW) ;
		m_GameClientView.m_HandSegCardControl[i].ShowWindow(SW_HIDE) ;
	}

	//设置按钮

	m_GameClientView.m_btShowCard.ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btShowCard.EnableWindow(FALSE) ;
	m_GameClientView.m_btSetChip[0].ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btSetChip[0].EnableWindow(FALSE) ;
	m_GameClientView.m_btSetChip[1].ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btSetChip[1].EnableWindow(FALSE) ;
	m_GameClientView.m_btSetChip[2].ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btSetChip[2].EnableWindow(FALSE) ;
	m_GameClientView.m_btSetChip[3].ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btSetChip[3].EnableWindow(FALSE) ;

	m_GameClientView.m_btFrontCard.ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btFrontCard.EnableWindow(FALSE) ;
	m_GameClientView.m_btMidCard.ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btMidCard.EnableWindow(FALSE) ;
	m_GameClientView.m_btBackCard.ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btBackCard.EnableWindow(FALSE) ;

	m_GameClientView.m_btAutoOutCard.ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btAutoOutCard.EnableWindow(FALSE) ;
	m_GameClientView.m_btDescend.ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btDescend.EnableWindow(FALSE) ;
	m_GameClientView.m_btAscend.ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btAscend.EnableWindow(FALSE) ;
	m_GameClientView.m_btColor.ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btColor.EnableWindow(FALSE) ;

//	m_GameClientView.m_btBaoPai.ShowWindow(SW_HIDE) ;

	m_GameClientView.m_btRestoreCard.ShowWindow(SW_HIDE) ;
    
	KillGameTimer(IDI_SHOW_CARD) ;

#ifdef _DEBUG
	name.Format("%ld.txt" , GetMeChairID()) ;
	file.Open(name , CFile::modeWrite | CFile::modeNoTruncate | CFile::modeCreate) ;
	file.SeekToEnd() ;
	file.Write("\r\n\r\n" , (UINT)strlen("\r\n\r\n")) ;
	file.Write("after OnShowCard" , (UINT)(strlen("OnShowCard"))) ;
	file.Close() ;
#endif

	//播放声音
	PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));

	return true ;
}
//设置后墩
LRESULT CGameClientDlg::OnBackCard(WPARAM wParam, LPARAM lParam)
{
	//无用参数
	UNREFERENCED_PARAMETER(wParam) ;
	UNREFERENCED_PARAMETER(lParam) ;

	CMD_C_SegCard  SegCard ;
	memset(&SegCard , 0 , sizeof(SegCard)) ;

	//获取扑克
	BYTE bCardData[HAND_CARD_COUNT];
	BYTE bShootCount=(BYTE)m_GameClientView.m_UserCardControl[2].GetShootCard(bCardData,HAND_CARD_COUNT);
	//排列扑克
	m_GameLogic.SortCardList(bCardData , bShootCount) ;

	ASSERT(5==bShootCount) ;
	if(5!=bShootCount) 
	{
		AfxMessageBox("错误扑克,请重新选择!") ;
		m_GameClientView.m_UserCardControl[2].ShootAllCard(false) ;
		return true ;
	}

	//删除扑克
	m_GameLogic.RemoveCard(bCardData , bShootCount , m_bHandCardData , 	m_bHandCardCount) ;
	m_bHandCardCount -= bShootCount ;
	//排列扑克
	m_GameLogic.SortCardList(m_bHandCardData , m_bHandCardCount) ;


	//隐藏扑克
	m_GameClientView.m_AllSegCardControl[2][2].ShowWindow(SW_HIDE) ;
	//保存扑克
	m_GameClientView.m_AllSegCardControl[2][2].SetCardData(bCardData , bShootCount) ;

	//设置界面
	m_GameClientView.SetCardData(2 , bCardData , bShootCount , enSegmentCard) ;
	m_GameClientView.SetCardData(2 , m_bHandCardData , m_bHandCardCount , enHandCard) ;
	m_GameClientView.m_SegCardControl[2].ShowWindow(SW_SHOW) ;

	SegCard.SegmentType=enBack ;

	SendData(SUB_C_SEGCARD , &SegCard , sizeof(SegCard)) ;

	//设置按钮
	m_GameClientView.m_btBackCard.EnableWindow(FALSE) ;
	m_GameClientView.m_btBackCard.ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btMidCard.EnableWindow(FALSE) ;
	m_GameClientView.m_btAutoOutCard.ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btAutoOutCard.EnableWindow(FALSE) ;

	if(3==m_bHandCardCount || 5==m_bHandCardCount)
	{
		//显示扑克
//		m_GameClientView.m_AllSegCardControl[2][2].ShowWindow(SW_SHOW) ;

		m_bFinishSegment=true ; 

		//设置扑克
		if(0!=m_GameClientView.m_AllSegCardControl[2][0].GetCardCount())
		{
	//		m_GameClientView.m_AllSegCardControl[2][0].ShowWindow(SW_SHOW) ;
			m_GameClientView.m_AllSegCardControl[2][1].SetCardData(m_bHandCardData , m_bHandCardCount) ;
			m_GameClientView.m_AllSegCardControl[2][1].ShowWindow(SW_HIDE) ;
		}
		else if(0!=m_GameClientView.m_AllSegCardControl[2][1].GetCardCount())
		{
	//		m_GameClientView.m_AllSegCardControl[2][1].ShowWindow(SW_SHOW) ;
			m_GameClientView.m_AllSegCardControl[2][0].SetCardData(m_bHandCardData , m_bHandCardCount) ;
			m_GameClientView.m_AllSegCardControl[2][0].ShowWindow(SW_HIDE) ;
		}
		else AfxMessageBox("OnBackCard：错误扑克！") ;

		//设置扑克
		m_GameClientView.SetCardData(2 , NULL , 0 , enHandCard) ;
		m_GameClientView.SetCardData(2 , NULL , 0 , enSegmentCard) ;

		m_GameClientView.m_SegCardControl[2].ShowWindow(SW_HIDE) ;

		m_GameClientView.m_AllSegCardControl[2][0].GetCardData(bCardData , 3) ;
		m_GameClientView.m_HandSegCardControl[0].SetCardData(bCardData , 3) ;

		m_GameClientView.m_AllSegCardControl[2][1].GetCardData(bCardData , 5) ;
		m_GameClientView.m_HandSegCardControl[1].SetCardData(bCardData , 5) ;

		m_GameClientView.m_AllSegCardControl[2][2].GetCardData(bCardData , 5) ;
		m_GameClientView.m_HandSegCardControl[2].SetCardData(bCardData , 5) ;

		//包牌按钮
//		if(VerifyBaoPai() && IsLookonMode()==false) m_GameClientView.m_btBaoPai.ShowWindow(SW_SHOW) ;

		for(BYTE i=0 ; i<3 ; ++i)
		{
			m_GameClientView.m_HandSegCardControl[i].ShowWindow(SW_SHOW) ;
		}
		if(true==IsLookonMode())
			for(BYTE i=0 ; i<3 ; ++i)
			{
				m_GameClientView.m_HandSegCardControl[i].SetPositively(false) ;
				m_GameClientView.m_HandSegCardControl[i].SetDisplayFlag(false) ;
			}


	
		//设置按钮
		if(TRUE==m_GameClientView.m_btFrontCard.IsWindowVisible())
			m_GameClientView.m_btFrontCard.ShowWindow(SW_HIDE) ;
		if(TRUE==m_GameClientView.m_btMidCard.IsWindowVisible())
			m_GameClientView.m_btMidCard.ShowWindow(SW_HIDE) ;
		m_GameClientView.m_btShowCard.ShowWindow(TRUE) ;
		m_GameClientView.m_btShowCard.EnableWindow(TRUE) ;

		m_GameClientView.m_btDescend.ShowWindow(SW_HIDE) ;
		m_GameClientView.m_btDescend.EnableWindow(FALSE) ;
		m_GameClientView.m_btAscend.ShowWindow(SW_HIDE) ;
		m_GameClientView.m_btAscend.EnableWindow(FALSE) ;
		m_GameClientView.m_btColor.ShowWindow(SW_HIDE) ;
		m_GameClientView.m_btColor.EnableWindow(FALSE) ;
	}

	m_GameClientView.m_btRestoreCard.ShowWindow(SW_SHOW) ;

	//播放声音
	PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));

	return true ;
}

//设置下注
bool CGameClientDlg::OnSubSetChip(const void * pBuffer , WORD wDataSize)
{
#ifdef _DEBUG
	CFile file ;
	CString name ;
	name.Format("%ld.txt" , GetMeChairID()) ;
	if(false==g_bNewStart)
        file.Open(name , CFile::modeWrite | CFile::modeNoTruncate | CFile::modeCreate) ;
	else
		file.Open(name , CFile::modeWrite | CFile::modeCreate) ;
	file.SeekToEnd() ;
	file.Write("\r\n\r\n" , (UINT)strlen("\r\n\r\n")) ;
	file.Write("CGameClientDlg::OnSubSetChip(const void * pBuffer , WORD wDataSize)" , (UINT)(strlen("CGameClientDlg::OnSubSetChip(const void * pBuffer , WORD wDataSize"))) ;
	file.Close() ;
	g_bNewStart = false ;
#endif
	//数据验证
	ASSERT(wDataSize==(sizeof(CMD_S_SetChip))) ;
	if(sizeof(CMD_S_SetChip)!=wDataSize) return false ;
	CMD_S_SetChip *pSetChip = (CMD_S_SetChip*)pBuffer ;

	m_nChip[pSetChip->wCurrentUser] = pSetChip->nChip ;

	m_GameClientView.SetChip(SwitchViewChairID(pSetChip->wCurrentUser) , pSetChip->nChip) ;

	m_GameClientView.m_UserCardControl[2].ShootAllCard(false) ;

#ifdef _DEBUG
	CString str ;
	str.Format("%ld" , pSetChip->wCurrentUser) ;
	str += CString("CGameClientDlg::OnSubSetChip(const void * pBuffer , WORD wDataSize") ;
	name.Format("%ld.txt" , GetMeChairID()) ;
	file.Open(name , CFile::modeWrite | CFile::modeNoTruncate | CFile::modeCreate) ;
	file.SeekToEnd() ;
	file.Write("\r\n" , (UINT)strlen("\r\n")) ;
	file.Write(str.GetBuffer() , str.GetLength()) ;
	str.ReleaseBuffer(str.GetLength()) ;
	file.Close() ;
#endif
	return true ;
}

//设置分段
bool CGameClientDlg::OnSubSetSegment(const void * pBuffer , WORD wDataSize)
{
#ifdef _DEBUG
	CFile file ;
	CString name ;
	name.Format("%ld.txt" , GetMeChairID()) ;
	file.Open(name , CFile::modeWrite | CFile::modeNoTruncate | CFile::modeCreate) ;
	file.SeekToEnd() ;
	file.Write("\r\n\r\n" , (UINT)strlen("\r\n\r\n")) ;
	file.Write("CGameClientDlg::OnSubSetSegment(const void * pBuffer , WORD wDataSize" , (UINT)(strlen("CGameClientDlg::OnSubSetSegment(const void * pBuffer , WORD wDataSize"))) ;
	file.Close() ;
#endif

	//数据验证
	ASSERT(sizeof(CMD_S_SetSegment)==wDataSize) ;
	if(sizeof(CMD_S_SetSegment)!=wDataSize) return false ;
    
	CMD_S_SetSegment *pSetSegment = (CMD_S_SetSegment*)pBuffer ;

	//其他玩家只是设置背面
    if(GetMeChairID()!=pSetSegment->wCurrentUser)
	{
		BYTE bCardData[5] ;
		memset(bCardData , 0 , sizeof(bCardData)) ;
		BYTE bCardCount = pSetSegment->SegmentType==enFront ? 3 : 5 ;
        m_GameClientView.SetCardData(SwitchViewChairID(pSetSegment->wCurrentUser) , bCardData , bCardCount , enSegmentCard) ;
	}
#ifdef _DEBUG
	CString str ;
	str.Format("%ld" , pSetSegment->wCurrentUser) ;
	str += CString("CGameClientDlg::OnSubSetSegment(const void * pBuffer , WORD wDataSize:") ;
	name.Format("%ld.txt" , GetMeChairID()) ;
	file.Open(name , CFile::modeWrite | CFile::modeNoTruncate | CFile::modeCreate) ;
	file.SeekToEnd() ;
	file.Write("\r\n" , (UINT)strlen("\r\n")) ;
	file.Write(str.GetBuffer() , str.GetLength()) ;
	str.ReleaseBuffer(str.GetLength()) ;
	file.Close() ;
#endif
	return true ;
}

//玩家摊牌
bool CGameClientDlg::OnSubShowCard(const void * pBuffer , WORD wDataSize)
{
#ifdef _DEBUG
	CFile file ;
	CString name ;
	name.Format("%ld.txt" , GetMeChairID()) ;
	file.Open(name , CFile::modeWrite | CFile::modeNoTruncate | CFile::modeCreate) ;
	file.SeekToEnd() ;
	file.Write("\r\n\r\n" , (UINT)strlen("\r\n\r\n")) ;
	file.Write("CGameClientDlg::OnSubShowCard(const void * pBuffer , WORD wDataSize" , (UINT)(strlen("CGameClientDlg::OnSubShowCard(const void * pBuffer , WORD wDataSize"))) ;
	file.Close() ;
#endif
	//数据验证
	ASSERT(sizeof(CMD_S_ShowCard)==wDataSize) ;
	if(sizeof(CMD_S_ShowCard)!=wDataSize) return false ;

	CMD_S_ShowCard *pShowCard = (CMD_S_ShowCard*)pBuffer ;

	WORD wChiarID = SwitchViewChairID(pShowCard->wCurrentUser) ;

	if(true==pShowCard->bCanSeeShowCard)
	{

		//设置扑克
		m_GameClientView.SetCardData(wChiarID , pShowCard->bFrontCard , 3 , enAllSegmentCard , 0) ;
		m_GameClientView.SetCardData(wChiarID , pShowCard->bMidCard , 5 , enAllSegmentCard , 1) ;
		m_GameClientView.SetCardData(wChiarID , pShowCard->bBackCard , 5 , enAllSegmentCard , 2) ;

		if(true==IsLookonMode())
		{
			m_GameClientView.m_AllSegCardControl[wChiarID][0].SetDisplayFlag(false) ;
			m_GameClientView.m_AllSegCardControl[wChiarID][1].SetDisplayFlag(false) ;
			m_GameClientView.m_AllSegCardControl[wChiarID][2].SetDisplayFlag(false) ;
		}

		//保存数据
		CopyMemory(m_bSegmentCard[pShowCard->wCurrentUser][0] , pShowCard->bFrontCard , 3) ;
		CopyMemory(m_bSegmentCard[pShowCard->wCurrentUser][1] , pShowCard->bMidCard , 5) ;
		CopyMemory(m_bSegmentCard[pShowCard->wCurrentUser][2] , pShowCard->bBackCard , 5) ;
	}
	else
	{
		//显示背面
		BYTE bCardData[5] ;
		memset(bCardData , 0 , sizeof(bCardData)) ;
		m_GameClientView.m_AllSegCardControl[wChiarID][0].SetDisplayFlag(false) ;
		m_GameClientView.m_AllSegCardControl[wChiarID][0].SetCardData(bCardData , 3) ;

		m_GameClientView.m_AllSegCardControl[wChiarID][1].SetDisplayFlag(false) ;
		m_GameClientView.m_AllSegCardControl[wChiarID][1].SetCardData(bCardData , 5) ;

		m_GameClientView.m_AllSegCardControl[wChiarID][2].SetDisplayFlag(false) ;
		m_GameClientView.m_AllSegCardControl[wChiarID][2].SetCardData(bCardData , 5) ;
	}
	m_GameClientView.SetCardData(wChiarID , NULL , 0 , enHandCard) ;
	m_GameClientView.SetCardData(wChiarID , NULL , 0 , enSegmentCard) ;

#ifdef _DEBUG
	CString str ;
	str.Format("%ld" , pShowCard->wCurrentUser) ;
	str += CString(":CGameClientDlg::OnSubShowCard(const void * pBuffer , WORD wDataSize") ;
	name.Format("%ld.txt" , GetMeChairID()) ;
	file.Open(name , CFile::modeWrite | CFile::modeNoTruncate | CFile::modeCreate) ;
	file.SeekToEnd() ;
	file.Write("\r\n" , (UINT)strlen("\r\n")) ;
	file.Write(str.GetBuffer() , str.GetLength()) ;
	str.ReleaseBuffer(str.GetLength()) ;
	file.Close() ;
#endif
	return true ;
}


//比较扑克
bool CGameClientDlg::OnSubCompareCard(const void * pBuffer , WORD wDataSize)
{

	//隐藏扑克
	for(WORD i=0 ; i<GAME_PLAYER ; ++i)
        for(BYTE j=0 ; j<3 ; ++j)
			m_GameClientView.m_AllSegCardControl[i][j].ShowWindow(SW_HIDE) ;

	//比较扑克
	for(WORD i=0 ; i<GAME_PLAYER ; ++i)
	{
		const tagUserData * pUserData=GetUserData(i);
		WORD wChairID = SwitchViewChairID(i) ;

		if(NULL!=pUserData)
		{
			if(wChairID!=SwitchViewChairID(m_wBanker))
			{
				//获取扑克
				BYTE bCardData[3] ;
				BYTE bCardCount = (BYTE)(m_GameClientView.m_AllSegCardControl[wChairID][0].GetCardData(bCardData , 3)) ;
#ifdef _DEBUG
				if(3!=bCardCount)
				{
					CString str ;
					str.Format(":%d" , bCardCount) ;
					str = CString(pUserData->szName) + str ;
					AfxMessageBox(str) ;
				}
#endif
				ASSERT(3==bCardCount) ;
				if(3!=bCardCount) return false ;
				m_GameClientView.SetCardData(wChairID , bCardData , bCardCount , enCompareCard) ;

				//庄家扑克
				WORD wBanker = SwitchViewChairID(m_wBanker) ;
				bCardCount = (BYTE)(m_GameClientView.m_AllSegCardControl[wBanker][0].GetCardData(bCardData , 3)) ;
				ASSERT(3==bCardCount) ;
				if(3!=bCardCount) return false ;
				m_GameClientView.SetCardData(wChairID , bCardData , bCardCount , enBankCard) ;
			}
		}

	}
	//显示信息
	m_GameClientView.SetShowCompare(true , enFront) ;
	m_GameClientView.ShowBankerName(true) ;

	//计算结果
	CString strWindMsg="" ;
//	InsertShtickString("前墩比较结果:" , RGB(255,0,0) , true);
	const tagUserData *pBanker = GetUserData(m_wBanker) ;
	for(WORD i=0 ; i<GAME_PLAYER ; ++i)
	{
		strWindMsg="" ;
		const tagUserData * pUserData=GetUserData(i);

		if(NULL!=pUserData)
		{
			if(i!=m_wBanker)
			{
				const tagUserData * pUserData=GetUserData(i);
				//闲家赢
				if(true==m_GameLogic.CompareCard(m_bSegmentCard[m_wBanker][0] , m_bSegmentCard[i][0] , 3 , 3 , true)) 
				{
					strWindMsg = (CString(pUserData->szName) + CString(" 赢 庄家") + CString(pBanker->szName)) ;
				}
				//庄家赢
				else
				{
					strWindMsg = (CString("庄家[ ") + CString(pBanker->szName) + CString(" ] 赢 [ ") + CString(pUserData->szName) + " ]") ;
				}

		//		InsertShtickString(strWindMsg , RGB(0,0,255) , true) ;
			}
			else
			{
				for(BYTE bSeg=0 ; bSeg<3 ; ++bSeg)
					m_GameClientView.m_AllSegCardControl[SwitchViewChairID(m_wBanker)][bSeg].ShowWindow(SW_SHOW) ;
			}
		}
	}

	//针对旁观
	if(true==IsLookonMode())
	{
		WORD  wChairID = SwitchViewChairID(m_wBanker) ;

		m_GameClientView.m_AllSegCardControl[wChairID][0].SetDisplayFlag(true) ;
		m_GameClientView.m_AllSegCardControl[wChairID][1].SetDisplayFlag(true) ;
		m_GameClientView.m_AllSegCardControl[wChairID][2].SetDisplayFlag(true) ;
	}

	//设置定时器
	m_nTimer=SetTimer(IDI_COMPARE_FRONT , m_bCompareTime , NULL) ;

	return true ;
}

//定时器
void CGameClientDlg::OnTimer(UINT nIDEvent)
{
	if(nIDEvent==IDI_COMPARE_FRONT)		//前墩扑克
	{
		KillTimer(m_nTimer) ;
 		m_nTimer=SetTimer(IDI_COMPARE_MID , m_bCompareTime , NULL) ;

		//隐藏扑克
		for(WORD i=0 ; i<GAME_PLAYER ; ++i)
			for(BYTE j=0 ; j<3 ; ++j)
				m_GameClientView.m_AllSegCardControl[i][j].ShowWindow(SW_HIDE) ;

		//比较扑克
		for(WORD i=0 ; i<GAME_PLAYER ; ++i)
		{
			const tagUserData * pUserData=GetUserData(i);
			WORD wChairID = SwitchViewChairID(i) ;

			if(NULL!=pUserData)
			{
				if(i!=m_wBanker)
				{
					//获取扑克
					BYTE bCardData[5] ;
					BYTE bCardCount = (BYTE)(m_GameClientView.m_AllSegCardControl[wChairID][1].GetCardData(bCardData , 5)) ;
					ASSERT(5==bCardCount) ;
					if(5!=bCardCount) return ; 
					m_GameClientView.SetCardData(wChairID , bCardData , bCardCount , enCompareCard) ;

					//庄家扑克
					WORD wBanker = SwitchViewChairID(m_wBanker) ;
					bCardCount = (BYTE)(m_GameClientView.m_AllSegCardControl[wBanker][1].GetCardData(bCardData , 5)) ;
					ASSERT(5==bCardCount) ;
					if(5!=bCardCount) return ;
					m_GameClientView.SetCardData(wChairID , bCardData , bCardCount , enBankCard) ;
				}
				else
				{
					for(BYTE bSeg=0 ; bSeg<3 ; ++bSeg)
						m_GameClientView.m_AllSegCardControl[SwitchViewChairID(m_wBanker)][bSeg].ShowWindow(SW_SHOW) ;
				}
			}
		}

		//显示信息
		m_GameClientView.SetShowCompare(true , enMid) ;
		//计算结果
		CString strWindMsg="" ;
//		InsertShtickString("中墩比较结果:" , RGB(255,0,0) , true);
		const tagUserData *pBanker = GetUserData(m_wBanker) ;
		for(WORD i=0 ; i<GAME_PLAYER ; ++i)
		{
			strWindMsg="" ;
			const tagUserData * pUserData=GetUserData(i);

			if(NULL!=pUserData)
			{
				if(i!=m_wBanker)
				{
					const tagUserData * pUserData=GetUserData(i);
					//闲家赢
					if(true==m_GameLogic.CompareCard(m_bSegmentCard[m_wBanker][1] , m_bSegmentCard[i][1] , 5 , 5 , true)) 
					{
						strWindMsg = "[ "+(CString(pUserData->szName) + CString(" ] 赢 庄家[ ") + CString(pBanker->szName) + " ]") ;
					}
					//庄家赢
					else
					{					
						strWindMsg = (CString("庄家[ ") + CString(pBanker->szName) + CString(" ] 赢 [ ") + CString(pUserData->szName) + " ]") ;
					}

	//				InsertShtickString(strWindMsg , RGB(0,0,255) , true) ;
				}
			}
		}
		return ;
	}
	if(nIDEvent==IDI_COMPARE_MID)		//中墩扑克
	{
		KillTimer(m_nTimer) ;
		m_nTimer=SetTimer(IDI_COMPARE_BACK , m_bCompareTime , NULL) ;
		//比较扑克
		for(WORD i=0 ; i<GAME_PLAYER ; ++i)
		{
			const tagUserData * pUserData=GetUserData(i);
			WORD wChairID = SwitchViewChairID(i) ;

			if(NULL!=pUserData)
			{
				if(i!=m_wBanker)
				{
					//获取扑克
					BYTE bCardData[5] ;
					BYTE bCardCount = (BYTE)(m_GameClientView.m_AllSegCardControl[wChairID][2].GetCardData(bCardData , 5)) ;
					ASSERT(5==bCardCount) ;
					if(5!=bCardCount) return ;
					m_GameClientView.SetCardData(wChairID , bCardData , bCardCount , enCompareCard) ;

					//庄家扑克
					WORD wBanker = SwitchViewChairID(m_wBanker) ;
					bCardCount = (BYTE)(m_GameClientView.m_AllSegCardControl[wBanker][2].GetCardData(bCardData , 5)) ;
					ASSERT(5==bCardCount) ;
					if(5!=bCardCount) return ;
					m_GameClientView.SetCardData(wChairID , bCardData , bCardCount , enBankCard) ;
				}
				else
				{
					for(BYTE bSeg=0 ; bSeg<3 ; ++bSeg)
						m_GameClientView.m_AllSegCardControl[SwitchViewChairID(m_wBanker)][bSeg].ShowWindow(SW_SHOW) ;
				}
			}
		}
		//显示信息
		m_GameClientView.SetShowCompare(true , enBack) ;
		//计算结果
		CString strWindMsg="" ;
//		InsertShtickString("后墩比较结果:" , RGB(255,0,0) , true);
		const tagUserData *pBanker = GetUserData(m_wBanker) ;
		for(WORD i=0 ; i<GAME_PLAYER ; ++i)
		{
			strWindMsg="" ;
			const tagUserData * pUserData=GetUserData(i);

			if(NULL!=pUserData)
			{
				if(i!=m_wBanker)
				{
					const tagUserData * pUserData=GetUserData(i);
					//闲家赢
					if(true==m_GameLogic.CompareCard(m_bSegmentCard[m_wBanker][2] , m_bSegmentCard[i][2] , 5 , 5 , true)) 
					{
						strWindMsg = "[ " + (CString(pUserData->szName) + CString(" ] 赢 庄家[ ") + CString(pBanker->szName)+" ]") ;
					}
					//庄家赢
					else
					{
						strWindMsg = (CString("庄家[ ") + CString(pBanker->szName) + CString(" ] 赢 [ ") + CString(pUserData->szName) + " ]") ;
					}

	//				InsertShtickString(strWindMsg , RGB(0,0,255) , true) ;
				}
			}
		}
		return  ;
	}
	if(nIDEvent==IDI_COMPARE_BACK)		//后墩扑克
	{
		KillTimer(m_nTimer) ;
		m_nTimer=SetTimer(IDI_COMPLETE_COMPARE , m_bCompareTime , NULL) ;
		//庄稼信息
		const tagUserData * pBankerData=GetUserData(m_wBanker);
		CString strWindMsg , strChip ;

		//比较扑克
		m_GameClientView.SetCardData(INVALID_CHAIR , NULL , 0 , enCompareCard) ;
		m_GameClientView.SetCardData(INVALID_CHAIR , NULL , 0 , enBankCard) ;

		//显示扑克
		for(WORD i=0 ; i<GAME_PLAYER ; ++i)
			for(BYTE j=0 ; j<3 ; ++j)
				m_GameClientView.m_AllSegCardControl[i][j].ShowWindow(SW_SHOW) ;

		//显示信息
		m_GameClientView.SetShowCompare(true , enAllSeg) ;
		m_GameClientView.ShowBankerName(false) ;


		//计算结果
//		InsertShtickString("三墩比较结果:" , RGB(255,0,0) , true);
		int nBankerWinChip = 0 ; 

		bool bBankerDragon = false , //庄家乌龙
			bUserDragon   = false ; //闲家乌龙

		if((true==m_GameLogic.CompareCard(m_bSegmentCard[m_wBanker][0] , m_bSegmentCard[m_wBanker][1] , 3 , 5 , false)) &&
			(true==m_GameLogic.CompareCard(m_bSegmentCard[m_wBanker][1] , m_bSegmentCard[m_wBanker][2] , 5 , 5 , false)))
			bBankerDragon = false ;
		else
			bBankerDragon = true ;

		const tagUserData *pBanker = GetUserData(m_wBanker) ;
		for(WORD i=0 ; i<GAME_PLAYER ; ++i)
		{
			BYTE bWindCount=0 ;
			strWindMsg="" ;
			const tagUserData * pUserData=GetUserData(i);

			if(NULL!=pUserData)
			{
				if(i!=m_wBanker)
				{
					const tagUserData * pUserData=GetUserData(i);

					if(true==m_GameLogic.CompareCard(m_bSegmentCard[m_wBanker][0] , m_bSegmentCard[i][0] , 3 , 3 , true)) ++bWindCount ;
					if(true==m_GameLogic.CompareCard(m_bSegmentCard[m_wBanker][1] , m_bSegmentCard[i][1] , 5 , 5 , true)) ++bWindCount ;
					if(true==m_GameLogic.CompareCard(m_bSegmentCard[m_wBanker][2] , m_bSegmentCard[i][2] , 5 , 5 , true)) ++bWindCount ;

					if((true==m_GameLogic.CompareCard(m_bSegmentCard[i][0] , m_bSegmentCard[i][1] , 3 , 5 , false)) &&
						(true==m_GameLogic.CompareCard(m_bSegmentCard[i][1] , m_bSegmentCard[i][2] , 5 , 5 , false)))
						bUserDragon = false ;
					else
						bUserDragon = true ;

					//摆乌龙
					if(true==bUserDragon && true==bBankerDragon)
					{
						strWindMsg = CString("庄家") + CString(pBanker->szName) + CString("和") +  CString(pUserData->szName) + CString("摆乌龙，和局！") ; 
					}
					else if(true==bBankerDragon)
					{
						strChip.Format(": %d 注" , m_nChip[i]) ;
						strWindMsg = (CString("庄家[ ") + CString(pBanker->szName) + CString(" ]摆乌龙，[ ")+CString(pUserData->szName) + CString(" ] 赢 庄家[ ") + CString(pBanker->szName) +" ]"+ strChip) ;
						nBankerWinChip-=m_nChip[i] ;
					}
					else if(true==bUserDragon)
					{
						strChip.Format(": %d 注" , m_nChip[i]) ;
						strWindMsg = "[ "+(CString(pUserData->szName) + CString(" ]摆乌龙，") + CString("庄家[ ") + CString(pBanker->szName) + CString(" ] 赢 [ ") + CString(pUserData->szName) +" ]"+ strChip) ;
						nBankerWinChip+=m_nChip[i] ;
					}
					//闲家赢
					else if(bWindCount>=2)
					{
						strChip.Format(": %d 注" , m_nChip[i]) ;
						strWindMsg = "[ "+(CString(pUserData->szName) + CString(" ] 赢 庄家[ ") + CString(pBanker->szName)+" ]" + strChip) ;
						nBankerWinChip-=m_nChip[i] ;
					}
					//庄家赢
					else
					{
						strChip.Format(": %d 注" , m_nChip[i]) ;
						strWindMsg = (CString("庄家[ ") + CString(pBanker->szName) + CString(" ] 赢 [ ") + CString(pUserData->szName) +" ]"+ strChip) ;
						nBankerWinChip+=m_nChip[i] ;
					}

			//		InsertShtickString(strWindMsg , RGB(0,0,255) , true) ;			
				}
			}
		}


		//针对旁观
		if(true==IsLookonMode())
		{
			for(WORD i=0 ; i<GAME_PLAYER ; ++i)
			{
				const tagUserData * pUserData=GetUserData(i);
				WORD  wChairID = SwitchViewChairID(i) ;

				if(NULL!=pUserData)
				{
					m_GameClientView.m_AllSegCardControl[wChairID][0].SetDisplayFlag(true) ;
					m_GameClientView.m_AllSegCardControl[wChairID][1].SetDisplayFlag(true) ;
					m_GameClientView.m_AllSegCardControl[wChairID][2].SetDisplayFlag(true) ;
				}
			}
		}
	 return ;
	}
	if(nIDEvent==IDI_COMPLETE_COMPARE)
	{
        KillTimer(m_nTimer) ;	

		//显示信息
		m_GameClientView.SetShowCompare(false) ;

        if(false==IsLookonMode()) SendData(SUB_C_COMPLETE_COMPARE , NULL , 0) ;

		return ;
	}
	__super::OnTimer(nIDEvent) ;
}

/*
 *	算法描述：随机选出正确的十种分段（不摆乌龙），然后计算每种分段的权重，最后选权重最大者
 */
//自动代打
void CGameClientDlg::AutomaticOutCard() 
{
#ifdef _DEBUG
	CFile file ;
	CString name ;
	name.Format("%ld.txt" , GetMeChairID()) ;
	file.Open(name , CFile::modeWrite | CFile::modeNoTruncate | CFile::modeCreate) ;
	file.SeekToEnd() ;
	file.Write("\r\n\r\n" , (UINT)strlen("\r\n\r\n")) ;
	file.Write("1" , (UINT)(strlen("1"))) ;
	file.Close() ;
#endif

	//设置压注
	if(-1==m_nChip[GetMeChairID()])
	{
		m_nChip[2] = min(300,m_wMaxChip) ;
		m_GameClientView.SetChip(2 , m_nChip[2]) ;

		CMD_C_SetChip  SetChip ;
		memset(&SetChip , 0 , sizeof(SetChip)) ;
		SetChip.nChip = m_nChip[2] ;
		SendData(SUB_C_SETCHIP , &SetChip , sizeof(SetChip)) ;
	}

	//恢复扑克
	CopyMemory(m_bHandCardData , m_bTmpHandCardData , sizeof(m_bHandCardData)) ; //防止玩家分了一段或两段后超时调用此函数
	m_bHandCardCount = sizeof(m_bHandCardData) ;
	m_GameClientView.m_UserCardControl[2].SetCardData(m_bHandCardData , m_bHandCardCount) ;

#ifdef _DEBUG
	name.Format("%ld.txt" , GetMeChairID()) ;
	file.Open(name , CFile::modeWrite | CFile::modeNoTruncate | CFile::modeCreate) ;
	file.SeekToEnd() ;
	file.Write("\r\n\r\n" , (UINT)strlen("\r\n\r\n")) ;
	file.Write("2" , (UINT)(strlen("2"))) ;
	file.Close() ;
#endif

	BYTE bCardCount = (BYTE)(m_GameClientView.m_UserCardControl[2].GetCardCount()) ;

	m_GameClientView.m_UserCardControl[2].ShootAllCard(false) ;

	const BYTE SEARCHE_COUNT = 10 ;
	ASSERT(bCardCount==13) ;
	if(bCardCount!=13) return ;
	BYTE bAllSegmentCardIndex[SEARCHE_COUNT][3][5] ;			//分段扑克
	
	BYTE bSegmentScore[SEARCHE_COUNT] ;							//分段权重
	BYTE bCardDataIndex[13] = {0,1,2,3,4,5,6,7,8,9,10,11,12} ;	//扑克下标
	BYTE bFrontCardType ,										//前墩类型
		 bMidCardType,											//中墩类型
		 bBackCardType;											//后墩类型
	BYTE bCardData[5] ;
	ZeroMemory(bAllSegmentCardIndex , sizeof(bAllSegmentCardIndex)) ;
	ZeroMemory(bSegmentScore , sizeof(bSegmentScore)) ;

	srand((unsigned)time( NULL ));

#ifdef _DEBUG
	name.Format("%ld.txt" , GetMeChairID()) ;
	file.Open(name , CFile::modeWrite | CFile::modeNoTruncate | CFile::modeCreate) ;
	file.SeekToEnd() ;
	file.Write("\r\n\r\n" , (UINT)strlen("\r\n\r\n")) ;
	file.Write("3" , (UINT)(strlen("3"))) ;
	file.Close() ;
#endif

	int  bSegCount=0 ;
	LONG bSearchCount = 0 ;
	bool bStop = false ;
	while(false==bStop)
	{
		++bSearchCount ;
		BYTE bCardIndex=0 ;										//扑克下标
		//前墩扑克

/*
#ifdef _DEBUG
		name.Format("%ld.txt" , GetMeChairID()) ;
		file.Open(name , CFile::modeWrite | CFile::modeNoTruncate | CFile::modeCreate) ;
		file.SeekToEnd() ;
		file.Write("\r\n\r\n" , (UINT)strlen("\r\n\r\n")) ;
		CString str2 ;
		str2.Format("%d" , bSearchCount) ;
		file.Write(str2 , str2.GetLength()) ;
		file.Close() ;
#endif*/


		for(BYTE bFrontCard=0 ; bFrontCard<3 ; ++bFrontCard)
		{
			bCardIndex = rand()%(13-bFrontCard) ;
			bAllSegmentCardIndex[bSegCount][0][bFrontCard] = bCardDataIndex[bCardIndex] ;
			bCardData[bFrontCard] = m_bHandCardData[bCardDataIndex[bCardIndex]] ;
			bCardDataIndex[bCardIndex] = bCardDataIndex[13-bFrontCard-1] ;
		}
/*
#ifdef _DEBUG
		name.Format("%ld.txt" , GetMeChairID()) ;
		file.Open(name , CFile::modeWrite | CFile::modeNoTruncate | CFile::modeCreate) ;
		file.SeekToEnd() ;
		file.Write("\r\n\r\n" , (UINT)strlen("\r\n\r\n")) ;
		file.Write("b" , (UINT)(strlen("b"))) ;
		file.Close() ;
#endif*/


		m_GameLogic.SortCardList(bCardData , 3 , enDescend) ;
		bFrontCardType = m_GameLogic.GetCardType(bCardData , 3) ;

/*
#ifdef _DEBUG
		name.Format("%ld.txt" , GetMeChairID()) ;
		file.Open(name , CFile::modeWrite | CFile::modeNoTruncate | CFile::modeCreate) ;
		file.SeekToEnd() ;
		file.Write("\r\n\r\n" , (UINT)strlen("\r\n\r\n")) ;
		file.Write("c" , (UINT)(strlen("c"))) ;
		file.Close() ;
#endif*/


		//中墩扑克
		for(BYTE bMidCard=0 ; bMidCard<5 ; ++bMidCard)
		{
			bCardIndex = rand()%(10-bMidCard) ;
			bAllSegmentCardIndex[bSegCount][1][bMidCard] = bCardDataIndex[bCardIndex] ;
			bCardData[bMidCard] = m_bHandCardData[bCardDataIndex[bCardIndex]] ;
			bCardDataIndex[bCardIndex] = bCardDataIndex[10-bMidCard-1] ;
		}
/*
#ifdef _DEBUG
		name.Format("%ld.txt" , GetMeChairID()) ;
		file.Open(name , CFile::modeWrite | CFile::modeNoTruncate | CFile::modeCreate) ;
		file.SeekToEnd() ;
		file.Write("\r\n\r\n" , (UINT)strlen("\r\n\r\n")) ;
		file.Write("d" , (UINT)(strlen("d"))) ;
		file.Close() ;
#endif*/

		m_GameLogic.SortCardList(bCardData , 5 , enDescend) ;
		bMidCardType = m_GameLogic.GetCardType(bCardData , 5) ;

/*
#ifdef _DEBUG
		name.Format("%ld.txt" , GetMeChairID()) ;
		file.Open(name , CFile::modeWrite | CFile::modeNoTruncate | CFile::modeCreate) ;
		file.SeekToEnd() ;
		file.Write("\r\n\r\n" , (UINT)strlen("\r\n\r\n")) ;
		file.Write("e" , (UINT)(strlen("e"))) ;
		file.Close() ;
#endif*/

		//后墩扑克
		for(BYTE bBackCard=0 ; bBackCard<5 ; ++bBackCard)
		{
			bAllSegmentCardIndex[bSegCount][2][bBackCard] = bCardDataIndex[bBackCard] ;
			bCardData[bBackCard] = m_bHandCardData[bCardDataIndex[bBackCard]] ;
		}
		m_GameLogic.SortCardList(bCardData , 5 , enDescend) ;

/*
#ifdef _DEBUG
		name.Format("%ld.txt" , GetMeChairID()) ;
		file.Open(name , CFile::modeWrite | CFile::modeNoTruncate | CFile::modeCreate) ;
		file.SeekToEnd() ;
		file.Write("\r\n\r\n" , (UINT)strlen("\r\n\r\n")) ;
		file.Write("f" , (UINT)(strlen("f"))) ;
		file.Close() ;
#endif*/

		bBackCardType = m_GameLogic.GetCardType(bCardData , 5) ;


		if(bBackCardType>bMidCardType && bMidCardType>bFrontCardType) 
		{
			bSegmentScore[bSegCount] = bFrontCardType+bMidCardType+bBackCardType ;
			bSegCount++;				
		}

		//恢复数据
		for(BYTE i=0 ; i<13 ; ++i)
			bCardDataIndex[i] = i ;

		//停止搜索
		if(bSegCount>=SEARCHE_COUNT || bSearchCount>=10000) bStop = true ;
		//搜到一个
		if(true==bStop && 0==bSegCount) bStop = false ; 

		//搜索不到
		if(bSearchCount>=100000)
		{
#ifdef _DEBUG
			name.Format("%ld.txt" , GetMeChairID()) ;
			file.Open(name , CFile::modeWrite | CFile::modeNoTruncate | CFile::modeCreate) ;
			file.SeekToEnd() ;
			file.Write("\r\n\r\n" , (UINT)strlen("\r\n\r\n")) ;
			file.Write("bSearchCount>=10000" , (UINT)(strlen("bSearchCount>=10000"))) ;
			file.Close() ;
#endif

			BYTE bIndex=0 ; 
			for(BYTE i=0 ; i<3 ; ++i)
				for(BYTE j=0 ; j<5 ; ++j)
					bAllSegmentCardIndex[0][i][j]=bIndex++ ;
			bStop = true ;
			break ;
		}
	}

#ifdef _DEBUG
	name.Format("%ld.txt" , GetMeChairID()) ;
	file.Open(name , CFile::modeWrite | CFile::modeNoTruncate | CFile::modeCreate) ;
	file.SeekToEnd() ;
	file.Write("\r\n\r\n" , (UINT)strlen("\r\n\r\n")) ;
	file.Write("4" , (UINT)(strlen("4"))) ;
	file.Close() ;
#endif

	//最大权重
	BYTE bMaxScore=bSegmentScore[0] ;
	BYTE bIndex=0 ;
	for(BYTE i=0 ; i<bSegCount ; ++i)
		if(bMaxScore<bSegmentScore[i])
		{
			bIndex=i ;
            bMaxScore=bSegmentScore[i] ;
		}


	//保留扑克
	BYTE bHandCardData[13] ;
	CopyMemory(bHandCardData , m_bHandCardData , 13) ;
	//设置前墩
	m_GameClientView.m_UserCardControl[2].SetShootCard(bAllSegmentCardIndex[bIndex][0] , 3) ;
	OnFrontCard(0 , 0) ;

#ifdef _DEBUG
	name.Format("%ld.txt" , GetMeChairID()) ;
	file.Open(name , CFile::modeWrite | CFile::modeNoTruncate | CFile::modeCreate) ;
	file.SeekToEnd() ;
	file.Write("\r\n\r\n" , (UINT)strlen("\r\n\r\n")) ;
	file.Write("5" , (UINT)(strlen("5"))) ;
	file.Close() ;
#endif

	BYTE bNewIndex[5] ;
	for(BYTE i=0 ; i<5 ; ++i)
	{
		for(BYTE j=0 ; j<10 ; ++j)
			if(m_bHandCardData[j]==bHandCardData[bAllSegmentCardIndex[bIndex][1][i]])
			{
				bNewIndex[i] = j ;
				break ;
			}
	}
	//设置中墩
	m_GameClientView.m_UserCardControl[2].SetShootCard(bNewIndex , 5) ;
	OnMidCard(0 , 0) ;

#ifdef _DEBUG
	name.Format("%ld.txt" , GetMeChairID()) ;
	file.Open(name , CFile::modeWrite | CFile::modeNoTruncate | CFile::modeCreate) ;
	file.SeekToEnd() ;
	file.Write("\r\n\r\n" , (UINT)strlen("\r\n\r\n")) ;
	file.Write("6" , (UINT)(strlen("6"))) ;
	file.Close() ;
#endif
	

	//摊牌
//	OnShowCard(0 , 0) ;
}

//降序排列
LRESULT	CGameClientDlg::OnDescendSort(WPARAM wParam, LPARAM lParam) 
{
	if(m_bHandCardCount<1) return false ;

	m_GameLogic.SortCardList(m_bHandCardData , m_bHandCardCount , enDescend) ;
	m_GameClientView.SetCardData(2 , m_bHandCardData , m_bHandCardCount , enHandCard) ;

	//按钮设置
	m_GameClientView.m_btFrontCard.EnableWindow(FALSE) ;
	m_GameClientView.m_btMidCard.EnableWindow(FALSE) ;
	m_GameClientView.m_btBackCard.EnableWindow(FALSE) ;

	return true ;
}
//升序排列
LRESULT	CGameClientDlg::OnAscendSort(WPARAM wParam, LPARAM lParam) 
{
	if(m_bHandCardCount<1) return false ;

	m_GameLogic.SortCardList(m_bHandCardData , m_bHandCardCount , enAscend) ;
	m_GameClientView.SetCardData(2 , m_bHandCardData , m_bHandCardCount , enHandCard) ;

	//按钮设置
	m_GameClientView.m_btFrontCard.EnableWindow(FALSE) ;
	m_GameClientView.m_btMidCard.EnableWindow(FALSE) ;
	m_GameClientView.m_btBackCard.EnableWindow(FALSE) ;

	return true ;
}
//花色排列
LRESULT	CGameClientDlg::OnColorSort(WPARAM wParam, LPARAM lParam) 
{
	if(m_bHandCardCount<1) return false ;

	m_GameLogic.SortCardList(m_bHandCardData , m_bHandCardCount , enColor) ;
	m_GameClientView.SetCardData(2 , m_bHandCardData , m_bHandCardCount , enHandCard) ;
	//按钮设置
	m_GameClientView.m_btFrontCard.EnableWindow(FALSE) ;
	m_GameClientView.m_btMidCard.EnableWindow(FALSE) ;
	m_GameClientView.m_btBackCard.EnableWindow(FALSE) ;

	return true ;
}

//自动代打
LRESULT CGameClientDlg::OnAutoOutCard(WPARAM wParam, LPARAM lParam)
{
	//无用参数
	UNREFERENCED_PARAMETER(wParam) ;
	UNREFERENCED_PARAMETER(lParam) ;

	m_GameClientView.m_btSetChip[0].ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btSetChip[0].EnableWindow(FALSE) ;
	m_GameClientView.m_btSetChip[1].ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btSetChip[1].EnableWindow(FALSE) ;
	m_GameClientView.m_btSetChip[2].ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btSetChip[2].EnableWindow(FALSE) ;
	m_GameClientView.m_btSetChip[3].ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btSetChip[3].EnableWindow(FALSE) ;

	m_GameClientView.m_btFrontCard.ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btFrontCard.EnableWindow(FALSE) ;
	m_GameClientView.m_btMidCard.ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btMidCard.EnableWindow(FALSE) ;
	m_GameClientView.m_btBackCard.ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btBackCard.EnableWindow(FALSE) ;

	m_GameClientView.m_btRestoreCard.ShowWindow(SW_SHOW) ;

	AutomaticOutCard() ;

	return true ;
}

//摊牌时间
bool CGameClientDlg::OnSubShowCardTime(const void * pBuffer , WORD wDataSize)
{
	//数据验证
	ASSERT(sizeof(CMD_S_ShowCardTime)==wDataSize) ;
	if(sizeof(CMD_S_ShowCardTime)!=wDataSize) return false ;

	CMD_S_ShowCardTime *pShowCardTime = (CMD_S_ShowCardTime*)pBuffer ;
	m_bShowCardTime=pShowCardTime->bShowCardTime ;
	m_wMaxChip = pShowCardTime->wMaxChip ;

	return true ;
}
//////////////////////////////////////////////////////////////////////////
//恢复扑克
LRESULT CGameClientDlg::OnRestoreCard(WPARAM wParam, LPARAM lParam) 
{
	//不用参数
	UNREFERENCED_PARAMETER(wParam) ;
	UNREFERENCED_PARAMETER(lParam) ;

	//恢复数据
	CopyMemory(m_bHandCardData, m_bTmpHandCardData, sizeof(m_bHandCardData)) ;
	m_bHandCardCount = 13 ;

	//设置按钮
	m_GameClientView.m_btColor.ShowWindow(SW_SHOW) ;
	m_GameClientView.m_btColor.EnableWindow(TRUE);

	m_GameClientView.m_btAscend.ShowWindow(SW_SHOW) ;
	m_GameClientView.m_btAscend.EnableWindow(TRUE);

	m_GameClientView.m_btDescend.ShowWindow(SW_SHOW) ;
	m_GameClientView.m_btDescend.EnableWindow(TRUE);

	m_GameClientView.m_btFrontCard.ShowWindow(SW_SHOW) ;

	m_GameClientView.m_btMidCard.ShowWindow(SW_SHOW) ;

	m_GameClientView.m_btBackCard.ShowWindow(SW_SHOW) ;

	m_GameClientView.m_btAutoOutCard.ShowWindow(SW_SHOW) ;
	m_GameClientView.m_btAutoOutCard.EnableWindow(TRUE);
    
	m_GameClientView.m_btShowCard.ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btRestoreCard.ShowWindow(SW_HIDE) ;

	//设置扑克
	m_GameClientView.m_UserCardControl[2].SetCardData(m_bHandCardData, 13) ;
	m_GameClientView.m_AllSegCardControl[2][0].SetCardData(NULL, 0) ;
	m_GameClientView.m_AllSegCardControl[2][1].SetCardData(NULL, 0) ;
	m_GameClientView.m_AllSegCardControl[2][2].SetCardData(NULL, 0) ;

	//分段扑克
	m_GameClientView.SetCardData(2, NULL, 0, enSegmentCard) ;
	m_GameClientView.m_HandSegCardControl[2].SetCardData(NULL, 0) ;
	m_GameClientView.m_HandSegCardControl[1].SetCardData(NULL, 0) ;
	m_GameClientView.m_HandSegCardControl[0].SetCardData(NULL, 0) ;
     
	//设置变量
	m_bFinishSegment = false ;

	return 0 ;
}

//叫庄消息
LRESULT CGameClientDlg::OnCallBanker(WPARAM wParam, LPARAM lParam)
{
	//发送消息
	SendData( SUB_C_CALL_BANKER ) ;

	//删除定时器
	KillGameTimer( IDI_CALL_BANKER ) ;

	//设置按钮
	m_GameClientView.m_btNoCallBanker.ShowWindow( SW_HIDE ) ;
	m_GameClientView.m_btCallBanker.ShowWindow( SW_HIDE ) ;

	return true ;
}

//不叫消息
LRESULT CGameClientDlg::OnNoCallBanker(WPARAM wParam, LPARAM lParam)
{
	//发送消息
	SendData( SUB_C_NO_CALL_BANKER ) ;

	//删除定时器
	KillGameTimer( IDI_CALL_BANKER ) ;
	
	//设置按钮
	m_GameClientView.m_btNoCallBanker.ShowWindow( SW_HIDE ) ;
	m_GameClientView.m_btCallBanker.ShowWindow( SW_HIDE ) ;

	return true ;
}

//获取下注
void CGameClientDlg::GetChips( LONG lChipArray[SET_CHIP_COUNT] )
{
	//最大下注
	LONG lBankerScore, lMeScore, lMaxChip ;
	lBankerScore = GetUserData( m_wBanker )->lScore ;
	lMeScore = GetUserData( GetMeChairID() )->lScore ;

	lMaxChip = lMeScore <= ( lBankerScore / 3 ) ? ( lMeScore / 9 ) :  ( lBankerScore / 9 ) ;

	for ( BYTE cbChipIndex = 0; cbChipIndex < SET_CHIP_COUNT; ++cbChipIndex )
	{
		lChipArray[cbChipIndex] = LONG ( ( cbChipIndex + 1.0 ) / SET_CHIP_COUNT * lMaxChip ) ;

		//整百处理
		if ( lChipArray[cbChipIndex] > 100 )
		{
			LONG lHundredCount = lChipArray[cbChipIndex] / 100 ;
			lChipArray[cbChipIndex] = lHundredCount * 100 ;
		}
	}
}
