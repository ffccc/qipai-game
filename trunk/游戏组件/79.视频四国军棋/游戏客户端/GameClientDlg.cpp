#include "Stdafx.h"
#include "GameClient.h"
#include "SaveChessDlg.h"
#include "GameOption.h"

//////////////////////////////////////////////////////////////////////////
//�궨��

#define IDI_GAME_TIMER				100									//��Ϸ��ʱ��

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientDlg, CGameFrameDlg)
	ON_WM_TIMER()
	ON_MESSAGE(IDM_PLACEMENT,OnHitPlacement)
	ON_MESSAGE(WM_HIT_CHESS_BORAD , OnHitChessBoard)
	ON_MESSAGE(WM_CHANGE_CURSOR , OnChangeCursor)
	ON_MESSAGE(WM_RESET_GAME , OnHitResetGame)
	ON_MESSAGE(WM_SAVE_PLACEMENT , OnHitSavePlacement)
	ON_MESSAGE(WM_LOAD_PLACEMENT , OnHitLoadPlacement)
	ON_MESSAGE(IDM_PREV_STEP,OnHitPrevStep)
	ON_MESSAGE(IDM_NEXT_STEP,OnHitNextStep)
	ON_MESSAGE(IDM_TURN_CHESS,OnHitTurn)
	ON_MESSAGE(IDM_LOAD_HISTORY,OnHitLoadHistory)
	ON_MESSAGE(IDM_LOSE,OnHitLose)
	ON_MESSAGE(IDM_GIVEUP,OnHitGiveUp)
	ON_MESSAGE(IDM_PEACE,OnHitPeace)
							
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientDlg::CGameClientDlg() : CGameFrameDlg(&m_GameClientView)
{
	//���ñ���

	//��Ϸ����
	m_bChessColor=CHESS_COLOR_NO;
	m_wCurrentUser=INVALID_CHAIR;
	ZeroMemory(m_Chess , sizeof(m_Chess)) ;
	m_bFinishPlacement=false ;
	m_bGameSart=false ;
	m_bPlayerCount=0 ;
	m_lCurrentStep=0 ; 
	m_bDeasilOrder = false ;
	m_lCurrentStepCount=0 ;

	//��������
	m_bXSourcePos=255;
	m_bYSourcePos=255;
	m_bXTargetPos=255;
	m_bYTargetPos=255;
	m_bChooseChess=false ;

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
	SetWindowText(TEXT("��Ƶ�Ĺ�����"));

	//����ͼ��
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);

	//��ȡ����
	m_bDeasilOrder = (AfxGetApp()->GetProfileInt(TEXT("GameOption"),TEXT("DeasilOrder") , false)) ? true : false ;
	m_GameClientView.SetUserOrder(m_bDeasilOrder) ;

	return true;
}

//���ÿ��
void CGameClientDlg::ResetGameFrame()
{
	//��������
	m_bXSourcePos=255;
	m_bYSourcePos=255;
	m_bXTargetPos=255;
	m_bYTargetPos=255;
	m_lCurrentStep=0 ; 

	//��Ϸ����
	m_wCurrentUser=INVALID_CHAIR;
	m_bChooseChess=false ;
	m_bGameSart=false ;
	m_bPlayerCount=0 ;
	
	for(BYTE x=0 ; x<17 ; ++x)
		for(BYTE y=0 ; y<17 ; ++y)
		{
			//�Լ�����
			if(x>=6 && x<=10 && y>=11 && y<=16)
				continue ;
			m_Chess[x][y].bChessID   = CHESS_NULL ;
			m_Chess[x][y].bColor     = CHESS_COLOR_NO ;
			m_Chess[x][y].bChessMode = CHESS_NO ;
		}

	return;
}

//��Ϸ����
void CGameClientDlg::OnGameOptionSet()
{
	//��������
	CGameOption GameOption;
	GameOption.m_bEnableSound = IsEnableSound();
	GameOption.m_bDeasilOrder = m_bDeasilOrder ;

	//��������
	if (GameOption.DoModal()==IDOK)
	{

		//���ÿؼ�
		EnableSound(GameOption.m_bEnableSound);
		m_GameClientView.SetUserOrder(GameOption.m_bDeasilOrder);

		if(m_bDeasilOrder!=GameOption.m_bDeasilOrder)
		{
			m_bDeasilOrder = GameOption.m_bDeasilOrder ;

			//�û�����
			tagChessItem tagChess ; 
			for(BYTE x=0 ; x<8 ; ++x)
				for(BYTE y=0 ; y<17 ; ++y)
				{
					tagChess		 = m_Chess[x][y] ;
					m_Chess[x][y]	 = m_Chess[16-x][y] ;
					m_Chess[16-x][y] = tagChess ;
				}

			//���½���
			m_GameClientView.m_ChessBoard.SetChess(m_Chess) ;
			m_GameClientView.SetUserOrder(m_bDeasilOrder) ;
			BYTE bXPos , bYPos ;
			m_GameClientView.GetChessFrame(bXPos , bYPos) ;
			m_GameClientView.DrawChessFrame(16-bXPos , bYPos) ;
			//�滭��ͷ
			if(m_GameLogic.m_ptPathArray.GetCount()>0)
			{
				for(int n=0 ; n<m_GameLogic.m_ptPathArray.GetCount() ; ++n)
				{
					m_GameLogic.m_ptPathArray[n].x = 16 - m_GameLogic.m_ptPathArray[n].x ;
				}
				m_GameClientView.m_ChessBoard.SetPath(&(m_GameLogic.m_ptPathArray)) ;
			}

			//��־ͼ��
			m_GameClientView.m_ChessBoard.ReverseFlagImage() ;
		}

		//��������
		AfxGetApp()->WriteProfileInt(TEXT("GameOption"),TEXT("EnableSound"),GameOption.m_bEnableSound);
		AfxGetApp()->WriteProfileInt(TEXT("GameOption"),TEXT("DeasilOrder"),GameOption.m_bDeasilOrder?TRUE:FALSE);
	}

	return;
}

