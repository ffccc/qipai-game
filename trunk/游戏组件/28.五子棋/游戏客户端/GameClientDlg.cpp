#include "Stdafx.h"
#include "GameClient.h"
#include "GameOption.h"
#include "ChessManual.h"
#include "GameClientDlg.h"

//////////////////////////////////////////////////////////////////////////
//�궨��

#define IDI_GAME_TIMER				100									//��Ϸ��ʱ��
#define IDI_START_GAME				101									//��ʼ��ʱ��

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientDlg, CGameFrameDlg)
	ON_WM_TIMER()
	ON_MESSAGE(IDM_START,OnStart)
	ON_MESSAGE(IDM_PEACE,OnPeace)
	ON_MESSAGE(IDM_TRADE,OnTrade)
	ON_MESSAGE(IDM_STUDY,OnStudy)
	ON_MESSAGE(IDM_REGRET,OnRegret)
	ON_MESSAGE(IDM_GIVEUP,OnGiveUp)
	ON_MESSAGE(IDM_PRESERVE,OnPreserve)
	ON_MESSAGE(IDM_SHOW_ID,OnShowChessID)
	ON_MESSAGE(IDM_PEACE_ANSWER,OnPeaceAnswer)
	ON_MESSAGE(IDM_REGRET_ANSWER,OnRegretAnswer)
	ON_MESSAGE(WM_HIT_CHESS_BORAD, OnHitChessBorad)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientDlg::CGameClientDlg() : CGameFrameDlg(&m_GameClientView)
{
	//�������
	m_bRestrict=true;
	m_bTradeUser=true;
	m_bDoubleChess=true;

	//���ñ���
	m_bDoubleChess=false;
	m_bShowChessID=false;

	//��Ϸ����
	m_ShamView=false;
	m_wBlackUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	memset(m_wLeftClock,0,sizeof(m_wLeftClock));

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
	TCHAR szTitle[128]=TEXT("");
	_snprintf(szTitle,sizeof(szTitle),TEXT("%s  --  Ver��6.0.1.0"),GAME_NAME);
	SetWindowText(szTitle);

	//��ȡ����
	m_bDoubleMouse=AfxGetApp()->GetProfileInt(TEXT("GameOption"),TEXT("DoubleMouse"),FALSE)?true:false;
	m_bShowChessID=AfxGetApp()->GetProfileInt(TEXT("GameOption"),TEXT("ShowChessID"),FALSE)?true:false;

	//����ͼ��
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);

	//���ÿؼ�
	m_GameClientView.m_ChessBorad.ShowChessID(m_bShowChessID);
	m_GameClientView.m_ChessBorad.SetDoubleMouse(m_bDoubleMouse);

	return true;
}

//���ÿ��
void CGameClientDlg::ResetGameFrame()
{
	//�������
	m_bRestrict=true;
	m_bTradeUser=true;
	m_bDoubleChess=true;
	m_wBlackUser=INVALID_CHAIR;

	//��Ϸ����
	m_ShamView=false;
	m_wCurrentUser=INVALID_CHAIR;
	memset(m_wLeftClock,0,sizeof(m_wLeftClock));

	//�������
	m_GameLogic.InitGameLogic();

	return;
}

//��Ϸ����
void CGameClientDlg::OnGameOptionSet()
{
	//��������
	CGameOption GameOption;
	GameOption.m_bDoubleMouse=m_bDoubleMouse;
	GameOption.m_bShowChessID=m_bShowChessID;
	GameOption.m_bEnableSound=IsEnableSound();

	//��������
	if (GameOption.DoModal()==IDOK)
	{
		//���ñ���
		m_bDoubleMouse=GameOption.m_bDoubleMouse;
		m_bShowChessID=GameOption.m_bShowChessID;

		//��������
		AfxGetApp()->WriteProfileInt(TEXT("GameOption"),TEXT("DoubleMouse"),m_bDoubleMouse);
		AfxGetApp()->WriteProfileInt(TEXT("GameOption"),TEXT("ShowChessID"),m_bShowChessID);

		//��������
		EnableSound(GameOption.m_bEnableSound);
		m_GameClientView.m_ChessBorad.ShowChessID(m_bShowChessID);
		m_GameClientView.m_ChessBorad.SetDoubleMouse(m_bDoubleMouse);
	}

	return;
}

