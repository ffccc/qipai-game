#include "StdAfx.h"
#include "Resource.h"
#include "GameClient.h"
#include "GameClientDlg.h"
#include "GameOption.h"

//////////////////////////////////////////////////////////////////////////

//ʱ���ʶ
#define IDI_GAME_TIME				100									//��Ϸ��ʱ��
#define IDI_START_GAME				200									//��ʼ��ʱ��

//��Ϸʱ��
#define TIME_START_GAME				90									//��ʼ��ʱ��

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientDlg, CGameFrameDlg)

	//ϵͳ��Ϣ
	ON_WM_TIMER()

	//���ư�ť
	ON_MESSAGE(IDM_START,OnMessageStart)
	ON_MESSAGE(IDM_PEACE,OnMessagePeace)
	ON_MESSAGE(IDM_REGRET,OnMessageRegret)
	ON_MESSAGE(IDM_GIVEUP,OnMessageGiveUp)
	ON_MESSAGE(IDM_MANUAL,OnMessageManual)
	ON_MESSAGE(IDM_PRESERVE,OnMessagePreserve)

	//�ؼ���Ϣ
	ON_MESSAGE(WM_HIT_CHESS_BORAD, OnHitChessBorad)
	ON_MESSAGE(IDM_GAME_RULE_AGREE, OnGameRuleAgree)
	ON_MESSAGE(IDM_GAME_RULE_REJECT, OnGameRuleReject)
	ON_MESSAGE(IDM_ORIENTATION_MANUAL, OnOrientationManual)
	ON_MESSAGE(IDM_USER_REQUEST_RESPOND, OnUserRequestRespond)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientDlg::CGameClientDlg(): CGameFrameDlg(&m_GameClientView)
{
	//��Ϸ����
	m_cbChessColor=CHESS_BLACK;
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;

	//ѡ����Ϣ
	m_cbXSourcePos=255;
	m_cbYSourcePos=255;

	//��������
	m_bDrawTimeMode=false;
	m_bRequestAgree=false;
	m_bRequestCustomize=false;
	ZeroMemory(m_szUserAccounts,sizeof(m_szUserAccounts));

	//�������
	m_wRuleRoundID=0L;
	ZeroMemory(&m_GameRuleInfo,sizeof(m_GameRuleInfo));

	//״̬����
	m_wViewStepCount=0;
	ZeroMemory(&m_UserStatusInfo,sizeof(m_UserStatusInfo));

	//��������
	ZeroMemory(m_ChessItemView,sizeof(m_ChessItemView));
	ZeroMemory(m_ChessBoradView,sizeof(m_ChessBoradView));

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
	SetWindowText(TEXT("�й�����  --  Ver��6.0.1.0"));

	//����ͼ��
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);

	////�ں˳�ʼ
	//if (__super::InitGameEngine()==false) return false;

	//�����߼�
	m_GameLogic.ResetChessBorad();

	return true;
}

//��������
void CGameClientDlg::ResetGameFrame()
{
	//�ں�����
	//if (__super::ResetGameEngine()==false) return false;

	//ɾ��ʱ��
	KillTimer(IDI_GAME_TIME);
	KillGameTimer(IDI_START_GAME);

	//��Ϸ����
	m_cbChessColor=CHESS_BLACK;
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;

	//ѡ����Ϣ
	m_cbXSourcePos=255;
	m_cbYSourcePos=255;

	//��������
	m_bDrawTimeMode=false;
	m_bRequestAgree=false;
	m_bRequestCustomize=false;
	ZeroMemory(m_szUserAccounts,sizeof(m_szUserAccounts));

	//��Ϸ�߼�
	m_GameLogic.ResetChessBorad();

	//�������
	m_wRuleRoundID=0L;
	ZeroMemory(&m_GameRuleInfo,sizeof(m_GameRuleInfo));

	//״̬����
	m_wViewStepCount=0;
	ZeroMemory(&m_UserStatusInfo,sizeof(m_UserStatusInfo));

	//��������
	ZeroMemory(m_ChessItemView,sizeof(m_ChessItemView));
	ZeroMemory(m_ChessBoradView,sizeof(m_ChessBoradView));

	return ;
}

//��Ϸ����
void CGameClientDlg::OnGameOptionSet()
{
	//��������
	CGameOption GameOption;
	GameOption.m_bEnableSound=IsEnableSound();
	GameOption.m_bAllowLookon=IsAllowUserLookon();

	//��������
	if (GameOption.DoModal()==IDOK)
	{
		//��������
		EnableSound(GameOption.m_bEnableSound);
		AllowUserLookon(0,GameOption.m_bAllowLookon);
	}

	return;
}

//�Թ�״̬
void CGameClientDlg::OnLookonChanged(bool bLookonUser, const void * pBuffer, WORD wDataSize)
{
}

//ʱ����Ϣ
bool CGameClientDlg::OnTimerMessage(WORD wChairID, UINT nElapse, UINT nTimerID)
{
	switch (nTimerID)
	{
	case IDI_START_GAME:	//��Ϸ��ʼ
		{
			//�Զ�����
			if (nElapse==0)
			{
				//�뿪��Ϸ
				if ((IsLookonMode()==false)&&(wChairID==GetMeChairID())) AfxGetMainWnd()->PostMessage(WM_CLOSE);

				return true;
			}

			//��ʱ����
			if ((nElapse<=5)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false))
			{
				PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));
			}

			//���½���
			m_GameClientView.UpdateGameView(NULL);

			return true;
		}
	}

	return false;
}

//������Ϣ
bool  CGameClientDlg::OnGameMessage(WORD wSubCmdID,const VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_GAME_START:		//��Ϸ��ʼ
		{
			return OnSubGameStart(pData,wDataSize);
		}
	case SUB_S_MOVE_CHESS:		//�ƶ�����
		{
			return OnSubMoveChess(pData,wDataSize);
		}
	case SUB_S_GAME_END:		//��Ϸ����
		{
			return OnSubGameEnd(pData,wDataSize);
		}
	case SUB_S_REGRET_FAILE:	//����ʧ��
		{
			return OnSubRegretFaile(pData,wDataSize);
		}
	case SUB_S_REGRET_RESULT:	//������
		{
			return OnSubRegretResult(pData,wDataSize);
		}
	case SUB_S_REGRET_REQUEST:	//��������
		{
			return OnSubRegretRequest(pData,wDataSize);
		}
	case SUB_S_PEACE_RESPOND:	//������Ӧ
		{
			return OnSubPeaceRespond(pData,wDataSize);
		}
	case SUB_S_PEACE_REQUEST:	//��������
		{
			return OnSubPeaceRequest(pData,wDataSize);
		}
	case SUB_S_REQUEST_RULE:	//��������
		{
			return OnSubRequestRule(pData,wDataSize);
		}
	case SUB_S_REQUEST_AGREE:	//����ͬ��
		{
			return OnSubRequestAgree(pData,wDataSize);
		}
	case SUB_S_CHESS_MANUAL:	//��Ϸ����
		{
			return OnSubChessManual(pData,wDataSize);
		}
	}

	//�������
	ASSERT(FALSE);

	return true;
}