//ʱ����Ϣ
bool CGameClientDlg::OnTimerMessage(WORD wChairID, UINT nElapse, UINT nTimerID)
{
	switch (nTimerID)
	{
	case IDI_GAME_TIMER:			//��Ϸʱ��
		{
			if (nElapse==0)
			{
				if ((IsLookonMode()==false)&&(wChairID==GetMeChairID())) SendData(SUB_C_GAME_TIME_OUT , NULL , 0);
			
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
	case SUB_S_GAME_START:		//��Ϸ��ʼ
		{
			return OnSubGameStart(pBuffer,wDataSize);
		}
	case SUB_S_FINISH_PLACEMENT://��ɲ���
		{
			return OnSubFinishiPlacement(pBuffer, wDataSize) ;
		}
	case SUB_S_MOVE_CHESS:		//�ƶ�����
		{
			return OnSubMoveChess(pBuffer, wDataSize) ;
		}
	case SUB_S_ONE_LOSE:		//���û��
		{
			return OnSubLoseOne(pBuffer, wDataSize) ;
		}
	case SUB_S_GAME_END:		//��Ϸ����
		{
			return OnSubGameEnd(pBuffer,wDataSize);
		}	
	case SUB_S_GAME_TIME_OUT:	//��ҳ�ʱ
		{
			CMD_S_GameTimeOut *pGameTimeOut=(CMD_S_GameTimeOut *)pBuffer ;
			m_wCurrentUser = pGameTimeOut->wCurrentUser ;
			//���ö�ʱ��
			SetGameTimer(m_wCurrentUser , IDI_GAME_TIMER , 30) ;

			//���ð�ť
			if(GetMeChairID()==m_wCurrentUser)
			{
				m_GameClientView.m_btLose.ShowWindow(SW_SHOW) ;
				m_GameClientView.m_btLose.EnableWindow(TRUE) ;
				m_GameClientView.m_btGiveUp.ShowWindow(SW_SHOW) ;
				m_GameClientView.m_btGiveUp.EnableWindow(TRUE) ;
				m_GameClientView.m_btPeace.ShowWindow(SW_SHOW) ;
				m_GameClientView.m_btPeace.EnableWindow(TRUE) ;
			}
			else
			{
				m_GameClientView.m_btLose.ShowWindow(SW_HIDE) ;
				m_GameClientView.m_btLose.EnableWindow(FALSE) ;
				m_GameClientView.m_btGiveUp.ShowWindow(SW_HIDE) ;
				m_GameClientView.m_btGiveUp.EnableWindow(FALSE) ;
				m_GameClientView.m_btPeace.ShowWindow(SW_HIDE) ;
				m_GameClientView.m_btPeace.EnableWindow(FALSE) ;
			}

			return true ;
		}
	case SUB_S_CHESS_MANUAL:	//��Ϸ����
		{
			return OnSubChessManual(pBuffer,wDataSize);
		}
	case SUB_S_PLACEMENT:		//��ʼ����
		{
			return OnSubPlacement(pBuffer, wDataSize) ;
		}
	case SUB_S_REQUEST_PEACE:	//�������
		{
			CMD_S_RuquestPeace *pRuquestPeace = (CMD_S_RuquestPeace *) pBuffer ;
			const tagUserData *pUserData = GetUserData(pRuquestPeace->wRequestUser) ;
			CString strMsg = CString(pUserData->szName) + CString("������壬���Ƿ�ͬ�⣿") ;
			CMD_C_PeaceAnswer PeaceAnswer ;
			memset(&PeaceAnswer , 0 , sizeof(PeaceAnswer)) ;
			if(MessageBox(strMsg,"��ʾ",MB_YESNO)==IDYES)
			{
				PeaceAnswer.bAgree = true ;				
			}
			else
				PeaceAnswer.bAgree = false ;
			SendData(SUB_C_PEACE_ANSWER , &PeaceAnswer , sizeof(PeaceAnswer)) ;

			return true ;
		}
	case SUB_S_PEACE_FAIL:	//����ʧ��
		{
			MessageBox("����ʧ�ܣ���������壡" , "��ʾ" , MB_ICONASTERISK) ;

			return true ;
		}
	case SUB_S_USER_UP:
		{
			CMD_S_UserUp *pUserUp = (CMD_S_UserUp*)pBuffer ;

			//��������
			BYTE bClientX = 255 , 
				bClientY = 255 ;
			for(BYTE x=0 ; x<5 ; ++x)
				for(BYTE y=0 ; y<6 ; ++y)
				{
					//ʵ������
					BYTE bCurrentColor = pUserUp->wUser+1 ;
					BYTE bRealClientX = 255 ,
						bRealClientY = 255 ;

					switch(bCurrentColor) 
					{
					case CHESS_COLOR_GREEN:	//��ɫ����
						bRealClientX = 10-x ;
						bRealClientY = 5-y ;
						break;

					case CHESS_COLOR_BLACK:	//��ɫ����
						bRealClientX = 11+y ;
						bRealClientY = 10-x ;
						break;

					case CHESS_COLOR_BLUE:	//��ɫ����
						bRealClientX = 6+x ;
						bRealClientY = 11+y ;
						break;

					case CHESS_COLOR_RED:	//��ɫ����
						bRealClientX = 5-y ;
						bRealClientY = 6+x ;
						break;

					default:
						AfxMessageBox("ClientToServer:��������") ;
						bClientX = 255 ;
						bClientY = 255 ;
					}

					ServerToClient(m_bChessColor , bRealClientX , bRealClientY , bClientX , bClientY) ;

					if(false==m_bDeasilOrder)
					{
						bClientX = 16 - bClientX ;
						bClientY = bClientY ;
					}

					ASSERT(255!=bClientX && 255 !=bClientY) ;
					if(255==bClientX || 255==bClientY) return false ;

					m_Chess[bClientX][bClientY].bColor	   = CHESS_COLOR_NO ;
					m_Chess[bClientX][bClientY].bChessMode = CHESS_NO ;
					m_Chess[bClientX][bClientY].bChessID   = CHESS_NULL ;
				}

				//���½���
				m_GameClientView.m_ChessBoard.SetChess(m_Chess) ;

				return true ;
		}

	}

	return false;
}

//��Ϸ��ʼ
bool CGameClientDlg::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	//����У��
	ASSERT(sizeof(CMD_S_GameStart)==wDataSize) ;
	if(sizeof(CMD_S_GameStart)!=wDataSize) return false ;

	CMD_S_GameStart *pGameStart = (CMD_S_GameStart*)pBuffer ;

	m_lCurrentStepCount=0 ;

	//��Ϸ����
	m_bPlayerCount=0 ;
	for(WORD wChairID = 0 ; wChairID <GAME_PLAYER ; ++wChairID)
	{
		const tagUserData *pUserData = GetUserData(wChairID) ;
		if(NULL!=pUserData) ++m_bPlayerCount ;
	}

	m_wCurrentUser = pGameStart->wCurrentUser ;
	m_bChooseChess = false ;
	//���沼��
	for(BYTE x=0 ; x<17 ; ++x)
		for(BYTE y=0 ; y<17 ; ++y)
		{
			//����ת��
			BYTE bServerX = 255 ,
				 bServerY = 255 ;				

			ClientToServer(m_bChessColor , x , y , bServerX , bServerY) ;

			//˳ʱ����
			if(true==m_bDeasilOrder)
			{
				m_HistoryRecord.m_Chess[bServerX][bServerY].bChessID	= m_Chess[x][y].bChessID ;
				m_HistoryRecord.m_Chess[bServerX][bServerY].bColor		= m_Chess[x][y].bColor ;
				m_HistoryRecord.m_Chess[bServerX][bServerY].bChessMode  = m_Chess[x][y].bChessMode ;
			}
			else
			{
				m_HistoryRecord.m_Chess[bServerX][bServerY].bChessID	= m_Chess[16-x][y].bChessID ;
				m_HistoryRecord.m_Chess[bServerX][bServerY].bColor		= m_Chess[16-x][y].bColor ;
				m_HistoryRecord.m_Chess[bServerX][bServerY].bChessMode  = m_Chess[16-x][y].bChessMode ;
			}
		}

#ifdef _DEBUG
	TRACE("OnSubGameStart:\n") ;
	for(BYTE x=0 ; x<17 ; ++x)
		for(BYTE y=0 ; y<17 ; ++y)
			TRACE("(%d , %d):%s\n" , x , y , m_GameLogic.IDToName(m_HistoryRecord.m_Chess[x][y].bChessID)) ;
#endif

	m_HistoryRecord.m_Player = m_bPlayerCount ;
	m_lCurrentStep=0 ; 

	m_bGameSart=true ;

	//�����߿�
	m_GameClientView.DrawChessFrame(255 , 255) ;
	//����·��
	m_GameClientView.m_ChessBoard.SetPath(NULL) ;
	
	//��������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));

	SetGameTimer(m_wCurrentUser , IDI_GAME_TIMER , 30) ;

	//��Ϸ״̬
	SetGameStatus(GS_PLAYING) ;

	//���ð�ť
	if(GetMeChairID()==m_wCurrentUser)
	{
		m_GameClientView.m_btLose.ShowWindow(SW_SHOW) ;
		m_GameClientView.m_btLose.EnableWindow(TRUE) ;
		m_GameClientView.m_btGiveUp.ShowWindow(SW_SHOW) ;
		m_GameClientView.m_btGiveUp.EnableWindow(TRUE) ;
		m_GameClientView.m_btPeace.ShowWindow(SW_SHOW) ;
		m_GameClientView.m_btPeace.EnableWindow(TRUE) ;
	}
	else
	{
		m_GameClientView.m_btLose.ShowWindow(SW_HIDE) ;
		m_GameClientView.m_btLose.EnableWindow(FALSE) ;
		m_GameClientView.m_btGiveUp.ShowWindow(SW_HIDE) ;
		m_GameClientView.m_btGiveUp.EnableWindow(FALSE) ;
		m_GameClientView.m_btPeace.ShowWindow(SW_HIDE) ;
		m_GameClientView.m_btPeace.EnableWindow(FALSE) ;
	}

	m_lCurrentStepCount=0 ; 
	m_GameClientView.SetStepCount(0) ;

	return true;
}


//��Ϸ����
bool CGameClientDlg::OnGameSceneMessage(BYTE cbGameStation, bool bLookonOther, const void * pBuffer, WORD wDataSize)
{
	switch (cbGameStation)
	{
	case GS_FREE:		//����״̬

		m_bChessColor = GetMeChairID()+1 ;

		//�Ѿ�����
		if(true==m_bFinishPlacement)
		{
			OnHitPlacement(0,0) ;
			m_GameClientView.m_ChessBoard.SetChess(m_Chess) ;
			return true ;
		}

		//��ʼ����
		InitCurrentUserChess() ;
		m_GameClientView.m_ChessBoard.SetChess(m_Chess) ;

		return true ;
		
	case GS_PLAYING:		//��Ϸ״̬
		{
			//��ȡ����
			m_bDeasilOrder = (AfxGetApp()->GetProfileInt(TEXT("GameOption"),TEXT("DeasilOrder") , false)) ? true : false ;
			m_GameClientView.SetUserOrder(m_bDeasilOrder) ;
			m_bChessColor = GetMeChairID() + 1 ;

			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pBuffer;
			m_wCurrentUser = pStatusPlay->wCurrentUser ;

			m_wCurrentUser = pStatusPlay->wCurrentUser ;

			BYTE bClientX , bClientY ;
			for(BYTE x=0 ; x<17 ; ++x)
				for(BYTE y=0 ; y<17 ; ++y)
				{
					ServerToClient(m_bChessColor , x , y , bClientX , bClientY) ;

					//��ʱ����
					if(false==m_bDeasilOrder)
					{
						bClientX = 16 - bClientX ;
					}
					m_Chess[bClientX][bClientY].bChessID   = pStatusPlay->Chess[x][y].bChessID ;
					m_Chess[bClientX][bClientY].bColor	   = pStatusPlay->Chess[x][y].bColor ;

					if(CHESS_NULL != m_Chess[bClientX][bClientY].bChessID && CHESS_COLOR_NO != m_Chess[bClientX][bClientY].bColor)
						m_Chess[bClientX][bClientY].bChessMode = CHESS_FORCE ;
					else
						m_Chess[bClientX][bClientY].bChessMode = CHESS_NO ;

					if(m_Chess[bClientX][bClientY].bColor == m_bChessColor && CHESS_NULL!=m_Chess[bClientX][bClientY].bChessID)
					{
						if(false==IsLookonMode())
							m_Chess[bClientX][bClientY].bChessMode = CHESS_FORCE ;
						else
							m_Chess[bClientX][bClientY].bChessMode = CHESS_BACK ;
					}
					else if(m_Chess[bClientX][bClientY].bColor != m_bChessColor && CHESS_NULL!=m_Chess[bClientX][bClientY].bChessID)
						m_Chess[bClientX][bClientY].bChessMode = CHESS_BACK ;
					

				}
			//���ý���
			m_GameClientView.m_ChessBoard.SetChess(m_Chess) ;
			m_lCurrentStepCount = pStatusPlay->lStepCount ;
			m_GameClientView.SetStepCount(m_lCurrentStepCount) ;
			
			SetGameTimer(m_wCurrentUser , IDI_GAME_TIMER , 30) ;

			m_bFinishPlacement = true ;					
			m_bGameSart = true ;						
			//��Ϸ����
			m_bPlayerCount=0 ;
			for(WORD wChairID = 0 ; wChairID <GAME_PLAYER ; ++wChairID)
			{
				const tagUserData *pUserData = GetUserData(wChairID) ;
				if(NULL!=pUserData) ++m_bPlayerCount ;
			}			

			//��ť����
			m_GameClientView.m_btPlacement.ShowWindow(SW_HIDE) ;
			m_GameClientView.m_btPlacement.EnableWindow(FALSE) ;
			m_GameClientView.m_btSavePlacement.ShowWindow(SW_HIDE) ;
			m_GameClientView.m_btSavePlacement.EnableWindow(FALSE) ;
			m_GameClientView.m_btLoadPlacement.ShowWindow(SW_HIDE) ;
			m_GameClientView.m_btLoadPlacement.EnableWindow(FALSE) ;
			m_GameClientView.m_btResetGame.ShowWindow(SW_HIDE) ;
			m_GameClientView.m_btResetGame.EnableWindow(FALSE) ;

			m_GameClientView.m_btLoadHistory.ShowWindow(SW_HIDE) ;
			m_GameClientView.m_btLoadHistory.EnableWindow(FALSE) ;
			m_GameClientView.m_btPrevStep.ShowWindow(SW_HIDE) ;
			m_GameClientView.m_btPrevStep.EnableWindow(FALSE) ;
			m_GameClientView.m_btNextStep.ShowWindow(SW_HIDE) ;
			m_GameClientView.m_btNextStep.EnableWindow(FALSE) ;
			m_GameClientView.m_btTurn.ShowWindow(SW_HIDE) ;
			m_GameClientView.m_btTurn.EnableWindow(FALSE) ;

			return true;
		}
	}

	return false;
}

//��ʱ����Ϣ
void CGameClientDlg::OnTimer(UINT nIDEvent)
{
}


//��ʼ����
void CGameClientDlg::InitCurrentUserChess() 
{
   	m_bChessColor = GetMeChairID() + 1 ;

#ifdef _DEBUG
	CString strMsg ;
	strMsg.Format("User %ld , color %d" , GetMeChairID() , m_bChessColor) ;
//	InsertSystemString(strMsg) ;
#endif

	for(int x=6 ; x<11 ; x++)
	{
		for(int y=11 ; y<17 ; y++)
		{
			//��Ӫ����
			if(true==m_GameLogic.IsBarback(x , y))
			{
				m_Chess[x][y].bColor	 = CHESS_COLOR_NO ;
				m_Chess[x][y].bChessMode = CHESS_NO ;
				m_Chess[x][y].bChessID   = CHESS_NULL ;
			}
			else
			{
				m_Chess[x][y].bColor	=GetMeChairID()+1 ;
				m_Chess[x][y].bChessMode=CHESS_FORCE ;
			}
		}
	}

	m_Chess[6][11].bChessID	 = CHESS_LIAN_ZHANG;			
	m_Chess[7][11].bChessID	 = CHESS_SI_LING;			
	m_Chess[8][11].bChessID	 = CHESS_JUN_ZHANG;			
	m_Chess[9][11].bChessID	 = CHESS_GONG_BING;			
	m_Chess[10][11].bChessID = CHESS_SHI_ZHANG;			
	m_Chess[6][12].bChessID	 = CHESS_YING_ZHANG;			
	m_Chess[8][12].bChessID	 = CHESS_LV_ZHANG;			
	m_Chess[10][12].bChessID = CHESS_TUAN_ZHANG;		
	m_Chess[6][13].bChessID	 = CHESS_TUAN_ZHANG;			
	m_Chess[7][13].bChessID	 = CHESS_ZHA_DAN;			
	m_Chess[9][13].bChessID	 = CHESS_ZHA_DAN;			
	m_Chess[10][13].bChessID = CHESS_LIAN_ZHANG;		
	m_Chess[6][14].bChessID	 = CHESS_YING_ZHANG;			
	m_Chess[8][14].bChessID	 = CHESS_LIAN_ZHANG;			
	m_Chess[10][14].bChessID = CHESS_SHI_ZHANG;			
	m_Chess[6][15].bChessID	 = CHESS_PAI_ZHANG;			
	m_Chess[7][15].bChessID	 = CHESS_DI_LEI;				
	m_Chess[8][15].bChessID	 = CHESS_GONG_BING;			
	m_Chess[9][15].bChessID	 = CHESS_DI_LEI;				
	m_Chess[10][15].bChessID = CHESS_GONG_BING;			

	m_Chess[6][16].bChessID	 = CHESS_LV_ZHANG;			
	m_Chess[7][16].bChessID	 = CHESS_JUN_QI;				
	m_Chess[8][16].bChessID	 = CHESS_DI_LEI;				
	m_Chess[9][16].bChessID	 = CHESS_PAI_ZHANG;			
	m_Chess[10][16].bChessID = CHESS_PAI_ZHANG;		

	//�Թ��û�
	if(true==IsLookonMode())
	{
		for(int x=0;x<11;x++)
		{
			for(int y=11;y<17;y++)
			{	
				if(false==m_GameLogic.IsBarback(x , y))	
                    m_Chess[x][y].bChessMode=CHESS_BACK ;
			}
		}
	}
}

//�������
LRESULT	CGameClientDlg::OnHitPlacement(WPARAM wparam, LPARAM lparam)
{
	UNREFERENCED_PARAMETER(wparam) ;
	UNREFERENCED_PARAMETER(lparam) ;

	//���Ͳ���
	CMD_C_FinishPlacement FinishPlacement ;
	ZeroMemory(&FinishPlacement , sizeof(FinishPlacement)) ;
	FinishPlacement.bColor = m_bChessColor ;

	//���ý���
	m_GameClientView.m_ScoreView.ShowWindow(SW_HIDE);
	m_GameClientView.m_ScoreView.ResetScore() ;

	m_lCurrentStepCount=0 ; 
	m_GameClientView.SetStepCount(0) ;

	//����У��
	ASSERT(m_bChessColor==(GetMeChairID()+1)) ;
	if(m_bChessColor!=GetMeChairID()+1) return false ;

	for(BYTE x=6 ; x<11 ; ++x)
		for(BYTE y=11 ; y<17 ; ++y)
		{
			FinishPlacement.bChessID[x-6][y-11] = m_Chess[x][y].bChessID ;
		}

	//��ʱ����
	if(false==m_bDeasilOrder)
	{
		BYTE bChess ;
		for(BYTE x=0 ; x<2 ; ++x)
			for(BYTE y=0 ; y<6 ; ++y)
			{
				bChess = FinishPlacement.bChessID[x][y] ;
				FinishPlacement.bChessID[x][y] = FinishPlacement.bChessID[4-x][y] ;
				FinishPlacement.bChessID[4-x][y] = bChess ;
			}
	}

	SendData(SUB_C_FINISH_PLACEMENT,&FinishPlacement,sizeof(FinishPlacement));
	SendUserReady(NULL,0);

	//���ñ���
	m_bFinishPlacement=true ;

	//��ť����
	m_GameClientView.m_btPlacement.ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btPlacement.EnableWindow(FALSE) ;
	m_GameClientView.m_btSavePlacement.ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btSavePlacement.EnableWindow(FALSE) ;
	m_GameClientView.m_btLoadPlacement.ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btLoadPlacement.EnableWindow(FALSE) ;
	m_GameClientView.m_btResetGame.ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btResetGame.EnableWindow(FALSE) ;

	m_GameClientView.m_btLoadHistory.ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btLoadHistory.EnableWindow(FALSE) ;
	m_GameClientView.m_btPrevStep.ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btPrevStep.EnableWindow(FALSE) ;
	m_GameClientView.m_btNextStep.ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btNextStep.EnableWindow(FALSE) ;
	m_GameClientView.m_btTurn.ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btTurn.EnableWindow(FALSE) ;

	return true ;	
}

//�������
LRESULT CGameClientDlg::OnHitChessBoard(WPARAM wParam , LPARAM lParam) 
{

	//�Թ��ж�
	if(true==IsLookonMode()) return 0 ;


	BYTE bXPos = (BYTE)wParam ,
		 bYPos = (BYTE)lParam ;
	ASSERT(bXPos>=0 && bXPos<17 && bYPos>=0 && bYPos<17) ;
	if(bXPos<0 || bXPos>=17 || bYPos<0 || bYPos>=17) return false ;
	WORD wChairID = GetMeChairID() ;


	//��������
	if(bXPos>=6 && bXPos<=10 && bYPos>=11 && bYPos<=16 && !m_GameLogic.IsBarback(bXPos , bYPos) && false==m_bFinishPlacement)
	{
		if(false==m_bChooseChess)
		{
			m_bXSourcePos=bXPos;
			m_bYSourcePos=bYPos;

			//�滭�߿�
			m_GameClientView.DrawChessFrame(m_bXSourcePos , m_bYSourcePos) ;
		}
		else
		{
			m_bXTargetPos=bXPos;
			m_bYTargetPos=bYPos;

			//�����߿�
			m_GameClientView.DrawChessFrame(255 , 255) ;
		}

		if(true==m_bChooseChess && m_Chess[m_bXSourcePos][m_bYSourcePos].bColor==m_bChessColor && m_Chess[m_bXTargetPos][m_bYTargetPos].bColor==m_bChessColor)
		{
			if(true==m_GameLogic.SwapChess(m_Chess , m_bXSourcePos , m_bYSourcePos , m_bXTargetPos , m_bYTargetPos , true))
				m_GameClientView.m_ChessBoard.SetChess(m_Chess) ;
		}

		//���ı���
		m_bChooseChess=!m_bChooseChess ;

		//��������
		PlayGameSound(AfxGetInstanceHandle(),TEXT("SELECT_CHESS"));

	}
	//�����ж�
	else if(true==m_bFinishPlacement && true==m_bGameSart && false==m_bChooseChess && GetMeChairID()==m_wCurrentUser && true==m_GameLogic.AbleMove(m_bChessColor , m_Chess , bXPos , bYPos))
	{

		m_bXSourcePos=bXPos;
		m_bYSourcePos=bYPos;

		//�滭�߿�
		m_GameClientView.DrawChessFrame(m_bXSourcePos , m_bYSourcePos) ;

		//���ı���
		m_bChooseChess=!m_bChooseChess ;

		//��������
		PlayGameSound(AfxGetInstanceHandle(),TEXT("SELECT_CHESS"));

	}
	//����ѡ��
	else if(true==m_bFinishPlacement && true==m_bGameSart && true==m_bChooseChess && GetMeChairID()==m_wCurrentUser && true==m_GameLogic.AbleMove(m_bChessColor , m_Chess , bXPos , bYPos))
	{
		m_bXSourcePos=bXPos;
		m_bYSourcePos=bYPos;

		//�滭�߿�
		m_GameClientView.DrawChessFrame(m_bXSourcePos , m_bYSourcePos) ;

		//��������
		PlayGameSound(AfxGetInstanceHandle(),TEXT("SELECT_CHESS"));
	}
	else if(true==m_bFinishPlacement && true==m_bGameSart && true==m_bChooseChess && GetMeChairID()==m_wCurrentUser && true==m_GameLogic.MoveChess(m_bPlayerCount ,  m_Chess , m_bXSourcePos , m_bYSourcePos  , bXPos , bYPos , true))
	{
		if(CHESS_JUN_QI==m_Chess[bXPos][bYPos].bChessID)
			if(false==m_GameLogic.CanCarry(m_Chess[m_bXSourcePos][m_bYSourcePos].bChessID , m_bChessColor , m_Chess))
			{
				InsertShtickString("������С�����������죡" , RGB(255,0,0) , true);
				return false ;
			}
		m_bXTargetPos=bXPos;
		m_bYTargetPos=bYPos;

		//�滭��ͷ
		m_GameClientView.m_ChessBoard.SetPath(&(m_GameLogic.m_ptPathArray)) ;


		//��������
		LONGLONG bCount = m_GameLogic.m_ptPathArray.GetCount() ;
		for (BYTE i = 0 ; i<bCount ; ++i)
		{
			PlayGameSound(AfxGetInstanceHandle(),TEXT("CHESS_MOVE"));
		}
		//�滭�߿�
		m_GameClientView.DrawChessFrame(m_bXTargetPos , m_bYTargetPos) ;

		//���ı���
		m_bChooseChess=!m_bChooseChess ;

		//��������
		CMD_C_MoveChess SendMoveChess ;
		memset(&SendMoveChess , 0 , sizeof(SendMoveChess)) ;
		SendMoveChess.bXSourcePos = m_bXSourcePos ;
		SendMoveChess.bYSourcePos = m_bYSourcePos ;
		SendMoveChess.bXTargetPos = m_bXTargetPos ;
		SendMoveChess.bYTargetPos = m_bYTargetPos ;

		//��ʱ����
		if(false==m_bDeasilOrder)
		{			
				SendMoveChess.bXSourcePos = 16 - SendMoveChess.bXSourcePos ;
				SendMoveChess.bYSourcePos = SendMoveChess.bYSourcePos ;

				SendMoveChess.bXTargetPos = 16 - SendMoveChess.bXTargetPos ;
				SendMoveChess.bYTargetPos = SendMoveChess.bYTargetPos ;
		}

		SendData(SUB_C_MOVE_CHESS,&SendMoveChess,sizeof(SendMoveChess));
	}

	return true ;
}

//�ı����
LRESULT CGameClientDlg::OnChangeCursor(WPARAM wParam , LPARAM lParam) 
{
	//�Թ��ж�
	if(true==IsLookonMode())
	{
		return 0 ;
	}

	BYTE bXPos = (BYTE)wParam ,
		 bYPos = (BYTE)lParam ;
	ASSERT(bXPos>=0 && bXPos<17 && bYPos>=0 && bYPos<17) ;
	if(bXPos<0 || bXPos>=17 || bYPos<0 || bYPos>=17) 
		return false ;

	//��������
	if(bXPos>=6 && bXPos<=10 && bYPos>=11 && bYPos<=16 && !m_GameLogic.IsBarback(bXPos , bYPos) && false==m_bFinishPlacement)
	{
		HCURSOR hCursor;
		hCursor=AfxGetApp()->LoadCursor(IDC_HAND_CURSOR);
		SetCursor(hCursor);

	}
	//�����ж�
	else if(true==m_bFinishPlacement && true==m_bGameSart && false==m_bChooseChess && GetMeChairID()==m_wCurrentUser && true==m_GameLogic.AbleMove(m_bChessColor , m_Chess , bXPos , bYPos))
	{
		HCURSOR hCursor;
		hCursor=AfxGetApp()->LoadCursor(IDC_HAND_CURSOR);
		SetCursor(hCursor);

	}
	else if(true==m_bFinishPlacement && true==m_bGameSart && true==m_bChooseChess && GetMeChairID()==m_wCurrentUser && true==m_GameLogic.MoveChess(m_bPlayerCount ,  m_Chess , m_bXSourcePos , m_bYSourcePos  , bXPos , bYPos , false))
	{
		HCURSOR hCursor;
		hCursor=AfxGetApp()->LoadCursor(IDC_MOVE_CURSOR);
		SetCursor(hCursor);
	}
	else if(true==m_bChooseChess)
	{
		HCURSOR hCursor;
		hCursor=AfxGetApp()->LoadCursor(IDC_FORBID_MOVE_CURSOR);
		SetCursor(hCursor);
	}
	else
	{
		HCURSOR hCursor;
		hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW) ;
		SetCursor(hCursor);
	}

	return true ;
}