//ʱ����Ϣ
bool CGameClientDlg::OnTimerMessage(WORD wChairID, UINT nElapse, UINT nTimerID)
{
	if ( nTimerID == IDI_START_GAME )
	{
		if (wChairID == GetMeChairID() && ! IsLookonMode())		//��ʼ��Ϸ
		{
			if (nElapse==0)
			{
				SendMessage(WM_CLOSE,0,0);
				return false;
			}
			if ((nElapse<=10)&&(IsLookonMode()==false)) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));

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
	case SUB_S_PLACE_CHESS:		//��������
		{
			return OnSubPlaceChess(pBuffer,wDataSize);
		}
	case SUB_S_REGRET_REQ:		//��������
		{
			return OnSubRegretReq(pBuffer,wDataSize);
		}
	case SUB_S_REGRET_FAILE:	//����ʧ��
		{
			return OnSubRegretFaile(pBuffer,wDataSize);
		}
	case SUB_S_REGRET_RESULT:	//������
		{
			return OnSubRegretResult(pBuffer,wDataSize);
		}
	case SUB_S_PEACE_REQ:		//��������
		{
			return OnSubPeaceReq(pBuffer,wDataSize);
		}
	case SUB_S_PEACE_ANSWER:	//����Ӧ��
		{
			return OnSubPeaceAnser(pBuffer,wDataSize);
		}
	case SUB_S_BLACK_TRADE:		//�����Լ�
		{
			return OnSubBlackTrade(pBuffer,wDataSize);
		}
	case SUB_S_GAME_END:		//��Ϸ����
		{
			return OnSubGameEnd(pBuffer,wDataSize);
		}
	case SUB_S_CHESS_MANUAL:	//��Ϸ����
		{
			return OnSubChessManual(pBuffer,wDataSize);
		}
	}

	return false;
}