//��Ϸ����
bool  CGameClientDlg::OnGameSceneMessage(BYTE cbGameStatus, bool bLookonOther,const VOID * pData, WORD wDataSize)
{
	switch (cbGameStatus)
	{
	case GAME_SCENE_FREE:	//����״̬
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_StatusFree));
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;

			//��������
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pData;

			//��������
			WORD wMeChairID=GetMeChairID();
			bool bPlayerMode=(IsLookonMode()==false);

			//���ñ���
			m_wBankerUser=pStatusFree->wBankerUser;
			m_wCurrentUser=pStatusFree->wBankerUser;
			m_GameRuleInfo=pStatusFree->GameRuleInfo;

			//��������
			m_bRequestAgree=((m_wBankerUser!=wMeChairID)&&(pStatusFree->cbGameRule==TRUE));
			m_bRequestCustomize=((m_wBankerUser==wMeChairID)&&(pStatusFree->cbGameRule==FALSE));

			//���ý���
			m_GameClientView.SetGameRuleInfo(m_GameRuleInfo);
			m_GameClientView.SetBankerUser(SwitchViewChairID(m_wBankerUser));

			//���ù���
			if ((bPlayerMode==true)&&((m_bRequestAgree==true)||(m_bRequestCustomize==true)))
			{
				//��������
				m_GameClientView.m_DlgRuleOption.SetGameRuleInfo(m_GameRuleInfo,m_bRequestCustomize,true);

				//���ڴ���
				CDlgRuleOption * pDlgRuleOption=&m_GameClientView.m_DlgRuleOption;
				if (pDlgRuleOption->m_hWnd==NULL) pDlgRuleOption->Create(IDD_RULE_OPTION,&m_GameClientView);

				//��ʾ����
				pDlgRuleOption->ShowWindow(SW_SHOW);
				pDlgRuleOption->SetForegroundWindow();
			}
			else
			{
				//��ʾ��Ϣ
				if (bPlayerMode==false)
				{
					m_GameClientView.m_ChessBorad.SetWaitGameRule(true);
					InsertSystemString(TEXT("���ȴ����������Ϸ����"));
				}
				else
				{
					m_GameClientView.m_ChessBorad.SetWaitGameRule(true);
					InsertSystemString(TEXT("���ȴ����췽�����������Ϸ����"));
				}

				//���½���
				m_GameClientView.UpdateGameView(NULL);
			}

			return true;
		}
	case GAME_SCENE_PLAY:	//��Ϸ״̬
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_StatusPlay));
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;

			//��������
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pData;

			//��������
			WORD wMeChairID=GetMeChairID();
			bool bPlayerMode=(IsLookonMode()==false);

			//���ñ���
			m_wBankerUser=pStatusPlay->wBankerUser;
			m_wCurrentUser=pStatusPlay->wCurrentUser;
			m_cbChessColor=(m_wBankerUser==wMeChairID)?CHESS_BLACK:CHESS_WHITE;

			//�������
			m_GameRuleInfo=pStatusPlay->GameRuleInfo;
			m_bDrawTimeMode=(pStatusPlay->wLeaveDrawTime[m_wCurrentUser]>0);

			//���ý���
			m_GameClientView.m_btPreserve.EnableWindow(TRUE);
			m_GameClientView.SetBankerUser(SwitchViewChairID(m_wBankerUser));
			m_GameClientView.SetCurrentUser(SwitchViewChairID(m_wCurrentUser));

			//���̽���
			m_GameClientView.m_ChessBorad.SetChessColor(m_cbChessColor);
			m_GameClientView.m_ChessBorad.SetChessBorad(m_GameLogic.m_ChessBorad);

			//��������
			UpdateManualControl((WORD)m_GameLogic.m_ChessManualArray.GetCount());

			//����·��
			tagChessManual * pChessManual=m_GameLogic.GetLastChessManual();
			if (pChessManual!=NULL)
			{
				BYTE cbXPosStart=pChessManual->cbXSourceChessPos;
				BYTE cbYPosStart=pChessManual->cbYSourceChessPos;
				BYTE cbXPosTerminal=pChessManual->cbXTargetChessPos;
				BYTE cbYPosTerminal=pChessManual->cbYTargetChessPos;
				m_GameClientView.SetMoveChessTrace(cbXPosStart,cbYPosStart,cbXPosTerminal,cbYPosTerminal);
			}

			//�û���Ϣ
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				m_UserStatusInfo[i].wDrawTimeCount=pStatusPlay->wLeaveDrawTime[i];
				m_UserStatusInfo[i].wStepTimeCount=pStatusPlay->GameRuleInfo.wRuleStepTime;
				m_UserStatusInfo[i].wSecondTimeCount=pStatusPlay->GameRuleInfo.wRuleSecondTime;
				m_GameClientView.SetUserStatusInfo(SwitchViewChairID(i),m_UserStatusInfo[i]);
			}

			//�����û�
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				const tagUserData * pUserData=GetUserData(i);
				//IClientUserItem * pIClientUserItem=GetTableUserItem(i);
				lstrcpyn(m_szUserAccounts[i],pUserData->szName,CountArray(m_szUserAccounts[i]));
			}

			//�������
			if (bPlayerMode==true)
			{
				//��������
				BYTE cbMyselfStatus=pStatusPlay->cbRequestStatus[wMeChairID];
				BYTE cbEnemyStatus=pStatusPlay->cbRequestStatus[(wMeChairID+1)%GAME_PLAYER];
				WORD wStepCount=m_GameLogic.GetStepCount((wMeChairID==m_wBankerUser)?CHESS_BLACK:CHESS_WHITE);

				//��ť����
				m_GameClientView.m_btGiveUp.EnableWindow(TRUE);
				m_GameClientView.m_btPeace.EnableWindow(((cbMyselfStatus&GU_WAIT_PEACE)==0)?TRUE:FALSE);
				m_GameClientView.m_btRegret.EnableWindow(((cbMyselfStatus&GU_WAIT_REGRET)==0)?TRUE:FALSE);

				//���尴ť
				if ((wStepCount>0)&&(m_GameRuleInfo.cbRegretFlag==TRUE))
				{
					bool bAllowRegret=((cbMyselfStatus&GU_WAIT_REGRET)==0);
					m_GameClientView.m_btRegret.EnableWindow((bAllowRegret==true)?TRUE:FALSE);
				}

				//��������
				if (cbEnemyStatus&GU_WAIT_PEACE)
				{
					//��������
					CDlgUserRequest * pDlgUserRequest=&m_GameClientView.m_UserRequestPeace;
					if (pDlgUserRequest->m_hWnd==NULL) pDlgUserRequest->Create(IDD_USER_REQUEST,&m_GameClientView);

					//��ʾ����
					pDlgUserRequest->ShowWindow(SW_SHOW);
					pDlgUserRequest->SetForegroundWindow();
				}

				//��������
				if (cbEnemyStatus&GU_WAIT_REGRET) 
				{
					//��������
					CDlgUserRequest * pDlgUserRequest=&m_GameClientView.m_UserRequestPeace;
					if (pDlgUserRequest->m_hWnd==NULL) pDlgUserRequest->Create(IDD_USER_REQUEST,&m_GameClientView);

					//��ʾ����
					pDlgUserRequest->ShowWindow(SW_SHOW);
					pDlgUserRequest->SetForegroundWindow();
				}

				//��������
				if (wMeChairID==m_wCurrentUser) m_GameClientView.m_ChessBorad.SetPositively(true);
			}

			//����ʱ��
			SetTimer(IDI_GAME_TIME,1000,NULL);

			//���½���
			m_GameClientView.UpdateGameView(NULL);

			return true;
		}
	}

	return false;
}


