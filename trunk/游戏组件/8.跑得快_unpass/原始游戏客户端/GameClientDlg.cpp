#include "Stdafx.h"
#include "GameClient.h"
#include "GameClientDlg.h"
#include "..\��Ϣ����\CMD_QuickRun.h"
#include ".\gameclientdlg.h"
#include "GameOption.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientDlg, CGameFrameDlg)
	ON_MESSAGE(WM_OUTPISA,OnCmdOutPisa)
	ON_MESSAGE(WM_USERREADY,OnCmdUserReady)
	ON_MESSAGE(IDM_TIMER_OUT,OnCmdTimerOut)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientDlg::CGameClientDlg() : CGameFrameDlg(&m_GameClientView)
{
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
#ifdef _TWICE
	SetWindowText(TEXT("�ܵÿ���Ϸ�������ƣ�  --  Ver��6.0.1.0"));
#else
	SetWindowText(TEXT("�ܵÿ���Ϸ  --  Ver��6.0.1.0"));
#endif

	//����ͼ��
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);

	//��ȡ���� ���ÿؼ�
	m_GameClientView.SetUserOrder(AfxGetApp()->GetProfileInt(TEXT("GameOption"),TEXT("DeasilOrder"),TRUE)?true:false);
	return true;
}

//���ÿ��
void CGameClientDlg::ResetGameFrame()
{
	TRACE("CGameClientDlg::ResetGameFrame");
	return;
}

//��Ϸ����
void CGameClientDlg::OnGameOptionSet()
{
	//��������
	CGameOption GameOption;
	GameOption.m_bEnableSound=IsEnableSound();
	GameOption.m_bDeasilOrder=m_GameClientView.IsDeasilOrder();

	//��������
	if (GameOption.DoModal()==IDOK)
	{
		//���ÿؼ�
		EnableSound(GameOption.m_bEnableSound);
		m_GameClientView.SetUserOrder(GameOption.m_bDeasilOrder);

		//��������
		AfxGetApp()->WriteProfileInt(TEXT("GameOption"),TEXT("DeasilOrder"),GameOption.m_bDeasilOrder?TRUE:FALSE);
	}

	return;
}

//ʱ����Ϣ
bool CGameClientDlg::OnTimerMessage(WORD wChairID, UINT nElapse, UINT nTimerID)
{
	return true;
}

//�Թ�״̬
void CGameClientDlg::OnLookonChanged(bool bLookonUser, const void * pBuffer, WORD wDataSize)
{
	TRACE("CGameClientDlg::OnLookonChanged");
}