//��ɲ���
bool CGameClientDlg::OnSubFinishiPlacement(const void * pDataBuffer, WORD wDataSize) 
{
	//����У��
	ASSERT(sizeof(CMD_S_FinishPlacement)==wDataSize) ;
	if(sizeof(CMD_S_FinishPlacement)!=wDataSize) return false ;

	CMD_S_FinishPlacement *pFinishPlacement = (CMD_S_FinishPlacement*)pDataBuffer ;


	//��������
	BYTE bClientX = 255 , 
		 bClientY = 255 ;
	WORD wChairID = SwitchViewChairID(pFinishPlacement->wCurrentUser) ;
	for(BYTE x=0 ; x<5 ; ++x)
		for(BYTE y=0 ; y<6 ; ++y)
		{
			//ʵ������
			BYTE bCurrentColor = pFinishPlacement->wCurrentUser+1 ;
			BYTE bRealClientX = 255 ,
				 bRealClientY = 255 ;

			switch(bCurrentColor) {
			case CHESS_COLOR_GREEN:	//��ɫ����
				bRealClientX = 10-x ;
				bRealClientY = 5-y ;
				break;

			case CHESS_COLOR_BLACK:	//��ɫ����
				bRealClientX = 11+y ;
				bRealClientY = 10-x ;
				break;

			case CHESS_COLOR_BLUE:	//��ɫ����
				bRealClientX = 6+x ;
				bRealClientY = 11+y ;
				break;

			case CHESS_COLOR_RED:	//��ɫ����
				bRealClientX = 5-y ;
				bRealClientY = 6+x ;
				break;

			default:
				AfxMessageBox("ClientToServer:��������") ;
				bClientX = 255 ;
				bClientY = 255 ;
			}

            ServerToClient(m_bChessColor , bRealClientX , bRealClientY , bClientX , bClientY) ;

			if(false==m_bDeasilOrder)
			{
				bClientX = 16 - bClientX ;
				bClientY = bClientY ;
			}
			
			ASSERT(255!=bClientX && 255 !=bClientY) ;
			if(255==bClientX || 255==bClientY) return false ;

			//��Ӫ����
			if(true==m_GameLogic.IsBarback(bClientX , bClientY))
			{
				m_Chess[bClientX][bClientY].bColor	   = CHESS_COLOR_NO ;
				m_Chess[bClientX][bClientY].bChessMode = CHESS_NO ;
				m_Chess[bClientX][bClientY].bChessID   = CHESS_NULL ;
			}
			else
			{
				m_Chess[bClientX][bClientY].bChessID = pFinishPlacement->bChessID[x][y] ;
				if(GetMeChairID()==pFinishPlacement->wCurrentUser)
					m_Chess[bClientX][bClientY].bChessMode = CHESS_FORCE ;
				else
                	m_Chess[bClientX][bClientY].bChessMode =  CHESS_BACK;// CHESS_FORCE ;
				m_Chess[bClientX][bClientY].bColor = pFinishPlacement->bColor ;
			}
		}

	//���½���
	m_GameClientView.m_ChessBoard.SetChess(m_Chess) ;

	return true ;
}