//���°�ť
bool CGameClientDlg::UpdateManualControl(WORD wViewStepCount)
{
	//���ñ���
	m_wViewStepCount=wViewStepCount;
	WORD wManualCount=(WORD)m_GameLogic.m_ChessManualArray.GetCount();

	//���ÿؼ�
	m_GameClientView.m_btManualHead.EnableWindow(m_wViewStepCount>0);
	m_GameClientView.m_btManualLast.EnableWindow(m_wViewStepCount>0);
	m_GameClientView.m_btManualNext.EnableWindow(m_wViewStepCount<wManualCount);
	m_GameClientView.m_btManualTail.EnableWindow(m_wViewStepCount<wManualCount);

	//�����б�
	m_GameClientView.m_ChessBorad.SetManualView(m_wViewStepCount<wManualCount);
	m_GameClientView.m_ManualList.SetCurSel((m_wViewStepCount>0)?m_wViewStepCount-1:LB_ERR);

	return true;
}

//ִ������
bool CGameClientDlg::PerformMoveChess(BYTE cbXSourcePos, BYTE cbYSourcePos, BYTE cbXTargetPos, BYTE cbYTargetPos)
{
	//��ȡ����
	tagChessItem * pChessItem=NULL;
	pChessItem=m_GameLogic.GetChessItem(cbXTargetPos,cbYTargetPos);

	//�ƶ�����
	TCHAR szManualName[32]=TEXT("");
	tagMoveChessResult MoveChessResult;
	m_GameLogic.MoveChess(cbXSourcePos,cbYSourcePos,cbXTargetPos,cbYTargetPos,MoveChessResult,szManualName);

	//�������
	m_GameClientView.m_ManualList.AddString(szManualName);
	m_GameClientView.m_ChessBorad.SetChessBorad(m_GameLogic.m_ChessBorad);
	m_GameClientView.SetMoveChessTrace(cbXSourcePos,cbYSourcePos,cbXTargetPos,cbYTargetPos);

	//��������
	UpdateManualControl((WORD)m_GameLogic.m_ChessManualArray.GetCount());

	//��������
	if (MoveChessResult.bMinatoryKing==false)
	{
		if (pChessItem!=NULL) PlayGameSound(AfxGetInstanceHandle(),TEXT("EAT_CHESS"));
		else PlayGameSound(AfxGetInstanceHandle(),TEXT("MOVE_CHESS"));
	}
	else PlayGameSound(AfxGetInstanceHandle(),TEXT("GENERAL")); 

	//���½���
	m_GameClientView.UpdateGameView(NULL);

	return true;
}

//��Ϸ��ʼ
bool CGameClientDlg::OnSubGameStart(const VOID * pData, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;

	//��Ϣ����
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pData;

	//�Թ�����
	if(IsLookonMode())
	{
		m_GameClientView.m_ChessBorad.SetWaitGameRule(false);
	}

	//��Ϸ����
	m_wBankerUser=pGameStart->wBankerUser;
	m_wCurrentUser=pGameStart->wBankerUser;
	m_GameRuleInfo=pGameStart->GameRuleInfo;
	m_cbChessColor=(m_wBankerUser==GetMeChairID())?CHESS_BLACK:CHESS_WHITE;

	//ѡ����Ϣ
	m_cbXSourcePos=INVALID_POS;
	m_cbYSourcePos=INVALID_POS;

	//��������
	m_bRequestAgree=false;
	m_bRequestCustomize=false;
	m_bDrawTimeMode=(m_GameRuleInfo.wRuleStepTime>0);

	//�����û�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		const tagUserData * pUserData=GetUserData(i);
		lstrcpyn(m_szUserAccounts[i],pUserData->szName,CountArray(m_szUserAccounts[i]));
	}

	//״̬����
	m_wViewStepCount=0;
	ZeroMemory(&m_UserStatusInfo,sizeof(m_UserStatusInfo));

	//��������
	m_GameLogic.ResetChessBorad();
	m_GameClientView.m_ChessBorad.SetChessColor(m_cbChessColor);
	m_GameClientView.m_ChessBorad.SetChessBorad(m_GameLogic.m_ChessBorad);
	m_GameClientView.SetMoveChessTrace(INVALID_POS,INVALID_POS,INVALID_POS,INVALID_POS);

	//���׿ؼ�
	UpdateManualControl(0);
	m_GameClientView.m_ManualList.ResetContent();

	//��ͼ�ؼ�
	m_GameClientView.m_btPreserve.EnableWindow(TRUE);
	m_GameClientView.m_ScoreControl.RestorationData();

	//���ý���
	m_GameClientView.SetGameRuleInfo(m_GameRuleInfo);
	m_GameClientView.SetBankerUser(SwitchViewChairID(m_wBankerUser));
	m_GameClientView.SetCurrentUser(SwitchViewChairID(m_wCurrentUser));

	//�û�״̬
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_UserStatusInfo[i].wDrawTimeCount=m_GameRuleInfo.wRuleDrawTime;
		m_UserStatusInfo[i].wStepTimeCount=m_GameRuleInfo.wRuleStepTime;
		m_UserStatusInfo[i].wSecondTimeCount=m_GameRuleInfo.wRuleSecondTime;
		m_GameClientView.SetUserStatusInfo(SwitchViewChairID(i),m_UserStatusInfo[i]);
	}

	//��ť����
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
		m_GameClientView.m_ChessBorad.SetPositively(true);
	}

	//���ö�ʱ��
	SetTimer(IDI_GAME_TIME,1000,NULL);

	//���½���
	m_GameClientView.UpdateGameView(NULL);

	//��������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));

	return true;
}

