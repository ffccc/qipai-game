#include "Stdafx.h"
#include "GameOption.h"
#include "GameClient.h"
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
	ON_MESSAGE(IDM_STUDY,OnStudy)
	ON_MESSAGE(IDM_REGRET,OnRegret)
	ON_MESSAGE(IDM_GIVEUP,OnGiveUp)
	ON_MESSAGE(IDM_PRESERVE,OnPreserve)
	ON_MESSAGE(WM_SELECT_CHESS, OnSelectChess)
	ON_MESSAGE(IDM_PEACE_ANSWER,OnPeaceAnswer)
	ON_MESSAGE(IDM_REGRET_ANSWER,OnRegretAnswer)
	ON_MESSAGE(WM_HIT_CHESS_BORAD, OnHitChessBorad)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientDlg::CGameClientDlg() : CGameFrameDlg(&m_GameClientView)
{
	//��Ϸ����
	m_cbChessColor=BLACK_CHESS;
	m_wBlackUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	memset(m_wLeftClock,0,sizeof(m_wLeftClock));

	//��������
	m_ShamView=false;
	m_cbXSourcePos=255;
	m_cbYSourcePos=255;
	m_cbXTargetPos=255;
	m_cbYTargetPos=255;

	//���ñ���
	m_bShowChessName=false;
	m_bAutoSaveManual=false;

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
	_snprintf(szTitle,sizeof(szTitle),TEXT("%s"),GAME_NAME);
	SetWindowText(szTitle);

	//����ͼ��
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);

	//��ȡ����
	m_bShowChessName=AfxGetApp()->GetProfileInt(TEXT("GameOption"),TEXT("ShowChessName"),FALSE)?true:false;
	m_bAutoSaveManual=AfxGetApp()->GetProfileInt(TEXT("GameOption"),TEXT("AutoSaveManual"),FALSE)?true:false;

	//���ÿؼ�
	m_GameClientView.m_ChessBorad.ShowChessName(m_bShowChessName);

	return true;
}

//���ÿ��
void CGameClientDlg::ResetGameFrame()
{
	//��������
	m_cbXSourcePos=255;
	m_cbYSourcePos=255;
	m_cbXTargetPos=255;
	m_cbYTargetPos=255;

	//��Ϸ����
	m_cbChessColor=BLACK_CHESS;
	m_wBlackUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	memset(m_wLeftClock,0,sizeof(m_wLeftClock));

	return;
}