//������Ϣ
bool CGameClientDlg::OnGameMessage(WORD wSubCmdID, const void * pBuffer, WORD wDataSize)
{
	TRACE("CGameClientDlg::OnGameMessage:SubCmdID:%d",wSubCmdID);
	switch(wSubCmdID)
	{
		case MSG_SET_INITPISA:
		{
			//��ʼ��
			struct tagSetInitPisa *pInit=(struct tagSetInitPisa *)pBuffer;
			m_GameClientView.m_nOwnerIndex=(pInit->nOwnerIndex+2)%GAME_PLAYER;//�������ϵ����ƫ��
			TRACE("�յ���ʼ����������%d,������������%d,�ҵ�����id��%d\n",pInit->nOwnerIndex,m_GameClientView.m_nOwnerIndex,GetMeChairID());
			//m_GameClientView.m_nFocusIndex=0;
			for(int i=0;i<GAME_PLAYER;i++)
			{
				m_GameClientView.m_nPlayerLeftCount[i]=PISACOUNT;
				m_GameClientView.m_OutPisaArray[i].RemoveAll();
				m_GameClientView.m_bOutPass[i]=FALSE;
			}
			m_GameClientView.m_PisaArray.RemoveAll();
			for(i=0;i<PISACOUNT;i++)
			{
				m_GameClientView.m_PisaArray.Add(pInit->tPisa[i]);
				m_GameClientView.m_bOutSelect[i]=FALSE;
			}
			m_GameClientView.m_ScoreView.ShowWindow(SW_HIDE);
			PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));
			m_GameClientView.m_bStart=TRUE;
		}
			break;
		case MSG_SET_FOCUS:
		{
			//���õ�ǰ˭����
			struct tagSetFocus *pFocus=(struct tagSetFocus *)pBuffer;
			m_GameClientView.m_nFocusIndex=(pFocus->nFoucsId+GAME_PLAYER-m_GameClientView.m_nOwnerIndex)%GAME_PLAYER;
			//������ý��㲻���Լ�����ô��˵�������Լ��ȳ���
			if (m_GameClientView.m_nFocusIndex!=2)
				m_GameClientView.m_bStart=FALSE;
			TRACE("�յ����ý����������%d,������������%d\n",pFocus->nFoucsId,m_GameClientView.m_nFocusIndex);
			if (pFocus->bClean)
			{
				for(int i=0;i<GAME_PLAYER;i++)
				{
					m_GameClientView.m_OutPisaArray[i].RemoveAll();
					m_GameClientView.m_bOutPass[i]=FALSE;
				}
			}
			m_GameClientView.SetTimerUp(TRUE);
		}
			break;
		case MSG_SET_OTHERPISA:
		{
			//����������ҳ���ʲô��
			struct tagSetOtherPisa *pOther=(struct tagSetOtherPisa *)pBuffer;
			int nLocalIndex=(pOther->nChairId+GAME_PLAYER-m_GameClientView.m_nOwnerIndex)%GAME_PLAYER;
			TRACE("�յ�������ҳ��Ƶ�������%d,������������%d\n",pOther->nChairId,nLocalIndex);
			m_GameClientView.m_nPlayerLeftCount[nLocalIndex]-=pOther->nCount;
			m_GameClientView.m_OutPisaArray[nLocalIndex].RemoveAll();
			if (pOther->nCount==0)
			{
				m_GameClientView.m_bOutPass[nLocalIndex]=TRUE;
			}
			else
			{
				m_GameClientView.m_bOutPass[nLocalIndex]=FALSE;
				for(int i=0;i<pOther->nCount;i++)
					m_GameClientView.m_OutPisaArray[nLocalIndex].Add(pOther->tOutPisa[i]);
			}
		}
			break;
		case MSG_OUTPISA_MSG:
		{
			//��ҳ����ƾ�����������鷵�صĽ��
			struct tagOutPisaMsg *pOutPisa=(struct tagOutPisaMsg *)pBuffer;
			if (pOutPisa->nResult==0)
			{
				PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));
				//delete all select out pisa
				for(int i=PISACOUNT-1;i>=0;i--)
				{
					if (m_GameClientView.m_bOutSelect[i])
					{
						m_GameClientView.m_PisaArray.RemoveAt(i);
						m_GameClientView.m_bOutSelect[i]=FALSE;
					}
				}
			}
			else
			{
				m_GameClientView.SetTimerPause(false);
				MessageBox(pOutPisa->chErrMsg,"������������");
			}
		}
			break;
		case MSG_GAME_END:
		{
			struct tagGameEnd *pGameEnd=(struct tagGameEnd *)pBuffer;
			//���û���
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				const tagUserData * pUserData=GetUserData(i);
				m_GameClientView.m_ScoreView.SetGameScore(i,pUserData->szName,pGameEnd->lGameScore[i]);
			}
			m_GameClientView.m_nFocusIndex=-1;
			m_GameClientView.m_bReady=FALSE;
			m_GameClientView.m_ScoreView.SetGameTax(pGameEnd->lGameTax);
			m_GameClientView.m_ScoreView.ShowWindow(SW_SHOW);

			//��������
			WORD wMeChairID=GetMeChairID();
			LONG lMeScore=pGameEnd->lGameScore[GetMeChairID()];
			if (lMeScore>0L) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WIN"));
			else if (lMeScore<0L) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_LOST"));
			else PlayGameSound(GetModuleHandle(NULL),TEXT("GAME_END"));

		}
			break;
	}
	m_GameClientView.UpdateGameView(NULL);
	return true;
	return false;
}

//��Ϸ����
bool CGameClientDlg::OnGameSceneMessage(BYTE cbGameStation, bool bLookonOther, const void * pBuffer, WORD wDataSize)
{
	TRACE("CGameClientDlg::OnGameMessage:GameStation:%d,Lookon:%d",cbGameStation,bLookonOther);
	return true;
	return false;
}

LRESULT CGameClientDlg::OnCmdOutPisa(WPARAM wParam,LPARAM lParam)
{
	struct tagSetOutPisa *pOut=(struct tagSetOutPisa *)wParam;
	TRACE("�ҵ�����id:%d��id:%d\n",this->GetMeChairID(),pOut->nChairId);
	m_GameClientView.SetTimerPause(true);
	SendData(MSG_SET_OUTPISA,pOut,sizeof(struct tagSetOutPisa));
	return 0;
}

LRESULT CGameClientDlg::OnCmdUserReady(WPARAM wParam,LPARAM lParam)
{
	SendUserReady(NULL,0);
	PlayGameSound(AfxGetInstanceHandle(),TEXT("Ready"));
	return 0;
}

LRESULT CGameClientDlg::OnCmdTimerOut(WPARAM wParam,LPARAM lParam)
{
	struct tagSetOutPisa out;
	memset(&out,0,sizeof(tagSetOutPisa));
	if (m_GameClientView.m_bStart)
	{
		//����Ǹտ�ʼ�ĳ��ƣ���Ҫ�Ѻ���3����ȥ
		int index=m_GameClientView.SetHeiTao3Out();
		//��ȡ����3����
		if (index==-1) return 0;
		out.tOutPisa[0].line=4;
		out.tOutPisa[0].row=3;
		out.nCount=1;
	}
	SendData(MSG_SET_OUTPISA,&out,sizeof(struct tagSetOutPisa));
	return 0;
}