//�ƶ�����
bool CGameClientDlg::OnSubMoveChess(const VOID * pData, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_MoveChess));
	if (wDataSize!=sizeof(CMD_S_MoveChess)) return false;

	//��Ϣ����
	CMD_S_MoveChess * pMoveChess=(CMD_S_MoveChess *)pData;

	//������
	CDlgHint * pDlgUserHint=&m_GameClientView.m_UserHint;
	if (pDlgUserHint->m_hWnd!=NULL) pDlgUserHint->DestroyWindow();

	//�����û�
	m_wCurrentUser=pMoveChess->wCurrentUser;
	m_GameClientView.SetCurrentUser(SwitchViewChairID(m_wCurrentUser));

	//ʱ��ģʽ
	if (m_wCurrentUser!=INVALID_CHAIR) m_bDrawTimeMode=(m_UserStatusInfo[m_wCurrentUser].wDrawTimeCount>0);

	//�ƶ�����
	if ((IsLookonMode()==true)||(pMoveChess->wActionUser!=GetMeChairID()))
	{
		//��������
		WORD wActionUser=pMoveChess->wActionUser;
		WORD wStepTimeCount=__min(m_GameRuleInfo.wRuleStepTime,pMoveChess->wLeaveDrawTime);

		//�������
		BYTE cbXSourcePos=pMoveChess->cbXSourcePos;
		BYTE cbYSourcePos=pMoveChess->cbYSourcePos;
		BYTE cbXTargetPos=pMoveChess->cbXTargetPos;
		BYTE cbYTargetPos=pMoveChess->cbYTargetPos;
		PerformMoveChess(cbXSourcePos,cbYSourcePos,cbXTargetPos,cbYTargetPos);

		//����ʱ��
		m_UserStatusInfo[wActionUser].wStepTimeCount=wStepTimeCount;
		m_UserStatusInfo[wActionUser].wDrawTimeCount=pMoveChess->wLeaveDrawTime;
		m_UserStatusInfo[wActionUser].wSecondTimeCount=m_GameRuleInfo.wRuleSecondTime;
		m_GameClientView.SetUserStatusInfo(SwitchViewChairID(wActionUser),m_UserStatusInfo[wActionUser]);
	}

	//�������
	if ((IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID()))
	{
		//���ñ���
		m_cbXSourcePos=255;
		m_cbYSourcePos=255;

		//���ý���
		ActiveGameFrame();
		m_GameClientView.m_ChessBorad.SetPositively(true);
		m_GameClientView.SelectChessItem(m_cbXSourcePos,m_cbYSourcePos);
	}

	//���½���
	m_GameClientView.UpdateGameView(NULL);

	return true;
}

//����ʧ��
bool CGameClientDlg::OnSubRegretFaile(const VOID * pData, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_RegretFaile));
	if (wDataSize!=sizeof(CMD_S_RegretFaile)) return false;

	//��Ϣ����
	CMD_S_RegretFaile * pRegretFaile=(CMD_S_RegretFaile *)pData;

	//�Լҷ���
	if (pRegretFaile->cbFaileReason==FR_PLAYER_OPPOSE)
	{
		m_GameClientView.m_btRegret.EnableWindow(TRUE);
		InsertSystemString(TEXT("�ԼҾܾ������ġ����塱����"));

		return true;
	}

	return true;
}

//������
bool CGameClientDlg::OnSubRegretResult(const VOID * pData, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_RegretResult));
	if (wDataSize!=sizeof(CMD_S_RegretResult)) return false;

	//��������
	CMD_S_RegretResult * pRegretResult=(CMD_S_RegretResult *)pData;

	//���ñ���
	m_wCurrentUser=pRegretResult->wCurrentUser;
	m_bDrawTimeMode=(m_UserStatusInfo[m_wCurrentUser].wDrawTimeCount>0);

	//�����û�
	m_GameClientView.SetCurrentUser(SwitchViewChairID(m_wCurrentUser));

	//�����߼�
	m_GameLogic.RegretChess(pRegretResult->wTargetStep);

	//��������
	m_GameClientView.m_ChessBorad.SetManualView(false);
	m_GameClientView.m_ChessBorad.SetChessBorad(m_GameLogic.m_ChessBorad);
	m_GameClientView.m_ChessBorad.SelectChessItem(INVALID_POS,INVALID_POS);

	//���ÿ��
	if (m_GameLogic.m_ChessManualArray.GetCount()>0) 
	{
		tagChessManual * pChessManual=m_GameLogic.GetLastChessManual();
		m_GameClientView.SetMoveChessTrace(pChessManual->cbXSourceChessPos,pChessManual->cbYSourceChessPos,
			pChessManual->cbXTargetChessPos,pChessManual->cbYTargetChessPos);
	}
	else m_GameClientView.m_ChessBorad.SetMoveChessTrace(INVALID_POS,INVALID_POS,INVALID_POS,INVALID_POS);

	//��������
	do
	{
		//��ȡ����
		INT nListCount=m_GameClientView.m_ManualList.GetCount();

		//ɾ������
		if (nListCount>pRegretResult->wTargetStep) 
		{
			m_GameClientView.m_ManualList.DeleteString(nListCount-1);
			continue;
		}

		//��������
		UpdateManualControl(pRegretResult->wTargetStep);

		break;

	} while (true);

	//����ʱ��
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//����ʱ��
		m_UserStatusInfo[i].wDrawTimeCount=pRegretResult->wLeaveDrawTime[i];
		m_UserStatusInfo[i].wSecondTimeCount=m_GameRuleInfo.wRuleSecondTime;
		m_UserStatusInfo[i].wStepTimeCount=__min(pRegretResult->wLeaveDrawTime[i],m_GameRuleInfo.wRuleStepTime);

		//���ý���
		m_GameClientView.SetUserStatusInfo(SwitchViewChairID(i),m_UserStatusInfo[i]);
	}

	//�������
	WORD wMeChairID=GetMeChairID();
	if ((IsLookonMode()==false)&&(pRegretResult->wCurrentUser==wMeChairID))
	{
		//���ñ���
		m_cbXSourcePos=INVALID_POS;
		m_cbYSourcePos=INVALID_POS;

		//���ý���
		ActiveGameFrame();
		m_GameClientView.m_ChessBorad.SetPositively(true);
		m_GameClientView.SelectChessItem(INVALID_POS,INVALID_POS);
	}

	//��ť����
	if (IsLookonMode()==false)
	{
		WORD wStepCount=m_GameLogic.GetStepCount(m_cbChessColor);
		if (wStepCount==0) m_GameClientView.m_btRegret.EnableWindow(FALSE);
		if ((wStepCount>0)&&(pRegretResult->wRegretUser==wMeChairID)) m_GameClientView.m_btRegret.EnableWindow(TRUE);
	}

	//������ͼ
	m_GameClientView.UpdateGameView(NULL);

	return true;
}

//��������
bool CGameClientDlg::OnSubRegretRequest(const VOID * pData, WORD wDataSize)
{
	//��������
	CDlgUserRequest * pDlgUserRequest=&m_GameClientView.m_UserRequestRegret;
	if (pDlgUserRequest->m_hWnd==NULL) pDlgUserRequest->Create(IDD_USER_REQUEST,&m_GameClientView);

	//��ʾ����
	pDlgUserRequest->ShowWindow(SW_SHOW);
	pDlgUserRequest->SetForegroundWindow();

	return true;
}

//������Ӧ
bool CGameClientDlg::OnSubPeaceRespond(const VOID * pData, WORD wDataSize)
{
	//���ý���
	m_GameClientView.m_btPeace.EnableWindow(TRUE);

	//��ʾ��Ϣ
	InsertSystemString(TEXT("�ԼҾܾ������ġ����塱����"));

	return true;
}

