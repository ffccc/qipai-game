#include "Stdafx.h"
#include "GameClient.h"
#include "GameOption.h"
#include "GameClientDlg.h"

//////////////////////////////////////////////////////////////////////////
//�궨��

#define IDI_GAME_TIMER				100									//��Ϸ��ʱ��

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientDlg, CGameFrameDlg)
	ON_WM_TIMER()
	ON_MESSAGE(IDM_START,OnStart)
	ON_MESSAGE(IDM_PEACE,OnPeace)
	ON_MESSAGE(IDM_STUDY,OnStudy)
	ON_MESSAGE(IDM_REGRET,OnRegret)
	ON_MESSAGE(IDM_GIVEUP,OnGiveUp)
	ON_MESSAGE(IDM_PRESERVE,OnPreserve)
	ON_MESSAGE(IDM_PEACE_ANSWER,OnPeaceAnswer)
	ON_MESSAGE(IDM_REGRET_ANSWER,OnRegretAnswer)
	ON_MESSAGE(WM_HIT_CHESS_BORAD, OnHitChessBorad)
	ON_MESSAGE(IDM_AGREE_GAME_RULE, OnAgreeGameRule)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientDlg::CGameClientDlg() : CGameFrameDlg(&m_GameClientView)
{
	//���ñ���
	m_bAutoSaveManual=false;

	//��Ϸ����
	m_cbChessColor=BLACK_CHESS;
	m_wBlackUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	memset(m_StatusInfo,0,sizeof(m_StatusInfo));
	memset(&m_GameRuleInfo,0,sizeof(m_GameRuleInfo));

	//��������
	m_ShamView=false;
	m_bAgreeRule=false;
	m_bCustomize=false;
	m_bLimitTime=false;
	m_cbXSourcePos=255;
	m_cbYSourcePos=255;
	m_cbXTargetPos=255;
	m_cbYTargetPos=255;

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
	SetWindowText(TEXT("�й�����"));

	//����ͼ��
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);

	//��ȡ����
	m_bAutoSaveManual=AfxGetApp()->GetProfileInt(TEXT("GameOption"),TEXT("AutoSaveManual"),FALSE)?true:false;

	return true;
}

//���ÿ��
void CGameClientDlg::ResetGameFrame()
{
	//��������
	m_ShamView=false;
	m_bAgreeRule=false;
	m_bCustomize=false;
	m_bLimitTime=false;
	m_cbXSourcePos=255;
	m_cbYSourcePos=255;
	m_cbXTargetPos=255;
	m_cbYTargetPos=255;

	//��Ϸ����
	m_cbChessColor=BLACK_CHESS;
	m_wBlackUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	memset(m_StatusInfo,0,sizeof(m_StatusInfo));
	memset(&m_GameRuleInfo,0,sizeof(m_GameRuleInfo));

	return;
}

//��Ϸ����
void CGameClientDlg::OnGameOptionSet()
{
	//��������
	CGameOption GameOption;
	GameOption.m_bEnableSound=IsEnableSound();
	GameOption.m_bAutoSaveManual=m_bAutoSaveManual;

	//��������
	if (GameOption.DoModal()==IDOK)
	{
		//���ñ���
		m_bAutoSaveManual=GameOption.m_bAutoSaveManual;

		//��������
		AfxGetApp()->WriteProfileInt(TEXT("GameOption"),TEXT("AutoSaveManual"),m_bAutoSaveManual);

		//��������
		EnableSound(GameOption.m_bEnableSound);
	}

	return;
}