//��Ϸ����
void CGameClientDlg::OnGameOptionSet()
{
	//��������
	CGameOption GameOption;
	GameOption.m_bEnableSound=IsEnableSound();
	GameOption.m_bShowChessName=m_bShowChessName;

	//��������
	if (GameOption.DoModal()==IDOK)
	{
		//���ñ���
		m_bShowChessName=GameOption.m_bShowChessName;

		//��������
		AfxGetApp()->WriteProfileInt(TEXT("GameOption"),TEXT("ShowChessName"),m_bShowChessName);

		//��������
		EnableSound(GameOption.m_bEnableSound);
		m_GameClientView.m_ChessBorad.ShowChessName(m_bShowChessName);
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
	case SUB_S_MOVE_CHESS:		//�ƶ�����
		{
			return OnSubMoveChess(pBuffer,wDataSize);
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

//��Ϸ��ʼ
bool CGameClientDlg::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;

	//��Ϣ����
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;
	m_wBlackUser=pGameStart->wBlackUser;
	m_wCurrentUser=pGameStart->wBlackUser;
	m_cbChessColor=(m_wBlackUser==GetMeChairID())?BLACK_CHESS:WHITE_CHESS;

	//����ʱ��
	m_wLeftClock[0]=m_wLeftClock[1]=pGameStart->wGameClock;
	m_GameClientView.SetGameClock(pGameStart->wGameClock);
	m_GameClientView.SetUserClock(SwitchViewChairID(0),m_wLeftClock[0]);
	m_GameClientView.SetUserClock(SwitchViewChairID(1),m_wLeftClock[1]);

	//���ñ���
	m_cbXSourcePos=255;
	m_cbYSourcePos=255;
	m_cbXTargetPos=255;
	m_cbYTargetPos=255;

	//�������� 
	m_GameLogic.ResetChessBorad();
	m_GameClientView.m_ChessBorad.SetChessColor(m_cbChessColor);
	m_GameClientView.m_ChessBorad.SetChessBorad(m_GameLogic.m_ChessBorad);

	//���ý���
	m_GameClientView.m_btPreserve.EnableWindow(TRUE);
	m_GameClientView.m_GameScoreWnd.ShowWindow(SW_HIDE);
	m_GameClientView.SetBlackUser(SwitchViewChairID(m_wBlackUser));

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
		CancelChessSelect();
		m_GameClientView.m_ChessBorad.SetPositively(true);
	}

	//���ö�ʱ��
	SetTimer(IDI_GAME_TIMER,1000,NULL);

	//��������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));

	return true;
}

//�ƶ�����
bool CGameClientDlg::OnSubMoveChess(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_MoveChess));
	if (wDataSize!=sizeof(CMD_S_MoveChess)) return false;

	//��Ϣ����
	CMD_S_MoveChess * pMoveChess=(CMD_S_MoveChess *)pBuffer;
	m_wCurrentUser=pMoveChess->wCurrentUser;

	//����ʱ��
	CopyMemory(m_wLeftClock,pMoveChess->wLeftClock,sizeof(m_wLeftClock));

	//��¼����
	WORD wMoveUser=(pMoveChess->wCurrentUser+1)%GAME_PLAYER;

	//���ý���
	m_ShamView=false;
	WORD wMeChairID=GetMeChairID();
	if ((IsLookonMode()==true)||(wMoveUser!=wMeChairID))
	{
		//��������
		BYTE cbXSourcePos=pMoveChess->cbXSourcePos,cbYSourcePos=pMoveChess->cbYSourcePos;
		BYTE cbXTargetPos=pMoveChess->cbXTargetPos,cbYTargetPos=pMoveChess->cbYTargetPos;

		//��������
		const tagChessItem * pSourceChessItem=m_GameLogic.GetChessItem(cbXSourcePos,cbYSourcePos);
		const tagChessItem * pTargetChessItem=m_GameLogic.GetChessItem(cbXTargetPos,cbYTargetPos);
		if ((pTargetChessItem!=NULL)&&(pSourceChessItem->cbColor!=pTargetChessItem->cbColor))
		{
			if (pSourceChessItem->cbColor==m_cbChessColor) PlayGameSound(AfxGetInstanceHandle(),TEXT("CHESS_EAT"));
			else PlayGameSound(AfxGetInstanceHandle(),TEXT("CHESS_DEAD"));
		}
		else PlayGameSound(AfxGetInstanceHandle(),TEXT("PLACE_CHESS"));

		//�ƶ�����
		m_GameLogic.MoveChess(cbXSourcePos,cbYSourcePos,cbXTargetPos,cbYTargetPos,pMoveChess->cbSwitchChess);

		//��������
		m_GameClientView.m_ChessBorad.SetChessBorad(m_GameLogic.m_ChessBorad);
		m_GameClientView.m_ChessBorad.SetChessFrame(cbXSourcePos,cbYSourcePos,cbXTargetPos,cbYTargetPos);
	}

	//�������
	if ((IsLookonMode()==false)&&(pMoveChess->wCurrentUser==wMeChairID))
	{
		//���ý���
		ActiveGameFrame();
		CancelChessSelect();
		m_GameClientView.m_ChessBorad.SetPositively(true);
		m_GameClientView.m_ChessBorad.SetSelectChess(m_cbXSourcePos,m_cbYSourcePos);
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
		WORD wStepCount=m_GameLogic.GetStepCount((GetMeChairID()==m_wBlackUser)?BLACK_CHESS:WHITE_CHESS);
		m_GameClientView.m_btRegret.EnableWindow(wStepCount>0);
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
		pRegretResult->cbRegretCount++;
	}

	//��������
	m_GameLogic.RegretChess(pRegretResult->cbRegretCount);
	m_GameClientView.m_ChessBorad.SetChessBorad(m_GameLogic.m_ChessBorad);

	//���ÿ��
	const tagChessManual * pChessManual=m_GameLogic.GetLastChessManual();
	if (pChessManual!=NULL) 
	{
		BYTE cbXPosStart=pChessManual->cbXSourceChessPos;
		BYTE cbYPosStart=pChessManual->cbYSourceChessPos;
		BYTE cbXPosTerminal=pChessManual->cbXTargetChessPos;
		BYTE cbYPosTerminal=pChessManual->cbYTargetChessPos;
		m_GameClientView.m_ChessBorad.SetChessFrame(cbXPosStart,cbYPosStart,cbXPosTerminal,cbYPosTerminal);
	}
	else m_GameClientView.m_ChessBorad.SetChessFrame(255,255,255,255);

	//�������
	WORD wMeChairID=GetMeChairID();
	m_wCurrentUser=pRegretResult->wCurrentUser;
	if ((IsLookonMode()==false)&&(pRegretResult->wCurrentUser==wMeChairID))
	{
		ActiveGameFrame();
		CancelChessSelect();
		m_GameClientView.m_ChessBorad.SetPositively(true);
	}

	//�������
	if (IsLookonMode()==false)
	{
		WORD wStepCount=m_GameLogic.GetStepCount((GetMeChairID()==m_wBlackUser)?BLACK_CHESS:WHITE_CHESS);
		if (wStepCount==0) m_GameClientView.m_btRegret.EnableWindow(FALSE);
		if ((wStepCount>0)&&(pRegretResult->wRegretUser==wMeChairID)) m_GameClientView.m_btRegret.EnableWindow(TRUE);
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
	m_GameClientView.m_btRegret.EnableWindow(FALSE);
	m_GameClientView.m_btGiveUp.EnableWindow(FALSE);
	m_GameClientView.m_ChessBorad.SetPositively(false);

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
		CancelChessSelect();
		m_GameClientView.m_btStart.EnableWindow(TRUE);
		m_GameClientView.m_btStart.SetFocus();
		if (m_GameClientView.m_PeaceRequest.GetSafeHwnd()!=NULL) m_GameClientView.m_PeaceRequest.DestroyWindow();
		if (m_GameClientView.m_RegretRequest.GetSafeHwnd()!=NULL) m_GameClientView.m_RegretRequest.DestroyWindow();

		//����ʱ��
		SetGameTimer(GetMeChairID(), IDI_START_GAME, 30) ;
	}

	//ɾ����ʱ��
	KillTimer(IDI_GAME_TIMER);

	//��������
	if (m_bAutoSaveManual==true)
	{
		//��ȡʱ��
		SYSTEMTIME SystemTime;
		GetLocalTime(&SystemTime);

		//��ȡĿ¼
		TCHAR szPath[MAX_PATH]=TEXT("");
		GetCurrentDirectory(sizeof(szPath),szPath);

		//�����ļ�
		TCHAR szFileName[MAX_PATH];
		_snprintf(szFileName,sizeof(szFileName),TEXT("%s\\Χ������_%04ld_%02ld_%02ld_%02ld_%02ld_%02ld.WQM"),
			szPath,SystemTime.wYear,SystemTime.wMonth,SystemTime.wDay,SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond);

		//��������
		if (SaveChessManual(szFileName)==false)	InsertSystemString(TEXT("Χ����Ϸ�����׼�¼������ʧ��"));
		else InsertSystemString(TEXT("Χ����Ϸ�����׼�¼������ɹ�"));
	}
	else
	{
		static bool bNotify=false;
		if (bNotify==false)
		{
			bNotify=true;
			InsertSystemString(TEXT("��Ϸ�����ˣ�����Ҫ�������ף����� [ ���� ] ��ť�������׼�¼"));
		}
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
	tagChessManual * pChessManual=(tagChessManual *)pBuffer;
	m_GameLogic.InsertChessManual(pChessManual,wManualCount);

	return true;
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
			m_cbChessColor=(m_wBlackUser==GetMeChairID())?BLACK_CHESS:WHITE_CHESS;

			//����ʱ��
			CopyMemory(m_wLeftClock,pStatusPlay->wLeftClock,sizeof(m_wLeftClock));
			m_GameClientView.SetGameClock(pStatusPlay->wGameClock);
			m_GameClientView.SetUserClock(SwitchViewChairID(0),m_wLeftClock[0]);
			m_GameClientView.SetUserClock(SwitchViewChairID(1),m_wLeftClock[1]);

			//���ý���
			m_GameClientView.m_btPreserve.EnableWindow(TRUE);
			m_GameClientView.SetBlackUser(SwitchViewChairID(m_wBlackUser));

			//�߼�����
			CopyMemory(m_GameLogic.m_ChessItem,pStatusPlay->ChessItemArray,sizeof(m_GameLogic.m_ChessItem));
			CopyMemory(m_GameLogic.m_wStepCount,pStatusPlay->wChessStepCount,sizeof(m_GameLogic.m_wStepCount));

			//���̽���
			m_GameLogic.UpdateChessBorad();
			m_GameClientView.m_ChessBorad.SetChessColor(m_cbChessColor);
			m_GameClientView.m_ChessBorad.SetChessBorad(m_GameLogic.m_ChessBorad);

			//���ÿ��
			const tagChessManual * pChessManual=m_GameLogic.GetLastChessManual();
			if (pChessManual!=NULL) 
			{
				BYTE cbXPosStart=pChessManual->cbXSourceChessPos;
				BYTE cbYPosStart=pChessManual->cbYSourceChessPos;
				BYTE cbXPosTerminal=pChessManual->cbXTargetChessPos;
				BYTE cbYPosTerminal=pChessManual->cbYTargetChessPos;
				m_GameClientView.m_ChessBorad.SetChessFrame(cbXPosStart,cbYPosStart,cbXPosTerminal,cbYPosTerminal);
			}

			//�������
			if (IsLookonMode()==false)
			{
				//��������
				WORD wMeChairID=GetMeChairID();
				BOOL bEnablePeace=((pStatusPlay->cbBegStatus[wMeChairID]&GU_WAIT_PEACE)==0);
				WORD wStepCount=m_GameLogic.GetStepCount((GetMeChairID()==m_wBlackUser)?BLACK_CHESS:WHITE_CHESS);

				//��ť����
				m_GameClientView.m_btGiveUp.EnableWindow(TRUE);
				m_GameClientView.m_btPeace.EnableWindow(bEnablePeace);
				if (wStepCount>0)
				{
					BOOL bEnableRegret=((pStatusPlay->cbBegStatus[wMeChairID]&GU_WAIT_REGRET)==0);
					m_GameClientView.m_btRegret.EnableWindow(bEnableRegret);
				}

				//��������
				WORD wEnemyUser=(wMeChairID+1)%GAME_PLAYER;
				BOOL bPeaceReq=((pStatusPlay->cbBegStatus[wEnemyUser]&GU_WAIT_PEACE)!=0);
				BOOL bRegretReq=((pStatusPlay->cbBegStatus[wEnemyUser]&GU_WAIT_REGRET)!=0);

				//��������
				if (bPeaceReq==TRUE) m_GameClientView.ShowPeaceRequest();
				if (bRegretReq==TRUE) m_GameClientView.ShowRegretRequest();

				//��������
				if (wMeChairID==m_wCurrentUser) m_GameClientView.m_ChessBorad.SetPositively(true);
			}

			//���ö�ʱ��
			SetTimer(IDI_GAME_TIMER,1000,NULL);

			return true;
		}
	}

	return false;
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
				m_GameClientView.m_btRegret.EnableWindow(FALSE);
				m_GameClientView.m_btGiveUp.EnableWindow(FALSE);
				m_GameClientView.m_ChessBorad.SetPositively(false);

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
	//��Ϸ����
	m_wBlackUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	memset(m_wLeftClock,0,sizeof(m_wLeftClock));

	//���ý���
	m_GameClientView.m_ChessBorad.CleanChess();
	m_GameClientView.m_btStart.EnableWindow(FALSE);
	m_GameClientView.m_btPreserve.EnableWindow(FALSE);
	m_GameClientView.m_GameScoreWnd.ShowWindow(SW_HIDE);

	//������Ϣ
	SendUserReady(NULL,0);

	//�ر�ʱ��
	KillGameTimer(IDI_START_GAME) ;

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
	m_GameClientView.m_btRegret.EnableWindow(FALSE);
	m_GameClientView.m_btGiveUp.EnableWindow(FALSE);
	m_GameClientView.m_ChessBorad.SetPositively(false);

	return 0;
}