//��������
bool CGameClientDlg::OnSubPeaceRequest(const VOID * pData, WORD wDataSize)
{
	//��������
	CDlgUserRequest * pDlgUserRequest=&m_GameClientView.m_UserRequestPeace;
	if (pDlgUserRequest->m_hWnd==NULL) pDlgUserRequest->Create(IDD_USER_REQUEST,&m_GameClientView);

	//��ʾ����
	pDlgUserRequest->ShowWindow(SW_SHOW);
	pDlgUserRequest->SetForegroundWindow();

	return true;
}

//��Ϸ����
bool CGameClientDlg::OnSubGameEnd(const VOID * pData, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;

	//������
	CDlgHint * pDlgUserHint=&m_GameClientView.m_UserHint;
	if (pDlgUserHint->m_hWnd!=NULL) pDlgUserHint->DestroyWindow();

	//��Ϣ����
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pData;

	//ѡ�����
	m_cbXSourcePos=INVALID_POS;
	m_cbYSourcePos=INVALID_POS;

	//���ñ���
	m_bRequestAgree=false;
	m_bRequestCustomize=false;
	m_wCurrentUser=INVALID_CHAIR;

	//��������
	tagScoreInfo ScoreInfo;
	ZeroMemory(&ScoreInfo,sizeof(ScoreInfo));

	//����ԭ��
	ScoreInfo.wMeChiarID = GetMeChairID();
	ScoreInfo.cbReason=pGameEnd->cbReason;
	if(m_GameLogic.GetStepCount(CHESS_WHITE)<5)ScoreInfo.cbReason=REASON_TEN_STEP;
	else if(m_GameLogic.GetStepCount(CHESS_WHITE)<10)ScoreInfo.cbReason=REASON_TWENTY_STEP;

	//�ɼ��û�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��ȡ�û�
		const tagUserData * pUserData=GetUserData(i);
		if (pUserData==NULL) continue;

		//���óɼ�
		ScoreInfo.lGameScore[i]=pGameEnd->lGameScore[i];

		//����ʤ��
		//if(ScoreInfo.cbReason<REASON_TEN_STEP)
		//{
		//	FLOAT fPlayCount = FLOAT(pUserData->lFleeCount+pUserData->lWinCount+pUserData->lDrawCount+pUserData->lLostCount)+1;
		//	FLOAT fWinCount = FLOAT(pUserData->lWinCount);
		//	if(pGameEnd->lGameScore[i]>=0 && pGameEnd->cbReason<REASON_AGREE_DRAW)fWinCount++;
		//	ScoreInfo.fWinRate[i] = fWinCount/fPlayCount*100;
		//}
		//else 
		{
			FLOAT fPlayCount = FLOAT(pUserData->lWinCount+pUserData->lDrawCount+pUserData->lLostCount+pUserData->lFleeCount);
			FLOAT fWinCount = FLOAT(pUserData->lWinCount);
			if((INT)fPlayCount==0) ScoreInfo.fWinRate[i] = 0.0f;
			else ScoreInfo.fWinRate[i] = fWinCount/fPlayCount*100;
		}
		//lstrcpyn(ScoreInfo.szLevelName[i],m_GameClientView.GetLevelDescribe(pIClientUserItem),CountArray(ScoreInfo.szLevelName[i]));

		//��������
		//CW2CT strAccounts(pUserData->szName);
		lstrcpyn(ScoreInfo.szUserName[i],(LPCTSTR)pUserData->szName,CountArray(ScoreInfo.szUserName[i]));
	}

	//�ɼ�����
	m_GameClientView.m_ScoreControl.SetScoreInfo(ScoreInfo);

	//��������
	LONGLONG lMeScore=pGameEnd->lGameScore[GetMeChairID()];
	if (lMeScore>0L) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WIN"));
	else if (lMeScore<0L) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_LOSE"));
	else PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_END"));

	//���ý���
	if (IsLookonMode()==false)
	{
		//���ý���
		ActiveGameFrame();
		m_GameClientView.m_btStart.EnableWindow(TRUE);
		m_GameClientView.m_btPeace.EnableWindow(FALSE);
		m_GameClientView.m_btRegret.EnableWindow(FALSE);
		m_GameClientView.m_btGiveUp.EnableWindow(FALSE);

		//��ʼʱ��
		SetGameTimer(GetMeChairID(),IDI_START_GAME,TIME_START_GAME);

		//��������
		m_GameClientView.m_ChessBorad.SetPositively(false);
		m_GameClientView.SetMoveChessTrace(INVALID_POS,INVALID_POS,INVALID_POS,INVALID_POS);

		//�رմ���
		if (m_GameClientView.m_UserRequestPeace.m_hWnd!=NULL) m_GameClientView.m_UserRequestPeace.DestroyWindow();
		if (m_GameClientView.m_UserRequestRegret.m_hWnd!=NULL) m_GameClientView.m_UserRequestRegret.DestroyWindow();
	}

	//���ý���
	m_GameClientView.SetCurrentUser(INVALID_CHAIR);

	//ɾ����ʱ��
	KillTimer(IDI_GAME_TIME);

	//��������
/*	if (m_bAutoSaveManual==true)
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
	}*/

	//���½���
	m_GameClientView.UpdateGameView(NULL);

	return true;
}

//��������
bool CGameClientDlg::OnSubRequestRule(const VOID * pData, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_RequestRule));
	if (wDataSize!=sizeof(CMD_S_RequestRule)) return false;

	//��������
	CMD_S_RequestRule * pRequestRule=(CMD_S_RequestRule *)pData;

	//���ñ���
	m_wRuleRoundID=pRequestRule->wRuleRoundID;
	m_GameRuleInfo=pRequestRule->GameRuleInfo;

	//���ý���
	if (IsLookonMode()==false)
	{
		//���ñ���
		m_bRequestAgree=false;
		m_bRequestCustomize=true;
		m_wBankerUser=GetMeChairID();

		//���ý���
		if (m_GameClientView.m_btStart.IsWindowEnabled()==FALSE)
		{
			//���ý���
			m_GameClientView.SetGameRuleInfo(m_GameRuleInfo);
			m_GameClientView.SetBankerUser(SwitchViewChairID(m_wBankerUser));

			//���ÿؼ�
			m_GameClientView.m_ChessBorad.SetWaitGameRule(false);
			m_GameClientView.m_DlgRuleOption.SetGameRuleInfo(m_GameRuleInfo,m_bRequestCustomize,true);

			//���ڴ���
			CDlgRuleOption * pDlgRuleOption=&m_GameClientView.m_DlgRuleOption;
			if (pDlgRuleOption->m_hWnd==NULL) pDlgRuleOption->Create(IDD_RULE_OPTION,&m_GameClientView);

			//��ʾ����
			pDlgRuleOption->ShowWindow(SW_SHOW);
			pDlgRuleOption->SetForegroundWindow();

			//���½���
			m_GameClientView.UpdateGameView(NULL);
		}
	}

	return true;
}