//�ƶ�����
bool CGameClientDlg::OnSubMoveChess(const void * pDataBuffer, WORD wDataSize)
{
	//����У��
	ASSERT(sizeof(CMD_S_MoveChess)==wDataSize) ;
	if(sizeof(CMD_S_MoveChess)!=wDataSize) return false ;

	CMD_S_MoveChess *pMoveChess = (CMD_S_MoveChess*)pDataBuffer ;

	m_wCurrentUser = pMoveChess->wCurrentUser ;


	//����ת�� 
	BYTE bSourceX = 255 ,
		 bSourceY = 255 ,
		 bTargetX = 255 ,
		 bTargetY = 255 ;

	ServerToClient(m_bChessColor , pMoveChess->bXSourcePos , pMoveChess->bYSourcePos , bSourceX , bSourceY) ;
	ServerToClient(m_bChessColor , pMoveChess->bXTargetPos , pMoveChess->bYTargetPos , bTargetX , bTargetY) ;
	ASSERT(255!=bSourceX && 255!=bSourceY && 255!=bTargetX && 255!=bTargetY) ;

	if(false==m_bDeasilOrder)
	{
		bSourceX = 16 - bSourceX ;
		bSourceY = bSourceY ;
	
		bTargetX = 16 - bTargetX ;
		bTargetY = bTargetY ;
	
	}

	//�Ƚ�����
	enCompareResult CompareResult = m_GameLogic.CompareChess(m_Chess , bSourceX , bSourceY , bTargetX , bTargetY) ;
	//�滭��ͷ
	if(enError!=CompareResult && GetMeChairID()!=pMoveChess->wMoveUser) 
	{
		m_GameLogic.MoveChess(m_bPlayerCount , m_Chess , bSourceX , bSourceY , bTargetX , bTargetY , true) ;
		m_GameClientView.m_ChessBoard.SetPath(&(m_GameLogic.m_ptPathArray)) ;

		//��������
		LONGLONG bCount = m_GameLogic.m_ptPathArray.GetCount() ;
		for(BYTE i = 0 ; i<bCount ; ++i)
		{
			PlayGameSound(AfxGetInstanceHandle(),TEXT("CHESS_MOVE"));
		}
		//�滭�߿�
		m_GameClientView.DrawChessFrame(bTargetX , bTargetY) ;
	}	

	switch(CompareResult)
	{
	case enSmall:
		//��ʾ����
		if(CHESS_SI_LING == m_Chess[bSourceX][bSourceY].bChessID)
		{
            ShowJunQi(m_Chess[bSourceX][bSourceY].bColor) ;
			PlayGameSound(AfxGetInstanceHandle(),TEXT("SHOW_JUN_QI"));
		}

		m_Chess[bSourceX][bSourceY].bChessID   = CHESS_NULL ;
		m_Chess[bSourceX][bSourceY].bColor	   = CHESS_COLOR_NO ;
		m_Chess[bSourceX][bSourceY].bChessMode = CHESS_NO ;

		//���ñ�־
		m_GameClientView.m_ChessBoard.MoveFlagImage(bSourceX , bSourceY , 255 , 255 , false) ;

		//��������
		PlayGameSound(AfxGetInstanceHandle(),TEXT("AO"));
		break;

	case enEqual:
		//��������
		if(CHESS_ZHA_DAN==m_Chess[bSourceX][bSourceY].bChessID || CHESS_ZHA_DAN==m_Chess[bTargetX][bTargetY].bChessID)
			PlayGameSound(AfxGetInstanceHandle(),TEXT("BOMB"));
		else
			PlayGameSound(AfxGetInstanceHandle(),TEXT("LOSE_ONE_CHESS"));	

		//��ʾ����
		if(CHESS_SI_LING == m_Chess[bSourceX][bSourceY].bChessID)
		{
			ShowJunQi(m_Chess[bSourceX][bSourceY].bColor) ;
			PlayGameSound(AfxGetInstanceHandle(),TEXT("SHOW_JUN_QI"));
		}
		//��ʾ����
		if(CHESS_SI_LING == m_Chess[bTargetX][bTargetY].bChessID)
		{
			ShowJunQi(m_Chess[bTargetX][bTargetY].bColor) ;
			PlayGameSound(AfxGetInstanceHandle(),TEXT("SHOW_JUN_QI"));
		}

	
		m_Chess[bSourceX][bSourceY].bChessID   = CHESS_NULL ;
		m_Chess[bSourceX][bSourceY].bColor     = CHESS_COLOR_NO ;
		m_Chess[bSourceX][bSourceY].bChessMode = CHESS_NO ;

		m_Chess[bTargetX][bTargetY].bChessID   = CHESS_NULL ;
		m_Chess[bTargetX][bTargetY].bColor     = CHESS_COLOR_NO ;
		m_Chess[bTargetX][bTargetY].bChessMode = CHESS_NO ;

		//���ñ�־
		m_GameClientView.m_ChessBoard.MoveFlagImage(bSourceX , bSourceY , 255 , 255 , false) ;
		m_GameClientView.m_ChessBoard.MoveFlagImage(bTargetX , bTargetY , 255 , 255 , false) ;
		
		break;

	case enLarge:
		//��������
		if(CHESS_NULL!=m_Chess[bTargetX][bTargetY].bChessID)
            PlayGameSound(AfxGetInstanceHandle(),TEXT("WIN_ONE_CHESS"));
		
		m_Chess[bTargetX][bTargetY].bChessID   = m_Chess[bSourceX][bSourceY].bChessID ; 
		m_Chess[bTargetX][bTargetY].bColor     = m_Chess[bSourceX][bSourceY].bColor ;   
		m_Chess[bTargetX][bTargetY].bChessMode = m_Chess[bSourceX][bSourceY].bChessMode;

		m_Chess[bSourceX][bSourceY].bChessID   = CHESS_NULL ;
		m_Chess[bSourceX][bSourceY].bColor	   = CHESS_COLOR_NO ;
		m_Chess[bSourceX][bSourceY].bChessMode = CHESS_NO ;

		//���ñ�־
		m_GameClientView.m_ChessBoard.MoveFlagImage(bSourceX , bSourceY , bTargetX , bTargetY , true) ;

		break;

	case enError:
		AfxMessageBox("OnSubMoveChess:�������ӣ�") ;
		return false ;

	default:
		return false ;
	}

	//���½���
	m_GameClientView.m_ChessBoard.SetChess(m_Chess) ;

	//��¼��ʷ

	SaveChessStruct ChessStruct ;
	ChessStruct.bSourceX = pMoveChess->bXSourcePos ;
	ChessStruct.bSourceY = pMoveChess->bYSourcePos ;
	ChessStruct.bTargetX = pMoveChess->bXTargetPos ;
	ChessStruct.bTargetY = pMoveChess->bYTargetPos ;
	m_HistoryRecord.AddToList(ChessStruct) ;

	//���ö�ʱ��
	KillGameTimer(IDI_GAME_TIMER) ;
	SetGameTimer(pMoveChess->wCurrentUser , IDI_GAME_TIMER , 30) ;

	//��������
	++m_lCurrentStepCount ;
	//���ò���
	m_GameClientView.SetStepCount(m_lCurrentStepCount) ;

	//���ð�ť
	if(GetMeChairID()==pMoveChess->wCurrentUser)
	{
		m_GameClientView.m_btLose.ShowWindow(SW_SHOW) ;
		m_GameClientView.m_btLose.EnableWindow(TRUE) ;
		m_GameClientView.m_btGiveUp.ShowWindow(SW_SHOW) ;
		m_GameClientView.m_btGiveUp.EnableWindow(TRUE) ;
		m_GameClientView.m_btPeace.ShowWindow(SW_SHOW) ;
		m_GameClientView.m_btPeace.EnableWindow(TRUE) ;
	}
	else
	{
		m_GameClientView.m_btLose.ShowWindow(SW_HIDE) ;
		m_GameClientView.m_btLose.EnableWindow(FALSE) ;
		m_GameClientView.m_btGiveUp.ShowWindow(SW_HIDE) ;
		m_GameClientView.m_btGiveUp.EnableWindow(FALSE) ;
		m_GameClientView.m_btPeace.ShowWindow(SW_HIDE) ;
		m_GameClientView.m_btPeace.EnableWindow(FALSE) ;
	}


	return true ;
}