//��Ϸ����
bool CGameClientDlg::OnGameSceneMessage(BYTE cbGameStation, bool bLookonOther, const void * pBuffer, WORD wDataSize)
{
	switch (cbGameStation)
	{
	case GS_FREE:		//����״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pBuffer;

			//���ý���
			m_wBlackUser=pStatusFree->wBlackUser;
			m_wCurrentUser=pStatusFree->wBlackUser;
			m_GameClientView.SetBlackUser(SwitchViewChairID(m_wBlackUser));

			//���ÿؼ�
			if (IsLookonMode()==false)
			{
				m_GameClientView.m_btStart.EnableWindow(TRUE);
				m_GameClientView.m_btStart.SetFocus();

				//����ʱ��
				SetGameTimer(GetMeChairID(), IDI_START_GAME, 30) ;
			}

			return true;
		}
	case GS_PLAYING:	//��Ϸ״̬
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_StatusPlay));
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;

			//��Ϣ����
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pBuffer;

			//���ñ���
			m_wBlackUser=pStatusPlay->wBlackUser;
			m_wCurrentUser=pStatusPlay->wCurrentUser;
			m_bRestrict=pStatusPlay->cbRestrict?true:false;
			m_bTradeUser=pStatusPlay->cbTradeUser?true:false;
			m_bDoubleChess=pStatusPlay->cbDoubleChess?true:false;
			
			//����ʱ��
			CopyMemory(m_wLeftClock,pStatusPlay->wLeftClock,sizeof(m_wLeftClock));
			m_GameClientView.SetGameClock(pStatusPlay->wGameClock);
			m_GameClientView.SetUserClock(SwitchViewChairID(0),m_wLeftClock[0]);
			m_GameClientView.SetUserClock(SwitchViewChairID(1),m_wLeftClock[1]);

			//���ý���
			m_GameClientView.m_btPreserve.EnableWindow(TRUE);
			m_GameClientView.SetBlackUser(SwitchViewChairID(m_wBlackUser));

			//���̽���
			m_GameClientView.m_ChessBorad.SetChessBorad(m_GameLogic.m_cbChessBorad,m_GameLogic.m_cbIdentifier);

			//���ÿ��
			const tagChessManual * pChessManual=m_GameLogic.GetLastChessManual();
			if (pChessManual!=NULL) m_GameClientView.m_ChessBorad.SetChessFocus(pChessManual->cbXPos,pChessManual->cbYPos);

			//�������
			if (IsLookonMode()==false)
			{
				//��������
				WORD wMeChairID=GetMeChairID();
				BOOL bEnablePeace=((pStatusPlay->cbBegStatus[wMeChairID]&GU_WAIT_PEACE)==0);
				BYTE cbStepCount=m_GameLogic.GetStepCount((GetMeChairID()==m_wBlackUser)?BLACK_CHESS:WHITE_CHESS);

				//��ť����
				m_GameClientView.m_btGiveUp.EnableWindow(TRUE);
				m_GameClientView.m_btPeace.EnableWindow(bEnablePeace);
				if (cbStepCount>0)
				{
					BOOL bEnableRegret=((pStatusPlay->cbBegStatus[wMeChairID]&GU_WAIT_REGRET)==0);
					m_GameClientView.m_btRegret.EnableWindow(bEnableRegret);
				}

				//�Ի�����
				BYTE cbAllStepCount=m_GameLogic.m_cbStepCount[0]+m_GameLogic.m_cbStepCount[1];
				if ((m_bTradeUser==true)&&(wMeChairID!=m_wBlackUser)&&(cbAllStepCount==3))
				{
					m_GameClientView.m_btTrade.EnableWindow(TRUE);
				}

				//��������
				WORD wTowardsUser=(wMeChairID+1)%GAME_PLAYER;
				BOOL bPeaceReq=((pStatusPlay->cbBegStatus[wTowardsUser]&GU_WAIT_PEACE)!=0);
				BOOL bRegretReq=((pStatusPlay->cbBegStatus[wTowardsUser]&GU_WAIT_REGRET)!=0);

				//��������
				if (bPeaceReq==TRUE) m_GameClientView.ShowPeaceRequest();
				if (bRegretReq==TRUE) m_GameClientView.ShowRegretRequest();

				//��������
				if (wMeChairID==m_wCurrentUser) m_GameClientView.m_ChessBorad.SetSelectMode(SM_NULL_CHESS);
			}

			//���ö�ʱ��
			SetTimer(IDI_GAME_TIMER,1000,NULL);

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

	//��Ϸ����
	m_ShamView=false;
	m_wBlackUser=pGameStart->wBlackUser;
	m_wCurrentUser=pGameStart->wBlackUser;
	m_bRestrict=pGameStart->cbRestrict?true:false;
	m_bTradeUser=pGameStart->cbTradeUser?true:false;
	m_bDoubleChess=pGameStart->cbDoubleChess?true:false;

	//����ʱ��
	m_wLeftClock[0]=m_wLeftClock[1]=pGameStart->wGameClock;
	m_GameClientView.SetGameClock(pGameStart->wGameClock);
	m_GameClientView.SetUserClock(SwitchViewChairID(0),m_wLeftClock[0]);
	m_GameClientView.SetUserClock(SwitchViewChairID(1),m_wLeftClock[1]);

	//���ñ���
	m_GameLogic.InitGameLogic();

	//���ý���
	m_GameClientView.m_btPreserve.EnableWindow(TRUE);
	m_GameClientView.m_GameScoreWnd.ShowWindow(SW_HIDE);
	m_GameClientView.SetBlackUser(SwitchViewChairID(m_wBlackUser));
	if (IsLookonMode()==true) m_GameClientView.m_ChessBorad.CleanChess();

	//ȫ�����
	if (IsLookonMode()==false)
	{
		m_GameClientView.m_btStart.EnableWindow(FALSE);
		m_GameClientView.m_btPeace.EnableWindow(TRUE);
		m_GameClientView.m_btGiveUp.EnableWindow(TRUE);
	}

	//�������
	if ((IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID()))
	{
		ActiveGameFrame();
		m_GameClientView.m_ChessBorad.SetSelectMode(SM_NULL_CHESS);
	}

	//���ö�ʱ��
	SetTimer(IDI_GAME_TIMER,1000,NULL);

	//��������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));

	return true;
}