//ʱ����Ϣ
bool CGameClientDlg::OnTimerMessage(WORD wChairID, UINT nElapse, UINT nTimerID)
{
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
	case SUB_S_REQ_SET_RULE:	//��������
		{
			return OnSubSetRule(pBuffer,wDataSize);
		}
	case SUB_S_REQ_AGREE_RULE:	//����ͬ��
		{
			return OnSubAgreeRule(pBuffer,wDataSize);
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
	m_wBlackUser=pGameStart->wBlackUser;
	m_wCurrentUser=pGameStart->wBlackUser;
	m_GameRuleInfo=pGameStart->GameRuleInfo;
	m_cbChessColor=(m_wBlackUser==GetMeChairID())?BLACK_CHESS:WHITE_CHESS;

	//��������
	m_ShamView=false;
	m_bCustomize=false;
	m_bAgreeRule=false;
	m_cbXSourcePos=255;
	m_cbYSourcePos=255;
	m_cbXTargetPos=255;
	m_cbYTargetPos=255;
	memset(m_StatusInfo,0,sizeof(m_StatusInfo));
	m_bLimitTime=((m_GameRuleInfo.wRuleStepTime>0)||(m_GameRuleInfo.wRuleLimitTime>0));

	//�������� 
	m_GameLogic.ResetChessBorad();
	m_GameClientView.m_ChessBorad.SetChessColor(m_cbChessColor);
	m_GameClientView.m_ChessBorad.SetChessBorad(m_GameLogic.m_ChessBorad);

	//���ý���
	m_GameClientView.m_btPreserve.EnableWindow(TRUE);
	m_GameClientView.m_GameScoreWnd.ShowWindow(SW_HIDE);
	m_GameClientView.SetBlackUser(SwitchViewChairID(m_wBlackUser));

	//������Ϣ
	WORD wLimitTimeCount=m_GameRuleInfo.wRuleLimitTime;
	if (wLimitTimeCount==0) wLimitTimeCount=m_GameRuleInfo.wRuleStepTime;
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_StatusInfo[i].wUseTimeCount=0;
		m_StatusInfo[i].wLimitTimeCount=wLimitTimeCount;
		m_GameClientView.SetUserStatusInfo(SwitchViewChairID(i),m_StatusInfo[i]);
	}

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
		//���ñ���
		m_cbXSourcePos=255;
		m_cbYSourcePos=255;
		m_cbXTargetPos=255;
		m_cbYTargetPos=255;

		//���ý���
		ActiveGameFrame();
		m_GameClientView.m_ChessBorad.SetPositively(true);
		m_GameClientView.m_ChessBorad.SetSelectChess(m_cbXSourcePos,m_cbYSourcePos);
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

	//���ñ���
	m_wCurrentUser=pMoveChess->wCurrentUser;
	WORD wMoveUser=(pMoveChess->wCurrentUser+1)%GAME_PLAYER;

	//���ý���
	m_ShamView=false;
	WORD wMeChairID=GetMeChairID();
	if ((IsLookonMode()==true)||(wMoveUser!=wMeChairID))
	{
		//��������
		BYTE cbXSourcePos=pMoveChess->cbXSourcePos,cbYSourcePos=pMoveChess->cbYSourcePos;
		BYTE cbXTargetPos=pMoveChess->cbXTargetPos,cbYTargetPos=pMoveChess->cbYTargetPos;

		//��ȡ����
		const tagChessItem * pSourceChessItem=m_GameLogic.GetChessItem(cbXSourcePos,cbYSourcePos);
		const tagChessItem * pTargetChessItem=m_GameLogic.GetChessItem(cbXTargetPos,cbYTargetPos);

		//�ƶ�����
		m_GameLogic.MoveChess(cbXSourcePos,cbYSourcePos,cbXTargetPos,cbYTargetPos);

		//��������
		m_GameClientView.m_ChessBorad.SetChessBorad(m_GameLogic.m_ChessBorad);
		m_GameClientView.m_ChessBorad.SetChessFrame(cbXSourcePos,cbYSourcePos,cbXTargetPos,cbYTargetPos);

		//�����ж�
		bool bKingDanger=m_GameLogic.IsKingDanger(BLACK_CHESS);
		if (bKingDanger==false) bKingDanger=m_GameLogic.IsKingDanger(WHITE_CHESS);

		//��������
		if (bKingDanger==false)
		{
			if ((pTargetChessItem!=NULL)&&(pSourceChessItem->cbColor!=pTargetChessItem->cbColor))
			{
				if (pSourceChessItem->cbColor==m_cbChessColor) PlayGameSound(AfxGetInstanceHandle(),TEXT("CHESS_EAT"));
				else PlayGameSound(AfxGetInstanceHandle(),TEXT("CHESS_DEAD"));
			}
			else PlayGameSound(AfxGetInstanceHandle(),TEXT("CHESS_MOVE"));
		}
		else PlayGameSound(AfxGetInstanceHandle(),TEXT("GENERAL"));
	}

	//�û���Ϣ
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		BYTE cbColor=(i==m_wBlackUser)?BLACK_CHESS:WHITE_CHESS;
		m_StatusInfo[i].wUseTimeCount=pMoveChess->wUseClock[i];
		m_StatusInfo[i].wLimitTimeCount=pMoveChess->wLeftClock[i];
		m_GameClientView.SetUserStatusInfo(SwitchViewChairID(i),m_StatusInfo[i]);
	}

	//�������
	if ((IsLookonMode()==false)&&(pMoveChess->wCurrentUser==wMeChairID))
	{
		//���ñ���
		m_cbXSourcePos=255;
		m_cbYSourcePos=255;
		m_cbXTargetPos=255;
		m_cbYTargetPos=255;

		//���ý���
		ActiveGameFrame();
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
		//���ñ���
		m_cbXSourcePos=255;
		m_cbYSourcePos=255;
		m_cbXTargetPos=255;
		m_cbYTargetPos=255;

		//���ý���
		ActiveGameFrame();
		m_GameClientView.m_ChessBorad.SetPositively(true);
		m_GameClientView.m_ChessBorad.SetSelectChess(m_cbXSourcePos,m_cbYSourcePos);
	}

	//�û���Ϣ
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		BYTE cbColor=(i==m_wBlackUser)?BLACK_CHESS:WHITE_CHESS;
		m_StatusInfo[i].wUseTimeCount=pRegretResult->wUseClock[i];
		m_StatusInfo[i].wLimitTimeCount=pRegretResult->wLeftClock[i];
		m_GameClientView.SetUserStatusInfo(SwitchViewChairID(i),m_StatusInfo[i]);
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
		//���ñ���
		m_cbXSourcePos=255;
		m_cbYSourcePos=255;
		m_cbXTargetPos=255;
		m_cbYTargetPos=255;

		//���ý���
		ActiveGameFrame();
		m_GameClientView.m_btStart.EnableWindow(TRUE);
		m_GameClientView.m_btStart.SetFocus();
		m_GameClientView.m_ChessBorad.SetSelectChess(m_cbXSourcePos,m_cbYSourcePos);

		//�رմ���
		if (m_GameClientView.m_PeaceRequest.GetSafeHwnd()!=NULL) m_GameClientView.m_PeaceRequest.DestroyWindow();
		if (m_GameClientView.m_RegretRequest.GetSafeHwnd()!=NULL) m_GameClientView.m_RegretRequest.DestroyWindow();
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
		_snprintf(szFileName,sizeof(szFileName),TEXT("%s\\�й���������_%04ld_%02ld_%02ld_%02ld_%02ld_%02ld.CCM"),
			szPath,SystemTime.wYear,SystemTime.wMonth,SystemTime.wDay,SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond);

		//��������
		if (SaveChessManual(szFileName)==false)	InsertSystemString(TEXT("�й�������Ϸ�����׼�¼������ʧ��"));
		else InsertSystemString(TEXT("�й�������Ϸ�����׼�¼������ɹ�"));
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