//���û��
bool CGameClientDlg::OnSubLoseOne(const void * pDataBuffer, WORD wDataSize)
{
	//����У��
	ASSERT(sizeof(CMD_S_OneLose)==wDataSize) ;
	if(sizeof(CMD_S_OneLose)!=wDataSize) return false ;
	
	//��������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("TWO_DIE"));

	CMD_S_OneLose *pOneLose = (CMD_S_OneLose*)pDataBuffer ;
	m_wCurrentUser = pOneLose->wCurrentUser ;
	WORD wLoseUser = pOneLose->wLoseUser ;
	BYTE bChessColor = wLoseUser+1 ;
	for(BYTE x=0 ; x<17 ; ++x)
		for(BYTE y=0 ; y<17 ; ++y)
			if(bChessColor==m_Chess[x][y].bColor)
			{
				m_Chess[x][y].bChessID	 = CHESS_NULL ;
				m_Chess[x][y].bChessMode = CHESS_NO ;
				m_Chess[x][y].bColor	 = CHESS_COLOR_NO ;
			}
	//���½���
	m_GameClientView.m_ChessBoard.SetChess(m_Chess) ;
	
	return true ;
}

//��Ϸ����
bool CGameClientDlg::OnSubGameEnd(const void * pDataBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;

	//��Ϣ����
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pDataBuffer;

	KillGameTimer(IDI_GAME_TIMER) ;

	//����״̬
	SetGameStatus(GS_FREE);

	//��������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_END"));

	//���û���
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		const tagUserData * pUserData=GetUserData(i);

		if(NULL!=pUserData)
			m_GameClientView.m_ScoreView.SetGameScore(i,pUserData->szName,pGameEnd->lUserScore[i]);
		else
			m_GameClientView.m_ScoreView.SetGameScore(i,"",0);
	}

	m_GameClientView.m_ScoreView.ShowWindow(SW_SHOW);
	m_GameClientView.m_ScoreView.CenterWindow(&m_GameClientView) ;

	//���ð�ť

	if(false==IsLookonMode())
	{
		m_GameClientView.m_btResetGame.ShowWindow(SW_SHOW) ;
		m_GameClientView.m_btResetGame.EnableWindow(TRUE) ;

		m_GameClientView.m_btLoadHistory.ShowWindow(SW_SHOW) ;
		m_GameClientView.m_btLoadHistory.EnableWindow(TRUE) ;
		m_GameClientView.m_btPrevStep.ShowWindow(SW_SHOW) ;
		m_GameClientView.m_btPrevStep.EnableWindow(FALSE) ;
		m_GameClientView.m_btNextStep.ShowWindow(SW_SHOW) ;
		m_GameClientView.m_btNextStep.EnableWindow(FALSE) ;
		m_GameClientView.m_btTurn.ShowWindow(SW_SHOW) ;
		m_GameClientView.m_btTurn.EnableWindow(FALSE) ;
	}

	m_GameClientView.m_btLose.ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btLose.EnableWindow(FALSE) ;
	m_GameClientView.m_btGiveUp.ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btGiveUp.EnableWindow(FALSE) ;
	m_GameClientView.m_btPeace.ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btPeace.EnableWindow(FALSE) ;

	//���ñ���
	ZeroMemory(m_Chess , sizeof(m_Chess)) ;

	//�������
	ShowSave() ;

	return true ;
}

//������Ϸ
LRESULT CGameClientDlg::OnHitResetGame(WPARAM wParam , LPARAM lParam) 
{
	//���ý���
	m_GameClientView.m_ScoreView.ShowWindow(SW_HIDE);
	m_GameClientView.m_ScoreView.ResetScore() ;
	//�����߿�
	m_GameClientView.DrawChessFrame(255 , 255) ;
	//������ͷ
	m_GameClientView.m_ChessBoard.SetPath(NULL) ;

	//���ð�ť
	m_GameClientView.m_btPlacement.ShowWindow(SW_SHOW) ;
	m_GameClientView.m_btPlacement.EnableWindow(TRUE) ;
	m_GameClientView.m_btSavePlacement.ShowWindow(SW_SHOW) ;
	m_GameClientView.m_btSavePlacement.EnableWindow(TRUE) ;
	m_GameClientView.m_btLoadPlacement.ShowWindow(SW_SHOW) ;
	m_GameClientView.m_btLoadPlacement.EnableWindow(TRUE) ;
	m_GameClientView.m_btResetGame.ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btResetGame.EnableWindow(FALSE) ;

	m_GameClientView.m_btLoadHistory.ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btLoadHistory.EnableWindow(FALSE) ;
	m_GameClientView.m_btPrevStep.ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btPrevStep.EnableWindow(FALSE) ;
	m_GameClientView.m_btNextStep.ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btNextStep.EnableWindow(FALSE) ;
	m_GameClientView.m_btTurn.ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btTurn.EnableWindow(FALSE) ;

	m_GameClientView.m_btLose.ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btLose.EnableWindow(FALSE) ;
	m_GameClientView.m_btGiveUp.ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btGiveUp.EnableWindow(FALSE) ;
	m_GameClientView.m_btPeace.ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btPeace.EnableWindow(FALSE) ;

	//��ʼ����
	memset(m_Chess , 0 , sizeof(m_Chess)) ;
	InitCurrentUserChess() ;
	m_GameClientView.m_ChessBoard.SetChess(m_Chess) ;

	//������ɫ
	m_bChessColor = GetMeChairID() + 1 ;

	//���ñ���
	m_bFinishPlacement = false ;
	m_bChooseChess = false ;
	m_bGameSart = false ;

	return true ;
}

//���沼��
LRESULT CGameClientDlg::OnHitSavePlacement(WPARAM wParam , LPARAM lParam) 
{
	UNREFERENCED_PARAMETER(wParam) ;
	UNREFERENCED_PARAMETER(lParam) ;

	CFileDialog FileDlg(false,"jqp",0,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"���岼���ļ�(*.jqp)|*.jqp||",this);
	if(FileDlg.DoModal()==IDOK)
	{
		CFile file;
		if(file.Open(FileDlg.GetPathName(),CFile::modeCreate|CFile::modeReadWrite|CFile::typeBinary))
		{
			BYTE bXPos ;
			for(BYTE i=6;i<11;i++)
			{
				for(BYTE j=11;j<17;j++)
				{
					//��ʱ����
					if(false==m_bDeasilOrder)
						bXPos = 16 - i ;
					else
						bXPos = i ;

					file.Write(&(m_Chess[bXPos][j].bChessID),sizeof(BYTE));
				}
			}
			file.Close();
		}
	}
	return true ;
}