//���水ť
LRESULT CGameClientDlg::OnPreserve(WPARAM wParam, LPARAM lParam)
{
	//��ȡĿ¼
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(sizeof(szPath),szPath);

	//ѡ���ļ�
	LPCTSTR pszFilter=TEXT("�������������ļ� ��*.CHM��|*.CHM||");
	CFileDialog DlgSaveManual(FALSE,TEXT("CHM"),NULL,OFN_NOCHANGEDIR|OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,pszFilter);
	DlgSaveManual.m_pOFN->lpstrInitialDir=szPath;
	if (DlgSaveManual.DoModal()!=IDOK) return 0;

	//��������
	if (SaveChessManual(DlgSaveManual.GetPathName())==false)	
	{
		InsertSystemString(TEXT("����������Ϸ�����׼�¼������ʧ��"));
	}
	else InsertSystemString(TEXT("����������Ϸ�����׼�¼������ɹ�"));

	return 0;
}

//�о���ť
LRESULT CGameClientDlg::OnStudy(WPARAM wParam, LPARAM lParam)
{
	//�����û�
	if (m_wBlackUser!=INVALID_CHAIR)
	{
		BYTE cbChessColor=(GetMeChairID()==m_wBlackUser)?BLACK_CHESS:WHITE_CHESS;
		m_GameClientView.m_ChessManual.m_ChessBorad.SetChessColor(cbChessColor);
	}

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
		m_GameClientView.m_ChessBorad.SetPositively(false);
	}

	//������Ϣ
	CMD_C_RegretAnswer ReqretAnswer;
	ReqretAnswer.cbApprove=BYTE(wParam);
	SendData(SUB_C_REGRET_ANSWER,&ReqretAnswer,sizeof(ReqretAnswer));

	return 0;
}