//��������
bool CGameClientDlg::OnSubSetRule(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_Req_SetRult));
	if (wDataSize!=sizeof(CMD_S_Req_SetRult)) return false;

	//��Ϣ����
	CMD_S_Req_SetRult * pSetRult=(CMD_S_Req_SetRult *)pBuffer;
	m_GameRuleInfo=pSetRult->GameRuleInfo;

	//���ý���
	if (IsLookonMode()==false)
	{
		//���ñ���
		m_bCustomize=true;
		m_bAgreeRule=false;
		m_wBlackUser=GetMeChairID();
		m_GameClientView.SetBlackUser(SwitchViewChairID(m_wBlackUser));

		//���ý���
		if (m_GameClientView.m_btStart.IsWindowEnabled()==FALSE)
		{
			//��������
			m_GameClientView.m_GameRule.m_GameRuleInfo=m_GameRuleInfo;

			//��ʾ����
			if (m_GameClientView.m_GameRule.GetSafeHwnd()==NULL)
			{
				m_GameClientView.m_GameRule.Create(IDD_GAME_RULE,&m_GameClientView);
			}
			else m_GameClientView.m_GameRule.UpdateControlStatus();

			//���ÿؼ�
			m_GameClientView.m_GameRule.ControlEnable(true);
			m_GameClientView.m_GameRule.ShowWindow(SW_SHOW);

			//�������
			ActiveGameFrame();
		}
	}

	return true;
}