//װ�ز���	
LRESULT CGameClientDlg::OnHitLoadPlacement(WPARAM wParam , LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam) ;
	UNREFERENCED_PARAMETER(lParam) ;

	//���ý���
	m_GameClientView.m_ScoreView.ShowWindow(SW_HIDE) ;


	CFileDialog FileDlg(true,"jqp","*.jqp",OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"���岼���ļ�(*.jqp)|*.jqp||",this);
	if(FileDlg.DoModal()==IDOK)
	{
		//Ԥ�ȳ�ʼ 
		InitCurrentUserChess();

		CFile file;
		if(file.Open(FileDlg.GetPathName(),CFile::modeRead|CFile::typeBinary))
		{
			BYTE bXPos ;
			for(int i=6;i<11;i++)
			{
				for(int j=11;j<17;j++)
				{
					//��ʱ����
					if(false==m_bDeasilOrder)
						bXPos = 16 - i ;
					else
						bXPos = i ;
					file.Read(&(m_Chess[bXPos][j].bChessID),sizeof(BYTE));
				}
			}
			file.Close();
			//�Ϸ�У��
			BOOL HeFa=true;
			BYTE Chess40=0 , Chess39=0 , Chess38=0 , Chess37=0 , Chess36=0 , Chess35=0 , Chess34=0 , Chess33=0 , Chess32=0 , Chess31=0 , Chess30=0 , Chess29=0 ;
			for(int i=11;i<17;i++)
			{
				for(int j=6;j<11;j++)
				{
					if(i==11 && (m_Chess[j][i].bChessID==CHESS_ZHA_DAN || 
						m_Chess[j][i].bChessID==CHESS_DI_LEI))//��һ�Ų�����ը��������
					{
						HeFa=false;
					}
					if(i!=16 && m_Chess[j][i].bChessID==CHESS_JUN_QI)//����ֻ�ܷ������һ��
					{
						HeFa=false;
					}

					if(m_Chess[j][i].bChessID==CHESS_SI_LING	)	Chess40++;
					if(m_Chess[j][i].bChessID==CHESS_JUN_ZHANG	)	Chess39++;
					if(m_Chess[j][i].bChessID==CHESS_SHI_ZHANG	)	Chess38++;
					if(m_Chess[j][i].bChessID==CHESS_LV_ZHANG	)	Chess37++;
					if(m_Chess[j][i].bChessID==CHESS_TUAN_ZHANG)	Chess36++;
					if(m_Chess[j][i].bChessID==CHESS_YING_ZHANG)	Chess35++;	
					if(m_Chess[j][i].bChessID==CHESS_LIAN_ZHANG)	Chess34++;	
					if(m_Chess[j][i].bChessID==CHESS_PAI_ZHANG	)	Chess33++;
					if(m_Chess[j][i].bChessID==CHESS_GONG_BING	)	Chess32++;
					if(m_Chess[j][i].bChessID==CHESS_ZHA_DAN	)	Chess31++;
					if(m_Chess[j][i].bChessID==CHESS_DI_LEI	)		Chess30++;	
					if(m_Chess[j][i].bChessID==CHESS_JUN_QI	)		Chess29++;	
				}									   
			}	

			if(	m_Chess[7][12].bChessID!=0 || m_Chess[7][12].bChessID!=0 ||
				m_Chess[9][12].bChessID!=0 || m_Chess[8][13].bChessID!=0 || 
				m_Chess[7][14].bChessID!=0 || m_Chess[9][14].bChessID!=0)	//��Ӫ�ﲻ�ܷ�����
			{
				HeFa=false;
			}
			if(m_Chess[7][16].bChessID!=CHESS_JUN_QI && m_Chess[7][16].bChessID!=CHESS_JUN_QI)//���岻�ڴ�Ӫ��
			{
				HeFa=false;
			}
			if(!(Chess40==1 && Chess39==1 && Chess38==2 && Chess37==2 && 
				Chess36==2 && Chess35==2 && Chess34==3 && Chess33==3 && 
				Chess32==3 && Chess31==2 && Chess30==3 && Chess29==1))
			{
				HeFa=false;
			}
			//�ָ�����
			if(!HeFa)
			{
				InitCurrentUserChess();
				AfxMessageBox("���벼��ʧ��!!!");
			}
		}

		m_GameClientView.m_ChessBoard.SetChess(m_Chess) ;
	}
	return 0;
}

//��ʾ����
void CGameClientDlg::ShowJunQi(BYTE bChessColor) 
{
	if(m_Chess[7][0].bColor==bChessColor && CHESS_JUN_QI==m_Chess[7][0].bChessID)
		m_Chess[7][0].bChessMode = CHESS_FORCE ;

	else if(m_Chess[7][16].bColor==bChessColor && CHESS_JUN_QI==m_Chess[7][16].bChessID)
		m_Chess[7][16].bChessMode = CHESS_FORCE ;

	else if(m_Chess[9][0].bColor==bChessColor && CHESS_JUN_QI==m_Chess[9][0].bChessID)
		m_Chess[9][0].bChessMode = CHESS_FORCE ;

	else if(m_Chess[9][16].bColor==bChessColor && CHESS_JUN_QI==m_Chess[9][16].bChessID)
		m_Chess[9][16].bChessMode = CHESS_FORCE ;

	else if(m_Chess[0][7].bColor==bChessColor && CHESS_JUN_QI==m_Chess[0][7].bChessID)
		m_Chess[0][7].bChessMode = CHESS_FORCE ;

	else if(m_Chess[0][9].bColor==bChessColor && CHESS_JUN_QI==m_Chess[0][9].bChessID)
		m_Chess[0][9].bChessMode = CHESS_FORCE ;

	else if(m_Chess[16][7].bColor==bChessColor && CHESS_JUN_QI==m_Chess[16][7].bChessID)
		m_Chess[16][7].bChessMode = CHESS_FORCE ;

	else if(m_Chess[16][9].bColor==bChessColor && CHESS_JUN_QI==m_Chess[16][9].bChessID)
		m_Chess[16][9].bChessMode = CHESS_FORCE ;

	else 
		AfxMessageBox("ShowJunQi:��������");
}

/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

ע�⣺m_Chess��SaveChessStruct������꣬����ʱ�ǰ��շ��������걣��ģ�
	 

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/