//����ͬ��
bool CGameClientDlg::OnSubRequestAgree(const VOID * pData, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_RequestRule));
	if (wDataSize!=sizeof(CMD_S_RequestRule)) return false;

	//��������
	CMD_S_RequestRule * pRequestRule=(CMD_S_RequestRule *)pData;

	//���ñ���
	m_wRuleRoundID=pRequestRule->wRuleRoundID;
	m_GameRuleInfo=pRequestRule->GameRuleInfo;

	//���ý���
	if (IsLookonMode()==false)
	{
		//���ñ���
		m_bRequestAgree=true;
		m_bRequestCustomize=false;
		m_wBankerUser=(GetMeChairID()+1)%GAME_PLAYER;

		//���ý���
		if (m_GameClientView.m_btStart.IsWindowEnabled()==FALSE)
		{
			//���ý���
			m_GameClientView.SetGameRuleInfo(m_GameRuleInfo);
			m_GameClientView.SetBankerUser(SwitchViewChairID(m_wBankerUser));

			//���ÿؼ�
			m_GameClientView.m_ChessBorad.SetWaitGameRule(false);
			m_GameClientView.m_DlgRuleOption.SetGameRuleInfo(m_GameRuleInfo,m_bRequestCustomize,true);

			//���ڴ���
			CDlgRuleOption * pDlgRuleOption=&m_GameClientView.m_DlgRuleOption;
			if (pDlgRuleOption->m_hWnd==NULL) pDlgRuleOption->Create(IDD_RULE_OPTION,&m_GameClientView);

			//��ʾ����
			pDlgRuleOption->ShowWindow(SW_SHOW);
			pDlgRuleOption->SetForegroundWindow();

			//���½���
			m_GameClientView.UpdateGameView(NULL);
		}
	}

	return true;
}

//��Ϸ����
bool CGameClientDlg::OnSubChessManual(const VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize%sizeof(tagChessManual)==0);
	if (wDataSize%sizeof(tagChessManual)!=0) return false;

	//��������
	WORD wManualCount=wDataSize/sizeof(tagChessManual);
	tagChessManual * pChessManual=(tagChessManual *)pData;

	//��������
	for (WORD i=0;i<wManualCount;i++) 
	{
		//��������
		tagChessManual * pChessManualItem=(pChessManual+i);
		BYTE cbXSourcePos=pChessManualItem->cbXSourceChessPos;
		BYTE cbYSourcePos=pChessManualItem->cbYSourceChessPos;
		BYTE cbXTargetPos=pChessManualItem->cbXTargetChessPos;
		BYTE cbYTargetPos=pChessManualItem->cbYTargetChessPos;

		//��������
		TCHAR szManualName[32]=TEXT("");
		tagMoveChessResult MoveChessResult;
		m_GameLogic.MoveChess(cbXSourcePos,cbYSourcePos,cbXTargetPos,cbYTargetPos,MoveChessResult,szManualName);

		//���׽���
		m_GameClientView.m_ManualList.AddString(szManualName);
	}

	return true;
}

//��ʼ��Ϣ
LRESULT CGameClientDlg::OnMessageStart(WPARAM wParam, LPARAM lParam)
{
	//ѡ����Ϣ
	m_cbXSourcePos=255;
	m_cbYSourcePos=255;

	//��Ϸ����
	m_cbChessColor=CHESS_BLACK;
	m_wCurrentUser=INVALID_CHAIR;

	//״̬����
	m_wViewStepCount=0;
	ZeroMemory(&m_UserStatusInfo,sizeof(m_UserStatusInfo));

	//ɾ��ʱ��
	KillGameTimer(IDI_START_GAME);

	//��������
	m_GameLogic.ResetChessBorad();
	m_GameClientView.m_ChessBorad.CleanChessItem();
	m_GameClientView.m_ChessBorad.SetPositively(false);

	//���׿ؼ�
	UpdateManualControl(0);
	m_GameClientView.m_ManualList.ResetContent();

	//��ͼ����
	m_GameClientView.m_btStart.EnableWindow(FALSE);
	m_GameClientView.m_btPreserve.EnableWindow(FALSE);
	m_GameClientView.m_ScoreControl.RestorationData();

	//��������
	m_GameClientView.SetBankerUser(INVALID_CHAIR);
	for (WORD i=0;i<GAME_PLAYER;i++) m_GameClientView.SetUserStatusInfo(i,m_UserStatusInfo[i]);

	//���ƹ���
	if ((m_bRequestCustomize==true)||(m_bRequestAgree==true))
	{
		//��������
		m_GameClientView.SetGameRuleInfo(m_GameRuleInfo);
		m_GameClientView.SetBankerUser(SwitchViewChairID(m_wBankerUser));
		m_GameClientView.m_DlgRuleOption.SetGameRuleInfo(m_GameRuleInfo,m_bRequestCustomize,true);

		//���ڴ���
		CDlgRuleOption * pDlgRuleOption=&m_GameClientView.m_DlgRuleOption;
		if (pDlgRuleOption->m_hWnd==NULL) pDlgRuleOption->Create(IDD_RULE_OPTION,&m_GameClientView);

		//��ʾ����
		pDlgRuleOption->ShowWindow(SW_SHOW);
		pDlgRuleOption->SetForegroundWindow();
	}
	else
	{
		//��������
		m_wBankerUser=(GetMeChairID()+1)%GAME_PLAYER;
		m_GameClientView.SetBankerUser(SwitchViewChairID(m_wBankerUser));

		//��ʾ��Ϣ
		//ASSERT(m_pIStringMessage!=NULL);
		m_GameClientView.m_ChessBorad.SetWaitGameRule(true);
		InsertSystemString(TEXT("���ȴ����췽�����������Ϸ����"));
	}

	//���½���
	m_GameClientView.UpdateGameView(NULL);

	return 0;
}

//�����Ϣ
LRESULT CGameClientDlg::OnMessagePeace(WPARAM wParam, LPARAM lParam)
{
	//������Ϣ
	SendData(SUB_C_PEACE_REQUEST);

	//���ý���
	m_GameClientView.m_btPeace.EnableWindow(FALSE);
	InsertSystemString(TEXT("���ڵȴ��Լһ�Ӧ [ ���� ] ����..."));

	return 0;
}

//������Ϣ		 
LRESULT CGameClientDlg::OnMessageRegret(WPARAM wParam, LPARAM lParam)
{
	//������Ϣ
	SendData(SUB_C_REGRET_REQUEST);

	//���ý���
	m_GameClientView.m_btRegret.EnableWindow(FALSE);
	InsertSystemString(TEXT("���ڵȴ��Լһ�Ӧ [ ���� ] ����..."));

	return 0;
}

//������Ϣ		 
LRESULT CGameClientDlg::OnMessageGiveUp(WPARAM wParam, LPARAM lParam)
{
	//ɾ��ʱ��
	KillTimer(IDI_GAME_TIME);

	//���ý���
	m_GameClientView.m_btPeace.EnableWindow(FALSE);
	m_GameClientView.m_btRegret.EnableWindow(FALSE);
	m_GameClientView.m_btGiveUp.EnableWindow(FALSE);
	m_GameClientView.m_ChessBorad.SetPositively(false);

	//��������
	CMD_C_ConcludeRequest ConcludeRequest;
	ZeroMemory(&ConcludeRequest,sizeof(ConcludeRequest));

	//������Ϣ
	ConcludeRequest.cbReason=REASON_GIVE_UP;
	SendData(SUB_C_CONCLUDE_REQUEST,&ConcludeRequest,sizeof(ConcludeRequest));

	return 0;
}