//�û�����
bool CGameClientDlg::OnSubPlaceChess(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_PlaceChess));
	if (wDataSize!=sizeof(CMD_S_PlaceChess)) return false;

	//��Ϣ����
	CMD_S_PlaceChess * pPlaceChess=(CMD_S_PlaceChess *)pBuffer;

	//��Ϸ����
	m_wCurrentUser=pPlaceChess->wCurrentUser;
	CopyMemory(m_wLeftClock,pPlaceChess->wLeftClock,sizeof(m_wLeftClock));

	//���ý���
	m_ShamView=false;
	WORD wMeChairID=GetMeChairID();
	if ((IsLookonMode()==true)||(pPlaceChess->wPlaceUser!=wMeChairID))
	{
		//��������
		WORD wPlaceUser=pPlaceChess->wPlaceUser;
		BYTE cbColor=(wPlaceUser==m_wBlackUser)?BLACK_CHESS:WHITE_CHESS;

		//��������
		BYTE cbIdentifier=m_GameLogic.PlaceChess(pPlaceChess->cbXPos,pPlaceChess->cbYPos,cbColor);

		//��������
		m_GameClientView.m_ChessBorad.SetChess(pPlaceChess->cbXPos,pPlaceChess->cbYPos,cbColor,cbIdentifier);
		m_GameClientView.m_ChessBorad.SetChessFocus(pPlaceChess->cbXPos,pPlaceChess->cbYPos);

		//��������
		PlayGameSound(AfxGetInstanceHandle(),TEXT("PLACE_CHESS"));
	}

	//�������
	if ((IsLookonMode()==false)&&(pPlaceChess->wCurrentUser==wMeChairID))
	{
		//�����ж�
		BYTE cbAllStepCount=m_GameLogic.m_cbStepCount[0]+m_GameLogic.m_cbStepCount[1];
		if ((m_bTradeUser==true)&&(cbAllStepCount==3)) m_GameClientView.m_btTrade.EnableWindow(TRUE);

		//��������
		ActiveGameFrame();
		m_GameClientView.m_ChessBorad.SetSelectMode(SM_NULL_CHESS);
	}

	return true;
}

//��������
bool CGameClientDlg::OnSubRegretReq(const void * pBuffer, WORD wDataSize)
{
	m_GameClientView.ShowRegretRequest();

	return true;
}

//����ʧ��
bool CGameClientDlg::OnSubRegretFaile(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_RegretFaile));
	if (wDataSize!=sizeof(CMD_S_RegretFaile)) return false;

	//��Ϣ����
	CMD_S_RegretFaile * pRegretFaile=(CMD_S_RegretFaile *)pBuffer;

	//��������
	if (pRegretFaile->cbFaileReason==FR_COUNT_LIMIT)
	{
		InsertSystemString(TEXT("������� [ ���� ] ����������࣬����ʧ��"));
		return true;
	}

	//�Լҷ���
	if (pRegretFaile->cbFaileReason==FR_PLAYER_OPPOSE)
	{
		InsertSystemString(TEXT("�ԼҲ�ͬ���� [ ���� ]"));
		BYTE cbColor=(GetMeChairID()==m_wBlackUser)?BLACK_CHESS:WHITE_CHESS;
		m_GameClientView.m_btRegret.EnableWindow(m_GameLogic.GetStepCount(cbColor)>0);
		return true;
	}

	return true;
}