//�������
LRESULT CGameClientDlg::OnHitLoadHistory(WPARAM wParam , LPARAM lParam)
{
	//���ý���
	m_GameClientView.m_ScoreView.ShowWindow(SW_HIDE) ;
	//�����߿�
	m_GameClientView.DrawChessFrame(255 , 255) ;
	m_GameClientView.m_ChessBoard.SetPath(NULL) ;

	//��ʼ��¼
	m_HistoryRecord.Init() ;
	
	CFileDialog FileDlg(true,"junqi","*.junqi",OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"��������ļ�(*.junqi)|*.junqi||",this);
	if(FileDlg.DoModal()==IDOK)
	{
		CFile file;

		//����ת�� 
		BYTE bClientX = 255 ,
			 bClientY = 255 ;

		if(file.Open(FileDlg.GetPathName(),CFile::modeRead|CFile::typeBinary))
		{
			LONGLONG lOffset=strlen("��������ļ�");
			file.Seek(lOffset,CFile::begin);

			file.Read(&m_HistoryRecord.m_Player , sizeof(m_HistoryRecord.m_Player)) ;

			file.Read(&m_HistoryRecord.m_Chess,sizeof(m_HistoryRecord.m_Chess));

			for(BYTE x=0 ; x<17 ; x++)
			{
				for(BYTE y=0 ; y<17 ; y++)
				{
					BYTE bTmpX = x ,
						 bTmpY = y ;
					
					//����ת�� 
					ServerToClient(m_bChessColor , bTmpX , bTmpY , bClientX , bClientY) ;
			
					ASSERT(255!=bClientX && 255!=bClientY) ;

					//��ʱ����
					if(false==m_bDeasilOrder)
					{
						bClientX = 16 - bClientX ;
					}

					m_Chess[bClientX][bClientY].bChessID   = m_HistoryRecord.m_Chess[x][y].bChessID ;
					m_Chess[bClientX][bClientY].bColor     = m_HistoryRecord.m_Chess[x][y].bColor ;
					if(CHESS_NULL==m_Chess[bClientX][bClientY].bChessID)
						m_Chess[bClientX][bClientY].bChessMode = CHESS_NO ;
					else
                        m_Chess[bClientX][bClientY].bChessMode = CHESS_FORCE ;//m_HistoryRecord.m_Chess[x][y].bChessMode ;
				}
			}

			SaveChessStruct SaveChess;
			int num=0;
			num=file.Read(&SaveChess,sizeof(SaveChess));
			while(num!=0)
			{
				m_HistoryRecord.AddToList(SaveChess);
				num=file.Read(&SaveChess,sizeof(SaveChess));
			}

			file.Close();

			//��ť����
			m_GameClientView.m_btPlacement.ShowWindow(SW_HIDE) ;
			m_GameClientView.m_btPlacement.EnableWindow(FALSE) ;
			m_GameClientView.m_btSavePlacement.ShowWindow(SW_HIDE) ;
			m_GameClientView.m_btSavePlacement.EnableWindow(FALSE) ; 
			m_GameClientView.m_btLoadPlacement.ShowWindow(SW_HIDE) ;
			m_GameClientView.m_btLoadPlacement.EnableWindow(FALSE) ;
			m_GameClientView.m_btResetGame.ShowWindow(SW_SHOW) ;
			m_GameClientView.m_btResetGame.EnableWindow(TRUE) ;

			//��ʼ����
			m_lCurrentStep=0 ; 
			//���½���
			m_GameClientView.m_ChessBoard.SetChess(m_Chess) ;

		}
		//���ð�ť
		m_GameClientView.m_btPrevStep.ShowWindow(SW_SHOW) ;
		m_GameClientView.m_btPrevStep.EnableWindow(TRUE) ;
		m_GameClientView.m_btNextStep.ShowWindow(SW_SHOW) ;
		m_GameClientView.m_btNextStep.EnableWindow(TRUE) ;
		m_GameClientView.m_btTurn.ShowWindow(SW_SHOW) ;
		m_GameClientView.m_btTurn.EnableWindow(TRUE) ;
	}

	return 0 ;
}
//��һ����
LRESULT CGameClientDlg::OnHitPrevStep(WPARAM wParam , LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam) ;
	UNREFERENCED_PARAMETER(lParam) ;

	--m_lCurrentStep ;
	if(m_lCurrentStep<0)
	{
		m_lCurrentStep = 0 ;
		MessageBox("�Ѿ�����ͷ�ˣ�" , "�Ĺ�����"  , MB_OK|MB_ICONASTERISK | MB_ICONINFORMATION) ;
		return 0 ;
	}
	
	//���ñ���
	CopyMemory(m_Chess , m_HistoryRecord.m_Chess , sizeof(m_Chess)) ;
	SaveChessStruct SaveChess;

	//����ת�� 
	BYTE bClientX = 255 ,
		bClientY = 255 ;
	for(int x=0 ; x<17 ; x++)
	{
		for(int y=0 ; y<17 ; y++)
		{
			//����ת�� 
			ServerToClient(m_bChessColor , x , y , bClientX , bClientY) ;

			ASSERT(255!=bClientX && 255!=bClientY) ;

			//��ʱ����
			if(false==m_bDeasilOrder)
			{
				bClientX = 16 - bClientX ;
				bClientY = bClientY ;
			}
			m_Chess[bClientX][bClientY].bChessID   = m_HistoryRecord.m_Chess[x][y].bChessID ;
			m_Chess[bClientX][bClientY].bColor     = m_HistoryRecord.m_Chess[x][y].bColor ;
			if(CHESS_NULL==m_Chess[bClientX][bClientY].bChessID)
				m_Chess[bClientX][bClientY].bChessMode = CHESS_NO ;
			else
				m_Chess[bClientX][bClientY].bChessMode = CHESS_FORCE ;//m_HistoryRecord.m_Chess[x][y].bChessMode ;
		}
	}

	for(LONG lStep=0 ; lStep< m_lCurrentStep ; ++lStep)
	{
		
		if(0==lStep)
		{
			if(false==m_HistoryRecord.GetFirstFromList(SaveChess))
			{
				MessageBox("�Ѿ�����ͷ�ˣ�" , "�Ĺ�����"  , MB_OK|MB_ICONASTERISK | MB_ICONINFORMATION) ;
				return 0 ;
			}
		}
		else if(false==m_HistoryRecord.GetNextFromList(SaveChess))
		{
			MessageBox("�Ѿ�����ͷ�ˣ�" , "�Ĺ�����"  , MB_OK|MB_ICONASTERISK | MB_ICONINFORMATION) ;
			return 0 ;
		}

		BYTE  bSourceX = 255 ,
			 bSourceY = 255 ,
			 bTargetX = 255 ,
			 bTargetY = 255 ;

		ServerToClient(m_bChessColor , SaveChess.bSourceX , SaveChess.bSourceY , bSourceX , bSourceY) ;
		ServerToClient(m_bChessColor , SaveChess.bTargetX , SaveChess.bTargetY , bTargetX , bTargetY) ;

		//��ʱ����
		if(false==m_bDeasilOrder)
		{
			bSourceX = 16 - bSourceX ;
			bTargetX = 16 - bTargetX ;
		}

		//�Ƚ�����
		enCompareResult CompareResult = m_GameLogic.CompareChess(m_Chess , bSourceX , bSourceY , bTargetX , bTargetY) ;
		//�滭��ͷ
		if(enError!=CompareResult) 
		{
			m_GameLogic.MoveChess(m_bPlayerCount , m_Chess , bSourceX , bSourceY , bTargetX , bTargetY , true) ;
			m_GameClientView.m_ChessBoard.SetPath(&(m_GameLogic.m_ptPathArray)) ;

			//��������
			LONGLONG bCount = m_GameLogic.m_ptPathArray.GetCount() ;
			for(BYTE i = 0 ; i<bCount ; ++i)
			{
				PlayGameSound(AfxGetInstanceHandle(),TEXT("CHESS_MOVE"));
			}
			//�滭�߿�
			m_GameClientView.DrawChessFrame(bTargetX , bTargetY) ;
		}	

		switch(CompareResult)
		{
		case enSmall:
			//��ʾ����
			if(CHESS_SI_LING == m_Chess[bSourceX][bSourceY].bChessID)
			{
				ShowJunQi(m_Chess[bSourceX][bSourceY].bColor) ;
				PlayGameSound(AfxGetInstanceHandle(),TEXT("SHOW_JUN_QI"));
			}

			m_Chess[bSourceX][bSourceY].bChessID   = CHESS_NULL ;
			m_Chess[bSourceX][bSourceY].bColor	   = CHESS_COLOR_NO ;
			m_Chess[bSourceX][bSourceY].bChessMode = CHESS_NO ;

			//��������
			PlayGameSound(AfxGetInstanceHandle(),TEXT("AO"));
			break;

		case enEqual:
			//��������
			if(CHESS_ZHA_DAN==m_Chess[bSourceX][bSourceY].bChessID || CHESS_ZHA_DAN==m_Chess[bTargetX][bTargetY].bChessID)
				PlayGameSound(AfxGetInstanceHandle(),TEXT("BOMB"));
			else
				PlayGameSound(AfxGetInstanceHandle(),TEXT("LOSE_ONE_CHESS"));	

			//��ʾ����
			if(CHESS_SI_LING == m_Chess[bSourceX][bSourceY].bChessID)
			{
				ShowJunQi(m_Chess[bSourceX][bSourceY].bColor) ;
				PlayGameSound(AfxGetInstanceHandle(),TEXT("SHOW_JUN_QI"));
			}
			//��ʾ����
			if(CHESS_SI_LING == m_Chess[bTargetX][bTargetY].bChessID)
			{
				ShowJunQi(m_Chess[bTargetX][bTargetY].bColor) ;
				PlayGameSound(AfxGetInstanceHandle(),TEXT("SHOW_JUN_QI"));
			}


			m_Chess[bSourceX][bSourceY].bChessID   = CHESS_NULL ;
			m_Chess[bSourceX][bSourceY].bColor     = CHESS_COLOR_NO ;
			m_Chess[bSourceX][bSourceY].bChessMode = CHESS_NO ;

			m_Chess[bTargetX][bTargetY].bChessID   = CHESS_NULL ;
			m_Chess[bTargetX][bTargetY].bColor     = CHESS_COLOR_NO ;
			m_Chess[bTargetX][bTargetY].bChessMode = CHESS_NO ;

			break;

		case enLarge:
			//��������
			if(CHESS_NULL!=m_Chess[bTargetX][bTargetY].bChessID)
				PlayGameSound(AfxGetInstanceHandle(),TEXT("WIN_ONE_CHESS"));

			m_Chess[bTargetX][bTargetY].bChessID   = m_Chess[bSourceX][bSourceY].bChessID ; 
			m_Chess[bTargetX][bTargetY].bColor     = m_Chess[bSourceX][bSourceY].bColor ;   
			m_Chess[bTargetX][bTargetY].bChessMode = m_Chess[bSourceX][bSourceY].bChessMode;

			m_Chess[bSourceX][bSourceY].bChessID   = CHESS_NULL ;
			m_Chess[bSourceX][bSourceY].bColor	   = CHESS_COLOR_NO ;
			m_Chess[bSourceX][bSourceY].bChessMode = CHESS_NO ;

			break;

		case enError:
			AfxMessageBox("OnHitPrevStep:�������ӣ�") ;
			return false ;

		default:
			return false ;
		}		
	}
	//���½���
	m_GameClientView.m_ChessBoard.SetChess(m_Chess) ;



	return 0 ;
}
//��һ����
LRESULT CGameClientDlg::OnHitNextStep(WPARAM wParam , LPARAM lParam) 
{
	UNREFERENCED_PARAMETER(wParam) ;
	UNREFERENCED_PARAMETER(lParam) ;

	m_lCurrentStep++ ;
	SaveChessStruct SaveChess;
	if(1==m_lCurrentStep)
	{
		if(false==m_HistoryRecord.GetFirstFromList(SaveChess))
		{
			MessageBox("�Ѿ�����ͷ�ˣ�" , "�Ĺ�����"  , MB_OK|MB_ICONASTERISK | MB_ICONINFORMATION) ;
			m_lCurrentStep-- ;
			return 0 ;
		}
	}
	else if(false==m_HistoryRecord.GetNextFromList(SaveChess))
	{
		MessageBox("�Ѿ�����ͷ�ˣ�" , "�Ĺ�����"  , MB_OK|MB_ICONASTERISK | MB_ICONINFORMATION) ;
		m_lCurrentStep-- ;
		return 0 ;
	}

	BYTE  bSourceX = 255 ,
		  bSourceY = 255 ,
		  bTargetX = 255 ,
		  bTargetY = 255 ;

	ServerToClient(m_bChessColor , SaveChess.bSourceX , SaveChess.bSourceY , bSourceX , bSourceY) ;
	ServerToClient(m_bChessColor , SaveChess.bTargetX , SaveChess.bTargetY , bTargetX , bTargetY) ;

	//��ʱ����
	if(false==m_bDeasilOrder)
	{
		bSourceX = 16 - bSourceX ;
		bTargetX = 16 - bTargetX ;
	}

	//������ɫ
	BYTE bSourceColor = m_Chess[bSourceX][bSourceY].bColor ,
		bTargetColor = m_Chess[bTargetX][bTargetY].bColor ;

	//�Ƚ�����
	enCompareResult CompareResult = m_GameLogic.CompareChess(m_Chess , bSourceX , bSourceY , bTargetX , bTargetY) ;

	bool bActive[GAME_PLAYER] ;
	WORD wLoseUser = INVALID_CHAIR ;
	for(WORD wID = 0 ; wID < GAME_PLAYER ; ++wID)
		bActive[wID] = true ;

	if(enError!=CompareResult && CHESS_JUN_QI==m_Chess[bTargetX][bTargetY].bChessID && 
		true==m_GameLogic.CanCarry(m_Chess[bSourceX][bSourceY].bChessID , m_Chess[bSourceX][bSourceY].bColor , m_Chess))
	{
		wLoseUser = bTargetColor - 1 ;
		bActive[wLoseUser] = false ;
	}


	//�滭��ͷ
	if(enError!=CompareResult) 
	{
		m_GameLogic.MoveChess(m_bPlayerCount , m_Chess , bSourceX , bSourceY , bTargetX , bTargetY , true) ;
		m_GameClientView.m_ChessBoard.SetPath(&(m_GameLogic.m_ptPathArray)) ;

		//��������
		LONGLONG bCount = m_GameLogic.m_ptPathArray.GetCount() ;
		for(BYTE i = 0 ; i<bCount ; ++i)
		{
			PlayGameSound(AfxGetInstanceHandle(),TEXT("CHESS_MOVE"));
		}
		//�滭�߿�
		m_GameClientView.DrawChessFrame(bTargetX , bTargetY) ;
	}	

	switch(CompareResult)
	{
	case enSmall:
		//��ʾ����
		if(CHESS_SI_LING == m_Chess[bSourceX][bSourceY].bChessID)
		{
			ShowJunQi(m_Chess[bSourceX][bSourceY].bColor) ;
			PlayGameSound(AfxGetInstanceHandle(),TEXT("SHOW_JUN_QI"));
		}

		m_Chess[bSourceX][bSourceY].bChessID   = CHESS_NULL ;
		m_Chess[bSourceX][bSourceY].bColor	   = CHESS_COLOR_NO ;
		m_Chess[bSourceX][bSourceY].bChessMode = CHESS_NO ;

		//��������
		PlayGameSound(AfxGetInstanceHandle(),TEXT("AO"));
		break;

	case enEqual:
		//��������
		if(CHESS_ZHA_DAN==m_Chess[bSourceX][bSourceY].bChessID || CHESS_ZHA_DAN==m_Chess[bTargetX][bTargetY].bChessID)
			PlayGameSound(AfxGetInstanceHandle(),TEXT("BOMB"));
		else
			PlayGameSound(AfxGetInstanceHandle(),TEXT("LOSE_ONE_CHESS"));	

		//��ʾ����
		if(CHESS_SI_LING == m_Chess[bSourceX][bSourceY].bChessID)
		{
			ShowJunQi(m_Chess[bSourceX][bSourceY].bColor) ;
			PlayGameSound(AfxGetInstanceHandle(),TEXT("SHOW_JUN_QI"));
		}
		//��ʾ����
		if(CHESS_SI_LING == m_Chess[bTargetX][bTargetY].bChessID)
		{
			ShowJunQi(m_Chess[bTargetX][bTargetY].bColor) ;
			PlayGameSound(AfxGetInstanceHandle(),TEXT("SHOW_JUN_QI"));
		}


		m_Chess[bSourceX][bSourceY].bChessID   = CHESS_NULL ;
		m_Chess[bSourceX][bSourceY].bColor     = CHESS_COLOR_NO ;
		m_Chess[bSourceX][bSourceY].bChessMode = CHESS_NO ;

		m_Chess[bTargetX][bTargetY].bChessID   = CHESS_NULL ;
		m_Chess[bTargetX][bTargetY].bColor     = CHESS_COLOR_NO ;
		m_Chess[bTargetX][bTargetY].bChessMode = CHESS_NO ;

		break;

	case enLarge:
		//��������
		if(CHESS_NULL!=m_Chess[bTargetX][bTargetY].bChessID)
			PlayGameSound(AfxGetInstanceHandle(),TEXT("WIN_ONE_CHESS"));

		m_Chess[bTargetX][bTargetY].bChessID   = m_Chess[bSourceX][bSourceY].bChessID ; 
		m_Chess[bTargetX][bTargetY].bColor     = m_Chess[bSourceX][bSourceY].bColor ;   
		m_Chess[bTargetX][bTargetY].bChessMode = m_Chess[bSourceX][bSourceY].bChessMode;

		m_Chess[bSourceX][bSourceY].bChessID   = CHESS_NULL ;
		m_Chess[bSourceX][bSourceY].bColor	   = CHESS_COLOR_NO ;
		m_Chess[bSourceX][bSourceY].bChessMode = CHESS_NO ;

		break;

	case enError:
		AfxMessageBox("OnHitNextStep:�������ӣ�") ;
		return false ;

	default:
		return false ;
	}

	//////////////////////////////////////////////////////////////////////////

	//�ж�����
	if(true==m_GameLogic.NoChess(bSourceColor , m_Chess))
	{		
		wLoseUser = bSourceColor-1 ;
		bActive[wLoseUser] = false ;
	}
	else if(true==m_GameLogic.NoChess(bTargetColor , m_Chess))
	{
		if(CHESS_COLOR_NO!=bTargetColor)
		{
			wLoseUser = bTargetColor-1 ;
			bActive[wLoseUser] = false ;
		}
	}

	//���û��
	if(INVALID_CHAIR!=wLoseUser && false==bActive[wLoseUser])
	{
		BYTE bChessColor = wLoseUser+1 ;
		for(BYTE x=0 ; x<17 ; ++x)
			for(BYTE y=0 ; y<17 ; ++y)
				if(bChessColor==m_Chess[x][y].bColor)
				{
					m_Chess[x][y].bChessID	 = CHESS_NULL ;
					m_Chess[x][y].bChessMode = CHESS_NO ;
					m_Chess[x][y].bColor	 = CHESS_COLOR_NO ;
				}
	}


	//////////////////////////////////////////////////////////////////////////
	

	//���½���
	m_GameClientView.m_ChessBoard.SetChess(m_Chess) ;

	return 0 ;
}
//��ת����				
LRESULT CGameClientDlg::OnHitTurn(WPARAM wParam , LPARAM lParam) 
{
	UNREFERENCED_PARAMETER(wParam) ;
	UNREFERENCED_PARAMETER(lParam) ;

	if(0==m_HistoryRecord.GetStepCount())
	{
		MessageBox("����װ����ȷ�����ף�" , "�Ĺ�����"  , MB_OK|MB_ICONASTERISK | MB_ICONINFORMATION) ;
		return 0 ;
	}

	//ת����ɫ
	m_bChessColor = (m_bChessColor+1)%GAME_PLAYER ;
	if(CHESS_COLOR_NO==m_bChessColor)
		m_bChessColor = CHESS_COLOR_RED ;

	m_lCurrentStep = 0 ;

	//����ת�� 
	BYTE bClientX = 255 ,
		bClientY = 255 ;
	for(int x=0 ; x<17 ; x++)
	{
		for(int y=0 ; y<17 ; y++)
		{
			//����ת�� 
			ServerToClient(m_bChessColor , x , y , bClientX , bClientY) ;

			//��ʱ����
			if(false==m_bDeasilOrder)
			{
				bClientX = 16 - bClientX ;
			}

			ASSERT(255!=bClientX && 255!=bClientY) ;

			m_Chess[bClientX][bClientY].bChessID   = m_HistoryRecord.m_Chess[x][y].bChessID ;
			m_Chess[bClientX][bClientY].bColor     = m_HistoryRecord.m_Chess[x][y].bColor ;
			m_Chess[bClientX][bClientY].bChessMode = CHESS_FORCE ;//m_HistoryRecord.m_Chess[x][y].bChessMode ;
		}
	}

	//���ý���
	m_GameClientView.m_ChessBoard.SetChess(m_Chess) ;
	return 0 ;
}