//����ͬ��
bool CGameClientDlg::OnSubAgreeRule(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_Req_AgreeRult));
	if (wDataSize!=sizeof(CMD_S_Req_AgreeRult)) return false;

	//��Ϣ����
	CMD_S_Req_AgreeRult * pAgreeRult=(CMD_S_Req_AgreeRult *)pBuffer;
	m_GameRuleInfo=pAgreeRult->GameRuleInfo;

	//���ý���
	if (IsLookonMode()==false)
	{
		//���ñ���
		m_bAgreeRule=true;
		m_bCustomize=false;

		//���ý���
		if (m_GameClientView.m_btStart.IsWindowEnabled()==FALSE)
		{
			//��������
			m_GameClientView.m_GameRule.m_GameRuleInfo=m_GameRuleInfo;

			//��ʾ����
			if (m_GameClientView.m_GameRule.GetSafeHwnd()==NULL)
			{
				m_GameClientView.m_GameRule.Create(IDD_GAME_RULE,&m_GameClientView);
			}
			else m_GameClientView.m_GameRule.UpdateControlStatus();

			//���ÿؼ�
			m_GameClientView.m_GameRule.ControlEnable(false);
			m_GameClientView.m_GameRule.ShowWindow(SW_SHOW);

			//�������
			ActiveGameFrame();
		}
	}

	return true;
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
			m_GameRuleInfo=pStatusFree->GameRuleInfo;
			m_bCustomize=((IsLookonMode()==false)&&(m_wBlackUser==GetMeChairID()));

			//���ý���
			m_GameClientView.SetBlackUser(SwitchViewChairID(m_wBlackUser));

			//���ù���
			bool bShowGameRule=((IsLookonMode()==false)&&((m_wBlackUser==GetMeChairID())||(pStatusFree->cbGameRule==TRUE)));
			if (bShowGameRule==true)
			{
				//��������
				m_GameClientView.m_GameRule.m_GameRuleInfo=m_GameRuleInfo;

				//��ʾ����
				if (m_GameClientView.m_GameRule.GetSafeHwnd()==NULL)
				{
					m_GameClientView.m_GameRule.Create(IDD_GAME_RULE,&m_GameClientView);
				}
				else m_GameClientView.m_GameRule.UpdateControlStatus();

				//���ÿؼ�
				bool bEnableControl=(m_wBlackUser==GetMeChairID());
				m_GameClientView.m_GameRule.ControlEnable(bEnableControl);
				m_GameClientView.m_GameRule.ShowWindow(SW_SHOW);
			}
			else InsertSystemString(TEXT("���ȴ����췽�����������Ϸ����"));

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
			m_GameRuleInfo=pStatusPlay->GameRuleInfo;
			m_cbChessColor=(m_wBlackUser==GetMeChairID())?BLACK_CHESS:WHITE_CHESS;
			m_bLimitTime=((m_GameRuleInfo.wRuleStepTime>0)||(m_GameRuleInfo.wRuleLimitTime>0));

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

			//�û���Ϣ
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				BYTE cbColor=(i==m_wBlackUser)?BLACK_CHESS:WHITE_CHESS;
				m_StatusInfo[i].wUseTimeCount=pStatusPlay->wUseClock[i];
				m_StatusInfo[i].wLimitTimeCount=pStatusPlay->wLeftClock[i];
				m_GameClientView.SetUserStatusInfo(SwitchViewChairID(i),m_StatusInfo[i]);
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
				if ((wStepCount>0)&&(m_GameRuleInfo.cbRegretFlag==TRUE))
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
		//����ʱ��
		if ((m_bLimitTime==true)&&(m_StatusInfo[m_wCurrentUser].wLimitTimeCount!=0xFFFF))
		{
			//�ݼ�ʱ��
			m_StatusInfo[m_wCurrentUser].wLimitTimeCount--;

			//ʱ���л�
			if (m_StatusInfo[m_wCurrentUser].wLimitTimeCount==0xFFFF)
			{
				if (m_GameRuleInfo.wRuleTimeOutCount>0)
				{
					//���ñ���
					m_GameRuleInfo.wRuleTimeOutCount--;
					m_StatusInfo[m_wCurrentUser].wLimitTimeCount=m_GameRuleInfo.wRuleStepTime;

					//�������
					if ((GetMeChairID()==m_wCurrentUser)&&(IsLookonMode()==false))
					{
						//������Ϣ
						SendData(SUB_C_TIME_OUT);

						//��ʾ��Ϣ
						TCHAR szMessage[128];
						_snprintf(szMessage,sizeof(szMessage),TEXT("���峬ʱ�ˣ����� %ld �� %ld ��ĳ�ʱ����"),
							m_GameRuleInfo.wRuleTimeOutCount+1,m_GameRuleInfo.wRuleStepTime);
						InsertSystemString(szMessage);
					}
				}
				else 
				{
					//ɾ����ʱ��
					KillTimer(IDI_GAME_TIMER);
					m_StatusInfo[m_wCurrentUser].wLimitTimeCount=0;

					//�������
					if ((GetMeChairID()==m_wCurrentUser)&&(IsLookonMode()==false))
					{
						//������Ϣ
						SendData(SUB_C_TIME_OUT);

						//���ý���
						m_GameClientView.m_btPeace.EnableWindow(FALSE);
						m_GameClientView.m_btRegret.EnableWindow(FALSE);
						m_GameClientView.m_btGiveUp.EnableWindow(FALSE);
						m_GameClientView.m_ChessBorad.SetPositively(false);

						//��ʾ��Ϣ
						InsertSystemString(TEXT("���峬ʱ���㱻����"));
						PlayGameSound(AfxGetInstanceHandle(),"TIME_OUT");
					}
				}
			}

			//��������
			if ((m_StatusInfo[m_wCurrentUser].wLimitTimeCount<=5)&&(GetMeChairID()==m_wCurrentUser)&&(IsLookonMode()==false))
			{
				PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));
			}
		}
		
		//������ͼ
		m_StatusInfo[m_wCurrentUser].wUseTimeCount++;
		m_GameClientView.SetUserStatusInfo(SwitchViewChairID(m_wCurrentUser),m_StatusInfo[m_wCurrentUser]);

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
	memset(m_StatusInfo,0,sizeof(m_StatusInfo));

	//���ý���
	m_GameClientView.m_ChessBorad.CleanChess();
	m_GameClientView.m_btStart.EnableWindow(FALSE);
	m_GameClientView.m_btPreserve.EnableWindow(FALSE);
	m_GameClientView.m_GameScoreWnd.ShowWindow(SW_HIDE);

	//���ƹ���
	if ((m_bCustomize==true)||(m_bAgreeRule==true))
	{
		//��������
		m_GameClientView.m_GameRule.m_GameRuleInfo=m_GameRuleInfo;

		//��ʾ����
		if (m_GameClientView.m_GameRule.GetSafeHwnd()==NULL)
		{
			m_GameClientView.m_GameRule.Create(IDD_GAME_RULE,&m_GameClientView);
		}
		else m_GameClientView.m_GameRule.UpdateControlStatus();

		//���ÿؼ�
		m_GameClientView.m_GameRule.ShowWindow(SW_SHOW);
		m_GameClientView.m_GameRule.ControlEnable(m_bCustomize);
	}
	else InsertSystemString(TEXT("���ȴ����췽�����������Ϸ����"));

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
	LPCTSTR pszFilter=TEXT("�й����������ļ� ��*.CCM��|*.CCM||");
	CFileDialog DlgSaveManual(FALSE,TEXT("CCM"),NULL,OFN_NOCHANGEDIR|OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,pszFilter);
	DlgSaveManual.m_pOFN->lpstrInitialDir=szPath;
	if (DlgSaveManual.DoModal()!=IDOK) return 0;

	//��������
	if (SaveChessManual(DlgSaveManual.GetPathName())==false)	
	{
		InsertSystemString(TEXT("�й�������Ϸ�����׼�¼������ʧ��"));
	}
	else InsertSystemString(TEXT("�й�������Ϸ�����׼�¼������ɹ�"));

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

