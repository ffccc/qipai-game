#include "Stdafx.h"
#include "GameClient.h"
#include "GameOption.h"
#include "GameClientDlg.h"

//////////////////////////////////////////////////////////////////////////
//�궨��

#define IDI_GAME_TIMER				100									//��Ϸ��ʱ��
#define IDI_START_GAME				101									//��ʼ��ʱ��

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientDlg, CGameFrameDlg)
	ON_WM_TIMER()
	ON_MESSAGE(IDM_START,OnStart)
	ON_MESSAGE(IDM_COUNT,OnCount)
	ON_MESSAGE(IDM_PASS,OnPass)
	ON_MESSAGE(IDM_PEACE,OnPeace)
	ON_MESSAGE(IDM_STUDY,OnStudy)
	ON_MESSAGE(IDM_REGRET,OnRegret)
	ON_MESSAGE(IDM_GIVEUP,OnGiveUp)
	ON_MESSAGE(IDM_PRESERVE,OnPreserve)
	ON_MESSAGE(IDM_PEACE_ANSWER,OnPeaceAnswer)
	ON_MESSAGE(IDM_REGRET_ANSWER,OnRegretAnswer)
	ON_MESSAGE(WM_HIT_CHESS_BORAD, OnHitChessBorad)
	ON_MESSAGE(IDM_AGREE_GAME_RULE, OnAgreeGameRule)
	ON_MESSAGE(IDM_CHESS_COUNT_RESPONSION, OnChessCountResponsion)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientDlg::CGameClientDlg() : CGameFrameDlg(&m_GameClientView)
{
	//�������
	memset(&m_GameRuleInfo,0,sizeof(m_GameRuleInfo));

	//���ñ���
	m_bDoubleMouse=false;
	m_bShowChessID=false;
	m_bAutoSaveManual=false;

	//��Ϸ����
	m_ShamView=false;
	m_bAgreeRule=false;
	m_bCustomize=false;
	m_bGameStart=false;
	m_bLimitTime=false;
	m_bReqChessCount=false;
	m_bResponsionChessCount=false;

	//��Ϸ����
	m_wBlackUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	memset(m_StatusInfo,0,sizeof(m_StatusInfo));

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
	SetWindowText(TEXT("Χ����Ϸ  --  Ver��6.0.1.0"));

	//��ȡ����
	m_bDoubleMouse=AfxGetApp()->GetProfileInt(TEXT("GameOption"),TEXT("DoubleMouse"),FALSE)?true:false;
	m_bShowChessID=AfxGetApp()->GetProfileInt(TEXT("GameOption"),TEXT("ShowChessID"),FALSE)?true:false;
	m_bAutoSaveManual=AfxGetApp()->GetProfileInt(TEXT("GameOption"),TEXT("AutoSaveManual"),FALSE)?true:false;

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
	memset(&m_GameRuleInfo,0,sizeof(m_GameRuleInfo));

	//��Ϸ����
	m_ShamView=false;
	m_bAgreeRule=false;
	m_bCustomize=false;
	m_bGameStart=false;
	m_bLimitTime=false;
	m_bReqChessCount=false;
	m_bResponsionChessCount=false;

	//��Ϸ����
	m_wBlackUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	memset(m_StatusInfo,0,sizeof(m_StatusInfo));

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
	GameOption.m_bAutoSaveManual=m_bAutoSaveManual;

	//��������
	if (GameOption.DoModal()==IDOK)
	{
		//���ñ���
		m_bDoubleMouse=GameOption.m_bDoubleMouse;
		m_bShowChessID=GameOption.m_bShowChessID;
		m_bAutoSaveManual=GameOption.m_bAutoSaveManual;

		//��������
		AfxGetApp()->WriteProfileInt(TEXT("GameOption"),TEXT("DoubleMouse"),m_bDoubleMouse);
		AfxGetApp()->WriteProfileInt(TEXT("GameOption"),TEXT("ShowChessID"),m_bShowChessID);
		AfxGetApp()->WriteProfileInt(TEXT("GameOption"),TEXT("AutoSaveManual"),m_bAutoSaveManual);

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
	case SUB_S_PASS_CHESS:		//��������
		{
			return OnSubPassChess(pBuffer,wDataSize);
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
	case SUB_S_REQ_CHESS_COUNT:	//�����Ŀ
		{
			return OnSubReqChessCount(pBuffer,wDataSize);
		}
	case SUB_S_UNAGREE_CHESS_COUNT:	//��ĿӦ��
		{
			return OnSubChessCountResponsion(pBuffer,wDataSize);
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

			//���ñ���
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
			else InsertSystemString(TEXT("���ȴ����ڷ������������Ϸ����"));

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
			m_bGameStart=true;
			m_wBlackUser=pStatusPlay->wBlackUser;
			m_wCurrentUser=pStatusPlay->wCurrentUser;
			m_GameRuleInfo=pStatusPlay->GameRuleInfo;
			m_bLimitTime=((m_GameRuleInfo.wRuleStepTime>0)||(m_GameRuleInfo.wRuleLimitTime>0));

			//���ý���
			m_GameClientView.m_btPreserve.EnableWindow(TRUE);
			m_GameClientView.SetBlackUser(SwitchViewChairID(m_wBlackUser));

			//���̽���
			m_GameClientView.m_ChessBorad.SetChessBorad(m_GameLogic.m_cbChessBorad,m_GameLogic.m_wIdentifier);

			//���ÿ��
			const tagChessManual * pChessManual=m_GameLogic.GetLastChessManual();
			if (pChessManual!=NULL) m_GameClientView.m_ChessBorad.SetChessFocus(pChessManual->cbXPos,pChessManual->cbYPos);

			//�û���Ϣ
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				BYTE cbColor=(i==m_wBlackUser)?BLACK_CHESS:WHITE_CHESS;
				m_StatusInfo[i].wUseTimeCount=pStatusPlay->wUseClock[i];
				m_StatusInfo[i].wLimitTimeCount=pStatusPlay->wLeftClock[i];
				m_StatusInfo[i].wStepCount=m_GameLogic.GetStepCount(cbColor);
				m_StatusInfo[i].wTakeChessCount=m_GameLogic.GetTakeChessCount(cbColor);
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

				//��Ŀ�ж�
				WORD wBlackStepCount=m_GameLogic.GetStepCount(BLACK_CHESS);
				if (wBlackStepCount==1) m_GameClientView.m_btCount.EnableWindow(TRUE);
				m_bReqChessCount=((pStatusPlay->cbBegStatus[wMeChairID]&GU_WAIT_CHESS_COUNT)!=0);
				m_bResponsionChessCount=((pStatusPlay->cbBegStatus[(wMeChairID+1)%GAME_PLAYER]&GU_WAIT_CHESS_COUNT)!=0);

				//��������
				WORD wEnemyUser=(wMeChairID+1)%GAME_PLAYER;
				bool bPeaceReq=((pStatusPlay->cbBegStatus[wEnemyUser]&GU_WAIT_PEACE)!=0);
				bool bRegretReq=((pStatusPlay->cbBegStatus[wEnemyUser]&GU_WAIT_REGRET)!=0);

				//��������
				if (bPeaceReq==true) m_GameClientView.ShowPeaceRequest();
				if (bRegretReq==true) m_GameClientView.ShowRegretRequest();

				//��Ŀ����
				if (m_bResponsionChessCount==true)
				{
					//��ȡ��Ŀ
					tagCountResult CountResult;
					memset(&CountResult,0,sizeof(CountResult));
					m_GameLogic.GetChessCountInfo(CountResult);

					//���ý���
					float fBlackCount=(float)CountResult.wChessCount[0]-((float)CountResult.wCommomCount)/(float)2.0;
					float fWhiteCount=(float)CountResult.wChessCount[1]-((float)CountResult.wCommomCount)/(float)2.0;
					m_GameClientView.m_ChessBorad.SetChessCountInfo(CountResult.cbCountInfo);
					m_GameClientView.m_ChessCountInfo.SetChessCountInfo(fBlackCount,fWhiteCount,CountResult.wCommomCount);

					//��ʾ����
					m_GameClientView.m_ChessCountInfo.Create(IDD_CHESS_COUNT,&m_GameClientView);
					m_GameClientView.m_ChessCountInfo.ShowWindow(SW_SHOW);
				}

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
	m_wBlackUser=pGameStart->wBlackUser;
	m_wCurrentUser=pGameStart->wBlackUser;
	m_GameRuleInfo=pGameStart->GameRuleInfo;

	//��������
	m_ShamView=false;
	m_bGameStart=true;
	m_bCustomize=false;
	m_bAgreeRule=false;
	memset(m_StatusInfo,0,sizeof(m_StatusInfo));
	m_bLimitTime=((m_GameRuleInfo.wRuleStepTime>0)||(m_GameRuleInfo.wRuleLimitTime>0));

	//�����߼�
	m_GameLogic.InitGameLogic();

	//������Ϣ
	WORD wLimitTimeCount=m_GameRuleInfo.wRuleLimitTime;
	if (wLimitTimeCount==0) wLimitTimeCount=m_GameRuleInfo.wRuleStepTime;
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_StatusInfo[i].wLimitTimeCount=wLimitTimeCount;
		m_GameClientView.SetUserStatusInfo(SwitchViewChairID(i),m_StatusInfo[i]);
	}

	//���ý���
	m_GameClientView.m_btPreserve.EnableWindow(TRUE);
	m_GameClientView.m_GameScoreWnd.ShowWindow(SW_HIDE);
	m_GameClientView.SetBlackUser(SwitchViewChairID(m_wBlackUser));
	if (IsLookonMode()==true) m_GameClientView.m_ChessBorad.CleanChess();

	//ȫ�����
	if (IsLookonMode()==false)
	{
		m_GameClientView.m_btPeace.EnableWindow(TRUE);
		m_GameClientView.m_btGiveUp.EnableWindow(TRUE);
		m_GameClientView.m_btStart.EnableWindow(FALSE);
	}

	//�������
	if ((IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID()))
	{
		ActiveGameFrame();
		m_GameClientView.m_btPass.EnableWindow(TRUE);
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
	m_wCurrentUser=pPlaceChess->wCurrentUser;

	//���ý���
	m_ShamView=false;
	WORD wMeChairID=GetMeChairID();
	if ((IsLookonMode()==true)||(pPlaceChess->wPlaceUser!=wMeChairID))
	{
		//��������
		WORD wPlaceUser=pPlaceChess->wPlaceUser;
		BYTE cbColor=(wPlaceUser==m_wBlackUser)?BLACK_CHESS:WHITE_CHESS;

		//��������
		WORD wDeadChess[361],wDeadChessCount=0;
		WORD wIdentifier=m_GameLogic.PlaceChess(pPlaceChess->cbXPos,pPlaceChess->cbYPos,cbColor,wDeadChess,wDeadChessCount);

		//��Ŀ�ж�
		WORD wBlackStepCount=m_GameLogic.GetStepCount(BLACK_CHESS);
		if (wBlackStepCount==1) m_GameClientView.m_btCount.EnableWindow(TRUE);

		//��������
		m_GameClientView.m_ChessBorad.SetChess(pPlaceChess->cbXPos,pPlaceChess->cbYPos,cbColor,wIdentifier);
		m_GameClientView.m_ChessBorad.SetChessFocus(pPlaceChess->cbXPos,pPlaceChess->cbYPos);
		if (wDeadChessCount>0) m_GameClientView.m_ChessBorad.CleanChess(wDeadChess,wDeadChessCount);

		//��������
		if (wDeadChessCount>0)
		{
			if (wMeChairID==wPlaceUser) PlayGameSound(AfxGetInstanceHandle(),TEXT("CHESS_EAT"));
			else PlayGameSound(AfxGetInstanceHandle(),TEXT("CHESS_DEAD"));
		}
		else PlayGameSound(AfxGetInstanceHandle(),TEXT("PLACE_CHESS"));
	}

	//�û���Ϣ
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		BYTE cbColor=(i==m_wBlackUser)?BLACK_CHESS:WHITE_CHESS;
		m_StatusInfo[i].wUseTimeCount=pPlaceChess->wUseClock[i];
		m_StatusInfo[i].wLimitTimeCount=pPlaceChess->wLeftClock[i];
		m_StatusInfo[i].wStepCount=m_GameLogic.GetStepCount(cbColor);
		m_StatusInfo[i].wTakeChessCount=m_GameLogic.GetTakeChessCount(cbColor);
		m_GameClientView.SetUserStatusInfo(SwitchViewChairID(i),m_StatusInfo[i]);
	}

	//�������
	if ((IsLookonMode()==false)&&(pPlaceChess->wCurrentUser==wMeChairID))
	{
		ActiveGameFrame();
		m_GameClientView.m_btPass.EnableWindow(TRUE);
		m_GameClientView.m_ChessBorad.SetSelectMode(SM_NULL_CHESS);
	}

	return true;
}

//��������
bool CGameClientDlg::OnSubPassChess(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_PassChess));
	if (wDataSize!=sizeof(CMD_S_PassChess)) return false;

	//��Ϣ����
	CMD_S_PassChess * pPassChess=(CMD_S_PassChess *)pBuffer;
	m_wCurrentUser=pPassChess->wCurrentUser;

	//��������
	WORD wMeChairID=GetMeChairID();
	if ((IsLookonMode()==true)||(pPassChess->wPassUser!=wMeChairID))
	{
		PlayGameSound(AfxGetInstanceHandle(),TEXT("PASS_CHESS"));
	}

	//�������
	if ((IsLookonMode()==false)&&(pPassChess->wCurrentUser==wMeChairID))
	{
		ActiveGameFrame();
		m_GameClientView.m_btPass.EnableWindow(TRUE);
		m_GameClientView.m_ChessBorad.SetSelectMode(SM_NULL_CHESS);
	}

	//�û���Ϣ
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_StatusInfo[i].wUseTimeCount=pPassChess->wUseClock[i];
		m_StatusInfo[i].wLimitTimeCount=pPassChess->wLeftClock[i];
		m_GameClientView.SetUserStatusInfo(SwitchViewChairID(i),m_StatusInfo[i]);
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

	//�Լҷ���
	if (pRegretFaile->cbFaileReason==FR_PLAYER_OPPOSE)
	{
		WORD wMeChairID=GetMeChairID();
		InsertSystemString(TEXT("�ԼҲ�ͬ���� [ ���� ]"));
		m_GameClientView.m_btRegret.EnableWindow((m_StatusInfo[wMeChairID].wStepCount>0)?TRUE:FALSE);

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
		pRegretResult->wRegretCount++;
	}

	//��������
	m_GameLogic.RegretChess(pRegretResult->wRegretCount);
	m_GameClientView.m_ChessBorad.SetChessBorad(m_GameLogic.m_cbChessBorad,m_GameLogic.m_wIdentifier);

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
		m_GameClientView.m_btPass.EnableWindow(TRUE);
		m_GameClientView.m_ChessBorad.SetSelectMode(SM_NULL_CHESS);
	}

	//�û���Ϣ
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		BYTE cbColor=(i==m_wBlackUser)?BLACK_CHESS:WHITE_CHESS;
		m_StatusInfo[i].wUseTimeCount=pRegretResult->wUseClock[i];
		m_StatusInfo[i].wLimitTimeCount=pRegretResult->wLeftClock[i];
		m_StatusInfo[i].wStepCount=m_GameLogic.GetStepCount(cbColor);
		m_StatusInfo[i].wTakeChessCount=m_GameLogic.GetTakeChessCount(cbColor);
		m_GameClientView.SetUserStatusInfo(SwitchViewChairID(i),m_StatusInfo[i]);
	}

	//��Ŀ�ж�
	WORD wBlackStepCount=m_GameLogic.GetStepCount(BLACK_CHESS);
	if (wBlackStepCount==0)
	{
		m_GameClientView.m_btCount.EnableWindow(FALSE);
		if (m_GameClientView.m_ChessCountInfo.GetSafeHwnd()!=NULL) m_GameClientView.m_ChessCountInfo.DestroyWindow();
	}

	//�������
	if (IsLookonMode()==false)
	{
		WORD wStepCount=m_GameLogic.GetStepCount((GetMeChairID()==m_wBlackUser)?BLACK_CHESS:WHITE_CHESS);
		if ((wStepCount==0)&&(m_GameRuleInfo.cbRegretFlag==TRUE)) m_GameClientView.m_btRegret.EnableWindow(FALSE);
		if ((wStepCount>0)&&(m_GameRuleInfo.cbRegretFlag==TRUE)&&(pRegretResult->wRegretUser==wMeChairID)) m_GameClientView.m_btRegret.EnableWindow(TRUE);
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
	m_bGameStart=false;
	m_wCurrentUser=INVALID_CHAIR;

	//���ý���
	m_GameClientView.m_btPass.EnableWindow(FALSE);
	m_GameClientView.m_btPeace.EnableWindow(FALSE);
	m_GameClientView.m_btRegret.EnableWindow(FALSE);
	m_GameClientView.m_btGiveUp.EnableWindow(FALSE);
	m_GameClientView.m_ChessBorad.CleanCountInfo();
	m_GameClientView.m_ChessBorad.SetSelectMode(SM_NO_SELECT);
	if (m_GameClientView.m_ChessCountInfo.GetSafeHwnd()!=NULL) m_GameClientView.m_ChessCountInfo.DestroyWindow();

	//��������
	if (pGameEnd->wWinUser!=INVALID_CHAIR)
	{
		if (pGameEnd->lUserScore[GetMeChairID()]>=0) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WIN"));
		else PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_LOST"));
	}
	else PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WIN"));

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
	WORD wDeadChess[361],wDeadChessCount=0;
	WORD wManualCount=wDataSize/sizeof(tagChessManual);
	for (WORD wIndex=0;wIndex<wManualCount;wIndex++) 
	{
		tagChessManual * pChessManual=(((tagChessManual *)pBuffer)+wIndex);
		m_GameLogic.PlaceChess(pChessManual->cbXPos,pChessManual->cbYPos,pChessManual->cbColor,wDeadChess,wDeadChessCount);
	}

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

//�����Ŀ
bool CGameClientDlg::OnSubReqChessCount(const void * pBuffer, WORD wDataSize)
{
	//���ñ���
	m_bResponsionChessCount=true;

	//��ȡ��Ŀ
	tagCountResult CountResult;
	memset(&CountResult,0,sizeof(CountResult));
	m_GameLogic.GetChessCountInfo(CountResult);

	//������
	float fBlackCount=(float)CountResult.wChessCount[0]-((float)CountResult.wCommomCount)/(float)2.0;
	float fWhiteCount=(float)CountResult.wChessCount[1]-((float)CountResult.wCommomCount)/(float)2.0;
	m_GameClientView.m_ChessBorad.SetChessCountInfo(CountResult.cbCountInfo);
	m_GameClientView.m_ChessCountInfo.SetChessCountInfo(fBlackCount,fWhiteCount,CountResult.wCommomCount);

	//��ʾ����
	if (m_GameClientView.m_ChessCountInfo.GetSafeHwnd()==NULL)
	{
		m_GameClientView.m_ChessCountInfo.Create(IDD_CHESS_COUNT,&m_GameClientView);
	}
	m_GameClientView.m_ChessCountInfo.ShowWindow(SW_SHOW);

	//�������
	ActiveGameFrame();
	InsertSystemString(TEXT("�Է�������󡰵�Ŀ��"));

	return true;
}

//��ĿӦ��
bool CGameClientDlg::OnSubChessCountResponsion(const void * pBuffer, WORD wDataSize)
{
	//���ñ���
	m_bReqChessCount=false;

	//�������
	InsertSystemString(TEXT("�Է���Ҳ�ͬ����ġ���Ŀ������"));

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
						m_GameClientView.m_ChessBorad.CleanCountInfo();
						m_GameClientView.m_ChessBorad.SetSelectMode(SM_NO_SELECT);
						if (m_GameClientView.m_ChessCountInfo.GetSafeHwnd()!=NULL) m_GameClientView.m_ChessCountInfo.DestroyWindow();

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

	//�����߼�
	m_GameLogic.InitGameLogic();

	//���ý���
	m_GameClientView.m_ChessBorad.CleanChess();
	m_GameClientView.m_btStart.EnableWindow(FALSE);
	m_GameClientView.m_btCount.EnableWindow(FALSE);
	m_GameClientView.m_btPreserve.EnableWindow(FALSE);
	m_GameClientView.m_GameScoreWnd.ShowWindow(SW_HIDE);
	m_GameClientView.SetUserStatusInfo(0,m_StatusInfo[0]);
	m_GameClientView.SetUserStatusInfo(1,m_StatusInfo[1]);

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
	else InsertSystemString(TEXT("���ȴ����ڷ������������Ϸ����"));

	//�ر�ʱ��
	KillGameTimer(IDI_START_GAME) ;

	return 0;
}

//��Ŀ��ť
LRESULT CGameClientDlg::OnCount(WPARAM wParam, LPARAM lParam)
{
	//��Ŀ�ж�
	if (m_GameClientView.m_ChessBorad.IsChessCount()==false)
	{
		//��ȡ��Ŀ
		tagCountResult CountResult;
		memset(&CountResult,0,sizeof(CountResult));
		m_GameLogic.GetChessCountInfo(CountResult);

		//���ý���
		m_GameClientView.m_ChessBorad.SetChessCountInfo(CountResult.cbCountInfo);

		//��Ŀͳ��
		if ((IsLookonMode()==false)&&(m_bGameStart==true))
		{
			//������
			float fBlackCount=(float)CountResult.wChessCount[0]-((float)CountResult.wCommomCount)/(float)2.0;
			float fWhiteCount=(float)CountResult.wChessCount[1]-((float)CountResult.wCommomCount)/(float)2.0;
			m_GameClientView.m_ChessCountInfo.SetChessCountInfo(fBlackCount,fWhiteCount,CountResult.wCommomCount);

			//��ʾ����
			if (m_GameClientView.m_ChessCountInfo.GetSafeHwnd()==NULL)
			{
				m_GameClientView.m_ChessCountInfo.Create(IDD_CHESS_COUNT,&m_GameClientView);
			}
			m_GameClientView.m_ChessCountInfo.ShowWindow(SW_SHOW);
		}
	}
	else m_GameClientView.m_ChessBorad.CleanCountInfo();

	return 0;
}

//ͨ����ť
LRESULT CGameClientDlg::OnPass(WPARAM wParam, LPARAM lParam)
{
	//������Ϣ
	SendData(SUB_C_PASS_CHESS);

	//���ñ���
	m_wCurrentUser=INVALID_CHAIR;

	//��������
	m_GameClientView.m_btPass.EnableWindow(FALSE);
	m_GameClientView.m_ChessBorad.SetSelectMode(SM_NO_SELECT);

	//��������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("PASS_CHESS"));

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
	m_GameClientView.m_ChessBorad.CleanCountInfo();
	m_GameClientView.m_ChessBorad.SetSelectMode(SM_NO_SELECT);
	if (m_GameClientView.m_ChessCountInfo.GetSafeHwnd()!=NULL) m_GameClientView.m_ChessCountInfo.DestroyWindow();

	return 0;
}

//���水ť
LRESULT CGameClientDlg::OnPreserve(WPARAM wParam, LPARAM lParam)
{
	//��ȡĿ¼
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(sizeof(szPath),szPath);

	//ѡ���ļ�
	LPCTSTR pszFilter=TEXT("Χ�������ļ� ��*.WQM��|*.WQM||");
	CFileDialog DlgSaveManual(FALSE,TEXT("WQM"),NULL,OFN_NOCHANGEDIR|OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,pszFilter);
	DlgSaveManual.m_pOFN->lpstrInitialDir=szPath;
	if (DlgSaveManual.DoModal()!=IDOK) return 0;

	//��������
	if (SaveChessManual(DlgSaveManual.GetPathName())==false)	
	{
		InsertSystemString(TEXT("Χ����Ϸ�����׼�¼������ʧ��"));
	}
	else InsertSystemString(TEXT("Χ����Ϸ�����׼�¼������ɹ�"));

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
		WORD wDeadChess[361],wDeadChessCount=0;
		WORD wIdentifier=m_GameLogic.PlaceChess(cbXPos,cbYPos,cbColor,wDeadChess,wDeadChessCount);
		if ((wIdentifier==0)||(wIdentifier==0xFFFF))
		{
			InsertSystemString(TEXT("�������Ѩ���߽��ŵ㣬�����ڴ˵����"));
			PlayGameSound(AfxGetInstanceHandle(),TEXT("INVALID_PLACE"));
			return 0;
		}

		//���ñ���
		m_ShamView=true;
		m_wCurrentUser=INVALID_CHAIR;

		//�û���Ϣ
		m_StatusInfo[wMeChairID].wStepCount=m_GameLogic.GetStepCount(cbColor);
		m_StatusInfo[wMeChairID].wTakeChessCount=m_GameLogic.GetTakeChessCount(cbColor);
		m_GameClientView.SetUserStatusInfo(SwitchViewChairID(wMeChairID),m_StatusInfo[wMeChairID]);

		//��������
		m_GameClientView.m_btPass.EnableWindow(FALSE);
		m_GameClientView.m_ChessBorad.SetSelectMode(SM_NO_SELECT);
		m_GameClientView.m_ChessBorad.SetChess((BYTE)wParam,(BYTE)lParam,cbColor,wIdentifier);
		m_GameClientView.m_ChessBorad.SetChessFocus((BYTE)wParam,(BYTE)lParam);
		if (wDeadChessCount>0) m_GameClientView.m_ChessBorad.CleanChess(wDeadChess,wDeadChessCount);

		//�����ж�
		WORD wStepCount=m_GameLogic.GetStepCount(cbColor);
		if ((wStepCount==1)&&(m_GameRuleInfo.cbRegretFlag==TRUE)) m_GameClientView.m_btRegret.EnableWindow(TRUE);

		//��Ŀ�ж�
		WORD wBlackStepCount=m_GameLogic.GetStepCount(BLACK_CHESS);
		if (wBlackStepCount==1) m_GameClientView.m_btCount.EnableWindow(TRUE);

		//��������
		if (wDeadChessCount==0)	PlayGameSound(AfxGetInstanceHandle(),TEXT("PLACE_CHESS"));
		else PlayGameSound(AfxGetInstanceHandle(),TEXT("CHESS_EAT"));

		//������Ϣ
		CMD_C_PlaceChess PlaceChess;
		PlaceChess.cbXPos=(BYTE)wParam;
		PlaceChess.cbYPos=(BYTE)lParam;
		SendData(SUB_C_PLACE_CHESS,&PlaceChess,sizeof(PlaceChess));
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

//��ĿӦ��
LRESULT CGameClientDlg::OnChessCountResponsion(WPARAM wParam, LPARAM lParam)
{
	//���ý���
	m_GameClientView.m_ChessBorad.CleanCountInfo();

	//ͬ���Ŀ
	if ((wParam==TRUE)&&(m_bReqChessCount==false))
	{
		//���ñ���
		m_bReqChessCount=true;

		//������Ϣ
		SendData(SUB_C_REQ_CHESS_COUNT);

		//��ʾ��Ϣ
		InsertSystemString(TEXT("���ڶԷ������Ӧ����Ŀ������"));
	}

	//�ܾ���Ŀ
	if ((wParam==FALSE)&&(m_bResponsionChessCount==true))
	{
		//���ñ���
		m_bResponsionChessCount=false;

		//������Ϣ
		SendData(SUB_C_UNAGREE_CHESS_COUNT);

		//��ʾ��Ϣ
		InsertSystemString(TEXT("��ܾ��˶Է��ġ���Ŀ������"));
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////