//������
bool CGameClientDlg::OnSubRegretResult(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_RegretResult));
	if (wDataSize!=sizeof(CMD_S_RegretResult)) return false;

	//��������
	CMD_S_RegretResult * pRegretResult=(CMD_S_RegretResult *)pBuffer;
	CopyMemory(m_wLeftClock,pRegretResult->wLeftClock,sizeof(m_wLeftClock));

	//��������
	if (m_ShamView==true)
	{
		m_ShamView=false;
		pRegretResult->wRegretCount++;
	}

	//�޸�����
	m_GameLogic.RegretChess(pRegretResult->wRegretCount);
	m_GameClientView.m_ChessBorad.SetChessBorad(m_GameLogic.m_cbChessBorad,m_GameLogic.m_cbIdentifier);

	//���ý���
	const tagChessManual * pChessManual=m_GameLogic.GetLastChessManual();
	if (pChessManual==NULL) m_GameClientView.m_ChessBorad.SetChessFocus(255,255);
	else m_GameClientView.m_ChessBorad.SetChessFocus(pChessManual->cbXPos,pChessManual->cbYPos);

	//�������
	WORD wMeChairID=GetMeChairID();
	m_wCurrentUser=pRegretResult->wCurrentUser;
	if ((IsLookonMode()==false)&&(pRegretResult->wCurrentUser==wMeChairID))
	{
		ActiveGameFrame();
		m_GameClientView.m_ChessBorad.SetSelectMode(SM_NULL_CHESS);
	}

	//�������
	if (IsLookonMode()==false)
	{
		BYTE cbStepCount=m_GameLogic.GetStepCount((GetMeChairID()==m_wBlackUser)?BLACK_CHESS:WHITE_CHESS);
		if (cbStepCount==0) m_GameClientView.m_btRegret.EnableWindow(FALSE);
		if ((cbStepCount>0)&&(pRegretResult->wRegretUser==wMeChairID)) m_GameClientView.m_btRegret.EnableWindow(TRUE);
	}

	return true;
}

//��������
bool CGameClientDlg::OnSubPeaceReq(const void * pBuffer, WORD wDataSize)
{
	m_GameClientView.ShowPeaceRequest();

	return true;
}

//����Ӧ��
bool CGameClientDlg::OnSubPeaceAnser(const void * pBuffer, WORD wDataSize)
{
	//���ý���
	m_GameClientView.m_btPeace.EnableWindow(TRUE);
	InsertSystemString(TEXT("�ԼҲ�ͬ���� [ ���� ]"));

	return true;
}

//�����Լ�
bool CGameClientDlg::OnSubBlackTrade(const void * pBuffer, WORD wDataSize)
{
	//�Ի��û�
	m_wCurrentUser=m_wBlackUser;
	m_wBlackUser=(m_wBlackUser+1)%GAME_PLAYER;

	//��������
	InsertSystemString(TEXT("[ ���巽 ] �� [ ���巽 ] �Ի���"));
	m_GameClientView.SetBlackUser(SwitchViewChairID(m_wBlackUser));

	//�������
	if ((IsLookonMode()==false)&&(GetMeChairID()!=m_wBlackUser))
	{
		ActiveGameFrame();
		m_GameClientView.m_ChessBorad.SetSelectMode(SM_NULL_CHESS);
	}

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

	//���ñ���
	m_wCurrentUser=INVALID_CHAIR;

	//���ý���
	m_GameClientView.m_btPeace.EnableWindow(FALSE);
	m_GameClientView.m_btTrade.EnableWindow(FALSE);
	m_GameClientView.m_btRegret.EnableWindow(FALSE);
	m_GameClientView.m_btGiveUp.EnableWindow(FALSE);
	m_GameClientView.m_ChessBorad.SetSelectMode(SM_NO_SELECT);

	//��������
	if (IsLookonMode()==false)
	{
		if (pGameEnd->wWinUser==GetMeChairID()) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WIN"));
		else PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_LOST"));
	}
	else PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_END"));

	//�ɼ�����
	tagGameScoreInfo GameScoreInfo;
	memset(&GameScoreInfo,0,sizeof(GameScoreInfo));
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		WORD wUserChairID=(m_wBlackUser+i)%GAME_PLAYER;
		const tagUserData * pUserData=GetUserData(wUserChairID);
		GameScoreInfo.lScore[i]=pGameEnd->lUserScore[wUserChairID];
		lstrcpyn(GameScoreInfo.szUserName[i],pUserData->szName,CountArray(GameScoreInfo.szUserName[i]));
	}
	m_GameClientView.m_GameScoreWnd.SetScoreInfo(GameScoreInfo);

	//���ý���
	if (IsLookonMode()==false)
	{
		ActiveGameFrame();
		m_GameClientView.m_btStart.EnableWindow(TRUE);
		m_GameClientView.m_btStart.SetFocus();
		if (m_GameClientView.m_PeaceRequest.GetSafeHwnd()!=NULL) m_GameClientView.m_PeaceRequest.DestroyWindow();
		if (m_GameClientView.m_RegretRequest.GetSafeHwnd()!=NULL) m_GameClientView.m_RegretRequest.DestroyWindow();
	}

	//ɾ����ʱ��
	KillTimer(IDI_GAME_TIMER);

	//ϵͳ��Ϣ
	static bool bNotify=false;
	if (bNotify==false)
	{
		bNotify=true;
		InsertSystemString(TEXT("��Ϸ�����ˣ�����Ҫ�������ף����� [ ���� ] ��ť�������׼�¼"));
	}

	//���ö�ʱ��
	if(false==IsLookonMode())
	{
		SetGameTimer(GetMeChairID(), IDI_START_GAME, 30) ;
		m_nElapse = 30 ;
	}

	return true;
}