//������Ϣ
LRESULT CGameClientDlg::OnMessageManual(WPARAM wParam, LPARAM lParam)
{
	//��������
	if (m_GameClientView.m_DlgChessManual.m_hWnd==NULL)
	{
		m_GameClientView.m_DlgChessManual.Create(IDD_CHESS_MANUAL,GetDesktopWindow());
	}

	//��ʾ����
	m_GameClientView.m_DlgChessManual.ShowWindow(SW_SHOW);
	m_GameClientView.m_DlgChessManual.SetForegroundWindow();

	return 0;
}

//������Ϣ		 
LRESULT CGameClientDlg::OnMessagePreserve(WPARAM wParam, LPARAM lParam)
{
	//��ȡʱ��
	SYSTEMTIME SystemTime;
	GetLocalTime(&SystemTime);

	//��ȡĿ¼
	TCHAR szFilePath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(CountArray(szFilePath),szFilePath);

	//��������
	TCHAR szManualName[MAX_PATH]=TEXT("");
	_sntprintf(szManualName,CountArray(szManualName),TEXT("%s_%s_%02d_%02d.CML"),(m_szUserAccounts[0]),
		(m_szUserAccounts[1]),SystemTime.wMonth,SystemTime.wDay);

	//ѡ���ļ�
	LPCTSTR pszFilter=TEXT("�й��������� ��*.CML��|*.CML||");
	CFileDialog DlgSaveManual(FALSE,TEXT("CML"),szManualName,OFN_NOCHANGEDIR|OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,pszFilter);
	DlgSaveManual.m_pOFN->lpstrInitialDir=szFilePath;

	//ѡ���ļ�
	if (DlgSaveManual.DoModal()==IDOK)
	{
		//�����ļ�
		CChessManualFile ChessManualFile;
		ChessManualFile.SetBankerUser(m_wBankerUser);
		ChessManualFile.SetUserAccounts(0,m_szUserAccounts[0]);
		ChessManualFile.SetUserAccounts(1,m_szUserAccounts[1]);

		//��������
		if (ChessManualFile.SaveChessManual(DlgSaveManual.GetPathName(),m_GameLogic.m_ChessManualArray)==false)
		{
			//ASSERT(m_pIStringMessage!=NULL);
			/*if (m_pIStringMessage!=NULL) */InsertSystemString(TEXT("�й����塰���ס�����ʧ��"));
		}
	}

	return 0;
}

//�������
LRESULT CGameClientDlg::OnHitChessBorad(WPARAM wParam, LPARAM lParam)
{
	if (m_wCurrentUser==GetMeChairID())
	{
		//��������
		BYTE cbXTargetPos=(BYTE)wParam;
		BYTE cbYTargetPos=(BYTE)lParam;
		tagChessItem * pChessItem=m_GameLogic.GetChessItem(cbXTargetPos,cbYTargetPos);

		//ȡ���ж�
		if ((m_cbXSourcePos==cbXTargetPos)&&(m_cbYSourcePos==cbYTargetPos))
		{
			//��������
			m_cbXSourcePos=INVALID_POS;
			m_cbYSourcePos=INVALID_POS;
			m_GameClientView.SelectChessItem(INVALID_POS,INVALID_POS);

			return 0;
		}

		//ѡ���ж�
		if ((pChessItem!=NULL)&&(pChessItem->cbColor==m_cbChessColor))
		{
			//��������
			m_cbXSourcePos=cbXTargetPos;
			m_cbYSourcePos=cbYTargetPos;
			m_GameClientView.SelectChessItem(m_cbXSourcePos,m_cbYSourcePos);

			//��������
			PlayGameSound(AfxGetInstanceHandle(),TEXT("SELECT_CHESS"));

			return 0;
		}

		//�����ж�
		bool bWalkLegality=false;
		if ((m_cbXSourcePos!=INVALID_POS)&&(m_cbYSourcePos!=INVALID_POS))
		{
			//�����ж�
			tagChessItem * pSourceChessItem=m_GameLogic.GetChessItem(m_cbXSourcePos,m_cbYSourcePos);
			bWalkLegality=m_GameLogic.IsWalkLegality(pSourceChessItem,cbXTargetPos,cbYTargetPos);

			if(bWalkLegality)
			{
				//�ж�����
				if(m_GameLogic.IsLoseChess(pSourceChessItem->cbColor)==false)
				{
					//��ɱ�岽
					if(m_GameLogic.IsKillGeneral(m_cbXSourcePos,m_cbYSourcePos,cbXTargetPos,cbYTargetPos))
					{
						//��������
						CDlgHint * pDlgUserHint=&m_GameClientView.m_UserHint;
						if (pDlgUserHint->m_hWnd==NULL) pDlgUserHint->Create(IDD_USER_HINT,&m_GameClientView);

						//��ʾ����
						pDlgUserHint->InitTimerCount();
						pDlgUserHint->ShowWindow(SW_SHOW);
						pDlgUserHint->SetForegroundWindow();

						bWalkLegality = false;
					}
				}
			}
		}

		//�����ж�
		if (bWalkLegality==true)
		{
			//���ñ���
			m_wCurrentUser=INVALID_CHAIR;

			//��������
			WORD wActionUser=GetMeChairID();
			WORD wStepTimeCount=__min(m_GameRuleInfo.wRuleStepTime,m_UserStatusInfo[wActionUser].wDrawTimeCount);

			//����ʱ��
			m_UserStatusInfo[wActionUser].wStepTimeCount=wStepTimeCount;
			m_UserStatusInfo[wActionUser].wSecondTimeCount=m_GameRuleInfo.wRuleSecondTime;
			m_GameClientView.SetUserStatusInfo(SwitchViewChairID(wActionUser),m_UserStatusInfo[wActionUser]);

			//�ƶ�����
			m_GameClientView.m_ChessBorad.SetPositively(false);
			PerformMoveChess(m_cbXSourcePos,m_cbYSourcePos,cbXTargetPos,cbYTargetPos);

			//��������
			WORD wStepCount=m_GameLogic.GetStepCount((GetMeChairID()==m_wBankerUser)?CHESS_BLACK:CHESS_WHITE);
			if (wStepCount==1) m_GameClientView.m_btRegret.EnableWindow(TRUE);

			//��������
			CMD_C_MoveChess MoveChess;
			ZeroMemory(&MoveChess,sizeof(MoveChess));

			//���ñ���
			MoveChess.cbXTargetPos=cbXTargetPos;
			MoveChess.cbYTargetPos=cbYTargetPos;
			MoveChess.cbXSourcePos=m_cbXSourcePos;
			MoveChess.cbYSourcePos=m_cbYSourcePos;
			MoveChess.wUsedTimeCount=m_GameRuleInfo.wRuleDrawTime-m_UserStatusInfo[GetMeChairID()].wDrawTimeCount;

			//������Ϣ
			SendData(SUB_C_MOVE_CHESS,&MoveChess,sizeof(MoveChess));

			//���ñ���
			m_cbXSourcePos=INVALID_POS;
			m_cbYSourcePos=INVALID_POS;
			m_GameClientView.SelectChessItem(INVALID_POS,INVALID_POS);

			return 0;
		}
		else
		{
			//ȡ��ѡ��
			m_cbXSourcePos=INVALID_POS;
			m_cbYSourcePos=INVALID_POS;
			m_GameClientView.SelectChessItem(m_cbXSourcePos,m_cbYSourcePos);

			return 0;
		}
	}

	return 0;
}

