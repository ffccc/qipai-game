#include "StdAfx.h"
#include "Resource.h"
#include "GameClientView.h"


//////////////////////////////////////////////////////////////////////////
//��ť��ʶ 

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameView)
	ON_WM_TIMER()
	ON_WM_CREATE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientView::CGameClientView(void) : CGameFrameView(true,24)
{
	//��Ϸ����


	//���ñ���
	m_bDeasilOrder=true;

	//״̬����


	//λ����Ϣ


	//������Դ

	//����λ��

	return;
}

//��������
CGameClientView::~CGameClientView(void)
{
}

//������Ϣ
int CGameClientView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{	
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;


	//�����ؼ�
	CRect CreateRect(0,0,0,0);
	m_ScoreView.Create(IDD_GAME_SCORE,this);


	//��ȡ����
	m_bDeasilOrder=AfxGetApp()->GetProfileInt(TEXT("GameOption"),TEXT("DeasilOrder"),FALSE)?true:false;

	return 0;
}


//���ý���
void CGameClientView::ResetGameView()
{
	//��Ϸ����


	//״̬����

	//���ؿؼ�
	m_ScoreView.ShowWindow(SW_HIDE);

	//���ذ�ť

	//���ÿؼ�

	//�˿˿ؼ�

	return;
}

//�����ؼ�
void CGameClientView::RectifyGameView(int nWidth, int nHeight)
{

	//������С
	if(m_DxDialog.GetSafeHwnd()!=NULL)
	{
		//m_DxDialog.MoveWindow(0,0,nWidth,nHeight);
		m_DxDialog.CenterWindow(this);
		//m_DxDialog.UpdateDxWindow(815,706);
	}

	//����dx����
	if(m_DxDialog.GetSafeHwnd()==NULL)
	{
		/*if(nWidth<813||nHeight<704) 
		{
			AfxMessageBox("���ķֱ��ʹ��ͣ�����֧�ִ���Ϸ���ڣ����򼴽��˳���");
			AfxGetMainWnd()->SendMessage(WM_CLOSE);
			return;
		}*/
		//m_DxDialog.Create(AfxRegisterWndClass(0,NULL,NULL,NULL),NULL,WS_CHILD|WS_VISIBLE,CRect(0,0,nWidth,nHeight ),this,0);
		m_DxDialog.Create(AfxRegisterWndClass(0,NULL,NULL,NULL),NULL,WS_CHILD|WS_VISIBLE,CRect(0,0,813,709 ),this,0);
		m_DxDialog.CenterWindow(this);
		SetWindowLong(m_DxDialog.GetSafeHwnd(),GWL_STYLE,GetStyle()|WS_CLIPCHILDREN);
		bool bSuccess=m_DxDialog.InitDxWindow(813,709);
		if(bSuccess==false) 
		{
			AfxMessageBox("D3D�豸��ʼ��ʧ�ܣ����򼴽��˳���");
			AfxGetMainWnd()->SendMessage(WM_CLOSE);
			return;
		}
	}

	return;
}

//�滭����
void CGameClientView::DrawGameView(CDC * pDC, int nWidth, int nHeight)
{
	//�滭����

	return;
}

//�û�˳��
void CGameClientView::SetUserOrder(bool bDeasilOrder)
{
	//״̬�ж�
	if (m_bDeasilOrder==bDeasilOrder) return;

	//���ñ���
	m_bDeasilOrder=bDeasilOrder;
	AfxGetApp()->WriteProfileInt(TEXT("GameOption"),TEXT("DeasilOrder"),m_bDeasilOrder?TRUE:FALSE);

	//���ý���
	CRect rcClient;
	GetClientRect(&rcClient);
	RectifyGameView(rcClient.Width(),rcClient.Height());

	//ˢ�½���
	UpdateGameView(NULL);

	return;
}



//��ʼ��ť
void CGameClientView::OnStart()
{
	AfxGetMainWnd()->SendMessage(IDM_START,0,0);
	return;
}

//���ý���
void CGameClientView::SetUserCome(BYTE bHistoryInfo[],long lMinBetScore,long lMaxBetWinScore,long lMaxUserScore,bool bLookOnUser)
{
	m_DxDialog.m_btChipIn.ShowWindow(SW_SHOW);
	m_DxDialog.m_bAllowBet=true;
	m_DxDialog.m_lMinBetScore=lMinBetScore;
	m_DxDialog.m_lMaxBetWin=lMaxBetWinScore;
	m_DxDialog.m_lMaxUserScore=lMaxUserScore;
	m_DxDialog.ClearAllMoney();
	m_DxDialog.SetHistoryNumber(bHistoryInfo);

	//�����Թ��û�
	if(bLookOnUser) 
	{
		m_DxDialog.m_bAllowBet=false;
		m_DxDialog.m_btChipIn.ShowWindow(SW_HIDE);
	}

	//ˢ�½���
	UpdateGameView(NULL);
}

//��ʼ��ת
void CGameClientView::SetGameStart(BYTE bTargetNumber)
{
	m_DxDialog.OnGameStart(bTargetNumber);
}
//������ע
void CGameClientView::SetUserChip(long lAllChip[256])
{
	m_DxDialog.OnUserChip(lAllChip);
}
//������ת
void CGameClientView::SetRollStop(BYTE bWinNumber)
{
	m_DxDialog.OnRollStop(bWinNumber);
}
//��ʾ����
void CGameClientView::SetGameEnd(long lScore)
{
	m_DxDialog.m_lGameEndScore=lScore;
	if(lScore>=0)
	{
		m_DxDialog.m_bShowEnd=1;
	}
	if(lScore<0)
	{
		m_DxDialog.m_bShowEnd=2;
	}
	m_DxDialog.OnGameEnd();
	SetTimer(100,3000,NULL);
}

//���ñ���
void CGameClientView::SetCellScore(long lCellScore)
{
	m_DxDialog.OnSetCellScore(lCellScore);
}

//��ʱ��Ϣ
void CGameClientView::OnTimer(UINT_PTR nIDEvent)
{
	switch(nIDEvent)
	{
	case 100:
		{
			m_DxDialog.m_bShowEnd=0;
			m_DxDialog.m_bDrawMoveChips=false;
			m_DxDialog.m_iMoveChipsCount=0;
			KillTimer(100);
			break;
		}
	default:
		break;
	}
	return;
}