//ѡ������
LRESULT CGameClientDlg::OnSelectChess(WPARAM wParam, LPARAM lParam)
{
	//ִ������
	if (m_wCurrentUser==GetMeChairID())	PerformMoveChess((BYTE)wParam);

	return 0;
}

//�������
LRESULT CGameClientDlg::OnHitChessBorad(WPARAM wParam, LPARAM lParam)
{
	if (m_wCurrentUser==GetMeChairID())
	{
		//��������
		bool bMoveChess=false;
		m_cbXTargetPos=(BYTE)wParam;
		m_cbYTargetPos=(BYTE)lParam;
		const tagChessItem * pChessItem=m_GameLogic.GetChessItem(m_cbXTargetPos,m_cbYTargetPos);

		//ȡ���ж�
		if ((m_cbXSourcePos==m_cbXTargetPos)&&(m_cbYSourcePos==m_cbYTargetPos))
		{
			CancelChessSelect();
			return 0;
		}

		//ѡ���ж�
		if ((pChessItem!=NULL)&&(pChessItem->cbColor==m_cbChessColor))
		{
			//������λ
			if ((m_cbXSourcePos!=255)&&(m_cbYSourcePos!=255))
			{
				bMoveChess=m_GameLogic.IsWalkLegality(m_cbXSourcePos,m_cbYSourcePos,m_cbXTargetPos,m_cbYTargetPos);
			}

			//ѡ������
			if (bMoveChess==false)
			{
				m_cbXSourcePos=m_cbXTargetPos;
				m_cbYSourcePos=m_cbYTargetPos;
				PlayGameSound(AfxGetInstanceHandle(),TEXT("CHESS_SELECT"));
				m_GameClientView.m_ChessBorad.SetSelectChess(m_cbXSourcePos,m_cbYSourcePos);

				return 0;
			}
		}

		//�����ж�
		if ((m_cbXSourcePos!=255)&&(m_cbYSourcePos!=255))
		{
			bMoveChess=m_GameLogic.IsWalkLegality(m_cbXSourcePos,m_cbYSourcePos,m_cbXTargetPos,m_cbYTargetPos);
		}

		//�����ж�
		if (bMoveChess==true)
		{
			//�����ж�
			const tagChessItem * pSourceChessItem=m_GameLogic.GetChessItem(m_cbXSourcePos,m_cbYSourcePos);
			if ((pSourceChessItem->cbChess==CHESS_SOLDIER)&&((m_cbYTargetPos==0)||(m_cbYTargetPos==7)))
			{
				m_GameClientView.m_ChessBorad.SetPositively(false);
				m_GameClientView.m_ChessSelect.ShowWindow(SW_SHOW);
				InsertSystemString(TEXT("����ѡ�񡰱������������"));
				return 0;
			}

			//ִ������
			PerformMoveChess(NO_CHESS);

			return 0;
		}
		else
		{
			CancelChessSelect();
			return 0;
		}
	}

	return 0;
}

