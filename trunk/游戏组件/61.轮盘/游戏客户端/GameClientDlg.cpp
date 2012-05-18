#include "Stdafx.h"
#include "GameOption.h"
#include "GameClient.h"
#include "GameClientDlg.h"

//////////////////////////////////////////////////////////////////////////
//�궨��

//��Ϸ��ʱ��
#define IDI_OUT_CARD					200								//���ƶ�ʱ��
#define IDI_START_ROLL					300								//��ʼ��ʱ��

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientDlg, CGameFrameDlg)
	ON_WM_TIMER()
	ON_MESSAGE(IDM_START,OnStart)
	ON_MESSAGE(IDM_CHIP_IN,OnChipIn)
	ON_MESSAGE(IDM_WIN_NUMBER,OnWinNumber)
	ON_MESSAGE(IDM_GET_STOP,OnGetStop)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//��ʼ��ע
LRESULT	CGameClientDlg::OnChipIn(WPARAM wParam, LPARAM lParam)
{
	//��ȡ��Ϣ
	CopyMemory(m_wChipsInfo,(void *)wParam,sizeof(m_wChipsInfo));

	//��������
	CMD_C_ChipIn ChipIn;
	CopyMemory(ChipIn.ChipsInfo,m_wChipsInfo,sizeof(m_wChipsInfo));

	//��������
	SendData(SUB_C_CHIP_IN,&ChipIn,sizeof(CMD_C_ChipIn));
	return 0;
}

//ʤ������
LRESULT	CGameClientDlg::OnWinNumber(WPARAM wParam, LPARAM lParam)
{
	//��ȡ��Ϣ
	m_bWinNumber=(BYTE)wParam;

	//��������
	CMD_C_RollEnd RollEnd;
	RollEnd.WinNumber=m_bWinNumber;

	//��������
	SendData(SUB_C_ROLL_END,&RollEnd,sizeof(CMD_C_RollEnd));
	return 0;
}

//�õ�ֹͣ��Ϣ
LRESULT	CGameClientDlg::OnGetStop(WPARAM wParam, LPARAM lParam)
{
	//��������
	//SendData(SUB_C_GET_STOP);
	return 0;
}

//���캯��
CGameClientDlg::CGameClientDlg() : CGameFrameDlg(&m_GameClientView)
{
	//��Ϸ����
	m_bWinNumber=255;
	ZeroMemory(m_wChipsInfo,sizeof(m_wChipsInfo));
	//memset(m_bHistoryInfo,0xff,sizeof(m_bHistoryInfo));

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
	SetWindowText(TEXT("������Ϸ  --  Ver��6.0.1.0"));

	//����ͼ��
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);
	INT nCom = m_btControl.GetDlgCtrlID();
	PostMessage(WM_COMMAND,nCom,0);
	return true;
}

//���ÿ��
void CGameClientDlg::ResetGameFrame()
{

	return;
}

//��Ϸ����
void CGameClientDlg::OnGameOptionSet()
{
	//��������
	CGameOption GameOption;
	GameOption.m_bEnableSound=IsEnableSound();

	//��������
	if (GameOption.DoModal()==IDOK)
	{
		//���ÿؼ�
		EnableSound(GameOption.m_bEnableSound);
	}

	return;
}

//ʱ����Ϣ
bool CGameClientDlg::OnTimerMessage(WORD wChairID, UINT nElapse, UINT nTimerID)
{
	switch(nTimerID)
	{
	case IDI_START_ROLL:
		{
			m_GameClientView.m_DxDialog.m_bTimeLeave=nElapse;

			//��ʱ�ж�
			if (nElapse<=0)
			{
				KillGameTimer(IDI_START_ROLL);
				return true;
			}
			return true;
		}
	}
	return false;
}

//�Թ�״̬
void CGameClientDlg::OnLookonChanged(bool bLookonUser, const void * pBuffer, WORD wDataSize)
{
	//�Թ�����

	return;
}

//������Ϣ
bool CGameClientDlg::OnGameMessage(WORD wSubCmdID, const void * pBuffer, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_GAME_START:		//�û��з�
		{
			return OnSubGameStart(pBuffer,wDataSize);
		}
	case SUB_S_USER_CHIP:
		{
			return OnSubUserChip(pBuffer,wDataSize);
		}
	case SUB_S_STOP_ROLL:
		{
			return OnSubStopRoll(pBuffer,wDataSize);
		}
	case SUB_S_GAME_END:
		{
			return OnSubGameEnd(pBuffer,wDataSize);
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

			//��Ԫ��Ϣ
			m_lCellScore = pStatusFree->lCellScore;

			//const tagUserData * pUserData=GetUserData(GetMeChairID());
			//long lMaxScore=(long)__max(pUserData->lScore,0);
			//lMaxScore=(long)__min(lMaxScore,m_lCellScore*700);
			long lMaxWinScore=m_lCellScore*720;
			m_GameClientView.SetCellScore(m_lCellScore);
			m_GameClientView.SetUserCome(pStatusFree->bHistoryInfo,m_lCellScore,lMaxWinScore,pStatusFree->lUserScore,bLookonOther);

			//���ÿؼ�
			if (IsLookonMode()==false)
			{
				//��ע����
				SetGameTimer(GetMeChairID(),IDI_START_ROLL,pStatusFree->cbTimeLeave);
			}

			return true;
		}
	}

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

	//��������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));

	m_GameClientView.SetGameStart(pGameStart->bWinNumber);

	//����ʱ��

	return true;
}

//�û���ע
bool CGameClientDlg::OnSubUserChip(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_UserChipIn));
	if (wDataSize!=sizeof(CMD_S_UserChipIn)) return false;

	//��Ϣ����
	CMD_S_UserChipIn * pUserChip=(CMD_S_UserChipIn *)pBuffer;

	m_GameClientView.SetUserChip(pUserChip->lAllChips);

	return true;
}
//ֹͣ��ת
bool CGameClientDlg::OnSubStopRoll(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_StopRoll));
	if (wDataSize!=sizeof(CMD_S_StopRoll)) return false;

	//��Ϣ����
	CMD_S_StopRoll * pStopRoll=(CMD_S_StopRoll *)pBuffer;

	m_GameClientView.SetRollStop(pStopRoll->bWinNumber);

	SendData(SUB_C_GET_STOP);
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

	//����״̬
	m_GameClientView.SetGameEnd(pGameEnd->lGameScore[GetMeChairID()]);
	m_GameClientView.SetUserCome(pGameEnd->bHistoryInfo,m_lCellScore,pGameEnd->lMaxBetWinScore[GetMeChairID()],pGameEnd->lMaxUserScore[GetMeChairID()],IsLookonMode());

	SetGameTimer(GetMeChairID(),IDI_START_ROLL,30);
	return true;
}


//��ʱ����Ϣ
void CGameClientDlg::OnTimer(UINT nIDEvent)
{
	__super::OnTimer(nIDEvent);
}

//��ʼ��ť
LRESULT CGameClientDlg::OnStart(WPARAM wParam, LPARAM lParam)
{

	//������Ϣ
	SendUserReady(NULL,0);

	return 0;
}


//////////////////////////////////////////////////////////////////////////