//�������
LRESULT CGameClientDlg::OnHitChessBorad(WPARAM wParam, LPARAM lParam)
{
	if (m_wCurrentUser==GetMeChairID())
	{
		//��������
		m_cbXTargetPos=(BYTE)wParam;
		m_cbYTargetPos=(BYTE)lParam;
		const tagChessItem * pChessItem=m_GameLogic.GetChessItem(m_cbXTargetPos,m_cbYTargetPos);

		//ȡ���ж�
		if ((m_cbXSourcePos==m_cbXTargetPos)&&(m_cbYSourcePos==m_cbYTargetPos))
		{
			m_cbXSourcePos=255;
			m_cbYSourcePos=255;
			m_cbXTargetPos=255;
			m_cbYTargetPos=255;
			m_GameClientView.m_ChessBorad.SetSelectChess(m_cbXSourcePos,m_cbYSourcePos);

			return 0;
		}

		//ѡ���ж�
		if ((pChessItem!=NULL)&&(pChessItem->cbColor==m_cbChessColor))
		{
			m_cbXSourcePos=m_cbXTargetPos;
			m_cbYSourcePos=m_cbYTargetPos;
			PlayGameSound(AfxGetInstanceHandle(),TEXT("CHESS_SELECT"));
			m_GameClientView.m_ChessBorad.SetSelectChess(m_cbXSourcePos,m_cbYSourcePos);

			return 0;
		}

		//�����ж�
		bool bMoveChess=false;
		if ((m_cbXSourcePos!=255)&&(m_cbYSourcePos!=255))
		{
			const tagChessItem * pSourceChessItem=m_GameLogic.GetChessItem(m_cbXSourcePos,m_cbYSourcePos);
			bMoveChess=m_GameLogic.IsWalkLegality(pSourceChessItem,m_cbXTargetPos,m_cbYTargetPos);
		}

		//�����ж�
		if (bMoveChess==true)
		{
			//��ȡ����
			const tagChessItem * pSourceChessItem=m_GameLogic.GetChessItem(m_cbXSourcePos,m_cbYSourcePos);
			const tagChessItem * pTargetChessItem=m_GameLogic.GetChessItem(m_cbXTargetPos,m_cbYTargetPos);

			//�ƶ�����
			m_GameLogic.MoveChess(m_cbXSourcePos,m_cbYSourcePos,m_cbXTargetPos,m_cbYTargetPos);

			//��������
			m_GameClientView.m_ChessBorad.SetPositively(false);
			m_GameClientView.m_ChessBorad.SetChessBorad(m_GameLogic.m_ChessBorad);

			//Σ���ж�
			if (m_GameLogic.IsKingDanger(m_cbChessColor)==true)
			{
				int nResult=AfxMessageBox(TEXT("[ �� ] ���ᱻɱ��ȷ��Ҫ����������"),MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2);
				if (m_wCurrentUser!=GetMeChairID()) return 0;
				if (nResult!=IDYES)
				{
					//���ñ���
					m_cbXSourcePos=255;
					m_cbYSourcePos=255;
					m_cbXTargetPos=255;
					m_cbYTargetPos=255;

					//�������
					m_GameLogic.RegretChess(1);
					m_GameClientView.m_ChessBorad.SetPositively(true);
					m_GameClientView.m_ChessBorad.SetChessBorad(m_GameLogic.m_ChessBorad);
					m_GameClientView.m_ChessBorad.SetSelectChess(m_cbXSourcePos,m_cbYSourcePos);

					return 0;
				}
			}

			//���ñ���
			m_ShamView=true;
			m_wCurrentUser=INVALID_CHAIR;

			//��������
			m_GameClientView.m_ChessBorad.SetChessFrame(m_cbXSourcePos,m_cbYSourcePos,m_cbXTargetPos,m_cbYTargetPos);

			//�����ж�
			WORD wStepCount=m_GameLogic.GetStepCount((GetMeChairID()==m_wBlackUser)?BLACK_CHESS:WHITE_CHESS);
			if (wStepCount==1) m_GameClientView.m_btRegret.EnableWindow(TRUE);

			//�����ж�
			bool bKingDanger=m_GameLogic.IsKingDanger(BLACK_CHESS);
			if (bKingDanger==false) bKingDanger=m_GameLogic.IsKingDanger(WHITE_CHESS);

			//��������
			if (bKingDanger==false)
			{
				if (pTargetChessItem!=NULL) PlayGameSound(AfxGetInstanceHandle(),TEXT("CHESS_EAT"));
				else PlayGameSound(AfxGetInstanceHandle(),TEXT("CHESS_MOVE"));
			}
			else PlayGameSound(AfxGetInstanceHandle(),TEXT("GENERAL"));

			//������Ϣ
			CMD_C_MoveChess MoveChess;
			MoveChess.cbXSourcePos=m_cbXSourcePos;
			MoveChess.cbYSourcePos=m_cbYSourcePos;
			MoveChess.cbXTargetPos=m_cbXTargetPos;
			MoveChess.cbYTargetPos=m_cbYTargetPos;
			SendData(SUB_C_MOVE_CHESS,&MoveChess,sizeof(MoveChess));

			//���ñ���
			m_cbXSourcePos=255;
			m_cbYSourcePos=255;
			m_cbXTargetPos=255;
			m_cbYTargetPos=255;
			m_GameClientView.m_ChessBorad.SetSelectChess(255,255);

			return 0;
		}
		else
		{
			//ȡ��ѡ��
			m_cbXSourcePos=255;
			m_cbYSourcePos=255;
			m_cbXTargetPos=255;
			m_cbYTargetPos=255;
			m_GameClientView.m_ChessBorad.SetSelectChess(m_cbXSourcePos,m_cbYSourcePos);

			return 0;
		}
	}

	return 0;
}

//ͬ�����
LRESULT CGameClientDlg::OnAgreeGameRule(WPARAM wParam, LPARAM lParam)
{
	//�������
	m_GameRuleInfo=m_GameClientView.m_GameRule.m_GameRuleInfo;

	//����ͬ��
	if (m_bCustomize==true)
	{
		//��������
		CMD_C_CustomizeRult CustomizeRult;
		CustomizeRult.GameRuleInfo=m_GameRuleInfo;

		//������Ϣ
		SendUserReady(&CustomizeRult,sizeof(CustomizeRult));
	}
	else SendUserReady(NULL,0);

	return 0;
}

//////////////////////////////////////////////////////////////////////////