//��Ϸ����
bool CGameClientDlg::OnSubChessManual(const void * pBuffer, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize%sizeof(tagChessManual)==0);
	if (wDataSize%sizeof(tagChessManual)!=0) return false;

	//��������
	WORD wManualCount=wDataSize/sizeof(tagChessManual);
	for (WORD wIndex=0;wIndex<wManualCount;wIndex++) 
	{
		tagChessManual * pChessManual=(((tagChessManual *)pBuffer)+wIndex);
		m_GameLogic.PlaceChess(pChessManual->cbXPos,pChessManual->cbYPos,pChessManual->cbColor);
	}

	return true;
}

//��ʱ����Ϣ
void CGameClientDlg::OnTimer(UINT nIDEvent)
{
	//������ʱ��
	if ((nIDEvent==IDI_GAME_TIMER)&&(m_wCurrentUser!=INVALID_CHAIR))
	{
		//������ͼ
		if (m_wLeftClock[m_wCurrentUser]>0)
		{
			m_wLeftClock[m_wCurrentUser]--;
			m_GameClientView.SetUserClock(SwitchViewChairID(m_wCurrentUser),m_wLeftClock[m_wCurrentUser]);
		}
		
		//��ʱ�ж�
		if ((IsLookonMode()==false)&&(GetMeChairID()==m_wCurrentUser))
		{
			if (m_wLeftClock[m_wCurrentUser]==0)
			{
				//������Ϣ
				SendData(SUB_C_GIVEUP_REQ);

				//���ý���
				m_GameClientView.m_btPeace.EnableWindow(FALSE);
				m_GameClientView.m_btTrade.EnableWindow(FALSE);
				m_GameClientView.m_btRegret.EnableWindow(FALSE);
				m_GameClientView.m_btGiveUp.EnableWindow(FALSE);
				m_GameClientView.m_ChessBorad.SetSelectMode(SM_NO_SELECT);

				return;
			}
			else if (m_wLeftClock[m_wCurrentUser]<=30) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));
		}

		return;
	}

	__super::OnTimer(nIDEvent);
}

//��ʼ��ť
LRESULT CGameClientDlg::OnStart(WPARAM wParam, LPARAM lParam)
{
	//�������
	m_bRestrict=true;
	m_bTradeUser=true;
	m_bDoubleChess=true;

	//��Ϸ����
	m_wBlackUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;

	//���ý���
	m_GameClientView.m_ChessBorad.CleanChess();
	m_GameClientView.m_btStart.EnableWindow(FALSE);
	m_GameClientView.m_btPreserve.EnableWindow(FALSE);
	m_GameClientView.m_GameScoreWnd.ShowWindow(SW_HIDE);

	KillGameTimer(IDI_START_GAME) ;

	//������Ϣ
	SendUserReady(NULL,0);

	return 0;
}

//���尴ť
LRESULT CGameClientDlg::OnRegret(WPARAM wParam, LPARAM lParam)
{
	//������Ϣ
	SendData(SUB_C_REGRET_REQ);

	//���ý���
	m_GameClientView.m_btRegret.EnableWindow(FALSE);
	InsertSystemString(TEXT("���ڵȴ��Լһ�Ӧ [ ���� ] ����..."));

	return 0;
}