//�������
void CGameClientDlg::ShowSave() 
{
	CSaveChessDlg dlg(this);
	if(dlg.DoModal()==IDOK)
	{
		CFileDialog FileDlg(false,"junqi",0,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"��������ļ�(*.junqi)|*.junqi||",this);
		if(FileDlg.DoModal()==IDOK)
		{
			CFile file;
			if(file.Open(FileDlg.GetPathName(),CFile::modeCreate|CFile::modeReadWrite|CFile::typeBinary))
			{
				file.Write("��������ļ�",(UINT)strlen("��������ļ�"));
				//��������
				file.Write(&m_HistoryRecord.m_Player , sizeof(m_HistoryRecord.m_Player)) ;
				//���沼��
				file.Write(&m_HistoryRecord.m_Chess,sizeof(m_HistoryRecord.m_Chess));
#ifdef _DEBUG
				TRACE("ShowSave:\n") ;
				for(BYTE x=0 ; x<17 ; ++x)
					for(BYTE y=0 ; y<17 ; ++y)
						TRACE("(%d , %d):%s\n" , x , y , m_GameLogic.IDToName(m_HistoryRecord.m_Chess[x][y].bChessID)) ;
#endif
				//��������
				POSITION pos = m_HistoryRecord.m_list.GetHeadPosition();
				for (int i=0;i < m_HistoryRecord.m_list.GetCount();i++)
				{
					SaveChessStruct SaveChess = m_HistoryRecord.m_list.GetNext(pos) ;
					file.Write(&SaveChess , sizeof(SaveChessStruct));
					TRACE("(x%d , y%d) : (x%d , y%d)" , SaveChess.bSourceX , SaveChess.bSourceY , SaveChess.bTargetX , SaveChess.bTargetY) ;
				}
				file.Close();
			}
		}
	}
}

//����ת��
void CGameClientDlg::ClientToServer(const BYTE bChessColor , const BYTE bRealClientX , const BYTE bRealClientY , BYTE &bServerX , BYTE &bServerY) 
{
	switch(bChessColor) 
	{
	case CHESS_COLOR_GREEN:	//��ɫ����
		bServerX = 16-bRealClientX ;
		bServerY = 16-bRealClientY ;
		break;

	case CHESS_COLOR_BLACK:	//��ɫ����
		bServerX = bRealClientY ;
		bServerY = 16-bRealClientX ;
		break;

	case CHESS_COLOR_BLUE:	//��ɫ����
		bServerX = bRealClientX ;
		bServerY = bRealClientY ;
		break;

	case CHESS_COLOR_RED:	//��ɫ����
		bServerX = 16-bRealClientY ;
		bServerY = bRealClientX ;
		break;

	default:
		AfxMessageBox("ClientToServer:��������") ;
		bServerX = 255 ;
		bServerY = 255 ;
	}
}

//����ת��
void CGameClientDlg::ServerToClient(const BYTE bChessColor , const BYTE bServerX , const BYTE bServerY , BYTE &bClientX , BYTE &bClientY) 
{
	switch(bChessColor) 
	{
	case CHESS_COLOR_GREEN:	//��ɫ����
		bClientX = 16-bServerX ;
		bClientY = 16-bServerY ;
		break;

	case CHESS_COLOR_BLACK:	//��ɫ����
		bClientX = 16-bServerY ;
		bClientY = bServerX ;
		break;

	case CHESS_COLOR_BLUE:	//��ɫ����
		bClientX = bServerX ;
		bClientY = bServerY ;
		break;

	case CHESS_COLOR_RED:	//��ɫ����
		bClientX = bServerY ;
		bClientY = 16-bServerX;
		break;

	default:
		AfxMessageBox("ClientToServer:��������") ;
		bClientX = 255 ;
		bClientY = 255 ;
	}

}

//��Ϸ����
bool CGameClientDlg::OnSubChessManual(const void * pBuffer, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize%sizeof(SaveChessStruct)==0);
	if (wDataSize%sizeof(SaveChessStruct)!=0) return false;

	//��������
	WORD wManualCount=wDataSize/sizeof(SaveChessStruct);
	SaveChessStruct * pChessManual=(SaveChessStruct *)pBuffer;
	
	for(LONG lCount=0 ; lCount < wManualCount ; ++lCount)
	{
		m_HistoryRecord.AddToList(pChessManual[lCount]) ;
	}

	return true;
}

//��ʼ����
bool CGameClientDlg::OnSubPlacement(const void * pDataBuffer, WORD wDataSize) 
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_S_StatusPlay));
	if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;

	CMD_S_StatusPlay *pStatusPlay = (CMD_S_StatusPlay *)pDataBuffer ;

    m_HistoryRecord.SavePlacement(pStatusPlay->Chess) ;
	m_wCurrentUser = pStatusPlay->wCurrentUser ;

	return true ;
}


//�������
LRESULT CGameClientDlg::OnHitLose(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam) ;
	UNREFERENCED_PARAMETER(lParam) ;

	SendData(SUB_C_USER_SURRENDER , NULL , 0) ;
	
	return 0 ;
}
//��������
LRESULT CGameClientDlg::OnHitGiveUp(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam) ;
	UNREFERENCED_PARAMETER(lParam) ;

	SendData(SUB_C_GAME_TIME_OUT , NULL , 0);
	KillGameTimer(IDI_GAME_TIMER) ;
	
	return 0 ;
}
//�������
LRESULT CGameClientDlg::OnHitPeace(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam) ;
	UNREFERENCED_PARAMETER(lParam) ;

	SendData(SUB_C_REQUEST_PEACE , NULL , 0) ;

	return 0 ;
}

//////////////////////////////////////////////////////////////////////////