//ͬ�����
LRESULT CGameClientDlg::OnGameRuleAgree(WPARAM wParam, LPARAM lParam)
{
	//��ȡ����
	m_GameClientView.m_DlgRuleOption.GetGameRuleInfo(m_GameRuleInfo);

	//����ͬ��
	if (m_bRequestCustomize==true)
	{
		//���ý���
		m_GameClientView.SetGameRuleInfo(m_GameRuleInfo);

		//������Ϣ
		CMD_C_CustomizeRult CustomizeRult;
		CustomizeRult.GameRuleInfo=m_GameRuleInfo;
		SendUserReady(&CustomizeRult,sizeof(CustomizeRult));
	}
	else SendUserReady(NULL,0);

	return 0;
}

//�ܾ�����
LRESULT CGameClientDlg::OnGameRuleReject(WPARAM wParam, LPARAM lParam)
{
	//�뿪��Ϸ
	AfxGetMainWnd()->PostMessage(WM_CLOSE);

	return 0;
}

//���׶�λ
LRESULT CGameClientDlg::OnOrientationManual(WPARAM wParam, LPARAM lParam)
{
	//��ʾ����
	if (m_wViewStepCount!=wParam)
	{
		//���ñ���
		m_cbXSourcePos=INVALID_POS;
		m_cbYSourcePos=INVALID_POS;

		//����״̬
		WORD wStepCount=(WORD)wParam;
		ZeroMemory(m_ChessItemView,sizeof(m_ChessItemView));
		ZeroMemory(m_ChessBoradView,sizeof(m_ChessBoradView));
		m_GameLogic.GetHistoryStatus(m_ChessItemView,m_ChessBoradView,wStepCount);

		//���ý���
		UpdateManualControl(wStepCount);
		m_GameClientView.SelectChessItem(INVALID_POS,INVALID_POS);
		m_GameClientView.m_ChessBorad.SetChessBorad(m_ChessBoradView);

		//���ÿ��
		if (wStepCount>0)
		{
			tagChessManual * pChessManual=&m_GameLogic.m_ChessManualArray[wStepCount-1];
			m_GameClientView.SetMoveChessTrace(pChessManual->cbXSourceChessPos,pChessManual->cbYSourceChessPos,
				pChessManual->cbXTargetChessPos,pChessManual->cbYTargetChessPos);
		}
		else
		{
			m_GameClientView.SetMoveChessTrace(INVALID_POS,INVALID_POS,INVALID_POS,INVALID_POS);
		}

		//���½���
		m_GameClientView.UpdateGameView(NULL);
	}

	return 0;
}

//ͬ������
LRESULT CGameClientDlg::OnUserRequestRespond(WPARAM wParam, LPARAM lParam)
{
	//ͬ�⴦��
	switch (wParam)
	{
	case REQUEST_PEACE:		//�����Ӧ
		{
			//��������
			CMD_C_PeaceRespond PeaceRespond;
			ZeroMemory(&PeaceRespond,sizeof(PeaceRespond));

			//���ñ���
			PeaceRespond.cbApprove=(BYTE)lParam;

			//������Ϣ
			SendData(SUB_C_PEACE_RESPOND,&PeaceRespond,sizeof(PeaceRespond));

			break;
		}
	case REQUEST_REGRET:	//�����Ӧ
		{
			//��������
			CMD_C_RegretRespond RegretRespond;
			ZeroMemory(&RegretRespond,sizeof(RegretRespond));

			//���ñ���
			RegretRespond.cbApprove=(BYTE)lParam;

			//������Ϣ
			SendData(SUB_C_REGRET_RESPOND,&RegretRespond,sizeof(RegretRespond));

			break;
		}
	}

	return 0;
}

//ʱ����Ϣ
VOID CGameClientDlg::OnTimer(UINT nIDEvent)
{
	//��Ϸʱ��
	if ((nIDEvent==IDI_GAME_TIME)&&(m_wCurrentUser!=INVALID_CHAIR))
	{
		//��������
		bool bWarnning=false;
		bool bDeductTime=false;
		tagStatusInfo * pStatusInfo=&m_UserStatusInfo[m_wCurrentUser];

		//��ʱģʽ
		if (/*(bDeductTime==false)&&*/(m_bDrawTimeMode==true)&&(pStatusInfo->wStepTimeCount>0))
		{
			//����ʱ��
			pStatusInfo->wStepTimeCount--;
			pStatusInfo->wDrawTimeCount--;

			//���ñ�־
			bDeductTime=true;
			bWarnning=(pStatusInfo->wStepTimeCount<=10);

			//ʱ���л�
			if (pStatusInfo->wDrawTimeCount==0)
			{
				m_bDrawTimeMode=false;
				pStatusInfo->wSecondTimeCount=m_GameRuleInfo.wRuleSecondTime;
			}
		}

		//����ģʽ
		if ((bDeductTime==false)&&(m_bDrawTimeMode==false)&&(pStatusInfo->wSecondTimeCount>0))
		{
			//���ñ�־
			bDeductTime=true;
			bWarnning=(pStatusInfo->wSecondTimeCount<=10);

			//����ʱ��
			pStatusInfo->wSecondTimeCount--;
		}

		//���ý���
		WORD wViewChairID=SwitchViewChairID(m_wCurrentUser);
		m_GameClientView.SetUserStatusInfo(wViewChairID,m_UserStatusInfo[m_wCurrentUser]);

		//��������
		if ((bWarnning==true)&&(IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID()))
		{
			PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));
		}

		//���ͳ�ʱ
		if ((IsLookonMode()==false)&&(bDeductTime==false)&&(m_wCurrentUser==GetMeChairID()))
		{
			//���ý���
			m_GameClientView.m_btPeace.EnableWindow(FALSE);
			m_GameClientView.m_btRegret.EnableWindow(FALSE);
			m_GameClientView.m_btGiveUp.EnableWindow(FALSE);
			m_GameClientView.m_ChessBorad.SetPositively(false);

			//��������
			CMD_C_ConcludeRequest ConcludeRequest;
			ZeroMemory(&ConcludeRequest,sizeof(ConcludeRequest));

			//������Ϣ
			ConcludeRequest.cbReason=REASON_OVER_TIME;
			SendData(SUB_C_CONCLUDE_REQUEST,&ConcludeRequest,sizeof(ConcludeRequest));
		}

		return;
	}

	__super::OnTimer(nIDEvent);
}

//////////////////////////////////////////////////////////////////////////