//ȡ��ѡ��
void CGameClientDlg::CancelChessSelect()
{
	//���ñ���
	m_cbXSourcePos=255;
	m_cbYSourcePos=255;
	m_cbXTargetPos=255;
	m_cbYTargetPos=255;

	//���ý���
	m_GameClientView.m_ChessBorad.SetSelectChess(m_cbXSourcePos,m_cbYSourcePos);

	return;
}

//ִ������
void CGameClientDlg::PerformMoveChess(BYTE cbSwitchChess)
{
	//Ч��״̬
	ASSERT((m_cbXSourcePos<8)&&(m_cbYSourcePos<8));
	ASSERT((m_cbXTargetPos<8)&&(m_cbYTargetPos<8));

	//��ȡ����
	const tagChessItem * pSourceChessItem=m_GameLogic.GetChessItem(m_cbXSourcePos,m_cbYSourcePos);
	const tagChessItem * pTargetChessItem=m_GameLogic.GetChessItem(m_cbXTargetPos,m_cbYTargetPos);

	//�ƶ�����
	m_ShamView=true;
	m_GameLogic.MoveChess(m_cbXSourcePos,m_cbYSourcePos,m_cbXTargetPos,m_cbYTargetPos,cbSwitchChess);

	//��������
	m_GameClientView.m_ChessBorad.SetPositively(false);
	m_GameClientView.m_ChessBorad.SetChessBorad(m_GameLogic.m_ChessBorad);

	//Σ���ж�
	if (m_GameLogic.IsKingDanger(m_cbChessColor)==true)
	{
		int nResult=AfxMessageBox(TEXT("[ �� ] ���ᱻɱ��ȷ��Ҫ����������"),MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2);
		if (m_wCurrentUser!=GetMeChairID()) return;
		if (nResult!=IDYES)
		{
			//ȡ��ѡ��
			CancelChessSelect();

			//�������
			m_ShamView=false;
			m_GameLogic.RegretChess(1);
			m_GameClientView.m_ChessBorad.SetPositively(true);
			m_GameClientView.m_ChessBorad.SetChessBorad(m_GameLogic.m_ChessBorad);

			return;
		}
	}

	//���ÿ��
	m_GameClientView.m_ChessBorad.SetChessFrame(m_cbXSourcePos,m_cbYSourcePos,m_cbXTargetPos,m_cbYTargetPos);

	//�����ж�
	WORD wStepCount=m_GameLogic.GetStepCount((GetMeChairID()==m_wBlackUser)?BLACK_CHESS:WHITE_CHESS);
	if (wStepCount==1) m_GameClientView.m_btRegret.EnableWindow(TRUE);

	//��������
	if ((pTargetChessItem!=NULL)&&(pSourceChessItem->cbColor!=pTargetChessItem->cbColor))
	{
		if (pSourceChessItem->cbColor==m_cbChessColor) PlayGameSound(AfxGetInstanceHandle(),TEXT("CHESS_EAT"));
		else PlayGameSound(AfxGetInstanceHandle(),TEXT("CHESS_DEAD"));
	}
	else PlayGameSound(AfxGetInstanceHandle(),TEXT("PLACE_CHESS"));

	//������Ϣ
	CMD_C_MoveChess MoveChess;
	MoveChess.cbXSourcePos=m_cbXSourcePos;
	MoveChess.cbYSourcePos=m_cbYSourcePos;
	MoveChess.cbXTargetPos=m_cbXTargetPos;
	MoveChess.cbYTargetPos=m_cbYTargetPos;
	MoveChess.cbSwitchChess=cbSwitchChess;
	SendData(SUB_C_MOVE_CHESS,&MoveChess,sizeof(MoveChess));

	//���ñ���
	m_cbXSourcePos=255;
	m_cbYSourcePos=255;
	m_cbXTargetPos=255;
	m_cbYTargetPos=255;
	m_GameClientView.m_ChessBorad.SetSelectChess(255,255);

	return;
}

//��������
bool CGameClientDlg::SaveChessManual(LPCTSTR pszFileName)
{
	//���ļ�
	CFile FileManual;
	if (FileManual.Open(pszFileName,CFile::modeWrite|CFile::modeCreate)==FALSE) return false;

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

	return true;
}

//////////////////////////////////////////////////////////////////////////