//��Ͱ�ť
LRESULT CGameClientDlg::OnPeace(WPARAM wParam, LPARAM lParam)
{
	//����ȷ��
	int nResult=AfxMessageBox(TEXT("��ȷʵҪ [ ���� ] ��"),MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2);
	if (nResult!=IDYES) return 0;

	//״̬�ж�
	if (m_wCurrentUser==INVALID_CHAIR) return 0;

	//������Ϣ
	SendData(SUB_C_PEACE_REQ);

	//���ý���
	m_GameClientView.m_btPeace.EnableWindow(FALSE);
	InsertSystemString(TEXT("���ڵȴ��Լһ�Ӧ [ ���� ] ����..."));

	return 0;
}

//���䰴ť
LRESULT CGameClientDlg::OnGiveUp(WPARAM wParam, LPARAM lParam)
{
	//����ȷ��
	int nResult=AfxMessageBox(TEXT("��ȷʵҪ [ ���� ] ��"),MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2);
	if (nResult!=IDYES) return 0;

	//״̬�ж�
	if (m_wCurrentUser==INVALID_CHAIR) return 0;

	//������Ϣ
	SendData(SUB_C_GIVEUP_REQ);

	//���ý���
	m_GameClientView.m_btPeace.EnableWindow(FALSE);
	m_GameClientView.m_btTrade.EnableWindow(FALSE);
	m_GameClientView.m_btRegret.EnableWindow(FALSE);
	m_GameClientView.m_btGiveUp.EnableWindow(FALSE);
	m_GameClientView.m_ChessBorad.SetSelectMode(SM_NO_SELECT);

	return 0;
}

//������ť
LRESULT CGameClientDlg::OnShowChessID(WPARAM wParam, LPARAM lParam)
{
	//���ÿؼ�
	m_bShowChessID=!m_bShowChessID;
	m_GameClientView.m_ChessBorad.ShowChessID(m_bShowChessID);

	//��������
	AfxGetApp()->WriteProfileInt(TEXT("GameOption"),TEXT("ShowChessID"),m_bShowChessID);

	//ϵͳ��Ϣ
	if (m_bShowChessID==true) InsertSystemString(TEXT("����Ϊ��ʾ��������"));
	else InsertSystemString(TEXT("����Ϊ������������"));

	return 0;
}

//�Ի���ť
LRESULT CGameClientDlg::OnTrade(WPARAM wParam, LPARAM lParam)
{
	//����ȷ��
	int nResult=AfxMessageBox(TEXT("��ȷʵҪ����� [ ���� ] ��"),MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2);
	if (nResult!=IDYES) return 0;

	//״̬�ж�
	if (m_wCurrentUser==INVALID_CHAIR) return 0;

	//������Ϣ
	SendData(SUB_C_TRADE_REQ);

	//���ý���
	m_GameClientView.m_btTrade.EnableWindow(FALSE);
	m_GameClientView.m_ChessBorad.SetSelectMode(SM_NO_SELECT);

	return 0;
}

//���水ť
LRESULT CGameClientDlg::OnPreserve(WPARAM wParam, LPARAM lParam)
{
	//��ȡĿ¼
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(sizeof(szPath),szPath);

	//ѡ���ļ�
	LPCTSTR pszFilter=TEXT("�����������ļ� ��*.GBM��|*.GBM||");
	CFileDialog DlgSaveManual(FALSE,TEXT("GBM"),NULL,OFN_NOCHANGEDIR|OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,pszFilter);
	DlgSaveManual.m_pOFN->lpstrInitialDir=szPath;
	if (DlgSaveManual.DoModal()!=IDOK) return 0;

	//���ļ�
	CFile FileManual;
	if (FileManual.Open(DlgSaveManual.GetPathName(),CFile::modeWrite|CFile::modeCreate)==FALSE)
	{
		AfxMessageBox(TEXT("�޷������ļ������ױ���ʧ�ܣ�"),MB_ICONSTOP);
		return 0;
	}

	//�ļ�ͷ
	tagChessManualHead ChessManualHead;
	ChessManualHead.wVersion=VER_MANUAL;
	ChessManualHead.dwManualDate=(DWORD)time(NULL);
	ChessManualHead.dwManualCount=(DWORD)m_GameLogic.m_ChessManualInfo.GetCount();

	//д���ļ�ͷ
	FileManual.SeekToBegin();
	FileManual.Write(&ChessManualHead,sizeof(ChessManualHead));

	//��������
	tagChessManual * pChessManual=NULL;
	for (DWORD i=0;i<ChessManualHead.dwManualCount;i++)
	{
		pChessManual=&m_GameLogic.m_ChessManualInfo[i];
		FileManual.Write(pChessManual,sizeof(tagChessManual));
	}

	//�ر��ļ�
	FileManual.SetLength(sizeof(ChessManualHead)+ChessManualHead.dwManualCount*sizeof(tagChessManual));
	FileManual.Close();

	//ϵͳ��Ϣ
	InsertSystemString(TEXT("��������Ϸ�����׼�¼������ɹ�"));

	return 0;
}

//�о���ť
LRESULT CGameClientDlg::OnStudy(WPARAM wParam, LPARAM lParam)
{
	//��������
	if (m_GameClientView.m_ChessManual.GetSafeHwnd()==NULL)
	{
		m_GameClientView.m_ChessManual.m_pDlgGameLogic=&m_GameLogic;
		m_GameClientView.m_ChessManual.Create(IDD_MANUAL,&m_GameClientView);
	}

	//��ʾ����
	m_GameClientView.m_ChessManual.CenterWindow(NULL);
	m_GameClientView.m_ChessManual.ShowWindow(SW_SHOW);

	return 0;
}

//����Ӧ��
LRESULT CGameClientDlg::OnPeaceAnswer(WPARAM wParam, LPARAM lParam)
{
	//������Ϣ
	CMD_C_PeaceAnswer PeaceAnswer;
	PeaceAnswer.cbApprove=BYTE(wParam);
	SendData(SUB_C_PEACE_ANSWER,&PeaceAnswer,sizeof(PeaceAnswer));

	return 0;
}

//����Ӧ��
LRESULT CGameClientDlg::OnRegretAnswer(WPARAM wParam, LPARAM lParam)
{
	//���ý���
	if (wParam==TRUE)
	{
		m_wCurrentUser=INVALID_CHAIR;
		m_GameClientView.m_ChessBorad.SetSelectMode(SM_NO_SELECT);
	}

	//������Ϣ
	CMD_C_RegretAnswer ReqretAnswer;
	ReqretAnswer.cbApprove=BYTE(wParam);
	SendData(SUB_C_REGRET_ANSWER,&ReqretAnswer,sizeof(ReqretAnswer));

	return 0;
}

//�������
LRESULT CGameClientDlg::OnHitChessBorad(WPARAM wParam, LPARAM lParam)
{
	if (m_wCurrentUser==GetMeChairID())
	{
		//��������
		WORD wMeChairID=GetMeChairID();
		BYTE cbXPos=(BYTE)wParam,cbYPos=(BYTE)lParam;
		BYTE cbColor=(m_wBlackUser==wMeChairID)?BLACK_CHESS:WHITE_CHESS;

		//��������
		BYTE cbIdentifier=m_GameLogic.PlaceChess(cbXPos,cbYPos,cbColor);
		if (cbIdentifier==0)
		{
			InsertSystemString(TEXT("����ǽ��ֵ㣬�����ڴ˵����"));
			PlayGameSound(AfxGetInstanceHandle(),TEXT("INVALID_PLACE"));
			return 0;
		}

		//���ñ���
		m_ShamView=true;
		m_wCurrentUser=INVALID_CHAIR;

		//��������
		m_GameClientView.m_ChessBorad.SetSelectMode(SM_NO_SELECT);
		m_GameClientView.m_ChessBorad.SetChess((BYTE)wParam,(BYTE)lParam,cbColor,cbIdentifier);
		m_GameClientView.m_ChessBorad.SetChessFocus((BYTE)wParam,(BYTE)lParam);

		//�����ж�
		if (m_GameLogic.GetStepCount(cbColor)==1) m_GameClientView.m_btRegret.EnableWindow(TRUE);

		//��������
		PlayGameSound(AfxGetInstanceHandle(),TEXT("PLACE_CHESS"));

		//������Ϣ
		CMD_C_PlaceChess PlaceChess;
		PlaceChess.cbXPos=(BYTE)wParam;
		PlaceChess.cbYPos=(BYTE)lParam;
		SendData(SUB_C_PLACE_CHESS,&PlaceChess,sizeof(PlaceChess));
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////
